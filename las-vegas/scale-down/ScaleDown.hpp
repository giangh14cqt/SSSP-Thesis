#ifndef SD_H
#define SD_H

#include "../GraphBase.h"
#include "../low-diameter-decomposition/LDD.hpp"

/**
 * @brief Scale Down algorithm
 * @param G The origin graph
 * @param detal The detal of the scale down
 * @param B 
*/
double ScaleDown(Graph &G, int64_t detal, int64_t B);

#endif // SD_H