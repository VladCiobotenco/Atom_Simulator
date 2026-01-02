#include <atomic>
#include <fstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>


#include "include/atom.hpp"
#include "include/molecule.hpp"
#include "include/input.hpp"

#include "include/simulator_manager.hpp"
#include "include/audio_manager.hpp"
#include "include/chemical_database.hpp"
#include "include/exceptions.hpp"

simulator_manager simulator_manager::instance;

int main()
{
    try
    {
        simulator_manager& simulator=simulator_manager::getInstance();

        simulator.simulation();
    }

    catch (const fileLoadingException& e)
    {
        std::cerr << "EROARE: " << e.what() << "\n";
        std::cerr << "Verificati daca fisierul specificat se afla la locatia corecta!\n";
        return 1;
    }

    catch (const fileFormatException& e)
    {
        std::cerr << "EROARE: " << e.what() << "\n";
        std::cerr << "Verificati daca fisierul specificat contine informatii corecte!\n";
        return 1;
    }

    catch (const resourceMissingException& e)
    {
        std::cerr << "EROARE: " << e.what() << "\n";
        std::cerr << "Verificati daca resursa specificata exista!\n";
        return 1;
    }

    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }


    return 0;
}
