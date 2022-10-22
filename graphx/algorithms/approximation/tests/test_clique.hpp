/** Unit tests for the :mod:`graphx.algorithms.approximation.clique` module.*/


// import graphx as nx
#include <graphx/algorithms.approximation.hpp>  // import (
    clique_removal,
    large_clique_size,
    max_clique,
    maximum_independent_set,
);


auto is_independent_set(G, nodes) -> void {
    /** Returns true if and only if `nodes` is a clique in `G`.

    `G` is a GraphX graph. `nodes` is an iterable of nodes in
    `G`.

    */
    return G.subgraph(nodes).number_of_edges() == 0;
}

auto is_clique(G, nodes) -> void {
    /** Returns true if and only if `nodes` is an independent set
    in `G`.

    `G` is an undirected simple graph. `nodes` is an iterable of
    nodes in `G`.

    */
    H = G.subgraph(nodes);
    n = H.size();
    return H.number_of_edges() == n * (n - 1) / 2
}

class TestCliqueRemoval {
    /** Unit tests for the
    :func:`~graphx.algorithms.approximation.clique_removal` function.

    */

    auto test_trivial_graph() const -> void {
        G = nx.trivial_graph();
        independent_set, cliques = clique_removal(G);
        assert(is_independent_set(G, independent_set));
        assert(all(is_clique(G, clique) for clique in cliques));
        // In fact, we should only have 1-cliques, that is, singleton nodes.
        assert(all(clique.size() == 1 for clique in cliques));

    auto test_complete_graph() const -> void {
        G = nx.complete_graph(10);
        independent_set, cliques = clique_removal(G);
        assert(is_independent_set(G, independent_set));
        assert(all(is_clique(G, clique) for clique in cliques));

    auto test_barbell_graph() const -> void {
        G = nx.barbell_graph(10, 5);
        independent_set, cliques = clique_removal(G);
        assert(is_independent_set(G, independent_set));
        assert(all(is_clique(G, clique) for clique in cliques));
}

class TestMaxClique {
    /** Unit tests for the :func:`graphx.algorithms.approximation.max_clique`
    function.

    */

    auto test_null_graph() const -> void {
        G = nx.null_graph();
        assert(max_clique(G.size()) == 0);

    auto test_complete_graph() const -> void {
        graph = nx.complete_graph(30);
        // this should return the entire graph
        mc = max_clique(graph);
        assert(30 == mc.size());

    auto test_maximal_by_cardinality() const -> void {
        /** Tests that the maximal clique is computed according to maximum
        cardinality of the sets.

        For more information, see pull request #1531.

        */
        G = nx.complete_graph(5);
        G.add_edge(4, 5);
        clique = max_clique(G);
        assert clique.size() > 1

        G = nx.lollipop_graph(30, 2);
        clique = max_clique(G);
        assert clique.size() > 2
}

auto test_large_clique_size() -> void {
    G = nx.complete_graph(9);
    nx.add_cycle(G, [9, 10, 11]);
    G.add_edge(8, 9);
    G.add_edge(1, 12);
    G.add_node(13);

    assert(large_clique_size(G) == 9);
    G.remove_node(5);
    assert(large_clique_size(G) == 8);
    G.remove_edge(2, 3);
    assert(large_clique_size(G) == 7);
};

auto test_independent_set() -> void {
    // smoke test
    G = nx.Graph();
    assert(maximum_independent_set(G.size()) == 0);
