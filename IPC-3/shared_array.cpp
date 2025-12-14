#include "shared_array.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

shared_array::shared_array(const std::string& name, size_t size) : shm_name("/" + name), sem_name("/" + name + "_sem"),
    arr_size(size), data(nullptr), shm_fd(-1), sem(nullptr)
{
    if (size < 1 || size > 1000000000)
	throw std::runtime_error("wrong size");

    byte_size = arr_size * sizeof(int);
    shm_fd = shm_open(shm_name.c_str(), O_RDWR | O_CREAT | O_EXCL, 0666);
    if (shm_fd != -1) {
	if (ftruncate(shm_fd, byte_size) == -1) {
	    close(shm_fd);
            shm_unlink(shm_name.c_str());
	    throw std::runtime_error("ftruncate failed");
	}
    } else {
	shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
	if (shm_fd == -1)
	    throw std::runtime_error("shm_open failed");
}
void* map = mmap(nullptr, byte_size,
		PROT_READ | PROT_WRITE,
		MAP_SHARED, shm_fd, 0);
    if (map == MAP_FAILED) {
        close(shm_fd);
	throw std::runtime_error("mmap failed");
}

    data = static_cast<int*>(map);
    sem = sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
	munmap(data, byte_size);
	close(shm_fd);
	throw std::runtime_error("sem_open failed");
    }  
}
    shared_array::~shared_array()
{
    if (data)
	munmap(data, byte_size);
    if (shm_fd != -1)
	close(shm_fd);
    if (sem)
	sem_close(sem);
}

int& shared_array::operator[](size_t index)
{
    if (index >= arr_size)
	throw std::out_of_range("index out of range");
    return data[index];
}

size_t shared_array::size() const
{
    return arr_size;
}
