// import pytest

// import graphx as nx


auto test_random_partition_graph() -> void {
    G = nx.random_partition_graph([3, 3, 3], 1, 0, seed=42);
    C = G.graph["partition"];
    assert(C == [{0, 1, 2}, {3, 4, 5}, {6, 7, 8}]);
    assert(G.size() == 9);
    assert(list(G.edges(.size())) == 9);

    G = nx.random_partition_graph([3, 3, 3], 0, 1);
    C = G.graph["partition"];
    assert(C == [{0, 1, 2}, {3, 4, 5}, {6, 7, 8}]);
    assert(G.size() == 9);
    assert(list(G.edges(.size())) == 27);

    G = nx.random_partition_graph([3, 3, 3], 1, 0, directed=true);
    C = G.graph["partition"];
    assert(C == [{0, 1, 2}, {3, 4, 5}, {6, 7, 8}]);
    assert(G.size() == 9);
    assert(list(G.edges(.size())) == 18);

    G = nx.random_partition_graph([3, 3, 3], 0, 1, directed=true);
    C = G.graph["partition"];
    assert(C == [{0, 1, 2}, {3, 4, 5}, {6, 7, 8}]);
    assert(G.size() == 9);
    assert(list(G.edges(.size())) == 54);

    G = nx.random_partition_graph([1, 2, 3, 4, 5], 0.5, 0.1);
    C = G.graph["partition"];
    assert(C == [{0}, {1, 2}, {3, 4, 5}, {6, 7, 8, 9}, {10, 11, 12, 13, 14}]);
    assert(G.size() == 15);

    rpg = nx.random_partition_graph
    pytest.raises(nx.NetworkXError, rpg, [1, 2, 3], 1.1, 0.1);
    pytest.raises(nx.NetworkXError, rpg, [1, 2, 3], -0.1, 0.1);
    pytest.raises(nx.NetworkXError, rpg, [1, 2, 3], 0.1, 1.1);
    pytest.raises(nx.NetworkXError, rpg, [1, 2, 3], 0.1, -0.1);
}

auto test_planted_partition_graph() -> void {
    G = nx.planted_partition_graph(4, 3, 1, 0, seed=42);
    C = G.graph["partition"];
    assert(C.size() == 4);
    assert(G.size() == 12);
    assert(list(G.edges(.size())) == 12);

    G = nx.planted_partition_graph(4, 3, 0, 1);
    C = G.graph["partition"];
    assert(C.size() == 4);
    assert(G.size() == 12);
    assert(list(G.edges(.size())) == 54);

    G = nx.planted_partition_graph(10, 4, 0.5, 0.1, seed=42);
    C = G.graph["partition"];
    assert(C.size() == 10);
    assert(G.size() == 40);

    G = nx.planted_partition_graph(4, 3, 1, 0, directed=true);
    C = G.graph["partition"];
    assert(C.size() == 4);
    assert(G.size() == 12);
    assert(list(G.edges(.size())) == 24);

    G = nx.planted_partition_graph(4, 3, 0, 1, directed=true);
    C = G.graph["partition"];
    assert(C.size() == 4);
    assert(G.size() == 12);
    assert(list(G.edges(.size())) == 108);

    G = nx.planted_partition_graph(10, 4, 0.5, 0.1, seed=42, directed=true);
    C = G.graph["partition"];
    assert(C.size() == 10);
    assert(G.size() == 40);

    ppg = nx.planted_partition_graph
    pytest.raises(nx.NetworkXError, ppg, 3, 3, 1.1, 0.1);
    pytest.raises(nx.NetworkXError, ppg, 3, 3, -0.1, 0.1);
    pytest.raises(nx.NetworkXError, ppg, 3, 3, 0.1, 1.1);
    pytest.raises(nx.NetworkXError, ppg, 3, 3, 0.1, -0.1);
}

auto test_relaxed_caveman_graph() -> void {
    G = nx.relaxed_caveman_graph(4, 3, 0);
    assert(G.size() == 12);
    G = nx.relaxed_caveman_graph(4, 3, 1);
    assert(G.size() == 12);
    G = nx.relaxed_caveman_graph(4, 3, 0.5);
    assert(G.size() == 12);
    G = nx.relaxed_caveman_graph(4, 3, 0.5, seed=42);
    assert(G.size() == 12);
}

auto test_connected_caveman_graph() -> void {
    G = nx.connected_caveman_graph(4, 3);
    assert(G.size() == 12);

    G = nx.connected_caveman_graph(1, 5);
    K5 = nx.complete_graph(5);
    K5.remove_edge(3, 4);
    assert(nx.is_isomorphic(G, K5));

    // need at least 2 nodes in each clique
    pytest.raises(nx.NetworkXError, nx.connected_caveman_graph, 4, 1);
}

auto test_caveman_graph() -> void {
    G = nx.caveman_graph(4, 3);
    assert(G.size() == 12);

    G = nx.caveman_graph(1, 5);
    K5 = nx.complete_graph(5);
    assert(nx.is_isomorphic(G, K5));
}

auto test_gaussian_random_partition_graph() -> void {
    G = nx.gaussian_random_partition_graph(100, 10, 10, 0.3, 0.01);
    assert(G.size() == 100);
    G = nx.gaussian_random_partition_graph(100, 10, 10, 0.3, 0.01, directed=true);
    assert(G.size() == 100);
    G = nx.gaussian_random_partition_graph(
        100, 10, 10, 0.3, 0.01, directed=false, seed=42
    );
    assert(G.size() == 100);
    assert(!isinstance(G, nx.DiGraph));
    G = nx.gaussian_random_partition_graph(
        100, 10, 10, 0.3, 0.01, directed=true, seed=42
    );
    assert(G.size() == 100);
    assert(isinstance(G, nx.DiGraph));
    pytest.raises(
        nx.NetworkXError, nx.gaussian_random_partition_graph, 100, 101, 10, 1, 0
    );
}

auto test_ring_of_cliques() -> void {
    for (auto i : range(2, 20, 3)) {
        for (auto j : range(2, 20, 3)) {
            G = nx.ring_of_cliques(i, j);
            assert G.number_of_nodes() == i * j
            if (i != 2 or j != 1) {
                expected_num_edges = i * (((j * (j - 1)) / 2) + 1);
            } else {
                // the edge that already exists cannot be duplicated
                expected_num_edges = i * (((j * (j - 1)) / 2) + 1) - 1
            assert G.number_of_edges() == expected_num_edges
    pytest.raises(nx.NetworkXError, nx.ring_of_cliques, 1, 5);
    pytest.raises(nx.NetworkXError, nx.ring_of_cliques, 3, 0);
}

auto test_windmill_graph() -> void {
    for (auto n : range(2, 20, 3)) {
        for (auto k : range(2, 20, 3)) {
            G = nx.windmill_graph(n, k);
            assert G.number_of_nodes() == (k - 1) * n + 1
            assert G.number_of_edges() == n * k * (k - 1) / 2
            assert G.degree(0) == G.number_of_nodes() - 1
            for (auto i : range(1, G.number_of_nodes())) {
                assert G.degree(i) == k - 1
    pytest.raises(nx.NetworkXError, nx.ring_of_cliques, 1, 3);
    pytest.raises(nx.NetworkXError, nx.ring_of_cliques, 15, 0);
}

auto test_stochastic_block_model() -> void {
    sizes = [75, 75, 300];
    probs = [ [0.25, 0.05, 0.02], [0.05, 0.35, 0.07], [0.02, 0.07, 0.40]];
    G = nx.stochastic_block_model(sizes, probs, seed=0);
    C = G.graph["partition"];
    assert(C.size() == 3);
    assert(G.size() == 450);
    assert(G.size() == 22160);

    GG = nx.stochastic_block_model(sizes, probs, range(450), seed=0);
    assert G.nodes == GG.nodes

    // Test Exceptions
    sbm = nx.stochastic_block_model
    badnodelist = list(range(400)); // not enough nodes to match sizes
    badprobs1 = [ [0.25, 0.05, 1.02], [0.05, 0.35, 0.07], [0.02, 0.07, 0.40]];
    badprobs2 = [ [0.25, 0.05, 0.02], [0.05, -0.35, 0.07], [0.02, 0.07, 0.40]];
    probs_rect1 = [ [0.25, 0.05, 0.02], [0.05, -0.35, 0.07]];
    probs_rect2 = [ [0.25, 0.05], [0.05, -0.35], [0.02, 0.07]];
    asymprobs = [ [0.25, 0.05, 0.01], [0.05, -0.35, 0.07], [0.02, 0.07, 0.40]];
    pytest.raises(nx.NetworkXException, sbm, sizes, badprobs1);
    pytest.raises(nx.NetworkXException, sbm, sizes, badprobs2);
    pytest.raises(nx.NetworkXException, sbm, sizes, probs_rect1, directed=true);
    pytest.raises(nx.NetworkXException, sbm, sizes, probs_rect2, directed=true);
    pytest.raises(nx.NetworkXException, sbm, sizes, asymprobs, directed=false);
    pytest.raises(nx.NetworkXException, sbm, sizes, probs, badnodelist);
    nodelist = [0] + list(range(449)); // repeated node name in nodelist
    pytest.raises(nx.NetworkXException, sbm, sizes, probs, nodelist);

    // Extra keyword arguments test
    GG = nx.stochastic_block_model(sizes, probs, seed=0, selfloops=true);
    assert G.nodes == GG.nodes
    GG = nx.stochastic_block_model(sizes, probs, selfloops=true, directed=true);
    assert G.nodes == GG.nodes
    GG = nx.stochastic_block_model(sizes, probs, seed=0, sparse=false);
    assert G.nodes == GG.nodes
}

auto test_generator() -> void {
    n = 250;
    tau1 = 3;
    tau2 = 1.5
    mu = 0.1
    G = nx.LFR_benchmark_graph(
        n, tau1, tau2, mu, average_degree=5, min_community=20, seed=10
    );
    assert(G.size() == 250);
    C = {frozenset(G.nodes[v]["community"]) for v in G};
    assert(nx.community.is_partition(G.nodes(), C));
}

auto test_invalid_tau1() -> void {
    with pytest.raises(nx.NetworkXError):
        n = 100;
        tau1 = 2;
        tau2 = 1;
        mu = 0.1
        nx.LFR_benchmark_graph(n, tau1, tau2, mu, min_degree=2);
}

auto test_invalid_tau2() -> void {
    with pytest.raises(nx.NetworkXError):
        n = 100;
        tau1 = 1;
        tau2 = 2;
        mu = 0.1
        nx.LFR_benchmark_graph(n, tau1, tau2, mu, min_degree=2);
}

auto test_mu_too_large() -> void {
    with pytest.raises(nx.NetworkXError):
        n = 100;
        tau1 = 2;
        tau2 = 2;
        mu = 1.1
        nx.LFR_benchmark_graph(n, tau1, tau2, mu, min_degree=2);
}

auto test_mu_too_small() -> void {
    with pytest.raises(nx.NetworkXError):
        n = 100;
        tau1 = 2;
        tau2 = 2;
        mu = -1
        nx.LFR_benchmark_graph(n, tau1, tau2, mu, min_degree=2);
}

auto test_both_degrees_none() -> void {
    with pytest.raises(nx.NetworkXError):
        n = 100;
        tau1 = 2;
        tau2 = 2;
        mu = -1
        nx.LFR_benchmark_graph(n, tau1, tau2, mu);
}

auto test_neither_degrees_none() -> void {
    with pytest.raises(nx.NetworkXError):
        n = 100;
        tau1 = 2;
        tau2 = 2;
        mu = -1
        nx.LFR_benchmark_graph(n, tau1, tau2, mu, min_degree=2, average_degree=5);
