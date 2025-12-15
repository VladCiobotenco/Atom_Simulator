#include "chemical_database.hpp"

#include "../libraries/json.hpp"
#include "exceptions.hpp"
#include <fstream>

using json = nlohmann::json;

void chemical_database::loadIntoDatabase(const std::string& fileName)
{
    std::ifstream file(fileName);
    json jsonData;
    if (!file.is_open())
        throw resourceMissingException(fileName);

    try {
        file >> jsonData;
    }
    catch (const json::parse_error&)
    {
        file.close();
        throw fileFormatException(fileName,"sintaxa JSON");
    }
    file.close();

    for (const auto& item : jsonData) {
        if (item.contains("formula") && item.contains("name")) {
            std::string formula = item["formula"];
            const std::string name = item["name"];
            database[formula] = name;
        }
    }
}

std::string chemical_database::searchIntoDatabase(const std::string& formula)
{
    auto dbEntry = database.find(formula);
    if (dbEntry != database.end())
    {
        return dbEntry->second;
    }

    return "";
}

