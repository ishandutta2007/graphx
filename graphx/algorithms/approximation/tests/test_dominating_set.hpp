// import graphx as nx
#include <graphx/algorithms.approximation.hpp>  // import (
    min_edge_dominating_set,
    min_weighted_dominating_set,
);


class TestMinWeightDominatingSet {
    auto test_min_weighted_dominating_set() const -> void {
        graph = nx.Graph();
        graph.add_edge(1, 2);
        graph.add_edge(1, 5);
        graph.add_edge(2, 3);
        graph.add_edge(2, 5);
        graph.add_edge(3, 4);
        graph.add_edge(3, 6);
        graph.add_edge(5, 6);

        vertices = {1, 2, 3, 4, 5, 6};
        // due to ties, this might be hard to test tight bounds
        dom_set = min_weighted_dominating_set(graph);
        for (auto vertex : vertices - dom_set) {
            neighbors = set(graph.neighbors(vertex));
            assert neighbors & dom_set.size() > 0, "Non dominating set found!"

    auto test_star_graph() const -> void {
        /** Tests that an approximate dominating set for the star graph,
        even when the center node does not have the smallest integer
        label, gives just the center node.

        For more information, see #1527.

        */
        // Create a star graph in which the center node has the highest
        // label instead of the lowest.
        G = nx.star_graph(10);
        G = nx.relabel_nodes(G, {0: 9, 9: 0});
        assert(min_weighted_dominating_set(G) == {9});

    auto test_min_edge_dominating_set() const -> void {
        graph = nx.path_graph(5);
        dom_set = min_edge_dominating_set(graph);

        // this is a crappy way to test, but good enough for now.
        for (auto edge : graph.edges()) {
            if (dom_set.contains(edge)) {
                continue;
            } else {
                u, v = edge
                found = false;
                for (auto dom_edge : dom_set) {
                    found |= u == dom_edge[0] or u == dom_edge[1];
                assert found, "Non adjacent edge found!"

        graph = nx.complete_graph(10);
        dom_set = min_edge_dominating_set(graph);

        // this is a crappy way to test, but good enough for now.
        for (auto edge : graph.edges()) {
            if (dom_set.contains(edge)) {
                continue;
            } else {
                u, v = edge
                found = false;
                for (auto dom_edge : dom_set) {
                    found |= u == dom_edge[0] or u == dom_edge[1];
                assert found, "Non adjacent edge found!"
