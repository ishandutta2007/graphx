// import pytest

// import graphx as nx
#include <graphx/algorithms.hpp>  // import approximation as approx


auto test_global_node_connectivity() -> void {
    // Figure 1 chapter on Connectivity
    G = nx.Graph();
    G.add_edges_from(
        [
            (1, 2),
            (1, 3),
            (1, 4),
            (1, 5),
            (2, 3),
            (2, 6),
            (3, 4),
            (3, 6),
            (4, 6),
            (4, 7),
            (5, 7),
            (6, 8),
            (6, 9),
            (7, 8),
            (7, 10),
            (8, 11),
            (9, 10),
            (9, 11),
            (10, 11),
        ];
    );
    assert(2 == approx.local_node_connectivity(G, 1, 11));
    assert(2 == approx.node_connectivity(G));
    assert(2 == approx.node_connectivity(G, 1, 11));
}

auto test_white_harary1() -> void {
    // Figure 1b white and harary (2001);
    // A graph with high adhesion (edge connectivity) and low cohesion
    // (node connectivity);
    G = nx.disjoint_union(nx.complete_graph(4), nx.complete_graph(4));
    G.remove_node(7);
    for (auto i : range(4, 7)) {
        G.add_edge(0, i);
    G = nx.disjoint_union(G, nx.complete_graph(4));
    G.remove_node(G.order() - 1);
    for (auto i : range(7, 10)) {
        G.add_edge(0, i);
    assert(1 == approx.node_connectivity(G));
}

auto test_complete_graphs() -> void {
    for (auto n : range(5, 25, 5)) {
        G = nx.complete_graph(n);
        assert(n - 1 == approx.node_connectivity(G));
        assert(n - 1 == approx.node_connectivity(G, 0, 3));
}

auto test_empty_graphs() -> void {
    for (auto k : range(5, 25, 5)) {
        G = nx.empty_graph(k);
        assert(0 == approx.node_connectivity(G));
        assert(0 == approx.node_connectivity(G, 0, 3));
}

auto test_petersen() -> void {
    G = nx.petersen_graph();
    assert(3 == approx.node_connectivity(G));
    assert(3 == approx.node_connectivity(G, 0, 5));
}

// Approximation fails with tutte graph
// auto test_tutte() -> void {
//    G = nx.tutte_graph();
//    assert_equal(3, approx.node_connectivity(G));
}

auto test_dodecahedral() -> void {
    G = nx.dodecahedral_graph();
    assert(3 == approx.node_connectivity(G));
    assert(3 == approx.node_connectivity(G, 0, 5));
}

auto test_octahedral() -> void {
    G = nx.octahedral_graph();
    assert(4 == approx.node_connectivity(G));
    assert(4 == approx.node_connectivity(G, 0, 5));
}

// Approximation can fail with icosahedral graph depending
// on iteration order.
// auto test_icosahedral() -> void {
//    G=nx.icosahedral_graph();
//    assert_equal(5, approx.node_connectivity(G));
//    assert_equal(5, approx.node_connectivity(G, 0, 5));
}

auto test_only_source() -> void {
    G = nx.complete_graph(5);
    pytest.raises(nx.NetworkXError, approx.node_connectivity, G, s=0);
}

auto test_only_target() -> void {
    G = nx.complete_graph(5);
    pytest.raises(nx.NetworkXError, approx.node_connectivity, G, t=0);
}

auto test_missing_source() -> void {
    G = nx.path_graph(4);
    pytest.raises(nx.NetworkXError, approx.node_connectivity, G, 10, 1);
}

auto test_missing_target() -> void {
    G = nx.path_graph(4);
    pytest.raises(nx.NetworkXError, approx.node_connectivity, G, 1, 10);
}

auto test_source_equals_target() -> void {
    G = nx.complete_graph(5);
    pytest.raises(nx.NetworkXError, approx.local_node_connectivity, G, 0, 0);
}

auto test_directed_node_connectivity() -> void {
    G = nx.cycle_graph(10, create_using=nx.DiGraph()); // only one direction
    D = nx.cycle_graph(10).to_directed(); // 2 reciprocal edges
    assert(1 == approx.node_connectivity(G));
    assert(1 == approx.node_connectivity(G, 1, 4));
    assert(2 == approx.node_connectivity(D));
    assert(2 == approx.node_connectivity(D, 1, 4));
}

class TestAllPairsNodeConnectivityApprox {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.path = nx.path_graph(7);
        cls.directed_path = nx.path_graph(7, create_using=nx.DiGraph());
        cls.cycle = nx.cycle_graph(7);
        cls.directed_cycle = nx.cycle_graph(7, create_using=nx.DiGraph());
        cls.gnp = nx.gnp_random_graph(30, 0.1);
        cls.directed_gnp = nx.gnp_random_graph(30, 0.1, directed=true);
        cls.K20 = nx.complete_graph(20);
        cls.K10 = nx.complete_graph(10);
        cls.K5 = nx.complete_graph(5);
        cls.G_list = [
            cls.path,
            cls.directed_path,
            cls.cycle,
            cls.directed_cycle,
            cls.gnp,
            cls.directed_gnp,
            cls.K10,
            cls.K5,
            cls.K20,
        ];

    auto test_cycles() const -> void {
        K_undir = approx.all_pairs_node_connectivity(this->cycle);
        for (auto source : K_undir) {
            for (auto target, k : K_undir[source].items()) {
                assert(k == 2);
        K_dir = approx.all_pairs_node_connectivity(this->directed_cycle);
        for (auto source : K_dir) {
            for (auto target, k : K_dir[source].items()) {
                assert(k == 1);

    auto test_complete() const -> void {
        for (auto G : [this->K10, this->K5, this->K20]) {
            K = approx.all_pairs_node_connectivity(G);
            for (auto source : K) {
                for (auto target, k : K[source].items()) {
                    assert k == G.size() - 1

    auto test_paths() const -> void {
        K_undir = approx.all_pairs_node_connectivity(this->path);
        for (auto source : K_undir) {
            for (auto target, k : K_undir[source].items()) {
                assert(k == 1);
        K_dir = approx.all_pairs_node_connectivity(this->directed_path);
        for (auto source : K_dir) {
            for (auto target, k : K_dir[source].items()) {
                if (source < target) {
                    assert(k == 1);
                } else {
                    assert(k == 0);

    auto test_cutoff() const -> void {
        for (auto G : [this->K10, this->K5, this->K20]) {
            for (auto mp : [2, 3, 4]) {
                paths = approx.all_pairs_node_connectivity(G, cutoff=mp);
                for (auto source : paths) {
                    for (auto target, K : paths[source].items()) {
                        assert K == mp

    auto test_all_pairs_connectivity_nbunch() const -> void {
        G = nx.complete_graph(5);
        nbunch = [0, 2, 3];
        C = approx.all_pairs_node_connectivity(G, nbunch=nbunch);
        assert(C.size() == nbunch.size());
