#include "TcpServer.hpp"
#include <thread>
#include <memory>

namespace SimpleNet {

TcpServer::TcpServer(int port, size_t threads) : pool_(threads) {
    listen_socket_.bind(port);
    listen_socket_.listen();
}

void TcpServer::run(ClientHandler handler) {
    while (true) {
        Socket client = listen_socket_.accept();

	auto client_ptr = std::make_shared<Socket>(std::move(client));

	pool_.enqueue([handler, client_ptr]() {
    	handler(std::move(*client_ptr));
	});

    }
}

}

