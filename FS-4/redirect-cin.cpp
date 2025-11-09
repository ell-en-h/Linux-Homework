#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void initialize(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./redirect-cin <input-file>\n";
        exit(1); }
    const char* path = argv[1];
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1); }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        exit(1); }

    close(fd);
}

int main(int argc, char** argv)
{
    initialize(argc, argv);

    std::string input;
    std::cin >> input;

    std::string reversed(input.rbegin(), input.rend());

    std::cout << reversed << std::endl;

    return 0;
}
