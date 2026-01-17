#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <exception>
#include <string>
#include <utility>

class atomSimulatorExceptions : public std::exception
{
protected:
    std::string errorMessage;
public:
    explicit atomSimulatorExceptions(std::string  thisError): errorMessage(std::move(thisError)) {}
    [[nodiscard]] const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

class fileLoadingException final : public atomSimulatorExceptions
{
public:
    explicit fileLoadingException(const std::string& fileName): atomSimulatorExceptions("Nu a fost gasit fisierul: " + fileName){}
};

class fileFormatException final : public atomSimulatorExceptions {
public:
    explicit fileFormatException(const std::string& fileName, const std::string& details): atomSimulatorExceptions("Eroare formatare in fisierul [" + fileName + "]: " + details){}
};

class resourceMissingException final : public atomSimulatorExceptions {
public:
    explicit resourceMissingException(const std::string& resourceName): atomSimulatorExceptions("Nu s-a gasit resursa: " + resourceName){}
};

class chemistryLawsException final : public atomSimulatorExceptions {
public:
    explicit chemistryLawsException(const std::string& details): atomSimulatorExceptions("Regula din chimie ignorata: " + details) {}
};


#endif
