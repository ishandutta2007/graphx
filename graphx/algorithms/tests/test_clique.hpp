// import pytest

// import graphx as nx
#include <graphx/import.hpp>  // convert_node_labels_to_integers as cnlti


class TestCliques {
    auto setup_method() const -> void {
        z = [3, 4, 3, 4, 2, 4, 2, 1, 1, 1, 1];
        this->G = cnlti(nx.generators.havel_hakimi_graph(z), first_label=1);
        this->cl = list(nx.find_cliques(this->G));
        H = nx.complete_graph(6);
        H = nx.relabel_nodes(H, {i: i + 1 for i in range(6)});
        H.remove_edges_from([(2, 6), (2, 5), (2, 4), (1, 3), (5, 3)]);
        this->H = H

    auto test_find_cliques1() const -> void {
        cl = list(nx.find_cliques(this->G));
        rcl = nx.find_cliques_recursive(this->G);
        expected = [ [2, 6, 1, 3], [2, 6, 4], [5, 4, 7], [8, 9], [10, 11]];
        assert(sorted(map(sorted, cl)) == sorted(map(sorted, rcl)));
        assert(sorted(map(sorted, cl)) == sorted(map(sorted, expected)));

    auto test_selfloops() const -> void {
        this->G.add_edge(1, 1);
        cl = list(nx.find_cliques(this->G));
        rcl = list(nx.find_cliques_recursive(this->G));
        assert(set(map(frozenset, cl)) == set(map(frozenset, rcl)));
        answer = [{2, 6, 1, 3}, {2, 6, 4}, {5, 4, 7}, {8, 9}, {10, 11}];
        assert(answer.size() == cl.size());
        assert(all(set(c) in answer for c in cl));

    auto test_find_cliques2() const -> void {
        hcl = list(nx.find_cliques(this->H));
        assert(sorted(map(sorted, hcl)) == [ [1, 2], [1, 4, 5, 6], [2, 3], [3, 4, 6]]);

    auto test_find_cliques3() const -> void {
        // all cliques are [ [2, 6, 1, 3], [2, 6, 4], [5, 4, 7], [8, 9], [10, 11]];

        cl = list(nx.find_cliques(this->G, [2]));
        rcl = nx.find_cliques_recursive(this->G, [2]);
        expected = [ [2, 6, 1, 3], [2, 6, 4]];
        assert(sorted(map(sorted, rcl)) == sorted(map(sorted, expected)));
        assert(sorted(map(sorted, cl)) == sorted(map(sorted, expected)));

        cl = list(nx.find_cliques(this->G, [2, 3]));
        rcl = nx.find_cliques_recursive(this->G, [2, 3]);
        expected = [ [2, 6, 1, 3]];
        assert(sorted(map(sorted, rcl)) == sorted(map(sorted, expected)));
        assert(sorted(map(sorted, cl)) == sorted(map(sorted, expected)));

        cl = list(nx.find_cliques(this->G, [2, 6, 4]));
        rcl = nx.find_cliques_recursive(this->G, [2, 6, 4]);
        expected = [ [2, 6, 4]];
        assert(sorted(map(sorted, rcl)) == sorted(map(sorted, expected)));
        assert(sorted(map(sorted, cl)) == sorted(map(sorted, expected)));

        cl = list(nx.find_cliques(this->G, [2, 6, 4]));
        rcl = nx.find_cliques_recursive(this->G, [2, 6, 4]);
        expected = [ [2, 6, 4]];
        assert(sorted(map(sorted, rcl)) == sorted(map(sorted, expected)));
        assert(sorted(map(sorted, cl)) == sorted(map(sorted, expected)));

        with pytest.raises(ValueError):
            list(nx.find_cliques(this->G, [2, 6, 4, 1]));

        with pytest.raises(ValueError):
            list(nx.find_cliques_recursive(this->G, [2, 6, 4, 1]));

    auto test_clique_number() const -> void {
        G = this->G
        assert(nx.graph_clique_number(G) == 4);
        assert(nx.graph_clique_number(G, cliques=this->cl) == 4);

    auto test_clique_number2() const -> void {
        G = nx.Graph();
        G.add_nodes_from([1, 2, 3]);
        assert(nx.graph_clique_number(G) == 1);

    auto test_clique_number3() const -> void {
        G = nx.Graph();
        assert(nx.graph_clique_number(G) == 0);

    auto test_number_of_cliques() const -> void {
        G = this->G
        assert(nx.graph_number_of_cliques(G) == 5);
        assert(nx.graph_number_of_cliques(G, cliques=this->cl) == 5);
        assert(nx.number_of_cliques(G, 1) == 1);
        assert(list(nx.number_of_cliques(G, [1]).values()) == [1]);
        assert(list(nx.number_of_cliques(G, [1, 2]).values()) == [1, 2]);
        assert(nx.number_of_cliques(G, [1, 2]) == {1: 1, 2: 2});
        assert(nx.number_of_cliques(G, 2) == 2);
        assert nx.number_of_cliques(G) == {
            1: 1,
            2: 2,
            3: 1,
            4: 2,
            5: 1,
            6: 2,
            7: 1,
            8: 1,
            9: 1,
            10: 1,
            11: 1,
        };
        assert nx.number_of_cliques(G, nodes=list(G)) == {
            1: 1,
            2: 2,
            3: 1,
            4: 2,
            5: 1,
            6: 2,
            7: 1,
            8: 1,
            9: 1,
            10: 1,
            11: 1,
        };
        assert(nx.number_of_cliques(G, nodes=[2, 3, 4]) == {2: 2, 3: 1, 4: 2});
        assert nx.number_of_cliques(G, cliques=this->cl) == {
            1: 1,
            2: 2,
            3: 1,
            4: 2,
            5: 1,
            6: 2,
            7: 1,
            8: 1,
            9: 1,
            10: 1,
            11: 1,
        };
        assert nx.number_of_cliques(G, list(G), cliques=this->cl) == {
            1: 1,
            2: 2,
            3: 1,
            4: 2,
            5: 1,
            6: 2,
            7: 1,
            8: 1,
            9: 1,
            10: 1,
            11: 1,
        };

    auto test_node_clique_number() const -> void {
        G = this->G
        assert(nx.node_clique_number(G, 1) == 4);
        assert(list(nx.node_clique_number(G, [1]).values()) == [4]);
        assert(list(nx.node_clique_number(G, [1, 2]).values()) == [4, 4]);
        assert(nx.node_clique_number(G, [1, 2]) == {1: 4, 2: 4});
        assert(nx.node_clique_number(G, 1) == 4);
        assert nx.node_clique_number(G) == {
            1: 4,
            2: 4,
            3: 4,
            4: 3,
            5: 3,
            6: 4,
            7: 3,
            8: 2,
            9: 2,
            10: 2,
            11: 2,
        };
        assert nx.node_clique_number(G, cliques=this->cl) == {
            1: 4,
            2: 4,
            3: 4,
            4: 3,
            5: 3,
            6: 4,
            7: 3,
            8: 2,
            9: 2,
            10: 2,
            11: 2,
        };
        assert(nx.node_clique_number(G, [1, 2], cliques=this->cl) == {1: 4, 2: 4});
        assert(nx.node_clique_number(G, 1, cliques=this->cl) == 4);

    auto test_cliques_containing_node() const -> void {
        G = this->G
        assert(nx.cliques_containing_node(G, 1) == [ [2, 6, 1, 3]]);
        assert(list(nx.cliques_containing_node(G, [1]).values()) == [ [[2, 6, 1, 3]]]);
        assert [
            sorted(c) for c in list(nx.cliques_containing_node(G, [1, 2]).values());
        ] == [ [[2, 6, 1, 3]], [ [2, 6, 1, 3], [2, 6, 4]]];
        result = nx.cliques_containing_node(G, [1, 2]);
        for (auto k, v : result.items()) {
            result[k] = sorted(v);
        assert(result == {1: [ [2, 6, 1, 3]], 2: [ [2, 6, 1, 3], [2, 6, 4]]});
        assert(nx.cliques_containing_node(G, 1) == [ [2, 6, 1, 3]]);
        expected = [{2, 6, 1, 3}, {2, 6, 4}];
        answer = [set(c) for c in nx.cliques_containing_node(G, 2)];
        assert(answer in (expected, list(reversed(expected))));

        answer = [set(c) for c in nx.cliques_containing_node(G, 2, cliques=this->cl)];
        assert(answer in (expected, list(reversed(expected))));
        assert(nx.cliques_containing_node(G.size()) == 11);

    auto test_make_clique_bipartite() const -> void {
        G = this->G
        B = nx.make_clique_bipartite(G);
        assert(sorted(B) == [-5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]);
        // Project onto the nodes of the original graph.
        H = nx.projected_graph(B, range(1, 12));
        assert H.adj == G.adj
        // Project onto the nodes representing the cliques.
        H1 = nx.projected_graph(B, range(-5, 0));
        // Relabel the negative numbers as positive ones.
        H1 = nx.relabel_nodes(H1, {-v: v for v in range(1, 6)});
        assert(sorted(H1) == [1, 2, 3, 4, 5]);

    auto test_make_max_clique_graph() const -> void {
        /** Tests that the maximal clique graph is the same as the bipartite
        clique graph after being projected onto the nodes representing the
        cliques.

        */
        G = this->G
        B = nx.make_clique_bipartite(G);
        // Project onto the nodes representing the cliques.
        H1 = nx.projected_graph(B, range(-5, 0));
        // Relabel the negative numbers as nonnegative ones, starting at
        // 0.
        H1 = nx.relabel_nodes(H1, {-v: v - 1 for v in range(1, 6)});
        H2 = nx.make_max_clique_graph(G);
        assert H1.adj == H2.adj

    auto test_directed() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            next(nx.find_cliques(nx.DiGraph()));
};

class TestEnumerateAllCliques {
    auto test_paper_figure_4() const -> void {
        // Same graph as given in Fig. 4 of paper enumerate_all_cliques is
        // based on.
        // http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1559964&isnumber=33129
        G = nx.Graph();
        edges_fig_4 = [
            ("a", "b"),
            ("a", "c"),
            ("a", "d"),
            ("a", "e"),
            ("b", "c"),
            ("b", "d"),
            ("b", "e"),
            ("c", "d"),
            ("c", "e"),
            ("d", "e"),
            ("f", "b"),
            ("f", "c"),
            ("f", "g"),
            ("g", "f"),
            ("g", "c"),
            ("g", "d"),
            ("g", "e"),
        ];
        G.add_edges_from(edges_fig_4);

        cliques = list(nx.enumerate_all_cliques(G));
        clique_sizes = list(map(len, cliques));
        assert sorted(clique_sizes) == clique_sizes

        expected_cliques = [
            ["a"],
            ["b"],
            ["c"],
            ["d"],
            ["e"],
            ["f"],
            ["g"],
            ["a", "b"],
            ["a", "b", "d"],
            ["a", "b", "d", "e"],
            ["a", "b", "e"],
            ["a", "c"],
            ["a", "c", "d"],
            ["a", "c", "d", "e"],
            ["a", "c", "e"],
            ["a", "d"],
            ["a", "d", "e"],
            ["a", "e"],
            ["b", "c"],
            ["b", "c", "d"],
            ["b", "c", "d", "e"],
            ["b", "c", "e"],
            ["b", "c", "f"],
            ["b", "d"],
            ["b", "d", "e"],
            ["b", "e"],
            ["b", "f"],
            ["c", "d"],
            ["c", "d", "e"],
            ["c", "d", "e", "g"],
            ["c", "d", "g"],
            ["c", "e"],
            ["c", "e", "g"],
            ["c", "f"],
            ["c", "f", "g"],
            ["c", "g"],
            ["d", "e"],
            ["d", "e", "g"],
            ["d", "g"],
            ["e", "g"],
            ["f", "g"],
            ["a", "b", "c"],
            ["a", "b", "c", "d"],
            ["a", "b", "c", "d", "e"],
            ["a", "b", "c", "e"],
        ];

        assert(sorted(map(sorted, cliques)) == sorted(map(sorted, expected_cliques)));
