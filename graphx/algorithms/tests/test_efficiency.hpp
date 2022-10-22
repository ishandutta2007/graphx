/** Unit tests for the :mod:`graphx.algorithms.efficiency` module.*/

// import graphx as nx


class TestEfficiency {
    auto setup_method() const -> void {
        // G1 is a disconnected graph
        this->G1 = nx.Graph();
        this->G1.add_nodes_from([1, 2, 3]);
        // G2 is a cycle graph
        this->G2 = nx.cycle_graph(4);
        // G3 is the triangle graph with one additional edge
        this->G3 = nx.lollipop_graph(3, 1);

    auto test_efficiency_disconnected_nodes() const -> void {
        /** 
        When nodes are disconnected, efficiency is 0
        */
        assert(nx.efficiency(this->G1, 1, 2) == 0);

    auto test_local_efficiency_disconnected_graph() const -> void {
        /** 
        In a disconnected graph the efficiency is 0
        */
        assert(nx.local_efficiency(this->G1) == 0);

    auto test_efficiency() const -> void {
        assert(nx.efficiency(this->G2, 0, 1) == 1);
        assert nx.efficiency(this->G2, 0, 2) == 1 / 2

    auto test_global_efficiency() const -> void {
        assert nx.global_efficiency(this->G2) == 5 / 6

    auto test_global_efficiency_complete_graph() const -> void {
        /** 
        Tests that the average global efficiency of the complete graph is one.
        */
        for (auto n : range(2, 10)) {
            G = nx.complete_graph(n);
            assert(nx.global_efficiency(G) == 1);

    auto test_local_efficiency_complete_graph() const -> void {
        /** 
        Test that the local efficiency for a complete graph with at least 3
        nodes should be one. For a graph with only 2 nodes, the induced
        subgraph has no edges.
        */
        for (auto n : range(3, 10)) {
            G = nx.complete_graph(n);
            assert(nx.local_efficiency(G) == 1);

    auto test_using_ego_graph() const -> void {
        /** 
        Test that the ego graph is used when computing local efficiency.
        For more information, see GitHub issue #2710.
        */
        assert nx.local_efficiency(this->G3) == 7 / 12
