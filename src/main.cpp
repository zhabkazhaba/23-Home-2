#include <iostream>
#include "Matrix.hpp"

using namespace std;

int main() {
    cout << boolalpha;
    Matrix Mat1(3, 3);
    Mat1.initialize();
    Matrix Mat2(3, 3);
    Mat2.initialize();
    Mat1.readFromFile();
    Mat1.print();

    if (Mat1 == 3.0) {
        cout << true << endl;
    } else {
        cout << false << endl;
    }

    Mat1 + Mat2;
    Mat1.print();

    cout << Mat1.getDeterminant() << endl;
    !Mat1;
    Mat1.print();
    Mat1.writeToFile();
    return 0;
}
