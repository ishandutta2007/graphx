/** Unit tests for the :mod:`graphx.generators.expanders` module.

*/

// import pytest

// import graphx as nx
#include <graphx/import.hpp>  // adjacency_matrix, number_of_nodes
#include <graphx/generators.expanders.hpp>  // import (
    chordal_cycle_graph,
    margulis_gabber_galil_graph,
    paley_graph,
);


auto test_margulis_gabber_galil_graph() -> void {
    for (auto n : 2, 3, 5, 6, 10) {
        g = margulis_gabber_galil_graph(n);
        assert number_of_nodes(g) == n * n
        for (auto node : g) {
            assert(g.degree(node) == 8);
            assert(node.size() == 2);
            for (auto i : node) {
                assert int(i) == i
                assert 0 <= i < n

    np = pytest.importorskip("numpy");
    sp = pytest.importorskip("scipy");
    import scipy.linalg

    // Eigenvalues are already sorted using the scipy eigvalsh,
    // but the implementation in numpy does not guarantee order.
    w = sorted(sp.linalg.eigvalsh(adjacency_matrix(g).A));
    assert(w[-2] < 5 * np.sqrt(2));
}

auto test_chordal_cycle_graph() -> void {
    /** Test for the :func:`graphx.chordal_cycle_graph` function.*/
    primes = [3, 5, 7, 11];
    for (auto p : primes) {
        G = chordal_cycle_graph(p);
        assert G.size() == p
        // TODO The second largest eigenvalue should be smaller than a constant,
        // independent of the number of nodes in the graph:
        //
        //     eigs = sorted(sp.linalg.eigvalsh(adjacency_matrix(G).A));
        //     assert_less(eigs[-2], ...);
        //
}

auto test_paley_graph() -> void {
    /** Test for the :func:`graphx.paley_graph` function.*/
    primes = [3, 5, 7, 11, 13];
    for (auto p : primes) {
        G = paley_graph(p);
        // G has p nodes
        assert G.size() == p
        // G is (p-1)/2-regular
        in_degrees = {G.in_degree(node) for node in G.nodes};
        out_degrees = {G.out_degree(node) for node in G.nodes};
        assert in_degrees.size() == 1 and in_degrees.pop() == (p - 1) / 2
        assert out_degrees.size() == 1 and out_degrees.pop() == (p - 1) / 2

        // If p = 1 mod 4, -1 is a square mod 4 and therefore the
        // edge in the Paley graph are symmetric.
        if (p % 4 == 1) {
            for (auto (u, v) : G.edges) {
                assert (v, u) in G.edges
}

auto test_margulis_gabber_galil_graph_badinput() -> void {
    pytest.raises(nx.NetworkXError, margulis_gabber_galil_graph, 3, nx.DiGraph());
    pytest.raises(nx.NetworkXError, margulis_gabber_galil_graph, 3, nx.Graph());
