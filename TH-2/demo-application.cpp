#include <iostream>
#include "lib_sch.h"

int x = 0, y = 0, z = 0;

void workX(int n) {
    while (n--) x++;
    std::cout << "X = " << x << "\n";
}

void workY(int n) {
    for (int i = 0; i < n; ++i) y++;
    std::cout << "Y = " << y << "\n";
}

void workZ(int n) {
    int t = 0;
    while (t < n) {
        z++;
        t++;
    }
    std::cout << "Z = " << z << "\n";
}

int main() {
    parallel_scheduler scheduler(2); 

    using F = void(*)(int);
    F tasks[3] = { workX, workY, workZ };
    int args[3] = { 1000, 1000, 1000 };

    for (int i = 0; i < 3; i++)
        scheduler.run(tasks[i], args[i]); 

    return 0;
}

