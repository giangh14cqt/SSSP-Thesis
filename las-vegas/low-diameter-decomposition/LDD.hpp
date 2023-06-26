#ifndef LDD_H
#define LDD_H

#include "../GraphBase.h"

const int64_t C = 50000; // constant for LDD

/**
 * @brief Low Diameter Decomposition
 * @param G The origin graph
 * @param d The diameter of the LDD
 * @return The edge set of the LDD with format {u, v, w} - from u to v with weight w
*/
vector<iii> LDD(Graph &G, int d);

/**
 * @brief Get the cross vertices of two balls
 * @param a The first ball
 * @param b The second ball
 * @return The cross vertices of two balls
*/
vector<int> get_cross(vector<int> a, vector<int> b);
/**
 * @brief Merge two edge sets to one
 * @param erem The first edge set
 * @param e_other The second edge set
*/
void get_cross_e(vector<iii> &erem, vector<iii> e_other);
/**
 * @brief Get the boundary of two balls
 * @param G The origin graph
 * @param ball_in The first ball
 * @param ball_out The second ball
*/
vector<iii> get_boundary(Graph &G, vector<int> &ball_in, vector<int> &ball_out);
/**
 * @brief To check if a ball contains a light vertex and return it
 * @param weight The weight of each vertex
 * @param G The origin graph
 * @return The light vertex if the ball contains one, otherwise -1
*/
int contain_light(vector<Weight> &weight, Graph &G);
/**
 * @brief Computer the ball in a graph
 * @param G The origin graph
 * @param s The source vertex
 * @param d The diameter of the ball
 * @return The ball in from s with diameter d
*/
vector<int> computer_ball_in(Graph &G, int s, int d);
/**
 * @brief Computer the ball out a graph
 * @param G The origin graph
 * @param s The source vertex
 * @param d The diameter of the ball
 * @return The ball out from s with diameter d
*/
vector<int> computer_ball_out(Graph &G, int s, int d);
/**
 * @brief To check if a set is a subset of another set
 * @param a The first set
 * @param b The second set
 * @return True if a is a subset of b, otherwise false
*/
bool is_sub_set(const vector<int> &a, const vector<int> &b);
/**
 * @brief To check if a vertex is in a set
 * @param x The vertex
 * @param a The set
 * @return True if x is in a, otherwise false
*/
bool is_in(int x, const vector<int> &a);

#endif // LDD_H