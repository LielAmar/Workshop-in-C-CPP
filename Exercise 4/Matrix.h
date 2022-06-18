// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <fstream>
#include <iostream>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

class Matrix
{
  public:
    Matrix (int rows, int cols);
    Matrix ();
    Matrix (const Matrix &b);
    ~Matrix ();

    // Functions
    int get_rows () const;
    int get_cols () const;

    /**
     * @brief Transposes the current matrix and returns a reference to it.
     */
    Matrix &transpose ();
    /**
     * @brief Vectorizes the current matrix and returns a reference to it.
     */
    Matrix &vectorize ();

    /**
     * @brief Prints the matrix as plain text.
     */
    void plain_print () const;

    /**
     * @brief Combines the current matrix with the given one into a copy matrix
     * and returns a copy of the result.
     */
    Matrix dot (const Matrix &b) const;
    /**
     * @brief Returns the normalized value of the matrix.
     */
    float norm () const;

    // Operators
    Matrix operator+ (const Matrix &b) const;
    Matrix &operator= (const Matrix &b);
    Matrix operator* (const Matrix &b) const;
    Matrix operator* (float c) const;
    friend Matrix operator* (float c, const Matrix &b);
    Matrix &operator+= (const Matrix &b);
    float &operator() (int row, int col);
    float &operator[] (int index);
    float operator() (int row, int col) const;
    float operator[] (int index) const;
    /**
     * @brief Prints the matrix to the given output stream.
     */
    friend std::ostream &operator<< (std::ostream &os, const Matrix &b);
    /**
     * @brief Reads the matrix from the given input stream.
     */
    friend std::istream &operator>> (std::istream &os, Matrix &b);

  private:
    matrix_dims _dims;
    float **_data;

    /**
     * @brief Destructs the matrix.
     */
    void destruct ();
    /**
     * @brief Creates the data array for the matrix of size ${rows}x${cols}.
     */
    void create_data_array (int rows, int cols);
    /**
     * @brief Compares the size of the current matrix with the given one.
     */
    bool cmp_size (const Matrix &b) const;
    /**
     * @brief Checks if the current matrix can be multiplied with the given
     * one.
     */
    bool can_mult (const Matrix &b) const;
};

#endif // MATRIX_H