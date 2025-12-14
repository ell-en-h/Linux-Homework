#include "shared_array.h"
#include <iostream>
#include <unistd.h>

int main()
{
    shared_array arr("demo_array", 10);
    while (true) {
	std::cout << "[second] arr[0] = " << arr[0] << std::endl;
	sleep(1);
    }
}
