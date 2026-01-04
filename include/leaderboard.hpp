#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP
#include <string>
#include <vector>

class leaderboard {
    std::string filename;
    std::vector<std::pair<int,std::string>> scores;
public:
    explicit leaderboard(std::string);

    void addScore(const std::string&, int);
    void load();
    void save() const;
    [[nodiscard]] std::vector<std::pair<int, std::string>> getScores() const;
};

#endif