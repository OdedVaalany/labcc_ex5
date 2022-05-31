
#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include "Movie.h"
#include "RSUser.h"
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

class RecommenderSystem;

typedef std::shared_ptr<RecommenderSystem> RS_ptr;

class RecommenderSystem {
 private:
  bool comapre (const sp_movie &first, const sp_movie &sec)
  {
    return first < sec;
  }
  double similarity (const sp_movie &a, const sp_movie &b);
  static double norm (const std::vector<double> &vec);
  double standard_product (const std::vector<double> &vec1,
                           const std::vector<double> &vec2);
  std::vector<double> connect_vectors
      (double alpha, const std::vector<double> &a, double beta,
       const std::vector<double> &b);
  std::vector<double> get_features (const sp_movie &movie);
  std::vector<double> build_preference_vector (const RSUser &user);
  std::map<sp_movie, std::vector<double>, equal_func> _movies_list;
 public:
  explicit RecommenderSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie add_movie (const std::string &name,
                      int year, const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score based
   * on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted
   * score based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);
  
  /**
   * Predict a user rating for a movie given argument using
   * item cf procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  /**
   * overload of << operator
   * @param os ostream reference
   * @param RS recommender system reference
   * @return ostream
   * */
  friend std::ostream &
  operator<< (std::ostream &os, const RecommenderSystem &RS);

};

#endif //RECOMMENDERSYSTEM_H
