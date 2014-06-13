// util
#ifndef RPC_TYPE1
#   define RPC_TYPE1(name, type1) type_##name##_##type1
#endif
#ifndef RPC_TYPE2
#   define RPC_TYPE2(name, type1, type2) type_##name##_##type1##_##type2
#endif
#ifndef RPC_TYPE3
#   define RPC_TYPE3(name, type1, type2, type3) type_##name##_##type1##_##type2##_##type3
#endif

#ifdef _RPC_GEN_TYPE_
// type enum
#   define RPC_BEGIN(name) namespace name { enum type_enum {
#   define RPC_METHOD(rtype, name, type1, var1) RPC_TYPE1(name, type1),
#   define RPC_METHOD2(rtype, name, type1, var1, type2, var2) RPC_TYPE2(name, type1, type2),
#   define RPC_METHOD3(rtype, name, type1, var1, type2, var2, type3, var3) RPC_TYPE3(name, type1, type2, type3),
#   define RPC_END(name) }; /* enum */ } /* namespace */

#   include _RPC_GEN_TARGET_

#   undef RPC_END
#   undef RPC_METHOD3
#   undef RPC_METHOD2
#   undef RPC_METHOD1
#   undef RPC_BEGIN
#endif


#ifdef _RPC_GEN_SERVER_
// handler prototypes
#   define RPC_BEGIN(name) namespace name {
#   define RPC_METHOD1(rtype, name, type1, var1) rtype name(type1 var1);
#   define RPC_METHOD2(rtype, name, type1, var1, type2, var2) rtype name(type1 var1, type2 var2);
#   define RPC_METHOD3(rtype, name, type1, var1, type2, var2, type3, var3) rtype name(type1 var1, type2 var2, type3 var3);
#   define RPC_END(name) };

#   include _RPC_GEN_TARGET_

#   undef RPC_END
#   undef RPC_METHOD3
#   undef RPC_METHOD2
#   undef RPC_METHOD1
#   undef RPC_BEGIN

// rpc_server_t impl
#   define RPC_BEGIN(name) \
    namespace name { \
    class name##_rpc_server_t : public rpc_server_t { \
    public: \
        name##_rpc_server_t(boost::asio::io_service& io_service, int server_port) \
            : rpc_server_t(io_service, server_port) { \
            initialize_handlers(); \
        } \
    private: \
        void initialize_handlers() {
#   define RPC_METHOD1(rtype, name, type1, var1) \
            _handler_array[RPC_TYPE1(name, type1)] = [] \
                    (buffer_reader_t reader, buffer_writer_t writer) -> bool { \
                int req_num; \
                type1 var1; \
                reader >> req_num >> var1; \
                rtype result = name(var1); \
                writer << sizeof(uint16_t) + sizeof(int) + sizeof(rtype); \
                writer << static_cast<uint16_t>(RPC_TYPE1(name, type1)); \
                writer << req_num; \
                writer << result; \
                return true; \
            };
#   define RPC_METHOD2(rtype, name, type1, var1, type2, var2) \
            _handler_array[RPC_TYPE2(name, type1, type2)] = [] \
                    (buffer_reader_t reader, buffer_writer_t writer) -> bool { \
                int req_num; \
                type1 var1; \
                type2 var2; \
                reader >> req_num >> var1 >> var2; \
                rtype result = name(var1, var2); \
                writer << sizeof(uint16_t) + sizeof(int) + sizeof(rtype); \
                writer << static_cast<uint16_t>(RPC_TYPE2(name, type1, type2)); \
                writer << req_num; \
                writer << result; \
                return true; \
            };
#   define RPC_METHOD3(rtype, name, type1, var1, type2, var2, type3, var3) \
            _handler_array[RPC_TYPE3(name, type1, type2, type3)] = [] \
                    (buffer_reader_t reader, buffer_writer_t writer) -> bool { \
                int req_num; \
                type1 var1; \
                type2 var2; \
                type3 var3; \
                reader >> req_num >> var1 >> var2 >> var3; \
                rtype result = name(var1, var2, var3); \
                writer << sizeof(uint16_t) + sizeof(int) + sizeof(rtype); \
                writer << static_cast<uint16_t>(RPC_TYPE3(name, type1, type2, type3)); \
                writer << req_num; \
                writer << result; \
                return true; \
            };
#   define RPC_END(name) \
        } /* initialize_handlers */ \
    }; /* class */ \
    }  /* namespace */

#   include _RPC_GEN_TARGET_

#   undef RPC_END
#   undef RPC_METHOD3
#   undef RPC_METHOD2
#   undef RPC_METHOD1
#   undef RPC_BEGIN
#endif


#ifdef _RPC_GEN_CLIENT_
// rpc_client_t impl
#   define RPC_BEGIN(name) \
    namespace name { \
    class name##_rpc_client_t : public rpc_client_t { \
    public: \
        name##_rpc_client_t(boost::asio::io_service& io_service) \
            : rpc_client_t(io_service) { \
        }
#   define RPC_METHOD1(rtype, name, type1, var1) \
        void name(type1 var1, type2 var2, boost::function<void (rtype)> callback) { \
            buffer_writer_t writer; \
            writer << sizeof(uint16_t) + sizeof(int) + sizeof(type1); \
            writer << static_cast<uint16_t>(RPC_TYPE1(name, type1)); \
            struct __callback_t : public general_callback_t { \
                virtual void operator () (buffer_reader_t reader) { \
                    rtype result; \
                    reader >> result; \
                    _callback(result); \
                } \
                __callback_t(boost::function<void (rtype)> callback) \
                    : _callback(callback) {} \
                boost::function<void (rtype)> _callback; \
            }; \
            int req_num = register_callback(general_callback_ptr( \
                                new __callback_t(callback))); \
            writer << req_num << var1; \
            _session->request_write(writer.buffer()); \
        }
#   define RPC_METHOD2(rtype, name, type1, var1, type2, var2) \
        void name(type1 var1, type2 var2, boost::function<void (rtype)> callback) { \
            buffer_writer_t writer; \
            writer << sizeof(uint16_t) + sizeof(int) + sizeof(type1) + sizeof(type2); \
            writer << static_cast<uint16_t>(RPC_TYPE2(name, type1, type2)); \
            struct __callback_t : public general_callback_t { \
                virtual void operator () (buffer_reader_t reader) { \
                    rtype result; \
                    reader >> result; \
                    _callback(result); \
                } \
                __callback_t(boost::function<void (rtype)> callback) \
                    : _callback(callback) {} \
                boost::function<void (rtype)> _callback; \
            }; \
            int req_num = register_callback(general_callback_ptr( \
                                new __callback_t(callback))); \
            writer << req_num << var1 << var2; \
            _session->request_write(writer.buffer()); \
        }
#   define RPC_METHOD3(rtype, name, type1, var1, type2, var2, type3, var3) \
        void name(type1 var1, type2 var2, type3 var3, boost::function<void (rtype)> callback) { \
            buffer_writer_t writer; \
            writer << sizeof(uint16_t) + sizeof(int) + sizeof(type1) + sizeof(type2) + sizeof(type3); \
            writer << static_cast<uint16_t>(RPC_TYPE3(name, type1, type2, type3)); \
            struct __callback_t : public general_callback_t { \
                virtual void operator () (buffer_reader_t reader) { \
                    rtype result; \
                    reader >> result; \
                    _callback(result); \
                } \
                __callback_t(boost::function<void (rtype)> callback) \
                    : _callback(callback) {} \
                boost::function<void (rtype)> _callback; \
            }; \
            int req_num = register_callback(general_callback_ptr( \
                                new __callback_t(callback))); \
            writer << req_num << var1 << var2 << var3; \
            _session->request_write(writer.buffer()); \
        }
#   define RPC_END(name)   \
    }; /* class */  \
    }  /* namespace */

#   include _RPC_GEN_TARGET_

#   undef RPC_END
#   undef RPC_METHOD3
#   undef RPC_METHOD2
#   undef RPC_METHOD1
#   undef RPC_BEGIN
#endif
