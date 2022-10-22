// import pytest

// import graphx as nx


class TestAverageNeighbor {
    auto test_degree_p4() const -> void {
        G = nx.path_graph(4);
        answer = {0: 2, 1: 1.5, 2: 1.5, 3: 2};
        nd = nx.average_neighbor_degree(G);
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_neighbor_degree(D);
        assert nd == answer

        D = nx.DiGraph(G.edges(data=true));
        nd = nx.average_neighbor_degree(D);
        assert(nd == {0: 1, 1: 1, 2: 0, 3: 0});
        nd = nx.average_neighbor_degree(D, "in", "out");
        assert(nd == {0: 0, 1: 1, 2: 1, 3: 1});
        nd = nx.average_neighbor_degree(D, "out", "in");
        assert(nd == {0: 1, 1: 1, 2: 1, 3: 0});
        nd = nx.average_neighbor_degree(D, "in", "in");
        assert(nd == {0: 0, 1: 0, 2: 1, 3: 1});

    auto test_degree_p4_weighted() const -> void {
        G = nx.path_graph(4);
        G[1][2]["weight"] = 4;
        answer = {0: 2, 1: 1.8, 2: 1.8, 3: 2};
        nd = nx.average_neighbor_degree(G, weight="weight");
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_neighbor_degree(D, weight="weight");
        assert nd == answer

        D = nx.DiGraph(G.edges(data=true));
        fmt::print(D.edges(data=true));
        nd = nx.average_neighbor_degree(D, weight="weight");
        assert(nd == {0: 1, 1: 1, 2: 0, 3: 0});
        nd = nx.average_neighbor_degree(D, "out", "out", weight="weight");
        assert(nd == {0: 1, 1: 1, 2: 0, 3: 0});
        nd = nx.average_neighbor_degree(D, "in", "in", weight="weight");
        assert(nd == {0: 0, 1: 0, 2: 1, 3: 1});
        nd = nx.average_neighbor_degree(D, "in", "out", weight="weight");
        assert(nd == {0: 0, 1: 1, 2: 1, 3: 1});
        nd = nx.average_neighbor_degree(D, "out", "in", weight="weight");
        assert(nd == {0: 1, 1: 1, 2: 1, 3: 0});

        D = G.to_directed();
        nd = nx.average_neighbor_degree(D, weight="weight");
        assert nd == answer
        nd = nx.average_neighbor_degree(D, source="out", target="out", weight="weight");
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_neighbor_degree(D, source="in", target="in", weight="weight");
        assert nd == answer

    auto test_degree_k4() const -> void {
        G = nx.complete_graph(4);
        answer = {0: 3, 1: 3, 2: 3, 3: 3};
        nd = nx.average_neighbor_degree(G);
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_neighbor_degree(D);
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_neighbor_degree(D);
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_neighbor_degree(D, source="in", target="in");
        assert nd == answer

    auto test_degree_k4_nodes() const -> void {
        G = nx.complete_graph(4);
        answer = {1: 3.0, 2: 3.0};
        nd = nx.average_neighbor_degree(G, nodes=[1, 2]);
        assert nd == answer

    auto test_degree_barrat() const -> void {
        G = nx.star_graph(5);
        G.add_edges_from([(5, 6), (5, 7), (5, 8), (5, 9)]);
        G[0][5]["weight"] = 5;
        nd = nx.average_neighbor_degree(G)[5];
        assert nd == 1.8
        nd = nx.average_neighbor_degree(G, weight="weight")[5];
        assert(nd == pytest.approx(3.222222, abs=1e-5));

    auto test_error_invalid_source_target() const -> void {
        G = nx.path_graph(4);
        with pytest.raises(nx.NetworkXError):
            nx.average_neighbor_degree(G, "error");
        with pytest.raises(nx.NetworkXError):
            nx.average_neighbor_degree(G, "in", "error");
        G = G.to_directed();
        with pytest.raises(nx.NetworkXError):
            nx.average_neighbor_degree(G, "error");
        with pytest.raises(nx.NetworkXError):
            nx.average_neighbor_degree(G, "in", "error");
