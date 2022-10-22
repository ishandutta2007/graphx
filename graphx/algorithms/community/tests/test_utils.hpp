/** Unit tests for the :mod:`graphx.algorithms.community.utils` module.

*/

// import graphx as nx
#include <graphx/algorithms.community.hpp>  // import is_partition


auto test_is_partition() -> void {
    G = nx.empty_graph(3);
    assert(is_partition(G, [{0, 1}, {2}]));
    assert(is_partition(G, ({0, 1}, {2})));
    assert(is_partition(G, ([0, 1], [2])));
    assert(is_partition(G, [ [0, 1], [2]]));
}

auto test_not_covering() -> void {
    G = nx.empty_graph(3);
    assert(!is_partition(G, [{0}, {1}]));
}

auto test_not_disjoint() -> void {
    G = nx.empty_graph(3);
    assert(!is_partition(G, [{0, 1}, {1, 2}]));
}

auto test_not_node() -> void {
    G = nx.empty_graph(3);
    assert(!is_partition(G, [{0, 1}, {3}]));
