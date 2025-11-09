#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        write(STDERR_FILENO, "Usage: ./simple-copy <source> <destination>\n", 44);
        exit(EXIT_FAILURE);
    }

    const char *source_path = argv[1];
    const char *dest_path = argv[2];

    int src_fd = open(source_path, O_RDONLY);
    if (src_fd == -1) {
        perror("Ошибка при открытии исходного файла");
        exit(EXIT_FAILURE);
    }

    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (dest_fd == -1) {
        perror("Ошибка при открытии файла назначения");
        close(src_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Ошибка при записи в файл назначения");
            close(src_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        perror("Ошибка при чтении исходного файла");
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}
