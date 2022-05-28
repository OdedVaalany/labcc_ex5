//
// Created by עודד ועלני on 26/05/2022.
//
#include "RecommenderSystemLoader.h"

RS_ptr
RecommenderSystemLoader::create_rs_from_movies_file (const std::string &movies_file_path) noexcept (false)
{
  std::shared_ptr<RecommenderSystem> RS = std::make_shared<RecommenderSystem>(RecommenderSystem());
  std::ifstream users_file (movies_file_path, std::ios::in);
  std::vector<double> features_vec;
  if (!users_file.is_open ())
    {
      throw std::runtime_error ("Not the right path");
    }
  std::string line,word,movie_name,movie_year;
  std::istringstream iss(line);
  while (std::getline (users_file, line))
    {
      iss= std::istringstream (line);
      iss >> word;
      size_t sep_loc = word.find ("-");
      movie_name = word.substr (0,sep_loc);
      movie_year = word.substr (sep_loc+1,word.size());
      features_vec.clear();
      while (iss >> word)
        {
          double value = std::stod (word);
          if(!is_valid_number (value))
            {
              break;
            }
          features_vec.push_back (std::stod (word));
        }
        RS->add_movie (movie_name,std::stod(movie_year), std::vector<double>(features_vec));
    }
  users_file.close();
  return RS;
}

bool RecommenderSystemLoader::is_valid_number(double& num)
{
  return 10 >= num && num >= 1 ;
}
