#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sstream>

int main(int argc, char *argv[]){
    if(argc < 2){
        write(STDOUT_FILENO, "wzip: file1 [file2 ...]\n", 24);
        return 1;
    }
    char prevChar = '\0';
    int count = 0;

    for(int i = 1; i < argc; i++){
        int fileDescriptor = open(argv[i], O_RDONLY);
        if(fileDescriptor == -1){
            write(STDOUT_FILENO, "wzip: cannot open file\n", 24);
            return 1;
        }

        // Read from file
        char buffer[4096];
        int ret;
        while((ret = read(fileDescriptor, buffer, sizeof(buffer))) > 0){
            for(int j = 0; j < ret; j++){
                if(buffer[j] == prevChar){
                    count++;
                } else {
                    if(count > 0){
                        write(STDOUT_FILENO, &count, sizeof(count));
                        write(STDOUT_FILENO, &prevChar, sizeof(prevChar));
                    }
                    prevChar = buffer[j];
                    count = 1;
                }
            }
        }
        close(fileDescriptor);
    }

    // Write the last character and its count
    if(count > 0){
        write(STDOUT_FILENO, &count, sizeof(count));
        write(STDOUT_FILENO, &prevChar, sizeof(prevChar));
    }
}