#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H

#include "RSUser.h"
#include <cmath>
#include <map>

class RecommenderSystem
{
  public:
    explicit RecommenderSystem ()
    {
      compare_sp_movies_func f
          = [] (const sp_movie &a, const sp_movie &b) { return (*a) < (*b); };
      movies = movies_map (f);
    };

    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
    sp_movie add_movie (const std::string &name, int year,
                        const std::vector<double> &features);

    /**
     * gets a shared pointer to movie in system
     * @param name name of movie
     * @param year year movie was made
     * @return shared pointer to movie in system
     */
    sp_movie get_movie (const std::string &name, int year) const;

    /**
     * a function that calculates the movie with highest score based on movie
     * features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
    sp_movie get_recommendation_by_content (const RSUser &user);

    /**
     * a function that calculates the movie with highest predicted score based
     * on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
    sp_movie recommend_by_cf (const RSUser &user, int k);

    /**
     * Predict a user rating for a movie given argument using item cf procedure
     * with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
    double predict_movie_score (const RSUser &user, const sp_movie &movie,
                                int k);

    friend std::ostream &operator<< (std::ostream &os,
                                     const RecommenderSystem &rs);

  private:
    typedef bool (*compare_sp_movies_func) (const sp_movie &a,
                                            const sp_movie &b);

    typedef std::map<sp_movie, std::vector<double>, compare_sp_movies_func>
        movies_map;

    movies_map movies;

    /**
     * Creates a rank_map that has similar data to ${ranks}, without unwatched
     * movies
     * @param ranks user's rank_map
     * @return created copy of the user's rank_map
     */
    static rank_map create_watched_rank_map (const rank_map &ranks);

    /**
     * Normalizes a rank map by subtracting the average rank from each movie
     * rank
     * @param ranks ranks to normalize
     */
    static void normalize_rank_map (rank_map &ranks);

    /**
     * Generates a vector of preferences for the given user by the given ranks
     * map It is assumed that the rank map is normalized Each entry is the
     * preferences vector is the sum of: (the rank of the movie) * (movie
     * feature at that index)
     * @param ranks normalized rank map of the user
     * @return preferences vector of the user
     */
    std::vector<double> generate_preference_vector (const rank_map &ranks);

    /**
     * Calculates the best movie for the user to watch, that they had not
     * watched already, based on their preferences
     * @param preferences vector of the user's preferences
     * @param user_movies movies the user had watched and not watched yet
     * @return best movie for the user to watch
     */
    sp_movie get_best_movie (const std::vector<double> &preferences,
                             const rank_map &user_movies);

    /**
     * Calculates the multiplication of the given 2 vectors through the
     * following formula:
     * (v1 x v2)/(|v1|*|v2|) where x is the standard inner product

     * @param v1 first vector
     * @param v2 second vector
     * @return calculated score
     */
    static double
    calculate_vector_multiplication (const std::vector<double> &v1,
                                     const std::vector<double> &v2);

    /**
     * Loops over all watched movies in ${watched} and multiplies each movie's
     * features vector by the ${movie} features vector.
     * Then creates a vector of the multiplication of each movie's features
     * and sorts it from largest to smallest
     * @param watched movies the user has watched
     * @param movie movie to compare to
     * @return sorted vector of the multiplication of each movie's features
     */
    std::vector<std::pair<sp_movie, double> >
    sort_similar_movies (const rank_map &watched, const sp_movie &movie);
};

#endif // RECOMMENDERSYSTEM_H
