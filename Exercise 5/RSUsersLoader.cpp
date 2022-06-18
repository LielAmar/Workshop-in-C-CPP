#include "RSUsersLoader.h"

#define IS_ALLOWED_NUMBER(x) (x >= 0 && x <= 10)

std::vector<RSUser>
RSUsersLoader::create_users_from_file (const std::string &users_file_path,
                                       std::unique_ptr<RecommenderSystem> rs)
    noexcept (false)
{
  std::shared_ptr<RecommenderSystem> sp
      = std::shared_ptr<RecommenderSystem> (std::move (rs));

  std::vector<std::pair<std::string, int> > movies;
  std::vector<RSUser> users;

  std::ifstream file;
  file.open (users_file_path);

  if (!file)
  {
    throw std::runtime_error ("Could not open file: " + users_file_path);
  }

  std::string movies_data;

  std::getline (file, movies_data);
  std::istringstream iss (movies_data);

  std::string line;
  while (std::getline (iss, line, ' ')) // Read a single movie
  {
    parse_movie_line (line, movies);
  }

  while (std::getline (file, line))
  {
    parse_user_line (line, users, movies, sp);
  }

  file.close ();
  return users;
}

void RSUsersLoader::parse_movie_line (
    const std::string &line, std::vector<std::pair<std::string, int> > &movies)
{
  std::istringstream iss_movie (line);

  std::string movie_name;
  int year;

  std::getline (iss_movie, movie_name, '-');
  iss_movie >> year;

  movies.emplace_back (movie_name, year);
}

void RSUsersLoader::parse_user_line (
    const std::string &line, std::vector<RSUser> &users,
    const std::vector<std::pair<std::string, int> > &movies,
    const std::shared_ptr<RecommenderSystem> &rs)
{
  std::istringstream iss (line);

  std::string user_name;
  rank_map ratings (0, sp_movie_hash, sp_movie_equal);

  iss >> user_name;

  int index = 0;
  std::string rating;
  while (iss >> rating)
  {
    double number;

    if (rating == "NA")
    {
      number = -1;
    }
    else
    {
      std::istringstream iss_rating (rating);
      iss_rating >> number;

      if (!IS_ALLOWED_NUMBER (number))
      {
        throw std::invalid_argument ("Invalid feature value: "
                                     + std::to_string (number));
      }
    }

    sp_movie movie
        = (*rs).get_movie (movies[index].first, movies[index].second);

    ratings[movie] = number;
    index++;
  }

  users.emplace_back (user_name, ratings, rs);
}