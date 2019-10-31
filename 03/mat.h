#pragma once

#include <cstddef>

class Matrix
{
    class Vector
    {
      private:
        const Matrix *mat;
        int row;
        int &get(int index) const;

      public:
        Vector(const Matrix *mat, size_t row);
        const int &operator[](size_t column) const;
        int &operator[](size_t column);
    };

  private:
    int *buf;
    size_t rows;
    size_t cols;
    Matrix::Vector getRow(size_t row) const;

  public:
    Matrix(size_t rows, size_t cols);
    Matrix(Matrix&) = delete;
    Matrix& operator=(Matrix&) = delete;

    const Vector operator[](size_t n) const;
    Vector operator[](size_t n);
    Matrix &operator*=(int value);
    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;

    int getRows() const;
    int getColumns() const;

    ~Matrix();
};
