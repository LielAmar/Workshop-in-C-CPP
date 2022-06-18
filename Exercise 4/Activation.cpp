#include "Activation.h"

#include <cmath>

Matrix activation::relu (const Matrix &m)
{
  Matrix result (m);

  for (int i = 0; i < result.get_cols () * result.get_rows (); i++)
  {
    if (result[i] < 0)
    {
      result[i] = 0;
    }
  }

  return result;
}

Matrix activation::softmax (const Matrix &m)
{
  Matrix result (m);

  float sum = 0;
  for (int i = 0; i < result.get_cols () * result.get_rows (); i++)
  {
    sum += std::exp (result[i]);
  }

  float prefix = 1 / sum;

  for (int i = 0; i < result.get_cols () * result.get_rows (); i++)
  {
    result[i] = prefix * std::exp (result[i]);
  }

  return result;
}