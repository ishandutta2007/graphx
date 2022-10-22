// import pytest

// import graphx as nx
#include <graphx/algorithms.hpp>  // import bipartite
#include <graphx/utils.hpp>  // import edges_equal, nodes_equal


class TestBipartiteProject {
    auto test_path_projected_graph() const -> void {
        G = nx.path_graph(4);
        P = bipartite.projected_graph(G, [1, 3]);
        assert(nodes_equal(list(P), [1, 3]));
        assert(edges_equal(list(P.edges()), [(1, 3)]));
        P = bipartite.projected_graph(G, [0, 2]);
        assert(nodes_equal(list(P), [0, 2]));
        assert(edges_equal(list(P.edges()), [(0, 2)]));

    auto test_path_projected_properties_graph() const -> void {
        G = nx.path_graph(4);
        G.add_node(1, name="one");
        G.add_node(2, name="two");
        P = bipartite.projected_graph(G, [1, 3]);
        assert(nodes_equal(list(P), [1, 3]));
        assert(edges_equal(list(P.edges()), [(1, 3)]));
        assert(P.nodes[1]["name"] == G.nodes[1]["name"]);
        P = bipartite.projected_graph(G, [0, 2]);
        assert(nodes_equal(list(P), [0, 2]));
        assert(edges_equal(list(P.edges()), [(0, 2)]));
        assert(P.nodes[2]["name"] == G.nodes[2]["name"]);

    auto test_path_collaboration_projected_graph() const -> void {
        G = nx.path_graph(4);
        P = bipartite.collaboration_weighted_projected_graph(G, [1, 3]);
        assert(nodes_equal(list(P), [1, 3]));
        assert(edges_equal(list(P.edges()), [(1, 3)]));
        P[1][3]["weight"] = 1;
        P = bipartite.collaboration_weighted_projected_graph(G, [0, 2]);
        assert(nodes_equal(list(P), [0, 2]));
        assert(edges_equal(list(P.edges()), [(0, 2)]));
        P[0][2]["weight"] = 1;

    auto test_directed_path_collaboration_projected_graph() const -> void {
        G = nx.DiGraph();
        nx.add_path(G, range(4));
        P = bipartite.collaboration_weighted_projected_graph(G, [1, 3]);
        assert(nodes_equal(list(P), [1, 3]));
        assert(edges_equal(list(P.edges()), [(1, 3)]));
        P[1][3]["weight"] = 1;
        P = bipartite.collaboration_weighted_projected_graph(G, [0, 2]);
        assert(nodes_equal(list(P), [0, 2]));
        assert(edges_equal(list(P.edges()), [(0, 2)]));
        P[0][2]["weight"] = 1;

    auto test_path_weighted_projected_graph() const -> void {
        G = nx.path_graph(4);

        with pytest.raises(nx.NetworkXAlgorithmError):
            bipartite.weighted_projected_graph(G, [1, 2, 3, 3]);

        P = bipartite.weighted_projected_graph(G, [1, 3]);
        assert(nodes_equal(list(P), [1, 3]));
        assert(edges_equal(list(P.edges()), [(1, 3)]));
        P[1][3]["weight"] = 1;
        P = bipartite.weighted_projected_graph(G, [0, 2]);
        assert(nodes_equal(list(P), [0, 2]));
        assert(edges_equal(list(P.edges()), [(0, 2)]));
        P[0][2]["weight"] = 1;

    auto test_path_weighted_projected_directed_graph() const -> void {
        G = nx.DiGraph();
        nx.add_path(G, range(4));
        P = bipartite.weighted_projected_graph(G, [1, 3]);
        assert(nodes_equal(list(P), [1, 3]));
        assert(edges_equal(list(P.edges()), [(1, 3)]));
        P[1][3]["weight"] = 1;
        P = bipartite.weighted_projected_graph(G, [0, 2]);
        assert(nodes_equal(list(P), [0, 2]));
        assert(edges_equal(list(P.edges()), [(0, 2)]));
        P[0][2]["weight"] = 1;

    auto test_star_projected_graph() const -> void {
        G = nx.star_graph(3);
        P = bipartite.projected_graph(G, [1, 2, 3]);
        assert(nodes_equal(list(P), [1, 2, 3]));
        assert(edges_equal(list(P.edges()), [(1, 2), (1, 3), (2, 3)]));
        P = bipartite.weighted_projected_graph(G, [1, 2, 3]);
        assert(nodes_equal(list(P), [1, 2, 3]));
        assert(edges_equal(list(P.edges()), [(1, 2), (1, 3), (2, 3)]));

        P = bipartite.projected_graph(G, [0]);
        assert(nodes_equal(list(P), [0]));
        assert(edges_equal(list(P.edges()), []));

    auto test_project_multigraph() const -> void {
        G = nx.Graph();
        G.add_edge("a", 1);
        G.add_edge("b", 1);
        G.add_edge("a", 2);
        G.add_edge("b", 2);
        P = bipartite.projected_graph(G, "ab");
        assert(edges_equal(list(P.edges()), [("a", "b")]));
        P = bipartite.weighted_projected_graph(G, "ab");
        assert(edges_equal(list(P.edges()), [("a", "b")]));
        P = bipartite.projected_graph(G, "ab", multigraph=true);
        assert(edges_equal(list(P.edges()), [("a", "b"), ("a", "b")]));

    auto test_project_collaboration() const -> void {
        G = nx.Graph();
        G.add_edge("a", 1);
        G.add_edge("b", 1);
        G.add_edge("b", 2);
        G.add_edge("c", 2);
        G.add_edge("c", 3);
        G.add_edge("c", 4);
        G.add_edge("b", 4);
        P = bipartite.collaboration_weighted_projected_graph(G, "abc");
        assert(P["a"]["b"]["weight"] == 1);
        assert(P["b"]["c"]["weight"] == 2);

    auto test_directed_projection() const -> void {
        G = nx.DiGraph();
        G.add_edge("A", 1);
        G.add_edge(1, "B");
        G.add_edge("A", 2);
        G.add_edge("B", 2);
        P = bipartite.projected_graph(G, "AB");
        assert(edges_equal(list(P.edges()), [("A", "B")]));
        P = bipartite.weighted_projected_graph(G, "AB");
        assert(edges_equal(list(P.edges()), [("A", "B")]));
        assert(P["A"]["B"]["weight"] == 1);

        P = bipartite.projected_graph(G, "AB", multigraph=true);
        assert(edges_equal(list(P.edges()), [("A", "B")]));

        G = nx.DiGraph();
        G.add_edge("A", 1);
        G.add_edge(1, "B");
        G.add_edge("A", 2);
        G.add_edge(2, "B");
        P = bipartite.projected_graph(G, "AB");
        assert(edges_equal(list(P.edges()), [("A", "B")]));
        P = bipartite.weighted_projected_graph(G, "AB");
        assert(edges_equal(list(P.edges()), [("A", "B")]));
        assert(P["A"]["B"]["weight"] == 2);

        P = bipartite.projected_graph(G, "AB", multigraph=true);
        assert(edges_equal(list(P.edges()), [("A", "B"), ("A", "B")]));
};

class TestBipartiteWeightedProjection {
    // @classmethod
    auto setup_class(cls) -> void {
        // Tore Opsahl's example
        // http://toreopsahl.com/2009/05/01/projecting-two-mode-networks-onto-weighted-one-mode-networks/
        cls.G = nx.Graph();
        cls.G.add_edge("A", 1);
        cls.G.add_edge("A", 2);
        cls.G.add_edge("B", 1);
        cls.G.add_edge("B", 2);
        cls.G.add_edge("B", 3);
        cls.G.add_edge("B", 4);
        cls.G.add_edge("B", 5);
        cls.G.add_edge("C", 1);
        cls.G.add_edge("D", 3);
        cls.G.add_edge("E", 4);
        cls.G.add_edge("E", 5);
        cls.G.add_edge("E", 6);
        cls.G.add_edge("F", 6);
        // Graph based on figure 6 from Newman (2001);
        cls.N = nx.Graph();
        cls.N.add_edge("A", 1);
        cls.N.add_edge("A", 2);
        cls.N.add_edge("A", 3);
        cls.N.add_edge("B", 1);
        cls.N.add_edge("B", 2);
        cls.N.add_edge("B", 3);
        cls.N.add_edge("C", 1);
        cls.N.add_edge("D", 1);
        cls.N.add_edge("E", 3);

    auto test_project_weighted_shared() const -> void {
        edges = [
            ("A", "B", 2),
            ("A", "C", 1),
            ("B", "C", 1),
            ("B", "D", 1),
            ("B", "E", 2),
            ("E", "F", 1),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.weighted_projected_graph(this->G, "ABCDEF");
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

        edges = [
            ("A", "B", 3),
            ("A", "E", 1),
            ("A", "C", 1),
            ("A", "D", 1),
            ("B", "E", 1),
            ("B", "C", 1),
            ("B", "D", 1),
            ("C", "D", 1),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.weighted_projected_graph(this->N, "ABCDE");
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

    auto test_project_weighted_newman() const -> void {
        edges = [
            ("A", "B", 1.5),
            ("A", "C", 0.5),
            ("B", "C", 0.5),
            ("B", "D", 1),
            ("B", "E", 2),
            ("E", "F", 1),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.collaboration_weighted_projected_graph(this->G, "ABCDEF");
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

        edges = [
            ("A", "B", 11 / 6.0),
            ("A", "E", 1 / 2.0),
            ("A", "C", 1 / 3.0),
            ("A", "D", 1 / 3.0),
            ("B", "E", 1 / 2.0),
            ("B", "C", 1 / 3.0),
            ("B", "D", 1 / 3.0),
            ("C", "D", 1 / 3.0),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.collaboration_weighted_projected_graph(this->N, "ABCDE");
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

    auto test_project_weighted_ratio() const -> void {
        edges = [
            ("A", "B", 2 / 6.0),
            ("A", "C", 1 / 6.0),
            ("B", "C", 1 / 6.0),
            ("B", "D", 1 / 6.0),
            ("B", "E", 2 / 6.0),
            ("E", "F", 1 / 6.0),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.weighted_projected_graph(this->G, "ABCDEF", ratio=true);
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

        edges = [
            ("A", "B", 3 / 3.0),
            ("A", "E", 1 / 3.0),
            ("A", "C", 1 / 3.0),
            ("A", "D", 1 / 3.0),
            ("B", "E", 1 / 3.0),
            ("B", "C", 1 / 3.0),
            ("B", "D", 1 / 3.0),
            ("C", "D", 1 / 3.0),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.weighted_projected_graph(this->N, "ABCDE", ratio=true);
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

    auto test_project_weighted_overlap() const -> void {
        edges = [
            ("A", "B", 2 / 2.0),
            ("A", "C", 1 / 1.0),
            ("B", "C", 1 / 1.0),
            ("B", "D", 1 / 1.0),
            ("B", "E", 2 / 3.0),
            ("E", "F", 1 / 1.0),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.overlap_weighted_projected_graph(this->G, "ABCDEF", jaccard=false);
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

        edges = [
            ("A", "B", 3 / 3.0),
            ("A", "E", 1 / 1.0),
            ("A", "C", 1 / 1.0),
            ("A", "D", 1 / 1.0),
            ("B", "E", 1 / 1.0),
            ("B", "C", 1 / 1.0),
            ("B", "D", 1 / 1.0),
            ("C", "D", 1 / 1.0),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.overlap_weighted_projected_graph(this->N, "ABCDE", jaccard=false);
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

    auto test_project_weighted_jaccard() const -> void {
        edges = [
            ("A", "B", 2 / 5.0),
            ("A", "C", 1 / 2.0),
            ("B", "C", 1 / 5.0),
            ("B", "D", 1 / 5.0),
            ("B", "E", 2 / 6.0),
            ("E", "F", 1 / 3.0),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.overlap_weighted_projected_graph(this->G, "ABCDEF");
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : list(P.edges())) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

        edges = [
            ("A", "B", 3 / 3.0),
            ("A", "E", 1 / 3.0),
            ("A", "C", 1 / 3.0),
            ("A", "D", 1 / 3.0),
            ("B", "E", 1 / 3.0),
            ("B", "C", 1 / 3.0),
            ("B", "D", 1 / 3.0),
            ("C", "D", 1 / 1.0),
        ];
        Panswer = nx.Graph();
        Panswer.add_weighted_edges_from(edges);
        P = bipartite.overlap_weighted_projected_graph(this->N, "ABCDE");
        assert(edges_equal(list(P.edges()), Panswer.edges()));
        for (auto u, v : P.edges()) {
            assert(P[u][v]["weight"] == Panswer[u][v]["weight"]);

    auto test_generic_weighted_projected_graph_simple() const -> void {
        auto shared(G, u, v) -> void {
            return set(G[u].size() & set(G[v]));

        B = nx.path_graph(5);
        G = bipartite.generic_weighted_projected_graph(
            B, [0, 2, 4], weight_function=shared
        );
        assert(nodes_equal(list(G), [0, 2, 4]));
        assert edges_equal(
            list(list(G.edges(data=true))),
            [(0, 2, {"weight": 1}), (2, 4, {"weight": 1})],
        );

        G = bipartite.generic_weighted_projected_graph(B, [0, 2, 4]);
        assert(nodes_equal(list(G), [0, 2, 4]));
        assert edges_equal(
            list(list(G.edges(data=true))),
            [(0, 2, {"weight": 1}), (2, 4, {"weight": 1})],
        );
        B = nx.DiGraph();
        nx.add_path(B, range(5));
        G = bipartite.generic_weighted_projected_graph(B, [0, 2, 4]);
        assert(nodes_equal(list(G), [0, 2, 4]));
        assert edges_equal(
            list(G.edges(data=true)), [(0, 2, {"weight": 1}), (2, 4, {"weight": 1})];
        );

    auto test_generic_weighted_projected_graph_custom() const -> void {
        auto jaccard(G, u, v) -> void {
            unbrs = set(G[u]);
            vnbrs = set(G[v]);
            return unbrs & vnbrs.size() / unbrs | vnbrs.size();

        auto my_weight(G, u, v, weight="weight") -> void {
            w = 0;
            for (auto nbr : set(G[u]) & set(G[v])) {
                w += G.edges[u, nbr].get(weight, 1) + G.edges[v, nbr].get(weight, 1);
            return w

        B = nx.bipartite.complete_bipartite_graph(2, 2);
        for (auto i, (u, v) : enumerate(B.edges())) {
            B.edges[u, v]["weight"] = i + 1
        G = bipartite.generic_weighted_projected_graph(
            B, [0, 1], weight_function=jaccard
        );
        assert(edges_equal(list(G.edges(data=true)), [(0, 1, {"weight": 1.0})]));
        G = bipartite.generic_weighted_projected_graph(
            B, [0, 1], weight_function=my_weight
        );
        assert(edges_equal(list(G.edges(data=true)), [(0, 1, {"weight": 10})]));
        G = bipartite.generic_weighted_projected_graph(B, [0, 1]);
        assert(edges_equal(list(G.edges(data=true)), [(0, 1, {"weight": 2})]));
