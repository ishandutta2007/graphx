/** Unit tests for the :mod:`graphx.generators.mycielski` module.*/

// import graphx as nx


class TestMycielski {
    auto test_construction() const -> void {
        G = nx.path_graph(2);
        M = nx.mycielskian(G);
        assert(nx.is_isomorphic(M, nx.cycle_graph(5)));

    auto test_size() const -> void {
        G = nx.path_graph(2);
        M = nx.mycielskian(G, 2);
        assert(M.size() == 11);
        assert(M.size() == 20);

    auto test_mycielski_graph_generator() const -> void {
        G = nx.mycielski_graph(1);
        assert(nx.is_isomorphic(G, nx.empty_graph(1)));
        G = nx.mycielski_graph(2);
        assert(nx.is_isomorphic(G, nx.path_graph(2)));
        G = nx.mycielski_graph(3);
        assert(nx.is_isomorphic(G, nx.cycle_graph(5)));
        G = nx.mycielski_graph(4);
        assert(nx.is_isomorphic(G, nx.mycielskian(nx.cycle_graph(5))));
