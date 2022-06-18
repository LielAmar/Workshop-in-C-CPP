#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "RSUser.h"
#include "RecommenderSystem.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class RSUsersLoader
{
  private:
    static void
    parse_movie_line (const std::string &line,
                      std::vector<std::pair<std::string, int> > &movies);

    static void
    parse_user_line (const std::string &line, std::vector<RSUser> &users,
                     const std::vector<std::pair<std::string, int> > &movies,
                     const std::shared_ptr<RecommenderSystem> &rs);

  public:
    RSUsersLoader () = delete;

    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users and their movie
     * ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser>
    create_users_from_file (const std::string &users_file_path,
                            std::unique_ptr<RecommenderSystem> rs)
        noexcept (false);
};

#endif // USERFACTORY_H
