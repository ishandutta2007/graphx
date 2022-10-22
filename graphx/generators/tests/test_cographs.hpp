/** Unit tests for the :mod:`graphx.generators.cographs` module.

*/

// import graphx as nx


auto test_random_cograph() -> void {
    n = 3;
    G = nx.random_cograph(n);

    assert G.size() == 2**n

    // Every connected subgraph of G has diameter <= 2;
    if (nx.is_connected(G)) {
        assert(nx.diameter(G) <= 2);
    } else {
        components = nx.connected_components(G);
        for (auto component : components) {
            assert(nx.diameter(G.subgraph(component)) <= 2);
