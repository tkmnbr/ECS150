#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

void grepFromFileDescriptor(int fileDescriptor, const char *searchTerm) {
    char buffer[4096];
    int ret;
    std::string line;
    while ((ret = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < ret; i++) {
            if (buffer[i] == '\n') {
                line += buffer[i];
                if (line.find(searchTerm) != std::string::npos) {
                    write(STDOUT_FILENO, line.c_str(), line.size());
                }
                line.clear();
            } else {
                line += buffer[i];
            }
        }
    }
    if (!line.empty() && line.find(searchTerm) != std::string::npos) {
        write(STDOUT_FILENO, line.c_str(), line.size());
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(STDOUT_FILENO, "wgrep: searchterm [file ...]\n", 29);
        return 1;
    }
    const char *searchTerm = argv[1];
    if (argc == 2) {
        grepFromFileDescriptor(STDIN_FILENO, searchTerm);
    } else {
        for (int i = 2; i < argc; i++) {
            int fileDescriptor = open(argv[i], O_RDONLY);
            if (fileDescriptor == -1) {
                write(STDOUT_FILENO, "wgrep: cannot open file\n", 24);
                return 1;
            }
            grepFromFileDescriptor(fileDescriptor, searchTerm);
            close(fileDescriptor);
        }
    }
    return 0;
}
