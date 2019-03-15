#pragma once
#include <map>
#include <memory>
#include <string>

//wabt includes
#include <src/binary-reader.h>
#include <src/common.h>
#include <src/interp/interp.h>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/facilities/overload.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
// #include <boost/preprocessor/if.hpp>
// #include <boost/preprocessor/control/iif.hpp>
#include "basic-types.h"

namespace wabt {

// using namespace wabt;
// using namespace wabt::interp;
using namespace std;

struct wabt_apply_instance_vars {
   interp::Memory* memory;

   char* get_validated_pointer(uint32_t offset, uint32_t size) {
      // ULTRAIN_ASSERT(memory, wasm_execution_error, "access violation");
      // ULTRAIN_ASSERT(offset + size <= memory->data.size() && offset + size >= offset, wasm_execution_error, "access violation");
      if (!memory) return nullptr;
      if (offset + size > memory->data.size() || offset + size < offset) return nullptr;
      return memory->data.data() + offset;
   }
};

struct intrinsic_registrator {
   using intrinsic_fn = interp::TypedValue(*)(wabt_apply_instance_vars&, const interp::TypedValues&);

   struct intrinsic_func_info {
      interp::FuncSignature sig;
      intrinsic_fn func;
   };

   static map<string, map<string, intrinsic_func_info> >& get_map() {
      static map<string, map<string, intrinsic_func_info> > _map;
      return _map;
   };

   intrinsic_registrator(const char* mod, const char* name, const interp::FuncSignature& sig, intrinsic_fn fn) {
      get_map()[string(mod)][string(name)] = intrinsic_func_info{sig, fn};
   }
};

/**
 * class to represent an in-wasm-memory array
 * it is a hint to the transcriber that the next parameter will
 * be a size (data bytes length) and that the pair are validated together
 * This triggers the template specialization of intrinsic_invoker_impl
 * @tparam T
 */
template<typename T>
struct array_ptr {
   explicit array_ptr (T * value) : value(value) {}

   typename std::add_lvalue_reference<T>::type operator*() const {
      return *value;
   }

   T *operator->() const noexcept {
      return value;
   }

   template<typename U>
   operator U *() const {
      return static_cast<U *>(value);
   }

   T *value;
};

/**
 * class to represent an in-wasm-memory array
 * it is a hint to the transcriber that the next parameter will
 * be a size (data bytes length) and that the pair are validated together
 * This triggers the template specialization of intrinsic_invoker_impl
 * @tparam T
 */
template<typename T>
inline array_ptr<T> array_ptr_impl (wabt_apply_instance_vars& vars, uint32_t ptr, uint32_t length)
{
   // ULTRAIN_ASSERT( length < INT_MAX/(uint32_t)sizeof(T), binaryen_exception, "length will overflow" );
   return array_ptr<T>((T*)(vars.get_validated_pointer(ptr, length * (uint32_t)sizeof(T))));
}

/**
 * class to represent an in-wasm-memory char array that must be null terminated
 */
struct null_terminated_ptr {
explicit null_terminated_ptr(char* value) : value(value) {}

   typename std::add_lvalue_reference<char>::type operator*() const {
      return *value;
   }

   char *operator->() const noexcept {
      return value;
   }

   template<typename U>
   operator U *() const {
      return static_cast<U *>(value);
   }

   char *value;
};

/**
 * class to represent an in-wasm-memory char array that must be null terminated
 */
inline null_terminated_ptr null_terminated_ptr_impl(wabt_apply_instance_vars& vars, uint32_t ptr)
{
   char *value = vars.get_validated_pointer(ptr, 1);
   const char* p = value;
   const char* const top_of_memory = vars.memory->data.data() + vars.memory->data.size();
   while(p < top_of_memory)
      if(*p++ == '\0')
         return null_terminated_ptr(value);

   // FC_THROW_EXCEPTION(wasm_execution_error, "unterminated string");
   return null_terminated_ptr(nullptr);
}


template<typename T>
struct is_reference_from_value {
   static constexpr bool value = false;
};

template<typename T>
constexpr bool is_reference_from_value_v = is_reference_from_value<T>::value;

template<typename T>
T convert_literal_to_native(const interp::TypedValue& v);

template<>
inline array_ptr<char> convert_literal_to_native<array_ptr<char>>(const interp::TypedValue& v) {
   char not_implemented[] = "not implemented";
   return array_ptr<char>(not_implemented);
}

template<>
inline double convert_literal_to_native<double>(const interp::TypedValue& v) {
   return v.get_f64();
}

template<>
inline float convert_literal_to_native<float>(const interp::TypedValue& v) {
   return v.get_f32();
}

template<>
inline int64_t convert_literal_to_native<int64_t>(const interp::TypedValue& v) {
   return v.get_i64();
}

template<>
inline uint64_t convert_literal_to_native<uint64_t>(const interp::TypedValue& v) {
   return v.get_i64();
}

template<>
inline int32_t convert_literal_to_native<int32_t>(const interp::TypedValue& v) {
   return v.get_i32();
}

template<>
inline uint32_t convert_literal_to_native<uint32_t>(const interp::TypedValue& v) {
   return v.get_i32();
}

template<>
inline bool convert_literal_to_native<bool>(const interp::TypedValue& v) {
   return v.get_i32();
}

inline auto convert_native_to_literal(const wabt_apply_instance_vars&, const uint32_t &val) {
   interp::TypedValue tv(Type::I32);
   tv.set_i32(val);
   return tv;
}

inline auto convert_native_to_literal(const wabt_apply_instance_vars&, const int32_t &val) {
   interp::TypedValue tv(Type::I32);
   tv.set_i32(val);
   return tv;
}

inline auto convert_native_to_literal(const wabt_apply_instance_vars&, const uint64_t &val) {
   interp::TypedValue tv(Type::I64);
   tv.set_i64(val);
   return tv;
}

inline auto convert_native_to_literal(const wabt_apply_instance_vars&, const int64_t &val) {
   interp::TypedValue tv(Type::I64);
   tv.set_i64(val);
   return tv;
}

inline auto convert_native_to_literal(const wabt_apply_instance_vars&, const float &val) {
   interp::TypedValue tv(Type::F32);
   tv.set_f32(val);
   return tv;
}

inline auto convert_native_to_literal(const wabt_apply_instance_vars&, const double &val) {
   interp::TypedValue tv(Type::F64);
   tv.set_f64(val);
   return tv;
}

inline auto convert_native_to_literal(const wabt_apply_instance_vars& vars, char* ptr) {
   const char* base = vars.memory->data.data();
   const char* top_of_memory = base + vars.memory->data.size();
   interp::Value v;
   v.i32 = (int) 0;
   if (ptr < base || ptr >= top_of_memory) return interp::TypedValue(Type::I32, v);

   v.i32 = (int)(ptr - base);
   return interp::TypedValue(Type::I32, v);
}

struct void_type {
};

template<typename T>
struct wabt_to_value_type;

template<>
struct wabt_to_value_type<F32> {
   static constexpr auto value = Type::F32;
};

template<>
struct wabt_to_value_type<F64> {
   static constexpr auto value = Type::F64;
};
template<>
struct wabt_to_value_type<I32> {
   static constexpr auto value = Type::I32;
};
template<>
struct wabt_to_value_type<I64> {
   static constexpr auto value = Type::I64;
};

template<typename T>
constexpr auto wabt_to_value_type_v = wabt_to_value_type<T>::value;

template<typename T>
struct wabt_to_rvalue_type;
template<>
struct wabt_to_rvalue_type<F32> {
   static constexpr auto value = Type::F32;
};
template<>
struct wabt_to_rvalue_type<F64> {
   static constexpr auto value = Type::F64;
};
template<>
struct wabt_to_rvalue_type<I32> {
   static constexpr auto value = Type::I32;
};
template<>
struct wabt_to_rvalue_type<I64> {
   static constexpr auto value = Type::I64;
};

template<>
struct wabt_to_rvalue_type<char*> {
   static constexpr auto value = Type::I32;
};

template<typename T>
constexpr auto wabt_to_rvalue_type_v = wabt_to_rvalue_type<T>::value;

template<typename>
struct wabt_function_type_provider;

template<typename Ret, typename ...Args>
struct wabt_function_type_provider<Ret(Args...)> {
   static interp::FuncSignature type() {
      return interp::FuncSignature({wabt_to_value_type_v<Args> ...}, {wabt_to_rvalue_type_v<Ret>});
   }
};
template<typename ...Args>
struct wabt_function_type_provider<void(Args...)> {
   static interp::FuncSignature type() {
      return interp::FuncSignature({wabt_to_value_type_v<Args> ...}, {});
   }
};

/**
 * Forward declaration of the invoker type which transcribes arguments to/from a native method
 * and injects the appropriate checks
 *
 * @tparam Ret - the return type of the native function
 * @tparam NativeParameters - a std::tuple of the remaining native parameters to transcribe
 * @tparam WasmParameters - a std::tuple of the transribed parameters
 */
template<typename Ret, typename NativeParameters>
struct intrinsic_invoker_impl;

/**
 * Specialization for the fully transcribed signature
 * @tparam Ret - the return type of the native function
 */
template<typename Ret>
struct intrinsic_invoker_impl<Ret, std::tuple<>> {
   using next_method_type        = Ret (*)(wabt_apply_instance_vars&, const interp::TypedValues&, int);

   template<next_method_type Method>
   static interp::TypedValue invoke(wabt_apply_instance_vars& vars, const interp::TypedValues& args) {
      return convert_native_to_literal(vars, Method(vars, args, args.size() - 1));
   }

   template<next_method_type Method>
   static auto fn() {
      return invoke<Method>;
   }
};

/**
 * specialization of the fully transcribed signature for void return values
 * @tparam Translated - the arguments to the wasm function
 */
template<>
struct intrinsic_invoker_impl<void_type, std::tuple<>> {
   using next_method_type        = void_type (*)(wabt_apply_instance_vars&, const interp::TypedValues&, int);

   template<next_method_type Method>
   static interp::TypedValue invoke(wabt_apply_instance_vars& vars, const interp::TypedValues& args) {
      Method(vars, args, args.size() - 1);
      return interp::TypedValue(Type::Void);
   }

   template<next_method_type Method>
   static auto fn() {
      return invoke<Method>;
   }
};

/**
 * Sepcialization for transcribing  a simple type in the native method signature
 * @tparam Ret - the return type of the native method
 * @tparam Input - the type of the native parameter to transcribe
 * @tparam Inputs - the remaining native parameters to transcribe
 */
template<typename Ret, typename Input, typename... Inputs>
struct intrinsic_invoker_impl<Ret, std::tuple<Input, Inputs...>> {
   using next_step = intrinsic_invoker_impl<Ret, std::tuple<Inputs...>>;
   using then_type = Ret (*)(wabt_apply_instance_vars&, Input, Inputs..., const interp::TypedValues&, int);

   template<then_type Then>
   static Ret translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) {
      auto& last = args.at(offset);
      auto native = convert_literal_to_native<Input>(last);
      return Then(vars, native, rest..., args, (uint32_t)offset - 1);
   };

   template<then_type Then>
   static auto fn() {
      return next_step::template fn<translate_one<Then>>();
   }
};

/**
 * Specialization for transcribing  a array_ptr type in the native method signature
 * This type transcribes into 2 wasm parameters: a pointer and byte length and checks the validity of that memory
 * range before dispatching to the native method
 *
 * @tparam Ret - the return type of the native method
 * @tparam Inputs - the remaining native parameters to transcribe
 */
template<typename T, typename Ret, typename... Inputs>
struct intrinsic_invoker_impl<Ret, std::tuple<array_ptr<T>, size_t, Inputs...>> {
   using next_step = intrinsic_invoker_impl<Ret, std::tuple<Inputs...>>;
   using then_type = Ret(*)(wabt_apply_instance_vars&, array_ptr<T>, size_t, Inputs..., const interp::TypedValues&, int);

   template<then_type Then, typename U=T>
   static auto translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) -> std::enable_if_t<std::is_const<U>::value, Ret> {
      static_assert(!std::is_pointer<U>::value, "Currently don't support array of pointers");
      uint32_t ptr = args.at((uint32_t)offset - 1).get_i32();
      size_t length = args.at((uint32_t)offset).get_i32();
      T* base = array_ptr_impl<T>(vars, ptr, length);
      if ( reinterpret_cast<uintptr_t>(base) % alignof(T) != 0 ) {
         std::vector<std::remove_const<T> > copy(length > 0 ? length : 1);
         T* copy_ptr = &copy[0];
         memcpy( (void*)copy_ptr, (void*)base, length * sizeof(T) );
         return Then(vars, static_cast<array_ptr<T>>(copy_ptr), length, rest..., args, (uint32_t)offset - 2);
      }
      return Then(vars, static_cast<array_ptr<T>>(base), length, rest..., args, (uint32_t)offset - 2);
   };

   template<then_type Then, typename U=T>
   static auto translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) -> std::enable_if_t<!std::is_const<U>::value, Ret> {
      static_assert(!std::is_pointer<U>::value, "Currently don't support array of pointers");
      uint32_t ptr = args.at((uint32_t)offset - 1).get_i32();
      size_t length = args.at((uint32_t)offset).get_i32();
      T* base = array_ptr_impl<T>(vars, ptr, length);
      if ( reinterpret_cast<uintptr_t>(base) % alignof(T) != 0 ) {
         std::vector<std::remove_const_t<T> > copy(length > 0 ? length : 1);
         T* copy_ptr = &copy[0];
         memcpy( (void*)copy_ptr, (void*)base, length * sizeof(T) );
         Ret ret = Then(vars, static_cast<array_ptr<T>>(copy_ptr), length, rest..., args, (uint32_t)offset - 2);
         memcpy( (void*)base, (void*)copy_ptr, length * sizeof(T) );
         return ret;
      }
      return Then(vars, static_cast<array_ptr<T>>(base), length, rest..., args, (uint32_t)offset - 2);
   };

   template<then_type Then>
   static auto fn() {
      return next_step::template fn<translate_one<Then>>();
   }
};

/**
 * Specialization for transcribing  a null_terminated_ptr type in the native method signature
 * This type transcribes 1 wasm parameters: a char pointer which is validated to contain
 * a null value before the end of the allocated memory.
 *
 * @tparam Ret - the return type of the native method
 * @tparam Inputs - the remaining native parameters to transcribe
 */
template<typename Ret, typename... Inputs>
struct intrinsic_invoker_impl<Ret, std::tuple<null_terminated_ptr, Inputs...>> {
   using next_step = intrinsic_invoker_impl<Ret, std::tuple<Inputs...>>;
   using then_type = Ret(*)(wabt_apply_instance_vars&, null_terminated_ptr, Inputs..., const interp::TypedValues&, int);

   template<then_type Then>
   static Ret translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) {
      uint32_t ptr = args.at((uint32_t)offset).get_i32();
      return Then(vars, null_terminated_ptr_impl(vars, ptr), rest..., args, (uint32_t)offset - 1);
   };

   template<then_type Then>
   static auto fn() {
      return next_step::template fn<translate_one<Then>>();
   }
};

/**
 * Specialization for transcribing  a pair of array_ptr types in the native method signature that share size
 * This type transcribes into 3 wasm parameters: 2 pointers and byte length and checks the validity of those memory
 * ranges before dispatching to the native method
 *
 * @tparam Ret - the return type of the native method
 * @tparam Inputs - the remaining native parameters to transcribe
 */
template<typename T, typename U, typename Ret, typename... Inputs>
struct intrinsic_invoker_impl<Ret, std::tuple<array_ptr<T>, array_ptr<U>, size_t, Inputs...>> {
   using next_step = intrinsic_invoker_impl<Ret, std::tuple<Inputs...>>;
   using then_type = Ret(*)(wabt_apply_instance_vars&, array_ptr<T>, array_ptr<U>, size_t, Inputs..., const interp::TypedValues&, int);

   template<then_type Then>
   static Ret translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) {
      uint32_t ptr_t = args.at((uint32_t)offset - 2).get_i32();
      uint32_t ptr_u = args.at((uint32_t)offset - 1).get_i32();
      size_t length = args.at((uint32_t)offset).get_i32();
      static_assert(std::is_same<std::remove_const_t<T>, char>::value && std::is_same<std::remove_const_t<U>, char>::value, "Currently only support array of (const)chars");
      return Then(vars, array_ptr_impl<T>(vars, ptr_t, length), array_ptr_impl<U>(vars, ptr_u, length), length, args, (uint32_t)offset - 3);
   };

   template<then_type Then>
   static auto fn() {
      return next_step::template fn<translate_one<Then>>();
   }
};

/**
 * Specialization for transcribing memset parameters
 *
 * @tparam Ret - the return type of the native method
 * @tparam Inputs - the remaining native parameters to transcribe
 */
template<typename Ret>
struct intrinsic_invoker_impl<Ret, std::tuple<array_ptr<char>, int, size_t>> {
   using next_step = intrinsic_invoker_impl<Ret, std::tuple<>>;
   using then_type = Ret(*)(wabt_apply_instance_vars&, array_ptr<char>, int, size_t, const interp::TypedValues&, int);

   template<then_type Then>
   static Ret translate_one(wabt_apply_instance_vars& vars, const interp::TypedValues& args, int offset) {
      uint32_t ptr = args.at((uint32_t)offset - 2).get_i32();
      uint32_t value = args.at((uint32_t)offset - 1).get_i32();
      size_t length = args.at((uint32_t)offset).get_i32();
      return Then(vars, array_ptr_impl<char>(vars, ptr, length), value, length, args, (uint32_t)offset - 3);
   };

   template<then_type Then>
   static auto fn() {
      return next_step::template fn<translate_one<Then>>();
   }
};

/**
 * Specialization for transcribing  a pointer type in the native method signature
 * This type transcribes into an int32  pointer checks the validity of that memory
 * range before dispatching to the native method
 *
 * @tparam Ret - the return type of the native method
 * @tparam Inputs - the remaining native parameters to transcribe
 */
template<typename T, typename Ret, typename... Inputs>
struct intrinsic_invoker_impl<Ret, std::tuple<T *, Inputs...>> {
   using next_step = intrinsic_invoker_impl<Ret, std::tuple<Inputs...>>;
   using then_type = Ret (*)(wabt_apply_instance_vars&, T *, Inputs..., const interp::TypedValues&, int);

   template<then_type Then, typename U=T>
   static auto translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) -> std::enable_if_t<std::is_const<U>::value, Ret> {
      uint32_t ptr = args.at((uint32_t)offset).get_i32();
      T* base = array_ptr_impl<T>(vars, ptr, 1);
      if ( reinterpret_cast<uintptr_t>(base) % alignof(T) != 0 ) {
         std::remove_const<T> copy;
         T* copy_ptr = &copy;
         memcpy( (void*)copy_ptr, (void*)base, sizeof(T) );
         return Then(vars, copy_ptr, rest..., args, (uint32_t)offset - 1);
      }
      return Then(vars, base, rest..., args, (uint32_t)offset - 1);
   };

   template<then_type Then, typename U=T>
   static auto translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) -> std::enable_if_t<!std::is_const<U>::value, Ret> {
      uint32_t ptr = args.at((uint32_t)offset).get_i32();
      T* base = array_ptr_impl<T>(vars, ptr, 1);
      if ( reinterpret_cast<uintptr_t>(base) % alignof(T) != 0 ) {
         T copy;
         memcpy( (void*)&copy, (void*)base, sizeof(T) );
         Ret ret = Then(vars, &copy, rest..., args, (uint32_t)offset - 1);
         memcpy( (void*)base, (void*)&copy, sizeof(T) );
         return ret;
      }
      return Then(vars, base, rest..., args, (uint32_t)offset - 1);
   };

   template<then_type Then>
   static auto fn() {
      return next_step::template fn<translate_one<Then>>();
   }
};

/**
 * Specialization for transcribing  a reference type in the native method signature
 *    This type transcribes into an int32  pointer checks the validity of that memory
 *    range before dispatching to the native method
 *
 * @tparam Ret - the return type of the native method
 * @tparam Inputs - the remaining native parameters to transcribe
 */
template<typename T, typename Ret, typename... Inputs>
struct intrinsic_invoker_impl<Ret, std::tuple<T &, Inputs...>> {
   using next_step = intrinsic_invoker_impl<Ret, std::tuple<Inputs...>>;
   using then_type = Ret (*)(wabt_apply_instance_vars&, T &, Inputs..., const interp::TypedValues&, int);

   template<then_type Then, typename U=T>
   static auto translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) -> std::enable_if_t<std::is_const<U>::value, Ret> {
      // references cannot be created for null pointers
      uint32_t ptr = args.at((uint32_t)offset).get_i32();
      // ULTRAIN_ASSERT(ptr != 0, binaryen_exception, "references cannot be created for null pointers");
      T* base = array_ptr_impl<T>(vars, ptr, 1);
      if ( reinterpret_cast<uintptr_t>(base) % alignof(T) != 0 ) {
         std::remove_const<T> copy;
         T* copy_ptr = &copy;
         memcpy( (void*)copy_ptr, (void*)base, sizeof(T) );
         return Then(vars, *copy_ptr, rest..., args, (uint32_t)offset - 1);
      }
      return Then(vars, *base, rest..., args, (uint32_t)offset - 1);
   }

   template<then_type Then, typename U=T>
   static auto translate_one(wabt_apply_instance_vars& vars, Inputs... rest, const interp::TypedValues& args, int offset) -> std::enable_if_t<!std::is_const<U>::value, Ret> {
      // references cannot be created for null pointers
      uint32_t ptr = args.at((uint32_t)offset).get_i32();
      // ULTRAIN_ASSERT(ptr != 0, binaryen_exception, "references cannot be created for null pointers");
      T* base = array_ptr_impl<T>(vars, ptr, 1);
      if ( reinterpret_cast<uintptr_t>(base) % alignof(T) != 0 ) {
         T copy;
         memcpy( (void*)&copy, (void*)base, sizeof(T) );
         Ret ret = Then(vars, copy, rest..., args, (uint32_t)offset - 1);
         memcpy( (void*)base, (void*)&copy, sizeof(T) );
         return ret;
      }
      return Then(vars, *base, rest..., args, (uint32_t)offset - 1);
   }


   template<then_type Then>
   static auto fn() {
      return next_step::template fn<translate_one<Then>>();
   }
};

/**
 * forward declaration of a wrapper class to call methods of the class
 */
template<typename Ret, typename MethodSig, typename Cls, typename... Params>
struct intrinsic_function_invoker {
   using impl = intrinsic_invoker_impl<Ret, std::tuple<Params...>>;

   template<MethodSig Method>
   static Ret wrapper(wabt_apply_instance_vars& vars, Params... params, const interp::TypedValues&, int) {
      return (Cls().*Method)(params...);
   }

   template<MethodSig Method>
   static intrinsic_registrator::intrinsic_fn fn() {
      return impl::template fn<wrapper<Method>>();
   }
};

template<typename MethodSig, typename Cls, typename... Params>
struct intrinsic_function_invoker<void, MethodSig, Cls, Params...> {
   using impl = intrinsic_invoker_impl<void_type, std::tuple<Params...>>;

   template<MethodSig Method>
   static void_type wrapper(wabt_apply_instance_vars& vars, Params... params, const interp::TypedValues& args, int offset) {
      (Cls().*Method)(params...);
      return void_type();
   }

   template<MethodSig Method>
   static intrinsic_registrator::intrinsic_fn fn() {
      return impl::template fn<wrapper<Method>>();
   }

};

template<typename>
struct intrinsic_function_invoker_wrapper;

template<typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper<Ret (Cls::*)(Params...)> {
   using type = intrinsic_function_invoker<Ret, Ret (Cls::*)(Params...), Cls, Params...>;
};

template<typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper<Ret (Cls::*)(Params...) const> {
   using type = intrinsic_function_invoker<Ret, Ret (Cls::*)(Params...) const, Cls, Params...>;
};

template<typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper<Ret (Cls::*)(Params...) volatile> {
   using type = intrinsic_function_invoker<Ret, Ret (Cls::*)(Params...) volatile, Cls, Params...>;
};

template<typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper<Ret (Cls::*)(Params...) const volatile> {
   using type = intrinsic_function_invoker<Ret, Ret (Cls::*)(Params...) const volatile, Cls, Params...>;
};

#define __INTRINSIC_NAME(LABEL, SUFFIX) LABEL##SUFFIX
#define _INTRINSIC_NAME(LABEL, SUFFIX) __INTRINSIC_NAME(LABEL,SUFFIX)

#define _REGISTER_WABT_INTRINSIC(CLS, MOD, METHOD, WASM_SIG, NAME, SIG)\
   static wabt::intrinsic_registrator _INTRINSIC_NAME(__wabt_intrinsic_fn, __COUNTER__) (\
      MOD,\
      NAME,\
      wabt::wabt_function_type_provider<WASM_SIG>::type(),\
      wabt::intrinsic_function_invoker_wrapper<SIG>::type::fn<&CLS::METHOD>()\
   );

#define ULTRAINIO_INJECTED_MODULE_NAME "ultrainio_injection"

#define _REGISTER_INTRINSIC_EXPLICIT(CLS, MOD, METHOD, WASM_SIG, NAME, SIG) \
   _REGISTER_WABT_INTRINSIC(CLS, MOD, METHOD, WASM_SIG, NAME, SIG)

#define _REGISTER_INTRINSIC4(CLS, MOD, METHOD, WASM_SIG, NAME, SIG)\
   _REGISTER_INTRINSIC_EXPLICIT(CLS, MOD, METHOD, WASM_SIG, NAME, SIG )

#define _REGISTER_INTRINSIC3(CLS, MOD, METHOD, WASM_SIG, NAME)\
   _REGISTER_INTRINSIC_EXPLICIT(CLS, MOD, METHOD, WASM_SIG, NAME, decltype(&CLS::METHOD) )

#define _REGISTER_INTRINSIC2(CLS, MOD, METHOD, WASM_SIG)\
   _REGISTER_INTRINSIC_EXPLICIT(CLS, MOD, METHOD, WASM_SIG, BOOST_PP_STRINGIZE(METHOD), decltype(&CLS::METHOD) )

#define _REGISTER_INTRINSIC1(CLS, MOD, METHOD)\
   static_assert(false, "Cannot register " BOOST_PP_STRINGIZE(CLS) ":" BOOST_PP_STRINGIZE(METHOD) " without a signature");

#define _REGISTER_INTRINSIC0(CLS, MOD, METHOD)\
   static_assert(false, "Cannot register " BOOST_PP_STRINGIZE(CLS) ":<unknown> without a method name and signature");

#define _ADD_PAREN_1(...) ((__VA_ARGS__)) _ADD_PAREN_2
#define _ADD_PAREN_2(...) ((__VA_ARGS__)) _ADD_PAREN_1
#define _ADD_PAREN_1_END
#define _ADD_PAREN_2_END
#define _WRAPPED_SEQ(SEQ) BOOST_PP_CAT(_ADD_PAREN_1 SEQ, _END)

#define _UNWRAP_SEQ(...) __VA_ARGS__

#define _EXPAND_ARGS(CLS, MOD, INFO)\
   ( CLS, MOD, _UNWRAP_SEQ INFO )

#define _REGISTER_INTRINSIC(R, CLS, INFO)\
   BOOST_PP_CAT(BOOST_PP_OVERLOAD(_REGISTER_INTRINSIC, _UNWRAP_SEQ INFO) _EXPAND_ARGS(CLS, "env", INFO), BOOST_PP_EMPTY())

#define REGISTER_INTRINSICS(CLS, MEMBERS)\
   BOOST_PP_SEQ_FOR_EACH(_REGISTER_INTRINSIC, CLS, _WRAPPED_SEQ(MEMBERS))

#define _REGISTER_INJECTED_INTRINSIC(R, CLS, INFO)\
   BOOST_PP_CAT(BOOST_PP_OVERLOAD(_REGISTER_INTRINSIC, _UNWRAP_SEQ INFO) _EXPAND_ARGS(CLS, ULTRAINIO_INJECTED_MODULE_NAME, INFO), BOOST_PP_EMPTY())

#define REGISTER_INJECTED_INTRINSICS(CLS, MEMBERS)\
   BOOST_PP_SEQ_FOR_EACH(_REGISTER_INJECTED_INTRINSIC, CLS, _WRAPPED_SEQ(MEMBERS))

} // end of namespace wabt
