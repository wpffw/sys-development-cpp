#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>

void detach_process(const char* command);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }

    detach_process(argv[1]);

    return 0;
}

void detach_process(const char* command) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Error: fork failed." << std::endl;
        exit(1);
    }

    if (pid > 0) {
        exit(0);
    }

    // отвязываемся от терминала
    if (setsid() < 0) { 
        std::cerr << "Error: setsid failed." << std::endl;
        exit(1);
    }

    // Перенаправляем стандартный ввод, вывод и ошибки в /dev/null
    int dev_null = open("/dev/null", O_RDWR);
    if (dev_null < 0) {
        std::cerr << "Error: open /dev/null failed." << std::endl;
        exit(1);
    }

    dup2(dev_null, STDIN_FILENO);
    dup2(dev_null, STDOUT_FILENO);
    dup2(dev_null, STDERR_FILENO);
    close(dev_null);

    // int log_file = open("process_log.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
    // if (log_file < 0) {
    //     std::cerr << "Error: open log file failed." << std::endl;
    //     exit(1);
    // }

    // dup2(log_file, STDIN_FILENO);
    // dup2(log_file, STDOUT_FILENO);
    // dup2(log_file, STDERR_FILENO);
    // close(log_file);
    

    execlp(command, command, (char*)nullptr);

    std::cerr << "Error: exec failed." << std::endl;

    exit(1);
}


