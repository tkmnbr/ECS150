#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
//For read
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fileDescriptor;
    
    if(argc == 1){
        fileDescriptor = STDIN_FILENO;
    }
    else{
        for(int i = 1; i < argc; i++){
            fileDescriptor = open(argv[i], O_RDONLY);
            if(fileDescriptor == -1){
                // write error message to the file
                write(STDOUT_FILENO, "wcat: cannot open file\n", 23);
                return 1;
            }
            char buffer[4096];
            int ret;
            while ((ret = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
                write(STDOUT_FILENO, buffer, ret);
            }
            if(fileDescriptor != STDIN_FILENO){
                close(fileDescriptor);
            }
        }
    }
}