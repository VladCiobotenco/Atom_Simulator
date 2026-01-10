#include <fstream>
#include <iostream>
#include "include/input.hpp"

#include "include/simulator_manager.hpp"
#include "include/exceptions.hpp"

int main()
{
    try
    {
        simulator_manager& simulator=simulator_manager::getInstance();

        resolution thisResolution = simulator.newResolution();
        simulator.simulation(thisResolution);
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
