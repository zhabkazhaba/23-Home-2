#ifndef CPP_HOMEWORK_1_MATRIX_HPP
#define CPP_HOMEWORK_1_MATRIX_HPP


#include <iostream>
#include <fstream>
#include <cstring>

class Matrix {
private:
    long int row_num;
    long int col_num;
    double **data;
public:
    Matrix() : Matrix(0, 0) {};
    Matrix(long int m, long int n) {
        row_num = m;
        col_num = n;
        data = new double *[row_num];
        for (unsigned int i = 0; i < row_num; ++i) {
            data[i] = new double[col_num];
            for (unsigned int j = 0; j < col_num; ++j) {
                data[i][j] = 0;
            }
        }
    }
    ~Matrix() {
        for (unsigned int i = 0; i < row_num; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }

    void initialize() {
        for (unsigned int i = 0; i < row_num; ++i) {
            for (unsigned int j = 0; j < col_num; ++j) {
                std::cin >> data[i][j];
            }
        }
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
    __attribute__((unused)) void setElement(long int i, long int j, double val) {
        if (i <= row_num && j <= col_num && i >= 0 && j >= 0) {
            data[i][j] = val;
        } else {
            throw std::out_of_range("Error: Out of range while setting element");
        }
    }

    __attribute__((unused)) long int getRowNum() const { return row_num; }
    __attribute__((unused)) long int getColNum() const { return col_num; }
    double getElement(long int i, long int j) const {
        if (i <= row_num && j <= col_num && i >= 0 && j >= 0) {
            return data[i][j];
        } else {
            throw std::out_of_range("Error: Out of range while getting element");
        }
    }
    double getDeterminant() {
        double sum = 0;
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

    // Интерфейс
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
     * Метод чтения из файла реализован без C++ <string> с помощью обработки каждой строки.
     * Обработка происходит напрямую с помощью сдвига указателя.
     */
    void readFromFile() {
        FILE *file = fopen("data.txt", "r");
        if (file != nullptr) {
            char line[100];
            char subline[100];
            int flag = 0;
            int i = 0;
            while (fgets(line, sizeof(line), file) != nullptr) {
                if (flag == 0) {
                    char *ptr = line;
                    while (*ptr != ',') {
                        *ptr++;
                    }
                    *ptr = '\0';
                    strcpy(subline, line);
                    row_num = atoi(subline); //NOLINT

                    ptr += 1;
                    strcpy(subline, ptr);
                    col_num = atoi(subline); //NOLINT

                    if (col_num > 0 && row_num > 0) {
                        Matrix temp(row_num, col_num);
                        *this = temp;
                        flag = 1;
                    } else {
                        throw std::out_of_range("Error: bad indexes");
                    }
                } else {
                    for (unsigned int j = 0; j < col_num; j++) {
                        char *ptr = line;
                        while (*ptr != ',') {
                            *ptr++;
                        }
                        strcpy(subline, line);
                        if (*subline) {
                            data[i][j] = atof(subline); //NOLINT - для исправления варнинга предлагает юзать stl!
                        }

                        if (*ptr == ',') {
                            ptr += 1;
                        }
                        strcpy(line, ptr);
                    }
                    i += 1;
                }
            }
            fclose(file);
        }
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

    // Операторы
    /**
     * Присваивание значений одной матрицы другой реализовано с помощью
     * полного удаления содержимого изначальной матрицы и повторного
     * создания двумерного массива нового размера с последующим
     * присвоением соответствующих значений
     * @param second Матрица, содержимое которой присваивается матрице, использующей оператор равенства
     */
    void operator=(const Matrix &second) { // NOLINT
        if (!((this == &second) || (row_num == second.row_num && col_num == second.col_num && data == second.data))) {
            for (unsigned int i = 0; i < row_num; ++i)
                delete[] data[i];
            delete[] data;
            row_num = second.row_num;
            col_num = second.col_num;
            data = new double *[row_num];
            for (unsigned int i = 0; i < row_num; ++i) {
                data[i] = new double[col_num];
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

    // В операторах присваивания реализованы проверки выхода за пределы матрицы
    void operator+(const Matrix &second) const {
        if (row_num == second.row_num and col_num == second.col_num) {
            for (unsigned int i = 0; i < row_num; ++i) {
                for (unsigned int j = 0; j < col_num; ++j) {
                    data[i][j] = data[i][j] + second.data[i][j];
                }
            }
        } else {
            std::cerr << "Error: Matrices have to be same size";
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

    // Операторы-ссылки на обычные операторы присваивания выше
    void operator*=(const Matrix &second) const { *this * second; }
    void operator*=(double factor) const { if (factor != 1.0) *this * factor; }
    void operator+=(const Matrix &second) const { *this + second; }

    // Операторы проверки равенства/неравенства матрицы и матрицы, матрицы и скаляра
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
    bool operator!=(const Matrix &second) { return !(*this == second); }
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
    bool operator!=(double scalar) { return !(*this == scalar); }

    /**
     * Нахождение обратной матрицы реализовано с использованием формулы:
     * A^-1 = adj(A) / det(A)
     * Сначала проверяется, равен ли определитель нулю, в случае, если он не равен,
     * выводится, что обратной матрицы не существует.
     * В обратном случае с помощью метода makeAdjoint строится присоединённая матрица,
     * далее выводится обратная матрица.
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
    // Элементарные преобразования
    __attribute__((unused)) void ElementaryConversion1(long int i1, long int i2) {
        if (i1 <= row_num && i2 <= row_num && i1 > 0 && i2 > 0)
            this->swapLines(i1, i2);
        else {
            throw std::out_of_range("Error: Out of bounds while doing elementary conversion 1");
        }
    }
    __attribute__((unused)) void ElementaryConversion2(long int i, double multiplier) {
        if (i <= row_num) {
            for (unsigned int j = 0; j <= col_num; j++) {
                data[i][j] *= multiplier;
            }
        } else {
            throw std::out_of_range("Error: Out of bounds while doing elementary conversion 2");
        }
    }
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
    // Служебные функции
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
     * Метод преобразует одномерное представление матрицы в двумерный массив
     * @param a Массив, являющийся одномерным представлением двумерной матрицы
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
    // Методы нахождения вспомогательных матриц
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


#endif //CPP_HOMEWORK_1_MATRIX_HPP
