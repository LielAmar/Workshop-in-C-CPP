#include <string.h>

#include "tests.h"


// [=== Definitions ===]
#define ALPHABET_SIZE 52
#define AMPLIFIER 0

#define K_1 (3 + (ALPHABET_SIZE * AMPLIFIER))
#define K_2 (2 + (ALPHABET_SIZE * AMPLIFIER))
#define K_3 (-1 - (ALPHABET_SIZE * AMPLIFIER))
#define K_4 (-3 - (ALPHABET_SIZE * AMPLIFIER))
#define K_5 (29 + (ALPHABET_SIZE * AMPLIFIER))

// [=== Functions ===]
int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "abc";
  char out[] = "def";
  encode (in, K_1);

  return strcmp (in, out) != 0;
}

int test_encode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "xy, z";
  char out[] = "za, b";
  encode (in, K_2);

  char in2[] = "this is a test! for the cipher@# algorithm :)";
  char out2[] = "vjku ku c vguv! hqt vjg ekrjgt@# cniqtkvjo :)";
  encode (in2, K_2);

  return !(strcmp (in, out) == 0 && strcmp (in2, out2) == 0);
}

int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "xy, z!@";
  char out[] = "wx, y!@";
  encode (in, K_3);

  char in2[] = "hijk :) xy, z";
  char out2[] = "ghij :) wx, y";
  encode (in2, K_3);

  return !(strcmp (in, out) == 0 && strcmp (in2, out2) == 0);
}

int test_encode_cyclic_lower_case_negative_k ()
{
  char in[] = "ab, c";
  char out[] = "xy, z";
  encode (in, K_4);

  char in2[] = "@#abcdefg!, ^qrzt =] :)";
  char out2[] = "@#xyzabcd!, ^nowq =] :)";
  encode (in2, K_4);

  return !(strcmp (in, out) == 0 && strcmp (in2, out2) == 0);
}

int test_encode_cyclic_upper_case_positive_k ()
{
  char in[] = "ab, c";
  char out[] = "de, f";
  encode (in, K_5);

  char in2[] = "ab,cd!ef$ghijk 97 lmno $^ pqrs /. tuvw xyz :)";
  char out2[] = "de,fg!hi$jklmn 97 opqr $^ stuv /. wxyz abc :)";
  encode (in2, K_5);

  return !(strcmp (in, out) == 0 && strcmp (in2, out2) == 0);
}

int test_decode_non_cyclic_lower_case_positive_k ()
{
  char out[] = "abc";
  char in[] = "def";
  decode (in, K_1);

  return !(strcmp (in, out) == 0);
}

int test_decode_cyclic_lower_case_special_char_positive_k ()
{
  char out[] = "xy, z";
  char in[] = "za, b";
  decode (in, K_2);

  char out2[] = "this is a test! for the cipher@# algorithm :)";
  char in2[] = "vjku ku c vguv! hqt vjg ekrjgt@# cniqtkvjo :)";
  decode (in2, K_2);

  return !(strcmp (in, out) == 0 && strcmp (in2, out2) == 0);
}

int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
  char out[] = "xy, z!@";
  char in[] = "wx, y!@";
  decode (in, K_3);

  char out2[] = "hijk :) xy, z";
  char in2[] = "ghij :) wx, y";
  decode (in2, K_3);

  return !(strcmp (in, out) == 0 && strcmp (in2, out2) == 0);
}

int test_decode_cyclic_lower_case_negative_k ()
{
  char out[] = "ab, c";
  char in[] = "xy, z";
  decode (in, K_4);

  char out2[] = "@#abcdefg!, ^qrzt =] :)";
  char in2[] = "@#xyzabcd!, ^nowq =] :)";
  decode (in2, K_4);

  return !(strcmp (in, out) == 0 && strcmp (in2, out2) == 0);
}

int test_decode_cyclic_upper_case_positive_k ()
{
  char out[] = "ab, c";
  char in[] = "de, f";
  decode (in, K_5);

  char out2[] = "ab,cd!ef$ghijk 97 lmno $^ pqrs /. tuvw xyz :)";
  char in2[] = "de,fg!hi$jklmn 97 opqr $^ stuv /. wxyz abc :)";
  decode (in2, K_5);

  return !(strcmp (in, out) == 0 && strcmp (in2, out2) == 0);
}
