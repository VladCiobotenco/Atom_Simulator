#include <utility>

#include "../include/leaderboard.hpp"

#include <algorithm>
#include <fstream>

leaderboard::leaderboard(std::string file): filename(std::move(file))
{
    load();
}

void leaderboard::addScore(const std::string& name, int score)
{
    scores.push_back({score,name});

    std::sort(scores.begin(),scores.end(), std::greater<std::pair<int,std::string>>());

    if (scores.size() > 10) {
        scores.resize(10);
    }
    save();
}

void leaderboard::load() {
    scores.clear();
    std::ifstream in(filename);
    std::string name; int score;

    while (in>>score>>name)
        scores.push_back({score,name});

    std::sort(scores.begin(),scores.end(), std::greater<std::pair<int,std::string>>());
}

void leaderboard::save() const
{
    std::ofstream out(filename);
    for (const auto& entity: scores)
        out<<entity.first<<" "<<entity.second<<"\n";
    out.close();
}

const std::vector<std::pair<int, std::string>>& leaderboard::getScores() const
{
    return scores;
}




