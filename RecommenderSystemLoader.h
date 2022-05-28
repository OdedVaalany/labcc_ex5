
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <stdexcept>

class RecommenderSystemLoader {

 private:
  static bool is_valid_number (const double &num);

 public:
  RecommenderSystemLoader () = delete;
  /**
   * loads movies by the given format for movies with their feature's score
   * @param movies_file_path a path to the file of the movies
   * @return shared pointer to a RecommenderSystem which was created
   * with those movies
   */
  static RS_ptr create_rs_from_movies_file
      (const std::string &movies_file_path) noexcept (false);
};

#endif //RECOMMENDERSYSTEMLOADER_H
