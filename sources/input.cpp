#include <iostream>
#include <fstream>
#include <string>

#include "../include/atom.hpp"
#include "../include/ion.hpp"
#include "../libraries/json.hpp"

#include "../include/exceptions.hpp"

using json=nlohmann::json;

std::vector<atom> readAtomsFromJson(const std::string& fileName)
{
    std::vector<atom> atomList;
    json jsonData;
    std::ifstream file(fileName);

    if (!file.is_open())
        throw fileLoadingException(fileName);

    try
    {
        file>>jsonData;
    }
    catch (json::parse_error&)
    {
        file.close();
        throw fileFormatException(fileName,"sintaxa JSON");
    }

    file.close();

    for (const auto& atomObject:jsonData)
    {
        int period_temporary = atomObject.at("period").get<int>();
        int group_temporary = atomObject.at("group").get<int>();
        int atomicNumber_temporary = atomObject.at("atomicNumber").get<int>();
        int atomicMass_temporary = atomObject.at("atomicMass").get<int>();
        std::string atomName_temporary = atomObject.at("name").get<std::string>();
        std::string symbol_temporary = atomObject.at("symbol").get<std::string>();
        try
        {
            atom temporaryAtom(atomName_temporary,period_temporary, group_temporary, atomicNumber_temporary, atomicMass_temporary, symbol_temporary);
            atomList.push_back(temporaryAtom);
        }
        catch (chemistryLawsException& e)
        {
            std::cerr << "EROARE: " << e.what() << "\n";
        }


    }
    return atomList;
}

std::vector<ion> readIonsFromJson(const std::string& fileName, const sf::Font font)
{
    std::vector<ion> ionList;
    json jsonData;
    std::ifstream file(fileName);

    if (!file.is_open())
        throw fileLoadingException(fileName);

    try
    {
        file>>jsonData;
    }
    catch (json::parse_error&)
    {
        file.close();
        throw fileFormatException(fileName,"sintaxa JSON");
    }

    file.close();
    for (const auto& ionObject:jsonData)
    {
        int period_temporary = ionObject.at("period").get<int>();
        int group_temporary = ionObject.at("group").get<int>();
        int atomicNumber_temporary = ionObject.at("atomicNumber").get<int>();
        int atomicMass_temporary = ionObject.at("atomicMass").get<int>();
        std::string atomName_temporary = ionObject.at("name").get<std::string>();
        std::string symbol_temporary = ionObject.at("symbol").get<std::string>();
        int ionCharge_temporary = ionObject.at("ionCharge").get<int>();
        ion temporaryIon(atomName_temporary,period_temporary, group_temporary, atomicNumber_temporary, atomicMass_temporary, symbol_temporary, ionCharge_temporary, font);
        ionList.push_back(temporaryIon);
    }
    return ionList;
}

