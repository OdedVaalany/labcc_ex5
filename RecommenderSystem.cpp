//
// Created by עודד ועלני on 25/05/2022.
//

#include "RecommenderSystem.h"

RecommenderSystem::RecommenderSystem ()
    : _movies_list ([] (const sp_movie &lhs, const sp_movie &rhs)
                    { return *lhs < *rhs; })
{
};

sp_movie RecommenderSystem::add_movie (const std::string &name,
                                       int year,
                                       const std::vector<double> &features)
{
  sp_movie movie_ptr = std::make_shared<Movie> (Movie (name, year));
  _movies_list.insert ({movie_ptr, features});
  return movie_ptr;
}

sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  std::vector<double> pre_vec = build_preference_vector (user);
  sp_movie best_movie = nullptr;
  double best_movie_score = 0;
  for (const auto &i: _movies_list)
    {
      if (user.get_ranks ().find (i.first) == user.get_ranks ().end ())
        {
          double current_movie_score = standard_product (pre_vec, i.second)
                                       / (norm (pre_vec) * norm (i.second));
          if (current_movie_score > best_movie_score)
            {
              best_movie = i.first;
              best_movie_score = current_movie_score;
            }
        }
    }
  return best_movie;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  double max_score = 0;
  sp_movie max_socred_movie;
  for (const auto &i: _movies_list)
    {
      if (user.get_ranks ().find (i.first) == user.get_ranks ().end ())
        {
          double current_score = predict_movie_score (user, i.first, k);
          if (current_score >= max_score)
            {
              max_score = current_score;
              max_socred_movie = i.first;
            }
        }
    }
  return max_socred_movie;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  for (const auto &i: _movies_list)
    {
      if (i.first->get_name () == name && i.first->get_year () == year)
        {
          return i.first;
        }
    }
  return nullptr;
}

std::ostream &operator<< (std::ostream &os, const RecommenderSystem &RS)
{
  for (const auto &i: RS._movies_list)
    {
      os << (*i.first);
    }
  return os;
}

double RecommenderSystem::predict_movie_score (const RSUser &user,
                                               const sp_movie &movie, int k)
{
  std::vector<std::pair<sp_movie, double>> watched_k;
  double sum1 = 0, sum2 = 0;

  for (const auto &i: user.get_ranks ())
    {
      watched_k.emplace_back (std::pair<sp_movie, double>
                                  (i.first, similarity (movie, i.first)));
    }
  std::sort (watched_k.begin (), watched_k.end (), [] (auto &x, auto &y)
  { return x.second > y.second; });
  for (size_t i = 0; i < k; ++i)
    {
      sum1 += watched_k.at (i).second
              * user.get_ranks ().find (watched_k.at (i).first)->second;
      sum2 += watched_k.at (i).second;
    }
  return sum1 / sum2;
}

double RecommenderSystem::similarity (const sp_movie &a, const sp_movie &b)
{
  return standard_product (get_features (a), get_features (b))
         / (norm (get_features (a)) * norm (get_features (b)));
}

double RecommenderSystem::norm (const std::vector<double> &vec)
{
  double sum = 0;
  for (double i: vec)
    {
      sum += i;
    }
  return std::sqrt (sum);
}

double RecommenderSystem::standard_product
    (const std::vector<double> &vec1, const std::vector<double> &vec2)
{
  double sum = 0;
  for (size_t i = 0; i < vec1.size (); ++i)
    {
      sum += vec1[i] * vec2[i];
    }
  return sum;
}

std::vector<double> RecommenderSystem::get_features (const sp_movie &movie)
{
  for (const auto i: _movies_list)
    {
      if (movie == i.first)
        {
          return i.second;
        }
    }
  return {};
}

std::vector<double> RecommenderSystem::connect_vectors
    (double alpha, const std::vector<double> &a,
     double beta, const std::vector<double> &b)
{
  std::vector<double> vec;
  for (size_t i = 0; i < a.size (); ++i)
    {
      vec.push_back (alpha * a.at (i) + beta * b.at (i));
    }
  return vec;
}

std::vector<double>
RecommenderSystem::build_preference_vector (const RSUser &user)
{
  double avg = 0;
  int avg_counter = 0;
  for (const auto i: user.get_ranks ())
    {
      avg += i.second;
      avg_counter++;
    }
  avg = avg / avg_counter;
  auto ranks_copy = user.get_ranks ();
  for (auto i: ranks_copy)
    {
      i.second = i.second - avg;
    }
  auto preference_vec =
      connect_vectors (ranks_copy.begin ()->second,
                       get_features (ranks_copy.begin ()->first),
                       0, get_features (ranks_copy.begin ()->first));
  for (auto i = ++ranks_copy.begin (); i != ranks_copy.end (); ++i)
    {
      preference_vec = connect_vectors
          (1, preference_vec, i->second, get_features (i->first));
    }
  return preference_vec;
}