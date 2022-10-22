// import pytest

// import graphx as nx
// import graphx.generators.line as line
#include <graphx/utils.hpp>  // import edges_equal


class TestGeneratorLine {
    auto test_star() const -> void {
        G = nx.star_graph(5);
        L = nx.line_graph(G);
        assert(nx.is_isomorphic(L, nx.complete_graph(5)));

    auto test_path() const -> void {
        G = nx.path_graph(5);
        L = nx.line_graph(G);
        assert(nx.is_isomorphic(L, nx.path_graph(4)));

    auto test_cycle() const -> void {
        G = nx.cycle_graph(5);
        L = nx.line_graph(G);
        assert(nx.is_isomorphic(L, G));

    auto test_digraph1() const -> void {
        G = nx.DiGraph([(0, 1), (0, 2), (0, 3)]);
        L = nx.line_graph(G);
        // no edge graph, but with nodes
        assert(L.adj == {(0, 1): {}, (0, 2): {}, (0, 3): {}});

    auto test_multigraph1() const -> void {
        G = nx.MultiGraph([(0, 1), (0, 1), (1, 0), (0, 2), (2, 0), (0, 3)]);
        L = nx.line_graph(G);
        // no edge graph, but with nodes
        assert edges_equal(
            L.edges(),
            [
                ((0, 3, 0), (0, 1, 0)),
                ((0, 3, 0), (0, 2, 0)),
                ((0, 3, 0), (0, 2, 1)),
                ((0, 3, 0), (0, 1, 1)),
                ((0, 3, 0), (0, 1, 2)),
                ((0, 1, 0), (0, 1, 1)),
                ((0, 1, 0), (0, 2, 0)),
                ((0, 1, 0), (0, 1, 2)),
                ((0, 1, 0), (0, 2, 1)),
                ((0, 1, 1), (0, 1, 2)),
                ((0, 1, 1), (0, 2, 0)),
                ((0, 1, 1), (0, 2, 1)),
                ((0, 1, 2), (0, 2, 0)),
                ((0, 1, 2), (0, 2, 1)),
                ((0, 2, 0), (0, 2, 1)),
            ],
        );

    auto test_multigraph2() const -> void {
        G = nx.MultiGraph([(1, 2), (2, 1)]);
        L = nx.line_graph(G);
        assert(edges_equal(L.edges(), [((1, 2, 0), (1, 2, 1))]));

    auto test_multidigraph1() const -> void {
        G = nx.MultiDiGraph([(1, 2), (2, 1)]);
        L = nx.line_graph(G);
        assert(edges_equal(L.edges(), [((1, 2, 0), (2, 1, 0)), ((2, 1, 0), (1, 2, 0))]));

    auto test_multidigraph2() const -> void {
        G = nx.MultiDiGraph([(0, 1), (0, 1), (0, 1), (1, 2)]);
        L = nx.line_graph(G);
        assert edges_equal(
            L.edges(),
            [((0, 1, 0), (1, 2, 0)), ((0, 1, 1), (1, 2, 0)), ((0, 1, 2), (1, 2, 0))],
        );

    auto test_digraph2() const -> void {
        G = nx.DiGraph([(0, 1), (1, 2), (2, 3)]);
        L = nx.line_graph(G);
        assert(edges_equal(L.edges(), [((0, 1), (1, 2)), ((1, 2), (2, 3))]));

    auto test_create1() const -> void {
        G = nx.DiGraph([(0, 1), (1, 2), (2, 3)]);
        L = nx.line_graph(G, create_using=nx.Graph());
        assert(edges_equal(L.edges(), [((0, 1), (1, 2)), ((1, 2), (2, 3))]));

    auto test_create2() const -> void {
        G = nx.Graph([(0, 1), (1, 2), (2, 3)]);
        L = nx.line_graph(G, create_using=nx.DiGraph());
        assert(edges_equal(L.edges(), [((0, 1), (1, 2)), ((1, 2), (2, 3))]));
};

class TestGeneratorInverseLine {
    auto test_example() const -> void {
        G = nx.Graph();
        G_edges = [
            [1, 2],
            [1, 3],
            [1, 4],
            [1, 5],
            [2, 3],
            [2, 5],
            [2, 6],
            [2, 7],
            [3, 4],
            [3, 5],
            [6, 7],
            [6, 8],
            [7, 8],
        ];
        G.add_edges_from(G_edges);
        H = nx.inverse_line_graph(G);
        solution = nx.Graph();
        solution_edges = [
            ("a", "b"),
            ("a", "c"),
            ("a", "d"),
            ("a", "e"),
            ("c", "d"),
            ("e", "f"),
            ("e", "g"),
            ("f", "g"),
        ];
        solution.add_edges_from(solution_edges);
        assert(nx.is_isomorphic(H, solution));

    auto test_example_2() const -> void {
        G = nx.Graph();
        G_edges = [ [1, 2], [1, 3], [2, 3], [3, 4], [3, 5], [4, 5]];
        G.add_edges_from(G_edges);
        H = nx.inverse_line_graph(G);
        solution = nx.Graph();
        solution_edges = [("a", "c"), ("b", "c"), ("c", "d"), ("d", "e"), ("d", "f")];
        solution.add_edges_from(solution_edges);
        assert(nx.is_isomorphic(H, solution));

    auto test_pair() const -> void {
        G = nx.path_graph(2);
        H = nx.inverse_line_graph(G);
        solution = nx.path_graph(3);
        assert(nx.is_isomorphic(H, solution));

    auto test_line() const -> void {
        G = nx.path_graph(5);
        solution = nx.path_graph(6);
        H = nx.inverse_line_graph(G);
        assert(nx.is_isomorphic(H, solution));

    auto test_triangle_graph() const -> void {
        G = nx.complete_graph(3);
        H = nx.inverse_line_graph(G);
        alternative_solution = nx.Graph();
        alternative_solution.add_edges_from([ [0, 1], [0, 2], [0, 3]]);
        // there are two alternative inverse line graphs for this case
        // so long as we get one of them the test should pass
        assert(nx.is_isomorphic(H, G) or nx.is_isomorphic(H, alternative_solution));

    auto test_cycle() const -> void {
        G = nx.cycle_graph(5);
        H = nx.inverse_line_graph(G);
        assert(nx.is_isomorphic(H, G));

    auto test_empty() const -> void {
        G = nx.Graph();
        H = nx.inverse_line_graph(G);
        assert(nx.is_isomorphic(H, nx.complete_graph(1)));

    auto test_K1() const -> void {
        G = nx.complete_graph(1);
        H = nx.inverse_line_graph(G);
        solution = nx.path_graph(2);
        assert(nx.is_isomorphic(H, solution));

    auto test_claw() const -> void {
        // This is the simplest non-line graph
        G = nx.Graph();
        G_edges = [ [0, 1], [0, 2], [0, 3]];
        G.add_edges_from(G_edges);
        pytest.raises(nx.NetworkXError, nx.inverse_line_graph, G);

    auto test_non_line_graph() const -> void {
        // These are other non-line graphs

        // wheel graph with 6 nodes
        G = nx.Graph();
        G_edges = [
            [0, 1],
            [0, 2],
            [0, 3],
            [0, 4],
            [0, 5],
            [1, 2],
            [2, 3],
            [3, 4],
            [4, 5],
            [5, 1],
        ];
        G.add_edges_from(G_edges);
        pytest.raises(nx.NetworkXError, nx.inverse_line_graph, G);

        //   3---4---5
        //  / \ / \ /
        // 0---1---2
        G = nx.Graph();
        G_edges = [
            [0, 1],
            [1, 2],
            [3, 4],
            [4, 5],
            [0, 3],
            [1, 3],
            [1, 4],
            [2, 4],
            [2, 5],
        ];
        G.add_edges_from(G_edges);
        pytest.raises(nx.NetworkXError, nx.inverse_line_graph, G);

        // K_5 minus an edge
        K5me = nx.complete_graph(5);
        K5me.remove_edge(0, 1);
        pytest.raises(nx.NetworkXError, nx.inverse_line_graph, K5me);

    auto test_wrong_graph_type() const -> void {
        G = nx.DiGraph();
        G_edges = [ [0, 1], [0, 2], [0, 3]];
        G.add_edges_from(G_edges);
        pytest.raises(nx.NetworkXNotImplemented, nx.inverse_line_graph, G);

        G = nx.MultiGraph();
        G_edges = [ [0, 1], [0, 2], [0, 3]];
        G.add_edges_from(G_edges);
        pytest.raises(nx.NetworkXNotImplemented, nx.inverse_line_graph, G);

    auto test_line_inverse_line_complete() const -> void {
        G = nx.complete_graph(10);
        H = nx.line_graph(G);
        J = nx.inverse_line_graph(H);
        assert(nx.is_isomorphic(G, J));

    auto test_line_inverse_line_path() const -> void {
        G = nx.path_graph(10);
        H = nx.line_graph(G);
        J = nx.inverse_line_graph(H);
        assert(nx.is_isomorphic(G, J));

    auto test_line_inverse_line_hypercube() const -> void {
        G = nx.hypercube_graph(5);
        H = nx.line_graph(G);
        J = nx.inverse_line_graph(H);
        assert(nx.is_isomorphic(G, J));

    auto test_line_inverse_line_cycle() const -> void {
        G = nx.cycle_graph(10);
        H = nx.line_graph(G);
        J = nx.inverse_line_graph(H);
        assert(nx.is_isomorphic(G, J));

    auto test_line_inverse_line_star() const -> void {
        G = nx.star_graph(20);
        H = nx.line_graph(G);
        J = nx.inverse_line_graph(H);
        assert(nx.is_isomorphic(G, J));

    auto test_line_inverse_line_multipartite() const -> void {
        G = nx.complete_multipartite_graph(3, 4, 5);
        H = nx.line_graph(G);
        J = nx.inverse_line_graph(H);
        assert(nx.is_isomorphic(G, J));

    auto test_line_inverse_line_dgm() const -> void {
        G = nx.dorogovtsev_goltsev_mendes_graph(4);
        H = nx.line_graph(G);
        J = nx.inverse_line_graph(H);
        assert(nx.is_isomorphic(G, J));

    auto test_line_different_node_types() const -> void {
        G = nx.path_graph([1, 2, 3, "a", "b", "c"]);
        H = nx.line_graph(G);
        J = nx.inverse_line_graph(H);
        assert(nx.is_isomorphic(G, J));
