#include "RecommenderSystem.h"

#define RANK_MAP_NA -1

sp_movie RecommenderSystem::add_movie (const std::string &name, int year,
                                       const std::vector<double> &features)
{
  sp_movie movie = std::make_shared<Movie> (name, year); // O(1)
  this->movies[movie] = features;                        // O(log(n))
  return movie;                                          // O(1)
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  sp_movie movie = std::make_shared<Movie> (name, year); // O(1)
  auto entry = this->movies.find (movie);                // O(log(n))

  if (entry != this->movies.end ())
  {
    return entry->first;
  }

  return { nullptr };
}

sp_movie RecommenderSystem::get_recommendation_by_content (const RSUser &user)
{
  rank_map watched = create_watched_rank_map (user.get_ranks ());
  normalize_rank_map (watched);

  std::vector<double> preferences = generate_preference_vector (watched);
  return get_best_movie (preferences, watched);
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  rank_map watched = create_watched_rank_map (user.get_ranks ());

  sp_movie best_movie = nullptr;
  double best_score = -1;

  // We loop over this->movies instead of user_movies to be able to recommend
  // movies that are not in user_movies yet
  for (const auto &entry : this->movies)
  {
    // if the user had already watched the movie, skip it
    if (watched.find (entry.first) != watched.end ())
    {
      continue;
    }

    double movie_score = predict_movie_score (user, entry.first, k);

    if (movie_score > best_score || best_movie == nullptr)
    {
      best_movie = entry.first;
      best_score = movie_score;
    }
  }

  return best_movie;
}

double RecommenderSystem::predict_movie_score (const RSUser &user,
                                               const sp_movie &movie, int k)
{
  rank_map watched = create_watched_rank_map (user.get_ranks ());
  auto similar_movies = sort_similar_movies (watched, movie);

  double numerator = 0;
  double denominator = 0;
  for (int i = 0; i < k; i++)
  {
    numerator += watched[similar_movies[i].first] * similar_movies[i].second;
    denominator += similar_movies[i].second;
  }

  return numerator / denominator;
}

std::ostream &operator<< (std::ostream &os, const RecommenderSystem &rs)
{
  for (const auto &entry : rs.movies) // O(n)
  {
    os << (*entry.first);
  }

  os << std::flush;
  return os;
}

// ===== UTILITIES =====
rank_map RecommenderSystem::create_watched_rank_map (const rank_map &ranks)
{
  rank_map copy_map (0, sp_movie_hash, sp_movie_equal);

  for (const auto &rank : ranks)
  {
    // Only adding if rank is not NA
    if (rank.second != RANK_MAP_NA)
    {
      copy_map[rank.first] = rank.second;
    }
  }

  return copy_map;
}

void RecommenderSystem::normalize_rank_map (rank_map &ranks)
{
  double sum = 0;
  int counter = 0;
  for (const auto &rank : ranks)
  {
    sum += rank.second;
    counter++;
  }

  double avg = sum / counter; // we can assume counter is not 0

  for (const auto &rank : ranks)
  {
    ranks[rank.first] = rank.second - avg;
  }
}

std::vector<double>
RecommenderSystem::generate_preference_vector (const rank_map &ranks)
{
  std::vector<double> preference_vector;

  // TODO: might need to add a check if all features vectors are same size
  unsigned long size
      = (*this->movies.begin ())
            .second.size (); // size of feature vector of the first movie

  preference_vector.reserve (size);

  for (unsigned long i = 0; i < size; i++)
  {
    preference_vector.push_back (0);
  }

  for (std::pair<sp_movie, double> rank : ranks)
  {
    std::vector<double> movie_features = this->movies[rank.first];

    for (unsigned int i = 0; i < movie_features.size (); i++)
    {
      preference_vector[i] += rank.second * movie_features[i];
    }
  }

  return preference_vector;
}

sp_movie
RecommenderSystem::get_best_movie (const std::vector<double> &preferences,
                                   const rank_map &watched)
{
  sp_movie best_movie = nullptr;
  double best_score = -1;

  // We loop over this->movies instead of user_movies to be able to recommend
  // movies that are not in user_movies yet
  for (const auto &entry : this->movies)
  {
    // if the user had already watched the movie, skip it
    if (watched.find (entry.first) != watched.end ())
    {
      continue;
    }

    std::vector<double> movie_features = this->movies[entry.first];
    double movie_score
        = calculate_vector_multiplication (preferences, movie_features);

    if (movie_score > best_score || best_movie == nullptr)
    {
      best_movie = entry.first;
      best_score = movie_score;
    }
  }

  return best_movie;
}

double RecommenderSystem::calculate_vector_multiplication (
    const std::vector<double> &v1, const std::vector<double> &v2)
{
  double multed = 0;
  for (unsigned long i = 0; i < v1.size(); i++)
  {
    multed += v1[i] * v2[i];
  }

  double v1_size = 0;
  for (double i : v1)
  {
    v1_size += i * i;
  }

  double v2_size = 0;
  for (double i : v2)
  {
    v2_size += i * i;
  }

  return (multed / (std::sqrt (v1_size) * std::sqrt (v2_size)));
}

std::vector<std::pair<sp_movie, double> >
RecommenderSystem::sort_similar_movies (const rank_map &watched,
                                        const sp_movie &movie)
{
  std::vector<double> movie_features = this->movies[movie];

  std::vector<std::pair<sp_movie, double> > scores;

  for (const auto &entry : watched)
  {
    std::vector<double> entry_features = this->movies[entry.first];
    scores.emplace_back (entry.first, calculate_vector_multiplication (
                                          movie_features, entry_features));
  }

  std::sort (
      scores.begin (), scores.end (),
      [] (std::pair<sp_movie, double> &a, std::pair<sp_movie, double> &b)
      { return a.second > b.second; }); // Reversed order to get largest first

  return scores;
}
