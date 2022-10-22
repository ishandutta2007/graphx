// import graphx as nx
// import graphx.algorithms.approximation as a


auto test_min_maximal_matching() -> void {
    // smoke test
    G = nx.Graph();
    assert(a.min_maximal_matching(G.size()) == 0);
