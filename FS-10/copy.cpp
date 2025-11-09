#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    if (argc != 3) {
        write(2, "Usage: ./copy <source-file> <destination-file>\n", 47);
        return 1; }

    const char* src_path = argv[1];
    const char* dst_path = argv[2];

    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) { perror("open source");
	    return 1; }

    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd == -1) { perror("open destination"); close(src_fd);
	    return 1; }

    char buf[4096]; 
    ssize_t bytes;
    off_t total = 0;
    off_t data_bytes = 0;
    off_t hole_bytes = 0;

    while ((bytes = read(src_fd, buf, sizeof(buf))) > 0) {
        int all_zero = 1;
        for (ssize_t i = 0; i < bytes; i++) {
            if (buf[i] != 0) {
                all_zero = 0;
                break; }
        }
        if (all_zero) {
            if (lseek(dst_fd, bytes, SEEK_CUR) == -1) {
                perror("lseek");
                close(src_fd);
                close(dst_fd);
                return 1; }
            hole_bytes += bytes; }
        else {
            if (write(dst_fd, buf, bytes) != bytes) {
                perror("write");
                close(src_fd);
                close(dst_fd);
                return 1; }
            data_bytes += bytes; }

        total += bytes; }
    if (bytes == -1) perror("read");

    close(src_fd);
    close(dst_fd);

    printf("Successfully copied %lld bytes (data: %lld, hole: %lld).\n",
           (long long)total, (long long)data_bytes, (long long)hole_bytes);

    return 0;
}
