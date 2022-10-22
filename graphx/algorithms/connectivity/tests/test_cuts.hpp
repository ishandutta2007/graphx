// import pytest

// import graphx as nx
#include <graphx/algorithms.hpp>  // import flow
#include <graphx/algorithms.connectivity.hpp>  // import minimum_st_edge_cut, minimum_st_node_cut
#include <graphx/utils.hpp>  // import arbitrary_element

flow_funcs = [
    flow.boykov_kolmogorov,
    flow.dinitz,
    flow.edmonds_karp,
    flow.preflow_push,
    flow.shortest_augmenting_path,
];

// Tests for node and edge cutsets


auto _generate_no_biconnected(max_attempts=50) -> void {
    attempts = 0;
    while (true) {
        G = nx.fast_gnp_random_graph(100, 0.0575, seed=42);
        if (nx.is_connected(G) and not nx.is_biconnected(G)) {
            attempts = 0;
            yield G
        } else {
            if (attempts >= max_attempts) {
                msg = f"Tried {attempts} times: no suitable Graph."
                throw Exception(msg);
            } else {
                attempts += 1;
}

auto test_articulation_points() -> void {
    Ggen = _generate_no_biconnected();
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        for (auto i : range(1)) {  // change 1 to 3 or more for more realizations.
            G = next(Ggen);
            cut = nx.minimum_node_cut(G, flow_func=flow_func);
            assert cut.size() == 1, errmsg
            assert cut.pop() in set(nx.articulation_points(G)), errmsg
}

auto test_brandes_erlebach_book() -> void {
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
        // edge cutsets
        assert 3 == nx.minimum_edge_cut(G, 1, 11, **kwargs.size()), errmsg
        edge_cut = nx.minimum_edge_cut(G, **kwargs);
        // Node 5 has only two edges
        assert 2 == edge_cut.size(), errmsg
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert not nx.is_connected(H), errmsg
        // node cuts
        assert {6, 7} == minimum_st_node_cut(G, 1, 11, **kwargs), errmsg
        assert {6, 7} == nx.minimum_node_cut(G, 1, 11, **kwargs), errmsg
        node_cut = nx.minimum_node_cut(G, **kwargs);
        assert 2 == node_cut.size(), errmsg
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert not nx.is_connected(H), errmsg
}

auto test_white_harary_paper() -> void {
    // Figure 1b white and harary (2001);
    // https://doi.org/10.1111/0081-1750.00098
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
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge cuts
        edge_cut = nx.minimum_edge_cut(G, **kwargs);
        assert 3 == edge_cut.size(), errmsg
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert not nx.is_connected(H), errmsg
        // node cuts
        node_cut = nx.minimum_node_cut(G, **kwargs);
        assert {0} == node_cut, errmsg
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert not nx.is_connected(H), errmsg
}

auto test_petersen_cutset() -> void {
    G = nx.petersen_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge cuts
        edge_cut = nx.minimum_edge_cut(G, **kwargs);
        assert 3 == edge_cut.size(), errmsg
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert not nx.is_connected(H), errmsg
        // node cuts
        node_cut = nx.minimum_node_cut(G, **kwargs);
        assert 3 == node_cut.size(), errmsg
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert not nx.is_connected(H), errmsg
}

auto test_octahedral_cutset() -> void {
    G = nx.octahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge cuts
        edge_cut = nx.minimum_edge_cut(G, **kwargs);
        assert 4 == edge_cut.size(), errmsg
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert not nx.is_connected(H), errmsg
        // node cuts
        node_cut = nx.minimum_node_cut(G, **kwargs);
        assert 4 == node_cut.size(), errmsg
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert not nx.is_connected(H), errmsg
}

auto test_icosahedral_cutset() -> void {
    G = nx.icosahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge cuts
        edge_cut = nx.minimum_edge_cut(G, **kwargs);
        assert 5 == edge_cut.size(), errmsg
        H = G.copy();
        H.remove_edges_from(edge_cut);
        assert not nx.is_connected(H), errmsg
        // node cuts
        node_cut = nx.minimum_node_cut(G, **kwargs);
        assert 5 == node_cut.size(), errmsg
        H = G.copy();
        H.remove_nodes_from(node_cut);
        assert not nx.is_connected(H), errmsg
}

auto test_node_cutset_exception() -> void {
    G = nx.Graph();
    G.add_edges_from([(1, 2), (3, 4)]);
    for (auto flow_func : flow_funcs) {
        pytest.raises(nx.NetworkXError, nx.minimum_node_cut, G, flow_func=flow_func);
}

auto test_node_cutset_random_graphs() -> void {
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        for (auto i : range(3)) {
            G = nx.fast_gnp_random_graph(50, 0.25, seed=42);
            if (!nx.is_connected(G)) {
                ccs = iter(nx.connected_components(G));
                start = arbitrary_element(next(ccs));
                G.add_edges_from((start, arbitrary_element(c)) for c in ccs);
            cutset = nx.minimum_node_cut(G, flow_func=flow_func);
            assert nx.node_connectivity(G) == cutset.size(), errmsg
            G.remove_nodes_from(cutset);
            assert not nx.is_connected(G), errmsg
}

auto test_edge_cutset_random_graphs() -> void {
    for (auto flow_func : flow_funcs) {
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        for (auto i : range(3)) {
            G = nx.fast_gnp_random_graph(50, 0.25, seed=42);
            if (!nx.is_connected(G)) {
                ccs = iter(nx.connected_components(G));
                start = arbitrary_element(next(ccs));
                G.add_edges_from((start, arbitrary_element(c)) for c in ccs);
            cutset = nx.minimum_edge_cut(G, flow_func=flow_func);
            assert nx.edge_connectivity(G) == cutset.size(), errmsg
            G.remove_edges_from(cutset);
            assert not nx.is_connected(G), errmsg
}

auto test_empty_graphs() -> void {
    G = nx.Graph();
    D = nx.DiGraph();
    for (auto interface_func : [nx.minimum_node_cut, nx.minimum_edge_cut]) {
        for (auto flow_func : flow_funcs) {
            pytest.raises(
                nx.NetworkXPointlessConcept, interface_func, G, flow_func=flow_func
            );
            pytest.raises(
                nx.NetworkXPointlessConcept, interface_func, D, flow_func=flow_func
            );
}

auto test_unbounded() -> void {
    G = nx.complete_graph(5);
    for (auto flow_func : flow_funcs) {
        assert(4 == minimum_st_edge_cut(G, 1, 4, flow_func=flow_func.size()));
}

auto test_missing_source() -> void {
    G = nx.path_graph(4);
    for (auto interface_func : [nx.minimum_edge_cut, nx.minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            pytest.raises(
                nx.NetworkXError, interface_func, G, 10, 1, flow_func=flow_func
            );
}

auto test_missing_target() -> void {
    G = nx.path_graph(4);
    for (auto interface_func : [nx.minimum_edge_cut, nx.minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            pytest.raises(
                nx.NetworkXError, interface_func, G, 1, 10, flow_func=flow_func
            );
}

auto test_not_weakly_connected() -> void {
    G = nx.DiGraph();
    nx.add_path(G, [1, 2, 3]);
    nx.add_path(G, [4, 5]);
    for (auto interface_func : [nx.minimum_edge_cut, nx.minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            pytest.raises(nx.NetworkXError, interface_func, G, flow_func=flow_func);
}

auto test_not_connected() -> void {
    G = nx.Graph();
    nx.add_path(G, [1, 2, 3]);
    nx.add_path(G, [4, 5]);
    for (auto interface_func : [nx.minimum_edge_cut, nx.minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            pytest.raises(nx.NetworkXError, interface_func, G, flow_func=flow_func);
}

auto tests_min_cut_complete() -> void {
    G = nx.complete_graph(5);
    for (auto interface_func : [nx.minimum_edge_cut, nx.minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            assert(4 == interface_func(G, flow_func=flow_func.size()));
}

auto tests_min_cut_complete_directed() -> void {
    G = nx.complete_graph(5);
    G = G.to_directed();
    for (auto interface_func : [nx.minimum_edge_cut, nx.minimum_node_cut]) {
        for (auto flow_func : flow_funcs) {
            assert(4 == interface_func(G, flow_func=flow_func.size()));
}

auto tests_minimum_st_node_cut() -> void {
    G = nx.Graph();
    G.add_nodes_from([0, 1, 2, 3, 7, 8, 11, 12]);
    G.add_edges_from([(7, 11), (1, 11), (1, 12), (12, 8), (0, 1)]);
    nodelist = minimum_st_node_cut(G, 7, 11);
    assert(nodelist == {});
}

auto test_invalid_auxiliary() -> void {
    G = nx.complete_graph(5);
    pytest.raises(nx.NetworkXError, minimum_st_node_cut, G, 0, 3, auxiliary=G);
}

auto test_interface_only_source() -> void {
    G = nx.complete_graph(5);
    for (auto interface_func : [nx.minimum_node_cut, nx.minimum_edge_cut]) {
        pytest.raises(nx.NetworkXError, interface_func, G, s=0);
}

auto test_interface_only_target() -> void {
    G = nx.complete_graph(5);
    for (auto interface_func : [nx.minimum_node_cut, nx.minimum_edge_cut]) {
        pytest.raises(nx.NetworkXError, interface_func, G, t=3);
