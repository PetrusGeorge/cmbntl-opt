#ifndef GC_HPP
#define GC_HPP

#define EPS 0.00001

#include <vector>

#include "Instance.hpp"
#include "MasterProblem.hpp"
#include "SubProblem.hpp"


std::vector<double>* GCMinknap(Instance *instance, MasterProblem *master);
std::vector<double>* GC(Instance *instance, MasterProblem *master, SubProblem *sub);

#endif