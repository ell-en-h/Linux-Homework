#include <iostream>
#include <cstdlib> // для atoi

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: ./a.out a b c" << endl;
        return 1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int c = atoi(argv[3]);

    int maxVal = a;
    int minVal = a;

    if (b > maxVal) maxVal = b;
    if (c > maxVal) maxVal = c;

    if (b < minVal) minVal = b;
    if (c < minVal) minVal = c;

    cout << "min - " << minVal << ", max - " << maxVal << endl;
    return 0;
}
