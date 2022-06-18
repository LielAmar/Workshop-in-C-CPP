#include "Matrix.h"

#define MIN_OUTPUT_VALUE 0.1
#define SAME_SIZE_ERROR "Matrices must be of same size to run this operation!"
#define CANNOT_MULTIPLY_ERROR \
  "Cannot multiply matrices, A.cols must be equal to B.rows!"
#define CANNOT_ACCESS_INDEX_ERROR "Cannot access the requested index!"
#define MATRIX_DIMS_ERROR "Matrix dimensions must be greater than 0!"
#define INPUT_STREAM_ERR "The given input stream is invalid!"
#define INPUT_STREAM_SIZE_ERR "The given input stream is not the correct size!"

Matrix::Matrix (int rows, int cols)
{
  if (rows <= 0 || cols <= 0)
  {
    throw std::length_error (MATRIX_DIMS_ERROR);
  }

  this->_dims.rows = rows;
  this->_dims.cols = cols;

  create_data_array (rows, cols);

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      (*this) (i, j) = 0;
    }
  }
}

Matrix::Matrix () : Matrix (1, 1)
{
}

Matrix::Matrix (const Matrix &b)
{
  this->_dims.rows = b._dims.rows;
  this->_dims.cols = b._dims.cols;

  create_data_array (b._dims.rows, b._dims.cols);

  for (int i = 0; i < this->_dims.rows; i++)
  {
    for (int j = 0; j < this->_dims.cols; j++)
    {
      (*this) (i, j) = (b) (i, j);
    }
  }
}

Matrix::~Matrix ()
{
  destruct ();
}

// ===== Functions =====
int Matrix::get_rows () const
{
  return this->_dims.rows;
}

int Matrix::get_cols () const
{
  return this->_dims.cols;
}

Matrix &Matrix::transpose ()
{
  int rows = this->_dims.cols;
  int cols = this->_dims.rows;

  Matrix new_matrix (rows, cols);

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      (new_matrix) (i, j) = (*this) (j, i);
    }
  }

  (*this) = new_matrix;
  return *this;
}

Matrix &Matrix::vectorize ()
{
  int rows = this->_dims.rows * this->_dims.cols;
  int cols = 1;

  Matrix new_matrix (rows, cols);

  for (int i = 0; i < rows; i++)
  {
    new_matrix[i] = (*this)[i];
  }

  (*this) = new_matrix;
  return *this;
}

void Matrix::plain_print () const
{
  for (int i = 0; i < this->_dims.rows; i++)
  {
    for (int j = 0; j < this->_dims.cols; j++)
    {
      std::cout << (*this) (i, j) << " ";
    }

    std::cout << std::endl;
  }
}

Matrix Matrix::dot (const Matrix &b) const
{
  if (!cmp_size (b))
  {
    throw std::length_error (SAME_SIZE_ERROR);
  }

  Matrix copy = *this;

  for (int i = 0; i < this->_dims.rows; i++)
  {
    for (int j = 0; j < this->_dims.cols; j++)
    {
      (copy) (i, j) *= (b) (i, j);
    }
  }

  return copy;
}

float Matrix::norm () const
{
  float sum = 0;

  for (int i = 0; i < this->_dims.rows; i++)
  {
    for (int j = 0; j < this->_dims.cols; j++)
    {
      sum += std::pow ((*this) (i, j), 2);
    }
  }

  return std::sqrt (sum);
}

// ===== Operators =====
Matrix Matrix::operator+ (const Matrix &b) const
{
  if (!cmp_size (b))
  {
    throw std::length_error (SAME_SIZE_ERROR);
  }

  Matrix copy = *this;

  for (int i = 0; i < copy._dims.rows; i++)
  {
    for (int j = 0; j < copy._dims.cols; j++)
    {
      (copy) (i, j) += (b) (i, j);
    }
  }

  return copy;
}

Matrix &Matrix::operator= (const Matrix &b)
{
  if(&b == this) {
    return *this;
  }

  destruct ();
  create_data_array (b._dims.rows, b._dims.cols);

  this->_dims.rows = b._dims.rows;
  this->_dims.cols = b._dims.cols;

  for (int i = 0; i < this->_dims.rows; i++)
  {
    for (int j = 0; j < this->_dims.cols; j++)
    {
      (*this) (i, j) = (b) (i, j);
    }
  }

  return *this;
}

Matrix Matrix::operator* (const Matrix &b) const
{
  if (!can_mult (b))
  {
    throw std::length_error (CANNOT_MULTIPLY_ERROR);
  }

  int rows = this->_dims.rows;
  int cols = b._dims.cols;

  Matrix copy (rows, cols);

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      (copy) (i, j) = 0;

      for (int k = 0; k < b._dims.rows; k++)
      {
        (copy) (i, j) += (*this) (i, k) * (b) (k, j);
      }
    }
  }

  return copy;
}

Matrix Matrix::operator* (float c) const
{
  Matrix copy = *this;

  for (int i = 0; i < copy._dims.rows; i++)
  {
    for (int j = 0; j < copy._dims.cols; j++)
    {
      (copy) (i, j) *= c;
    }
  }

  return copy;
}

Matrix operator* (float c, const Matrix &b)
{
  return b * c;
}

Matrix &Matrix::operator+= (const Matrix &b)
{
  if (!cmp_size (b))
  {
    throw std::length_error (SAME_SIZE_ERROR);
  }

  for (int i = 0; i < this->_dims.rows; i++)
  {
    for (int j = 0; j < this->_dims.cols; j++)
    {
      (*this) (i, j) += (b) (i, j);
    }
  }

  return *this;
}

float &Matrix::operator() (int row, int col)
{
  if (row < 0 || row >= this->_dims.rows || col < 0 || col >= this->_dims.cols)
  {
    throw std::out_of_range (CANNOT_ACCESS_INDEX_ERROR);
  }

  return this->_data[row][col];
}

float &Matrix::operator[] (int index)
{
  if (index < 0 || index >= this->_dims.rows * this->_dims.cols)
  {
    throw std::out_of_range (CANNOT_ACCESS_INDEX_ERROR);
  }

  int row = index / this->_dims.cols;
  int col = index % this->_dims.cols;

  return this->_data[row][col];
}

float Matrix::operator() (int row, int col) const
{
  if (row < 0 || row >= this->_dims.rows || col < 0 || col >= this->_dims.cols)
  {
    throw std::out_of_range (CANNOT_ACCESS_INDEX_ERROR);
  }

  return this->_data[row][col];
}

float Matrix::operator[] (int index) const
{
  if (index < 0 || index >= this->_dims.rows * this->_dims.cols)
  {
    throw std::out_of_range (CANNOT_ACCESS_INDEX_ERROR);
  }

  int row = index / this->_dims.cols;
  int col = index % this->_dims.cols;

  return this->_data[row][col];
}

std::ostream &operator<< (std::ostream &os, const Matrix &b)
{
  if (!os)
  {
    return os;
  }

  for (int i = 0; i < b._dims.rows; i++)
  {
    for (int j = 0; j < b._dims.cols; j++)
    {
      if (b (i, j) > MIN_OUTPUT_VALUE)
      {
        os << "**";
      }
      else
      {
        os << "  ";
      }
    }
    os << std::endl;
  }

  return os;
}

std::istream &operator>> (std::istream &is, Matrix &b)
{
  if (!is)
  {
    throw std::runtime_error (INPUT_STREAM_ERR);
  }

  is.seekg (0, std::istream::end);
  int length = is.tellg ();
  is.seekg (0, std::istream::beg);

  if ((unsigned int) (length / sizeof (float))
      < (unsigned int) b._dims.rows * b._dims.cols)
  {
    throw std::length_error (INPUT_STREAM_SIZE_ERR);
  }

  char *buffer = new char[length];
  is.read (buffer, length);

  auto *values = (float *) buffer;

  for (int i = 0; i < b._dims.rows * b._dims.cols; i++)
  {
    b[i] = values[i];
  }

  delete[] buffer;

  return is;
}

// ===== Private utilities =====
/**
 * Deletes all memory allocations
 */
void Matrix::destruct ()
{
  for (int i = 0; i < this->_dims.rows; i++)
  {
    delete[] this->_data[i];
  }

  delete[] this->_data;
}

/**
 * Creates a data array of size ${rows}x${cols}
 * @param rows number of rows
 * @param cols number of cols
 */
void Matrix::create_data_array (int rows, int cols)
{
  this->_data = new float *[rows];

  for (int i = 0; i < rows; i++)
  {
    this->_data[i] = new float[cols];
  }
}

/**
 * Checks if current matrix is the same size as the given matrix
 * @param b other matrix
 * @return whether or not both matrices are same size
 */
bool Matrix::cmp_size (const Matrix &b) const
{
  return this->_dims.rows == b._dims.rows && this->_dims.cols == b._dims.cols;
}

/**
 * Checks if multiplication between current matrix and the given matrix is
 * defined
 * @param b other matrix
 * @return whether or not both matrices can be multiplied
 */
bool Matrix::can_mult (const Matrix &b) const
{
  return this->_dims.cols == b._dims.rows;
}