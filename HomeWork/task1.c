// ./mycp file.txt file_copy.txt
// cp command 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    int fd_copy = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }
    int bytes_read = 0;
    int count = 0, size = 128;
    char buf[100];

    while(1) {

    bytes_read = read(fd, buf, 100);
    if (bytes_read == -1) {
        perror("Error reading file");
        exit(1);
    }
    if (bytes_read == 0) {
        break; // End of file
    }
    write(fd_copy, buf, bytes_read); 
    count += bytes_read;
    }
    close(fd);
    close(fd_copy);
    return 0;
}
