#include "TcpServer.hpp"
#include <iostream>

using namespace SimpleNet;

int main() {
    try {
        TcpServer server(8080, 4);

        server.run([](Socket client) {
            auto data = client.receive();
            std::string message(data.begin(), data.end());

            std::cout << "Received: " << message << std::endl;

            client.send("Hello from server\n");
        });

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

