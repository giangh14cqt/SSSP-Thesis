#ifndef SD_H
#define SD_H

#include "../GraphBase.h"
#include "../low-diameter-decomposition/LDD.hpp"
#include "FixDAGEdges.hpp"
#include "ElimNeg.hpp"

/**
 * @brief Scale Down algorithm
 * @param G The origin graph
 * @param detal The detal of the scale down
 * @param B 
*/
vector<int> ScaleDown(Graph &G, int64_t detal, int64_t B);

#endif // SD_H