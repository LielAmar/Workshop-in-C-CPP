#include "Movie.h"

#define HASH_START 17
#define RES_MULT 31

bool operator< (const Movie &lhs, const Movie &rhs)
{
  if (lhs.get_year () < rhs.get_year ())
  {
    return true;
  }
  else if (lhs.get_year () > rhs.get_year ())
  {
    return false;
  }

  return lhs.get_name ().compare (rhs.get_name ()) < 0;
}

std::ostream &operator<< (std::ostream &os, const Movie &movie)
{
  os << movie.get_name () << " (" << movie.get_year () << ")\n";
  return os;
}

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash (const sp_movie &movie)
{
  std::size_t res = HASH_START;
  res = res * RES_MULT + std::hash<std::string> () (movie->get_name ());
  res = res * RES_MULT + std::hash<int> () (movie->get_year ());
  return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal (const sp_movie &m1, const sp_movie &m2)
{
  return !(*m1 < *m2) && !(*m2 < *m1);
}
