#include <cassert>
#include <iostream>

#include "../src/Dijkstra.h"
#include "libs/catch.hpp"

TEST_CASE("Test case 1", "[Dijkstra]")
{
    // Test case 1
    Graph g1(5);
    g1.add_edge(0, 1, 2);
    g1.add_edge(0, 2, 4);
    g1.add_edge(1, 2, 1);
    g1.add_edge(1, 3, 5);
    g1.add_edge(2, 3, 1);
    g1.add_edge(2, 4, 3);
    g1.add_edge(3, 4, 2);
    g1.dijkstra(0);
    REQUIRE(g1.get_shortest_distance(1) == 2);
    REQUIRE(g1.get_shortest_distance(2) == 3);
    REQUIRE(g1.get_shortest_distance(3) == 4);
    REQUIRE(g1.get_shortest_distance(4) == 6);
}

TEST_CASE("Test case 2", "[Dijkstra]")
{
    // Test case 2
    Graph g2(4);
    g2.add_edge(0, 1, 5);
    g2.add_edge(0, 2, 2);
    g2.add_edge(2, 1, 1);
    g2.add_edge(1, 3, 1);
    g2.dijkstra(0);
    REQUIRE(g2.get_shortest_distance(1) == 3);
    REQUIRE(g2.get_shortest_distance(2) == 2);
    REQUIRE(g2.get_shortest_distance(3) == 4);
}
