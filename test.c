#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd = open("input.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }
    int bytes_read = 0;
    int count = 0, size = 128;
    char buf[100];
    char *text = malloc(size);
    if (text == NULL) {
        perror("Error allocating memory");
        exit(1);
    }
    while(1) {

    bytes_read = read(fd, buf, 100);
    if (bytes_read == -1) {
        perror("Error reading file");
        exit(1);
    }
    if (bytes_read == 0) {
        break; // End of file
    }
    if(count + bytes_read > size) {
        size *= 2;
        text = realloc(text, size);
        if (text == NULL) {
            perror("Error reallocating memory");
            exit(1);
        }

    }
    strncpy(text + count, buf, bytes_read);
    count += bytes_read;
    
    
    }
    write(1, text, count);
    close(fd);
    free(text);
    return 0;
}
