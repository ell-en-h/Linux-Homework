#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        write(2, "Usage: ./manual-append <file>\n", 30);
        return 1; }

    const char* path = argv[1];

    int fd1 = open(path, O_WRONLY | O_CREAT, 0644);
    if (fd1 == -1)
    {
        perror("open fd1");
        return 1; }

    int fd2 = dup(fd1);
    if (fd2 == -1)
    {
        perror("dup");
        close(fd1);
        return 1; }

    const char* line1 = "first line\n";
    const char* line2 = "second line\n";

    if (write(fd1, line1, strlen(line1)) == -1)
    {
        perror("write fd1");
        close(fd1);
        close(fd2);
        return 1; }

    if (write(fd2, line2, strlen(line2)) == -1)
    {
        perror("write fd2");
        close(fd1);
        close(fd2);
        return 1; }

    close(fd1);
    close(fd2);

    return 0;
}
