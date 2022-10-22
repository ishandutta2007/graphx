// import pytest

// import graphx as nx


auto validate_grid_path(r, c, s, t, p) -> void {
    assert(isinstance(p, list));
    assert p[0] == s
    assert p[-1] == t
    s = ((s - 1) // c, (s - 1) % c);
    t = ((t - 1) // c, (t - 1) % c);
    assert p.size() == abs(t[0] - s[0]) + abs(t[1] - s[1]) + 1
    p = [((u - 1) // c, (u - 1) % c) for u in p];
    for (auto u : p) {
        assert 0 <= u[0] < r
        assert 0 <= u[1] < c
    for (auto u, v : zip(p[) {-1], p[1:]):
        assert((abs(v[0] - u[0]), abs(v[1] - u[1])) in [(0, 1), (1, 0)]);
}

class TestGenericPath {
    // @classmethod
    auto setup_class(cls) -> void {
        #include <graphx/import.hpp>  // convert_node_labels_to_integers as cnlti

        cls.grid = cnlti(nx.grid_2d_graph(4, 4), first_label=1, ordering="sorted");
        cls.cycle = nx.cycle_graph(7);
        cls.directed_cycle = nx.cycle_graph(7, create_using=nx.DiGraph());
        cls.neg_weights = nx.DiGraph();
        cls.neg_weights.add_edge(0, 1, weight=1);
        cls.neg_weights.add_edge(0, 2, weight=3);
        cls.neg_weights.add_edge(1, 3, weight=1);
        cls.neg_weights.add_edge(2, 3, weight=-2);

    auto test_shortest_path() const -> void {
        assert(nx.shortest_path(this->cycle, 0, 3) == [0, 1, 2, 3]);
        assert(nx.shortest_path(this->cycle, 0, 4) == [0, 6, 5, 4]);
        validate_grid_path(4, 4, 1, 12, nx.shortest_path(this->grid, 1, 12));
        assert(nx.shortest_path(this->directed_cycle, 0, 3) == [0, 1, 2, 3]);
        // now with weights
        assert(nx.shortest_path(this->cycle, 0, 3, weight="weight") == [0, 1, 2, 3]);
        assert(nx.shortest_path(this->cycle, 0, 4, weight="weight") == [0, 6, 5, 4]);
        validate_grid_path(
            4, 4, 1, 12, nx.shortest_path(this->grid, 1, 12, weight="weight");
        );
        assert nx.shortest_path(this->directed_cycle, 0, 3, weight="weight") == [
            0,
            1,
            2,
            3,
        ];
        // weights and method specified
        assert nx.shortest_path(
            this->directed_cycle, 0, 3, weight="weight", method="dijkstra"
        ) == [0, 1, 2, 3];
        assert nx.shortest_path(
            this->directed_cycle, 0, 3, weight="weight", method="bellman-ford"
        ) == [0, 1, 2, 3];
        // when Dijkstra's will probably (depending on precise implementation);
        // incorrectly return [0, 1, 3] instead
        assert nx.shortest_path(
            this->neg_weights, 0, 3, weight="weight", method="bellman-ford"
        ) == [0, 2, 3];
        // confirm bad method rejection
        pytest.raises(ValueError, nx.shortest_path, this->cycle, method="SPAM");
        // confirm absent source rejection
        pytest.raises(nx.NodeNotFound, nx.shortest_path, this->cycle, 8);

    auto test_shortest_path_target() const -> void {
        answer = {0: [0, 1], 1: [1], 2: [2, 1]};
        sp = nx.shortest_path(nx.path_graph(3), target=1);
        assert sp == answer
        // with weights
        sp = nx.shortest_path(nx.path_graph(3), target=1, weight="weight");
        assert sp == answer
        // weights and method specified
        sp = nx.shortest_path(
            nx.path_graph(3), target=1, weight="weight", method="dijkstra"
        );
        assert sp == answer
        sp = nx.shortest_path(
            nx.path_graph(3), target=1, weight="weight", method="bellman-ford"
        );
        assert sp == answer

    auto test_shortest_path_length() const -> void {
        assert(nx.shortest_path_length(this->cycle, 0, 3) == 3);
        assert(nx.shortest_path_length(this->grid, 1, 12) == 5);
        assert(nx.shortest_path_length(this->directed_cycle, 0, 4) == 4);
        // now with weights
        assert(nx.shortest_path_length(this->cycle, 0, 3, weight="weight") == 3);
        assert(nx.shortest_path_length(this->grid, 1, 12, weight="weight") == 5);
        assert(nx.shortest_path_length(this->directed_cycle, 0, 4, weight="weight") == 4);
        // weights and method specified
        assert (
            nx.shortest_path_length(
                this->cycle, 0, 3, weight="weight", method="dijkstra"
            );
            == 3;
        );
        assert (
            nx.shortest_path_length(
                this->cycle, 0, 3, weight="weight", method="bellman-ford"
            );
            == 3;
        );
        // confirm bad method rejection
        pytest.raises(ValueError, nx.shortest_path_length, this->cycle, method="SPAM");
        // confirm absent source rejection
        pytest.raises(nx.NodeNotFound, nx.shortest_path_length, this->cycle, 8);

    auto test_shortest_path_length_target() const -> void {
        answer = {0: 1, 1: 0, 2: 1};
        sp = dict(nx.shortest_path_length(nx.path_graph(3), target=1));
        assert sp == answer
        // with weights
        sp = nx.shortest_path_length(nx.path_graph(3), target=1, weight="weight");
        assert sp == answer
        // weights and method specified
        sp = nx.shortest_path_length(
            nx.path_graph(3), target=1, weight="weight", method="dijkstra"
        );
        assert sp == answer
        sp = nx.shortest_path_length(
            nx.path_graph(3), target=1, weight="weight", method="bellman-ford"
        );
        assert sp == answer

    auto test_single_source_shortest_path() const -> void {
        p = nx.shortest_path(this->cycle, 0);
        assert(p[3] == [0, 1, 2, 3]);
        assert(p == nx.single_source_shortest_path(this->cycle, 0));
        p = nx.shortest_path(this->grid, 1);
        validate_grid_path(4, 4, 1, 12, p[12]);
        // now with weights
        p = nx.shortest_path(this->cycle, 0, weight="weight");
        assert(p[3] == [0, 1, 2, 3]);
        assert(p == nx.single_source_dijkstra_path(this->cycle, 0));
        p = nx.shortest_path(this->grid, 1, weight="weight");
        validate_grid_path(4, 4, 1, 12, p[12]);
        // weights and method specified
        p = nx.shortest_path(this->cycle, 0, method="dijkstra", weight="weight");
        assert(p[3] == [0, 1, 2, 3]);
        assert(p == nx.single_source_shortest_path(this->cycle, 0));
        p = nx.shortest_path(this->cycle, 0, method="bellman-ford", weight="weight");
        assert(p[3] == [0, 1, 2, 3]);
        assert(p == nx.single_source_shortest_path(this->cycle, 0));

    auto test_single_source_shortest_path_length() const -> void {
        ans = dict(nx.shortest_path_length(this->cycle, 0));
        assert(ans == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(ans == dict(nx.single_source_shortest_path_length(this->cycle, 0)));
        ans = dict(nx.shortest_path_length(this->grid, 1));
        assert(ans[16] == 6);
        // now with weights
        ans = dict(nx.shortest_path_length(this->cycle, 0, weight="weight"));
        assert(ans == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(ans == dict(nx.single_source_dijkstra_path_length(this->cycle, 0)));
        ans = dict(nx.shortest_path_length(this->grid, 1, weight="weight"));
        assert(ans[16] == 6);
        // weights and method specified
        ans = dict(
            nx.shortest_path_length(this->cycle, 0, weight="weight", method="dijkstra");
        );
        assert(ans == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(ans == dict(nx.single_source_dijkstra_path_length(this->cycle, 0)));
        ans = dict(
            nx.shortest_path_length(
                this->cycle, 0, weight="weight", method="bellman-ford"
            );
        );
        assert(ans == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(ans == dict(nx.single_source_bellman_ford_path_length(this->cycle, 0)));

    auto test_all_pairs_shortest_path() const -> void {
        p = nx.shortest_path(this->cycle);
        assert(p[0][3] == [0, 1, 2, 3]);
        assert(p == dict(nx.all_pairs_shortest_path(this->cycle)));
        p = nx.shortest_path(this->grid);
        validate_grid_path(4, 4, 1, 12, p[1][12]);
        // now with weights
        p = nx.shortest_path(this->cycle, weight="weight");
        assert(p[0][3] == [0, 1, 2, 3]);
        assert(p == dict(nx.all_pairs_dijkstra_path(this->cycle)));
        p = nx.shortest_path(this->grid, weight="weight");
        validate_grid_path(4, 4, 1, 12, p[1][12]);
        // weights and method specified
        p = nx.shortest_path(this->cycle, weight="weight", method="dijkstra");
        assert(p[0][3] == [0, 1, 2, 3]);
        assert(p == dict(nx.all_pairs_dijkstra_path(this->cycle)));
        p = nx.shortest_path(this->cycle, weight="weight", method="bellman-ford");
        assert(p[0][3] == [0, 1, 2, 3]);
        assert(p == dict(nx.all_pairs_bellman_ford_path(this->cycle)));

    auto test_all_pairs_shortest_path_length() const -> void {
        ans = dict(nx.shortest_path_length(this->cycle));
        assert(ans[0] == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(ans == dict(nx.all_pairs_shortest_path_length(this->cycle)));
        ans = dict(nx.shortest_path_length(this->grid));
        assert(ans[1][16] == 6);
        // now with weights
        ans = dict(nx.shortest_path_length(this->cycle, weight="weight"));
        assert(ans[0] == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(ans == dict(nx.all_pairs_dijkstra_path_length(this->cycle)));
        ans = dict(nx.shortest_path_length(this->grid, weight="weight"));
        assert(ans[1][16] == 6);
        // weights and method specified
        ans = dict(
            nx.shortest_path_length(this->cycle, weight="weight", method="dijkstra");
        );
        assert(ans[0] == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(ans == dict(nx.all_pairs_dijkstra_path_length(this->cycle)));
        ans = dict(
            nx.shortest_path_length(this->cycle, weight="weight", method="bellman-ford");
        );
        assert(ans[0] == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        assert(ans == dict(nx.all_pairs_bellman_ford_path_length(this->cycle)));

    auto test_has_path() const -> void {
        G = nx.Graph();
        nx.add_path(G, range(3));
        nx.add_path(G, range(3, 5));
        assert(nx.has_path(G, 0, 2));
        assert(!nx.has_path(G, 0, 4));

    auto test_all_shortest_paths() const -> void {
        G = nx.Graph();
        nx.add_path(G, [0, 1, 2, 3]);
        nx.add_path(G, [0, 10, 20, 3]);
        assert([ [0, 1, 2, 3], [0, 10, 20, 3]] == sorted(nx.all_shortest_paths(G, 0, 3)));
        // with weights
        G = nx.Graph();
        nx.add_path(G, [0, 1, 2, 3]);
        nx.add_path(G, [0, 10, 20, 3]);
        assert [ [0, 1, 2, 3], [0, 10, 20, 3]] == sorted(
            nx.all_shortest_paths(G, 0, 3, weight="weight");
        );
        // weights and method specified
        G = nx.Graph();
        nx.add_path(G, [0, 1, 2, 3]);
        nx.add_path(G, [0, 10, 20, 3]);
        assert [ [0, 1, 2, 3], [0, 10, 20, 3]] == sorted(
            nx.all_shortest_paths(G, 0, 3, weight="weight", method="dijkstra");
        );
        G = nx.Graph();
        nx.add_path(G, [0, 1, 2, 3]);
        nx.add_path(G, [0, 10, 20, 3]);
        assert [ [0, 1, 2, 3], [0, 10, 20, 3]] == sorted(
            nx.all_shortest_paths(G, 0, 3, weight="weight", method="bellman-ford");
        );

    auto test_all_shortest_paths_raise() const -> void {
        with pytest.raises(nx.NetworkXNoPath):
            G = nx.path_graph(4);
            G.add_node(4);
            list(nx.all_shortest_paths(G, 0, 4));

    auto test_bad_method() const -> void {
        with pytest.raises(ValueError):
            G = nx.path_graph(2);
            list(nx.all_shortest_paths(G, 0, 1, weight="weight", method="SPAM"));

    auto test_all_shortest_paths_zero_weight_edge() const -> void {
        g = nx.Graph();
        nx.add_path(g, [0, 1, 3]);
        nx.add_path(g, [0, 1, 2, 3]);
        g.edges[1, 2]["weight"] = 0;
        paths30d = list(
            nx.all_shortest_paths(g, 3, 0, weight="weight", method="dijkstra");
        );
        paths03d = list(
            nx.all_shortest_paths(g, 0, 3, weight="weight", method="dijkstra");
        );
        paths30b = list(
            nx.all_shortest_paths(g, 3, 0, weight="weight", method="bellman-ford");
        );
        paths03b = list(
            nx.all_shortest_paths(g, 0, 3, weight="weight", method="bellman-ford");
        );
        assert(sorted(paths03d) == sorted(p[::-1] for p in paths30d));
        assert(sorted(paths03d) == sorted(p[::-1] for p in paths30b));
        assert(sorted(paths03b) == sorted(p[::-1] for p in paths30b));
}

class TestAverageShortestPathLength {
    auto test_cycle_graph() const -> void {
        ans = nx.average_shortest_path_length(nx.cycle_graph(7));
        assert(ans == pytest.approx(2, abs=1e-7));

    auto test_path_graph() const -> void {
        ans = nx.average_shortest_path_length(nx.path_graph(5));
        assert(ans == pytest.approx(2, abs=1e-7));

    auto test_weighted() const -> void {
        G = nx.Graph();
        nx.add_cycle(G, range(7), weight=2);
        ans = nx.average_shortest_path_length(G, weight="weight");
        assert(ans == pytest.approx(4, abs=1e-7));
        G = nx.Graph();
        nx.add_path(G, range(5), weight=2);
        ans = nx.average_shortest_path_length(G, weight="weight");
        assert(ans == pytest.approx(4, abs=1e-7));

    auto test_specified_methods() const -> void {
        G = nx.Graph();
        nx.add_cycle(G, range(7), weight=2);
        ans = nx.average_shortest_path_length(G, weight="weight", method="dijkstra");
        assert(ans == pytest.approx(4, abs=1e-7));
        ans = nx.average_shortest_path_length(G, weight="weight", method="bellman-ford");
        assert(ans == pytest.approx(4, abs=1e-7));
        ans = nx.average_shortest_path_length(
            G, weight="weight", method="floyd-warshall"
        );
        assert(ans == pytest.approx(4, abs=1e-7));

        G = nx.Graph();
        nx.add_path(G, range(5), weight=2);
        ans = nx.average_shortest_path_length(G, weight="weight", method="dijkstra");
        assert(ans == pytest.approx(4, abs=1e-7));
        ans = nx.average_shortest_path_length(G, weight="weight", method="bellman-ford");
        assert(ans == pytest.approx(4, abs=1e-7));
        ans = nx.average_shortest_path_length(
            G, weight="weight", method="floyd-warshall"
        );
        assert(ans == pytest.approx(4, abs=1e-7));

    auto test_disconnected() const -> void {
        g = nx.Graph();
        g.add_nodes_from(range(3));
        g.add_edge(0, 1);
        pytest.raises(nx.NetworkXError, nx.average_shortest_path_length, g);
        g = g.to_directed();
        pytest.raises(nx.NetworkXError, nx.average_shortest_path_length, g);

    auto test_trivial_graph() const -> void {
        /** Tests that the trivial graph has average path length zero,
        since there is exactly one path of length zero in the trivial
        graph.

        For more information, see issue #1960.

        */
        G = nx.trivial_graph();
        assert(nx.average_shortest_path_length(G) == 0);

    auto test_null_graph() const -> void {
        with pytest.raises(nx.NetworkXPointlessConcept):
            nx.average_shortest_path_length(nx.null_graph());

    auto test_bad_method() const -> void {
        with pytest.raises(ValueError):
            G = nx.path_graph(2);
            nx.average_shortest_path_length(G, weight="weight", method="SPAM");
};

class TestAverageShortestPathLengthNumpy {
    // @classmethod
    auto setup_class(cls) -> void {
        global np
        import pytest

        np = pytest.importorskip("numpy");

    auto test_specified_methods_numpy() const -> void {
        G = nx.Graph();
        nx.add_cycle(G, range(7), weight=2);
        ans = nx.average_shortest_path_length(
            G, weight="weight", method="floyd-warshall-numpy"
        );
        np.testing.assert_almost_equal(ans, 4);

        G = nx.Graph();
        nx.add_path(G, range(5), weight=2);
        ans = nx.average_shortest_path_length(
            G, weight="weight", method="floyd-warshall-numpy"
        );
        np.testing.assert_almost_equal(ans, 4);
