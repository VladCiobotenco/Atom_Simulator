#include <iostream>
#include <fstream>
#include <string>
#include "atom.hpp"
#include "json.hpp"

using json=nlohmann::json;

std::vector<atom> readAtomsFromJson(const std::string& fileName)
{
    std::vector<atom> atomList;
    json jsonData;
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "Could not open file \"" << fileName << "\"" << std::endl;
        return atomList;
    }
    try
    {
        file>>jsonData;
    }
    catch (json::parse_error& e)
    {
        std::cerr<<"Error reading json file"<<e.what()<<"\n";
        file.close();
        return atomList;
    }

    file.close();
    int period, group,atomicNumber, atomicMass;
    std::string atomName, symbol;
    for (const auto& atomObject:jsonData)
    {
        period=atomObject.at("period").get<int>();
        group=atomObject.at("group").get<int>();
        atomicNumber=atomObject.at("atomicNumber").get<int>();
        atomicMass=atomObject.at("atomicMass").get<int>();
        atomName=atomObject.at("name").get<std::string>();
        symbol=atomObject.at("symbol").get<std::string>();
        atom temporaryAtom(period, group, atomicNumber, atomicMass, atomName, symbol);
        atomList.push_back(temporaryAtom);
    }
    return atomList;
}

