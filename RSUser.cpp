//
// Created by עודד ועלני on 26/05/2022.
//

// don't change those includes
#include "RSUser.h"

#include <utility>
#include "RecommenderSystem.h"

RSUser::RSUser (std::string &user_name, rank_map ranking, RS_ptr user_rs)
{
  _user_name = user_name;
  _user_ranking_map = std::move (ranking);
  _user_RS = std::move (user_rs);
}

std::string RSUser::get_name () const
{
  return _user_name;
}

void RSUser::add_movie_to_rs
    (const std::string &name, int year,
     const std::vector<double> &features, double rate)
{
  sp_movie movie = _user_RS->add_movie (name, year, features);
  _user_ranking_map.insert ({movie, rate});
}

const rank_map &RSUser::get_ranks () const
{
  return _user_ranking_map;
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return _user_RS->recommend_by_cf (*this, k);
}

double
RSUser::get_prediction_score_for_movie (const std::string &name, int year, int k) const
{
  for (auto i: _user_ranking_map)
    {
      if (i.first->get_name () == name && i.first->get_year () == year)
        {
          _user_RS->predict_movie_score (*this, i.first, k);
        }
    }
  return 0.0;
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return _user_RS->recommend_by_content (*this);
}

std::ostream &operator<< (std::ostream &os, const RSUser &user)
{
  os << "name: " << user.get_name () << std::endl;
  std::vector<sp_movie> watched;
  for (const auto i: user.get_ranks ())
    {
      watched.push_back (i.first);
    }
  std::sort (watched.begin (), watched.end (), [] (const sp_movie &a, const sp_movie &b)
  { return *a < *b; });
  for (const auto i: watched)
    {
      os << *i;
    }
  return os;
}
// implement your cpp code here
