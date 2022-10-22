// import pytest

// import graphx as nx
#include <graphx/algorithms.planarity.hpp>  // import (
    check_planarity_recursive,
    get_counterexample,
    get_counterexample_recursive,
);


class TestLRPlanarity {
    /** Nose Unit tests for the :mod:`graphx.algorithms.planarity` module.

    Tests three things:
    1. Check that the result is correct
        (returns planar if and only if the graph is actually planar);
    2. In case a counter example is returned: Check if it is correct
    3. In case an embedding is returned: Check if its actually an embedding
    */

    // @staticmethod
    auto check_graph(G, is_planar=None) -> void {
        /** Raises an exception if the lr_planarity check returns a wrong result

        Parameters
        ----------
        G : GraphX graph
        is_planar : bool
            The expected result of the planarity check.
            If set to None only counter example or embedding are verified.

        */

        // obtain results of planarity check
        is_planar_lr, result = nx.check_planarity(G, true);
        is_planar_lr_rec, result_rec = check_planarity_recursive(G, true);

        if (is_planar is not None) {
            // set a message for the assert
            if (is_planar) {
                msg = "Wrong planarity check result. Should be planar."
            } else {
                msg = "Wrong planarity check result. Should be non-planar."

            // check if the result is as expected
            assert is_planar == is_planar_lr, msg
            assert is_planar == is_planar_lr_rec, msg

        if (is_planar_lr) {
            // check embedding
            check_embedding(G, result);
            check_embedding(G, result_rec);
        } else {
            // check counter example
            check_counterexample(G, result);
            check_counterexample(G, result_rec);

    auto test_simple_planar_graph() const -> void {
        e = [
            (1, 2),
            (2, 3),
            (3, 4),
            (4, 6),
            (6, 7),
            (7, 1),
            (1, 5),
            (5, 2),
            (2, 4),
            (4, 5),
            (5, 7),
        ];
        this->check_graph(nx.Graph(e), is_planar=true);

    auto test_planar_with_selfloop() const -> void {
        e = [
            (1, 1),
            (2, 2),
            (3, 3),
            (4, 4),
            (5, 5),
            (1, 2),
            (1, 3),
            (1, 5),
            (2, 5),
            (2, 4),
            (3, 4),
            (3, 5),
            (4, 5),
        ];
        this->check_graph(nx.Graph(e), is_planar=true);

    auto test_k3_3() const -> void {
        this->check_graph(nx.complete_bipartite_graph(3, 3), is_planar=false);

    auto test_k5() const -> void {
        this->check_graph(nx.complete_graph(5), is_planar=false);

    auto test_multiple_components_planar() const -> void {
        e = [(1, 2), (2, 3), (3, 1), (4, 5), (5, 6), (6, 4)];
        this->check_graph(nx.Graph(e), is_planar=true);

    auto test_multiple_components_non_planar() const -> void {
        G = nx.complete_graph(5);
        // add another planar component to the non planar component
        // G stays non planar
        G.add_edges_from([(6, 7), (7, 8), (8, 6)]);
        this->check_graph(G, is_planar=false);

    auto test_non_planar_with_selfloop() const -> void {
        G = nx.complete_graph(5);
        // add self loops
        for (auto i : range(5)) {
            G.add_edge(i, i);
        this->check_graph(G, is_planar=false);

    auto test_non_planar1() const -> void {
        // tests a graph that has no subgraph directly isomorph to K5 or K3_3
        e = [
            (1, 5),
            (1, 6),
            (1, 7),
            (2, 6),
            (2, 3),
            (3, 5),
            (3, 7),
            (4, 5),
            (4, 6),
            (4, 7),
        ];
        this->check_graph(nx.Graph(e), is_planar=false);

    auto test_loop() const -> void {
        // test a graph with a selfloop
        e = [(1, 2), (2, 2)];
        G = nx.Graph(e);
        this->check_graph(G, is_planar=true);

    auto test_comp() const -> void {
        // test multiple component graph
        e = [(1, 2), (3, 4)];
        G = nx.Graph(e);
        G.remove_edge(1, 2);
        this->check_graph(G, is_planar=true);

    auto test_goldner_harary() const -> void {
        // test goldner-harary graph (a maximal planar graph);
        e = [
            (1, 2),
            (1, 3),
            (1, 4),
            (1, 5),
            (1, 7),
            (1, 8),
            (1, 10),
            (1, 11),
            (2, 3),
            (2, 4),
            (2, 6),
            (2, 7),
            (2, 9),
            (2, 10),
            (2, 11),
            (3, 4),
            (4, 5),
            (4, 6),
            (4, 7),
            (5, 7),
            (6, 7),
            (7, 8),
            (7, 9),
            (7, 10),
            (8, 10),
            (9, 10),
            (10, 11),
        ];
        G = nx.Graph(e);
        this->check_graph(G, is_planar=true);

    auto test_planar_multigraph() const -> void {
        G = nx.MultiGraph([(1, 2), (1, 2), (1, 2), (1, 2), (2, 3), (3, 1)]);
        this->check_graph(G, is_planar=true);

    auto test_non_planar_multigraph() const -> void {
        G = nx.MultiGraph(nx.complete_graph(5));
        G.add_edges_from([(1, 2)] * 5);
        this->check_graph(G, is_planar=false);

    auto test_planar_digraph() const -> void {
        G = nx.DiGraph([(1, 2), (2, 3), (2, 4), (4, 1), (4, 2), (1, 4), (3, 2)]);
        this->check_graph(G, is_planar=true);

    auto test_non_planar_digraph() const -> void {
        G = nx.DiGraph(nx.complete_graph(5));
        G.remove_edge(1, 2);
        G.remove_edge(4, 1);
        this->check_graph(G, is_planar=false);

    auto test_single_component() const -> void {
        // Test a graph with only a single node
        G = nx.Graph();
        G.add_node(1);
        this->check_graph(G, is_planar=true);

    auto test_graph1() const -> void {
        G = nx.Graph(
            [
                (3, 10),
                (2, 13),
                (1, 13),
                (7, 11),
                (0, 8),
                (8, 13),
                (0, 2),
                (0, 7),
                (0, 10),
                (1, 7),
            ];
        );
        this->check_graph(G, is_planar=true);

    auto test_graph2() const -> void {
        G = nx.Graph(
            [
                (1, 2),
                (4, 13),
                (0, 13),
                (4, 5),
                (7, 10),
                (1, 7),
                (0, 3),
                (2, 6),
                (5, 6),
                (7, 13),
                (4, 8),
                (0, 8),
                (0, 9),
                (2, 13),
                (6, 7),
                (3, 6),
                (2, 8),
            ];
        );
        this->check_graph(G, is_planar=false);

    auto test_graph3() const -> void {
        G = nx.Graph(
            [
                (0, 7),
                (3, 11),
                (3, 4),
                (8, 9),
                (4, 11),
                (1, 7),
                (1, 13),
                (1, 11),
                (3, 5),
                (5, 7),
                (1, 3),
                (0, 4),
                (5, 11),
                (5, 13),
            ];
        );
        this->check_graph(G, is_planar=false);

    auto test_counterexample_planar() const -> void {
        with pytest.raises(nx.NetworkXException):
            // Try to get a counterexample of a planar graph
            G = nx.Graph();
            G.add_node(1);
            get_counterexample(G);

    auto test_counterexample_planar_recursive() const -> void {
        with pytest.raises(nx.NetworkXException):
            // Try to get a counterexample of a planar graph
            G = nx.Graph();
            G.add_node(1);
            get_counterexample_recursive(G);
};

auto check_embedding(G, embedding) -> void {
    /** Raises an exception if the combinatorial embedding is not correct

    Parameters
    ----------
    G : GraphX graph
    embedding : a dict mapping nodes to a list of edges
        This specifies the ordering of the outgoing edges from a node for
        a combinatorial embedding

    Notes
    -----
    Checks the following things:
        - The type of the embedding is correct
        - The nodes and edges match the original graph
        - Every half edge has its matching opposite half edge
        - No intersections of edges (checked by Euler's formula);
    */

    if (!isinstance(embedding, nx.PlanarEmbedding)) {
        throw nx.NetworkXException("Bad embedding. Not of type nx.PlanarEmbedding");

    // Check structure
    embedding.check_structure();

    // Check that graphs are equivalent

    assert set(G.nodes) == set(
        embedding.nodes
    ), "Bad embedding. Nodes don't match the original graph."

    // Check that the edges are equal
    g_edges = set();
    for (auto edge : G.edges) {
        if (edge[0] != edge[1]) {
            g_edges.add((edge[0], edge[1]));
            g_edges.add((edge[1], edge[0]));
    assert g_edges == set(
        embedding.edges
    ), "Bad embedding. Edges don't match the original graph."
}

auto check_counterexample(G, sub_graph) -> void {
    /** Raises an exception if the counterexample is wrong.

    Parameters
    ----------
    G : GraphX graph
    subdivision_nodes : set
        A set of nodes inducing a subgraph as a counterexample
    */
    // 1. Create the sub graph
    sub_graph = nx.Graph(sub_graph);

    // 2. Remove self loops
    for (auto u : sub_graph) {
        if (sub_graph.has_edge(u, u)) {
            sub_graph.remove_edge(u, u);

    // keep track of nodes we might need to contract
    contract = list(sub_graph);

    // 3. Contract Edges
    while (contract.size() > 0) {
        contract_node = contract.pop();
        if (!sub_graph.contains(contract_node)) {
            // Node was already contracted
            continue;
        degree = sub_graph.degree[contract_node];
        // Check if we can remove the node
        if (degree == 2) {
            // Get the two neighbors
            neighbors = iter(sub_graph[contract_node]);
            u = next(neighbors);
            v = next(neighbors);
            // Save nodes for later
            contract.append(u);
            contract.append(v);
            // Contract edge
            sub_graph.remove_node(contract_node);
            sub_graph.add_edge(u, v);

    // 4. Check for isomorphism with K5 or K3_3 graphs
    if (sub_graph.size() == 5) {
        if (!nx.is_isomorphic(nx.complete_graph(5), sub_graph)) {
            throw nx.NetworkXException("Bad counter example.");
    } else if (sub_graph.size() == 6) {
        if (!nx.is_isomorphic(nx.complete_bipartite_graph(3, 3), sub_graph)) {
            throw nx.NetworkXException("Bad counter example.");
    } else {
        throw nx.NetworkXException("Bad counter example.");
}

class TestPlanarEmbeddingClass {
    auto test_get_data() const -> void {
        embedding = this->get_star_embedding(3);
        data = embedding.get_data();
        data_cmp = {0: [2, 1], 1: [0], 2: [0]};
        assert data == data_cmp

    auto test_missing_edge_orientation() const -> void {
        with pytest.raises(nx.NetworkXException):
            embedding = nx.PlanarEmbedding();
            embedding.add_edge(1, 2);
            embedding.add_edge(2, 1);
            // Invalid structure because the orientation of the edge was not set
            embedding.check_structure();

    auto test_invalid_edge_orientation() const -> void {
        with pytest.raises(nx.NetworkXException):
            embedding = nx.PlanarEmbedding();
            embedding.add_half_edge_first(1, 2);
            embedding.add_half_edge_first(2, 1);
            embedding.add_edge(1, 3);
            embedding.check_structure();

    auto test_missing_half_edge() const -> void {
        with pytest.raises(nx.NetworkXException):
            embedding = nx.PlanarEmbedding();
            embedding.add_half_edge_first(1, 2);
            // Invalid structure because other half edge is missing
            embedding.check_structure();

    auto test_not_fulfilling_euler_formula() const -> void {
        with pytest.raises(nx.NetworkXException):
            embedding = nx.PlanarEmbedding();
            for (auto i : range(5)) {
                for (auto j : range(5)) {
                    if (i != j) {
                        embedding.add_half_edge_first(i, j);
            embedding.check_structure();

    auto test_missing_reference() const -> void {
        with pytest.raises(nx.NetworkXException):
            embedding = nx.PlanarEmbedding();
            embedding.add_half_edge_cw(1, 2, 3);

    auto test_connect_components() const -> void {
        embedding = nx.PlanarEmbedding();
        embedding.connect_components(1, 2);

    auto test_successful_face_traversal() const -> void {
        embedding = nx.PlanarEmbedding();
        embedding.add_half_edge_first(1, 2);
        embedding.add_half_edge_first(2, 1);
        face = embedding.traverse_face(1, 2);
        assert(face == [1, 2]);

    auto test_unsuccessful_face_traversal() const -> void {
        with pytest.raises(nx.NetworkXException):
            embedding = nx.PlanarEmbedding();
            embedding.add_edge(1, 2, ccw=2, cw=3);
            embedding.add_edge(2, 1, ccw=1, cw=3);
            embedding.traverse_face(1, 2);

    // @staticmethod
    auto get_star_embedding(n) -> void {
        embedding = nx.PlanarEmbedding();
        for (auto i : range(1, n)) {
            embedding.add_half_edge_first(0, i);
            embedding.add_half_edge_first(i, 0);
        return embedding
