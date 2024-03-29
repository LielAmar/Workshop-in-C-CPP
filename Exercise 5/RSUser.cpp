#include "RSUser.h"
#include "RecommenderSystem.h"

void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const std::vector<double> &features, double rate)
{
  sp_movie movie = this->rs->add_movie (name, year, features);
  this->ratings[movie] = rate;
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return this->rs->get_recommendation_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return this->rs->recommend_by_cf (*this, k);
}

double RSUser::get_prediction_score_for_movie (const std::string &name,
                                               int year, int k) const
{
  return this->rs->predict_movie_score (*this,
                                        (*this->rs).get_movie (name, year), k);
}

std::ostream &operator<< (std::ostream &os, const RSUser &user)
{
  os << "name: " << user.get_name () << "\n";
  os << (*user.rs);

  return os;
}