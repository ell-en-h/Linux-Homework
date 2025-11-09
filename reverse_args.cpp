#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./a.out arg1 arg2 ..." << endl;
        return 1;
    }

    for (int i = argc - 1; i > 0; --i) {
        cout << argv[i];
        if (i > 1) cout << " ";
    }
    cout << endl;

    return 0;
}
