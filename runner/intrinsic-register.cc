#include "intrinsic-register.h"
#include <iostream>

namespace wabt {

class context_aware_api {
   public:
      context_aware_api(bool context_free = false )
      {
      }

      void checktime() {
      }


};

class context_free_api : public context_aware_api {
   public:
      context_free_api( )
      :context_aware_api(true) {
      }

      int get_context_free_data( uint32_t index, array_ptr<char> buffer, size_t buffer_size )const {
         return 0;
      }
};

class privileged_api : public context_aware_api {
   public:
      privileged_api( )
      {
      }

      /**
       * This should return true if a feature is active and irreversible, false if not.
       *
       * Irreversiblity by fork-database is not consensus safe, therefore, this defines
       * irreversiblity only by block headers not by BFT short-cut.
       */
      int is_feature_active( int64_t feature_name ) {
         return false;
      }

      /**
       *  This should schedule the feature to be activated once the
       *  block that includes this call is irreversible. It should
       *  fail if the feature is already pending.
       *
       *  Feature name should be base32 encoded name.
       */
      void activate_feature( int64_t feature_name ) {
      }

      /**
       * update the resource limits associated with an account.  Note these new values will not take effect until the
       * next resource "tick" which is currently defined as a cycle boundary inside a block.
       *
       * @param account - the account whose limits are being modified
       * @param ram_bytes - the limit for ram bytes
       * @param net_weight - the weight for determining share of network capacity
       * @param cpu_weight - the weight for determining share of compute capacity
       */
      void set_resource_limits( uint64_t account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight) {
      }

      void get_resource_limits( uint64_t account, int64_t& ram_bytes, int64_t& net_weight, int64_t& cpu_weight ) {
      }

      int64_t set_proposed_producers( array_ptr<char> packed_producer_schedule, size_t datalen) {
         return 0;
      }

      uint32_t get_blockchain_parameters_packed( array_ptr<char> packed_blockchain_parameters, size_t buffer_size) {
         return 0;
      }

      void set_blockchain_parameters_packed( array_ptr<char> packed_blockchain_parameters, size_t datalen) {
      }

      bool is_privileged( uint64_t n ) {
         return false;
      }

      void set_privileged( uint64_t n, bool is_priv ) {
      }

};

class softfloat_api : public context_aware_api {
   public:
      // TODO add traps on truncations for special cases (NaN or outside the range which rounds to an integer)
      softfloat_api( ) : context_aware_api(true) {}

      // float binops
      float _eosio_f32_add( float a, float b ) {
         return 0.0f;
      }
      float _eosio_f32_sub( float a, float b ) {
         return 0.0f;
      }
      float _eosio_f32_div( float a, float b ) {
         return 0.0f;
      }
      float _eosio_f32_mul( float a, float b ) {
         return 0.0f;
      }
      float _eosio_f32_min( float af, float bf ) {
         return 0.0f;
      }
      float _eosio_f32_max( float af, float bf ) {
         return 0.0f;
      }
      float _eosio_f32_copysign( float af, float bf ) {
         return 0.0f;
      }
      // float unops
      float _eosio_f32_abs( float af ) {
         return 0.0f;
      }
      float _eosio_f32_neg( float af ) {
         return 0.0f;
      }
      float _eosio_f32_sqrt( float a ) {
         return 0.0f;
      }
      // ceil, floor, trunc and nearest are lifted from libc
      float _eosio_f32_ceil( float af ) {
         return 0.0f;
      }
      float _eosio_f32_floor( float af ) {
         return 0.0f;
      }
      float _eosio_f32_trunc( float af ) {
         return 0.0f;
      }
      float _eosio_f32_nearest( float af ) {
         return 0.0f;
      }

      // float relops
      bool _eosio_f32_eq( float a, float b ) {  return false; }
      bool _eosio_f32_ne( float a, float b ) { return false; }
      bool _eosio_f32_lt( float a, float b ) { return false; }
      bool _eosio_f32_le( float a, float b ) { return false; }
      bool _eosio_f32_gt( float af, float bf ) {
         return false;
      }
      bool _eosio_f32_ge( float af, float bf ) {
         return false;
      }

      // double binops
      double _eosio_f64_add( double a, double b ) {
         return 0.0f;
      }
      double _eosio_f64_sub( double a, double b ) {
         return 0.0f;
      }
      double _eosio_f64_div( double a, double b ) {
         return 0.0f;
      }
      double _eosio_f64_mul( double a, double b ) {
         return 0.0f;
      }
      double _eosio_f64_min( double af, double bf ) {
         return 0.0f;
      }
      double _eosio_f64_max( double af, double bf ) {
         return 0.0f;
      }
      double _eosio_f64_copysign( double af, double bf ) {
         return 0.0f;
      }

      // double unops
      double _eosio_f64_abs( double af ) {
         return 0.0f;
      }
      double _eosio_f64_neg( double af ) {
         return 0.0f;
      }
      double _eosio_f64_sqrt( double a ) {
         return 0.0f;
      }
      // ceil, floor, trunc and nearest are lifted from libc
      double _eosio_f64_ceil( double af ) {
         return 0.0f;
      }
      double _eosio_f64_floor( double af ) {
         return 0.0f;
      }
      double _eosio_f64_trunc( double af ) {
         return 0.0f;
      }

      double _eosio_f64_nearest( double af ) {
         return 0.0f;
      }

      // double relops
      bool _eosio_f64_eq( double a, double b ) { return false; }
      bool _eosio_f64_ne( double a, double b ) { return false; }
      bool _eosio_f64_lt( double a, double b ) { return false; }
      bool _eosio_f64_le( double a, double b ) { return false; }
      bool _eosio_f64_gt( double af, double bf ) {
         return false;
      }
      bool _eosio_f64_ge( double af, double bf ) {
         return false;
      }

      // float and double conversions
      double _eosio_f32_promote( float a ) {
         return 0.0f;
      }
      float _eosio_f64_demote( double a ) {
         return 0.0f;
      }
      int32_t _eosio_f32_trunc_i32s( float af ) {
         return 0;
      }
      int32_t _eosio_f64_trunc_i32s( double af ) {
         return 0;
      }
      uint32_t _eosio_f32_trunc_i32u( float af ) {
         return 0;
      }
      uint32_t _eosio_f64_trunc_i32u( double af ) {
         return 0;
      }
      int64_t _eosio_f32_trunc_i64s( float af ) {
         return 0;
      }
      int64_t _eosio_f64_trunc_i64s( double af ) {
         return 0;
      }
      uint64_t _eosio_f32_trunc_i64u( float af ) {
         return 0;
      }
      uint64_t _eosio_f64_trunc_i64u( double af ) {
         return 0;
      }
      float _eosio_i32_to_f32( int32_t a )  {
         return 0.0f;
      }
      float _eosio_i64_to_f32( int64_t a ) {
         return 0.0f;
      }
      float _eosio_ui32_to_f32( uint32_t a ) {
         return 0.0f;
      }
      float _eosio_ui64_to_f32( uint64_t a ) {
         return 0.0f;
      }
      double _eosio_i32_to_f64( int32_t a ) {
         return 0.0f;
      }
      double _eosio_i64_to_f64( int64_t a ) {
         return 0.0f;
      }
      double _eosio_ui32_to_f64( uint32_t a ) {
         return 0.0f;
      }
      double _eosio_ui64_to_f64( uint64_t a ) {
         return 0.0f;
      }

      static bool is_nan( const float f ) {
         return false;
      }

      static constexpr uint32_t inv_float_eps = 0x4B000000;
      static constexpr uint64_t inv_double_eps = 0x4330000000000000;
};

class producer_api : public context_aware_api {
   public:

      int get_active_producers(array_ptr<uint64_t> producers, size_t buffer_size) {
         return 0;
      }
};

class crypto_api : public context_aware_api {
   public:
      explicit crypto_api( )
      :context_aware_api(true){}
      /**
       * This method can be optimized out during replay as it has
       * no possible side effects other than "passing".
       */
      void assert_recover_key( const uint64_t digest,
                        array_ptr<char> sig, size_t siglen,
                        array_ptr<char> pub, size_t publen ) {
      }

      int recover_key( const uint64_t digest,
                        array_ptr<char> sig, size_t siglen,
                        array_ptr<char> pub, size_t publen ) {
                           return 0;
      }

      template<class Encoder> auto encode(char* data, size_t datalen) {
         return 0;
      }

      void assert_sha256(array_ptr<char> data, size_t datalen, int hash_val) {
      }

      void assert_sha1(array_ptr<char> data, size_t datalen, int hash_val) {
      }

      void assert_sha512(array_ptr<char> data, size_t datalen, int hash_val) {
      }

      void assert_ripemd160(array_ptr<char> data, size_t datalen,int hash_val) {
      }

      void sha1(array_ptr<char> data, size_t datalen, int hash_val) {
      }

      void sha256(array_ptr<char> data, size_t datalen, int hash_val) {
      }

      void sha512(array_ptr<char> data, size_t datalen, int hash_val) {
      }

      void ripemd160(array_ptr<char> data, size_t datalen, int hash_val) {
      }
};

class permission_api : public context_aware_api {
   public:
      using context_aware_api::context_aware_api;

      bool check_transaction_authorization( array_ptr<char> trx_data,     size_t trx_size,
                                            array_ptr<char> pubkeys_data, size_t pubkeys_size,
                                            array_ptr<char> perms_data,   size_t perms_size
                                          )
      {
         return false;
      }

      bool check_permission_authorization( uint64_t account, uint64_t permission,
                                           array_ptr<char> pubkeys_data, size_t pubkeys_size,
                                           array_ptr<char> perms_data,   size_t perms_size,
                                           uint64_t delay_us
                                         )
      {
         return false;
      }

      int64_t get_permission_last_used( uint64_t account, uint64_t permission ) {
         return 0;
      };

      int64_t get_account_creation_time( uint64_t account ) {
         return 0;
      }
};

class authorization_api : public context_aware_api {
   public:

   void require_authorization( const uint64_t account ) {
   }

   bool has_authorization( const uint64_t account )const {
      return false;
   }

   void require_authorization(const uint64_t account, const uint64_t permission) {
   }

   void require_recipient( uint64_t recipient ) {
   }

   bool is_account( const uint64_t account )const {
      return false;
   }

};

class system_api : public context_aware_api {
   public:
      using context_aware_api::context_aware_api;

      uint64_t current_time() {
         return 0;
      }

      uint64_t publication_time() {
         return 0;
      }

};

// constexpr size_t max_assert_message = 1024;

class context_free_system_api :  public context_aware_api {
public:
   explicit context_free_system_api()
   :context_aware_api(true){}

   void abort() {
   }

   // Kept as intrinsic rather than implementing on WASM side (using eosio_assert_message and strlen) because strlen is faster on native side.
   void eosio_assert( bool condition, null_terminated_ptr msg ) {
   }

   void eosio_assert_message( bool condition, array_ptr<char> msg, size_t msg_len ) {
   }

   void eosio_assert_code( bool condition, uint64_t error_code ) {
   }

   void eosio_exit(int32_t code) {
   }

};

class action_api : public context_aware_api {
   public:
   action_api()
      :context_aware_api(true){}

      int read_action_data(array_ptr<char> memory, size_t buffer_size) {
         return 0;
      }

      int action_data_size() {
         return 0;
      }

      uint64_t current_receiver() {
         return 0;
      }
};

class console_api : public context_aware_api {
   public:
      console_api()
      : context_aware_api(true){}

      // Kept as intrinsic rather than implementing on WASM side (using prints_l and strlen) because strlen is faster on native side.
      void prints(null_terminated_ptr str) {
      }

      void prints_l(array_ptr<char> str, size_t str_len ) {
      }

      void printi(int64_t val) {
      }

      void printui(uint64_t val) {
      }

      // void printi128(const __int128& val) {
      // }

      // void printui128(const unsigned __int128& val) {
      // }

      void printsf( float val ) {
      }

      void printdf( double val ) {
      }

      void printqf( const float val ) {
      }

      void printn(const uint64_t value) {
      }

      void printhex(array_ptr<char> data, size_t data_len ) {
      }

};

#define DB_API_METHOD_WRAPPERS_SIMPLE_SECONDARY(IDX, TYPE)\
      int db_##IDX##_store( uint64_t scope, uint64_t table, uint64_t payer, uint64_t id, const TYPE secondary ) {\
         return 0;\
      }\
      void db_##IDX##_update( int iterator, uint64_t payer, const TYPE secondary ) {\
         return;\
      }\
      void db_##IDX##_remove( int iterator ) {\
         return ;\
      }\
      int db_##IDX##_find_secondary( uint64_t code, uint64_t scope, uint64_t table, const TYPE secondary, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_find_primary( uint64_t code, uint64_t scope, uint64_t table, TYPE secondary, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_lowerbound( uint64_t code, uint64_t scope, uint64_t table,  TYPE secondary, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_upperbound( uint64_t code, uint64_t scope, uint64_t table,  TYPE secondary, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_end( uint64_t code, uint64_t scope, uint64_t table ) {\
         return 0;\
      }\
      int db_##IDX##_next( int iterator, uint64_t primary  ) {\
         return 0;\
      }\
      int db_##IDX##_previous( int iterator, uint64_t primary ) {\
         return 0;\
      }

#define DB_API_METHOD_WRAPPERS_ARRAY_SECONDARY(IDX, ARR_SIZE, ARR_ELEMENT_TYPE)\
      int db_##IDX##_store( uint64_t scope, uint64_t table, uint64_t payer, uint64_t id, array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len) {\
         return 0;\
      }\
      void db_##IDX##_update( int iterator, uint64_t payer, array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len ) {\
         return;\
      }\
      void db_##IDX##_remove( int iterator ) {\
         return;\
      }\
      int db_##IDX##_find_secondary( uint64_t code, uint64_t scope, uint64_t table, array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_find_primary( uint64_t code, uint64_t scope, uint64_t table, array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_lowerbound( uint64_t code, uint64_t scope, uint64_t table, array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_upperbound( uint64_t code, uint64_t scope, uint64_t table, array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_end( uint64_t code, uint64_t scope, uint64_t table ) {\
         return 0;\
      }\
      int db_##IDX##_next( int iterator, uint64_t primary  ) {\
         return 0;\
      }\
      int db_##IDX##_previous( int iterator, uint64_t primary ) {\
         return 0;\
      }

#define DB_API_METHOD_WRAPPERS_FLOAT_SECONDARY(IDX, TYPE)\
      int db_##IDX##_store( uint64_t scope, uint64_t table, uint64_t payer, uint64_t id, const TYPE secondary ) {\
         return 0;\
      }\
      void db_##IDX##_update( int iterator, uint64_t payer, const TYPE secondary ) {\
         return;\
      }\
      void db_##IDX##_remove( int iterator ) {\
         return;\
      }\
      int db_##IDX##_find_secondary( uint64_t code, uint64_t scope, uint64_t table, const TYPE secondary, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_find_primary( uint64_t code, uint64_t scope, uint64_t table, TYPE secondary, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_lowerbound( uint64_t code, uint64_t scope, uint64_t table,  TYPE secondary, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_upperbound( uint64_t code, uint64_t scope, uint64_t table,  TYPE secondary, uint64_t primary ) {\
         return 0;\
      }\
      int db_##IDX##_end( uint64_t code, uint64_t scope, uint64_t table ) {\
         return 0;\
      }\
      int db_##IDX##_next( int iterator, uint64_t primary  ) {\
         return 0;\
      }\
      int db_##IDX##_previous( int iterator, uint64_t primary ) {\
         return 0;\
      }

class database_api : public context_aware_api {
   public:

      int db_store_i64( uint64_t scope, uint64_t table, uint64_t payer, uint64_t id, array_ptr<char> buffer, int buffer_size ) {
         return 0;
      }
      void db_update_i64( int itr, uint64_t payer, array_ptr<char> buffer, int buffer_size ) {
      }
      void db_remove_i64( int itr ) {
      }
      int db_get_i64( int itr, array_ptr<char> buffer, int buffer_size ) {
         return 0;
      }
      int db_next_i64( int itr, uint64_t primary ) {
         return 0;
      }
      int db_previous_i64( int itr, uint64_t primary ) {
         return 0;
      }
      int db_find_i64( uint64_t code, uint64_t scope, uint64_t table, uint64_t id ) {
         return 0;
      }
      int db_lowerbound_i64( uint64_t code, uint64_t scope, uint64_t table, uint64_t id ) {
         return 0;
      }
      int db_upperbound_i64( uint64_t code, uint64_t scope, uint64_t table, uint64_t id ) {
         return 0;
      }
      int db_end_i64( uint64_t code, uint64_t scope, uint64_t table ) {
         return 0;
      }

      DB_API_METHOD_WRAPPERS_SIMPLE_SECONDARY(idx64,  uint64_t)
      DB_API_METHOD_WRAPPERS_SIMPLE_SECONDARY(idx128, uint64_t)
      DB_API_METHOD_WRAPPERS_ARRAY_SECONDARY(idx256, 2, uint64_t)
      DB_API_METHOD_WRAPPERS_FLOAT_SECONDARY(idx_double, float)
      DB_API_METHOD_WRAPPERS_FLOAT_SECONDARY(idx_long_double, float)
};

class memory_api : public context_aware_api {
   public:
      memory_api()
      :context_aware_api(true){}

      char* memcpy( array_ptr<char> dest, array_ptr<char> src, size_t length) {
         return nullptr;
      }

      char* memmove( array_ptr<char> dest, array_ptr<char> src, size_t length) {
         return nullptr;
      }

      int memcmp( array_ptr<char> dest, array_ptr<char> src, size_t length) {
         return 0;
      }

      char* memset( array_ptr<char> dest, int value, size_t length ) {
         return nullptr;
      }
};

class transaction_api : public context_aware_api {
   public:

      void send_inline( array_ptr<char> data, size_t data_len ) {
      }

      void send_context_free_inline( array_ptr<char> data, size_t data_len ) {
      }

      void send_deferred( const int sender_id, uint64_t payer, array_ptr<char> data, size_t data_len, uint32_t replace_existing) {
      }

      bool cancel_deferred( const int val ) {
         return false;
      }
};


class context_free_transaction_api : public context_aware_api {
   public:
      context_free_transaction_api( )
      :context_aware_api(true){}

      int read_transaction( array_ptr<char> data, size_t buffer_size ) {
         return 0;
      }

      int transaction_size() {
         return 0;
      }

      int expiration() {
        return 0;
      }

      int tapos_block_num() {
        return 0;
      }
      int tapos_block_prefix() {
        return 0;
      }

      int get_action( uint32_t type, uint32_t index, array_ptr<char> buffer, size_t buffer_size )const {
         return 0;
      }
};

class compiler_builtins : public context_aware_api {
   public:
      compiler_builtins( )
      :context_aware_api(true){}

      void __ashlti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
      }

      void __ashrti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
         // retain the signedness
      }

      void __lshlti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
      }

      void __lshrti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
      }

      void __divti3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
      }

      void __udivti3(unsigned __int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
      }

      void __multi3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
      }

      void __modti3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
      }

      void __umodti3(unsigned __int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
      }

      // arithmetic long double
      void __addtf3( float& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
      }
      void __subtf3( float& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
      }
      void __multf3( float& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
      }
      void __divtf3( float& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
      }
      void __negtf2( float& ret, uint64_t la, uint64_t ha ) {
      }

      // conversion long double
      void __extendsftf2( float& ret, float f ) {
      }
      void __extenddftf2( float& ret, double d ) {
      }
      double __trunctfdf2( uint64_t l, uint64_t h ) {
         return 0.0f;
      }
      float __trunctfsf2( uint64_t l, uint64_t h ) {
         return 0.0f;
      }
      int32_t __fixtfsi( uint64_t l, uint64_t h ) {
         return 0;
      }
      int64_t __fixtfdi( uint64_t l, uint64_t h ) {
         return 0;
      }
      void __fixtfti( __int128& ret, uint64_t l, uint64_t h ) {
      }
      uint32_t __fixunstfsi( uint64_t l, uint64_t h ) {
         return 0;
      }
      uint64_t __fixunstfdi( uint64_t l, uint64_t h ) {
         return 0;
      }
      void __fixunstfti( unsigned __int128& ret, uint64_t l, uint64_t h ) {
      }
      void __fixsfti( __int128& ret, float a ) {
      }
      void __fixdfti( __int128& ret, double a ) {
      }
      void __fixunssfti( unsigned __int128& ret, float a ) {
      }
      void __fixunsdfti( unsigned __int128& ret, double a ) {
      }
      double __floatsidf( int32_t i ) {
         return 0.0f;
      }
      void __floatsitf( float& ret, int32_t i ) {
      }
      void __floatditf( float& ret, uint64_t a ) {
      }
      void __floatunsitf( float& ret, uint32_t i ) {
      }
      void __floatunditf( float& ret, uint64_t a ) {
      }
      double __floattidf( uint64_t l, uint64_t h ) {
         return 0.0f;
      }
      double __floatuntidf( uint64_t l, uint64_t h ) {
         return 0.0f;
      }
      int ___cmptf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb, int return_value_if_nan ) {
         return 0;
      }
      int __eqtf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return 0;
      }
      int __netf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return 0;
      }
      int __getf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return 0;
      }
      int __gttf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return 0;
      }
      int __letf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return 0;
      }
      int __lttf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return 0;
      }
      int __cmptf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return 0;
      }
      int __unordtf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return 0;
      }

      static constexpr uint32_t SHIFT_WIDTH = (sizeof(uint64_t)*8)-1;
};


/*
 * This api will be removed with fix for `eos #2561`
 */
class call_depth_api : public context_aware_api {
   public:
      call_depth_api( )
      :context_aware_api(true){}
      void call_depth_assert() {
      }
};

class transaction_context : public context_aware_api {
   public:
      void checktime()const {}
};

REGISTER_INJECTED_INTRINSICS(call_depth_api,
   (call_depth_assert,  void()               )
);

REGISTER_INTRINSICS(compiler_builtins,
   (__ashlti3,     void(int, int64_t, int64_t, int)               )
   (__ashrti3,     void(int, int64_t, int64_t, int)               )
   (__lshlti3,     void(int, int64_t, int64_t, int)               )
   (__lshrti3,     void(int, int64_t, int64_t, int)               )
   (__divti3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__udivti3,     void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__modti3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__umodti3,     void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__multi3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__addtf3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__subtf3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__multf3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__divtf3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__eqtf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__netf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__getf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__gttf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__lttf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__letf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__cmptf2,      int(int64_t, int64_t, int64_t, int64_t)        )
   (__unordtf2,    int(int64_t, int64_t, int64_t, int64_t)        )
   (__negtf2,      void (int, int64_t, int64_t)                   )
   (__floatsitf,   void (int, int)                                )
   (__floatunsitf, void (int, int)                                )
   (__floatditf,   void (int, int64_t)                            )
   (__floatunditf, void (int, int64_t)                            )
   (__floattidf,   double (int64_t, int64_t)                      )
   (__floatuntidf, double (int64_t, int64_t)                      )
   (__floatsidf,   double(int)                                    )
   (__extendsftf2, void(int, float)                               )
   (__extenddftf2, void(int, double)                              )
   (__fixtfti,     void(int, int64_t, int64_t)                    )
   (__fixtfdi,     int64_t(int64_t, int64_t)                      )
   (__fixtfsi,     int(int64_t, int64_t)                          )
   (__fixunstfti,  void(int, int64_t, int64_t)                    )
   (__fixunstfdi,  int64_t(int64_t, int64_t)                      )
   (__fixunstfsi,  int(int64_t, int64_t)                          )
   (__fixsfti,     void(int, float)                               )
   (__fixdfti,     void(int, double)                              )
   (__fixunssfti,  void(int, float)                               )
   (__fixunsdfti,  void(int, double)                              )
   (__trunctfdf2,  double(int64_t, int64_t)                       )
   (__trunctfsf2,  float(int64_t, int64_t)                        )
);

REGISTER_INTRINSICS(privileged_api,
   (is_feature_active,                int(int64_t)                          )
   (activate_feature,                 void(int64_t)                         )
   (get_resource_limits,              void(int64_t,int,int,int)             )
   (set_resource_limits,              void(int64_t,int64_t,int64_t,int64_t) )
   (set_proposed_producers,           int64_t(int,int)                      )
   (get_blockchain_parameters_packed, int(int, int)                         )
   (set_blockchain_parameters_packed, void(int,int)                         )
   (is_privileged,                    int(int64_t)                          )
   (set_privileged,                   void(int64_t, int)                    )
);

REGISTER_INJECTED_INTRINSICS(transaction_context,
   (checktime,      void())
);

REGISTER_INTRINSICS(producer_api,
   (get_active_producers,      int(int, int) )
);

#define DB_SECONDARY_INDEX_METHODS_SIMPLE(IDX) \
   (db_##IDX##_store,          int(int64_t,int64_t,int64_t,int64_t,int))\
   (db_##IDX##_remove,         void(int))\
   (db_##IDX##_update,         void(int,int64_t,int))\
   (db_##IDX##_find_primary,   int(int64_t,int64_t,int64_t,int,int64_t))\
   (db_##IDX##_find_secondary, int(int64_t,int64_t,int64_t,int,int))\
   (db_##IDX##_lowerbound,     int(int64_t,int64_t,int64_t,int,int))\
   (db_##IDX##_upperbound,     int(int64_t,int64_t,int64_t,int,int))\
   (db_##IDX##_end,            int(int64_t,int64_t,int64_t))\
   (db_##IDX##_next,           int(int, int))\
   (db_##IDX##_previous,       int(int, int))

#define DB_SECONDARY_INDEX_METHODS_ARRAY(IDX) \
      (db_##IDX##_store,          int(int64_t,int64_t,int64_t,int64_t,int,int))\
      (db_##IDX##_remove,         void(int))\
      (db_##IDX##_update,         void(int,int64_t,int,int))\
      (db_##IDX##_find_primary,   int(int64_t,int64_t,int64_t,int,int,int64_t))\
      (db_##IDX##_find_secondary, int(int64_t,int64_t,int64_t,int,int,int))\
      (db_##IDX##_lowerbound,     int(int64_t,int64_t,int64_t,int,int,int))\
      (db_##IDX##_upperbound,     int(int64_t,int64_t,int64_t,int,int,int))\
      (db_##IDX##_end,            int(int64_t,int64_t,int64_t))\
      (db_##IDX##_next,           int(int, int))\
      (db_##IDX##_previous,       int(int, int))

REGISTER_INTRINSICS( database_api,
   (db_store_i64,        int(int64_t,int64_t,int64_t,int64_t,int,int))
   (db_update_i64,       void(int,int64_t,int,int))
   (db_remove_i64,       void(int))
   (db_get_i64,          int(int, int, int))
   (db_next_i64,         int(int, int))
   (db_previous_i64,     int(int, int))
   (db_find_i64,         int(int64_t,int64_t,int64_t,int64_t))
   (db_lowerbound_i64,   int(int64_t,int64_t,int64_t,int64_t))
   (db_upperbound_i64,   int(int64_t,int64_t,int64_t,int64_t))
   (db_end_i64,          int(int64_t,int64_t,int64_t))

   DB_SECONDARY_INDEX_METHODS_SIMPLE(idx64)
   DB_SECONDARY_INDEX_METHODS_SIMPLE(idx128)
   DB_SECONDARY_INDEX_METHODS_ARRAY(idx256)
   DB_SECONDARY_INDEX_METHODS_SIMPLE(idx_double)
   DB_SECONDARY_INDEX_METHODS_SIMPLE(idx_long_double)
);

REGISTER_INTRINSICS(crypto_api,
   (assert_recover_key,     void(int, int, int, int, int) )
   (recover_key,            int(int, int, int, int, int)  )
   (assert_sha256,          void(int, int, int)           )
   (assert_sha1,            void(int, int, int)           )
   (assert_sha512,          void(int, int, int)           )
   (assert_ripemd160,       void(int, int, int)           )
   (sha1,                   void(int, int, int)           )
   (sha256,                 void(int, int, int)           )
   (sha512,                 void(int, int, int)           )
   (ripemd160,              void(int, int, int)           )
);


REGISTER_INTRINSICS(permission_api,
   (check_transaction_authorization, int(int, int, int, int, int, int)                  )
   (check_permission_authorization,  int(int64_t, int64_t, int, int, int, int, int64_t) )
   (get_permission_last_used,        int64_t(int64_t, int64_t) )
   (get_account_creation_time,       int64_t(int64_t) )
);


REGISTER_INTRINSICS(system_api,
   (current_time, int64_t()       )
   (publication_time,   int64_t() )
);

REGISTER_INTRINSICS(context_free_system_api,
   (abort,                void()              )
   (eosio_assert,         void(int, int)      )
   (eosio_assert_message, void(int, int, int) )
   (eosio_assert_code,    void(int, int64_t)  )
   (eosio_exit,           void(int)           )
);

REGISTER_INTRINSICS(action_api,
   (read_action_data,       int(int, int)  )
   (action_data_size,       int()          )
   (current_receiver,   int64_t()          )
);

REGISTER_INTRINSICS(authorization_api,
   (require_recipient,     void(int64_t)          )
   (require_authorization, void(int64_t), "require_auth", void(authorization_api::*)(const uint64_t) )
   (require_authorization, void(int64_t, int64_t), "require_auth2", void(authorization_api::*)(const uint64_t, const uint64_t permission) )
   (has_authorization,     int(int64_t), "has_auth", bool(authorization_api::*)(const uint64_t)const )
   (is_account,            int(int64_t)           )
);

REGISTER_INTRINSICS(console_api,
   (prints,                void(int)      )
   (prints_l,              void(int, int) )
   (printi,                void(int64_t)  )
   (printui,               void(int64_t)  )
   // (printi128,             void(int)      )
   // (printui128,            void(int)      )
   (printsf,               void(float)    )
   (printdf,               void(double)   )
   (printqf,               void(int)      )
   (printn,                void(int64_t)  )
   (printhex,              void(int, int) )
);

REGISTER_INTRINSICS(context_free_transaction_api,
   (read_transaction,       int(int, int)            )
   (transaction_size,       int()                    )
   (expiration,             int()                    )
   (tapos_block_prefix,     int()                    )
   (tapos_block_num,        int()                    )
   (get_action,             int (int, int, int, int) )
);

REGISTER_INTRINSICS(transaction_api,
   (send_inline,               void(int, int)               )
   (send_context_free_inline,  void(int, int)               )
   (send_deferred,             void(int, int64_t, int, int, int32_t) )
   (cancel_deferred,           int(int)                     )
);

REGISTER_INTRINSICS(context_free_api,
   (get_context_free_data, int(int, int, int) )
)

REGISTER_INTRINSICS(memory_api,
   (memcpy,                 int(int, int, int)  )
   (memmove,                int(int, int, int)  )
   (memcmp,                 int(int, int, int)  )
   (memset,                 int(int, int, int)  )
);

REGISTER_INJECTED_INTRINSICS(softfloat_api,
      (_eosio_f32_add,       float(float, float)    )
      (_eosio_f32_sub,       float(float, float)    )
      (_eosio_f32_mul,       float(float, float)    )
      (_eosio_f32_div,       float(float, float)    )
      (_eosio_f32_min,       float(float, float)    )
      (_eosio_f32_max,       float(float, float)    )
      (_eosio_f32_copysign,  float(float, float)    )
      (_eosio_f32_abs,       float(float)           )
      (_eosio_f32_neg,       float(float)           )
      (_eosio_f32_sqrt,      float(float)           )
      (_eosio_f32_ceil,      float(float)           )
      (_eosio_f32_floor,     float(float)           )
      (_eosio_f32_trunc,     float(float)           )
      (_eosio_f32_nearest,   float(float)           )
      (_eosio_f32_eq,        int(float, float)      )
      (_eosio_f32_ne,        int(float, float)      )
      (_eosio_f32_lt,        int(float, float)      )
      (_eosio_f32_le,        int(float, float)      )
      (_eosio_f32_gt,        int(float, float)      )
      (_eosio_f32_ge,        int(float, float)      )
      (_eosio_f64_add,       double(double, double) )
      (_eosio_f64_sub,       double(double, double) )
      (_eosio_f64_mul,       double(double, double) )
      (_eosio_f64_div,       double(double, double) )
      (_eosio_f64_min,       double(double, double) )
      (_eosio_f64_max,       double(double, double) )
      (_eosio_f64_copysign,  double(double, double) )
      (_eosio_f64_abs,       double(double)         )
      (_eosio_f64_neg,       double(double)         )
      (_eosio_f64_sqrt,      double(double)         )
      (_eosio_f64_ceil,      double(double)         )
      (_eosio_f64_floor,     double(double)         )
      (_eosio_f64_trunc,     double(double)         )
      (_eosio_f64_nearest,   double(double)         )
      (_eosio_f64_eq,        int(double, double)    )
      (_eosio_f64_ne,        int(double, double)    )
      (_eosio_f64_lt,        int(double, double)    )
      (_eosio_f64_le,        int(double, double)    )
      (_eosio_f64_gt,        int(double, double)    )
      (_eosio_f64_ge,        int(double, double)    )
      (_eosio_f32_promote,    double(float)         )
      (_eosio_f64_demote,     float(double)         )
      (_eosio_f32_trunc_i32s, int(float)            )
      (_eosio_f64_trunc_i32s, int(double)           )
      (_eosio_f32_trunc_i32u, int(float)            )
      (_eosio_f64_trunc_i32u, int(double)           )
      (_eosio_f32_trunc_i64s, int64_t(float)        )
      (_eosio_f64_trunc_i64s, int64_t(double)       )
      (_eosio_f32_trunc_i64u, int64_t(float)        )
      (_eosio_f64_trunc_i64u, int64_t(double)       )
      (_eosio_i32_to_f32,     float(int32_t)        )
      (_eosio_i64_to_f32,     float(int64_t)        )
      (_eosio_ui32_to_f32,    float(int32_t)        )
      (_eosio_ui64_to_f32,    float(int64_t)        )
      (_eosio_i32_to_f64,     double(int32_t)       )
      (_eosio_i64_to_f64,     double(int64_t)       )
      (_eosio_ui32_to_f64,    double(int32_t)       )
      (_eosio_ui64_to_f64,    double(int64_t)       )
);


} // end of namespace wabt
