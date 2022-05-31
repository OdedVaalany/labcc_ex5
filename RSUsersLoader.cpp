//
// Created by עודד ועלני on 26/05/2022.


#include "RSUsersLoader.h"
#define BUCKETS_NUMBER 11
#define OUT_OF_RANGE_MSG "rating value is out of 1-10 range"
#define UPPER_BOUND(args) args <= 10
#define LOWER_BOUND(args) args >= 1

std::vector<RSUser> RSUsersLoader::create_users_from_file
    (const std::string &users_file_path, RS_ptr& rs) noexcept (false)
{
  std::vector<RSUser> users;
  std::vector<std::string> movies_headers;
  std::ifstream users_file (users_file_path, std::ios::in);
  if (!users_file.is_open ())
    {
      users_file.close();
      throw std::runtime_error ("Not the right path");
    }
  std::string line, word, name;
  std::istringstream iss (line);
  if (std::getline (users_file, line))
    {
      iss = std::istringstream (line);
      while (iss >> word)
        {
          movies_headers.push_back (word);
        }
    }
  while (std::getline (users_file, line))
    {
      auto user_rank = rank_map (BUCKETS_NUMBER, sp_movie_hash,
                                 sp_movie_equal);
      iss = std::istringstream (line);
      iss >> name;
      int i = 0;
      while (iss >> word)
        {
          if (word != "NA" )
            {
              if(!is_valid_number (std::stod (word)))
                {
                  users_file.close();
                  throw std::out_of_range(OUT_OF_RANGE_MSG);
                }
              size_t sep_loc = movies_headers.at (i).find ('-');
              std::string movie_name = movies_headers.at (i).substr
                  (0, sep_loc);
              int movie_year = std::stoi (movies_headers.at (i).substr (
                  sep_loc + 1, movies_headers.at (i).size ()));
              sp_movie movie = rs->get_movie (movie_name, movie_year);
              user_rank.insert ({movie, std::stod (word)});
            }
          i++;
        }
      users.emplace_back(name, user_rank, rs);
    }
  users_file.close ();
  return users;
}

bool RSUsersLoader::is_valid_number (const double &num)
{
  return UPPER_BOUND(num) && LOWER_BOUND(num);
}

