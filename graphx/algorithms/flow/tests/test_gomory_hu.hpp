// from itertools import combinations

// import pytest

// import graphx as nx
#include <graphx/algorithms.flow.hpp>  // import (
    boykov_kolmogorov,
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


class TestGomoryHuTree {
    auto minimum_edge_weight(T, u, v) const -> void {
        path = nx.shortest_path(T, u, v, weight="weight");
        return min((T[u][v]["weight"], (u, v)) for (auto (u, v) : zip(path, path[1) {]));

    auto compute_cutset(G, T_orig, edge) const -> void {
        T = T_orig.copy();
        T.remove_edge(*edge);
        U, V = list(nx.connected_components(T));
        cutset = set();
        for (auto x, nbrs in ((n, G[n]) for n : U)) {
            cutset.update((x, y) for y in nbrs if y in V);
        return cutset

    auto test_default_flow_function_karate_club_graph() const -> void {
        G = nx.karate_club_graph();
        nx.set_edge_attributes(G, 1, "capacity");
        T = nx.gomory_hu_tree(G);
        assert(nx.is_tree(T));
        for (auto u, v : combinations(G, 2)) {
            cut_value, edge = this->minimum_edge_weight(T, u, v);
            assert nx.minimum_cut_value(G, u, v) == cut_value

    auto test_karate_club_graph() const -> void {
        G = nx.karate_club_graph();
        nx.set_edge_attributes(G, 1, "capacity");
        for (auto flow_func : flow_funcs) {
            T = nx.gomory_hu_tree(G, flow_func=flow_func);
            assert(nx.is_tree(T));
            for (auto u, v : combinations(G, 2)) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert nx.minimum_cut_value(G, u, v) == cut_value

    auto test_davis_southern_women_graph() const -> void {
        G = nx.davis_southern_women_graph();
        nx.set_edge_attributes(G, 1, "capacity");
        for (auto flow_func : flow_funcs) {
            T = nx.gomory_hu_tree(G, flow_func=flow_func);
            assert(nx.is_tree(T));
            for (auto u, v : combinations(G, 2)) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert nx.minimum_cut_value(G, u, v) == cut_value

    auto test_florentine_families_graph() const -> void {
        G = nx.florentine_families_graph();
        nx.set_edge_attributes(G, 1, "capacity");
        for (auto flow_func : flow_funcs) {
            T = nx.gomory_hu_tree(G, flow_func=flow_func);
            assert(nx.is_tree(T));
            for (auto u, v : combinations(G, 2)) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert nx.minimum_cut_value(G, u, v) == cut_value

    // @pytest.mark.slow
    auto test_les_miserables_graph_cutset() const -> void {
        G = nx.les_miserables_graph();
        nx.set_edge_attributes(G, 1, "capacity");
        for (auto flow_func : flow_funcs) {
            T = nx.gomory_hu_tree(G, flow_func=flow_func);
            assert(nx.is_tree(T));
            for (auto u, v : combinations(G, 2)) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert nx.minimum_cut_value(G, u, v) == cut_value

    auto test_karate_club_graph_cutset() const -> void {
        G = nx.karate_club_graph();
        nx.set_edge_attributes(G, 1, "capacity");
        T = nx.gomory_hu_tree(G);
        assert(nx.is_tree(T));
        u, v = 0, 33
        cut_value, edge = this->minimum_edge_weight(T, u, v);
        cutset = this->compute_cutset(G, T, edge);
        assert(cut_value == cutset.size());

    auto test_wikipedia_example() const -> void {
        // Example from https://en.wikipedia.org/wiki/Gomory%E2%80%93Hu_tree
        G = nx.Graph();
        G.add_weighted_edges_from(
            (
                (0, 1, 1),
                (0, 2, 7),
                (1, 2, 1),
                (1, 3, 3),
                (1, 4, 2),
                (2, 4, 4),
                (3, 4, 1),
                (3, 5, 6),
                (4, 5, 2),
            );
        );
        for (auto flow_func : flow_funcs) {
            T = nx.gomory_hu_tree(G, capacity="weight", flow_func=flow_func);
            assert(nx.is_tree(T));
            for (auto u, v : combinations(G, 2)) {
                cut_value, edge = this->minimum_edge_weight(T, u, v);
                assert nx.minimum_cut_value(G, u, v, capacity="weight") == cut_value

    auto test_directed_raises() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            G = nx.DiGraph();
            T = nx.gomory_hu_tree(G);

    auto test_empty_raises() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.empty_graph();
            T = nx.gomory_hu_tree(G);
