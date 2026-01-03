#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "../include/atom.hpp"
#include "../include/ion.hpp"
#include "../libraries/json.hpp"

#include "../include/exceptions.hpp"

using json=nlohmann::json;

template <typename T>
class input {
public:
    template <typename... Args>
    static std::vector<T> read(const std::string& fileName, Args&&... args)
    {
        std::vector<T> entityList;
        std::ifstream file(fileName);

        if (!file.is_open())
            throw fileLoadingException(fileName);

        json jsonData;

        try {
            file >> jsonData;
        }
        catch (const json::parse_error&) {
            file.close();
            throw fileFormatException(fileName, "sintaxa JSON");
        }
        file.close();

        for (const auto& object : jsonData)
        {
            int period = object.at("period").get<int>();
            int group = object.at("group").get<int>();
            int atomicNumber = object.at("atomicNumber").get<int>();
            int atomicMass = object.at("atomicMass").get<int>();
            std::string name = object.at("name").get<std::string>();
            std::string symbol = object.at("symbol").get<std::string>();

            if constexpr (std::is_same_v<T, ion>)
            {
                int charge = object.at("ionCharge").get<int>();
                T temporaryEntity(name, period, group, atomicNumber, atomicMass, symbol, charge, std::forward<Args>(args)...);
                entityList.push_back(std::move(temporaryEntity));
            }
            else
            {
                T temporaryEntity(name, period, group, atomicNumber, atomicMass, symbol);
                entityList.push_back(std::move(temporaryEntity));
            }
        }

        return entityList;
    }
};

#endif