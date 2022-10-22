// import graphx as nx
#include <graphx/algorithms.approximation.hpp>  // import average_clustering

// This approximation has to be exact in regular graphs
// with no triangles or with all possible triangles.


auto test_petersen() -> void {
    // Actual coefficient is 0
    G = nx.petersen_graph();
    assert(average_clustering(G, trials=G.size() / 2) == nx.average_clustering(G));
}

auto test_petersen_seed() -> void {
    // Actual coefficient is 0
    G = nx.petersen_graph();
    assert(average_clustering(G, trials=G.size() / 2, seed=1) == nx.average_clustering(G));
}

auto test_tetrahedral() -> void {
    // Actual coefficient is 1
    G = nx.tetrahedral_graph();
    assert(average_clustering(G, trials=G.size() / 2) == nx.average_clustering(G));
}

auto test_dodecahedral() -> void {
    // Actual coefficient is 0
    G = nx.dodecahedral_graph();
    assert(average_clustering(G, trials=G.size() / 2) == nx.average_clustering(G));
}

auto test_empty() -> void {
    G = nx.empty_graph(5);
    assert(average_clustering(G, trials=G.size() / 2) == 0);
}

auto test_complete() -> void {
    G = nx.complete_graph(5);
    assert(average_clustering(G, trials=G.size() / 2) == 1);
    G = nx.complete_graph(7);
    assert(average_clustering(G, trials=G.size() / 2) == 1);
