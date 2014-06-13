#pragma once
#include "buffer.h"
#include "handler.h"

class session_t : public boost::enable_shared_from_this<session_t> {
public:
    session_t(boost::asio::io_service& io_service, handler_array_t& handler_array);

    void request_connect(boost::asio::ip::tcp::endpoint endpoint);
    void request_read_msg_size();
    void request_read_msg();
    void request_write(buffer_ref buffer);

    bool is_connected() const { return _connected; }

    friend class rpc_server_t;

private:
    void handle_connect(const boost::system::error_code& error);
    void handle_read_msg_size(const boost::system::error_code& error);
    void handle_read_msg(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error);

private:
    boost::asio::io_service& _io_service;
    boost::asio::ip::tcp::socket _socket;

    size_t _msg_size;
    buffer_t _msg_buffer;
    std::atomic_bool _connected;

    handler_array_t& _handler_array;
};

typedef boost::shared_ptr<session_t> session_ref;
