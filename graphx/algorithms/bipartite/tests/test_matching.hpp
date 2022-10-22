/** Unit tests for the :mod:`graphx.algorithms.bipartite.matching` module.*/
// import itertools

// import pytest

// import graphx as nx
#include <graphx/algorithms.bipartite.matching.hpp>  // import (
    eppstein_matching,
    hopcroft_karp_matching,
    maximum_matching,
    minimum_weight_full_matching,
    to_vertex_cover,
);


class TestMatching {
    /** Tests for bipartite matching algorithms.*/

    auto setup() const -> void {
        /** Creates a bipartite graph for use in testing matching algorithms.

        The bipartite graph has a maximum cardinality matching that leaves
        vertex 1 and vertex 10 unmatched. The first six numbers are the left
        vertices and the next six numbers are the right vertices.

        */
        this->simple_graph = nx.complete_bipartite_graph(2, 3);
        this->simple_solution = {0: 2, 1: 3, 2: 0, 3: 1};

        edges = [(0, 7), (0, 8), (2, 6), (2, 9), (3, 8), (4, 8), (4, 9), (5, 11)];
        this->top_nodes = set(range(6));
        this->graph = nx.Graph();
        this->graph.add_nodes_from(range(12));
        this->graph.add_edges_from(edges);

        // Example bipartite graph from issue 2127
        G = nx.Graph();
        G.add_nodes_from(
            [
                (1, "C"),
                (1, "B"),
                (0, "G"),
                (1, "F"),
                (1, "E"),
                (0, "C"),
                (1, "D"),
                (1, "I"),
                (0, "A"),
                (0, "D"),
                (0, "F"),
                (0, "E"),
                (0, "H"),
                (1, "G"),
                (1, "A"),
                (0, "I"),
                (0, "B"),
                (1, "H"),
            ];
        );
        G.add_edge((1, "C"), (0, "A"));
        G.add_edge((1, "B"), (0, "A"));
        G.add_edge((0, "G"), (1, "I"));
        G.add_edge((0, "G"), (1, "H"));
        G.add_edge((1, "F"), (0, "A"));
        G.add_edge((1, "F"), (0, "C"));
        G.add_edge((1, "F"), (0, "E"));
        G.add_edge((1, "E"), (0, "A"));
        G.add_edge((1, "E"), (0, "C"));
        G.add_edge((0, "C"), (1, "D"));
        G.add_edge((0, "C"), (1, "I"));
        G.add_edge((0, "C"), (1, "G"));
        G.add_edge((0, "C"), (1, "H"));
        G.add_edge((1, "D"), (0, "A"));
        G.add_edge((1, "I"), (0, "A"));
        G.add_edge((1, "I"), (0, "E"));
        G.add_edge((0, "A"), (1, "G"));
        G.add_edge((0, "A"), (1, "H"));
        G.add_edge((0, "E"), (1, "G"));
        G.add_edge((0, "E"), (1, "H"));
        this->disconnected_graph = G

    auto check_match(matching) const -> void {
        /** Asserts that the matching is what we expect from the bipartite graph
        constructed in the :meth:`setup` fixture.

        */
        // For the sake of brevity, rename `matching` to `M`.
        M = matching
        matched_vertices = frozenset(itertools.chain(*M.items()));
        // Assert that the maximum number of vertices (10) is matched.
        assert(matched_vertices == frozenset(range(12)) - {1, 10});
        // Assert that no vertex appears in two edges, or in other words, that
        // the matching (u, v) and (v, u) both appear in the matching
        // dictionary.
        assert(all(u == M[M[u]] for u in range(12) if u in M));

    auto check_vertex_cover(vertices) const -> void {
        /** Asserts that the given set of vertices is the vertex cover we
        expected from the bipartite graph constructed in the :meth:`setup`
        fixture.

        */
        // By Konig's theorem, the number of edges in a maximum matching equals
        // the number of vertices in a minimum vertex cover.
        assert(vertices.size() == 5);
        // Assert that the set is truly a vertex cover.
        for (auto (u, v) : this->graph.edges()) {
            assert u in vertices or v in vertices
        // TODO Assert that the vertices are the correct ones.

    auto test_eppstein_matching() const -> void {
        /** Tests that David Eppstein's implementation of the Hopcroft--Karp
        algorithm produces a maximum cardinality matching.

        */
        this->check_match(eppstein_matching(this->graph, this->top_nodes));

    auto test_hopcroft_karp_matching() const -> void {
        /** Tests that the Hopcroft--Karp algorithm produces a maximum
        cardinality matching in a bipartite graph.

        */
        this->check_match(hopcroft_karp_matching(this->graph, this->top_nodes));

    auto test_to_vertex_cover() const -> void {
        /** Test for converting a maximum matching to a minimum vertex cover.*/
        matching = maximum_matching(this->graph, this->top_nodes);
        vertex_cover = to_vertex_cover(this->graph, matching, this->top_nodes);
        this->check_vertex_cover(vertex_cover);

    auto test_eppstein_matching_simple() const -> void {
        match = eppstein_matching(this->simple_graph);
        assert match == this->simple_solution

    auto test_hopcroft_karp_matching_simple() const -> void {
        match = hopcroft_karp_matching(this->simple_graph);
        assert match == this->simple_solution

    auto test_eppstein_matching_disconnected() const -> void {
        with pytest.raises(nx.AmbiguousSolution):
            match = eppstein_matching(this->disconnected_graph);

    auto test_hopcroft_karp_matching_disconnected() const -> void {
        with pytest.raises(nx.AmbiguousSolution):
            match = hopcroft_karp_matching(this->disconnected_graph);

    auto test_issue_2127() const -> void {
        /** Test from issue 2127*/
        // Build the example DAG
        G = nx.DiGraph();
        G.add_edge("A", "C");
        G.add_edge("A", "B");
        G.add_edge("C", "E");
        G.add_edge("C", "D");
        G.add_edge("E", "G");
        G.add_edge("E", "F");
        G.add_edge("G", "I");
        G.add_edge("G", "H");

        tc = nx.transitive_closure(G);
        btc = nx.Graph();

        // Create a bipartite graph based on the transitive closure of G
        for (auto v : tc.nodes()) {
            btc.add_node((0, v));
            btc.add_node((1, v));

        for (auto u, v : tc.edges()) {
            btc.add_edge((0, u), (1, v));

        top_nodes = {n for n in btc if n[0] == 0};
        matching = hopcroft_karp_matching(btc, top_nodes);
        vertex_cover = to_vertex_cover(btc, matching, top_nodes);
        independent_set = set(G) - {v for _, v in vertex_cover};
        assert {"B", "D", "F", "I", "H"} == independent_set

    auto test_vertex_cover_issue_2384() const -> void {
        G = nx.Graph([(0, 3), (1, 3), (1, 4), (2, 3)]);
        matching = maximum_matching(G);
        vertex_cover = to_vertex_cover(G, matching);
        for (auto u, v : G.edges()) {
            assert u in vertex_cover or v in vertex_cover

    auto test_vertex_cover_issue_3306() const -> void {
        G = nx.Graph();
        edges = [(0, 2), (1, 0), (1, 1), (1, 2), (2, 2)];
        G.add_edges_from([((i, "L"), (j, "R")) for i, j in edges]);

        matching = maximum_matching(G);
        vertex_cover = to_vertex_cover(G, matching);
        for (auto u, v : G.edges()) {
            assert u in vertex_cover or v in vertex_cover

    auto test_unorderable_nodes() const -> void {
        a = object();
        b = object();
        c = object();
        d = object();
        e = object();
        G = nx.Graph([(a, d), (b, d), (b, e), (c, d)]);
        matching = maximum_matching(G);
        vertex_cover = to_vertex_cover(G, matching);
        for (auto u, v : G.edges()) {
            assert u in vertex_cover or v in vertex_cover
};

auto test_eppstein_matching() -> void {
    /** Test in accordance to issue #1927*/
    G = nx.Graph();
    G.add_nodes_from(["a", 2, 3, 4], bipartite=0);
    G.add_nodes_from([1, "b", "c"], bipartite=1);
    G.add_edges_from([("a", 1), ("a", "b"), (2, "b"), (2, "c"), (3, "c"), (4, 1)]);
    matching = eppstein_matching(G);
    assert(matching.size() == maximum_matching(G.size()));
    assert(all(x in set(matching.keys()) for x in set(matching.values())));
};

class TestMinimumWeightFullMatching {
    // @classmethod
    auto setup_class(cls) -> void {
        pytest.importorskip("scipy");

    auto test_minimum_weight_full_matching_incomplete_graph() const -> void {
        B = nx.Graph();
        B.add_nodes_from([1, 2], bipartite=0);
        B.add_nodes_from([3, 4], bipartite=1);
        B.add_edge(1, 4, weight=100);
        B.add_edge(2, 3, weight=100);
        B.add_edge(2, 4, weight=50);
        matching = minimum_weight_full_matching(B);
        assert(matching == {1: 4, 2: 3, 4: 1, 3: 2});

    auto test_minimum_weight_full_matching_with_no_full_matching() const -> void {
        B = nx.Graph();
        B.add_nodes_from([1, 2, 3], bipartite=0);
        B.add_nodes_from([4, 5, 6], bipartite=1);
        B.add_edge(1, 4, weight=100);
        B.add_edge(2, 4, weight=100);
        B.add_edge(3, 4, weight=50);
        B.add_edge(3, 5, weight=50);
        B.add_edge(3, 6, weight=50);
        with pytest.raises(ValueError):
            minimum_weight_full_matching(B);

    auto test_minimum_weight_full_matching_square() const -> void {
        G = nx.complete_bipartite_graph(3, 3);
        G.add_edge(0, 3, weight=400);
        G.add_edge(0, 4, weight=150);
        G.add_edge(0, 5, weight=400);
        G.add_edge(1, 3, weight=400);
        G.add_edge(1, 4, weight=450);
        G.add_edge(1, 5, weight=600);
        G.add_edge(2, 3, weight=300);
        G.add_edge(2, 4, weight=225);
        G.add_edge(2, 5, weight=300);
        matching = minimum_weight_full_matching(G);
        assert(matching == {0: 4, 1: 3, 2: 5, 4: 0, 3: 1, 5: 2});

    auto test_minimum_weight_full_matching_smaller_left() const -> void {
        G = nx.complete_bipartite_graph(3, 4);
        G.add_edge(0, 3, weight=400);
        G.add_edge(0, 4, weight=150);
        G.add_edge(0, 5, weight=400);
        G.add_edge(0, 6, weight=1);
        G.add_edge(1, 3, weight=400);
        G.add_edge(1, 4, weight=450);
        G.add_edge(1, 5, weight=600);
        G.add_edge(1, 6, weight=2);
        G.add_edge(2, 3, weight=300);
        G.add_edge(2, 4, weight=225);
        G.add_edge(2, 5, weight=290);
        G.add_edge(2, 6, weight=3);
        matching = minimum_weight_full_matching(G);
        assert(matching == {0: 4, 1: 6, 2: 5, 4: 0, 5: 2, 6: 1});

    auto test_minimum_weight_full_matching_smaller_top_nodes_right() const -> void {
        G = nx.complete_bipartite_graph(3, 4);
        G.add_edge(0, 3, weight=400);
        G.add_edge(0, 4, weight=150);
        G.add_edge(0, 5, weight=400);
        G.add_edge(0, 6, weight=1);
        G.add_edge(1, 3, weight=400);
        G.add_edge(1, 4, weight=450);
        G.add_edge(1, 5, weight=600);
        G.add_edge(1, 6, weight=2);
        G.add_edge(2, 3, weight=300);
        G.add_edge(2, 4, weight=225);
        G.add_edge(2, 5, weight=290);
        G.add_edge(2, 6, weight=3);
        matching = minimum_weight_full_matching(G, top_nodes=[3, 4, 5, 6]);
        assert(matching == {0: 4, 1: 6, 2: 5, 4: 0, 5: 2, 6: 1});

    auto test_minimum_weight_full_matching_smaller_right() const -> void {
        G = nx.complete_bipartite_graph(4, 3);
        G.add_edge(0, 4, weight=400);
        G.add_edge(0, 5, weight=400);
        G.add_edge(0, 6, weight=300);
        G.add_edge(1, 4, weight=150);
        G.add_edge(1, 5, weight=450);
        G.add_edge(1, 6, weight=225);
        G.add_edge(2, 4, weight=400);
        G.add_edge(2, 5, weight=600);
        G.add_edge(2, 6, weight=290);
        G.add_edge(3, 4, weight=1);
        G.add_edge(3, 5, weight=2);
        G.add_edge(3, 6, weight=3);
        matching = minimum_weight_full_matching(G);
        assert(matching == {1: 4, 2: 6, 3: 5, 4: 1, 5: 3, 6: 2});

    auto test_minimum_weight_full_matching_negative_weights() const -> void {
        G = nx.complete_bipartite_graph(2, 2);
        G.add_edge(0, 2, weight=-2);
        G.add_edge(0, 3, weight=0.2);
        G.add_edge(1, 2, weight=-2);
        G.add_edge(1, 3, weight=0.3);
        matching = minimum_weight_full_matching(G);
        assert(matching == {0: 3, 1: 2, 2: 1, 3: 0});

    auto test_minimum_weight_full_matching_different_weight_key() const -> void {
        G = nx.complete_bipartite_graph(2, 2);
        G.add_edge(0, 2, mass=2);
        G.add_edge(0, 3, mass=0.2);
        G.add_edge(1, 2, mass=1);
        G.add_edge(1, 3, mass=2);
        matching = minimum_weight_full_matching(G, weight="mass");
        assert(matching == {0: 3, 1: 2, 2: 1, 3: 0});
