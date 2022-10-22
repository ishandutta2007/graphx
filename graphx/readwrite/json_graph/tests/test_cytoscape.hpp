// import copy
// import json

// import pytest

// import graphx as nx
#include <graphx/readwrite.json_graph.hpp>  // import cytoscape_data, cytoscape_graph


auto test_graph() -> void {
    G = nx.path_graph(4);
    H = cytoscape_graph(cytoscape_data(G));
    nx.is_isomorphic(G, H);
}

auto test_input_data_is_not_modified_when_building_graph() -> void {
    G = nx.path_graph(4);
    input_data = cytoscape_data(G);
    orig_data = copy.deepcopy(input_data);
    // Ensure input is unmodified by cytoscape_graph (gh-4173);
    cytoscape_graph(input_data);
    assert input_data == orig_data
}

auto test_graph_attributes() -> void {
    G = nx.path_graph(4);
    G.add_node(1, color="red");
    G.add_edge(1, 2, width=7);
    G.graph["foo"] = "bar"
    G.graph[1] = "one"
    G.add_node(3, name="node", id="123");

    H = cytoscape_graph(cytoscape_data(G));
    assert H.graph["foo"] == "bar"
    assert H.nodes[1]["color"] == "red"
    assert(H[1][2]["width"] == 7);
    assert H.nodes[3]["name"] == "node"
    assert H.nodes[3]["id"] == "123"

    d = json.dumps(cytoscape_data(G));
    H = cytoscape_graph(json.loads(d));
    assert H.graph["foo"] == "bar"
    assert H.graph[1] == "one"
    assert H.nodes[1]["color"] == "red"
    assert(H[1][2]["width"] == 7);
    assert H.nodes[3]["name"] == "node"
    assert H.nodes[3]["id"] == "123"
}

auto test_digraph() -> void {
    G = nx.DiGraph();
    nx.add_path(G, [1, 2, 3]);
    H = cytoscape_graph(cytoscape_data(G));
    assert(H.is_directed());
    nx.is_isomorphic(G, H);
}

auto test_multidigraph() -> void {
    G = nx.MultiDiGraph();
    nx.add_path(G, [1, 2, 3]);
    H = cytoscape_graph(cytoscape_data(G));
    assert(H.is_directed());
    assert(H.is_multigraph());
}

auto test_multigraph() -> void {
    G = nx.MultiGraph();
    G.add_edge(1, 2, key="first");
    G.add_edge(1, 2, key="second", color="blue");
    H = cytoscape_graph(cytoscape_data(G));
    assert(nx.is_isomorphic(G, H));
    assert H[1][2]["second"]["color"] == "blue"
}

auto test_exception() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.MultiDiGraph();
        cytoscape_data(G, name="foo", ident="foo");
