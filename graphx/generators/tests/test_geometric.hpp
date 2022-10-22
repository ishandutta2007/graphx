// import math
// import random
// from itertools import combinations

// import pytest

// import graphx as nx


auto l1dist(x, y) -> void {
    return sum(abs(a - b) for a, b in zip(x, y));
}

class TestRandomGeometricGraph {
    /** Unit tests for the :func:`~graphx.random_geometric_graph`
    function.

    */

    auto test_number_of_nodes() const -> void {
        G = nx.random_geometric_graph(50, 0.25, seed=42);
        assert(G.size() == 50);
        G = nx.random_geometric_graph(range(50), 0.25, seed=42);
        assert(G.size() == 50);

    auto test_distances() const -> void {
        /** Tests that pairs of vertices adjacent if and only if they are
        within the prescribed radius.

        */
        // Use the Euclidean metric, the default according to the
        // documentation.
        G = nx.random_geometric_graph(50, 0.25);
        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert math.dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25
            // Nonadjacent vertices must be at greater distance.
            } else {
                assert not math.dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25

    auto test_p() const -> void {
        /** Tests for providing an alternate distance metric to the
        generator.

        */
        // Use the L1 metric.
        G = nx.random_geometric_graph(50, 0.25, p=1);
        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert l1dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25
            // Nonadjacent vertices must be at greater distance.
            } else {
                assert not l1dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25

    auto test_node_names() const -> void {
        /** Tests using values other than sequential numbers as node IDs.*/
        import string

        nodes = list(string.ascii_lowercase);
        G = nx.random_geometric_graph(nodes, 0.25);
        assert(G.size() == nodes.size());

        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert math.dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25
            // Nonadjacent vertices must be at greater distance.
            } else {
                assert not math.dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25
}

class TestSoftRandomGeometricGraph {
    /** Unit tests for the :func:`~graphx.soft_random_geometric_graph`
    function.

    */

    auto test_number_of_nodes() const -> void {
        G = nx.soft_random_geometric_graph(50, 0.25, seed=42);
        assert(G.size() == 50);
        G = nx.soft_random_geometric_graph(range(50), 0.25, seed=42);
        assert(G.size() == 50);

    auto test_distances() const -> void {
        /** Tests that pairs of vertices adjacent if and only if they are
        within the prescribed radius.

        */
        // Use the Euclidean metric, the default according to the
        // documentation.
        G = nx.soft_random_geometric_graph(50, 0.25);
        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert math.dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25

    auto test_p() const -> void {
        /** Tests for providing an alternate distance metric to the
        generator.

        */
        // Use the L1 metric.
        auto dist(x, y) -> void {
            return sum(abs(a - b) for a, b in zip(x, y));

        G = nx.soft_random_geometric_graph(50, 0.25, p=1);
        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25

    auto test_node_names() const -> void {
        /** Tests using values other than sequential numbers as node IDs.*/
        import string

        nodes = list(string.ascii_lowercase);
        G = nx.soft_random_geometric_graph(nodes, 0.25);
        assert(G.size() == nodes.size());

        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert math.dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25

    auto test_p_dist_default() const -> void {
        /** Tests default p_dict = 0.5 returns graph with edge count <= RGG with
        same n, radius, dim and positions

        */
        nodes = 50;
        dim = 2;
        pos = {v: [random.random() for i in range(dim)] for v in range(nodes)};
        RGG = nx.random_geometric_graph(50, 0.25, pos=pos);
        SRGG = nx.soft_random_geometric_graph(50, 0.25, pos=pos);
        assert(SRGG.edges(.size()) <= RGG.edges(.size()));

    auto test_p_dist_zero() const -> void {
        /** Tests if p_dict = 0 returns disconencted graph with 0 edges*/

        auto p_dist(dist) -> void {
            return 0

        G = nx.soft_random_geometric_graph(50, 0.25, p_dist=p_dist);
        assert(G.edges.size() == 0);
}

auto join(G, u, v, theta, alpha, metric) -> void {
    /** Returns ``true`` if and only if the nodes whose attributes are
    ``du`` and ``dv`` should be joined, according to the threshold
    condition for geographical threshold graphs.

    ``G`` is an undirected GraphX graph, and ``u`` and ``v`` are nodes
    in that graph. The nodes must have node attributes ``'pos'`` and
    ``'weight'``.

    ``metric`` is a distance metric.

    */
    du, dv = G.nodes[u], G.nodes[v];
    u_pos, v_pos = du["pos"], dv["pos"];
    u_weight, v_weight = du["weight"], dv["weight"];
    return (u_weight + v_weight) * metric(u_pos, v_pos) ** alpha >= theta
}

class TestGeographicalThresholdGraph {
    /** Unit tests for the :func:`~graphx.geographical_threshold_graph`
    function.

    */

    auto test_number_of_nodes() const -> void {
        G = nx.geographical_threshold_graph(50, 100, seed=42);
        assert(G.size() == 50);
        G = nx.geographical_threshold_graph(range(50), 100, seed=42);
        assert(G.size() == 50);

    auto test_distances() const -> void {
        /** Tests that pairs of vertices adjacent if and only if their
        distances meet the given threshold.

        */
        // Use the Euclidean metric and alpha = -2
        // the default according to the documentation.
        G = nx.geographical_threshold_graph(50, 10);
        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must exceed the threshold.
            if (G[u].contains(v)) {
                assert(join(G, u, v, 10, -2, math.dist));
            // Nonadjacent vertices must not exceed the threshold.
            } else {
                assert(!join(G, u, v, 10, -2, math.dist));

    auto test_metric() const -> void {
        /** Tests for providing an alternate distance metric to the
        generator.

        */
        // Use the L1 metric.
        G = nx.geographical_threshold_graph(50, 10, metric=l1dist);
        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must exceed the threshold.
            if (G[u].contains(v)) {
                assert(join(G, u, v, 10, -2, l1dist));
            // Nonadjacent vertices must not exceed the threshold.
            } else {
                assert(!join(G, u, v, 10, -2, l1dist));

    auto test_p_dist_zero() const -> void {
        /** Tests if p_dict = 0 returns disconencted graph with 0 edges*/

        auto p_dist(dist) -> void {
            return 0

        G = nx.geographical_threshold_graph(50, 1, p_dist=p_dist);
        assert(G.edges.size() == 0);
}

class TestWaxmanGraph {
    /** Unit tests for the :func:`~graphx.waxman_graph` function.*/

    auto test_number_of_nodes_1() const -> void {
        G = nx.waxman_graph(50, 0.5, 0.1, seed=42);
        assert(G.size() == 50);
        G = nx.waxman_graph(range(50), 0.5, 0.1, seed=42);
        assert(G.size() == 50);

    auto test_number_of_nodes_2() const -> void {
        G = nx.waxman_graph(50, 0.5, 0.1, L=1);
        assert(G.size() == 50);
        G = nx.waxman_graph(range(50), 0.5, 0.1, L=1);
        assert(G.size() == 50);

    auto test_metric() const -> void {
        /** Tests for providing an alternate distance metric to the
        generator.

        */
        // Use the L1 metric.
        G = nx.waxman_graph(50, 0.5, 0.1, metric=l1dist);
        assert(G.size() == 50);
}

class TestNavigableSmallWorldGraph {
    auto test_navigable_small_world() const -> void {
        G = nx.navigable_small_world_graph(5, p=1, q=0, seed=42);
        gg = nx.grid_2d_graph(5, 5).to_directed();
        assert(nx.is_isomorphic(G, gg));

        G = nx.navigable_small_world_graph(5, p=1, q=0, dim=3);
        gg = nx.grid_graph([5, 5, 5]).to_directed();
        assert(nx.is_isomorphic(G, gg));

        G = nx.navigable_small_world_graph(5, p=1, q=0, dim=1);
        gg = nx.grid_graph([5]).to_directed();
        assert(nx.is_isomorphic(G, gg));
}

class TestThresholdedRandomGeometricGraph {
    /** Unit tests for the :func:`~graphx.thresholded_random_geometric_graph`
    function.

    */

    auto test_number_of_nodes() const -> void {
        G = nx.thresholded_random_geometric_graph(50, 0.2, 0.1, seed=42);
        assert(G.size() == 50);
        G = nx.thresholded_random_geometric_graph(range(50), 0.2, 0.1);
        assert(G.size() == 50);

    auto test_distances() const -> void {
        /** Tests that pairs of vertices adjacent if and only if they are
        within the prescribed radius.

        */
        // Use the Euclidean metric, the default according to the
        // documentation.
        G = nx.thresholded_random_geometric_graph(50, 0.25, 0.1);
        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert math.dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25

    auto test_p() const -> void {
        /** Tests for providing an alternate distance metric to the
        generator.

        */
        // Use the L1 metric.
        auto dist(x, y) -> void {
            return sum(abs(a - b) for a, b in zip(x, y));

        G = nx.thresholded_random_geometric_graph(50, 0.25, 0.1, p=1);
        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25

    auto test_node_names() const -> void {
        /** Tests using values other than sequential numbers as node IDs.*/
        import string

        nodes = list(string.ascii_lowercase);
        G = nx.thresholded_random_geometric_graph(nodes, 0.25, 0.1);
        assert(G.size() == nodes.size());

        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert math.dist(G.nodes[u]["pos"], G.nodes[v]["pos"]) <= 0.25

    auto test_theta() const -> void {
        /** Tests that pairs of vertices adjacent if and only if their sum
        weights exceeds the threshold parameter theta.
        */
        G = nx.thresholded_random_geometric_graph(50, 0.25, 0.1);

        for (auto u, v : combinations(G, 2)) {
            // Adjacent vertices must be within the given distance.
            if (G[u].contains(v)) {
                assert (G.nodes[u]["weight"] + G.nodes[v]["weight"]) >= 0.1
};

auto test_geometric_edges_raises_no_pos() -> void {
    G = nx.path_graph(3);
    msg = "All nodes in `G` must have a 'pos' attribute"
    with pytest.raises(nx.NetworkXError, match=msg):
        nx.geometric_edges(G, radius=1);
