/*
* Your program **wgrep** is always passed a search term and zero or
  more files to grep through (thus, more than one is possible). It should go
  through each line and see if the search term is in it; if so, the line
  should be printed, and if not, the line should be skipped.
* The matching is case sensitive. Thus, if searching for **foo**, lines
  with **Foo** will *not* match.
* Lines can be arbitrarily long (that is, you may see many many characters
  before you encounter a newline character, \\n). **wgrep** should work
  as expected even with very long lines. For this, you might want to write a
  function that reads from a file descriptor and buffers data until you reach
  a newline character.
* If **wgrep** is passed no command-line arguments, it should print
  "wgrep: searchterm [file ...]" (followed by a newline) and exit with
  status 1.  
* If **wgrep** encounters a file that it cannot open, it should print
  "wgrep: cannot open file" (followed by a newline) and exit with status 1. 
* In all other cases, **wgrep** should exit with return code 0.
* If a search term, but no file, is specified, **wgrep** should work,
  but instead of reading from a file, **wgrep** should read from
  *standard input*. Doing so is easy, because the file descriptor **STDIN_FILENO**
  is already open; you can use **read()** to read from it.
* For simplicity, if passed the empty string as a search string, **wgrep**
  can either match NO lines or match ALL lines, both are acceptable.
*/

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fileDescriptor;
    if(argc < 2){
        write(STDOUT_FILENO, "wgrep: searchterm [file ...]\n", 29);
        return 1;
    }
    const char *searchTerm = argv[1];
    if(argc == 2){
        fileDescriptor = STDIN_FILENO;
        char buffer[4096];
        int ret;
        std::string line;
        while((ret = read(fileDescriptor, buffer, sizeof(buffer))) > 0){
                for(int i = 0; i < ret; i++){
                    if(buffer[i] == '\n'){
                        line += buffer[i];
                        if(line.find(searchTerm) != std::string::npos){
                            write(STDOUT_FILENO, line.c_str(), line.size());
                        }
                        line.clear();
                    } else {
                        line += buffer[i];
                    }
                }
            }
            if(!line.empty() && line.find(searchTerm) != std::string::npos){
                write(STDOUT_FILENO, line.c_str(), line.size());
            }
            if(fileDescriptor != STDIN_FILENO){
                close(fileDescriptor);
            }
    }
    else{
        for(int i = 2; i < argc; i++){
            fileDescriptor = open(argv[i], O_RDONLY);
            if(fileDescriptor == -1){
                write(STDOUT_FILENO, "wgrep: cannot open file\n", 24);
                return 1;
            }
            char buffer[4096];
            int ret;
            std::string line;
            // if there is a search term, but no specified file, read from standard input


            while((ret = read(fileDescriptor, buffer, sizeof(buffer))) > 0){
                for(int i = 0; i < ret; i++){
                    if(buffer[i] == '\n'){
                        line += buffer[i];
                        if(line.find(searchTerm) != std::string::npos){
                            write(STDOUT_FILENO, line.c_str(), line.size());
                        }
                        line.clear();
                    } else {
                        line += buffer[i];
                    }
                }
            }
            if(!line.empty() && line.find(searchTerm) != std::string::npos){
                write(STDOUT_FILENO, line.c_str(), line.size());
            }
            if(fileDescriptor != STDIN_FILENO){
                close(fileDescriptor);
            }
        }
    }
}