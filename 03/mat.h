#pragma once

#include <cstddef>

class Matrix
{
    class Vector
    {
      private:
        const Matrix &mat;
        int row;

      public:
        Vector(const Matrix &mat);

        void set_row(size_t row);
        int &operator[](size_t column) const;
    };

  private:
    int *buf;
    size_t rows;
    size_t cols;
    mutable Vector v;

  public:
    Matrix(size_t rows, size_t cols);

    Vector &operator[](size_t n) const;
    Matrix &operator*=(int value);
    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;

    int getRows();
    int getColumns();

    ~Matrix();
};