#include "../include/bond_factory.hpp"
#include "../include/exceptions.hpp"

std::shared_ptr<bond> bond_factory::createBond(const std::string& type, int atomIndex1, int atomIndex2)
{
    if (type == "single_bond")
        return std::make_shared<single_bond>(atomIndex1, atomIndex2);
    if (type == "double_bond")
        return std::make_shared<double_bond>(atomIndex1, atomIndex2);
    if (type == "triple_bond")
        return std::make_shared<triple_bond>(atomIndex1, atomIndex2);
    if (type == "quad_bond")
        return std::make_shared<quad_bond>(atomIndex1, atomIndex2);

    throw chemistryLawsException("Tip de legatura necunoscut: " + type);
}
