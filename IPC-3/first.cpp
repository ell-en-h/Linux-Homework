#include "shared_array.h"
#include <iostream>
#include <unistd.h>

int main()
{
    shared_array arr("demo_array", 10);
    int i = 0;
    while (true) {
   	 arr[0] = i;
   	 std::cout << "[first] arr[0] = " << i << std::endl;
   	 i++;
    	 sleep(1);
    }
}
