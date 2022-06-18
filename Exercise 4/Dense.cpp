#include "Dense.h"

Dense::Dense (const Matrix &weights, const Matrix &bias,
              ActivationFunction activation_function)
{
  this->weights = weights;
  this->bias = bias;
  this->activation_function = activation_function;
}

const Matrix &Dense::get_weights () const
{
  return this->weights;
}

const Matrix &Dense::get_bias () const
{
  return this->bias;
}

ActivationFunction Dense::get_activation () const
{
  return this->activation_function;
}

Matrix Dense::operator() (const Matrix &input) const
{
  return get_activation () ((this->weights * input) + this->bias);
}