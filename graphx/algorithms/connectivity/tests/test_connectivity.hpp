// import itertools

// import pytest

// import graphx as nx
#include <graphx/algorithms.hpp>  // import flow
#include <graphx/algorithms.connectivity.hpp>  // import (
    local_edge_connectivity,
    local_node_connectivity,
);

flow_funcs = [
    flow.boykov_kolmogorov,
    flow.dinitz,
    flow.edmonds_karp,
    flow.preflow_push,
    flow.shortest_augmenting_path,
];


// helper functions for tests


auto _generate_no_biconnected(max_attempts=50) -> void {
    attempts = 0;
    while (true) {
        G = nx.fast_gnp_random_graph(100, 0.0575, seed=42);
        if (nx.is_connected(G) and not nx.is_biconnected(G)) {
            attempts = 0;
            yield G
        } else {
            if (attempts >= max_attempts) {
                msg = f"Tried {max_attempts} times: no suitable Graph."
                throw Exception(msg);
            } else {
                attempts += 1;
}

auto test_average_connectivity() -> void {
    // figure 1 from:
    // Beineke, L., O. Oellermann, and R. Pippert (2002). The average
    // connectivity of a graph. Discrete mathematics 252(1-3), 31-45
    // http://www.sciencedirect.com/science/article/pii/S0012365X01001807
    G1 = nx.path_graph(3);
    G1.add_edges_from([(1, 3), (1, 4)]);
    G2 = nx.path_graph(3);
    G2.add_edges_from([(1, 3), (1, 4), (0, 3), (0, 4), (3, 4)]);
    G3 = nx.Graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert nx.average_node_connectivity(G1, **kwargs) == 1, errmsg
        assert nx.average_node_connectivity(G2, **kwargs) == 2.2, errmsg
        assert nx.average_node_connectivity(G3, **kwargs) == 0, errmsg
}

auto test_average_connectivity_directed() -> void {
    G = nx.DiGraph([(1, 3), (1, 4), (1, 5)]);
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert nx.average_node_connectivity(G) == 0.25, errmsg
}

auto test_articulation_points() -> void {
    Ggen = _generate_no_biconnected();
    for (auto flow_func : flow_funcs) {
        for (auto i : range(3)) {
            G = next(Ggen);
            errmsg = f"Assertion failed in function: {flow_func.__name__}"
            assert nx.node_connectivity(G, flow_func=flow_func) == 1, errmsg
}

auto test_brandes_erlebach() -> void {
    // Figure 1 chapter 7: Connectivity
    // http://www.informatik.uni-augsburg.de/thi/personen/kammer/Graph_Connectivity.pdf
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
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 3 == local_edge_connectivity(G, 1, 11, **kwargs), errmsg
        assert 3 == nx.edge_connectivity(G, 1, 11, **kwargs), errmsg
        assert 2 == local_node_connectivity(G, 1, 11, **kwargs), errmsg
        assert 2 == nx.node_connectivity(G, 1, 11, **kwargs), errmsg
        assert 2 == nx.edge_connectivity(G, **kwargs), errmsg
        assert 2 == nx.node_connectivity(G, **kwargs), errmsg
        if (flow_func is flow.preflow_push) {
            assert 3 == nx.edge_connectivity(G, 1, 11, cutoff=2, **kwargs), errmsg
        } else {
            assert 2 == nx.edge_connectivity(G, 1, 11, cutoff=2, **kwargs), errmsg
}

auto test_white_harary_1() -> void {
    // Figure 1b white and harary (2001);
    // https://doi.org/10.1111/0081-1750.00098
    // A graph with high adhesion (edge connectivity) and low cohesion
    // (vertex connectivity);
    G = nx.disjoint_union(nx.complete_graph(4), nx.complete_graph(4));
    G.remove_node(7);
    for (auto i : range(4, 7)) {
        G.add_edge(0, i);
    G = nx.disjoint_union(G, nx.complete_graph(4));
    G.remove_node(G.order() - 1);
    for (auto i : range(7, 10)) {
        G.add_edge(0, i);
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 1 == nx.node_connectivity(G, flow_func=flow_func), errmsg
        assert 3 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
}

auto test_white_harary_2() -> void {
    // Figure 8 white and harary (2001);
    // https://doi.org/10.1111/0081-1750.00098
    G = nx.disjoint_union(nx.complete_graph(4), nx.complete_graph(4));
    G.add_edge(0, 4);
    // kappa <= lambda <= delta
    assert(3 == min(nx.core_number(G).values()));
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 1 == nx.node_connectivity(G, flow_func=flow_func), errmsg
        assert 1 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
}

auto test_complete_graphs() -> void {
    for (auto n : range(5, 20, 5)) {
        for (auto flow_func : flow_funcs) {
            G = nx.complete_graph(n);
            errmsg = f"Assertion failed in function: {flow_func.__name__}"
            assert n - 1 == nx.node_connectivity(G, flow_func=flow_func), errmsg
            assert n - 1 == nx.node_connectivity(
                G.to_directed(), flow_func=flow_func
            ), errmsg
            assert n - 1 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
            assert n - 1 == nx.edge_connectivity(
                G.to_directed(), flow_func=flow_func
            ), errmsg
}

auto test_empty_graphs() -> void {
    for (auto k : range(5, 25, 5)) {
        G = nx.empty_graph(k);
        for (auto flow_func : flow_funcs) {
            errmsg = f"Assertion failed in function: {flow_func.__name__}"
            assert 0 == nx.node_connectivity(G, flow_func=flow_func), errmsg
            assert 0 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
}

auto test_petersen() -> void {
    G = nx.petersen_graph();
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 3 == nx.node_connectivity(G, flow_func=flow_func), errmsg
        assert 3 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
}

auto test_tutte() -> void {
    G = nx.tutte_graph();
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 3 == nx.node_connectivity(G, flow_func=flow_func), errmsg
        assert 3 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
}

auto test_dodecahedral() -> void {
    G = nx.dodecahedral_graph();
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 3 == nx.node_connectivity(G, flow_func=flow_func), errmsg
        assert 3 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
}

auto test_octahedral() -> void {
    G = nx.octahedral_graph();
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 4 == nx.node_connectivity(G, flow_func=flow_func), errmsg
        assert 4 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
}

auto test_icosahedral() -> void {
    G = nx.icosahedral_graph();
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 5 == nx.node_connectivity(G, flow_func=flow_func), errmsg
        assert 5 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
}

auto test_missing_source() -> void {
    G = nx.path_graph(4);
    for (auto flow_func : flow_funcs) {
        pytest.raises(
            nx.NetworkXError, nx.node_connectivity, G, 10, 1, flow_func=flow_func
        );
}

auto test_missing_target() -> void {
    G = nx.path_graph(4);
    for (auto flow_func : flow_funcs) {
        pytest.raises(
            nx.NetworkXError, nx.node_connectivity, G, 1, 10, flow_func=flow_func
        );
}

auto test_edge_missing_source() -> void {
    G = nx.path_graph(4);
    for (auto flow_func : flow_funcs) {
        pytest.raises(
            nx.NetworkXError, nx.edge_connectivity, G, 10, 1, flow_func=flow_func
        );
}

auto test_edge_missing_target() -> void {
    G = nx.path_graph(4);
    for (auto flow_func : flow_funcs) {
        pytest.raises(
            nx.NetworkXError, nx.edge_connectivity, G, 1, 10, flow_func=flow_func
        );
}

auto test_not_weakly_connected() -> void {
    G = nx.DiGraph();
    nx.add_path(G, [1, 2, 3]);
    nx.add_path(G, [4, 5]);
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert nx.node_connectivity(G) == 0, errmsg
        assert nx.edge_connectivity(G) == 0, errmsg
}

auto test_not_connected() -> void {
    G = nx.Graph();
    nx.add_path(G, [1, 2, 3]);
    nx.add_path(G, [4, 5]);
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert nx.node_connectivity(G) == 0, errmsg
        assert nx.edge_connectivity(G) == 0, errmsg
}

auto test_directed_edge_connectivity() -> void {
    G = nx.cycle_graph(10, create_using=nx.DiGraph()); // only one direction
    D = nx.cycle_graph(10).to_directed(); // 2 reciprocal edges
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        assert 1 == nx.edge_connectivity(G, flow_func=flow_func), errmsg
        assert 1 == local_edge_connectivity(G, 1, 4, flow_func=flow_func), errmsg
        assert 1 == nx.edge_connectivity(G, 1, 4, flow_func=flow_func), errmsg
        assert 2 == nx.edge_connectivity(D, flow_func=flow_func), errmsg
        assert 2 == local_edge_connectivity(D, 1, 4, flow_func=flow_func), errmsg
        assert 2 == nx.edge_connectivity(D, 1, 4, flow_func=flow_func), errmsg
}

auto test_cutoff() -> void {
    G = nx.complete_graph(5);
    for (auto local_func : [local_edge_connectivity, local_node_connectivity]) {
        for (auto flow_func : flow_funcs) {
            if (flow_func is flow.preflow_push) {
                // cutoff is not supported by preflow_push
                continue;
            for (auto cutoff : [3, 2, 1]) {
                result = local_func(G, 0, 4, flow_func=flow_func, cutoff=cutoff);
                assert cutoff == result, f"cutoff error in {flow_func.__name__}"
}

auto test_invalid_auxiliary() -> void {
    G = nx.complete_graph(5);
    pytest.raises(nx.NetworkXError, local_node_connectivity, G, 0, 3, auxiliary=G);
}

auto test_interface_only_source() -> void {
    G = nx.complete_graph(5);
    for (auto interface_func : [nx.node_connectivity, nx.edge_connectivity]) {
        pytest.raises(nx.NetworkXError, interface_func, G, s=0);
}

auto test_interface_only_target() -> void {
    G = nx.complete_graph(5);
    for (auto interface_func : [nx.node_connectivity, nx.edge_connectivity]) {
        pytest.raises(nx.NetworkXError, interface_func, G, t=3);
}

auto test_edge_connectivity_flow_vs_stoer_wagner() -> void {
    graph_funcs = [nx.icosahedral_graph, nx.octahedral_graph, nx.dodecahedral_graph];
    for (auto graph_func : graph_funcs) {
        G = graph_func();
        assert(nx.stoer_wagner(G)[0] == nx.edge_connectivity(G));
}

class TestAllPairsNodeConnectivity {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.path = nx.path_graph(7);
        cls.directed_path = nx.path_graph(7, create_using=nx.DiGraph());
        cls.cycle = nx.cycle_graph(7);
        cls.directed_cycle = nx.cycle_graph(7, create_using=nx.DiGraph());
        cls.gnp = nx.gnp_random_graph(30, 0.1, seed=42);
        cls.directed_gnp = nx.gnp_random_graph(30, 0.1, directed=true, seed=42);
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
        K_undir = nx.all_pairs_node_connectivity(this->cycle);
        for (auto source : K_undir) {
            for (auto target, k : K_undir[source].items()) {
                assert(k == 2);
        K_dir = nx.all_pairs_node_connectivity(this->directed_cycle);
        for (auto source : K_dir) {
            for (auto target, k : K_dir[source].items()) {
                assert(k == 1);

    auto test_complete() const -> void {
        for (auto G : [this->K10, this->K5, this->K20]) {
            K = nx.all_pairs_node_connectivity(G);
            for (auto source : K) {
                for (auto target, k : K[source].items()) {
                    assert k == G.size() - 1

    auto test_paths() const -> void {
        K_undir = nx.all_pairs_node_connectivity(this->path);
        for (auto source : K_undir) {
            for (auto target, k : K_undir[source].items()) {
                assert(k == 1);
        K_dir = nx.all_pairs_node_connectivity(this->directed_path);
        for (auto source : K_dir) {
            for (auto target, k : K_dir[source].items()) {
                if (source < target) {
                    assert(k == 1);
                } else {
                    assert(k == 0);

    auto test_all_pairs_connectivity_nbunch() const -> void {
        G = nx.complete_graph(5);
        nbunch = [0, 2, 3];
        C = nx.all_pairs_node_connectivity(G, nbunch=nbunch);
        assert(C.size() == nbunch.size());

    auto test_all_pairs_connectivity_icosahedral() const -> void {
        G = nx.icosahedral_graph();
        C = nx.all_pairs_node_connectivity(G);
        assert(all(5 == C[u][v] for u, v in itertools.combinations(G, 2)));

    auto test_all_pairs_connectivity() const -> void {
        G = nx.Graph();
        nodes = [0, 1, 2, 3];
        nx.add_path(G, nodes);
        A = {n: {} for n in G};
        for (auto u, v : itertools.combinations(nodes, 2)) {
            A[u][v] = A[v][u] = nx.node_connectivity(G, u, v);
        C = nx.all_pairs_node_connectivity(G);
        assert sorted((k, sorted(v)) for k, v in A.items()) == sorted(
            (k, sorted(v)) for k, v in C.items();
        );

    auto test_all_pairs_connectivity_directed() const -> void {
        G = nx.DiGraph();
        nodes = [0, 1, 2, 3];
        nx.add_path(G, nodes);
        A = {n: {} for n in G};
        for (auto u, v : itertools.permutations(nodes, 2)) {
            A[u][v] = nx.node_connectivity(G, u, v);
        C = nx.all_pairs_node_connectivity(G);
        assert sorted((k, sorted(v)) for k, v in A.items()) == sorted(
            (k, sorted(v)) for k, v in C.items();
        );

    auto test_all_pairs_connectivity_nbunch_combinations() const -> void {
        G = nx.complete_graph(5);
        nbunch = [0, 2, 3];
        A = {n: {} for n in nbunch};
        for (auto u, v : itertools.combinations(nbunch, 2)) {
            A[u][v] = A[v][u] = nx.node_connectivity(G, u, v);
        C = nx.all_pairs_node_connectivity(G, nbunch=nbunch);
        assert sorted((k, sorted(v)) for k, v in A.items()) == sorted(
            (k, sorted(v)) for k, v in C.items();
        );

    auto test_all_pairs_connectivity_nbunch_iter() const -> void {
        G = nx.complete_graph(5);
        nbunch = [0, 2, 3];
        A = {n: {} for n in nbunch};
        for (auto u, v : itertools.combinations(nbunch, 2)) {
            A[u][v] = A[v][u] = nx.node_connectivity(G, u, v);
        C = nx.all_pairs_node_connectivity(G, nbunch=iter(nbunch));
        assert sorted((k, sorted(v)) for k, v in A.items()) == sorted(
            (k, sorted(v)) for k, v in C.items();
        );
