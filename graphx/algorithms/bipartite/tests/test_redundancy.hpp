/** Unit tests for the :mod:`graphx.algorithms.bipartite.redundancy` module.

*/

// import pytest

#include <graphx/import.hpp>  // NetworkXError, cycle_graph
#include <graphx/algorithms.bipartite.hpp>  // import complete_bipartite_graph, node_redundancy


auto test_no_redundant_nodes() -> void {
    G = complete_bipartite_graph(2, 2);
    rc = node_redundancy(G);
    assert(all(redundancy == 1 for redundancy in rc.values()));
}

auto test_redundant_nodes() -> void {
    G = cycle_graph(6);
    edge = {0, 3};
    G.add_edge(*edge);
    redundancy = node_redundancy(G);
    for (auto v : edge) {
        assert redundancy[v] == 2 / 3
    for (auto v : set(G) - edge) {
        assert(redundancy[v] == 1);
}

auto test_not_enough_neighbors() -> void {
    with pytest.raises(NetworkXError):
        G = complete_bipartite_graph(1, 2);
        node_redundancy(G);
