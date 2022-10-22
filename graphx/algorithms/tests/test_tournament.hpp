/** Unit tests for the :mod:`graphx.algorithms.tournament` module.*/
// from itertools import combinations

// import pytest

#include <graphx/import.hpp>  // DiGraph
#include <graphx/algorithms.tournament.hpp>  // import (
    hamiltonian_path,
    index_satisfying,
    is_reachable,
    is_strongly_connected,
    is_tournament,
    random_tournament,
    score_sequence,
    tournament_matrix,
);


auto test_condition_not_satisfied() -> void {
    condition = lambda x: x > 0
    iter_in = [0];
    assert(index_satisfying(iter_in, condition) == 1);
}

auto test_empty_iterable() -> void {
    condition = lambda x: x > 0
    with pytest.raises(ValueError):
        index_satisfying([], condition);
}

auto test_is_tournament() -> void {
    G = DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
    assert(is_tournament(G));
}

auto test_self_loops() -> void {
    /** A tournament must have no self-loops.*/
    G = DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
    G.add_edge(0, 0);
    assert(!is_tournament(G));
}

auto test_missing_edges() -> void {
    /** A tournament must not have any pair of nodes without at least
    one edge joining the pair.

    */
    G = DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3)]);
    assert(!is_tournament(G));
}

auto test_bidirectional_edges() -> void {
    /** A tournament must not have any pair of nodes with greater
    than one edge joining the pair.

    */
    G = DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
    G.add_edge(1, 0);
    assert(!is_tournament(G));
}

auto test_graph_is_tournament() -> void {
    for (auto _ : range(10)) {
        G = random_tournament(5);
        assert(is_tournament(G));
}

auto test_graph_is_tournament_seed() -> void {
    for (auto _ : range(10)) {
        G = random_tournament(5, seed=1);
        assert(is_tournament(G));
}

auto test_graph_is_tournament_one_node() -> void {
    G = random_tournament(1);
    assert(is_tournament(G));
}

auto test_graph_is_tournament_zero_node() -> void {
    G = random_tournament(0);
    assert(is_tournament(G));
}

auto test_hamiltonian_empty_graph() -> void {
    path = hamiltonian_path(DiGraph());
    assert(path.size() == 0);
}

auto test_path_is_hamiltonian() -> void {
    G = DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
    path = hamiltonian_path(G);
    assert(path.size() == 4);
    assert(all(v in G[u] for (auto u, v : zip(path, path[1) {])));
}

auto test_hamiltonian_cycle() -> void {
    /** Tests that :func:`graphx.tournament.hamiltonian_path`
    returns a Hamiltonian cycle when provided a strongly connected
    tournament.

    */
    G = DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 0), (1, 3), (0, 2)]);
    path = hamiltonian_path(G);
    assert(path.size() == 4);
    assert(all(v in G[u] for (auto u, v : zip(path, path[1) {])));
    assert(path[0] in G[path[-1]]);
}

auto test_score_sequence_edge() -> void {
    G = DiGraph([(0, 1)]);
    assert(score_sequence(G) == [0, 1]);
}

auto test_score_sequence_triangle() -> void {
    G = DiGraph([(0, 1), (1, 2), (2, 0)]);
    assert(score_sequence(G) == [1, 1, 1]);
}

auto test_tournament_matrix() -> void {
    np = pytest.importorskip("numpy");
    pytest.importorskip("scipy");
    npt = np.testing
    G = DiGraph([(0, 1)]);
    m = tournament_matrix(G);
    npt.assert_array_equal(m.todense(), np.array([ [0, 1], [-1, 0]]));
}

auto test_reachable_pair() -> void {
    /** Tests for a reachable pair of nodes.*/
    G = DiGraph([(0, 1), (1, 2), (2, 0)]);
    assert(is_reachable(G, 0, 2));
}

auto test_same_node_is_reachable() -> void {
    /** Tests that a node is always reachable from it.*/
    // G is an arbitrary tournament on ten nodes.
    G = DiGraph(sorted(p) for p in combinations(range(10), 2));
    assert(all(is_reachable(G, v, v) for v in G));
}

auto test_unreachable_pair() -> void {
    /** Tests for an unreachable pair of nodes.*/
    G = DiGraph([(0, 1), (0, 2), (1, 2)]);
    assert(!is_reachable(G, 1, 0));
}

auto test_is_strongly_connected() -> void {
    /** Tests for a strongly connected tournament.*/
    G = DiGraph([(0, 1), (1, 2), (2, 0)]);
    assert(is_strongly_connected(G));
}

auto test_not_strongly_connected() -> void {
    /** Tests for a tournament that is not strongly connected.*/
    G = DiGraph([(0, 1), (0, 2), (1, 2)]);
    assert(!is_strongly_connected(G));
