#include "MlpNetwork.h"

MlpNetwork::MlpNetwork (Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE])
{
  this->weights = weights;
  this->biases = biases;
}

digit MlpNetwork::operator() (const Matrix &input) const
{
  Matrix current (input);
  for (int i = 0; i < MLP_SIZE; i++)
  {
    Dense dense (this->weights[i], this->biases[i],
                 this->activation_functions[i]);
    current = dense (current);
  }

  int digit = 0;

  for (int i = 1; i < current.get_rows (); i++)
  {
    if (current[i] > current[digit])
    {
      digit = i;
    }
  }

  return { (unsigned int) digit, current[digit] };
}