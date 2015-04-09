#include "stdafx.h"
#include "rpc.h"

std::atomic_int rpc_client_t::_req_num_gen;

rpc_server_t::rpc_server_t(boost::asio::io_service& io_service, int server_port)
    : _io_service(io_service)
    , _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), server_port)) {
}

void rpc_server_t::request_accept() {
    session_ref session = boost::make_shared<session_t>(_io_service, _handler_array);
    _acceptor.async_accept(session->_socket,
        boost::bind(&rpc_server_t::handle_accept, this, session,
            boost::asio::placeholders::error));
}
void rpc_server_t::handle_accept(session_ref session, const boost::system::error_code& error) {
    session->request_read_msg_size();
    request_accept();
}

rpc_client_t::rpc_client_t(boost::asio::io_service& io_service)
    : _session(boost::make_shared<session_t>(io_service, _handler_array)) {
    initialize_handlers();
}

void rpc_client_t::connect(const std::string& addr, unsigned short port) {
    _session->request_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(addr), port));
}

int rpc_client_t::register_callback(general_callback_ptr callback) {
    int req_num = ++_req_num_gen;

    std::unique_lock<std::mutex> lock(_callback_map_mutex);
    _callback_map.insert(std::make_pair(req_num, std::move(callback)));
    return req_num;
}

rpc_client_t::general_callback_ptr rpc_client_t::get_callback(int req_num) {
    std::unique_lock<std::mutex> lock(_callback_map_mutex);
    auto it = _callback_map.find(req_num);
    if (it == _callback_map.end())
        return nullptr;
    return std::move(it->second);
}

void rpc_client_t::initialize_handlers() {
    auto handler = boost::bind(&rpc_client_t::default_handler, this, _1, _2);
    for (auto& each : _handler_array)
        each = handler;
}

bool rpc_client_t::default_handler(buffer_reader_t reader, buffer_writer_t writer) {
    int req_num;
    reader >> req_num;
    general_callback_ptr callback = get_callback(req_num);
    if (callback != nullptr)
        (*callback)(reader);
    return false;
}

