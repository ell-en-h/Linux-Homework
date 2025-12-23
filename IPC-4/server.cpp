#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 4004
#define BUF_SIZE 1024

struct Client {
    int socket;
    std::string name;
};
std::vector<Client> clients;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(const std::string& msg, int sender) {
    pthread_mutex_lock(&clients_mutex);
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].socket != sender) {
            send(clients[i].socket, msg.c_str(), msg.size(), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}
void send_user_list(int sock) {
    std::string list = "Users:\n";
    pthread_mutex_lock(&clients_mutex);
    for (size_t i = 0; i < clients.size(); ++i) {
        list += "- " + clients[i].name + "\n";
    }
    pthread_mutex_unlock(&clients_mutex);
    send(sock, list.c_str(), list.size(), 0);
}
void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    delete (int*)arg;

    char buffer[BUF_SIZE] = {0};

    int bytes = recv(client_socket, buffer, BUF_SIZE, 0);
    if (bytes <= 0) {
        close(client_socket);
        pthread_exit(nullptr);
    }

    std::string name(buffer);
    pthread_mutex_lock(&clients_mutex);
    clients.push_back({client_socket, name});
    pthread_mutex_unlock(&clients_mutex);

    broadcast(name + " joined\n", client_socket);

    while (true) {
        memset(buffer, 0, BUF_SIZE);
        bytes = recv(client_socket, buffer, BUF_SIZE, 0);
        if (bytes <= 0) break;

        std::string msg(buffer);
        if (msg == "/exit\n") break;
        if (msg == "/list\n") {
            send_user_list(client_socket);
            continue;
        }
        broadcast(name + ": " + msg, client_socket);
    }
    pthread_mutex_lock(&clients_mutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->socket == client_socket) {
            clients.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    broadcast(name + " left\n", client_socket);
    close(client_socket);
    pthread_exit(nullptr);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);
    std::cout << "Server started\n";
    while (true) {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);

        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &len);
        pthread_t tid;
        int* p = new int(client_socket);
        pthread_create(&tid, nullptr, handle_client, p);
        pthread_detach(tid);
    }
}

