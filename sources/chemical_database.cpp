#include "../include/chemical_database.hpp"

#include "../libraries/json.hpp"
#include "../include/exceptions.hpp"
#include <fstream>
//#include <map>

using json = nlohmann::json;

void chemical_database::loadIntoDatabase(const std::string& fileName)
{
    std::ifstream file(fileName);
    json jsonData;
    if (!file.is_open())
        throw resourceMissingException(fileName);

    try {
        file >> jsonData;
        if (jsonData.empty())
            throw fileFormatException(fileName, "fisier gol");
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

std::ostream& operator<<(std::ostream& out, const chemical_database& thisDatabase)
{
    out << "\n=== Chemical Database Content ===\n";

    if (thisDatabase.database.empty()) {
        out << "[EMPTY] No substances loaded.\n";
        out << "=================================\n";
    }
    else
    {
        out << "Total Entries: " << thisDatabase.database.size() << "\n";
        out << "---------------------------------\n";
        out << std::left << std::setw(15) << "FORMULA" << "NAME\n";
        out << "---------------------------------\n";
        for (const auto& [formula, name] : thisDatabase.database)
            out << std::left << std::setw(15) << formula << name << "\n";
        out << "=================================\n\n";
    }

    return out;
}

