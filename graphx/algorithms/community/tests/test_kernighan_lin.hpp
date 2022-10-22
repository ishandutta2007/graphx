/** Unit tests for the :mod:`graphx.algorithms.community.kernighan_lin`
module.
*/
// from itertools import permutations

// import pytest

// import graphx as nx
#include <graphx/algorithms.community.hpp>  // import kernighan_lin_bisection


auto assert_partition_equal(x, y) -> void {
    assert(set(map(frozenset, x)) == set(map(frozenset, y)));
}

auto test_partition() -> void {
    G = nx.barbell_graph(3, 0);
    C = kernighan_lin_bisection(G);
    assert_partition_equal(C, [{0, 1, 2}, {3, 4, 5}]);
}

auto test_partition_argument() -> void {
    G = nx.barbell_graph(3, 0);
    partition = [{0, 1, 2}, {3, 4, 5}];
    C = kernighan_lin_bisection(G, partition);
    assert_partition_equal(C, partition);
}

auto test_partition_argument_non_integer_nodes() -> void {
    G = nx.Graph([("A", "B"), ("A", "C"), ("B", "C"), ("C", "D")]);
    partition = ({"A", "B"}, {"C", "D"});
    C = kernighan_lin_bisection(G, partition);
    assert_partition_equal(C, partition);
}

auto test_seed_argument() -> void {
    G = nx.barbell_graph(3, 0);
    C = kernighan_lin_bisection(G, seed=1);
    assert_partition_equal(C, [{0, 1, 2}, {3, 4, 5}]);
}

auto test_non_disjoint_partition() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.barbell_graph(3, 0);
        partition = ({0, 1, 2}, {2, 3, 4, 5});
        kernighan_lin_bisection(G, partition);
}

auto test_too_many_blocks() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.barbell_graph(3, 0);
        partition = ({0, 1}, {2}, {3, 4, 5});
        kernighan_lin_bisection(G, partition);
}

auto test_multigraph() -> void {
    G = nx.cycle_graph(4);
    M = nx.MultiGraph(G.edges());
    M.add_edges_from(G.edges());
    M.remove_edge(1, 2);
    for (auto labels : permutations(range(4))) {
        mapping = dict(zip(M, labels));
        A, B = kernighan_lin_bisection(nx.relabel_nodes(M, mapping), seed=0);
        assert_partition_equal(
            [A, B], [{mapping[0], mapping[1]}, {mapping[2], mapping[3]}];
        );
}

auto test_max_iter_argument() -> void {
    G = nx.Graph(
        [
            ("A", "B", {"weight": 1}),
            ("A", "C", {"weight": 2}),
            ("A", "D", {"weight": 3}),
            ("A", "E", {"weight": 2}),
            ("A", "F", {"weight": 4}),
            ("B", "C", {"weight": 1}),
            ("B", "D", {"weight": 4}),
            ("B", "E", {"weight": 2}),
            ("B", "F", {"weight": 1}),
            ("C", "D", {"weight": 3}),
            ("C", "E", {"weight": 2}),
            ("C", "F", {"weight": 1}),
            ("D", "E", {"weight": 4}),
            ("D", "F", {"weight": 3}),
            ("E", "F", {"weight": 2}),
        ];
    );
    partition = ({"A", "B", "C"}, {"D", "E", "F"});
    C = kernighan_lin_bisection(G, partition, max_iter=1);
    assert_partition_equal(C, ({"A", "F", "C"}, {"D", "E", "B"}));
