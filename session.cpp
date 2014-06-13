#include "stdafx.h"
#include "session.h"

session_t::session_t(boost::asio::io_service& io_service, handler_array_t& handler_array)
    : _io_service(io_service), _socket(io_service), _handler_array(handler_array) {
    _connected = false;
}

void session_t::request_connect(boost::asio::ip::tcp::endpoint endpoint) {
    _socket.async_connect(endpoint,
        boost::bind(&session_t::handle_connect, shared_from_this(),
        boost::asio::placeholders::error));
}
    
void session_t::request_read_msg_size() {
    boost::asio::async_read(_socket,
        boost::asio::buffer(&_msg_size, sizeof(_msg_size)),
        boost::asio::transfer_exactly(sizeof(_msg_size)),
        boost::bind(&session_t::handle_read_msg_size, shared_from_this(),
        boost::asio::placeholders::error));
}

void session_t::request_read_msg() {
    if (_msg_buffer.size() < _msg_size)
        _msg_buffer.resize(_msg_size);

    boost::asio::async_read(_socket,
        boost::asio::buffer(_msg_buffer.data(), _msg_size),
        boost::asio::transfer_exactly(_msg_size),
        boost::bind(&session_t::handle_read_msg, shared_from_this(),
        boost::asio::placeholders::error));
}
    
void session_t::request_write(buffer_ref buffer) {
    boost::asio::async_write(_socket,
        boost::asio::buffer(buffer->data(), buffer->size()),
        boost::bind(&session_t::handle_write, shared_from_this(),
        boost::asio::placeholders::error));
}

void session_t::handle_connect(const boost::system::error_code& error) {
    _connected = !error;
    request_read_msg_size();
}

void session_t::handle_read_msg_size(const boost::system::error_code& error) {
    if (!error) {
        request_read_msg();
    } else _connected = false;
}

void session_t::handle_read_msg(const boost::system::error_code& error) {
    if (!error) {
        buffer_reader_t reader(_msg_buffer.data());
        uint16_t msg_type;
        reader >> msg_type;

        handler_t handler = _handler_array[msg_type];
        if (handler != nullptr) {
            buffer_writer_t writer;
            if (handler(reader, writer))
                request_write(writer.buffer());
        }
        request_read_msg_size();
    } else _connected = false;
}
    
void session_t::handle_write(const boost::system::error_code& error) {
        if (error) _connected = false;
}
