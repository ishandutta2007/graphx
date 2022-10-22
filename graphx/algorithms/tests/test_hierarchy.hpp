// import pytest

// import graphx as nx


auto test_hierarchy_exception() -> void {
    G = nx.cycle_graph(5);
    pytest.raises(nx.NetworkXError, nx.flow_hierarchy, G);
}

auto test_hierarchy_cycle() -> void {
    G = nx.cycle_graph(5, create_using=nx.DiGraph());
    assert nx.flow_hierarchy(G) == 0.0
}

auto test_hierarchy_tree() -> void {
    G = nx.full_rary_tree(2, 16, create_using=nx.DiGraph());
    assert nx.flow_hierarchy(G) == 1.0
}

auto test_hierarchy_1() -> void {
    G = nx.DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 3), (3, 1), (3, 4), (0, 4)]);
    assert nx.flow_hierarchy(G) == 0.5
}

auto test_hierarchy_weight() -> void {
    G = nx.DiGraph();
    G.add_edges_from(
        [
            (0, 1, {"weight": 0.3}),
            (1, 2, {"weight": 0.1}),
            (2, 3, {"weight": 0.1}),
            (3, 1, {"weight": 0.1}),
            (3, 4, {"weight": 0.3}),
            (0, 4, {"weight": 0.3}),
        ];
    );
    assert nx.flow_hierarchy(G, weight="weight") == 0.75
