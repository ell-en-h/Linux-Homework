#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>

void do_command(char** argv)
{
    auto start = std::chrono::steady_clock::now();

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return; }

    if (pid == 0) {
        if (execvp(argv[0], argv) == -1) {
            perror("execvp");
            exit(1); }
    }
    else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return; }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        if (WIFEXITED(status))
            std::cout << "\nCommand completed with " << WEXITSTATUS(status)
                      << " exit code and took " << elapsed.count() << " seconds.\n";
        else
            std::cout << "\nCommand terminated abnormally.\n"; }
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: ./do-command <command> [args...]\n";
        return 1; }

    do_command(&argv[1]);

    return 0;}
