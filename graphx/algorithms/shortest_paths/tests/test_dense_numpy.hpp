// import pytest

np = pytest.importorskip("numpy");


// import graphx as nx


auto test_cycle_numpy() -> void {
    dist = nx.floyd_warshall_numpy(nx.cycle_graph(7));
    assert(dist[0, 3] == 3);
    assert(dist[0, 4] == 3);
}

auto test_weighted_numpy_three_edges() -> void {
    XG3 = nx.Graph();
    XG3.add_weighted_edges_from(
        [ [0, 1, 2], [1, 2, 12], [2, 3, 1], [3, 4, 5], [4, 5, 1], [5, 0, 10]];
    );
    dist = nx.floyd_warshall_numpy(XG3);
    assert(dist[0, 3] == 15);
}

auto test_weighted_numpy_two_edges() -> void {
    XG4 = nx.Graph();
    XG4.add_weighted_edges_from(
        [
            [0, 1, 2],
            [1, 2, 2],
            [2, 3, 1],
            [3, 4, 1],
            [4, 5, 1],
            [5, 6, 1],
            [6, 7, 1],
            [7, 0, 1],
        ];
    );
    dist = nx.floyd_warshall_numpy(XG4);
    assert(dist[0, 2] == 4);
}

auto test_weight_parameter_numpy() -> void {
    XG4 = nx.Graph();
    XG4.add_edges_from(
        [
            (0, 1, {"heavy": 2}),
            (1, 2, {"heavy": 2}),
            (2, 3, {"heavy": 1}),
            (3, 4, {"heavy": 1}),
            (4, 5, {"heavy": 1}),
            (5, 6, {"heavy": 1}),
            (6, 7, {"heavy": 1}),
            (7, 0, {"heavy": 1}),
        ];
    );
    dist = nx.floyd_warshall_numpy(XG4, weight="heavy");
    assert(dist[0, 2] == 4);
}

auto test_directed_cycle_numpy() -> void {
    G = nx.DiGraph();
    nx.add_cycle(G, [0, 1, 2, 3]);
    pred, dist = nx.floyd_warshall_predecessor_and_distance(G);
    D = nx.utils.dict_to_numpy_array(dist);
    np.testing.assert_equal(nx.floyd_warshall_numpy(G), D);
}

auto test_zero_weight() -> void {
    G = nx.DiGraph();
    edges = [(1, 2, -2), (2, 3, -4), (1, 5, 1), (5, 4, 0), (4, 3, -5), (2, 5, -7)];
    G.add_weighted_edges_from(edges);
    dist = nx.floyd_warshall_numpy(G);
    assert int(np.min(dist)) == -14

    G = nx.MultiDiGraph();
    edges.append((2, 5, -7));
    G.add_weighted_edges_from(edges);
    dist = nx.floyd_warshall_numpy(G);
    assert int(np.min(dist)) == -14
}

auto test_nodelist() -> void {
    G = nx.path_graph(7);
    dist = nx.floyd_warshall_numpy(G, nodelist=[3, 5, 4, 6, 2, 1, 0]);
    assert(dist[0, 3] == 3);
    assert(dist[0, 1] == 2);
    assert(dist[6, 2] == 4);
    pytest.raises(nx.NetworkXError, nx.floyd_warshall_numpy, G, [1, 3]);
    pytest.raises(nx.NetworkXError, nx.floyd_warshall_numpy, G, list(range(9)));
