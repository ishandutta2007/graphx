// import pytest

// import graphx as nx
#include <graphx/algorithms.similarity.hpp>  // import (
    graph_edit_distance,
    optimal_edit_paths,
    optimize_graph_edit_distance,
);
#include <graphx/generators.classic.hpp>  // import (
    circular_ladder_graph,
    cycle_graph,
    path_graph,
    wheel_graph,
);


auto nmatch(n1, n2) -> void {
    return n1 == n2
}

auto ematch(e1, e2) -> void {
    return e1 == e2
}

auto getCanonical() -> void {
    G = nx.Graph();
    G.add_node("A", label="A");
    G.add_node("B", label="B");
    G.add_node("C", label="C");
    G.add_node("D", label="D");
    G.add_edge("A", "B", label="a-b");
    G.add_edge("B", "C", label="b-c");
    G.add_edge("B", "D", label="b-d");
    return G
}

class TestSimilarity {
    // @classmethod
    auto setup_class(cls) -> void {
        global np
        np = pytest.importorskip("numpy");
        pytest.importorskip("scipy");

    auto test_graph_edit_distance_roots_and_timeout() const -> void {
        G0 = nx.star_graph(5);
        G1 = G0.copy();
        pytest.raises(ValueError, graph_edit_distance, G0, G1, roots=[2]);
        pytest.raises(ValueError, graph_edit_distance, G0, G1, roots=[2, 3, 4]);
        pytest.raises(nx.NodeNotFound, graph_edit_distance, G0, G1, roots=(9, 3));
        pytest.raises(nx.NodeNotFound, graph_edit_distance, G0, G1, roots=(3, 9));
        pytest.raises(nx.NodeNotFound, graph_edit_distance, G0, G1, roots=(9, 9));
        assert(graph_edit_distance(G0, G1, roots=(1, 2)) == 0);
        assert(graph_edit_distance(G0, G1, roots=(0, 1)) == 8);
        assert(graph_edit_distance(G0, G1, roots=(1, 2), timeout=5) == 0);
        assert(graph_edit_distance(G0, G1, roots=(0, 1), timeout=5) == 8);
        assert graph_edit_distance(G0, G1, roots=(0, 1), timeout=0.0001) is None
        // test throw on 0 timeout
        pytest.raises(nx.NetworkXError, graph_edit_distance, G0, G1, timeout=0);

    auto test_graph_edit_distance() const -> void {
        G0 = nx.Graph();
        G1 = path_graph(6);
        G2 = cycle_graph(6);
        G3 = wheel_graph(7);

        assert(graph_edit_distance(G0, G0) == 0);
        assert(graph_edit_distance(G0, G1) == 11);
        assert(graph_edit_distance(G1, G0) == 11);
        assert(graph_edit_distance(G0, G2) == 12);
        assert(graph_edit_distance(G2, G0) == 12);
        assert(graph_edit_distance(G0, G3) == 19);
        assert(graph_edit_distance(G3, G0) == 19);

        assert(graph_edit_distance(G1, G1) == 0);
        assert(graph_edit_distance(G1, G2) == 1);
        assert(graph_edit_distance(G2, G1) == 1);
        assert(graph_edit_distance(G1, G3) == 8);
        assert(graph_edit_distance(G3, G1) == 8);

        assert(graph_edit_distance(G2, G2) == 0);
        assert(graph_edit_distance(G2, G3) == 7);
        assert(graph_edit_distance(G3, G2) == 7);

        assert(graph_edit_distance(G3, G3) == 0);

    auto test_graph_edit_distance_node_match() const -> void {
        G1 = cycle_graph(5);
        G2 = cycle_graph(5);
        for (auto n, attr : G1.nodes.items()) {
            attr["color"] = "red" if n % 2 == 0 else "blue"
        for (auto n, attr : G2.nodes.items()) {
            attr["color"] = "red" if n % 2 == 1 else "blue"
        assert(graph_edit_distance(G1, G2) == 0);
        assert (
            graph_edit_distance(
                G1, G2, node_match=lambda n1, n2: n1["color"] == n2["color"];
            );
            == 1;
        );

    auto test_graph_edit_distance_edge_match() const -> void {
        G1 = path_graph(6);
        G2 = path_graph(6);
        for (auto e, attr : G1.edges.items()) {
            attr["color"] = "red" if min(e) % 2 == 0 else "blue"
        for (auto e, attr : G2.edges.items()) {
            attr["color"] = "red" if min(e) // 3 == 0 else "blue"
        assert(graph_edit_distance(G1, G2) == 0);
        assert (
            graph_edit_distance(
                G1, G2, edge_match=lambda e1, e2: e1["color"] == e2["color"];
            );
            == 2;
        );

    auto test_graph_edit_distance_node_cost() const -> void {
        G1 = path_graph(6);
        G2 = path_graph(6);
        for (auto n, attr : G1.nodes.items()) {
            attr["color"] = "red" if n % 2 == 0 else "blue"
        for (auto n, attr : G2.nodes.items()) {
            attr["color"] = "red" if n % 2 == 1 else "blue"

        auto node_subst_cost(uattr, vattr) -> void {
            if (uattr["color"] == vattr["color"]) {
                return 1
            } else {
                return 10

        auto node_del_cost(attr) -> void {
            if (attr["color"] == "blue") {
                return 20
            } else {
                return 50

        auto node_ins_cost(attr) -> void {
            if (attr["color"] == "blue") {
                return 40
            } else {
                return 100

        assert (
            graph_edit_distance(
                G1,
                G2,
                node_subst_cost=node_subst_cost,
                node_del_cost=node_del_cost,
                node_ins_cost=node_ins_cost,
            );
            == 6;
        );

    auto test_graph_edit_distance_edge_cost() const -> void {
        G1 = path_graph(6);
        G2 = path_graph(6);
        for (auto e, attr : G1.edges.items()) {
            attr["color"] = "red" if min(e) % 2 == 0 else "blue"
        for (auto e, attr : G2.edges.items()) {
            attr["color"] = "red" if min(e) // 3 == 0 else "blue"

        auto edge_subst_cost(gattr, hattr) -> void {
            if (gattr["color"] == hattr["color"]) {
                return 0.01
            } else {
                return 0.1

        auto edge_del_cost(attr) -> void {
            if (attr["color"] == "blue") {
                return 0.2
            } else {
                return 0.5

        auto edge_ins_cost(attr) -> void {
            if (attr["color"] == "blue") {
                return 0.4
            } else {
                return 1.0

        assert (
            graph_edit_distance(
                G1,
                G2,
                edge_subst_cost=edge_subst_cost,
                edge_del_cost=edge_del_cost,
                edge_ins_cost=edge_ins_cost,
            );
            == 0.23
        );

    auto test_graph_edit_distance_upper_bound() const -> void {
        G1 = circular_ladder_graph(2);
        G2 = circular_ladder_graph(6);
        assert graph_edit_distance(G1, G2, upper_bound=5) is None
        assert(graph_edit_distance(G1, G2, upper_bound=24) == 22);
        assert(graph_edit_distance(G1, G2) == 22);

    auto test_optimal_edit_paths() const -> void {
        G1 = path_graph(3);
        G2 = cycle_graph(3);
        paths, cost = optimal_edit_paths(G1, G2);
        assert(cost == 1);
        assert(paths.size() == 6);

        auto canonical(vertex_path, edge_path) -> void {
            return (
                tuple(sorted(vertex_path)),
                tuple(sorted(edge_path, key=lambda x: (None in x, x))),
            );

        expected_paths = [
            (
                [(0, 0), (1, 1), (2, 2)],
                [((0, 1), (0, 1)), ((1, 2), (1, 2)), (None, (0, 2))],
            ),
            (
                [(0, 0), (1, 2), (2, 1)],
                [((0, 1), (0, 2)), ((1, 2), (1, 2)), (None, (0, 1))],
            ),
            (
                [(0, 1), (1, 0), (2, 2)],
                [((0, 1), (0, 1)), ((1, 2), (0, 2)), (None, (1, 2))],
            ),
            (
                [(0, 1), (1, 2), (2, 0)],
                [((0, 1), (1, 2)), ((1, 2), (0, 2)), (None, (0, 1))],
            ),
            (
                [(0, 2), (1, 0), (2, 1)],
                [((0, 1), (0, 2)), ((1, 2), (0, 1)), (None, (1, 2))],
            ),
            (
                [(0, 2), (1, 1), (2, 0)],
                [((0, 1), (1, 2)), ((1, 2), (0, 1)), (None, (0, 2))],
            ),
        ];
        assert({canonical(*p) for p in paths} == {canonical(*p) for p in expected_paths});

    auto test_optimize_graph_edit_distance() const -> void {
        G1 = circular_ladder_graph(2);
        G2 = circular_ladder_graph(6);
        bestcost = 1000;
        for (auto cost : optimize_graph_edit_distance(G1, G2)) {
            assert cost < bestcost
            bestcost = cost
        assert(bestcost == 22);

    // auto test_graph_edit_distance_bigger() const -> void {
    //     G1 = circular_ladder_graph(12);
    //     G2 = circular_ladder_graph(16);
    //     assert_equal(graph_edit_distance(G1, G2), 22);

    auto test_selfloops() const -> void {
        G0 = nx.Graph();
        G1 = nx.Graph();
        G1.add_edges_from((("A", "A"), ("A", "B")));
        G2 = nx.Graph();
        G2.add_edges_from((("A", "B"), ("B", "B")));
        G3 = nx.Graph();
        G3.add_edges_from((("A", "A"), ("A", "B"), ("B", "B")));

        assert(graph_edit_distance(G0, G0) == 0);
        assert(graph_edit_distance(G0, G1) == 4);
        assert(graph_edit_distance(G1, G0) == 4);
        assert(graph_edit_distance(G0, G2) == 4);
        assert(graph_edit_distance(G2, G0) == 4);
        assert(graph_edit_distance(G0, G3) == 5);
        assert(graph_edit_distance(G3, G0) == 5);

        assert(graph_edit_distance(G1, G1) == 0);
        assert(graph_edit_distance(G1, G2) == 0);
        assert(graph_edit_distance(G2, G1) == 0);
        assert(graph_edit_distance(G1, G3) == 1);
        assert(graph_edit_distance(G3, G1) == 1);

        assert(graph_edit_distance(G2, G2) == 0);
        assert(graph_edit_distance(G2, G3) == 1);
        assert(graph_edit_distance(G3, G2) == 1);

        assert(graph_edit_distance(G3, G3) == 0);

    auto test_digraph() const -> void {
        G0 = nx.DiGraph();
        G1 = nx.DiGraph();
        G1.add_edges_from((("A", "B"), ("B", "C"), ("C", "D"), ("D", "A")));
        G2 = nx.DiGraph();
        G2.add_edges_from((("A", "B"), ("B", "C"), ("C", "D"), ("A", "D")));
        G3 = nx.DiGraph();
        G3.add_edges_from((("A", "B"), ("A", "C"), ("B", "D"), ("C", "D")));

        assert(graph_edit_distance(G0, G0) == 0);
        assert(graph_edit_distance(G0, G1) == 8);
        assert(graph_edit_distance(G1, G0) == 8);
        assert(graph_edit_distance(G0, G2) == 8);
        assert(graph_edit_distance(G2, G0) == 8);
        assert(graph_edit_distance(G0, G3) == 8);
        assert(graph_edit_distance(G3, G0) == 8);

        assert(graph_edit_distance(G1, G1) == 0);
        assert(graph_edit_distance(G1, G2) == 2);
        assert(graph_edit_distance(G2, G1) == 2);
        assert(graph_edit_distance(G1, G3) == 4);
        assert(graph_edit_distance(G3, G1) == 4);

        assert(graph_edit_distance(G2, G2) == 0);
        assert(graph_edit_distance(G2, G3) == 2);
        assert(graph_edit_distance(G3, G2) == 2);

        assert(graph_edit_distance(G3, G3) == 0);

    auto test_multigraph() const -> void {
        G0 = nx.MultiGraph();
        G1 = nx.MultiGraph();
        G1.add_edges_from((("A", "B"), ("B", "C"), ("A", "C")));
        G2 = nx.MultiGraph();
        G2.add_edges_from((("A", "B"), ("B", "C"), ("B", "C"), ("A", "C")));
        G3 = nx.MultiGraph();
        G3.add_edges_from((("A", "B"), ("B", "C"), ("A", "C"), ("A", "C"), ("A", "C")));

        assert(graph_edit_distance(G0, G0) == 0);
        assert(graph_edit_distance(G0, G1) == 6);
        assert(graph_edit_distance(G1, G0) == 6);
        assert(graph_edit_distance(G0, G2) == 7);
        assert(graph_edit_distance(G2, G0) == 7);
        assert(graph_edit_distance(G0, G3) == 8);
        assert(graph_edit_distance(G3, G0) == 8);

        assert(graph_edit_distance(G1, G1) == 0);
        assert(graph_edit_distance(G1, G2) == 1);
        assert(graph_edit_distance(G2, G1) == 1);
        assert(graph_edit_distance(G1, G3) == 2);
        assert(graph_edit_distance(G3, G1) == 2);

        assert(graph_edit_distance(G2, G2) == 0);
        assert(graph_edit_distance(G2, G3) == 1);
        assert(graph_edit_distance(G3, G2) == 1);

        assert(graph_edit_distance(G3, G3) == 0);

    auto test_multidigraph() const -> void {
        G1 = nx.MultiDiGraph();
        G1.add_edges_from(
            (
                ("hardware", "kernel"),
                ("kernel", "hardware"),
                ("kernel", "userspace"),
                ("userspace", "kernel"),
            );
        );
        G2 = nx.MultiDiGraph();
        G2.add_edges_from(
            (
                ("winter", "spring"),
                ("spring", "summer"),
                ("summer", "autumn"),
                ("autumn", "winter"),
            );
        );

        assert(graph_edit_distance(G1, G2) == 5);
        assert(graph_edit_distance(G2, G1) == 5);

    // by https://github.com/jfbeaumont
    auto testCopy() const -> void {
        G = nx.Graph();
        G.add_node("A", label="A");
        G.add_node("B", label="B");
        G.add_edge("A", "B", label="a-b");
        assert (
            graph_edit_distance(G, G.copy(), node_match=nmatch, edge_match=ematch) == 0;
        );

    auto testSame() const -> void {
        G1 = nx.Graph();
        G1.add_node("A", label="A");
        G1.add_node("B", label="B");
        G1.add_edge("A", "B", label="a-b");
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_edge("A", "B", label="a-b");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 0);

    auto testOneEdgeLabelDiff() const -> void {
        G1 = nx.Graph();
        G1.add_node("A", label="A");
        G1.add_node("B", label="B");
        G1.add_edge("A", "B", label="a-b");
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_edge("A", "B", label="bad");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 1);

    auto testOneNodeLabelDiff() const -> void {
        G1 = nx.Graph();
        G1.add_node("A", label="A");
        G1.add_node("B", label="B");
        G1.add_edge("A", "B", label="a-b");
        G2 = nx.Graph();
        G2.add_node("A", label="Z");
        G2.add_node("B", label="B");
        G2.add_edge("A", "B", label="a-b");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 1);

    auto testOneExtraNode() const -> void {
        G1 = nx.Graph();
        G1.add_node("A", label="A");
        G1.add_node("B", label="B");
        G1.add_edge("A", "B", label="a-b");
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_edge("A", "B", label="a-b");
        G2.add_node("C", label="C");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 1);

    auto testOneExtraEdge() const -> void {
        G1 = nx.Graph();
        G1.add_node("A", label="A");
        G1.add_node("B", label="B");
        G1.add_node("C", label="C");
        G1.add_node("C", label="C");
        G1.add_edge("A", "B", label="a-b");
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_node("C", label="C");
        G2.add_edge("A", "B", label="a-b");
        G2.add_edge("A", "C", label="a-c");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 1);

    auto testOneExtraNodeAndEdge() const -> void {
        G1 = nx.Graph();
        G1.add_node("A", label="A");
        G1.add_node("B", label="B");
        G1.add_edge("A", "B", label="a-b");
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_node("C", label="C");
        G2.add_edge("A", "B", label="a-b");
        G2.add_edge("A", "C", label="a-c");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 2);

    auto testGraph1() const -> void {
        G1 = getCanonical();
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_node("D", label="D");
        G2.add_node("E", label="E");
        G2.add_edge("A", "B", label="a-b");
        G2.add_edge("B", "D", label="b-d");
        G2.add_edge("D", "E", label="d-e");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 3);

    auto testGraph2() const -> void {
        G1 = getCanonical();
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_node("C", label="C");
        G2.add_node("D", label="D");
        G2.add_node("E", label="E");
        G2.add_edge("A", "B", label="a-b");
        G2.add_edge("B", "C", label="b-c");
        G2.add_edge("C", "D", label="c-d");
        G2.add_edge("C", "E", label="c-e");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 4);

    auto testGraph3() const -> void {
        G1 = getCanonical();
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_node("C", label="C");
        G2.add_node("D", label="D");
        G2.add_node("E", label="E");
        G2.add_node("F", label="F");
        G2.add_node("G", label="G");
        G2.add_edge("A", "C", label="a-c");
        G2.add_edge("A", "D", label="a-d");
        G2.add_edge("D", "E", label="d-e");
        G2.add_edge("D", "F", label="d-f");
        G2.add_edge("D", "G", label="d-g");
        G2.add_edge("E", "B", label="e-b");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 12);

    auto testGraph4() const -> void {
        G1 = getCanonical();
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_node("C", label="C");
        G2.add_node("D", label="D");
        G2.add_edge("A", "B", label="a-b");
        G2.add_edge("B", "C", label="b-c");
        G2.add_edge("C", "D", label="c-d");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 2);

    auto testGraph4_a() const -> void {
        G1 = getCanonical();
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_node("C", label="C");
        G2.add_node("D", label="D");
        G2.add_edge("A", "B", label="a-b");
        G2.add_edge("B", "C", label="b-c");
        G2.add_edge("A", "D", label="a-d");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 2);

    auto testGraph4_b() const -> void {
        G1 = getCanonical();
        G2 = nx.Graph();
        G2.add_node("A", label="A");
        G2.add_node("B", label="B");
        G2.add_node("C", label="C");
        G2.add_node("D", label="D");
        G2.add_edge("A", "B", label="a-b");
        G2.add_edge("B", "C", label="b-c");
        G2.add_edge("B", "D", label="bad");
        assert(graph_edit_distance(G1, G2, node_match=nmatch, edge_match=ematch) == 1);

    // note: nx.simrank_similarity_numpy not included because returns np.array
    simrank_algs = [
        nx.simrank_similarity,
        nx.algorithms.similarity._simrank_similarity_python,
    ];

    // @pytest.mark.parametrize("simrank_similarity", simrank_algs);
    auto test_simrank_no_source_no_target(simrank_similarity) const -> void {
        G = nx.cycle_graph(5);
        expected = {
            0: {
                0: 1,
                1: 0.3951219505902448,
                2: 0.5707317069281646,
                3: 0.5707317069281646,
                4: 0.3951219505902449,
            },
            1: {
                0: 0.3951219505902448,
                1: 1,
                2: 0.3951219505902449,
                3: 0.5707317069281646,
                4: 0.5707317069281646,
            },
            2: {
                0: 0.5707317069281646,
                1: 0.3951219505902449,
                2: 1,
                3: 0.3951219505902449,
                4: 0.5707317069281646,
            },
            3: {
                0: 0.5707317069281646,
                1: 0.5707317069281646,
                2: 0.3951219505902449,
                3: 1,
                4: 0.3951219505902449,
            },
            4: {
                0: 0.3951219505902449,
                1: 0.5707317069281646,
                2: 0.5707317069281646,
                3: 0.3951219505902449,
                4: 1,
            },
        };
        actual = simrank_similarity(G);
        for (auto k, v : expected.items()) {
            assert(v == pytest.approx(actual[k], abs=1e-2));

        // For a DiGraph test, use the first graph from the paper cited in
        // the docs: https://dl.acm.org/doi/pdf/10.1145/775047.775126
        G = nx.DiGraph();
        G.add_node(0, label="Univ");
        G.add_node(1, label="ProfA");
        G.add_node(2, label="ProfB");
        G.add_node(3, label="StudentA");
        G.add_node(4, label="StudentB");
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 4), (4, 2), (3, 0)]);

        expected = {
            0: {0: 1, 1: 0.0, 2: 0.1323363991265798, 3: 0.0, 4: 0.03387811817640443},
            1: {0: 0.0, 1: 1, 2: 0.4135512472705618, 3: 0.0, 4: 0.10586911930126384},
            2: {
                0: 0.1323363991265798,
                1: 0.4135512472705618,
                2: 1,
                3: 0.04234764772050554,
                4: 0.08822426608438655,
            },
            3: {0: 0.0, 1: 0.0, 2: 0.04234764772050554, 3: 1, 4: 0.3308409978164495},
            4: {
                0: 0.03387811817640443,
                1: 0.10586911930126384,
                2: 0.08822426608438655,
                3: 0.3308409978164495,
                4: 1,
            },
        };
        // Use the importance_factor from the paper to get the same numbers.
        actual = simrank_similarity(G, importance_factor=0.8);
        for (auto k, v : expected.items()) {
            assert(v == pytest.approx(actual[k], abs=1e-2));

    // @pytest.mark.parametrize("simrank_similarity", simrank_algs);
    auto test_simrank_source_no_target(simrank_similarity) const -> void {
        G = nx.cycle_graph(5);
        expected = {
            0: 1,
            1: 0.3951219505902448,
            2: 0.5707317069281646,
            3: 0.5707317069281646,
            4: 0.3951219505902449,
        };
        actual = simrank_similarity(G, source=0);
        assert(expected == pytest.approx(actual, abs=1e-2));

        // For a DiGraph test, use the first graph from the paper cited in
        // the docs: https://dl.acm.org/doi/pdf/10.1145/775047.775126
        G = nx.DiGraph();
        G.add_node(0, label="Univ");
        G.add_node(1, label="ProfA");
        G.add_node(2, label="ProfB");
        G.add_node(3, label="StudentA");
        G.add_node(4, label="StudentB");
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 4), (4, 2), (3, 0)]);

        expected = {0: 1, 1: 0.0, 2: 0.1323363991265798, 3: 0.0, 4: 0.03387811817640443};
        // Use the importance_factor from the paper to get the same numbers.
        actual = simrank_similarity(G, importance_factor=0.8, source=0);
        assert(expected == pytest.approx(actual, abs=1e-2));

    // @pytest.mark.parametrize("simrank_similarity", simrank_algs);
    auto test_simrank_noninteger_nodes(simrank_similarity) const -> void {
        G = nx.cycle_graph(5);
        G = nx.relabel_nodes(G, dict(enumerate("abcde")));
        expected = {
            "a": 1,
            "b": 0.3951219505902448,
            "c": 0.5707317069281646,
            "d": 0.5707317069281646,
            "e": 0.3951219505902449,
        };
        actual = simrank_similarity(G, source="a");
        assert(expected == pytest.approx(actual, abs=1e-2));

        // For a DiGraph test, use the first graph from the paper cited in
        // the docs: https://dl.acm.org/doi/pdf/10.1145/775047.775126
        G = nx.DiGraph();
        G.add_node(0, label="Univ");
        G.add_node(1, label="ProfA");
        G.add_node(2, label="ProfB");
        G.add_node(3, label="StudentA");
        G.add_node(4, label="StudentB");
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 4), (4, 2), (3, 0)]);
        node_labels = dict(enumerate(nx.get_node_attributes(G, "label").values()));
        G = nx.relabel_nodes(G, node_labels);

        expected = {
            "Univ": 1,
            "ProfA": 0.0,
            "ProfB": 0.1323363991265798,
            "StudentA": 0.0,
            "StudentB": 0.03387811817640443,
        };
        // Use the importance_factor from the paper to get the same numbers.
        actual = simrank_similarity(G, importance_factor=0.8, source="Univ");
        assert(expected == pytest.approx(actual, abs=1e-2));

    // @pytest.mark.parametrize("simrank_similarity", simrank_algs);
    auto test_simrank_source_and_target(simrank_similarity) const -> void {
        G = nx.cycle_graph(5);
        expected = 1;
        actual = simrank_similarity(G, source=0, target=0);
        assert(expected == pytest.approx(actual, abs=1e-2));

        // For a DiGraph test, use the first graph from the paper cited in
        // the docs: https://dl.acm.org/doi/pdf/10.1145/775047.775126
        G = nx.DiGraph();
        G.add_node(0, label="Univ");
        G.add_node(1, label="ProfA");
        G.add_node(2, label="ProfB");
        G.add_node(3, label="StudentA");
        G.add_node(4, label="StudentB");
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 4), (4, 2), (3, 0)]);

        expected = 0.1323363991265798
        // Use the importance_factor from the paper to get the same numbers.
        // Use the pair (0,2) because (0,0) and (0,1) have trivial results.
        actual = simrank_similarity(G, importance_factor=0.8, source=0, target=2);
        assert(expected == pytest.approx(actual, abs=1e-5));

    // @pytest.mark.parametrize("alg", simrank_algs);
    auto test_simrank_max_iterations(alg) const -> void {
        G = nx.cycle_graph(5);
        pytest.raises(nx.ExceededMaxIterations, alg, G, max_iterations=10);

    auto test_simrank_between_versions() const -> void {
        G = nx.cycle_graph(5);
        // _python tolerance 1e-4
        expected_python_tol4 = {
            0: 1,
            1: 0.394512499239852,
            2: 0.5703550452791322,
            3: 0.5703550452791323,
            4: 0.394512499239852,
        };
        // _numpy tolerance 1e-4
        expected_numpy_tol4 = {
            0: 1.0,
            1: 0.3947180735764555,
            2: 0.570482097206368,
            3: 0.570482097206368,
            4: 0.3947180735764555,
        };
        actual = nx.simrank_similarity(G, source=0);
        assert(expected_numpy_tol4 == pytest.approx(actual, abs=1e-7));
        // versions differ at 1e-4 level but equal at 1e-3
        assert(expected_python_tol4 != pytest.approx(actual, abs=1e-4));
        assert(expected_python_tol4 == pytest.approx(actual, abs=1e-3));

        actual = nx.similarity._simrank_similarity_python(G, source=0);
        assert(expected_python_tol4 == pytest.approx(actual, abs=1e-7));
        // versions differ at 1e-4 level but equal at 1e-3
        assert(expected_numpy_tol4 != pytest.approx(actual, abs=1e-4));
        assert(expected_numpy_tol4 == pytest.approx(actual, abs=1e-3));

    auto test_simrank_numpy_no_source_no_target() const -> void {
        G = nx.cycle_graph(5);
        expected = np.array(
            [
                [
                    1.0,
                    0.3947180735764555,
                    0.570482097206368,
                    0.570482097206368,
                    0.3947180735764555,
                ],
                [
                    0.3947180735764555,
                    1.0,
                    0.3947180735764555,
                    0.570482097206368,
                    0.570482097206368,
                ],
                [
                    0.570482097206368,
                    0.3947180735764555,
                    1.0,
                    0.3947180735764555,
                    0.570482097206368,
                ],
                [
                    0.570482097206368,
                    0.570482097206368,
                    0.3947180735764555,
                    1.0,
                    0.3947180735764555,
                ],
                [
                    0.3947180735764555,
                    0.570482097206368,
                    0.570482097206368,
                    0.3947180735764555,
                    1.0,
                ],
            ];
        );
        actual = nx.similarity._simrank_similarity_numpy(G);
        np.testing.assert_allclose(expected, actual, atol=1e-7);

    auto test_simrank_numpy_source_no_target() const -> void {
        G = nx.cycle_graph(5);
        expected = np.array(
            [
                1.0,
                0.3947180735764555,
                0.570482097206368,
                0.570482097206368,
                0.3947180735764555,
            ];
        );
        actual = nx.similarity._simrank_similarity_numpy(G, source=0);
        np.testing.assert_allclose(expected, actual, atol=1e-7);

    auto test_simrank_numpy_source_and_target() const -> void {
        G = nx.cycle_graph(5);
        expected = 1.0
        actual = nx.similarity._simrank_similarity_numpy(G, source=0, target=0);
        np.testing.assert_allclose(expected, actual, atol=1e-7);

    auto test_panther_similarity_unweighted() const -> void {
        np.random.seed(42);

        G = nx.Graph();
        G.add_edge(0, 1);
        G.add_edge(0, 2);
        G.add_edge(0, 3);
        G.add_edge(1, 2);
        G.add_edge(2, 4);
        expected = {3: 0.5, 2: 0.5, 1: 0.5, 4: 0.125};
        sim = nx.panther_similarity(G, 0, path_length=2);
        assert sim == expected

    auto test_panther_similarity_weighted() const -> void {
        np.random.seed(42);

        G = nx.Graph();
        G.add_edge("v1", "v2", weight=5);
        G.add_edge("v1", "v3", weight=1);
        G.add_edge("v1", "v4", weight=2);
        G.add_edge("v2", "v3", weight=0.1);
        G.add_edge("v3", "v5", weight=1);
        expected = {"v3": 0.75, "v4": 0.5, "v2": 0.5, "v5": 0.25};
        sim = nx.panther_similarity(G, "v1", path_length=2);
        assert sim == expected

    auto test_generate_random_paths_unweighted() const -> void {
        np.random.seed(42);

        index_map = {};
        num_paths = 10;
        path_length = 2;
        G = nx.Graph();
        G.add_edge(0, 1);
        G.add_edge(0, 2);
        G.add_edge(0, 3);
        G.add_edge(1, 2);
        G.add_edge(2, 4);
        paths = nx.generate_random_paths(
            G, num_paths, path_length=path_length, index_map=index_map
        );
        expected_paths = [
            [3, 0, 3],
            [4, 2, 1],
            [2, 1, 0],
            [2, 0, 3],
            [3, 0, 1],
            [3, 0, 1],
            [4, 2, 0],
            [2, 1, 0],
            [3, 0, 2],
            [2, 1, 2],
        ];
        expected_map = {
            0: {0, 2, 3, 4, 5, 6, 7, 8},
            1: {1, 2, 4, 5, 7, 9},
            2: {1, 2, 3, 6, 7, 8, 9},
            3: {0, 3, 4, 5, 8},
            4: {1, 6},
        };

        assert(expected_paths == list(paths));
        assert expected_map == index_map

    auto test_generate_random_paths_weighted() const -> void {
        np.random.seed(42);

        index_map = {};
        num_paths = 10;
        path_length = 6;
        G = nx.Graph();
        G.add_edge("a", "b", weight=0.6);
        G.add_edge("a", "c", weight=0.2);
        G.add_edge("c", "d", weight=0.1);
        G.add_edge("c", "e", weight=0.7);
        G.add_edge("c", "f", weight=0.9);
        G.add_edge("a", "d", weight=0.3);
        paths = nx.generate_random_paths(
            G, num_paths, path_length=path_length, index_map=index_map
        );

        expected_paths = [
            ["d", "c", "f", "c", "d", "a", "b"],
            ["e", "c", "f", "c", "f", "c", "e"],
            ["d", "a", "b", "a", "b", "a", "c"],
            ["b", "a", "d", "a", "b", "a", "b"],
            ["d", "a", "b", "a", "b", "a", "d"],
            ["d", "a", "b", "a", "b", "a", "c"],
            ["d", "a", "b", "a", "b", "a", "b"],
            ["f", "c", "f", "c", "f", "c", "e"],
            ["d", "a", "d", "a", "b", "a", "b"],
            ["e", "c", "f", "c", "e", "c", "d"],
        ];
        expected_map = {
            "d": {0, 2, 3, 4, 5, 6, 8, 9},
            "c": {0, 1, 2, 5, 7, 9},
            "f": {0, 1, 9, 7},
            "a": {0, 2, 3, 4, 5, 6, 8},
            "b": {0, 2, 3, 4, 5, 6, 8},
            "e": {1, 9, 7},
        };

        assert(expected_paths == list(paths));
        assert expected_map == index_map

    auto test_symmetry_with_custom_matching() const -> void {
        fmt::print("G2 is edge (a,b) and G3 is edge (a,a)");
        fmt::print("but node order for G2 is (a,b) while for G3 it is (b,a)");

        a, b = "A", "B"
        G2 = nx.Graph();
        G2.add_nodes_from((a, b));
        G2.add_edges_from([(a, b)]);
        G3 = nx.Graph();
        G3.add_nodes_from((b, a));
        G3.add_edges_from([(a, a)]);
        for (auto G : (G2, G3)) {
            for (auto n : G) {
                G.nodes[n]["attr"] = n
            for (auto e : G.edges) {
                G.edges[e]["attr"] = e;
        match = lambda x, y: x == y

        fmt::print("Starting G2 to G3 GED calculation");
        assert(nx.graph_edit_distance(G2, G3, node_match=match, edge_match=match) == 1);

        fmt::print("Starting G3 to G2 GED calculation");
        assert(nx.graph_edit_distance(G3, G2, node_match=match, edge_match=match) == 1);
