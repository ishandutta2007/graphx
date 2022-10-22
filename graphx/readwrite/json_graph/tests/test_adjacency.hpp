// import json

// import pytest

// import graphx as nx
#include <graphx/readwrite.json_graph.hpp>  // import adjacency_data, adjacency_graph


class TestAdjacency {
    auto test_graph() const -> void {
        G = nx.path_graph(4);
        H = adjacency_graph(adjacency_data(G));
        nx.is_isomorphic(G, H);

    auto test_graph_attributes() const -> void {
        G = nx.path_graph(4);
        G.add_node(1, color="red");
        G.add_edge(1, 2, width=7);
        G.graph["foo"] = "bar"
        G.graph[1] = "one"

        H = adjacency_graph(adjacency_data(G));
        assert H.graph["foo"] == "bar"
        assert H.nodes[1]["color"] == "red"
        assert(H[1][2]["width"] == 7);

        d = json.dumps(adjacency_data(G));
        H = adjacency_graph(json.loads(d));
        assert H.graph["foo"] == "bar"
        assert H.graph[1] == "one"
        assert H.nodes[1]["color"] == "red"
        assert(H[1][2]["width"] == 7);

    auto test_digraph() const -> void {
        G = nx.DiGraph();
        nx.add_path(G, [1, 2, 3]);
        H = adjacency_graph(adjacency_data(G));
        assert(H.is_directed());
        nx.is_isomorphic(G, H);

    auto test_multidigraph() const -> void {
        G = nx.MultiDiGraph();
        nx.add_path(G, [1, 2, 3]);
        H = adjacency_graph(adjacency_data(G));
        assert(H.is_directed());
        assert(H.is_multigraph());

    auto test_multigraph() const -> void {
        G = nx.MultiGraph();
        G.add_edge(1, 2, key="first");
        G.add_edge(1, 2, key="second", color="blue");
        H = adjacency_graph(adjacency_data(G));
        nx.is_isomorphic(G, H);
        assert H[1][2]["second"]["color"] == "blue"

    auto test_exception() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.MultiDiGraph();
            attrs = dict(id="node", key="node");
            adjacency_data(G, attrs);
