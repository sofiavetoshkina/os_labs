#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <string>
#include <stdexcept>
#include "parent.hpp"

// Глобальная переменная для обработки сигнала
volatile sig_atomic_t childDone = 0;

void SignalHandler(int) {
    childDone = 1;
}

void ParentRoutine(const std::string& fileName, std::ostream& output) {
    // Устанавливаем обработчик сигнала
    struct sigaction sa {};
    sa.sa_handler = SignalHandler;

    if (sigaction(SIGUSR1, &sa, nullptr) == -1) {
        throw std::runtime_error("Ошибка установки обработчика сигнала");
    }

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

    // Создаем дочерний процесс
    pid_t pid = fork();
    if (pid < 0) {
        close(shmFd);
        munmap(mappedMemory, fileSize);
        throw std::runtime_error("Ошибка создания дочернего процесса");
    }

    if (pid == 0) {
        // Дочерний процесс
        const char* pathToChild = getenv("PATH_TO_EXEC_CHILD");
        execl(pathToChild, pathToChild, std::to_string(fileSize).c_str(), nullptr);
        perror("Ошибка запуска дочернего процесса");
        exit(1);
    }

    while (!childDone) {
        pause(); // Ожидание сигнала
    }

    // Читаем результат из памяти
    long int result = 0;
    memcpy(&result, mappedMemory, sizeof(result));

    output << result << std::endl;

    // Очистка
    close(shmFd);
    munmap(mappedMemory, fileSize);
    shm_unlink("/shared_memory");
}
