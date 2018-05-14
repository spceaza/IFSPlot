#define ARMA_64BIT_WORD 1
// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::plugins(openmp)]]
#ifdef _OPENMP
#include <omp.h>
#endif
#include <vector>
#include <math.h>
#include <limits>
#include <RcppArmadillo.h>
#include <Matrix.hpp>

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

// [[Rcpp::export]]
List IFS2D(const List& transformation, const List& translation, const std::vector<int>& probability, const int& iterations, const int pixels)
{
  std::vector<sp::Matrix<double>>  _transformation, _translation;
  std::vector<int>                 rules_probability;
  std::vector<sp::Matrix<double>>  point;
  std::vector<std::vector<double>> x, y;
  int index;

  for(int i = 0; i < 8; ++i)
  {
    x.push_back(std::vector<double>(iterations / 8));
    y.push_back(std::vector<double>(iterations / 8));
    point.push_back(std::vector<double>({0, 0}));
  }

  int p_count = 0;
  double min_x, min_y, max_x, max_y;
  max_x = max_y = -std::numeric_limits<double>::infinity();
  min_x = min_y =  std::numeric_limits<double>::infinity();
  int w, h;

  for (int i = 0; i < transformation.length(); ++i)
    _transformation.push_back( ToMatrix( transformation(i) ) );

  for (int i = 0; i < translation.length(); ++i)
    _translation.push_back( sp::Matrix<double>( ToVector( translation(i) ) ) );

  for( int i = 0; i < probability.size(); ++i)
  {
    p_count += probability[i];
    for( int j = 0; j < probability[i]; ++j)
      rules_probability.push_back(i);
  }

  #pragma omp parallel for num_threads(8)
  for (int i = 0; i < iterations / 8; i++)
  {
    int tid = omp_get_thread_num();
    index = rules_probability[ R::runif( 0, p_count ) ];
    point[tid].Multiplication(_transformation[index], point[tid]);
    point[tid] += _translation[index];
    x[tid][i] = abs(point[tid][0][0]) > 10000 ? 0 : point[tid][0][0];
    y[tid][i] = abs(point[tid][1][0]) > 10000 ? 0 : point[tid][1][0];

    if(x[tid][i] > max_x) max_x = x[tid][i];
    if(x[tid][i] < min_x) min_x = x[tid][i];
    if(y[tid][i] > max_y) max_y = y[tid][i];
    if(y[tid][i] < min_y) min_y = y[tid][i];
  }

  double p = sqrt(pixels/((max_x - min_x)*(max_y - min_y)));

  w = (max_x - min_x) * p;
  h = (max_y - min_y) * p;

  NumericMatrix result(w, h);

  for(int tid = 0; tid < 8; ++tid)
    for(int i = 0; i < iterations / 8; ++i)
    {
      x[tid][i] = (x[tid][i] - min_x) * (w - 1) / (max_x - min_x);
      y[tid][i] = (y[tid][i] - min_y) * (h - 1) / (max_y - min_y);

      if(result(x[tid][i], y[tid][i]) < 100)
        result(x[tid][i], y[tid][i]) += 1;
    }

  return List::create(Named("ImageMatrix") = result);
}

// [[Rcpp::export]]
List IFS3D(const List& transformation, const List& translation, const std::vector<int>& probability, const int& iterations, const int pixels)
{
  std::vector<sp::Matrix<double>>  _transformation, _translation;
  std::vector<int>                 rules_probability;
  sp::Matrix<double>               point(std::vector<double>({0, 0, 0}));
  std::vector<double>              x(iterations), y(iterations), z(iterations);
  int p_count = 0;
  int index;
  double min_x, min_y, min_z, max_x, max_y, max_z;
  min_x = min_y = min_z =  std::numeric_limits<double>::infinity();
  max_x = max_y = max_z = -std::numeric_limits<double>::infinity();
  int w, h, l;

  for (int i = 0; i < transformation.length(); ++i)
    _transformation.push_back( ToMatrix( transformation(i) ) );

  for (int i = 0; i < translation.length(); ++i)
    _translation.push_back( sp::Matrix<double>( ToVector( translation(i) ) ) );

  for( int i = 0; i < probability.size(); ++i)
  {
    p_count += probability[i];
    for( int j = 0; j < probability[i]; ++j)
      rules_probability.push_back(i);
  }

  for (int i = 0; i < iterations; i++)
  {
    index = rules_probability[ R::runif( 0, p_count ) ];
    point.Multiplication(_transformation[index], point);
    point += _translation[index];
    x[i] = abs(point[0][0]) > 10000 ? 0 : point[0][0];
    y[i] = abs(point[1][0]) > 10000 ? 0 : point[1][0];
    z[i] = abs(point[2][0]) > 10000 ? 0 : point[2][0];

    if(x[i] > max_x) max_x = x[i];
    if(x[i] < min_x) min_x = x[i];
    if(y[i] > max_y) max_y = y[i];
    if(y[i] < min_y) min_y = y[i];
    if(z[i] > max_z) max_z = z[i];
    if(z[i] < min_z) min_z = z[i];
  }

  double p  = (max_x - min_x);
         p *= (max_y - min_y);
         p *= (max_z - min_z);
         p  = pixels / p;
         p  = cbrt( p );

  w = (max_x - min_x) * p;
  h = (max_y - min_y) * p;
  l = (max_z - min_z) * p;

  arma::cube result = arma::zeros<arma::cube>(w, h, l);

  for(int i = 0; i < iterations; ++i)
  {
    x[i] = (x[i] - min_x) * (w - 1) / (max_x - min_x);
    y[i] = (y[i] - min_y) * (h - 1) / (max_y - min_y);
    z[i] = (z[i] - min_z) * (l - 1) / (max_z - min_z);

    if((int) x[i] >= w || (int) y[i] >= h || (int) z[i] >= l || (int) x[i] < 0 || (int) y[i] < 0 || (int) z[i] < 0)
      continue;

    if(result(x[i], y[i], z[i]) < 100)
      result(x[i], y[i], z[i]) += 1;
  }

  return List::create(Named("ImageMatrix") = result);
}