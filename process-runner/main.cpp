#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
    
    if (argc < 2){
        std::cerr << "Использование: " << argv[0] << " <команда> [аргументы...]" << std::endl;
        return 1;
    }

    freopen("out.txt", "w", stdout);
    freopen("err.txt", "w", stderr);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Ошибка при создании процесса." << std::endl;
        return 1;
    } else if (pid == 0) { // Дочерний процесс
        execvp(argv[1], &argv[1]);

        std::cerr << "Ошибка при выполнении команды: " << argv[1] << std::endl;
        return 1;

    } else { // Родительский процесс
        int status;
        waitpid(pid, &status, 0); 

        if (WIFEXITED(status)) {
            std::cout << std::endl << "--> Процесс завершился с кодом: " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cerr << std::endl << "--> Процесс завершился с ошибкой." << std::endl;
        }
    }

    return 0;
}