#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 128

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <file> <word>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filepath = argv[1];
    const char *word = argv[2];
    size_t word_len = strlen(word);

    if (word_len == 0) {
        fprintf(stderr, "Ошибка: искомое слово не должно быть пустым\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    ssize_t bytes_read = 0;
    off_t global_offset = 0; // Для отслеживания реального смещения в файле
    int matches = 0;

    // Длина "нахлёста" между блоками, чтобы слово не разорвалось пополам
    size_t overlap = word_len - 1;

    while (1) {
        bytes_read = read(fd, buf, BUF_SIZE);
        if (bytes_read == -1) {
            perror("Ошибка чтения файла");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (bytes_read == 0) {
            break; // Конец файла
        }

        // Ищем слово в пределах фактически прочитанных байтов (bytes_read)
        if ((size_t)bytes_read >= word_len) {
            for (size_t i = 0; i <= (size_t)bytes_read - word_len; ++i) {
                // memcmp безопасен: ему не нужен '\0' в конце
                if (memcmp(buf + i, word, word_len) == 0) {
                    printf("Найдено совпадение на смещении (байт): %lld\n", 
                           (long long)(global_offset + (off_t)i));
                    matches++;
                }
            }
        }

        // Если прочитан полный буфер, нужно откатить указатель назад на overlap байт,
        // чтобы проверить слово, которое могло начаться в конце этого буфера 
        // и закончиться в начале следующего.
        if (bytes_read == BUF_SIZE && overlap > 0) {
            if (lseek(fd, -((off_t)overlap), SEEK_CUR) == (off_t)-1) {
                perror("Ошибка lseek");
                close(fd);
                exit(EXIT_FAILURE);
            }
            // Фактически мы продвинулись вперед на (BUF_SIZE - overlap)
            global_offset += (bytes_read - overlap);
        } else {
            // Если прочитали меньше BUF_SIZE, это конец файла — откат не нужен
            global_offset += bytes_read;
        }
    }

    printf("Всего найдено совпадений: %d\n", matches);

    close(fd);
    return EXIT_SUCCESS;
}

// //myfind file.txt word

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <string.h>

// int main(int argc, char *argv[]) {
//     if(argc < 3) {
//         fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
//         exit(1);
//     }
//     int fd = open(argv[1], O_RDONLY);
//     if (fd == -1) {
//         perror("Error opening file");
//         exit(1);
//     }
//     int bytes_read = 0;
//     int count = 0, size = 128;
//     char buf[size];

//     while(1) {

//     bytes_read = read(fd, buf, size);
//     if (bytes_read == -1) {
//         perror("Error reading file");
//         exit(1);
//     }
//     if (bytes_read == 0) {
//         break; // End of file
//     }
//     for(int i = 0; i < size ; ++i){
//         if(strcmp(buf + count, argv[2]) == 0){
//             write(1, "Found: %s\n", strlen("Found: %s\n"));
            
//         }   
//     }
//     count -= strlen(argv[2]);
//     lseek(fd, -strlen(argv[2]), SEEK_CUR);


//     // count += bytes_read;
    
    
//     }
//     close(fd);
//     return 0;
// }
