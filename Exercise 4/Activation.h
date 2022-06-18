#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

typedef Matrix (*ActivationFunction) (const Matrix &);

namespace activation
{
  /**
   * A function that applies the ReLU activation function on a given matrix
   *
   * @param m The matrix to apply the activation function on
   */
  Matrix relu (const Matrix &);

  /**
   * A function that applies the SoftMax activation function on a given matrix
   *
   * @param m The matrix to apply the activation function on
   */
  Matrix softmax (const Matrix &);
}

#endif // ACTIVATION_H