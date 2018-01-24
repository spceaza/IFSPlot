#include <Rcpp.h>
#include <vector>
#include <math.h>
#include <limits>
#include "Matrix.hpp"

using namespace Rcpp;

sp::Matrix<double> ToMatrix(const NumericMatrix& m)
{
  sp::Matrix<double> result(m.nrow(), m.ncol());
  for(int i = 0; i < m.nrow(); ++i)
    for(int j = 0; j < m.ncol(); ++j)
      result[i][j] = m(i, j);
  return result;
}

std::vector<double> ToVector(const NumericVector& v)
{
  return std::vector<double>(v.begin(), v.end());
}

template<typename... Ts>
void test(Ts... args)
{
  std::vector<int> size = std::vector<int>({args...});
}

// [[Rcpp::export]]
NumericMatrix IFS(const List& transformation, const List& translation, const std::vector<int>& probability, const int& iterations, int x_res, int y_res)
{
  std::vector<sp::Matrix<double>>  _transformation;
  std::vector<sp::Matrix<double>>  _translation;
  NumericMatrix                    result(x_res, y_res);
  std::vector<int>                 rules_probability;
  sp::Matrix<double>               point(std::vector<double>({0, 0}));
  std::vector<double>              x(iterations);
  std::vector<double>              y(iterations);
  int index;
  double min_x =  std::numeric_limits<double>::infinity();
  double max_x = -std::numeric_limits<double>::infinity();
  double min_y =  std::numeric_limits<double>::infinity();
  double max_y = -std::numeric_limits<double>::infinity();

  for (int i = 0; i < transformation.length(); ++i)
    _transformation.push_back( ToMatrix( transformation(i) ) );

  for (int i = 0; i < translation.length(); ++i)
    _translation.push_back( sp::Matrix<double>( ToVector( translation(i) ) ) );

  for( int i = 0; i < probability.size(); ++i)
    for( int j = 0; j < probability[i]; ++j)
      rules_probability.push_back(i);

  for (int i = 0; i < iterations; i++)
  {
    index = rules_probability[rand() % 100];
    point.Multiplication(_transformation[index], point);
    point += _translation[index];
    x[i] = point[0][0];
    y[i] = point[1][0];
    if(x[i] > max_x) max_x = x[i];
    if(x[i] < min_x) min_x = x[i];
    if(y[i] > max_y) max_y = y[i];
    if(y[i] < min_y) min_y = y[i];
  }
  
  for(int i = 0; i < iterations; i++)
  {
    x[i] = (x[i] - min_x) * (x_res - 1) / (max_x - min_x);
    y[i] = (y[i] - min_y) * (y_res - 1) / (max_y - min_y);

    result(x[i], y[i]) = 1;
  }

  return result;
}
