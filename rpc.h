#pragma once
#include "session.h"

class rpc_server_t {
public:
    rpc_server_t(boost::asio::io_service& io_service, int server_port);
    virtual ~rpc_server_t() {}

    void request_accept();
    void handle_accept(session_ref session, const boost::system::error_code& error);
    
protected:
    handler_array_t _handler_array;

private:
    boost::asio::io_service& _io_service;
    boost::asio::ip::tcp::acceptor _acceptor;
};

class rpc_client_t {
public:
    rpc_client_t(boost::asio::io_service& io_service);
    virtual ~rpc_client_t() {}

    void connect(const std::string& addr, unsigned short port);

protected:
    struct general_callback_t {
        virtual ~general_callback_t() {}
        virtual void operator () (buffer_reader_t) = 0;
    };
    typedef std::unique_ptr<general_callback_t> general_callback_ptr;
    typedef std::map<int, general_callback_ptr> callback_map_t;

    int register_callback(general_callback_ptr callback);
    general_callback_ptr get_callback(int req_num);

protected:
    session_ref _session;

private:
    void initialize_handlers();
    bool default_handler(buffer_reader_t reader, buffer_writer_t writer);

private:
    handler_array_t _handler_array;

    callback_map_t _callback_map;
    std::mutex _callback_map_mutex;

    static std::atomic_int _req_num_gen;
};
