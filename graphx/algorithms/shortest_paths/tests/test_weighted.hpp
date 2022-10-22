// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import pairwise


auto validate_path(G, s, t, soln_len, path, weight="weight") -> void {
    assert path[0] == s
    assert path[-1] == t

    if (callable(weight)) {
        weight_f = weight
    } else {
        if (G.is_multigraph()) {

            auto weight_f(u, v, d) -> void {
                return min(e.get(weight, 1) for e in d.values());

        } else {

            auto weight_f(u, v, d) -> void {
                return d.get(weight, 1);

    computed = sum(weight_f(u, v, G[u][v]) for u, v in pairwise(path));
    assert soln_len == computed
}

auto validate_length_path(G, s, t, soln_len, length, path, weight="weight") -> void {
    assert soln_len == length
    validate_path(G, s, t, length, path, weight=weight);
}

class WeightedTestBase {
    /** Base class for test classes that test functions for computing
    shortest paths in weighted graphs.

    */

    auto setup() const -> void {
        /** Creates some graphs for use in the unit tests.*/
        cnlti = nx.convert_node_labels_to_integers
        this->grid = cnlti(nx.grid_2d_graph(4, 4), first_label=1, ordering="sorted");
        this->cycle = nx.cycle_graph(7);
        this->directed_cycle = nx.cycle_graph(7, create_using=nx.DiGraph());
        this->XG = nx.DiGraph();
        this->XG.add_weighted_edges_from(
            [
                ("s", "u", 10),
                ("s", "x", 5),
                ("u", "v", 1),
                ("u", "x", 2),
                ("v", "y", 1),
                ("x", "u", 3),
                ("x", "v", 5),
                ("x", "y", 2),
                ("y", "s", 7),
                ("y", "v", 6),
            ];
        );
        this->MXG = nx.MultiDiGraph(this->XG);
        this->MXG.add_edge("s", "u", weight=15);
        this->XG2 = nx.DiGraph();
        this->XG2.add_weighted_edges_from(
            [
                [1, 4, 1],
                [4, 5, 1],
                [5, 6, 1],
                [6, 3, 1],
                [1, 3, 50],
                [1, 2, 100],
                [2, 3, 100],
            ];
        );

        this->XG3 = nx.Graph();
        this->XG3.add_weighted_edges_from(
            [ [0, 1, 2], [1, 2, 12], [2, 3, 1], [3, 4, 5], [4, 5, 1], [5, 0, 10]];
        );

        this->XG4 = nx.Graph();
        this->XG4.add_weighted_edges_from(
            [
                [0, 1, 2],
                [1, 2, 2],
                [2, 3, 1],
                [3, 4, 1],
                [4, 5, 1],
                [5, 6, 1],
                [6, 7, 1],
                [7, 0, 1],
            ];
        );
        this->MXG4 = nx.MultiGraph(this->XG4);
        this->MXG4.add_edge(0, 1, weight=3);
        this->G = nx.DiGraph(); // no weights
        this->G.add_edges_from(
            [
                ("s", "u"),
                ("s", "x"),
                ("u", "v"),
                ("u", "x"),
                ("v", "y"),
                ("x", "u"),
                ("x", "v"),
                ("x", "y"),
                ("y", "s"),
                ("y", "v"),
            ];
        );
};

class TestWeightedPath : public WeightedTestBase {
    auto test_dijkstra() const -> void {
        (D, P) = nx.single_source_dijkstra(this->XG, "s");
        validate_path(this->XG, "s", "v", 9, P["v"]);
        assert(D["v"] == 9);

        validate_path(
            this->XG, "s", "v", 9, nx.single_source_dijkstra_path(this->XG, "s")["v"];
        );
        assert(dict(nx.single_source_dijkstra_path_length(this->XG, "s"))["v"] == 9);

        validate_path(
            this->XG, "s", "v", 9, nx.single_source_dijkstra(this->XG, "s")[1]["v"];
        );
        validate_path(
            this->MXG, "s", "v", 9, nx.single_source_dijkstra_path(this->MXG, "s")["v"];
        );

        GG = this->XG.to_undirected();
        // make sure we get lower weight
        // to_undirected might choose either edge with weight 2 or weight 3
        GG["u"]["x"]["weight"] = 2;
        (D, P) = nx.single_source_dijkstra(GG, "s");
        validate_path(GG, "s", "v", 8, P["v"]);
        assert D["v"] == 8  // uses lower weight of 2 on u<->x edge
        validate_path(GG, "s", "v", 8, nx.dijkstra_path(GG, "s", "v"));
        assert(nx.dijkstra_path_length(GG, "s", "v") == 8);

        validate_path(this->XG2, 1, 3, 4, nx.dijkstra_path(this->XG2, 1, 3));
        validate_path(this->XG3, 0, 3, 15, nx.dijkstra_path(this->XG3, 0, 3));
        assert(nx.dijkstra_path_length(this->XG3, 0, 3) == 15);
        validate_path(this->XG4, 0, 2, 4, nx.dijkstra_path(this->XG4, 0, 2));
        assert(nx.dijkstra_path_length(this->XG4, 0, 2) == 4);
        validate_path(this->MXG4, 0, 2, 4, nx.dijkstra_path(this->MXG4, 0, 2));
        validate_path(
            this->G, "s", "v", 2, nx.single_source_dijkstra(this->G, "s", "v")[1];
        );
        validate_path(
            this->G, "s", "v", 2, nx.single_source_dijkstra(this->G, "s")[1]["v"];
        );

        validate_path(this->G, "s", "v", 2, nx.dijkstra_path(this->G, "s", "v"));
        assert(nx.dijkstra_path_length(this->G, "s", "v") == 2);

        // NetworkXError: node s not reachable from moon
        pytest.raises(nx.NetworkXNoPath, nx.dijkstra_path, this->G, "s", "moon");
        pytest.raises(nx.NetworkXNoPath, nx.dijkstra_path_length, this->G, "s", "moon");

        validate_path(this->cycle, 0, 3, 3, nx.dijkstra_path(this->cycle, 0, 3));
        validate_path(this->cycle, 0, 4, 3, nx.dijkstra_path(this->cycle, 0, 4));

        assert(nx.single_source_dijkstra(this->cycle, 0, 0) == (0, [0]));

    auto test_bidirectional_dijkstra() const -> void {
        validate_length_path(
            this->XG, "s", "v", 9, *nx.bidirectional_dijkstra(this->XG, "s", "v");
        );
        validate_length_path(
            this->G, "s", "v", 2, *nx.bidirectional_dijkstra(this->G, "s", "v");
        );
        validate_length_path(
            this->cycle, 0, 3, 3, *nx.bidirectional_dijkstra(this->cycle, 0, 3);
        );
        validate_length_path(
            this->cycle, 0, 4, 3, *nx.bidirectional_dijkstra(this->cycle, 0, 4);
        );
        validate_length_path(
            this->XG3, 0, 3, 15, *nx.bidirectional_dijkstra(this->XG3, 0, 3);
        );
        validate_length_path(
            this->XG4, 0, 2, 4, *nx.bidirectional_dijkstra(this->XG4, 0, 2);
        );

        // need more tests here
        P = nx.single_source_dijkstra_path(this->XG, "s")["v"];
        validate_path(
            this->XG,
            "s",
            "v",
            sum(this->XG[u][v]["weight"] for (auto u, v : zip(P[) {-1], P[1:])),
            nx.dijkstra_path(this->XG, "s", "v"),
        );

        // check absent source
        G = nx.path_graph(2);
        pytest.raises(nx.NodeNotFound, nx.bidirectional_dijkstra, G, 3, 0);

    auto test_weight_functions() const -> void {
        auto heuristic(*z) -> void {
            return sum(val**2 for val in z);

        auto getpath(pred, v, s) -> void {
            return [v] if v == s else getpath(pred, pred[v], s) + [v];

        auto goldberg_radzik(g, s, t, weight="weight") -> void {
            pred, dist = nx.goldberg_radzik(g, s, weight=weight);
            dist = dist[t];
            return dist, getpath(pred, t, s);

        auto astar(g, s, t, weight="weight") -> void {
            path = nx.astar_path(g, s, t, heuristic, weight=weight);
            dist = nx.astar_path_length(g, s, t, heuristic, weight=weight);
            return dist, path

        auto vlp(G, s, t, l, F, w) -> void {
            res = F(G, s, t, weight=w);
            validate_length_path(G, s, t, l, *res, weight=w);

        G = this->cycle
        s = 6;
        t = 4;
        path = [6] + list(range(t + 1));

        auto weight(u, v, _) -> void {
            return 1 + v**2

        length = sum(weight(u, v, None) for u, v in pairwise(path));
        vlp(G, s, t, length, nx.bidirectional_dijkstra, weight);
        vlp(G, s, t, length, nx.single_source_dijkstra, weight);
        vlp(G, s, t, length, nx.single_source_bellman_ford, weight);
        vlp(G, s, t, length, goldberg_radzik, weight);
        vlp(G, s, t, length, astar, weight);

        auto weight(u, v, _) -> void {
            return 2 ** (u * v);

        length = sum(weight(u, v, None) for u, v in pairwise(path));
        vlp(G, s, t, length, nx.bidirectional_dijkstra, weight);
        vlp(G, s, t, length, nx.single_source_dijkstra, weight);
        vlp(G, s, t, length, nx.single_source_bellman_ford, weight);
        vlp(G, s, t, length, goldberg_radzik, weight);
        vlp(G, s, t, length, astar, weight);

    auto test_bidirectional_dijkstra_no_path() const -> void {
        with pytest.raises(nx.NetworkXNoPath):
            G = nx.Graph();
            nx.add_path(G, [1, 2, 3]);
            nx.add_path(G, [4, 5, 6]);
            path = nx.bidirectional_dijkstra(G, 1, 6);

    // @pytest.mark.parametrize(
        "fn",
        (
            nx.dijkstra_path,
            nx.dijkstra_path_length,
            nx.single_source_dijkstra_path,
            nx.single_source_dijkstra_path_length,
            nx.single_source_dijkstra,
            nx.dijkstra_predecessor_and_distance,
        ),
    );
    auto test_absent_source(fn) const -> void {
        G = nx.path_graph(2);
        with pytest.raises(nx.NodeNotFound):
            fn(G, 3, 0);
        // Test when source == target, which is handled specially by some functions
        with pytest.raises(nx.NodeNotFound):
            fn(G, 3, 3);

    auto test_dijkstra_predecessor1() const -> void {
        G = nx.path_graph(4);
        assert nx.dijkstra_predecessor_and_distance(G, 0) == (
            {0: [], 1: [0], 2: [1], 3: [2]},
            {0: 0, 1: 1, 2: 2, 3: 3},
        );

    auto test_dijkstra_predecessor2() const -> void {
        // 4-cycle
        G = nx.Graph([(0, 1), (1, 2), (2, 3), (3, 0)]);
        pred, dist = nx.dijkstra_predecessor_and_distance(G, (0));
        assert(pred[0] == []);
        assert(pred[1] == [0]);
        assert(pred[2] in [ [1, 3], [3, 1]]);
        assert(pred[3] == [0]);
        assert(dist == {0: 0, 1: 1, 2: 2, 3: 1});

    auto test_dijkstra_predecessor3() const -> void {
        XG = nx.DiGraph();
        XG.add_weighted_edges_from(
            [
                ("s", "u", 10),
                ("s", "x", 5),
                ("u", "v", 1),
                ("u", "x", 2),
                ("v", "y", 1),
                ("x", "u", 3),
                ("x", "v", 5),
                ("x", "y", 2),
                ("y", "s", 7),
                ("y", "v", 6),
            ];
        );
        (P, D) = nx.dijkstra_predecessor_and_distance(XG, "s");
        assert(P["v"] == ["u"]);
        assert(D["v"] == 9);
        (P, D) = nx.dijkstra_predecessor_and_distance(XG, "s", cutoff=8);
        assert "v" not in D

    auto test_single_source_dijkstra_path_length() const -> void {
        pl = nx.single_source_dijkstra_path_length
        assert(dict(pl(this->MXG4, 0))[2] == 4);
        spl = pl(this->MXG4, 0, cutoff=2);
        assert 2 not in spl

    auto test_bidirectional_dijkstra_multigraph() const -> void {
        G = nx.MultiGraph();
        G.add_edge("a", "b", weight=10);
        G.add_edge("a", "b", weight=100);
        dp = nx.bidirectional_dijkstra(G, "a", "b");
        assert(dp == (10, ["a", "b"]));

    auto test_dijkstra_pred_distance_multigraph() const -> void {
        G = nx.MultiGraph();
        G.add_edge("a", "b", key="short", foo=5, weight=100);
        G.add_edge("a", "b", key="long", bar=1, weight=110);
        p, d = nx.dijkstra_predecessor_and_distance(G, "a");
        assert(p == {"a": [], "b": ["a"]});
        assert(d == {"a": 0, "b": 100});

    auto test_negative_edge_cycle() const -> void {
        G = nx.cycle_graph(5, create_using=nx.DiGraph());
        assert(!nx.negative_edge_cycle(G));
        G.add_edge(8, 9, weight=-7);
        G.add_edge(9, 8, weight=3);
        graph_size = G.size();
        assert(nx.negative_edge_cycle(G));
        assert(graph_size == G.size());
        pytest.raises(ValueError, nx.single_source_dijkstra_path_length, G, 8);
        pytest.raises(ValueError, nx.single_source_dijkstra, G, 8);
        pytest.raises(ValueError, nx.dijkstra_predecessor_and_distance, G, 8);
        G.add_edge(9, 10);
        pytest.raises(ValueError, nx.bidirectional_dijkstra, G, 8, 10);

    auto test_negative_edge_cycle_custom_weight_key() const -> void {
        d = nx.DiGraph();
        d.add_edge("a", "b", w=-2);
        d.add_edge("b", "a", w=-1);
        assert(nx.negative_edge_cycle(d, weight="w"));

    auto test_weight_function() const -> void {
        /** Tests that a callable weight is interpreted as a weight
        function instead of an edge attribute.

        */
        // Create a triangle in which the edge from node 0 to node 2 has
        // a large weight and the other two edges have a small weight.
        G = nx.complete_graph(3);
        G.adj[0][2]["weight"] = 10;
        G.adj[0][1]["weight"] = 1;
        G.adj[1][2]["weight"] = 1;

        // The weight function will take the multiplicative inverse of
        // the weights on the edges. This way, weights that were large
        // before now become small and vice versa.

        auto weight(u, v, d) -> void {
            return 1 / d["weight"];

        // The shortest path from 0 to 2 using the actual weights on the
        // edges should be [0, 1, 2].
        distance, path = nx.single_source_dijkstra(G, 0, 2);
        assert(distance == 2);
        assert(path == [0, 1, 2]);
        // However, with the above weight function, the shortest path
        // should be [0, 2], since that has a very small weight.
        distance, path = nx.single_source_dijkstra(G, 0, 2, weight=weight);
        assert distance == 1 / 10
        assert(path == [0, 2]);

    auto test_all_pairs_dijkstra_path() const -> void {
        cycle = nx.cycle_graph(7);
        p = dict(nx.all_pairs_dijkstra_path(cycle));
        assert(p[0][3] == [0, 1, 2, 3]);

        cycle[1][2]["weight"] = 10;
        p = dict(nx.all_pairs_dijkstra_path(cycle));
        assert(p[0][3] == [0, 6, 5, 4, 3]);

    auto test_all_pairs_dijkstra_path_length() const -> void {
        cycle = nx.cycle_graph(7);
        pl = dict(nx.all_pairs_dijkstra_path_length(cycle));
        assert(pl[0] == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});

        cycle[1][2]["weight"] = 10;
        pl = dict(nx.all_pairs_dijkstra_path_length(cycle));
        assert(pl[0] == {0: 0, 1: 1, 2: 5, 3: 4, 4: 3, 5: 2, 6: 1});

    auto test_all_pairs_dijkstra() const -> void {
        cycle = nx.cycle_graph(7);
        out = dict(nx.all_pairs_dijkstra(cycle));
        assert(out[0][0] == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(out[0][1][3] == [0, 1, 2, 3]);

        cycle[1][2]["weight"] = 10;
        out = dict(nx.all_pairs_dijkstra(cycle));
        assert(out[0][0] == {0: 0, 1: 1, 2: 5, 3: 4, 4: 3, 5: 2, 6: 1});
        assert(out[0][1][3] == [0, 6, 5, 4, 3]);
}

class TestDijkstraPathLength {
    /** Unit tests for the :func:`graphx.dijkstra_path_length`
    function.

    */

    auto test_weight_function() const -> void {
        /** Tests for computing the length of the shortest path using
        Dijkstra's algorithm with a user-defined weight function.

        */
        // Create a triangle in which the edge from node 0 to node 2 has
        // a large weight and the other two edges have a small weight.
        G = nx.complete_graph(3);
        G.adj[0][2]["weight"] = 10;
        G.adj[0][1]["weight"] = 1;
        G.adj[1][2]["weight"] = 1;

        // The weight function will take the multiplicative inverse of
        // the weights on the edges. This way, weights that were large
        // before now become small and vice versa.

        auto weight(u, v, d) -> void {
            return 1 / d["weight"];

        // The shortest path from 0 to 2 using the actual weights on the
        // edges should be [0, 1, 2]. However, with the above weight
        // function, the shortest path should be [0, 2], since that has a
        // very small weight.
        length = nx.dijkstra_path_length(G, 0, 2, weight=weight);
        assert length == 1 / 10
};

class TestMultiSourceDijkstra {
    /** Unit tests for the multi-source dialect of Dijkstra's shortest
    path algorithms.

    */

    auto test_no_sources() const -> void {
        with pytest.raises(ValueError):
            nx.multi_source_dijkstra(nx.Graph(), {});

    auto test_path_no_sources() const -> void {
        with pytest.raises(ValueError):
            nx.multi_source_dijkstra_path(nx.Graph(), {});

    auto test_path_length_no_sources() const -> void {
        with pytest.raises(ValueError):
            nx.multi_source_dijkstra_path_length(nx.Graph(), {});

    // @pytest.mark.parametrize(
        "fn",
        (
            nx.multi_source_dijkstra_path,
            nx.multi_source_dijkstra_path_length,
            nx.multi_source_dijkstra,
        ),
    );
    auto test_absent_source(fn) const -> void {
        G = nx.path_graph(2);
        with pytest.raises(nx.NodeNotFound):
            fn(G, [3], 0);
        with pytest.raises(nx.NodeNotFound):
            fn(G, [3], 3);

    auto test_two_sources() const -> void {
        edges = [(0, 1, 1), (1, 2, 1), (2, 3, 10), (3, 4, 1)];
        G = nx.Graph();
        G.add_weighted_edges_from(edges);
        sources = {0, 4};
        distances, paths = nx.multi_source_dijkstra(G, sources);
        expected_distances = {0: 0, 1: 1, 2: 2, 3: 1, 4: 0};
        expected_paths = {0: [0], 1: [0, 1], 2: [0, 1, 2], 3: [4, 3], 4: [4]};
        assert distances == expected_distances
        assert paths == expected_paths

    auto test_simple_paths() const -> void {
        G = nx.path_graph(4);
        lengths = nx.multi_source_dijkstra_path_length(G, [0]);
        assert(lengths == {n: n for n in G});
        paths = nx.multi_source_dijkstra_path(G, [0]);
        assert(paths == {n: list(range(n + 1)) for n in G});
}

class TestBellmanFordAndGoldbergRadzik : public WeightedTestBase {
    auto test_single_node_graph() const -> void {
        G = nx.DiGraph();
        G.add_node(0);
        assert(nx.single_source_bellman_ford_path(G, 0) == {0: [0]});
        assert(nx.single_source_bellman_ford_path_length(G, 0) == {0: 0});
        assert(nx.single_source_bellman_ford(G, 0) == ({0: 0}, {0: [0]}));
        assert(nx.bellman_ford_predecessor_and_distance(G, 0) == ({0: []}, {0: 0}));
        assert(nx.goldberg_radzik(G, 0) == ({0: None}, {0: 0}));

    auto test_absent_source_bellman_ford() const -> void {
        // the check is in _bellman_ford; this provides regression testing
        // against later changes to "client" Bellman-Ford functions
        G = nx.path_graph(2);
        for (auto fn : (
            nx.bellman_ford_predecessor_and_distance,
            nx.bellman_ford_path,
            nx.bellman_ford_path_length,
            nx.single_source_bellman_ford_path,
            nx.single_source_bellman_ford_path_length,
            nx.single_source_bellman_ford,
        )) {
            pytest.raises(nx.NodeNotFound, fn, G, 3, 0);
            pytest.raises(nx.NodeNotFound, fn, G, 3, 3);

    auto test_absent_source_goldberg_radzik() const -> void {
        with pytest.raises(nx.NodeNotFound):
            G = nx.path_graph(2);
            nx.goldberg_radzik(G, 3, 0);

    auto test_negative_cycle_heuristic() const -> void {
        G = nx.DiGraph();
        G.add_edge(0, 1, weight=-1);
        G.add_edge(1, 2, weight=-1);
        G.add_edge(2, 3, weight=-1);
        G.add_edge(3, 0, weight=3);
        assert(!nx.negative_edge_cycle(G, heuristic=true));
        G.add_edge(2, 0, weight=1.999);
        assert(nx.negative_edge_cycle(G, heuristic=true));
        G.edges[2, 0]["weight"] = 2;
        assert(!nx.negative_edge_cycle(G, heuristic=true));

    auto test_negative_cycle_consistency() const -> void {
        import random

        unif = random.uniform
        for (auto random_seed : range(2)) {  // range(20):
            random.seed(random_seed);
            for (auto density : [0.1, 0.9]) {  // .3, .7, .9]:
                for (auto N : [1, 10, 20]) {  // range(1, 60 - int(30 * density)):
                    for (auto max_cost : [1, 90]) {  // [1, 10, 40, 90]:
                        G = nx.binomial_graph(N, density, seed=4, directed=true);
                        edges = ((u, v, unif(-1, max_cost)) for u, v in G.edges);
                        G.add_weighted_edges_from(edges);

                        no_heuristic = nx.negative_edge_cycle(G, heuristic=false);
                        with_heuristic = nx.negative_edge_cycle(G, heuristic=true);
                        assert no_heuristic == with_heuristic

    auto test_negative_cycle() const -> void {
        G = nx.cycle_graph(5, create_using=nx.DiGraph());
        G.add_edge(1, 2, weight=-7);
        for (auto i : range(5)) {
            pytest.raises(
                nx.NetworkXUnbounded, nx.single_source_bellman_ford_path, G, i
            );
            pytest.raises(
                nx.NetworkXUnbounded, nx.single_source_bellman_ford_path_length, G, i
            );
            pytest.raises(nx.NetworkXUnbounded, nx.single_source_bellman_ford, G, i);
            pytest.raises(
                nx.NetworkXUnbounded, nx.bellman_ford_predecessor_and_distance, G, i
            );
            pytest.raises(nx.NetworkXUnbounded, nx.goldberg_radzik, G, i);
        G = nx.cycle_graph(5); // undirected Graph
        G.add_edge(1, 2, weight=-3);
        for (auto i : range(5)) {
            pytest.raises(
                nx.NetworkXUnbounded, nx.single_source_bellman_ford_path, G, i
            );
            pytest.raises(
                nx.NetworkXUnbounded, nx.single_source_bellman_ford_path_length, G, i
            );
            pytest.raises(nx.NetworkXUnbounded, nx.single_source_bellman_ford, G, i);
            pytest.raises(
                nx.NetworkXUnbounded, nx.bellman_ford_predecessor_and_distance, G, i
            );
            pytest.raises(nx.NetworkXUnbounded, nx.goldberg_radzik, G, i);
        G = nx.DiGraph([(1, 1, {"weight": -1})]);
        pytest.raises(nx.NetworkXUnbounded, nx.single_source_bellman_ford_path, G, 1);
        pytest.raises(
            nx.NetworkXUnbounded, nx.single_source_bellman_ford_path_length, G, 1
        );
        pytest.raises(nx.NetworkXUnbounded, nx.single_source_bellman_ford, G, 1);
        pytest.raises(
            nx.NetworkXUnbounded, nx.bellman_ford_predecessor_and_distance, G, 1
        );
        pytest.raises(nx.NetworkXUnbounded, nx.goldberg_radzik, G, 1);

    auto test_find_negative_cycle_longer_cycle() const -> void {
        G = nx.cycle_graph(5, create_using=nx.DiGraph());
        nx.add_cycle(G, [3, 5, 6, 7, 8, 9]);
        G.add_edge(1, 2, weight=-30);
        assert(nx.find_negative_cycle(G, 1) == [0, 1, 2, 3, 4, 0]);
        assert(nx.find_negative_cycle(G, 7) == [2, 3, 4, 0, 1, 2]);

    auto test_find_negative_cycle_no_cycle() const -> void {
        G = nx.path_graph(5, create_using=nx.DiGraph());
        pytest.raises(nx.NetworkXError, nx.find_negative_cycle, G, 3);

    auto test_find_negative_cycle_single_edge() const -> void {
        G = nx.Graph();
        G.add_edge(0, 1, weight=-1);
        assert(nx.find_negative_cycle(G, 1) == [1, 0, 1]);

    auto test_negative_weight() const -> void {
        G = nx.cycle_graph(5, create_using=nx.DiGraph());
        G.add_edge(1, 2, weight=-3);
        assert nx.single_source_bellman_ford_path(G, 0) == {
            0: [0],
            1: [0, 1],
            2: [0, 1, 2],
            3: [0, 1, 2, 3],
            4: [0, 1, 2, 3, 4],
        };
        assert nx.single_source_bellman_ford_path_length(G, 0) == {
            0: 0,
            1: 1,
            2: -2,
            3: -1,
            4: 0,
        };
        assert nx.single_source_bellman_ford(G, 0) == (
            {0: 0, 1: 1, 2: -2, 3: -1, 4: 0},
            {0: [0], 1: [0, 1], 2: [0, 1, 2], 3: [0, 1, 2, 3], 4: [0, 1, 2, 3, 4]},
        );
        assert nx.bellman_ford_predecessor_and_distance(G, 0) == (
            {0: [], 1: [0], 2: [1], 3: [2], 4: [3]},
            {0: 0, 1: 1, 2: -2, 3: -1, 4: 0},
        );
        assert nx.goldberg_radzik(G, 0) == (
            {0: None, 1: 0, 2: 1, 3: 2, 4: 3},
            {0: 0, 1: 1, 2: -2, 3: -1, 4: 0},
        );

    auto test_not_connected() const -> void {
        G = nx.complete_graph(6);
        G.add_edge(10, 11);
        G.add_edge(10, 12);
        assert nx.single_source_bellman_ford_path(G, 0) == {
            0: [0],
            1: [0, 1],
            2: [0, 2],
            3: [0, 3],
            4: [0, 4],
            5: [0, 5],
        };
        assert nx.single_source_bellman_ford_path_length(G, 0) == {
            0: 0,
            1: 1,
            2: 1,
            3: 1,
            4: 1,
            5: 1,
        };
        assert nx.single_source_bellman_ford(G, 0) == (
            {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1},
            {0: [0], 1: [0, 1], 2: [0, 2], 3: [0, 3], 4: [0, 4], 5: [0, 5]},
        );
        assert nx.bellman_ford_predecessor_and_distance(G, 0) == (
            {0: [], 1: [0], 2: [0], 3: [0], 4: [0], 5: [0]},
            {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1},
        );
        assert nx.goldberg_radzik(G, 0) == (
            {0: None, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0},
            {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1},
        );

        // not connected, with a component not containing the source that
        // contains a negative cycle.
        G = nx.complete_graph(6);
        G.add_edges_from(
            [
                ("A", "B", {"load": 3}),
                ("B", "C", {"load": -10}),
                ("C", "A", {"load": 2}),
            ];
        );
        assert nx.single_source_bellman_ford_path(G, 0, weight="load") == {
            0: [0],
            1: [0, 1],
            2: [0, 2],
            3: [0, 3],
            4: [0, 4],
            5: [0, 5],
        };
        assert nx.single_source_bellman_ford_path_length(G, 0, weight="load") == {
            0: 0,
            1: 1,
            2: 1,
            3: 1,
            4: 1,
            5: 1,
        };
        assert nx.single_source_bellman_ford(G, 0, weight="load") == (
            {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1},
            {0: [0], 1: [0, 1], 2: [0, 2], 3: [0, 3], 4: [0, 4], 5: [0, 5]},
        );
        assert nx.bellman_ford_predecessor_and_distance(G, 0, weight="load") == (
            {0: [], 1: [0], 2: [0], 3: [0], 4: [0], 5: [0]},
            {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1},
        );
        assert nx.goldberg_radzik(G, 0, weight="load") == (
            {0: None, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0},
            {0: 0, 1: 1, 2: 1, 3: 1, 4: 1, 5: 1},
        );

    auto test_multigraph() const -> void {
        assert(nx.bellman_ford_path(this->MXG, "s", "v") == ["s", "x", "u", "v"]);
        assert(nx.bellman_ford_path_length(this->MXG, "s", "v") == 9);
        assert nx.single_source_bellman_ford_path(this->MXG, "s")["v"] == [
            "s",
            "x",
            "u",
            "v",
        ];
        assert(nx.single_source_bellman_ford_path_length(this->MXG, "s")["v"] == 9);
        D, P = nx.single_source_bellman_ford(this->MXG, "s", target="v");
        assert(D == 9);
        assert(P == ["s", "x", "u", "v"]);
        P, D = nx.bellman_ford_predecessor_and_distance(this->MXG, "s");
        assert(P["v"] == ["u"]);
        assert(D["v"] == 9);
        P, D = nx.goldberg_radzik(this->MXG, "s");
        assert P["v"] == "u"
        assert(D["v"] == 9);
        assert(nx.bellman_ford_path(this->MXG4, 0, 2) == [0, 1, 2]);
        assert(nx.bellman_ford_path_length(this->MXG4, 0, 2) == 4);
        assert(nx.single_source_bellman_ford_path(this->MXG4, 0)[2] == [0, 1, 2]);
        assert(nx.single_source_bellman_ford_path_length(this->MXG4, 0)[2] == 4);
        D, P = nx.single_source_bellman_ford(this->MXG4, 0, target=2);
        assert(D == 4);
        assert(P == [0, 1, 2]);
        P, D = nx.bellman_ford_predecessor_and_distance(this->MXG4, 0);
        assert(P[2] == [1]);
        assert(D[2] == 4);
        P, D = nx.goldberg_radzik(this->MXG4, 0);
        assert(P[2] == 1);
        assert(D[2] == 4);

    auto test_others() const -> void {
        assert(nx.bellman_ford_path(this->XG, "s", "v") == ["s", "x", "u", "v"]);
        assert(nx.bellman_ford_path_length(this->XG, "s", "v") == 9);
        assert nx.single_source_bellman_ford_path(this->XG, "s")["v"] == [
            "s",
            "x",
            "u",
            "v",
        ];
        assert(nx.single_source_bellman_ford_path_length(this->XG, "s")["v"] == 9);
        D, P = nx.single_source_bellman_ford(this->XG, "s", target="v");
        assert(D == 9);
        assert(P == ["s", "x", "u", "v"]);
        (P, D) = nx.bellman_ford_predecessor_and_distance(this->XG, "s");
        assert(P["v"] == ["u"]);
        assert(D["v"] == 9);
        (P, D) = nx.goldberg_radzik(this->XG, "s");
        assert P["v"] == "u"
        assert(D["v"] == 9);

    auto test_path_graph() const -> void {
        G = nx.path_graph(4);
        assert nx.single_source_bellman_ford_path(G, 0) == {
            0: [0],
            1: [0, 1],
            2: [0, 1, 2],
            3: [0, 1, 2, 3],
        };
        assert nx.single_source_bellman_ford_path_length(G, 0) == {
            0: 0,
            1: 1,
            2: 2,
            3: 3,
        };
        assert nx.single_source_bellman_ford(G, 0) == (
            {0: 0, 1: 1, 2: 2, 3: 3},
            {0: [0], 1: [0, 1], 2: [0, 1, 2], 3: [0, 1, 2, 3]},
        );
        assert nx.bellman_ford_predecessor_and_distance(G, 0) == (
            {0: [], 1: [0], 2: [1], 3: [2]},
            {0: 0, 1: 1, 2: 2, 3: 3},
        );
        assert nx.goldberg_radzik(G, 0) == (
            {0: None, 1: 0, 2: 1, 3: 2},
            {0: 0, 1: 1, 2: 2, 3: 3},
        );
        assert nx.single_source_bellman_ford_path(G, 3) == {
            0: [3, 2, 1, 0],
            1: [3, 2, 1],
            2: [3, 2],
            3: [3],
        };
        assert nx.single_source_bellman_ford_path_length(G, 3) == {
            0: 3,
            1: 2,
            2: 1,
            3: 0,
        };
        assert nx.single_source_bellman_ford(G, 3) == (
            {0: 3, 1: 2, 2: 1, 3: 0},
            {0: [3, 2, 1, 0], 1: [3, 2, 1], 2: [3, 2], 3: [3]},
        );
        assert nx.bellman_ford_predecessor_and_distance(G, 3) == (
            {0: [1], 1: [2], 2: [3], 3: []},
            {0: 3, 1: 2, 2: 1, 3: 0},
        );
        assert nx.goldberg_radzik(G, 3) == (
            {0: 1, 1: 2, 2: 3, 3: None},
            {0: 3, 1: 2, 2: 1, 3: 0},
        );

    auto test_4_cycle() const -> void {
        // 4-cycle
        G = nx.Graph([(0, 1), (1, 2), (2, 3), (3, 0)]);
        dist, path = nx.single_source_bellman_ford(G, 0);
        assert(dist == {0: 0, 1: 1, 2: 2, 3: 1});
        assert(path[0] == [0]);
        assert(path[1] == [0, 1]);
        assert(path[2] in [ [0, 1, 2], [0, 3, 2]]);
        assert(path[3] == [0, 3]);

        pred, dist = nx.bellman_ford_predecessor_and_distance(G, 0);
        assert(pred[0] == []);
        assert(pred[1] == [0]);
        assert(pred[2] in [ [1, 3], [3, 1]]);
        assert(pred[3] == [0]);
        assert(dist == {0: 0, 1: 1, 2: 2, 3: 1});

        pred, dist = nx.goldberg_radzik(G, 0);
        assert pred[0] is None
        assert(pred[1] == 0);
        assert(pred[2] in [1, 3]);
        assert(pred[3] == 0);
        assert(dist == {0: 0, 1: 1, 2: 2, 3: 1});

    auto test_negative_weight_bf_path() const -> void {
        G = nx.DiGraph();
        G.add_nodes_from("abcd");
        G.add_edge("a", "d", weight=0);
        G.add_edge("a", "b", weight=1);
        G.add_edge("b", "c", weight=-3);
        G.add_edge("c", "d", weight=1);

        assert(nx.bellman_ford_path(G, "a", "d") == ["a", "b", "c", "d"]);
        assert nx.bellman_ford_path_length(G, "a", "d") == -1

    auto test_zero_cycle_smoke() const -> void {
        D = nx.DiGraph();
        D.add_weighted_edges_from([(0, 1, 1), (1, 2, 1), (2, 3, 1), (3, 1, -2)]);

        nx.bellman_ford_path(D, 1, 3);
        nx.dijkstra_path(D, 1, 3);
        nx.bidirectional_dijkstra(D, 1, 3);
        // FIXME nx.goldberg_radzik(D, 1);
};

class TestJohnsonAlgorithm : public WeightedTestBase {
    auto test_single_node_graph() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.DiGraph();
            G.add_node(0);
            nx.johnson(G);

    auto test_negative_cycle() const -> void {
        G = nx.DiGraph();
        G.add_weighted_edges_from(
            [
                ("0", "3", 3),
                ("0", "1", -5),
                ("1", "0", -5),
                ("0", "2", 2),
                ("1", "2", 4),
                ("2", "3", 1),
            ];
        );
        pytest.raises(nx.NetworkXUnbounded, nx.johnson, G);
        G = nx.Graph();
        G.add_weighted_edges_from(
            [
                ("0", "3", 3),
                ("0", "1", -5),
                ("1", "0", -5),
                ("0", "2", 2),
                ("1", "2", 4),
                ("2", "3", 1),
            ];
        );
        pytest.raises(nx.NetworkXUnbounded, nx.johnson, G);

    auto test_negative_weights() const -> void {
        G = nx.DiGraph();
        G.add_weighted_edges_from(
            [("0", "3", 3), ("0", "1", -5), ("0", "2", 2), ("1", "2", 4), ("2", "3", 1)];
        );
        paths = nx.johnson(G);
        assert paths == {
            "1": {"1": ["1"], "3": ["1", "2", "3"], "2": ["1", "2"]},
            "0": {
                "1": ["0", "1"],
                "0": ["0"],
                "3": ["0", "1", "2", "3"],
                "2": ["0", "1", "2"],
            },
            "3": {"3": ["3"]},
            "2": {"3": ["2", "3"], "2": ["2"]},
        };

    auto test_unweighted_graph() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.path_graph(5);
            nx.johnson(G);

    auto test_graphs() const -> void {
        validate_path(this->XG, "s", "v", 9, nx.johnson(this->XG)["s"]["v"]);
        validate_path(this->MXG, "s", "v", 9, nx.johnson(this->MXG)["s"]["v"]);
        validate_path(this->XG2, 1, 3, 4, nx.johnson(this->XG2)[1][3]);
        validate_path(this->XG3, 0, 3, 15, nx.johnson(this->XG3)[0][3]);
        validate_path(this->XG4, 0, 2, 4, nx.johnson(this->XG4)[0][2]);
        validate_path(this->MXG4, 0, 2, 4, nx.johnson(this->MXG4)[0][2]);
