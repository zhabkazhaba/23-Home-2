#ifndef CPP_HOMEWORK_1_MATRIX_HPP
#define CPP_HOMEWORK_1_MATRIX_HPP

#include <functional>

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
class Matrix {
 private:
    long int row_num;
    long int col_num;
    T** data;

 public:
    Matrix() : row_num(0), col_num(0), data(nullptr) {}
    Matrix(long int m, long int n) {
        row_num = m;
        col_num = n;
        data = new T*[row_num];
        for (unsigned int i = 0; i < row_num; ++i) {
            data[i] = new T[col_num];
            for (unsigned int j = 0; j < col_num; ++j) {
                data[i][j] = 0;
            }
        }
    }
    explicit Matrix(const char *filename) : row_num(0), col_num(0), data(nullptr) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Error: Couldn't open file");
        }
        std::string line;
        int i = 0;
        int row_num_tmp = 0;
        int col_num_tmp = 0;
        while (std::getline(file, line)) {
            if (i == 0) {
                std::stringstream ss(line);
                ss >> row_num_tmp;
                ss.ignore(1, ',');
                ss >> col_num_tmp;
                if (col_num_tmp > 0 && row_num_tmp > 0) {
                    row_num = row_num_tmp;
                    col_num = col_num_tmp;
                    data = new T*[row_num];
                    for (unsigned int l = 0; l < row_num; ++l) {
                        data[l] = new T[col_num];
                    }
                } else {
                    throw std::out_of_range("Error: bad indexes");
                }
            } else {
                std::stringstream ss(line);
                for (unsigned int j = 0; j < col_num_tmp; j++) {
                    T value;
                    ss >> value;
                    ss.ignore(1, ',');
                    data[i-1][j] = value;
                }
            }
            i += 1;
        }
        file.close();
    }
    Matrix(const Matrix &second) {
        row_num = second.row_num;
        col_num = second.col_num;
        data = new T*[row_num];
        for (unsigned int i = 0; i < row_num; ++i) {
            data[i] = new T[col_num];
            if (data[i]) {
                for (unsigned int j = 0; j < col_num; ++j) {
                    data[i][j] = second.data[i][j];
                }
            } else {
                throw std::runtime_error("Error: Something bad happened when allocating memory");
            }
        }
    }
    Matrix(Matrix &&second) noexcept {
        row_num = second.row_num;
        col_num = second.col_num;
        data = second.data;
        second.data = nullptr;
    }
    ~Matrix() {
        for (unsigned int i = 0; i < row_num; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }

    /**
     * Initializes the matrix with values from the user input.
     */
    void initialize() {
        for (unsigned int i = 0; i < row_num; ++i) {
            for (unsigned int j = 0; j < col_num; ++j) {
                std::cin >> data[i][j];
            }
        }
    }

    static Matrix<T> id(long int n) {
        Matrix<T> result(n,n);
        for (unsigned int i = 0; i < n; ++i) {
            for (unsigned int j = 0; j < n; ++j) {
                if (i == j) {
                    result.data[i][j] = 1;
                } else {
                    result.data[i][j] = 0;
                }
            }
        }
        return result;
    }
    static Matrix<T> zero(long int m, long int n) {
        Matrix<T> result(m,n);
        for (unsigned int i = 0; i < m; ++i) {
            for (unsigned int j = 0; j < n; ++j) {
                result.data[i][j] = 0;
            }
        }
        return result;
    }

    __attribute__((unused)) void setRowNum(long int m) {
        if (m > 0) {
            row_num = m;
        } else {
            throw std::out_of_range("Error: Out of range while setting row size");
        }
    }
    __attribute__((unused)) void setColNum(long int n) {
        if (n > 0) {
            col_num = n;
        } else {
            throw std::out_of_range("Error: Out of range while setting col size");
        }
    }
    __attribute__((unused)) void setElement(long int i, long int j, T val) {
        if (i <= row_num && j <= col_num && i >= 0 && j >= 0) {
            data[i][j] = val;
        } else {
            throw std::out_of_range("Error: Out of range while setting element");
        }
    }

    __attribute__((unused)) long int getRowNum() const { return row_num; }
    __attribute__((unused)) long int getColNum() const { return col_num; }
    T getElement(long int i, long int j) const {
        if (i <= row_num && j <= col_num && i >= 0 && j >= 0) {
            return data[i][j];
        } else {
            throw std::out_of_range("Error: Out of range while getting element");
        }
    }
    /**
     * Gets the determinant of the matrix. If the matrix is not square, throws an exception.
     * @return Returns the determinant of the matrix.
     */
    T getDeterminant() {
        T sum = 0;
        if (row_num == col_num) {
            if (row_num == 1) {
                return data[0][0];
            } else if (row_num == 2) {
                return data[0][0] * data[1][1] - data[1][0] * data[0][1];
            } else {
                for (unsigned int j = 0; j < col_num; j++) {
                    sum += (j % 2 == 0 ? 1 : -1) * data[0][j] * makeMinor(0, j).getDeterminant();
                }
            }
        } else {
            throw std::runtime_error("Error: Matrix has to be square");
        }
        return sum;
    }

    void print() const {
        if (row_num > 0 && col_num > 0) {
            for (unsigned int i = 0; i < row_num; ++i) {
                std::cout << "[";
                for (unsigned int j = 0; j < col_num; ++j) {
                    std::cout << getElement(i, j) << " ";
                }
                std::cout << "]\n";
            }
        }
    }
    /**
     * Reads matrix from data.txt file.
     */

    void readFromFile() {
        Matrix<T> tmp("data.txt");
        *this = tmp;
    }
    void writeToFile() {
        std::ofstream file("data.txt");
        if (file.is_open()) {
            file << row_num << "," << col_num << "," << std::endl;
            for (unsigned int i = 0; i < row_num; i++) {
                for (unsigned int j = 0; j < col_num; j++) {
                    file << data[i][j] << ",";
                }
                file << std::endl;
            }
            file.close();
        } else {
            throw std::runtime_error("Error: Couldn't open file");
        }
    }

    void operator=(const Matrix &second) { // NOLINT
        if (this != &second && this->data != nullptr) {
            for (unsigned int i = 0; i < row_num; ++i)
                delete[] data[i];
            delete[] data;
            row_num = second.row_num;
            col_num = second.col_num;
            data = new T *[row_num];
            for (unsigned int i = 0; i < row_num; ++i) {
                data[i] = new T[col_num];
                if (data[i]) {
                    for (unsigned int j = 0; j < col_num; ++j) {
                        data[i][j] = second.data[i][j];
                    }
                } else {
                    throw std::runtime_error("Error: Something bad happened when allocating memory");
                }
            }
        }
    }
    void operator+(const Matrix &second) const {
        if (row_num == second.row_num and col_num == second.col_num) {
            for (unsigned int i = 0; i < row_num; ++i) {
                for (unsigned int j = 0; j < col_num; ++j) {
                    data[i][j] = data[i][j] + second.data[i][j];
                }
            }
        } else {
            std::cerr << "Error: Matrices have to be same size\n";
        }
    }
    void operator-(const Matrix &second) const {
        if (row_num == second.row_num and col_num == second.col_num) {
            for (unsigned int i = 0; i < row_num; ++i) {
                for (unsigned int j = 0; j < col_num; ++j) {
                    data[i][j] = data[i][j] - second.data[i][j];
                }
            }
        } else {
            std::cerr << "Error: Matrices have to be same size\n";
        }
    }
    void operator*(const Matrix &second) const {
        double sum;
        if (col_num == second.row_num) {
            for (unsigned int i = 0; i < row_num; i++) {
                for (unsigned int j = 0; j < second.col_num; ++j) {
                    sum = 0;
                    for (unsigned int k = 0; k < col_num; ++k) {
                        sum += data[i][k] * second.data[k][j];
                    }
                    data[i][j] = sum;
                }
            }
        } else {
            std::cerr << "Error: Cols number should be same as rows number\n";
        }
    }
    void operator*(double factor) const {
        if (factor != 1.0) {
            for (unsigned int i = 0; i < row_num; i++) {
                for (unsigned int j = 0; j < col_num; j++) {
                    data[i][j] *= factor;
                }
            }
        }
    }
    void operator*=(const Matrix &second) const { *this * second; }
    void operator*=(double factor) const { if (factor != 1.0) *this * factor; }
    void operator+=(const Matrix &second) const { *this + second; }
    bool operator==(const Matrix &second) {
        bool flag = true;
        if (row_num == second.row_num && col_num == second.col_num) {
            for (unsigned int i = 0; i < row_num; i++) {
                for (unsigned int j = 0; j < col_num; j++) {
                    flag *= (data[i][j] == second.data[i][j]);
                }
            }
            return flag;
        }
        return false;
    }
    bool operator!=(const Matrix &second) { return !(*this == second); } // NOLINT
    bool operator==(double scalar) {
        bool flag = true;
        double num = data[0][0];
        if (row_num == col_num) {
            for (unsigned int i = 0; i < row_num; i++) {
                for (unsigned int j = 0; j < col_num; j++) {
                    if (i == j)
                        flag *= (data[i][j] == num && data[i][j] == scalar);
                    else {
                        flag *= (data[i][j] == 0);
                    }
                }
            }
        }
        return flag;
    }
    bool operator!=(double scalar) { return !(*this == scalar); } // NOLINT
    /**
     * Calculates the inverse of the matrix using the adjoint matrix and the determinant.
     */
    void operator!() {
        Matrix Inverse(row_num, col_num);
        double det = this->getDeterminant();
        if (det != 0.00) {
            Inverse = this->makeAdjoint();
            for (unsigned int i = 0; i < row_num; ++i) {
                for (unsigned int j = 0; j < col_num; ++j) {
                    this->data[i][j] = Inverse.data[i][j] / det;
                }
            }
        } else {
            std::cerr << "Inverse matrix doesn't exist.\n";
        }
    }

    /**
     * Performs first type elementary conversion on the matrix.
     * @param i1 Index of the first row.
     * @param i2 Index of the second row.
     */
    __attribute__((unused)) void ElementaryConversion1(long int i1, long int i2) {
        if (i1 <= row_num && i2 <= row_num && i1 > 0 && i2 > 0)
            this->swapLines(i1, i2);
        else {
            throw std::out_of_range("Error: Out of bounds while doing elementary conversion 1");
        }
    }
    /**
     * Performs second type elementary conversion on the matrix.
     * @param i Index of the row.
     * @param multiplier Multiplier.
     */
    __attribute__((unused)) void ElementaryConversion2(long int i, double multiplier) {
        if (i <= row_num) {
            for (unsigned int j = 0; j <= col_num; j++) {
                data[i][j] *= multiplier;
            }
        } else {
            throw std::out_of_range("Error: Out of bounds while doing elementary conversion 2");
        }
    }
    /**
     * Performs third type elementary conversion on the matrix.
     * @param i1 Index of the first row.
     * @param i2 Index of the second row.
     * @param multiplier Multiplier.
     */
    __attribute__((unused)) void ElementaryConversion3(long int i1, long int i2, double multiplier) {
        if (i1 <= row_num && i2 <= row_num && i1 > 0 && i2 > 0) {
            for (unsigned int j = 0; j < col_num; j++) {
                data[i1][j] += multiplier * data[i2][j];
            }
        } else {
            throw std::out_of_range("Error: Out of bounds while doing elementary conversion 3");
        }
    }
 private:
    void symmetricSwapElements(long int i, long int j) {
        double temp = data[i][j];
        data[i][j] = data[j][i];
        data[j][i] = temp;
    }
    void swapLines(long int i1, long int i2) {
        double *temp = data[i1];
        data[i1] = data[i2];
        data[i2] = temp;
    }

    /**
     * Transforms one-dimensional array into two-dimensional matrix.
     * @param a Array representing matrix.
     */
    void twodify(const double *a) {
        unsigned int k = 0;
        for (unsigned int i = 0; i < row_num; ++i) {
            for (unsigned int j = 0; j < col_num; ++j) {
                data[i][j] = a[k];
                k += 1;
            }
        }
    }

    void transpose() {
        if (row_num == col_num) {
            for (unsigned int i = 0; i < row_num; ++i) {
                for (unsigned int j = i; j < col_num; ++j) {
                    if (j != i)
                        this->symmetricSwapElements(i, j);
                }
            }
        } else {
            std::cerr << "Error: Transposing not square matrices is not supported\n";
        }
    }

    /**
     * @return Returns adjoint matrix of the current matrix.
     */
    Matrix makeAdjoint() {
        Matrix Adjoint(row_num, col_num);
        for (unsigned int i = 0; i < row_num; ++i) {
            for (unsigned int j = 0; j < col_num; ++j) {
                Adjoint.data[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * makeMinor(i, j).getDeterminant();
            }
        }
        Adjoint.transpose();
        return Adjoint;
    }
    Matrix makeMinor(long int i, long int j) {
        int k = 0;
        Matrix Minor(row_num - 1, col_num - 1);
        double data_1d[Minor.row_num * Minor.col_num];
        for (unsigned int sub_i = 0; sub_i <= Minor.row_num; ++sub_i) {
            for (unsigned int sub_j = 0; sub_j <= Minor.col_num; ++sub_j) {
                if (sub_i != i && sub_j != j) {
                    data_1d[k] = data[sub_i][sub_j];
                    k += 1;
                }
            }
        }
        Minor.twodify(data_1d);
        return Minor;
    }
};


#endif // CPP_HOMEWORK_1_MATRIX_HPP
