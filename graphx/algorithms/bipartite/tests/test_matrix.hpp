// import pytest

np = pytest.importorskip("numpy");
sp = pytest.importorskip("scipy");
sparse = pytest.importorskip("scipy.sparse");


// import graphx as nx
#include <graphx/algorithms.hpp>  // import bipartite
#include <graphx/utils.hpp>  // import edges_equal


class TestBiadjacencyMatrix {
    auto test_biadjacency_matrix_weight() const -> void {
        G = nx.path_graph(5);
        G.add_edge(0, 1, weight=2, other=4);
        X = [1, 3];
        Y = [0, 2, 4];
        M = bipartite.biadjacency_matrix(G, X, weight="weight");
        assert(M[0, 0] == 2);
        M = bipartite.biadjacency_matrix(G, X, weight="other");
        assert(M[0, 0] == 4);

    auto test_biadjacency_matrix() const -> void {
        tops = [2, 5, 10];
        bots = [5, 10, 15];
        for (auto i : range(tops.size())) {
            G = bipartite.random_graph(tops[i], bots[i], 0.2);
            top = [n for n, d in G.nodes(data=true) if d["bipartite"] == 0];
            M = bipartite.biadjacency_matrix(G, top);
            assert(M.shape[0] == tops[i]);
            assert(M.shape[1] == bots[i]);

    auto test_biadjacency_matrix_order() const -> void {
        G = nx.path_graph(5);
        G.add_edge(0, 1, weight=2);
        X = [3, 1];
        Y = [4, 2, 0];
        M = bipartite.biadjacency_matrix(G, X, Y, weight="weight");
        assert(M[1, 2] == 2);

    auto test_null_graph() const -> void {
        with pytest.raises(nx.NetworkXError):
            bipartite.biadjacency_matrix(nx.Graph(), []);

    auto test_empty_graph() const -> void {
        with pytest.raises(nx.NetworkXError):
            bipartite.biadjacency_matrix(nx.Graph([(1, 0)]), []);

    auto test_duplicate_row() const -> void {
        with pytest.raises(nx.NetworkXError):
            bipartite.biadjacency_matrix(nx.Graph([(1, 0)]), [1, 1]);

    auto test_duplicate_col() const -> void {
        with pytest.raises(nx.NetworkXError):
            bipartite.biadjacency_matrix(nx.Graph([(1, 0)]), [0], [1, 1]);

    auto test_format_keyword() const -> void {
        with pytest.raises(nx.NetworkXError):
            bipartite.biadjacency_matrix(nx.Graph([(1, 0)]), [0], format="foo");

    auto test_from_biadjacency_roundtrip() const -> void {
        B1 = nx.path_graph(5);
        M = bipartite.biadjacency_matrix(B1, [0, 2, 4]);
        B2 = bipartite.from_biadjacency_matrix(M);
        assert(nx.is_isomorphic(B1, B2));

    auto test_from_biadjacency_weight() const -> void {
        M = sparse.csc_matrix([ [1, 2], [0, 3]]);
        B = bipartite.from_biadjacency_matrix(M);
        assert(edges_equal(B.edges(), [(0, 2), (0, 3), (1, 3)]));
        B = bipartite.from_biadjacency_matrix(M, edge_attribute="weight");
        e = [(0, 2, {"weight": 1}), (0, 3, {"weight": 2}), (1, 3, {"weight": 3})];
        assert(edges_equal(B.edges(data=true), e));

    auto test_from_biadjacency_multigraph() const -> void {
        M = sparse.csc_matrix([ [1, 2], [0, 3]]);
        B = bipartite.from_biadjacency_matrix(M, create_using=nx.MultiGraph());
        assert(edges_equal(B.edges(), [(0, 2), (0, 3), (0, 3), (1, 3), (1, 3), (1, 3)]));
