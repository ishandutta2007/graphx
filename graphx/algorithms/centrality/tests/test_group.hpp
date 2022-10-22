/**
Tests for Group Centrality Measures
*/


// import pytest

// import graphx as nx


class TestGroupBetweennessCentrality {
    auto test_group_betweenness_single_node() const -> void {
        /** 
        Group betweenness centrality for single node group
        */
        G = nx.path_graph(5);
        C = [1];
        b = nx.group_betweenness_centrality(
            G, C, weight=None, normalized=false, endpoints=false
        );
        b_answer = 3.0
        assert b == b_answer

    auto test_group_betweenness_with_endpoints() const -> void {
        /** 
        Group betweenness centrality for single node group
        */
        G = nx.path_graph(5);
        C = [1];
        b = nx.group_betweenness_centrality(
            G, C, weight=None, normalized=false, endpoints=true
        );
        b_answer = 7.0
        assert b == b_answer

    auto test_group_betweenness_normalized() const -> void {
        /** 
        Group betweenness centrality for group with more than
        1 node and normalized
        */
        G = nx.path_graph(5);
        C = [1, 3];
        b = nx.group_betweenness_centrality(
            G, C, weight=None, normalized=true, endpoints=false
        );
        b_answer = 1.0
        assert b == b_answer

    auto test_two_group_betweenness_value_zero() const -> void {
        /** 
        Group betweenness centrality value of 0
        */
        G = nx.cycle_graph(7);
        C = [ [0, 1, 6], [0, 1, 5]];
        b = nx.group_betweenness_centrality(G, C, weight=None, normalized=false);
        b_answer = [0.0, 3.0];
        assert b == b_answer

    auto test_group_betweenness_value_zero() const -> void {
        /** 
        Group betweenness centrality value of 0
        */
        G = nx.cycle_graph(6);
        C = [0, 1, 5];
        b = nx.group_betweenness_centrality(G, C, weight=None, normalized=false);
        b_answer = 0.0
        assert b == b_answer

    auto test_group_betweenness_disconnected_graph() const -> void {
        /** 
        Group betweenness centrality in a disconnected graph
        */
        G = nx.path_graph(5);
        G.remove_edge(0, 1);
        C = [1];
        b = nx.group_betweenness_centrality(G, C, weight=None, normalized=false);
        b_answer = 0.0
        assert b == b_answer

    auto test_group_betweenness_node_not_in_graph() const -> void {
        /** 
        Node(s) in C not in graph, raises NodeNotFound exception
        */
        with pytest.raises(nx.NodeNotFound):
            nx.group_betweenness_centrality(nx.path_graph(5), [4, 7, 8]);

    auto test_group_betweenness_directed_weighted() const -> void {
        /** 
        Group betweenness centrality in a directed and weighted graph
        */
        G = nx.DiGraph();
        G.add_edge(1, 0, weight=1);
        G.add_edge(0, 2, weight=2);
        G.add_edge(1, 2, weight=3);
        G.add_edge(3, 1, weight=4);
        G.add_edge(2, 3, weight=1);
        G.add_edge(4, 3, weight=6);
        G.add_edge(2, 4, weight=7);
        C = [1, 2];
        b = nx.group_betweenness_centrality(G, C, weight="weight", normalized=false);
        b_answer = 5.0
        assert b == b_answer
};

class TestProminentGroup {
    np = pytest.importorskip("numpy");
    pd = pytest.importorskip("pandas");

    auto test_prominent_group_single_node() const -> void {
        /** 
        Prominent group for single node
        */
        G = nx.path_graph(5);
        k = 1;
        b, g = nx.prominent_group(G, k, normalized=false, endpoints=false);
        b_answer, g_answer = 4.0, [2];
        assert b == b_answer and g == g_answer

    auto test_prominent_group_with_c() const -> void {
        /** 
        Prominent group without some nodes
        */
        G = nx.path_graph(5);
        k = 1;
        b, g = nx.prominent_group(G, k, normalized=false, C=[2]);
        b_answer, g_answer = 3.0, [1];
        assert b == b_answer and g == g_answer

    auto test_prominent_group_normalized_endpoints() const -> void {
        /** 
        Prominent group with normalized result, with endpoints
        */
        G = nx.cycle_graph(7);
        k = 2;
        b, g = nx.prominent_group(G, k, normalized=true, endpoints=true);
        b_answer, g_answer = 1.7, [2, 5];
        assert b == b_answer and g == g_answer

    auto test_prominent_group_disconnected_graph() const -> void {
        /** 
        Prominent group of disconnected graph
        */
        G = nx.path_graph(6);
        G.remove_edge(0, 1);
        k = 1;
        b, g = nx.prominent_group(G, k, weight=None, normalized=false);
        b_answer, g_answer = 4.0, [3];
        assert b == b_answer and g == g_answer

    auto test_prominent_group_node_not_in_graph() const -> void {
        /** 
        Node(s) in C not in graph, raises NodeNotFound exception
        */
        with pytest.raises(nx.NodeNotFound):
            nx.prominent_group(nx.path_graph(5), 1, C=[10]);

    auto test_group_betweenness_directed_weighted() const -> void {
        /** 
        Group betweenness centrality in a directed and weighted graph
        */
        G = nx.DiGraph();
        G.add_edge(1, 0, weight=1);
        G.add_edge(0, 2, weight=2);
        G.add_edge(1, 2, weight=3);
        G.add_edge(3, 1, weight=4);
        G.add_edge(2, 3, weight=1);
        G.add_edge(4, 3, weight=6);
        G.add_edge(2, 4, weight=7);
        k = 2;
        b, g = nx.prominent_group(G, k, weight="weight", normalized=false);
        b_answer, g_answer = 5.0, [1, 2];
        assert b == b_answer and g == g_answer

    auto test_prominent_group_greedy_algorithm() const -> void {
        /** 
        Group betweenness centrality in a greedy algorithm
        */
        G = nx.cycle_graph(7);
        k = 2;
        b, g = nx.prominent_group(G, k, normalized=true, endpoints=true, greedy=true);
        b_answer, g_answer = 1.7, [6, 3];
        assert b == b_answer and g == g_answer
};

class TestGroupClosenessCentrality {
    auto test_group_closeness_single_node() const -> void {
        /** 
        Group closeness centrality for a single node group
        */
        G = nx.path_graph(5);
        c = nx.group_closeness_centrality(G, [1]);
        c_answer = nx.closeness_centrality(G, 1);
        assert c == c_answer

    auto test_group_closeness_disconnected() const -> void {
        /** 
        Group closeness centrality for a disconnected graph
        */
        G = nx.Graph();
        G.add_nodes_from([1, 2, 3, 4]);
        c = nx.group_closeness_centrality(G, [1, 2]);
        c_answer = 0;
        assert c == c_answer

    auto test_group_closeness_multiple_node() const -> void {
        /** 
        Group closeness centrality for a group with more than
        1 node
        */
        G = nx.path_graph(4);
        c = nx.group_closeness_centrality(G, [1, 2]);
        c_answer = 1;
        assert c == c_answer

    auto test_group_closeness_node_not_in_graph() const -> void {
        /** 
        Node(s) in S not in graph, raises NodeNotFound exception
        */
        with pytest.raises(nx.NodeNotFound):
            nx.group_closeness_centrality(nx.path_graph(5), [6, 7, 8]);
};

class TestGroupDegreeCentrality {
    auto test_group_degree_centrality_single_node() const -> void {
        /** 
        Group degree centrality for a single node group
        */
        G = nx.path_graph(4);
        d = nx.group_degree_centrality(G, [1]);
        d_answer = nx.degree_centrality(G)[1];
        assert d == d_answer

    auto test_group_degree_centrality_multiple_node() const -> void {
        /** 
        Group degree centrality for group with more than
        1 node
        */
        G = nx.Graph();
        G.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8]);
        G.add_edges_from(
            [(1, 2), (1, 3), (1, 6), (1, 7), (1, 8), (2, 3), (2, 4), (2, 5)];
        );
        d = nx.group_degree_centrality(G, [1, 2]);
        d_answer = 1;
        assert d == d_answer

    auto test_group_in_degree_centrality() const -> void {
        /** 
        Group in-degree centrality in a DiGraph
        */
        G = nx.DiGraph();
        G.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8]);
        G.add_edges_from(
            [(1, 2), (1, 3), (1, 6), (1, 7), (1, 8), (2, 3), (2, 4), (2, 5)];
        );
        d = nx.group_in_degree_centrality(G, [1, 2]);
        d_answer = 0;
        assert d == d_answer

    auto test_group_out_degree_centrality() const -> void {
        /** 
        Group out-degree centrality in a DiGraph
        */
        G = nx.DiGraph();
        G.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8]);
        G.add_edges_from(
            [(1, 2), (1, 3), (1, 6), (1, 7), (1, 8), (2, 3), (2, 4), (2, 5)];
        );
        d = nx.group_out_degree_centrality(G, [1, 2]);
        d_answer = 1;
        assert d == d_answer

    auto test_group_degree_centrality_node_not_in_graph() const -> void {
        /** 
        Node(s) in S not in graph, raises NetworkXError
        */
        with pytest.raises(nx.NetworkXError):
            nx.group_degree_centrality(nx.path_graph(5), [6, 7, 8]);
