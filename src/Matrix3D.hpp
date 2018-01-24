#ifndef MATRIX3D_HPP
#define MATRIX3D_HPP

#include <vector>

namespace sp
{
  template<typename domain>
  class Matrix
  {
    private:
      domain* data = 0;
      domain* tmp = 0;

    public:
      std::vector<int> sizes;

      Matrix() { }
      ~Matrix() { delete[] data; delete[] tmp; }

      template <class... Dim>
      Matrix(Dim... args)
      {
        sizes = std::vector<int>({args...});
        int count = 0;
        
        for(auto i : sizes)
          count *= i;

        data = new domain[count];
        tmp  = new domain[count];

        for(int i = 0; i < count; ++i)
        {
          data[i] = 0;
          tmp[i] = 0;
        }
      }

      Matrix3D(const Matrix3D<domain>& other)
      {
        data = new domain[other.x * other.y * other.z];
        tmp  = new domain[other.x * other.y * other.z];
        std::copy(other.data, other.data + (other.x * other.y * other.z), data);
        std::copy(other.tmp , other.tmp  + (other.x * other.y * other.z), tmp);

        x = other.x;
        y = other.y;
        z = other.z;
      }

      domain* operator()(int i, int j, int k) const { return data[i*y*z + j*z + k]; }

      Matrix3D<domain>& operator+=(const Matrix3D<domain>& b)
      {
        for(int i = 0; i < rows * columns; ++i)
          data[i] += *b(i);
        return *this;
      }

      Matrix<domain> operator+(const Matrix<domain>& b)
      {
        Matrix<domain> result = *this;
        result += b;
        return result;
      }

      Matrix<domain>& operator-=(const Matrix<domain>& b)
      {
        for(int i = 0; i < rows * columns; ++i)
          data[i] -= *b(i);
        return *this;
      }

      Matrix<domain> operator-(const Matrix<domain>& b)
      {
        Matrix<domain> result = *this;
        result -= b;
        return result;
      }

      Matrix<domain>& operator*=(const Matrix<domain>& b)
      {
        *this = *this * b;
        return *this;
      }

      Matrix<domain> operator*(const Matrix<domain>& b)
      {
        Matrix<domain> result(rows, b.columns);

        for(int i = 0; i < rows; ++i)
          for(int j = 0; j < b.columns; ++j)
            for(int k = 0; k < columns; ++k)
              result[i][j] += (data + (i * columns))[k] * b[k][j];

        return result;
      }

      // Use only when dim(A*B) = dim(A)
      void Multiplication(const Matrix<domain>& b)
      {
        domain* swap;
        for(int i = 0; i < rows; ++i)
          for(int j = 0; j < b.columns; ++j)
            for(int k = 0; k < columns; ++k)
              tmp[i * columns + j] += data[i * columns + k] * b[k][j];
        swap = data;
        data = tmp;
        tmp  = swap;
        for(int i = 0; i < rows * columns; ++i)
          tmp[i] = 0;
      }

      void Multiplication(const Matrix<domain>& a, const Matrix<domain>& b)
      {
        domain* swap;
        for(int i = 0; i < a.rows; ++i)
          for(int j = 0; j < b.columns; ++j)
            for(int k = 0; k < a.columns; ++k)
              tmp[i * b.columns + j] += a[i][k] * b[k][j];
        swap = data;
        data = tmp;
        tmp  = swap;
        for(int i = 0; i < rows * columns; ++i)
          tmp[i] = 0;
      }

      Matrix<domain> operator-() const
      {
        Matrix<domain> result(rows, columns);
        for(int i = 0; i < rows * columns; ++i)
          *result(i) = -data[i];
        return result;
      }

      Matrix<domain> operator!() const
      {
        Matrix<domain> result(columns, rows);
        for(int i = 0; i < columns; ++i)
          for(int j = 0; j < rows; ++j)
            result[i][j] = (data + (j * columns))[i];
        return result;
      }

      Matrix<domain>& operator=(const Matrix<domain>& other)
      {
        delete[] data;
        delete[] tmp;

        data = new domain[other.rows * other.columns];
        tmp  = new domain[other.rows * other.columns];
        std::copy(other.data, other.data + (other.rows * other.columns), data);
        std::copy(other.tmp , other.tmp  + (other.rows * other.columns), tmp);

        rows    = other.rows;
        columns = other.columns;

        return *this;
      }

      void Copy(const std::vector<domain>& m)
      {
        columns = m.size();
        rows    = 1;
        for(int i = 0; i < m.size(); ++i)
        {
          data[i] = m[i];
          tmp[i] = 0;
        }
      }

      void operator()(const Matrix<domain>& other)
      {
        std::copy(other.data, other.data + (other.rows * other.columns), data);
      }

      Matrix<domain>& operator()(domain (*f)(domain))
      {
        for(int i = 0; i < rows * columns; ++i)
          data[i] = f(data[i]);

        return *this;
      }

      Matrix<domain> Apply(domain (*f)(domain))
      {
        Matrix<domain> result = *this;
        result(f);

        return result;
      }

      std::vector<domain> ToVector()
      {
        return std::vector<domain>(data, data + rows * columns);
      }

      void ToVector(std::vector<domain>& t)
      {
        t.assign(data, data + (rows * columns));
      }
  };
}

#endif
