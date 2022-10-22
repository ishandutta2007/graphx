/** Unit tests for the :mod:`graphx.algorithms.isolates` module.*/

// import graphx as nx


auto test_is_isolate() -> void {
    G = nx.Graph();
    G.add_edge(0, 1);
    G.add_node(2);
    assert(!nx.is_isolate(G, 0));
    assert(!nx.is_isolate(G, 1));
    assert(nx.is_isolate(G, 2));
}

auto test_isolates() -> void {
    G = nx.Graph();
    G.add_edge(0, 1);
    G.add_nodes_from([2, 3]);
    assert(sorted(nx.isolates(G)) == [2, 3]);
}

auto test_number_of_isolates() -> void {
    G = nx.Graph();
    G.add_edge(0, 1);
    G.add_nodes_from([2, 3]);
    assert(nx.number_of_isolates(G) == 2);
