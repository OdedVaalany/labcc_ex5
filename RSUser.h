//
// Created by עודד ועלני on 26/05/2022.
//

#ifndef _RSUSER_H_
#define _RSUSER_H_
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "Movie.h"

class RecommenderSystem;
typedef std::unordered_map<sp_movie, double, hash_func, equal_func> rank_map;
typedef std::unique_ptr<RecommenderSystem> RS_ptr;

class RSUser {
 private:
  std::string _user_name;
  rank_map _user_ranking_map;
  RecommenderSystem *_user_RS;
 public:
  /**
   * Constructor for the class
   */
  RSUser () = default;
  RSUser (std::string &user_name, rank_map ranking, RS_ptr &user_rs);
  /**
   * a getter for the user's name
   * @return the username
   */
  std::string get_name () const;
  /**
   * function for adding a movie to the DB
   * @param movie a Movie object
   * @param features a vector of the movie's features
   * @param rate the user rate for this movie
   * @return true for success, false otherwise
   */
  void add_movie_to_rs (const std::string &name, int year,
                        const std::vector<double> &features,
                        double rate);

  /**
   * a getter for the ranks map
   * @return reference to rank_map
   */
  const rank_map &get_ranks () const;
  rank_map &get_ranks ();
  /**
   * returns a recommendation according to the movie's content
   * @return recommendation
   */
  sp_movie get_recommendation_by_content () const;

  /**
   * returns a recommendation according to the similarity recommendation method
   * @param k the number of the most similar movies to calculate by
   * @return recommendation
   */
  sp_movie get_recommendation_by_cf (int k) const;

  /**
   * predicts the score for a given movie
   * @param name the name of the movie
   * @param year the year the movie was created
   * @param k the parameter which represents the number
   * of the most similar movies to predict the score by
   * @return predicted score for the given movie
   */
  double get_prediction_score_for_movie
      (const std::string &name, int year, int k) const;

  /**
   * output stream operator
   * @param os the output stream
   * @param user the user
   * @return output stream
   */
  friend std::ostream &operator<< (std::ostream &os, const RSUser &user);
};
#endif //_RSUSER_H_
