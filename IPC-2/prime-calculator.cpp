#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>

bool isPrime(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

int nthPrime(int n) {
    int count = 0;
    int num = 1;
    while (count < n) {
        num++;
        if (isPrime(num)) count++;
    }
    return num;
}

int main() {
    int toChild[2];   
    int toParent[2]; 

    if (pipe(toChild) == -1 || pipe(toParent) == -1) {
        std::cerr << "Failed to create pipes\n";
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork\n";
        return 1;
    }

    if (pid == 0) {
        close(toChild[1]);   
        close(toParent[0]); 

        char buffer[64];

        while (true) {
            int r = read(toChild[0], buffer, sizeof(buffer) - 1);
            if (r == -1) {
                std::cerr << "Child: read failed\n";
                exit(1);
            }
            if (r == 0) break; 

            buffer[r] = '\0';
            int n = std::atoi(buffer);

            std::cout << "[Child] Calculating " << n << "-th prime number...\n";

            int result = nthPrime(n);

            std::cout << "[Child] Sending calculation result of prime("
                      << n << ")...\n";

            char out[64];
            int len = snprintf(out, sizeof(out), "%d", result);

            if (write(toParent[1], out, len) == -1) {
                std::cerr << "Child: write failed\n";
                exit(1);
            }
        }

        close(toChild[0]);
        close(toParent[1]);
        return 0;
    }

    close(toChild[0]);
    close(toParent[1]);

    std::string input;
    char buffer[64];

    while (true) {
        std::cout << "[Parent] Please enter the number: ";
        std::cin >> input;

        if (input == "exit") {
            close(toChild[1]);
            close(toParent[0]);
            wait(nullptr);
            break;
        }

        bool ok = true;
        for (char c : input)
            if (!isdigit(c)) ok = false;

        if (!ok || std::stoi(input) <= 0) {
            std::cout << "Invalid number\n";
            continue;
        }

        std::cout << "[Parent] Sending " << input << " to the child process...\n";
        std::cout << "[Parent] Waiting for the response from the child process...\n";

        if (write(toChild[1], input.c_str(), input.size()) == -1) {
            std::cerr << "Parent: write failed\n";
            return 1;
        }

        int r = read(toParent[0], buffer, sizeof(buffer) - 1);
        if (r == -1) {
            std::cerr << "Parent: read failed\n";
            return 1;
        }

        buffer[r] = '\0';
        int result = std::atoi(buffer);

        std::cout << "[Parent] Received calculation result of prime("
                  << input << ") = " << result << "...\n\n";
    }

    return 0;
}
