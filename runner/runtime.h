#pragma once

#include <map>
#include <memory>
#include <string>

//wabt includes
#include <src/binary-reader.h>
#include <src/common.h>
#include <src/interp/interp.h>
#include "basic-types.h"

namespace wabt {

using namespace std;

struct wasm_exit {
      int32_t code = 0;
};

enum class RunVerbosity {
  Quiet = 0,
  Verbose = 1,
};


class wabt_runtime {
   public:
      wabt_runtime(const char* wasm_file_name);

      bool init();
      void run(const char* fnname,
               const interp::TypedValues& args,
               RunVerbosity verbose);

   private:
      void register_intrinsics();
      wabt::Result read_module(const char* module_filename,
                               interp::Environment* env,
                               Errors* errors,
                               interp::DefinedModule** out_module);
      bool parse_initial_memory();
   private:
      std::string m_wasm_file;

      interp::Environment m_env;
      interp::DefinedModule* p_module;

      interp::Thread::Options s_thread_options;
      Stream* s_trace_stream;
      Features s_features;

      std::unique_ptr<FileStream> s_log_stream;
      std::vector<uint8_t>       s_initial_memory;
};

} // end of namespace wabt
