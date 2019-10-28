#include "mat.h"
#include <iostream>

Matrix::Vector::Vector(const Matrix &mat) : mat(mat)
{
}

void Matrix::Vector::set_row(size_t row)
{
    this->row = row;
}

int &Matrix::Vector::operator[](size_t col) const
{
    if (col >= mat.cols)
        throw std::out_of_range("column out of range");
    return mat.buf[row * mat.cols + col];
}

Matrix::Matrix(size_t rows, size_t cols)
    : rows(rows), cols(cols), v(Vector(*this))
{
    buf = static_cast<int *>(operator new(rows *cols));
}

Matrix::Vector &Matrix::operator[](size_t row) const
{
    if (row >= rows)
        throw std::out_of_range("row out of range");
    v.set_row(row);
    return v;
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
}

bool Matrix::operator!=(const Matrix &other) const
{
    return !this->operator==(other);
}
