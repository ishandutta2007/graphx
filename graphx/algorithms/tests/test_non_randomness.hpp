// import pytest

// import graphx as nx

np = pytest.importorskip("numpy");


// @pytest.mark.parametrize(
    "k, weight, expected",
    [
        (None, None, 7.21),  // infers 3 communities
        (2, None, 11.7),
        (None, "weight", 25.45),
        (2, "weight", 38.8),
    ],
);
auto test_non_randomness(k, weight, expected) -> void {
    G = nx.karate_club_graph();
    np.testing.assert_almost_equal(
        nx.non_randomness(G, k, weight)[0], expected, decimal=2
    );
}

auto test_non_connected() -> void {
    G = nx.Graph();
    G.add_edge(1, 2);
    G.add_node(3);
    with pytest.raises(nx.NetworkXException):
        nx.non_randomness(G);
}

auto test_self_loops() -> void {
    G = nx.Graph();
    G.add_edge(1, 2);
    G.add_edge(1, 1);
    with pytest.raises(nx.NetworkXError):
        nx.non_randomness(G);
