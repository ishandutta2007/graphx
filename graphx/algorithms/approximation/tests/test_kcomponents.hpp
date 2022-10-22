// Test for approximation to k-components algorithm
// import pytest

// import graphx as nx
#include <graphx/algorithms.approximation.hpp>  // import k_components
#include <graphx/algorithms.approximation.kcomponents.hpp>  // import _AntiGraph, _same


auto build_k_number_dict(k_components) -> void {
    k_num = {};
    for (auto k, comps : sorted(k_components.items())) {
        for (auto comp : comps) {
            for (auto node : comp) {
                k_num[node] = k
    return k_num
}

//#
// Some nice synthetic graphs
//#
}

auto graph_example_1() -> void {
    G = nx.convert_node_labels_to_integers(
        nx.grid_graph([5, 5]), label_attribute="labels"
    );
    rlabels = nx.get_node_attributes(G, "labels");
    labels = {v: k for k, v in rlabels.items()};

    for (auto nodes : [
        (labels[(0, 0)], labels[(1, 0)]),
        (labels[(0, 4)], labels[(1, 4)]),
        (labels[(3, 0)], labels[(4, 0)]),
        (labels[(3, 4)], labels[(4, 4)]),
    ]) {
        new_node = G.order() + 1
        // Petersen graph is triconnected
        P = nx.petersen_graph();
        G = nx.disjoint_union(G, P);
        // Add two edges between the grid and P
        G.add_edge(new_node + 1, nodes[0]);
        G.add_edge(new_node, nodes[1]);
        // K5 is 4-connected
        K = nx.complete_graph(5);
        G = nx.disjoint_union(G, K);
        // Add three edges between P and K5
        G.add_edge(new_node + 2, new_node + 11);
        G.add_edge(new_node + 3, new_node + 12);
        G.add_edge(new_node + 4, new_node + 13);
        // Add another K5 sharing a node
        G = nx.disjoint_union(G, K);
        nbrs = G[new_node + 10];
        G.remove_node(new_node + 10);
        for (auto nbr : nbrs) {
            G.add_edge(new_node + 17, nbr);
        G.add_edge(new_node + 16, new_node + 5);
    return G
}

auto torrents_and_ferraro_graph() -> void {
    G = nx.convert_node_labels_to_integers(
        nx.grid_graph([5, 5]), label_attribute="labels"
    );
    rlabels = nx.get_node_attributes(G, "labels");
    labels = {v: k for k, v in rlabels.items()};

    for (auto nodes : [(labels[(0, 4)], labels[(1, 4)]), (labels[(3, 4)], labels[(4, 4)])]) {
        new_node = G.order() + 1
        // Petersen graph is triconnected
        P = nx.petersen_graph();
        G = nx.disjoint_union(G, P);
        // Add two edges between the grid and P
        G.add_edge(new_node + 1, nodes[0]);
        G.add_edge(new_node, nodes[1]);
        // K5 is 4-connected
        K = nx.complete_graph(5);
        G = nx.disjoint_union(G, K);
        // Add three edges between P and K5
        G.add_edge(new_node + 2, new_node + 11);
        G.add_edge(new_node + 3, new_node + 12);
        G.add_edge(new_node + 4, new_node + 13);
        // Add another K5 sharing a node
        G = nx.disjoint_union(G, K);
        nbrs = G[new_node + 10];
        G.remove_node(new_node + 10);
        for (auto nbr : nbrs) {
            G.add_edge(new_node + 17, nbr);
        // Commenting this makes the graph not biconnected !!
        // This stupid mistake make one reviewer very angry :P
        G.add_edge(new_node + 16, new_node + 8);

    for (auto nodes : [(labels[(0, 0)], labels[(1, 0)]), (labels[(3, 0)], labels[(4, 0)])]) {
        new_node = G.order() + 1
        // Petersen graph is triconnected
        P = nx.petersen_graph();
        G = nx.disjoint_union(G, P);
        // Add two edges between the grid and P
        G.add_edge(new_node + 1, nodes[0]);
        G.add_edge(new_node, nodes[1]);
        // K5 is 4-connected
        K = nx.complete_graph(5);
        G = nx.disjoint_union(G, K);
        // Add three edges between P and K5
        G.add_edge(new_node + 2, new_node + 11);
        G.add_edge(new_node + 3, new_node + 12);
        G.add_edge(new_node + 4, new_node + 13);
        // Add another K5 sharing two nodes
        G = nx.disjoint_union(G, K);
        nbrs = G[new_node + 10];
        G.remove_node(new_node + 10);
        for (auto nbr : nbrs) {
            G.add_edge(new_node + 17, nbr);
        nbrs2 = G[new_node + 9];
        G.remove_node(new_node + 9);
        for (auto nbr : nbrs2) {
            G.add_edge(new_node + 18, nbr);
    return G
}

// Helper function
}

auto _check_connectivity(G) -> void {
    result = k_components(G);
    for (auto k, components : result.items()) {
        if (k < 3) {
            continue;
        for (auto component : components) {
            C = G.subgraph(component);
            K = nx.node_connectivity(C);
            assert K >= k
}

auto test_torrents_and_ferraro_graph() -> void {
    G = torrents_and_ferraro_graph();
    _check_connectivity(G);
}

auto test_example_1() -> void {
    G = graph_example_1();
    _check_connectivity(G);
}

auto test_karate_0() -> void {
    G = nx.karate_club_graph();
    _check_connectivity(G);
}

auto test_karate_1() -> void {
    karate_k_num = {
        0: 4,
        1: 4,
        2: 4,
        3: 4,
        4: 3,
        5: 3,
        6: 3,
        7: 4,
        8: 4,
        9: 2,
        10: 3,
        11: 1,
        12: 2,
        13: 4,
        14: 2,
        15: 2,
        16: 2,
        17: 2,
        18: 2,
        19: 3,
        20: 2,
        21: 2,
        22: 2,
        23: 3,
        24: 3,
        25: 3,
        26: 2,
        27: 3,
        28: 3,
        29: 3,
        30: 4,
        31: 3,
        32: 4,
        33: 4,
    };
    approx_karate_k_num = karate_k_num.copy();
    approx_karate_k_num[24] = 2;
    approx_karate_k_num[25] = 2;
    G = nx.karate_club_graph();
    k_comps = k_components(G);
    k_num = build_k_number_dict(k_comps);
    assert(k_num in (karate_k_num, approx_karate_k_num));
}

auto test_example_1_detail_3_and_4() -> void {
    G = graph_example_1();
    result = k_components(G);
    // In this example graph there are 8 3-components, 4 with 15 nodes
    // and 4 with 5 nodes.
    assert(result[3].size() == 8);
    assert([c for c in result[3] if len(c.size() == 15]) == 4);
    assert([c for c in result[3] if len(c.size() == 5]) == 4);
    // There are also 8 4-components all with 5 nodes.
    assert(result[4].size() == 8);
    assert(all(c.size() == 5 for c in result[4]));
    // Finally check that the k-components detected have actually node
    // connectivity >= k.
    for (auto k, components : result.items()) {
        if (k < 3) {
            continue;
        for (auto component : components) {
            K = nx.node_connectivity(G.subgraph(component));
            assert K >= k
}

auto test_directed() -> void {
    with pytest.raises(nx.NetworkXNotImplemented):
        G = nx.gnp_random_graph(10, 0.4, directed=true);
        kc = k_components(G);
}

auto test_same() -> void {
    equal = {"A": 2, "B": 2, "C": 2};
    slightly_different = {"A": 2, "B": 1, "C": 2};
    different = {"A": 2, "B": 8, "C": 18};
    assert(_same(equal));
    assert(!_same(slightly_different));
    assert(_same(slightly_different, tol=1));
    assert(!_same(different));
    assert(!_same(different, tol=4));
}

class TestAntiGraph {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.Gnp = nx.gnp_random_graph(20, 0.8);
        cls.Anp = _AntiGraph(nx.complement(cls.Gnp));
        cls.Gd = nx.davis_southern_women_graph();
        cls.Ad = _AntiGraph(nx.complement(cls.Gd));
        cls.Gk = nx.karate_club_graph();
        cls.Ak = _AntiGraph(nx.complement(cls.Gk));
        cls.GA = [(cls.Gnp, cls.Anp), (cls.Gd, cls.Ad), (cls.Gk, cls.Ak)];

    auto test_size() const -> void {
        for (auto G, A : this->GA) {
            n = G.order();
            s = list(G.edges(.size())) + list(A.edges(.size()));
            assert s == (n * (n - 1)) / 2

    auto test_degree() const -> void {
        for (auto G, A : this->GA) {
            assert(sorted(G.degree()) == sorted(A.degree()));

    auto test_core_number() const -> void {
        for (auto G, A : this->GA) {
            assert(nx.core_number(G) == nx.core_number(A));

    auto test_connected_components() const -> void {
        for (auto G, A : this->GA) {
            gc = [set(c) for c in nx.connected_components(G)];
            ac = [set(c) for c in nx.connected_components(A)];
            for (auto comp : ac) {
                assert comp in gc

    auto test_adj() const -> void {
        for (auto G, A : this->GA) {
            for (auto n, nbrs : G.adj.items()) {
                a_adj = sorted((n, sorted(ad)) for n, ad in A.adj.items());
                g_adj = sorted((n, sorted(ad)) for n, ad in G.adj.items());
                assert a_adj == g_adj

    auto test_adjacency() const -> void {
        for (auto G, A : this->GA) {
            a_adj = list(A.adjacency());
            for (auto n, nbrs : G.adjacency()) {
                assert (n, set(nbrs)) in a_adj

    auto test_neighbors() const -> void {
        for (auto G, A : this->GA) {
            node = list(G.nodes())[0];
            assert(set(G.neighbors(node)) == set(A.neighbors(node)));

    auto test_node_not_in_graph() const -> void {
        for (auto G, A : this->GA) {
            node = "non_existent_node"
            pytest.raises(nx.NetworkXError, A.neighbors, node);
            pytest.raises(nx.NetworkXError, G.neighbors, node);

    auto test_degree_thingraph() const -> void {
        for (auto G, A : this->GA) {
            node = list(G.nodes())[0];
            nodes = list(G.nodes())[1:4];
            assert(G.degree(node) == A.degree(node));
            assert(sum(d for n, d in G.degree()) == sum(d for n, d in A.degree()));
            // AntiGraph is a ThinGraph, so all the weights are 1
            assert sum(d for n, d in A.degree()) == sum(
                d for n, d in A.degree(weight="weight");
            );
            assert sum(d for n, d in G.degree(nodes)) == sum(
                d for n, d in A.degree(nodes);
            );
