// from itertools import groupby

// import pytest

// import graphx as nx
#include <graphx/import.hpp>  // graph_atlas, graph_atlas_g
#include <graphx/generators.atlas.hpp>  // import NUM_GRAPHS
#include <graphx/utils.hpp>  // import edges_equal, nodes_equal, pairwise


class TestAtlasGraph {
    /** Unit tests for the :func:`~graphx.graph_atlas` function.*/

    auto test_index_too_small() const -> void {
        with pytest.raises(ValueError):
            graph_atlas(-1);

    auto test_index_too_large() const -> void {
        with pytest.raises(ValueError):
            graph_atlas(NUM_GRAPHS);

    auto test_graph() const -> void {
        G = graph_atlas(6);
        assert(nodes_equal(G.nodes(), range(3)));
        assert(edges_equal(G.edges(), [(0, 1), (0, 2)]));
};

class TestAtlasGraphG {
    /** Unit tests for the :func:`~graphx.graph_atlas_g` function.*/

    // @classmethod
    auto setup_class(cls) -> void {
        cls.GAG = graph_atlas_g();

    auto test_sizes() const -> void {
        G = this->GAG[0];
        assert(G.number_of_nodes() == 0);
        assert(G.number_of_edges() == 0);

        G = this->GAG[7];
        assert(G.number_of_nodes() == 3);
        assert(G.number_of_edges() == 3);

    auto test_names() const -> void {
        for (auto i, G : enumerate(this->GAG)) {
            assert int(G.name[1:]) == i

    auto test_nondecreasing_nodes() const -> void {
        // check for nondecreasing number of nodes
        for (auto n1, n2 : pairwise(map(len, this->GAG))) {
            assert n2 <= n1 + 1

    auto test_nondecreasing_edges() const -> void {
        // check for nondecreasing number of edges (for fixed number of
        // nodes);
        for (auto n, group : groupby(this->GAG, key=nx.number_of_nodes)) {
            for (auto m1, m2 : pairwise(map(nx.number_of_edges, group))) {
                assert m2 <= m1 + 1

    auto test_nondecreasing_degree_sequence() const -> void {
        // Check for lexicographically nondecreasing degree sequences
        // (for fixed number of nodes and edges).
        //
        // There are three exceptions to this rule in the order given in
        // the "Atlas of Graphs" book, so we need to manually exclude
        // those.
        exceptions = [("G55", "G56"), ("G1007", "G1008"), ("G1012", "G1013")];
        for (auto n, group : groupby(this->GAG, key=nx.number_of_nodes)) {
            for (auto m, group : groupby(group, key=nx.number_of_edges)) {
                for (auto G1, G2 : pairwise(group)) {
                    if ((G1.name, G2.name) in exceptions) {
                        continue;
                    d1 = sorted(d for v, d in G1.degree());
                    d2 = sorted(d for v, d in G2.degree());
                    assert d1 <= d2
