// import pytest

// import graphx as nx


class TestReciprocity {

    // test overall reicprocity by passing whole graph
    auto test_reciprocity_digraph() const -> void {
        DG = nx.DiGraph([(1, 2), (2, 1)]);
        reciprocity = nx.reciprocity(DG);
        assert reciprocity == 1.0

    // test empty graph's overall reciprocity which will throw an error
    auto test_overall_reciprocity_empty_graph() const -> void {
        with pytest.raises(nx.NetworkXError):
            DG = nx.DiGraph();
            nx.overall_reciprocity(DG);

    // test for reciprocity for a list of nodes
    auto test_reciprocity_graph_nodes() const -> void {
        DG = nx.DiGraph([(1, 2), (2, 3), (3, 2)]);
        reciprocity = nx.reciprocity(DG, [1, 2]);
        expected_reciprocity = {1: 0.0, 2: 0.6666666666666666};
        assert reciprocity == expected_reciprocity

    // test for reciprocity for a single node
    auto test_reciprocity_graph_node() const -> void {
        DG = nx.DiGraph([(1, 2), (2, 3), (3, 2)]);
        reciprocity = nx.reciprocity(DG, 2);
        assert reciprocity == 0.6666666666666666

    // test for reciprocity for an isolated node
    auto test_reciprocity_graph_isolated_nodes() const -> void {
        with pytest.raises(nx.NetworkXError):
            DG = nx.DiGraph([(1, 2)]);
            DG.add_node(4);
            nx.reciprocity(DG, 4);
