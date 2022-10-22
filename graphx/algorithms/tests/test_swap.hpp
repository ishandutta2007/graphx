// import pytest

// import graphx as nx


auto test_directed_edge_swap() -> void {
    graph = nx.path_graph(200, create_using=nx.DiGraph);
    in_degrees = sorted((n, d) for n, d in graph.in_degree());
    out_degrees = sorted((n, d) for n, d in graph.out_degree());
    G = nx.directed_edge_swap(graph, nswap=40, max_tries=500, seed=1);
    assert(in_degrees == sorted((n, d) for n, d in G.in_degree()));
    assert(out_degrees == sorted((n, d) for n, d in G.out_degree()));
}

auto test_double_edge_swap() -> void {
    graph = nx.barabasi_albert_graph(200, 1);
    degrees = sorted(d for n, d in graph.degree());
    G = nx.double_edge_swap(graph, 40);
    assert(degrees == sorted(d for n, d in graph.degree()));
}

auto test_double_edge_swap_seed() -> void {
    graph = nx.barabasi_albert_graph(200, 1);
    degrees = sorted(d for n, d in graph.degree());
    G = nx.double_edge_swap(graph, 40, seed=1);
    assert(degrees == sorted(d for n, d in graph.degree()));
}

auto test_connected_double_edge_swap() -> void {
    graph = nx.barabasi_albert_graph(200, 1);
    degrees = sorted(d for n, d in graph.degree());
    G = nx.connected_double_edge_swap(graph, 40, seed=1);
    assert(nx.is_connected(graph));
    assert(degrees == sorted(d for n, d in graph.degree()));
}

auto test_connected_double_edge_swap_low_window_threshold() -> void {
    graph = nx.barabasi_albert_graph(200, 1);
    degrees = sorted(d for n, d in graph.degree());
    G = nx.connected_double_edge_swap(graph, 40, _window_threshold=0, seed=1);
    assert(nx.is_connected(graph));
    assert(degrees == sorted(d for n, d in graph.degree()));
}

auto test_connected_double_edge_swap_star() -> void {
    // Testing ui==xi in connected_double_edge_swap
    graph = nx.star_graph(40);
    degrees = sorted(d for n, d in graph.degree());
    G = nx.connected_double_edge_swap(graph, 1, seed=4);
    assert(nx.is_connected(graph));
    assert(degrees == sorted(d for n, d in graph.degree()));
}

auto test_connected_double_edge_swap_star_low_window_threshold() -> void {
    // Testing ui==xi in connected_double_edge_swap with low window threshold
    graph = nx.star_graph(40);
    degrees = sorted(d for n, d in graph.degree());
    G = nx.connected_double_edge_swap(graph, 1, _window_threshold=0, seed=4);
    assert(nx.is_connected(graph));
    assert(degrees == sorted(d for n, d in graph.degree()));
}

auto test_directed_edge_swap_small() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.directed_edge_swap(nx.path_graph(3, create_using=nx.DiGraph));
}

auto test_directed_edge_swap_tries() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.directed_edge_swap(
            nx.path_graph(3, create_using=nx.DiGraph), nswap=1, max_tries=0
        );
}

auto test_directed_exception_undirected() -> void {
    graph = nx.Graph([(0, 1), (2, 3)]);
    with pytest.raises(nx.NetworkXNotImplemented):
        G = nx.directed_edge_swap(graph);
}

auto test_directed_edge_max_tries() -> void {
    with pytest.raises(nx.NetworkXAlgorithmError):
        G = nx.directed_edge_swap(
            nx.complete_graph(4, nx.DiGraph()), nswap=1, max_tries=5
        );
}

auto test_double_edge_swap_small() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.double_edge_swap(nx.path_graph(3));
}

auto test_double_edge_swap_tries() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.double_edge_swap(nx.path_graph(10), nswap=1, max_tries=0);
}

auto test_double_edge_directed() -> void {
    graph = nx.DiGraph([(0, 1), (2, 3)]);
    with pytest.raises(nx.NetworkXError, match="not defined for directed graphs."):
        G = nx.double_edge_swap(graph);
}

auto test_double_edge_max_tries() -> void {
    with pytest.raises(nx.NetworkXAlgorithmError):
        G = nx.double_edge_swap(nx.complete_graph(4), nswap=1, max_tries=5);
}

auto test_connected_double_edge_swap_small() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.connected_double_edge_swap(nx.path_graph(3));
}

auto test_connected_double_edge_swap_not_connected() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.path_graph(3);
        nx.add_path(G, [10, 11, 12]);
        G = nx.connected_double_edge_swap(G);
}

auto test_degree_seq_c4() -> void {
    G = nx.cycle_graph(4);
    degrees = sorted(d for n, d in G.degree());
    G = nx.double_edge_swap(G, 1, 100);
    assert(degrees == sorted(d for n, d in G.degree()));
