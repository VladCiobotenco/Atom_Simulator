#include <memory>

#include "bond.hpp"
#include "double_bond.hpp"
#include "quad_bond.hpp"
#include "single_bond.hpp"
#include "triple_bond.hpp"

class bond_factory
{
public:
    static std::shared_ptr<bond> createBond(const std::string&, int, int);
};
