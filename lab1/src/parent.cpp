#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include "parent.hpp"

void ParentRoutine(std::string& fileName) {
    int pipe1[2];
    
    if (pipe(pipe1) == -1) {
        perror("Pipe error");
        return;
    }

    pid_t pid = fork();

    switch (pid) {

        case -1: {
            perror("Fork error");
            return;
        }

        case 0: {
            // Дочерний процесс
            close(pipe1[0]); // Закрываем конец канала для чтения

            // Получение дескриптора файла
            int fileFd = open(fileName.c_str(), O_RDONLY);
            if (fileFd < 0) {
                perror("Open file error");
                return;
            }

            // Стандартный поток ввода переопределен открытым файлом
            if (dup2(fileFd, STDIN_FILENO) == -1) {
                perror("dup2 error");
                close(fileFd);
                return;
            }

            // Стандартный поток вывода перенаправляется в pipe1 на запись
            if (dup2(pipe1[1], STDOUT_FILENO) == -1) {
                perror("dup2 error");
                close(pipe1[1]);
                return;
            }

            const char* pathToChild = getenv("PATH_TO_EXEC_CHILD");
            if (pathToChild == nullptr) {
                perror("Path error");
                return;
            }
            // Запуск дочерней программы
            execl(pathToChild, pathToChild, nullptr);
            break;
        }

        default: {
            // Родительский процесс
            close(pipe1[1]); // Закрываем конец канала для записи

            //Читаем данные и выводим в стандартный поток вывода
            char buffer[128];
            size_t bytes = read(pipe1[0], buffer, sizeof(buffer));
            if (bytes > 0){
                std::cout << buffer;
            } else {
                perror("Read error");
                return;
            }

            close(pipe1[0]); // Закрываем для чтения

            // Ожидаем завершения дочернего процесса
            wait(nullptr);
        }
    }
    
}