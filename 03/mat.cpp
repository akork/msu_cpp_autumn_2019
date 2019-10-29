#include "mat.h"
#include <stdexcept>

int &Matrix::Vector::operator[](size_t col) const
{
    if (col >= mat->cols)
        throw std::out_of_range("column out of range");
    return mat->buf[row * mat->cols + col];
}

Matrix::Vector::Vector(const Matrix *mat, size_t row) : mat(mat), row(row)
{
}

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols)
{
    buf = new int[rows * cols];
}

const Matrix::Vector Matrix::operator[](size_t row) const
{
    if (row >= rows)
        throw std::out_of_range("row out of range");
    return Vector(this, row);
}

Matrix &Matrix::operator*=(int value)
{
    for (int i = 0; i < rows * cols; ++i) {
        buf[i] *= value;
    }
    return *this;
}

bool Matrix::operator==(const Matrix &other) const
{
    if (this == &other)
        return true;

    if (rows != other.rows || cols != other.cols)
        return false;

    for (int i = 0; i < rows * cols; ++i) {
        if (buf[i] != other.buf[i])
            return false;
    }
    return true;
}

int Matrix::getRows()
{
    return rows;
}

int Matrix::getColumns()
{
    return cols;
}

Matrix::~Matrix()
{
    delete[] buf;
}

bool Matrix::operator!=(const Matrix &other) const
{
    return !this->operator==(other);
}
