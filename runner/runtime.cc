#include "runtime.h"
#include "intrinsic-register.h"

//wabt includes
#include <src/interp/binary-reader-interp.h>
#include <src/error-formatter.h>
#include <src/interp/interp.h>

#include <src/feature.h>

namespace wabt {

static wabt_apply_instance_vars* static_wabt_vars;

wabt::Result wabt_runtime::read_module(const char* module_filename,
                               interp::Environment* env,
                               Errors* errors,
                               interp::DefinedModule** out_module) {
  wabt::Result result;
  std::vector<uint8_t> file_data;

  *out_module = nullptr;

  result = ReadFile(module_filename, &file_data);
  if (Succeeded(result)) {
    const bool kReadDebugNames = true;
    const bool kStopOnFirstError = true;
    const bool kFailOnCustomSectionError = true;
    ReadBinaryOptions options(s_features, s_log_stream.get(), kReadDebugNames,
                              kStopOnFirstError, kFailOnCustomSectionError);
    result = ReadBinaryInterp(env, file_data.data(), file_data.size(), options,
                              errors, out_module);
  }
  return result;
}

static std::unique_ptr<FileStream> s_stdout_stream = FileStream::CreateStdout();

wabt_runtime::wabt_runtime(const char* wasm_file_name) : m_wasm_file(wasm_file_name) {
   p_module = nullptr;
   s_thread_options.value_stack_size = 10240;
   s_thread_options.call_stack_size = 10240;
}

void wabt_runtime::register_intrinsics() {
   for(auto it = intrinsic_registrator::get_map().begin() ; it != intrinsic_registrator::get_map().end(); ++it) {
      interp::HostModule* host_module = m_env.AppendHostModule(it->first);
      for(auto itf = it->second.begin(); itf != it->second.end(); ++itf) {
         // printf("intrinsic method: %s.%s\n", it->first.c_str(), itf->first.c_str());
         host_module->AppendFuncExport(itf->first, itf->second.sig, [&](const interp::HostFunc* f, const interp::FuncSignature* fs, const interp::TypedValues& args, interp::TypedValues& res) {
            auto fn = itf->second.func;
            interp::TypedValue ret = fn(*static_wabt_vars, args);
            if(ret.type != Type::Void)
               res[0] = ret;
            return interp::Result::Ok;
         });
      }
   }
}

bool wabt_runtime::init() {
   register_intrinsics();

   Errors errors;
   wabt::Result result = read_module(m_wasm_file.c_str(), &m_env, &errors, &p_module);
   FormatErrorsToFile(errors, Location::Type::Binary);
   if (Succeeded(result)) {
     interp::Executor executor(&m_env, s_trace_stream, s_thread_options);
     interp::ExecResult exec_result = executor.RunStartFunction(p_module);
     if (exec_result.result != interp::Result::Ok) { return false; }
   }
   return Succeeded(result);
}

bool wabt_runtime::parse_initial_memory() {
   s_initial_memory.resize(4*1024*1024);
   return true;
}

void wabt_runtime::run(const char* fnname,
                       const interp::TypedValues& args,
                       RunVerbosity verbose) {
      wabt_apply_instance_vars this_run_vars{nullptr};
      static_wabt_vars = &this_run_vars;

      //reset memory to inital size & copy back in initial data
      if(m_env.GetMemoryCount()) {
         auto initial_memory_configuration = m_env.GetMemory(0)->page_limits;
         interp::Memory* memory = this_run_vars.memory = m_env.GetMemory(0);
         memory->page_limits = initial_memory_configuration;
         memory->data.resize(initial_memory_configuration.initial * WABT_PAGE_SIZE);
         memset(memory->data.data(), 0, memory->data.size());
         memcpy(memory->data.data(), s_initial_memory.data(), s_initial_memory.size());
      }

    // TypedValues args;
    interp::TypedValues results;
    for (const interp::Export& export_ : p_module->exports) {
        if (export_.kind != ExternalKind::Func) continue;
      //   printf("export.name: %s\n", export_.name.c_str());
        if (export_.name != fnname) continue;

        interp::Executor executor(&m_env, s_trace_stream, s_thread_options);
        interp::ExecResult exec_result = executor.RunStartFunction(p_module);
        if (exec_result.result != interp::Result::Ok) { return; }

        exec_result = executor.RunExport(&export_, args);

        if (verbose == RunVerbosity::Verbose) {
            WriteCall(s_stdout_stream.get(), string_view(), export_.name, args,
                    exec_result.values, exec_result.result);
        }
    }
 }

} // end of namespace wabt
