#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <csignal>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Неверное количество аргументов" << std::endl;
        return 1;
    }

    size_t fileSize = std::stoul(argv[1]);

    int shmFd = shm_open("/shared_memory", O_RDWR, 0666);
    if (shmFd < 0) {
        perror("Ошибка открытия отображаемого файла");
        return 1;
    }

    void* mappedMemory = mmap(nullptr, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (mappedMemory == MAP_FAILED) {
        close(shmFd);
        perror("Ошибка отображения файла в память");
        return 1;
    }

    char* data = static_cast<char*>(mappedMemory);
    long int sum = 0;
    char* endPtr = data + fileSize;

    while (data < endPtr) {
        long int number = std::strtol(data, &data, 10);
        sum += number;
    }

    // Записываем результат в начало памяти
    memcpy(mappedMemory, &sum, sizeof(sum));

    // Отправляем сигнал родительскому процессу
    kill(getppid(), SIGUSR1);

    // Очистка
    close(shmFd);
    munmap(mappedMemory, fileSize);

    return 0;
}
