#include <iostream>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char *argv[]){
    if(argc < 2){
        write(STDOUT_FILENO, "wunzip: file1 [file2 ...]\n", 26);
        return 1;
    }
    for(int i = 1; i < argc; i++){
        int fileDescriptor = open(argv[i], O_RDONLY);
        if(fileDescriptor  == -1){
            write(STDOUT_FILENO, "wzip: cannot open file\n", 24);
            return 1;
        }
        char buffer[4096];
        int ret;
        int leftover = 0;
        char leftoverBuffer[5];

        while((ret = read(fileDescriptor, buffer + leftover, sizeof(buffer) - leftover)) > 0){
            int index = 0;
            ret += leftover;
            while(index + 5 <= ret){
            int count = *(int*)(buffer + index);
            char currentChar = buffer[index + 4];
            std::string output(count, currentChar);
            write(STDOUT_FILENO, output.c_str(), count);
            index += 5;
            }
            leftover = ret - index;
            if(leftover > 0){
            memcpy(leftoverBuffer, buffer + index, leftover);
            memcpy(buffer, leftoverBuffer, leftover);
            }
        }
        close(fileDescriptor);
    }
}