#include <string>
#include <semaphore.h>
#include <cstddef>

class shared_array {
public:
    shared_array(const std::string& name, size_t size);
    ~shared_array();
    int& operator[](size_t index);
    size_t size() const;

private:
    std::string shm_name;
    std::string sem_name;
    size_t arr_size;
    size_t byte_size;

    int* data;
    int shm_fd;
    sem_t* sem;
};
