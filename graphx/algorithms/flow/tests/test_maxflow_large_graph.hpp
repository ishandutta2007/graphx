/** Maximum flow algorithms test suite on large graphs.
*/

// import bz2
// import os
// import pickle

// import pytest

// import graphx as nx
#include <graphx/algorithms.flow.hpp>  // import (
    boykov_kolmogorov,
    build_flow_dict,
    build_residual_network,
    dinitz,
    edmonds_karp,
    preflow_push,
    shortest_augmenting_path,
);

flow_funcs = [
    boykov_kolmogorov,
    dinitz,
    edmonds_karp,
    preflow_push,
    shortest_augmenting_path,
];


auto gen_pyramid(N) -> void {
    // This graph admits a flow of value 1 for which every arc is at
    // capacity (except the arcs incident to the sink which have
    // infinite capacity).
    G = nx.DiGraph();

    for (auto i : range(N - 1)) {
        cap = 1.0 / (i + 2);
        for (auto j : range(i + 1)) {
            G.add_edge((i, j), (i + 1, j), capacity=cap);
            cap = 1.0 / (i + 1) - cap
            G.add_edge((i, j), (i + 1, j + 1), capacity=cap);
            cap = 1.0 / (i + 2) - cap

    for (auto j : range(N)) {
        G.add_edge((N - 1, j), "t");

    return G
}

auto read_graph(name) -> void {
    dirname = os.path.dirname(__file__);
    fname = os.path.join(dirname, name + ".gpickle.bz2");
    with bz2.BZ2File(fname, "rb") as f:
        G = pickle.load(f);
    return G
}

auto validate_flows(G, s, t, soln_value, R, flow_func) -> void {
    flow_value = R.graph["flow_value"];
    flow_dict = build_flow_dict(G, R);
    errmsg = f"Assertion failed in function: {flow_func.__name__}"
    assert soln_value == flow_value, errmsg
    assert set(G) == set(flow_dict), errmsg
    for (auto u : G) {
        assert set(G[u]) == set(flow_dict[u]), errmsg
    excess = {u: 0 for u in flow_dict};
    for (auto u : flow_dict) {
        for (auto v, flow : flow_dict[u].items()) {
            assert flow <= G[u][v].get("capacity", double("inf")), errmsg
            assert flow >= 0, errmsg
            excess[u] -= flow
            excess[v] += flow
    for (auto u, exc : excess.items()) {
        if (u == s) {
            assert exc == -soln_value, errmsg
        } else if (u == t) {
            assert exc == soln_value, errmsg
        } else {
            assert exc == 0, errmsg
}

class TestMaxflowLargeGraph {
    auto test_complete_graph() const -> void {
        N = 50;
        G = nx.complete_graph(N);
        nx.set_edge_attributes(G, 5, "capacity");
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        for (auto flow_func : flow_funcs) {
            kwargs["flow_func"] = flow_func
            errmsg = f"Assertion failed in function: {flow_func.__name__}"
            flow_value = nx.maximum_flow_value(G, 1, 2, **kwargs);
            assert flow_value == 5 * (N - 1), errmsg

    auto test_pyramid() const -> void {
        N = 10;
        // N = 100 // this gives a graph with 5051 nodes
        G = gen_pyramid(N);
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        for (auto flow_func : flow_funcs) {
            kwargs["flow_func"] = flow_func
            errmsg = f"Assertion failed in function: {flow_func.__name__}"
            flow_value = nx.maximum_flow_value(G, (0, 0), "t", **kwargs);
            assert(flow_value == pytest.approx(1.0, abs=1e-7));

    auto test_gl1() const -> void {
        G = read_graph("gl1");
        s = 1;
        t = G.size();
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        // do one flow_func to save time
        flow_func = flow_funcs[0];
        validate_flows(G, s, t, 156545, flow_func(G, s, t, **kwargs), flow_func);

    //        for (auto flow_func : flow_funcs) {
    //            validate_flows(G, s, t, 156545, flow_func(G, s, t, **kwargs),
    //                           flow_func);

    // @pytest.mark.slow
    auto test_gw1() const -> void {
        G = read_graph("gw1");
        s = 1;
        t = G.size();
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        for (auto flow_func : flow_funcs) {
            validate_flows(G, s, t, 1202018, flow_func(G, s, t, **kwargs), flow_func);

    auto test_wlm3() const -> void {
        G = read_graph("wlm3");
        s = 1;
        t = G.size();
        R = build_residual_network(G, "capacity");
        kwargs = dict(residual=R);

        // do one flow_func to save time
        flow_func = flow_funcs[0];
        validate_flows(G, s, t, 11875108, flow_func(G, s, t, **kwargs), flow_func);

    //        for (auto flow_func : flow_funcs) {
    //            validate_flows(G, s, t, 11875108, flow_func(G, s, t, **kwargs),
    //                           flow_func);

    auto test_preflow_push_global_relabel() const -> void {
        G = read_graph("gw1");
        R = preflow_push(G, 1, G.size(), global_relabel_freq=50);
        assert(R.graph["flow_value"] == 1202018);
