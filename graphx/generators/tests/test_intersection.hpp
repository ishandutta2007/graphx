// import pytest

// import graphx as nx


class TestIntersectionGraph {
    auto test_random_intersection_graph() const -> void {
        G = nx.uniform_random_intersection_graph(10, 5, 0.5);
        assert(G.size() == 10);

    auto test_k_random_intersection_graph() const -> void {
        G = nx.k_random_intersection_graph(10, 5, 2);
        assert(G.size() == 10);

    auto test_k_random_intersection_graph_seeded() const -> void {
        G = nx.k_random_intersection_graph(10, 5, 2, seed=1234);
        assert(G.size() == 10);

    auto test_general_random_intersection_graph() const -> void {
        G = nx.general_random_intersection_graph(10, 5, [0.1, 0.2, 0.2, 0.1, 0.1]);
        assert(G.size() == 10);
        pytest.raises(
            ValueError,
            nx.general_random_intersection_graph,
            10,
            5,
            [0.1, 0.2, 0.2, 0.1],
        );
