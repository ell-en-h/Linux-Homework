#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 4004
#define BUF_SIZE 1024

int sock;
void* receiver(void*) {
    char buf[BUF_SIZE];
    while (true) {
        int n = recv(sock, buf, BUF_SIZE, 0);
        if (n <= 0) break;
        std::cout << buf;
    }
    return nullptr;
}

int main() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (sockaddr*)&server, sizeof(server));

    std::string name;
    std::cout << "Name: ";
    std::getline(std::cin, name);
    name += "\n";
    send(sock, name.c_str(), name.size(), 0);

    pthread_t tid;
    pthread_create(&tid, nullptr, receiver, nullptr);

    std::string msg;
    while (std::getline(std::cin, msg)) {
        msg += "\n";
        send(sock, msg.c_str(), msg.size(), 0);
        if (msg == "/exit\n") break;
    }
    close(sock);
}

