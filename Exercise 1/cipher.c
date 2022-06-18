#include "cipher.h"

#define ALPHABET_SINGLE_SIZE 26
#define ALPHABET_SIZE (ALPHABET_SINGLE_SIZE * 2)
#define ALPHABET_UPPER_START 'A'
#define ALPHABET_UPPER_END 'Z'
#define ALPHABET_LOWER_START 'a'
#define ALPHABET_LOWER_END 'z'

typedef enum CharType
{
  LOWER_CHAR,
  UPPER_CHAR
} CharType;

int shift_character (int remainder, char c, CharType type);

/**
 * @brief Encodes the given string with the given key, using the caesar cipher.
 *
 * @param s string to encode
 * @param k key to use
 */
void encode (char s[], int k)
{
  int remainder = (k % ALPHABET_SIZE);

  int index = 0;

  while (s[index] != '\0')
  {
    if (s[index] >= ALPHABET_UPPER_START && s[index] <= ALPHABET_UPPER_END)
    {
      s[index] = shift_character (remainder, s[index], UPPER_CHAR);
    }
    else if (s[index] >= ALPHABET_LOWER_START
             && s[index] <= ALPHABET_LOWER_END)
    {
      s[index] = shift_character (remainder, s[index], LOWER_CHAR);
    }

    index++;
  }
}

/**
 * @brief Decodes the given string with the given key, using the caesar cipher.
 *
 * @param s string to decode
 * @param k key to use
 */
void decode (char s[], int k)
{
  return encode (s, (-1) * k);
}

/**
 * @brief Shifts the given character by the given remainder.
 *
 * @param remainder the remainder to shift by
 * @param c character to shift
 * @param type type of character to shift
 * @return the shifted character
 */
int shift_character (int remainder, char c, CharType type)
{
  while (remainder != 0)
  {
    c = c + (remainder > 0 ? 1 : -1);

    if (type == LOWER_CHAR)
    {
      if (c > ALPHABET_LOWER_END)
      {
        c = ALPHABET_LOWER_START;
      }
      if (c < ALPHABET_LOWER_START)
      {
        c = ALPHABET_LOWER_END;
      }
    }

    if (type == UPPER_CHAR)
    {
      if (c > ALPHABET_UPPER_END)
      {
        c = ALPHABET_UPPER_START;
      }
      if (c < ALPHABET_UPPER_START)
      {
        c = ALPHABET_UPPER_END;
      }
    }

    remainder = remainder + (remainder > 0 ? -1 : 1);
  }

  return c;
}