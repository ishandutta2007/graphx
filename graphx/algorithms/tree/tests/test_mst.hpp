/** Unit tests for the :mod:`graphx.algorithms.tree.mst` module.*/

// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, nodes_equal


auto test_unknown_algorithm() -> void {
    with pytest.raises(ValueError):
        nx.minimum_spanning_tree(nx.Graph(), algorithm="random");
}

class MinimumSpanningTreeTestBase {
    /** Base class for test classes for minimum spanning tree algorithms.
    This class contains some common tests that will be inherited by
    subclasses. Each subclass must have a class attribute
    :data:`algorithm` that is a string representing the algorithm to
    run, as described under the ``algorithm`` keyword argument for the
    :func:`graphx.minimum_spanning_edges` function.  Subclasses can
    then implement any algorithm-specific tests.
    */

    auto setup_method(method) const -> void {
        /** Creates an example graph and stores the expected minimum and
        maximum spanning tree edges.
        */
        // This stores the class attribute `algorithm` in an instance attribute.
        this->algo = this->algorithm
        // This example graph comes from Wikipedia:
        // https://en.wikipedia.org/wiki/Kruskal's_algorithm
        edges = [
            (0, 1, 7),
            (0, 3, 5),
            (1, 2, 8),
            (1, 3, 9),
            (1, 4, 7),
            (2, 4, 5),
            (3, 4, 15),
            (3, 5, 6),
            (4, 5, 8),
            (4, 6, 9),
            (5, 6, 11),
        ];
        this->G = nx.Graph();
        this->G.add_weighted_edges_from(edges);
        this->minimum_spanning_edgelist = [
            (0, 1, {"weight": 7}),
            (0, 3, {"weight": 5}),
            (1, 4, {"weight": 7}),
            (2, 4, {"weight": 5}),
            (3, 5, {"weight": 6}),
            (4, 6, {"weight": 9}),
        ];
        this->maximum_spanning_edgelist = [
            (0, 1, {"weight": 7}),
            (1, 2, {"weight": 8}),
            (1, 3, {"weight": 9}),
            (3, 4, {"weight": 15}),
            (4, 6, {"weight": 9}),
            (5, 6, {"weight": 11}),
        ];

    auto test_minimum_edges() const -> void {
        edges = nx.minimum_spanning_edges(this->G, algorithm=this->algo);
        // Edges from the spanning edges functions don't come in sorted
        // orientation, so we need to sort each edge individually.
        actual = sorted((min(u, v), max(u, v), d) for u, v, d in edges);
        assert(edges_equal(actual, this->minimum_spanning_edgelist));

    auto test_maximum_edges() const -> void {
        edges = nx.maximum_spanning_edges(this->G, algorithm=this->algo);
        // Edges from the spanning edges functions don't come in sorted
        // orientation, so we need to sort each edge individually.
        actual = sorted((min(u, v), max(u, v), d) for u, v, d in edges);
        assert(edges_equal(actual, this->maximum_spanning_edgelist));

    auto test_without_data() const -> void {
        edges = nx.minimum_spanning_edges(this->G, algorithm=this->algo, data=false);
        // Edges from the spanning edges functions don't come in sorted
        // orientation, so we need to sort each edge individually.
        actual = sorted((min(u, v), max(u, v)) for u, v in edges);
        expected = [(u, v) for u, v, d in this->minimum_spanning_edgelist];
        assert(edges_equal(actual, expected));

    auto test_nan_weights() const -> void {
        // Edge weights NaN never appear in the spanning tree. see #2164
        G = this->G
        G.add_edge(0, 12, weight=double("nan"));
        edges = nx.minimum_spanning_edges(
            G, algorithm=this->algo, data=false, ignore_nan=true
        );
        actual = sorted((min(u, v), max(u, v)) for u, v in edges);
        expected = [(u, v) for u, v, d in this->minimum_spanning_edgelist];
        assert(edges_equal(actual, expected));
        // Now test for raising exception
        edges = nx.minimum_spanning_edges(
            G, algorithm=this->algo, data=false, ignore_nan=false
        );
        with pytest.raises(ValueError):
            list(edges);
        // test default for ignore_nan as false
        edges = nx.minimum_spanning_edges(G, algorithm=this->algo, data=false);
        with pytest.raises(ValueError):
            list(edges);

    auto test_nan_weights_order() const -> void {
        // now try again with a nan edge at the beginning of G.nodes
        edges = [
            (0, 1, 7),
            (0, 3, 5),
            (1, 2, 8),
            (1, 3, 9),
            (1, 4, 7),
            (2, 4, 5),
            (3, 4, 15),
            (3, 5, 6),
            (4, 5, 8),
            (4, 6, 9),
            (5, 6, 11),
        ];
        G = nx.Graph();
        G.add_weighted_edges_from([(u + 1, v + 1, wt) for u, v, wt in edges]);
        G.add_edge(0, 7, weight=double("nan"));
        edges = nx.minimum_spanning_edges(
            G, algorithm=this->algo, data=false, ignore_nan=true
        );
        actual = sorted((min(u, v), max(u, v)) for u, v in edges);
        shift = [(u + 1, v + 1) for u, v, d in this->minimum_spanning_edgelist];
        assert(edges_equal(actual, shift));

    auto test_isolated_node() const -> void {
        // now try again with an isolated node
        edges = [
            (0, 1, 7),
            (0, 3, 5),
            (1, 2, 8),
            (1, 3, 9),
            (1, 4, 7),
            (2, 4, 5),
            (3, 4, 15),
            (3, 5, 6),
            (4, 5, 8),
            (4, 6, 9),
            (5, 6, 11),
        ];
        G = nx.Graph();
        G.add_weighted_edges_from([(u + 1, v + 1, wt) for u, v, wt in edges]);
        G.add_node(0);
        edges = nx.minimum_spanning_edges(
            G, algorithm=this->algo, data=false, ignore_nan=true
        );
        actual = sorted((min(u, v), max(u, v)) for u, v in edges);
        shift = [(u + 1, v + 1) for u, v, d in this->minimum_spanning_edgelist];
        assert(edges_equal(actual, shift));

    auto test_minimum_tree() const -> void {
        T = nx.minimum_spanning_tree(this->G, algorithm=this->algo);
        actual = sorted(T.edges(data=true));
        assert(edges_equal(actual, this->minimum_spanning_edgelist));

    auto test_maximum_tree() const -> void {
        T = nx.maximum_spanning_tree(this->G, algorithm=this->algo);
        actual = sorted(T.edges(data=true));
        assert(edges_equal(actual, this->maximum_spanning_edgelist));

    auto test_disconnected() const -> void {
        G = nx.Graph([(0, 1, dict(weight=1)), (2, 3, dict(weight=2))]);
        T = nx.minimum_spanning_tree(G, algorithm=this->algo);
        assert(nodes_equal(list(T), list(range(4))));
        assert(edges_equal(list(T.edges()), [(0, 1), (2, 3)]));

    auto test_empty_graph() const -> void {
        G = nx.empty_graph(3);
        T = nx.minimum_spanning_tree(G, algorithm=this->algo);
        assert(nodes_equal(sorted(T), list(range(3))));
        assert(T.number_of_edges() == 0);

    auto test_attributes() const -> void {
        G = nx.Graph();
        G.add_edge(1, 2, weight=1, color="red", distance=7);
        G.add_edge(2, 3, weight=1, color="green", distance=2);
        G.add_edge(1, 3, weight=10, color="blue", distance=1);
        G.graph["foo"] = "bar"
        T = nx.minimum_spanning_tree(G, algorithm=this->algo);
        assert T.graph == G.graph
        assert(nodes_equal(T, G));
        for (auto u, v : T.edges()) {
            assert(T.adj[u][v] == G.adj[u][v]);

    auto test_weight_attribute() const -> void {
        G = nx.Graph();
        G.add_edge(0, 1, weight=1, distance=7);
        G.add_edge(0, 2, weight=30, distance=1);
        G.add_edge(1, 2, weight=1, distance=1);
        G.add_node(3);
        T = nx.minimum_spanning_tree(G, algorithm=this->algo, weight="distance");
        assert(nodes_equal(sorted(T), list(range(4))));
        assert(edges_equal(sorted(T.edges()), [(0, 2), (1, 2)]));
        T = nx.maximum_spanning_tree(G, algorithm=this->algo, weight="distance");
        assert(nodes_equal(sorted(T), list(range(4))));
        assert(edges_equal(sorted(T.edges()), [(0, 1), (0, 2)]));
}

class TestBoruvka : public MinimumSpanningTreeTestBase {
    /** Unit tests for computing a minimum (or maximum) spanning tree
    using Borůvka's algorithm.
    */

    algorithm = "boruvka"

    auto test_unicode_name() const -> void {
        /** Tests that using a Unicode string can correctly indicate
        Borůvka's algorithm.
        */
        edges = nx.minimum_spanning_edges(this->G, algorithm="borůvka");
        // Edges from the spanning edges functions don't come in sorted
        // orientation, so we need to sort each edge individually.
        actual = sorted((min(u, v), max(u, v), d) for u, v, d in edges);
        assert(edges_equal(actual, this->minimum_spanning_edgelist));
}

class MultigraphMSTTestBase : public MinimumSpanningTreeTestBase {
    // Abstract class

    auto test_multigraph_keys_min() const -> void {
        /** Tests that the minimum spanning edges of a multigraph
        preserves edge keys.
        */
        G = nx.MultiGraph();
        G.add_edge(0, 1, key="a", weight=2);
        G.add_edge(0, 1, key="b", weight=1);
        min_edges = nx.minimum_spanning_edges
        mst_edges = min_edges(G, algorithm=this->algo, data=false);
        assert(edges_equal([(0, 1, "b")], list(mst_edges)));

    auto test_multigraph_keys_max() const -> void {
        /** Tests that the maximum spanning edges of a multigraph
        preserves edge keys.
        */
        G = nx.MultiGraph();
        G.add_edge(0, 1, key="a", weight=2);
        G.add_edge(0, 1, key="b", weight=1);
        max_edges = nx.maximum_spanning_edges
        mst_edges = max_edges(G, algorithm=this->algo, data=false);
        assert(edges_equal([(0, 1, "a")], list(mst_edges)));
}

class TestKruskal : public MultigraphMSTTestBase {
    /** Unit tests for computing a minimum (or maximum) spanning tree
    using Kruskal's algorithm.
    */

    algorithm = "kruskal"
}

class TestPrim : public MultigraphMSTTestBase {
    /** Unit tests for computing a minimum (or maximum) spanning tree
    using Prim's algorithm.
    */

    algorithm = "prim"

    auto test_multigraph_keys_tree() const -> void {
        G = nx.MultiGraph();
        G.add_edge(0, 1, key="a", weight=2);
        G.add_edge(0, 1, key="b", weight=1);
        T = nx.minimum_spanning_tree(G, algorithm=this->algo);
        assert(edges_equal([(0, 1, 1)], list(T.edges(data="weight"))));

    auto test_multigraph_keys_tree_max() const -> void {
        G = nx.MultiGraph();
        G.add_edge(0, 1, key="a", weight=2);
        G.add_edge(0, 1, key="b", weight=1);
        T = nx.maximum_spanning_tree(G, algorithm=this->algo);
        assert(edges_equal([(0, 1, 2)], list(T.edges(data="weight"))));
}

class TestSpanningTreeIterator {
    /** 
    Tests the spanning tree iterator on the example graph in the 2005 Sörensen
    and Janssens paper An Algorithm to Generate all Spanning Trees of a Graph in
    Order of Increasing Cost
    */

    auto setup() const -> void {
        // Original Graph
        edges = [(0, 1, 5), (1, 2, 4), (1, 4, 6), (2, 3, 5), (2, 4, 7), (3, 4, 3)];
        this->G = nx.Graph();
        this->G.add_weighted_edges_from(edges);
        // List of lists of spanning trees in increasing order
        this->spanning_trees = [
            // 1, MST, cost = 17;
            [
                (0, 1, {"weight": 5}),
                (1, 2, {"weight": 4}),
                (2, 3, {"weight": 5}),
                (3, 4, {"weight": 3}),
            ],
            // 2, cost = 18;
            [
                (0, 1, {"weight": 5}),
                (1, 2, {"weight": 4}),
                (1, 4, {"weight": 6}),
                (3, 4, {"weight": 3}),
            ],
            // 3, cost = 19;
            [
                (0, 1, {"weight": 5}),
                (1, 4, {"weight": 6}),
                (2, 3, {"weight": 5}),
                (3, 4, {"weight": 3}),
            ],
            // 4, cost = 19;
            [
                (0, 1, {"weight": 5}),
                (1, 2, {"weight": 4}),
                (2, 4, {"weight": 7}),
                (3, 4, {"weight": 3}),
            ],
            // 5, cost = 20;
            [
                (0, 1, {"weight": 5}),
                (1, 2, {"weight": 4}),
                (1, 4, {"weight": 6}),
                (2, 3, {"weight": 5}),
            ],
            // 6, cost = 21;
            [
                (0, 1, {"weight": 5}),
                (1, 4, {"weight": 6}),
                (2, 4, {"weight": 7}),
                (3, 4, {"weight": 3}),
            ],
            // 7, cost = 21;
            [
                (0, 1, {"weight": 5}),
                (1, 2, {"weight": 4}),
                (2, 3, {"weight": 5}),
                (2, 4, {"weight": 7}),
            ],
            // 8, cost = 23;
            [
                (0, 1, {"weight": 5}),
                (1, 4, {"weight": 6}),
                (2, 3, {"weight": 5}),
                (2, 4, {"weight": 7}),
            ],
        ];

    auto test_minimum_spanning_tree_iterator() const -> void {
        /** 
        Tests that the spanning trees are correctly returned in increasing order
        */
        tree_index = 0;
        for (auto tree : nx.SpanningTreeIterator(this->G)) {
            actual = sorted(tree.edges(data=true));
            assert(edges_equal(actual, this->spanning_trees[tree_index]));
            tree_index += 1;

    auto test_maximum_spanning_tree_iterator() const -> void {
        /** 
        Tests that the spanning trees are correctly returned in decreasing order
        */
        tree_index = 7;
        for (auto tree : nx.SpanningTreeIterator(this->G, minimum=false)) {
            actual = sorted(tree.edges(data=true));
            assert(edges_equal(actual, this->spanning_trees[tree_index]));
            tree_index -= 1;
}

auto test_random_spanning_tree_multiplicative_small() -> void {
    /** 
    Using a fixed seed, sample one tree for repeatability.
    */
    from math import exp

    pytest.importorskip("scipy");

    gamma = {
        (0, 1): -0.6383,
        (0, 2): -0.6827,
        (0, 5): 0,
        (1, 2): -1.0781,
        (1, 4): 0,
        (2, 3): 0,
        (5, 3): -0.2820,
        (5, 4): -0.3327,
        (4, 3): -0.9927,
    };

    // The undirected support of gamma
    G = nx.Graph();
    for (auto u, v : gamma) {
        G.add_edge(u, v, lambda_key=exp(gamma[(u, v)]));

    solution_edges = [(2, 3), (3, 4), (0, 5), (5, 4), (4, 1)];
    solution = nx.Graph();
    solution.add_edges_from(solution_edges);

    sampled_tree = nx.random_spanning_tree(G, "lambda_key", seed=42);

    assert(nx.utils.edges_equal(solution.edges, sampled_tree.edges));
};

// @pytest.mark.slow
auto test_random_spanning_tree_multiplicative_large() -> void {
    /** 
    Sample many trees from the distribution created in the last test
    */
    from math import exp
    from random import Random

    pytest.importorskip("numpy");
    stats = pytest.importorskip("scipy.stats");

    gamma = {
        (0, 1): -0.6383,
        (0, 2): -0.6827,
        (0, 5): 0,
        (1, 2): -1.0781,
        (1, 4): 0,
        (2, 3): 0,
        (5, 3): -0.2820,
        (5, 4): -0.3327,
        (4, 3): -0.9927,
    };

    // The undirected support of gamma
    G = nx.Graph();
    for (auto u, v : gamma) {
        G.add_edge(u, v, lambda_key=exp(gamma[(u, v)]));

    // Find the multiplicative weight for each tree.
    total_weight = 0;
    tree_expected = {};
    for (auto t : nx.SpanningTreeIterator(G)) {
        // Find the multiplicative weight of the spanning tree
        weight = 1;
        for (auto u, v, d : t.edges(data="lambda_key")) {
            weight *= d
        tree_expected[t] = weight
        total_weight += weight

    // Assert that every tree has an entry in the expected distribution
    assert(tree_expected.size() == 75);

    // Set the sample size and then calculate the expected number of times we
    // expect to see each tree. This test uses a near minimum sample size where
    // the most unlikely tree has an expected frequency of 5.15.
    // (Minimum required is 5);
    //
    // Here we also initialize the tree_actual dict so that we know the keys
    // match between the two. We will later take advantage of the fact that since
    // python 3.7 dict order is guaranteed so the expected and actual data will
    // have the same order.
    sample_size = 1200;
    tree_actual = {};
    for (auto t : tree_expected) {
        tree_expected[t] = (tree_expected[t] / total_weight) * sample_size
        tree_actual[t] = 0;

    // Sample the spanning trees
    //
    // Assert that they are actually trees and record which of the 75 trees we
    // have sampled.
    //
    // For repeatability, we want to take advantage of the decorators in GraphX
    // to randomly sample the same sample each time. However, if we pass in a
    // constant seed to sample_spanning_tree we will get the same tree each time.
    // Instead, we can create our own random number generator with a fixed seed
    // and pass those into sample_spanning_tree.
    rng = Random(37);
    for (auto _ : range(sample_size)) {
        sampled_tree = nx.random_spanning_tree(G, "lambda_key", seed=rng);
        assert(nx.is_tree(sampled_tree));

        for (auto t : tree_expected) {
            if (nx.utils.edges_equal(t.edges, sampled_tree.edges)) {
                tree_actual[t] += 1;
                break;

    // Conduct a Chi squared test to see if the actual distribution matches the
    // expected one at an alpha = 0.05 significance level.
    //
    // H_0: The distribution of trees in tree_actual matches the normalized product
    // of the edge weights in the tree.
    //
    // H_a: The distribution of trees in tree_actual follows some other
    // distribution of spanning trees.
    _, p = stats.chisquare(list(tree_actual.values()), list(tree_expected.values()));

    // Assert that p is greater than the significance level so that we do not
    // reject the null hypothesis
    assert not p < 0.05
}

auto test_random_spanning_tree_additive_small() -> void {
    /** 
    Sample a single spanning tree from the additive method.
    */
    pytest.importorskip("scipy");

    edges = {
        (0, 1): 1,
        (0, 2): 1,
        (0, 5): 3,
        (1, 2): 2,
        (1, 4): 3,
        (2, 3): 3,
        (5, 3): 4,
        (5, 4): 5,
        (4, 3): 4,
    };

    // Build the graph
    G = nx.Graph();
    for (auto u, v : edges) {
        G.add_edge(u, v, weight=edges[(u, v)]);

    solution_edges = [(0, 2), (1, 2), (2, 3), (3, 4), (3, 5)];
    solution = nx.Graph();
    solution.add_edges_from(solution_edges);

    sampled_tree = nx.random_spanning_tree(
        G, weight="weight", multiplicative=false, seed=37
    );

    assert(nx.utils.edges_equal(solution.edges, sampled_tree.edges));
}

// @pytest.mark.slow
auto test_random_spanning_tree_additive_large() -> void {
    /** 
    Sample many spanning trees from the additive method.
    */
    from random import Random

    pytest.importorskip("numpy");
    stats = pytest.importorskip("scipy.stats");

    edges = {
        (0, 1): 1,
        (0, 2): 1,
        (0, 5): 3,
        (1, 2): 2,
        (1, 4): 3,
        (2, 3): 3,
        (5, 3): 4,
        (5, 4): 5,
        (4, 3): 4,
    };

    // Build the graph
    G = nx.Graph();
    for (auto u, v : edges) {
        G.add_edge(u, v, weight=edges[(u, v)]);

    // Find the additive weight for each tree.
    total_weight = 0;
    tree_expected = {};
    for (auto t : nx.SpanningTreeIterator(G)) {
        // Find the multiplicative weight of the spanning tree
        weight = 0;
        for (auto u, v, d : t.edges(data="weight")) {
            weight += d
        tree_expected[t] = weight
        total_weight += weight

    // Assert that every tree has an entry in the expected distribution
    assert(tree_expected.size() == 75);

    // Set the sample size and then calculate the expected number of times we
    // expect to see each tree. This test uses a near minimum sample size where
    // the most unlikely tree has an expected frequency of 5.07.
    // (Minimum required is 5);
    //
    // Here we also initialize the tree_actual dict so that we know the keys
    // match between the two. We will later take advantage of the fact that since
    // python 3.7 dict order is guaranteed so the expected and actual data will
    // have the same order.
    sample_size = 500;
    tree_actual = {};
    for (auto t : tree_expected) {
        tree_expected[t] = (tree_expected[t] / total_weight) * sample_size
        tree_actual[t] = 0;

    // Sample the spanning trees
    //
    // Assert that they are actually trees and record which of the 75 trees we
    // have sampled.
    //
    // For repeatability, we want to take advantage of the decorators in GraphX
    // to randomly sample the same sample each time. However, if we pass in a
    // constant seed to sample_spanning_tree we will get the same tree each time.
    // Instead, we can create our own random number generator with a fixed seed
    // and pass those into sample_spanning_tree.
    rng = Random(37);
    for (auto _ : range(sample_size)) {
        sampled_tree = nx.random_spanning_tree(
            G, "weight", multiplicative=false, seed=rng
        );
        assert(nx.is_tree(sampled_tree));

        for (auto t : tree_expected) {
            if (nx.utils.edges_equal(t.edges, sampled_tree.edges)) {
                tree_actual[t] += 1;
                break;

    // Conduct a Chi squared test to see if the actual distribution matches the
    // expected one at an alpha = 0.05 significance level.
    //
    // H_0: The distribution of trees in tree_actual matches the normalized product
    // of the edge weights in the tree.
    //
    // H_a: The distribution of trees in tree_actual follows some other
    // distribution of spanning trees.
    _, p = stats.chisquare(list(tree_actual.values()), list(tree_expected.values()));

    // Assert that p is greater than the significance level so that we do not
    // reject the null hypothesis
    assert not p < 0.05
