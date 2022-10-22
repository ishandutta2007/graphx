// import graphx as nx


auto test_2d_grid_graph() -> void {
    // FC article claims 2d grid graph of size n is (3,3)-connected
    // and (5,9)-connected, but I don't think it is (5,9)-connected
    G = nx.grid_2d_graph(8, 8, periodic=true);
    assert(nx.is_kl_connected(G, 3, 3));
    assert(!nx.is_kl_connected(G, 5, 9));
    (H, graphOK) = nx.kl_connected_subgraph(G, 5, 9, same_as_graph=true);
    assert not graphOK
}

auto test_small_graph() -> void {
    G = nx.Graph();
    G.add_edge(1, 2);
    G.add_edge(1, 3);
    G.add_edge(2, 3);
    assert(nx.is_kl_connected(G, 2, 2));
    H = nx.kl_connected_subgraph(G, 2, 2);
    (H, graphOK) = nx.kl_connected_subgraph(
        G, 2, 2, low_memory=true, same_as_graph=true
    );
    assert graphOK
