#include "stdafx.h"
#include "example_rpc.h"

int example::add(int a, int b) { return a + b; }
double example::add(double a, double b) { return a + b; }
int example::add(int a, int b, int c) { return a + b + c; }

int _tmain(int argc, _TCHAR* argv[])
{
    boost::asio::io_service io_service;
    example::example_rpc_server_t server(io_service, 12345);
    server.request_accept();

    example::example_rpc_client_t client(io_service);
    client.connect("127.0.0.1", 12345);
    
    client.add(10, 20, [] (int result) {
        std::cout << result << std::endl;
    });
    client.add(11.5, 23.7, [] (double result) {
        std::cout << result << std::endl;
    });
    client.add(10, 20, 30, [] (int result) {
        std::cout << result << std::endl;
    });
    io_service.run();
    return 0;
}
