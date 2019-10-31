#include "mat.h"
#include <stdexcept>

int &Matrix::Vector::get(int col) const
{
    if (col >= mat->cols)
        throw std::out_of_range("column out of range");
    return mat->buf[row * mat->cols + col];
}

const int &Matrix::Vector::operator[](size_t col) const
{
    return get(col);
}

int &Matrix::Vector::operator[](size_t col)
{
    return get(col);
}

Matrix::Vector::Vector(const Matrix *mat, size_t row) : mat(mat), row(row)
{
}

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols)
{
    buf = new int[rows * cols];
}

Matrix::Vector Matrix::getRow(size_t row) const
{
    if (row >= rows)
        throw std::out_of_range("row out of range");
    return Matrix::Vector(this, row);
}

const Matrix::Vector Matrix::operator[](size_t row) const
{
    return getRow(row);
}

Matrix::Vector Matrix::operator[](size_t row)
{
    return getRow(row);
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

int Matrix::getRows() const
{
    return rows;
}

int Matrix::getColumns() const
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
