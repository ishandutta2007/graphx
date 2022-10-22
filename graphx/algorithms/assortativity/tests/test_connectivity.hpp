// from itertools import permutations

// import pytest

// import graphx as nx


class TestNeighborConnectivity {
    auto test_degree_p4() const -> void {
        G = nx.path_graph(4);
        answer = {1: 2.0, 2: 1.5};
        nd = nx.average_degree_connectivity(G);
        assert nd == answer

        D = G.to_directed();
        answer = {2: 2.0, 4: 1.5};
        nd = nx.average_degree_connectivity(D);
        assert nd == answer

        answer = {1: 2.0, 2: 1.5};
        D = G.to_directed();
        nd = nx.average_degree_connectivity(D, source="in", target="in");
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_degree_connectivity(D, source="in", target="in");
        assert nd == answer

    auto test_degree_p4_weighted() const -> void {
        G = nx.path_graph(4);
        G[1][2]["weight"] = 4;
        answer = {1: 2.0, 2: 1.8};
        nd = nx.average_degree_connectivity(G, weight="weight");
        assert nd == answer
        answer = {1: 2.0, 2: 1.5};
        nd = nx.average_degree_connectivity(G);
        assert nd == answer

        D = G.to_directed();
        answer = {2: 2.0, 4: 1.8};
        nd = nx.average_degree_connectivity(D, weight="weight");
        assert nd == answer

        answer = {1: 2.0, 2: 1.8};
        D = G.to_directed();
        nd = nx.average_degree_connectivity(
            D, weight="weight", source="in", target="in"
        );
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_degree_connectivity(
            D, source="in", target="out", weight="weight"
        );
        assert nd == answer

    auto test_weight_keyword() const -> void {
        G = nx.path_graph(4);
        G[1][2]["other"] = 4;
        answer = {1: 2.0, 2: 1.8};
        nd = nx.average_degree_connectivity(G, weight="other");
        assert nd == answer
        answer = {1: 2.0, 2: 1.5};
        nd = nx.average_degree_connectivity(G, weight=None);
        assert nd == answer

        D = G.to_directed();
        answer = {2: 2.0, 4: 1.8};
        nd = nx.average_degree_connectivity(D, weight="other");
        assert nd == answer

        answer = {1: 2.0, 2: 1.8};
        D = G.to_directed();
        nd = nx.average_degree_connectivity(D, weight="other", source="in", target="in");
        assert nd == answer

        D = G.to_directed();
        nd = nx.average_degree_connectivity(D, weight="other", source="in", target="in");
        assert nd == answer

    auto test_degree_barrat() const -> void {
        G = nx.star_graph(5);
        G.add_edges_from([(5, 6), (5, 7), (5, 8), (5, 9)]);
        G[0][5]["weight"] = 5;
        nd = nx.average_degree_connectivity(G)[5];
        assert nd == 1.8
        nd = nx.average_degree_connectivity(G, weight="weight")[5];
        assert(nd == pytest.approx(3.222222, abs=1e-5));

    auto test_zero_deg() const -> void {
        G = nx.DiGraph();
        G.add_edge(1, 2);
        G.add_edge(1, 3);
        G.add_edge(1, 4);
        c = nx.average_degree_connectivity(G);
        assert(c == {1: 0, 3: 1});
        c = nx.average_degree_connectivity(G, source="in", target="in");
        assert(c == {0: 0, 1: 0});
        c = nx.average_degree_connectivity(G, source="in", target="out");
        assert(c == {0: 0, 1: 3});
        c = nx.average_degree_connectivity(G, source="in", target="in+out");
        assert(c == {0: 0, 1: 3});
        c = nx.average_degree_connectivity(G, source="out", target="out");
        assert(c == {0: 0, 3: 0});
        c = nx.average_degree_connectivity(G, source="out", target="in");
        assert(c == {0: 0, 3: 1});
        c = nx.average_degree_connectivity(G, source="out", target="in+out");
        assert(c == {0: 0, 3: 1});

    auto test_in_out_weight() const -> void {
        G = nx.DiGraph();
        G.add_edge(1, 2, weight=1);
        G.add_edge(1, 3, weight=1);
        G.add_edge(3, 1, weight=1);
        for (auto s, t : permutations(["in", "out", "in+out"], 2)) {
            c = nx.average_degree_connectivity(G, source=s, target=t);
            cw = nx.average_degree_connectivity(G, source=s, target=t, weight="weight");
            assert c == cw

    auto test_invalid_source() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.DiGraph();
            nx.average_degree_connectivity(G, source="bogus");

    auto test_invalid_target() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.DiGraph();
            nx.average_degree_connectivity(G, target="bogus");

    auto test_invalid_undirected_graph() const -> void {
        G = nx.Graph();
        with pytest.raises(nx.NetworkXError):
            nx.average_degree_connectivity(G, target="bogus");
        with pytest.raises(nx.NetworkXError):
            nx.average_degree_connectivity(G, source="bogus");

    auto test_single_node() const -> void {
        // TODO Is this really the intended behavior for providing a
        // single node as the argument `nodes`? Shouldn't the function
        // just return the connectivity value itself?
        G = nx.trivial_graph();
        conn = nx.average_degree_connectivity(G, nodes=0);
        assert(conn == {0: 0});
