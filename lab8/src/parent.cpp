#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <semaphore.h>
#include <string>
#include <stdexcept>
#include <fstream> 
#include "parent.hpp"

void ParentRoutine(const std::string& fileName, std::ostream& output) {
    // Открываем файл на чтение
    int fileFd = open(fileName.c_str(), O_RDONLY);
    if (fileFd < 0) {
        throw std::runtime_error("Не удалось открыть файл");
    }

    struct stat fileStat {};
    if (fstat(fileFd, &fileStat) == -1) {
        close(fileFd);
        throw std::runtime_error("Не удалось получить информацию о файле");
    }

    size_t fileSize = fileStat.st_size;

    if (fileSize == 0) {
        output << 0 << std::endl;
        close(fileFd);
        return;
    }

    // Создаем отображаемый файл
    int shmFd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    if (shmFd < 0) {
        close(fileFd);
        throw std::runtime_error("Ошибка создания отображаемого файла");
    }
    
    if (ftruncate(shmFd, fileSize) == -1) {
        close(fileFd);
        close(shmFd);
        throw std::runtime_error("Ошибка выделения памяти в отображаемом файле");
    }

    // Отображаем файл в память
    void* mappedMemory = mmap(nullptr, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (mappedMemory == MAP_FAILED) {
        close(fileFd);
        close(shmFd);
        throw std::runtime_error("Ошибка отображения файла в память");
    }

    // Читаем данные из файла
    if (read(fileFd, mappedMemory, fileSize) != static_cast<ssize_t>(fileSize)) {
        close(fileFd);
        close(shmFd);
        munmap(mappedMemory, fileSize);
        throw std::runtime_error("Ошибка чтения данных из файла");
    }

    close(fileFd);

    // Создаем и открываем семафор
    sem_t* sem = sem_open("/semaphore", O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        close(shmFd);
        munmap(mappedMemory, fileSize);
        throw std::runtime_error("Ошибка создания семафора");
    }

    // Создаем дочерний процесс
    pid_t pid = fork();
    if (pid < 0) {
        close(shmFd);
        munmap(mappedMemory, fileSize);
        sem_unlink("/semaphore");
        throw std::runtime_error("Ошибка создания дочернего процесса");
    }

    if (pid == 0) {
        // Дочерний процесс
        const char* pathToChild = getenv("PATH_TO_EXEC_CHILD");
        if (pathToChild == nullptr) {
            perror("Переменная PATH_TO_EXEC_CHILD не установлена");
            exit(1);
        }

        // Запуск strace с дочерним процессом
        execlp("strace", "strace", "-o", "trace.log", pathToChild, std::to_string(fileSize).c_str(), nullptr);
        perror("Ошибка запуска strace");
        exit(1);
    }

    // Ожидаем завершения дочернего процесса
    sem_wait(sem);

    // Читаем результат из памяти
    long int result = 0;
    memcpy(&result, mappedMemory, sizeof(result));

    output << result << std::endl;

    // Очистка
    close(shmFd);
    munmap(mappedMemory, fileSize);
    shm_unlink("/shared_memory");
    sem_unlink("/semaphore");

}