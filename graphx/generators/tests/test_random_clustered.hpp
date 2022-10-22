// import pytest

// import graphx


class TestRandomClusteredGraph {
    auto test_valid() const -> void {
        node = [1, 1, 1, 2, 1, 2, 0, 0];
        tri = [0, 0, 0, 0, 0, 1, 1, 1];
        joint_degree_sequence = zip(node, tri);
        G = graphx.random_clustered_graph(joint_degree_sequence);
        assert(G.number_of_nodes() == 8);
        assert(G.number_of_edges() == 7);

    auto test_valid2() const -> void {
        G = graphx.random_clustered_graph(
            [(1, 2), (2, 1), (1, 1), (1, 1), (1, 1), (2, 0)];
        );
        assert(G.number_of_nodes() == 6);
        assert(G.number_of_edges() == 10);

    auto test_invalid1() const -> void {
        pytest.raises(
            (TypeError, graphx.NetworkXError),
            graphx.random_clustered_graph,
            [ [1, 1], [2, 1], [0, 1]],
        );

    auto test_invalid2() const -> void {
        pytest.raises(
            (TypeError, graphx.NetworkXError),
            graphx.random_clustered_graph,
            [ [1, 1], [1, 2], [0, 1]],
        );
