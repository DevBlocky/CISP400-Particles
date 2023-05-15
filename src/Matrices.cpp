#include "Matrices.h"

using namespace std;

namespace Matrices {
    Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols) {
        // create the empty matrix in memory
        for (int r = 0; r < rows; r++)
            a.emplace_back(std::move(vector<double>(cols)));
    }

    Matrix operator+(const Matrix &a, const Matrix &b) {
        // assert sizes are the same
        bool valid = a.getRows() == b.getRows() && a.getCols() == b.getCols();
        if (!valid) throw runtime_error("Error: dimensions must agree");

        // loop through all values and add them together
        int rows = a.getRows(), cols = a.getCols();
        Matrix out(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                out(i, j) = a(i, j) + b(i, j);
            }
        }
        return out;
    }

    Matrix operator*(const Matrix &a, const Matrix &b) {
        bool valid = a.getCols() == b.getRows();
        if (!valid) throw runtime_error("Error: dimensions must agree");

        int rows = a.getRows(), cols = b.getCols();
        Matrix out(rows, cols);
        for (int outI = 0; outI < rows; outI++) {
            for (int outJ = 0; outJ < cols; outJ++) {
                // perform the matrix multiplication by multiplying
                // the row from a with the col from b and summing
                //
                // outI corresponds to a's row
                // outJ corresponds to b's col
                double sum = 0;
                for (int i = 0; i < a.getCols(); i++)
                    sum += a(outI, i) * b(i, outJ);

                // set the output value to the sum
                out(outI, outJ) = sum;
            }
        }
        return out;
    }

    bool operator==(const Matrix &a, const Matrix &b) {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) return false;

        for (int i = 0; i < a.getRows(); i++) {
            for (int j = 0; j < a.getCols(); j++) {
                if (std::abs(a(i, j) - b(i, j)) > 0.001) return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix &a, const Matrix &b) {
        return !(a == b);
    }

    ostream &operator<<(ostream &os, const Matrix &a) {
        for (int i = 0; i < a.getRows(); i++) {
            for (int j = 0; j < a.getCols(); j++) {
                os << setw(10) << a(i, j);
                if (j != a.getCols() - 1) os << setw(0) << ' ';
            }
            if (i != a.getRows() - 1) os << "\n\n";
        }
        return os;
    }

    RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2) {
        double csin = sin(theta);
        double ccos = cos(theta);

        this->operator()(0, 0) = ccos;
        this->operator()(0, 1) = -csin;
        this->operator()(1, 0) = csin;
        this->operator()(1, 1) = ccos;
    }

    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) {
        this->operator()(0, 0) = scale;
        this->operator()(0, 1) = 0;
        this->operator()(1, 0) = 0;
        this->operator()(1, 1) = scale;
    }

    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols) {
        for (int col = 0; col < nCols; col++) {
            this->operator()(0, col) = xShift;
            this->operator()(1, col) = yShift;
        }
    }
}
