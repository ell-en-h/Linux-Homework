#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(2, "Usage: ./hacker-rm <file-to-erase>\n", 35);
        return 1;
    }

    const char *path = argv[1];
    struct stat st;
    if (stat(path, &st) == -1) { perror("stat");
	    return 1; }
    if (!S_ISREG(st.st_mode)) { write(2, "Not a regular file\n", 19);
	    return 1; }

    int fd = open(path, O_WRONLY);
    if (fd == -1) { perror("open");
	    return 1; }

    static const size_t BUF = 4096;  
    char buf[BUF];
    memset(buf, 0, BUF);

    off_t remaining = st.st_size;
    while (remaining > 0) {
        ssize_t to_write;
        if (remaining >= (off_t)BUF) to_write = BUF;
        else to_write = (ssize_t)remaining;

        ssize_t w = write(fd, buf, to_write);
        if (w <= 0) { perror("write"); close(fd);
	       	return 1; }
        remaining -= w;
    }

    if (fsync(fd) == -1) perror("fsync");
    close(fd);

    if (unlink(path) == -1) { perror("unlink");
	    return 1; }
    return 0;
}
