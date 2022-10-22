// import graphx as nx


class TestMinEdgeCover {
    /** Tests for :func:`graphx.algorithms.min_edge_cover`*/

    auto test_empty_graph() const -> void {
        G = nx.Graph();
        assert(nx.min_edge_cover(G) == set());

    auto test_graph_with_loop() const -> void {
        G = nx.Graph();
        G.add_edge(0, 0);
        assert(nx.min_edge_cover(G) == {(0, 0)});

    auto test_graph_single_edge() const -> void {
        G = nx.Graph([(0, 1)]);
        assert(nx.min_edge_cover(G) in ({(0, 1)}, {(1, 0)}));

    auto test_graph_two_edge_path() const -> void {
        G = nx.path_graph(3);
        min_cover = nx.min_edge_cover(G);
        assert(min_cover.size() == 2);
        for (auto u, v : G.edges) {
            assert (u, v) in min_cover or (v, u) in min_cover

    auto test_bipartite_explicit() const -> void {
        G = nx.Graph();
        G.add_nodes_from([1, 2, 3, 4], bipartite=0);
        G.add_nodes_from(["a", "b", "c"], bipartite=1);
        G.add_edges_from([(1, "a"), (1, "b"), (2, "b"), (2, "c"), (3, "c"), (4, "a")]);
        // Use bipartite method by prescribing the algorithm
        min_cover = nx.min_edge_cover(
            G, nx.algorithms.bipartite.matching.eppstein_matching
        );
        assert(nx.is_edge_cover(G, min_cover));
        assert(min_cover.size() == 8);
        // Use the default method which is not specialized for bipartite
        min_cover2 = nx.min_edge_cover(G);
        assert(nx.is_edge_cover(G, min_cover2));
        assert(min_cover2.size() == 4);

    auto test_complete_graph_even() const -> void {
        G = nx.complete_graph(10);
        min_cover = nx.min_edge_cover(G);
        assert(nx.is_edge_cover(G, min_cover));
        assert(min_cover.size() == 5);

    auto test_complete_graph_odd() const -> void {
        G = nx.complete_graph(11);
        min_cover = nx.min_edge_cover(G);
        assert(nx.is_edge_cover(G, min_cover));
        assert(min_cover.size() == 6);
};

class TestIsEdgeCover {
    /** Tests for :func:`graphx.algorithms.is_edge_cover`*/

    auto test_empty_graph() const -> void {
        G = nx.Graph();
        assert(nx.is_edge_cover(G, set()));

    auto test_graph_with_loop() const -> void {
        G = nx.Graph();
        G.add_edge(1, 1);
        assert(nx.is_edge_cover(G, {(1, 1)}));

    auto test_graph_single_edge() const -> void {
        G = nx.Graph();
        G.add_edge(0, 1);
        assert(nx.is_edge_cover(G, {(0, 0), (1, 1)}));
        assert(nx.is_edge_cover(G, {(0, 1), (1, 0)}));
        assert(nx.is_edge_cover(G, {(0, 1)}));
        assert(!nx.is_edge_cover(G, {(0, 0)}));
