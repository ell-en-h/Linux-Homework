#include <fcntl.h>      
#include <unistd.h>     
#include <stdio.h>      
#include <stdlib.h>     

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(STDERR_FILENO, "Usage: ./read-file <filename>\n", 30);
        exit(EXIT_FAILURE);
    }

    const char *filepath = argv[1];

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Ошибка при записи в stdout");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        perror("Ошибка при чтении файла");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}
