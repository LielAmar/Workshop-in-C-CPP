#include "RecommenderSystemLoader.h"

#define IS_ALLOWED_NUMBER(x) (x >= 0 && x <= 10)

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file (
    const std::string &movies_file_path) noexcept (false)
{
  RecommenderSystem system;

  std::ifstream file;
  file.open (movies_file_path);

  if (!file)
  {
    throw std::runtime_error ("Could not open file: " + movies_file_path);
  }

  std::string line;
  while (std::getline (file, line))
  {
    parse_movie_line (line, system);
  }

  file.close ();

  return std::make_unique<RecommenderSystem> (system);
}

void RecommenderSystemLoader::parse_movie_line (const std::string &line,
                                                RecommenderSystem &system)
{
  std::istringstream iss (line);

  std::string movie_name;
  int year;
  std::vector<double> features;

  std::getline (iss, movie_name, '-');

  iss >> year;

  double number;
  while (iss >> number)
  {
    if (!IS_ALLOWED_NUMBER (number))
    {
      throw std::invalid_argument ("Invalid feature value: "
                                   + std::to_string (number));
    }

    features.push_back (number);
  }

  system.add_movie (movie_name, year, features);
}