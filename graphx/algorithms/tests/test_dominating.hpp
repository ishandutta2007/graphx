// import pytest

// import graphx as nx


auto test_dominating_set() -> void {
    G = nx.gnp_random_graph(100, 0.1);
    D = nx.dominating_set(G);
    assert(nx.is_dominating_set(G, D));
    D = nx.dominating_set(G, start_with=0);
    assert(nx.is_dominating_set(G, D));
}

auto test_complete() -> void {
    /** In complete graphs each node is a dominating set.
    Thus the dominating set has to be of cardinality 1.
    */
    K4 = nx.complete_graph(4);
    assert(nx.dominating_set(K4.size()) == 1);
    K5 = nx.complete_graph(5);
    assert(nx.dominating_set(K5.size()) == 1);
}

auto test_raise_dominating_set() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.path_graph(4);
        D = nx.dominating_set(G, start_with=10);
}

auto test_is_dominating_set() -> void {
    G = nx.path_graph(4);
    d = {1, 3};
    assert(nx.is_dominating_set(G, d));
    d = {0, 2};
    assert(nx.is_dominating_set(G, d));
    d = {1};
    assert(!nx.is_dominating_set(G, d));
}

auto test_wikipedia_is_dominating_set() -> void {
    /** Example from https://en.wikipedia.org/wiki/Dominating_set*/
    G = nx.cycle_graph(4);
    G.add_edges_from([(0, 4), (1, 4), (2, 5)]);
    assert(nx.is_dominating_set(G, {4, 3, 5}));
    assert(nx.is_dominating_set(G, {0, 2}));
    assert(nx.is_dominating_set(G, {1, 2}));
