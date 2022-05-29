//
// Created by עודד ועלני on 26/05/2022.
//
#include "RecommenderSystemLoader.h"
#define OUT_OF_RANGE_MSG "feature value is out of range of 1-10"
#define UPPER_BOUND(args) args <= 10
#define LOWER_BOUND(args) args >= 1
RS_ptr
RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string &movies_file_path) noexcept (false)
{
  std::shared_ptr<RecommenderSystem> rs =
      std::make_shared<RecommenderSystem> (RecommenderSystem ());
  std::ifstream users_file (movies_file_path, std::ios::in);
  std::vector<double> features_vec;
  if (!users_file.is_open ())
    {
      users_file.close();
      throw std::runtime_error ("Not the right path");
    }
  std::string line, word, movie_name, movie_year;
  std::istringstream iss (line);
  while (std::getline (users_file, line))
    {
      iss = std::istringstream (line);
      iss >> word;
      size_t sep_loc = word.find ("-");
      movie_name = word.substr (0, sep_loc);
      movie_year = word.substr (sep_loc + 1, word.size ());
      features_vec.clear ();
      while (iss >> word)
        {
          double value = std::stod (word);
          if (!is_valid_number (value))
            {
              throw std::out_of_range(OUT_OF_RANGE_MSG);
            }
          features_vec.push_back (std::stod (word));
        }
      rs->add_movie (movie_name,
                     std::stod (movie_year),
                     std::vector<double> (features_vec));
    }
  users_file.close ();
  return rs;
}

bool RecommenderSystemLoader::is_valid_number (const double &num)
{
  return UPPER_BOUND(num) && LOWER_BOUND(num);
}
