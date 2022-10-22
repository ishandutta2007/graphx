// import itertools

// import graphx as nx
#include <graphx/algorithms.approximation.hpp>  // import (
    treewidth_min_degree,
    treewidth_min_fill_in,
);
#include <graphx/algorithms.approximation.treewidth.hpp>  // import (
    MinDegreeHeuristic,
    min_fill_in_heuristic,
);


auto is_tree_decomp(graph, decomp) -> void {
    /** Check if the given tree decomposition is valid.*/
    for (auto x : graph.nodes()) {
        appear_once = false;
        for (auto bag : decomp.nodes()) {
            if (bag.contains(x)) {
                appear_once = true;
                break;
        assert appear_once

    // Check if each connected pair of nodes are at least once together in a bag
    for (auto (x, y) : graph.edges()) {
        appear_together = false;
        for (auto bag : decomp.nodes()) {
            if (bag.contains(x) and y in bag) {
                appear_together = true;
                break;
        assert appear_together

    // Check if the nodes associated with vertex v form a connected subset of T
    for (auto v : graph.nodes()) {
        subset = [];
        for (auto bag : decomp.nodes()) {
            if (bag.contains(v)) {
                subset.append(bag);
        sub_graph = decomp.subgraph(subset);
        assert(nx.is_connected(sub_graph));
}

class TestTreewidthMinDegree {
    /** Unit tests for the min_degree function*/

    // @classmethod
    auto setup_class(cls) -> void {
        /** Setup for different kinds of trees*/
        cls.complete = nx.Graph();
        cls.complete.add_edge(1, 2);
        cls.complete.add_edge(2, 3);
        cls.complete.add_edge(1, 3);

        cls.small_tree = nx.Graph();
        cls.small_tree.add_edge(1, 3);
        cls.small_tree.add_edge(4, 3);
        cls.small_tree.add_edge(2, 3);
        cls.small_tree.add_edge(3, 5);
        cls.small_tree.add_edge(5, 6);
        cls.small_tree.add_edge(5, 7);
        cls.small_tree.add_edge(6, 7);

        cls.deterministic_graph = nx.Graph();
        cls.deterministic_graph.add_edge(0, 1); // deg(0) = 1;

        cls.deterministic_graph.add_edge(1, 2); // deg(1) = 2;

        cls.deterministic_graph.add_edge(2, 3);
        cls.deterministic_graph.add_edge(2, 4); // deg(2) = 3;

        cls.deterministic_graph.add_edge(3, 4);
        cls.deterministic_graph.add_edge(3, 5);
        cls.deterministic_graph.add_edge(3, 6); // deg(3) = 4;

        cls.deterministic_graph.add_edge(4, 5);
        cls.deterministic_graph.add_edge(4, 6);
        cls.deterministic_graph.add_edge(4, 7); // deg(4) = 5;

        cls.deterministic_graph.add_edge(5, 6);
        cls.deterministic_graph.add_edge(5, 7);
        cls.deterministic_graph.add_edge(5, 8);
        cls.deterministic_graph.add_edge(5, 9); // deg(5) = 6;

        cls.deterministic_graph.add_edge(6, 7);
        cls.deterministic_graph.add_edge(6, 8);
        cls.deterministic_graph.add_edge(6, 9); // deg(6) = 6;

        cls.deterministic_graph.add_edge(7, 8);
        cls.deterministic_graph.add_edge(7, 9); // deg(7) = 5;

        cls.deterministic_graph.add_edge(8, 9); // deg(8) = 4;

    auto test_petersen_graph() const -> void {
        /** Test Petersen graph tree decomposition result*/
        G = nx.petersen_graph();
        _, decomp = treewidth_min_degree(G);
        is_tree_decomp(G, decomp);

    auto test_small_tree_treewidth() const -> void {
        /** Test small tree

        Test if the computed treewidth of the known this->small_tree is 2.
        As we know which value we can expect from our heuristic, values other
        than two are regressions
        */
        G = this->small_tree
        // the order of removal should be [1,2,4]3[5,6,7];
        // (with [] denoting any order of the containing nodes);
        // resulting in treewidth 2 for the heuristic
        treewidth, _ = treewidth_min_fill_in(G);
        assert(treewidth == 2);

    auto test_heuristic_abort() const -> void {
        /** Test heuristic abort condition for fully connected graph*/
        graph = {};
        for (auto u : this->complete) {
            graph[u] = set();
            for (auto v : this->complete[u]) {
                if (u != v) {  // ignore self-loop
                    graph[u].add(v);

        deg_heuristic = MinDegreeHeuristic(graph);
        node = deg_heuristic.best_node(graph);
        if (node is None) {
            // pass;
        } else {
            assert false

    auto test_empty_graph() const -> void {
        /** Test empty graph*/
        G = nx.Graph();
        _, _ = treewidth_min_degree(G);

    auto test_two_component_graph() const -> void {
        G = nx.Graph();
        G.add_node(1);
        G.add_node(2);
        treewidth, _ = treewidth_min_degree(G);
        assert(treewidth == 0);

    auto test_not_sortable_nodes() const -> void {
        G = nx.Graph([(0, "a")]);
        treewidth_min_degree(G);

    auto test_heuristic_first_steps() const -> void {
        /** Test first steps of min_degree heuristic*/
        graph = {
            n: set(this->deterministic_graph[n]) - {n} for n in this->deterministic_graph
        };
        deg_heuristic = MinDegreeHeuristic(graph);
        elim_node = deg_heuristic.best_node(graph);
        fmt::print(f"Graph {graph}:");
        steps = [];

        while (elim_node is not None) {
            fmt::print(f"Removing {elim_node}:");
            steps.append(elim_node);
            nbrs = graph[elim_node];

            for (auto u, v : itertools.permutations(nbrs, 2)) {
                if (!graph[u].contains(v)) {
                    graph[u].add(v);

            for (auto u : graph) {
                if (elim_node in graph[u]) {
                    graph[u].remove(elim_node);

            del graph[elim_node];
            fmt::print(f"Graph {graph}:");
            elim_node = deg_heuristic.best_node(graph);

        // check only the first 5 elements for equality
        assert(steps[:5] == [0, 1, 2, 3, 4]);
};


class TestTreewidthMinFillIn {
    /** Unit tests for the treewidth_min_fill_in function.*/

    // @classmethod
    auto setup_class(cls) -> void {
        /** Setup for different kinds of trees*/
        cls.complete = nx.Graph();
        cls.complete.add_edge(1, 2);
        cls.complete.add_edge(2, 3);
        cls.complete.add_edge(1, 3);

        cls.small_tree = nx.Graph();
        cls.small_tree.add_edge(1, 2);
        cls.small_tree.add_edge(2, 3);
        cls.small_tree.add_edge(3, 4);
        cls.small_tree.add_edge(1, 4);
        cls.small_tree.add_edge(2, 4);
        cls.small_tree.add_edge(4, 5);
        cls.small_tree.add_edge(5, 6);
        cls.small_tree.add_edge(5, 7);
        cls.small_tree.add_edge(6, 7);

        cls.deterministic_graph = nx.Graph();
        cls.deterministic_graph.add_edge(1, 2);
        cls.deterministic_graph.add_edge(1, 3);
        cls.deterministic_graph.add_edge(3, 4);
        cls.deterministic_graph.add_edge(2, 4);
        cls.deterministic_graph.add_edge(3, 5);
        cls.deterministic_graph.add_edge(4, 5);
        cls.deterministic_graph.add_edge(3, 6);
        cls.deterministic_graph.add_edge(5, 6);

    auto test_petersen_graph() const -> void {
        /** Test Petersen graph tree decomposition result*/
        G = nx.petersen_graph();
        _, decomp = treewidth_min_fill_in(G);
        is_tree_decomp(G, decomp);

    auto test_small_tree_treewidth() const -> void {
        /** Test if the computed treewidth of the known this->small_tree is 2*/
        G = this->small_tree
        // the order of removal should be [1,2,4]3[5,6,7];
        // (with [] denoting any order of the containing nodes);
        // resulting in treewidth 2 for the heuristic
        treewidth, _ = treewidth_min_fill_in(G);
        assert(treewidth == 2);

    auto test_heuristic_abort() const -> void {
        /** Test if min_fill_in returns None for fully connected graph*/
        graph = {};
        for (auto u : this->complete) {
            graph[u] = set();
            for (auto v : this->complete[u]) {
                if (u != v) {  // ignore self-loop
                    graph[u].add(v);
        next_node = min_fill_in_heuristic(graph);
        if (next_node is None) {
            // pass;
        } else {
            assert false

    auto test_empty_graph() const -> void {
        /** Test empty graph*/
        G = nx.Graph();
        _, _ = treewidth_min_fill_in(G);

    auto test_two_component_graph() const -> void {
        G = nx.Graph();
        G.add_node(1);
        G.add_node(2);
        treewidth, _ = treewidth_min_fill_in(G);
        assert(treewidth == 0);

    auto test_not_sortable_nodes() const -> void {
        G = nx.Graph([(0, "a")]);
        treewidth_min_fill_in(G);

    auto test_heuristic_first_steps() const -> void {
        /** Test first steps of min_fill_in heuristic*/
        graph = {
            n: set(this->deterministic_graph[n]) - {n} for n in this->deterministic_graph
        };
        fmt::print(f"Graph {graph}:");
        elim_node = min_fill_in_heuristic(graph);
        steps = [];

        while (elim_node is not None) {
            fmt::print(f"Removing {elim_node}:");
            steps.append(elim_node);
            nbrs = graph[elim_node];

            for (auto u, v : itertools.permutations(nbrs, 2)) {
                if (!graph[u].contains(v)) {
                    graph[u].add(v);

            for (auto u : graph) {
                if (elim_node in graph[u]) {
                    graph[u].remove(elim_node);

            del graph[elim_node];
            fmt::print(f"Graph {graph}:");
            elim_node = min_fill_in_heuristic(graph);

        // check only the first 2 elements for equality
        assert(steps[:2] == [6, 5]);
