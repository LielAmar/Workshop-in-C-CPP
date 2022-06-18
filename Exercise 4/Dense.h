#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

class Dense
{
  public:
    Dense (const Matrix &weights, const Matrix &bias,
           ActivationFunction activation_function);

    /**
     * @brief Returns the weights of the layer.
     */
    const Matrix &get_weights () const;
    /**
     * @brief Returns the bias of the layer.
     */
    const Matrix &get_bias () const;
    /**
     * @brief Returns the activation function of the layer.
     */
    ActivationFunction get_activation () const;

    /**
     * @brief Operator that executes the layer on a copy of the given matrix
     * and returns the copied matrix.
     */
    Matrix operator() (const Matrix &input) const;

  private:
    Matrix weights;
    Matrix bias;
    ActivationFunction activation_function;
};

#endif // DENSE_H
