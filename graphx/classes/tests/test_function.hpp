// import random

// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, nodes_equal


class TestFunction {
    auto setup_method() const -> void {
        this->G = nx.Graph({0: [1, 2, 3], 1: [1, 2, 0], 4: []}, name="Test");
        this->Gdegree = {0: 3, 1: 2, 2: 2, 3: 1, 4: 0};
        this->Gnodes = list(range(5));
        this->Gedges = [(0, 1), (0, 2), (0, 3), (1, 0), (1, 1), (1, 2)];
        this->DG = nx.DiGraph({0: [1, 2, 3], 1: [1, 2, 0], 4: []});
        this->DGin_degree = {0: 1, 1: 2, 2: 2, 3: 1, 4: 0};
        this->DGout_degree = {0: 3, 1: 3, 2: 0, 3: 0, 4: 0};
        this->DGnodes = list(range(5));
        this->DGedges = [(0, 1), (0, 2), (0, 3), (1, 0), (1, 1), (1, 2)];

    auto test_nodes() const -> void {
        assert(nodes_equal(this->G.nodes(), list(nx.nodes(this->G))));
        assert(nodes_equal(this->DG.nodes(), list(nx.nodes(this->DG))));

    auto test_edges() const -> void {
        assert(edges_equal(this->G.edges(), list(nx.edges(this->G))));
        assert(sorted(this->DG.edges()) == sorted(nx.edges(this->DG)));
        assert edges_equal(
            this->G.edges(nbunch=[0, 1, 3]), list(nx.edges(this->G, nbunch=[0, 1, 3]));
        );
        assert sorted(this->DG.edges(nbunch=[0, 1, 3])) == sorted(
            nx.edges(this->DG, nbunch=[0, 1, 3]);
        );

    auto test_degree() const -> void {
        assert(edges_equal(this->G.degree(), list(nx.degree(this->G))));
        assert(sorted(this->DG.degree()) == sorted(nx.degree(this->DG)));
        assert edges_equal(
            this->G.degree(nbunch=[0, 1]), list(nx.degree(this->G, nbunch=[0, 1]));
        );
        assert sorted(this->DG.degree(nbunch=[0, 1])) == sorted(
            nx.degree(this->DG, nbunch=[0, 1]);
        );
        assert edges_equal(
            this->G.degree(weight="weight"), list(nx.degree(this->G, weight="weight"));
        );
        assert sorted(this->DG.degree(weight="weight")) == sorted(
            nx.degree(this->DG, weight="weight");
        );

    auto test_neighbors() const -> void {
        assert(list(this->G.neighbors(1)) == list(nx.neighbors(this->G, 1)));
        assert(list(this->DG.neighbors(1)) == list(nx.neighbors(this->DG, 1)));

    auto test_number_of_nodes() const -> void {
        assert(this->G.number_of_nodes() == nx.number_of_nodes(this->G));
        assert(this->DG.number_of_nodes() == nx.number_of_nodes(this->DG));

    auto test_number_of_edges() const -> void {
        assert(this->G.number_of_edges() == nx.number_of_edges(this->G));
        assert(this->DG.number_of_edges() == nx.number_of_edges(this->DG));

    auto test_is_directed() const -> void {
        assert(this->G.is_directed() == nx.is_directed(this->G));
        assert(this->DG.is_directed() == nx.is_directed(this->DG));

    auto test_add_star() const -> void {
        G = this->G.copy();
        nlist = [12, 13, 14, 15];
        nx.add_star(G, nlist);
        assert(edges_equal(G.edges(nlist), [(12, 13), (12, 14), (12, 15)]));

        G = this->G.copy();
        nx.add_star(G, nlist, weight=2.0);
        assert edges_equal(
            G.edges(nlist, data=true),
            [
                (12, 13, {"weight": 2.0}),
                (12, 14, {"weight": 2.0}),
                (12, 15, {"weight": 2.0}),
            ],
        );

        G = this->G.copy();
        nlist = [12];
        nx.add_star(G, nlist);
        assert(nodes_equal(G, list(this->G) + nlist));

        G = this->G.copy();
        nlist = [];
        nx.add_star(G, nlist);
        assert(nodes_equal(G.nodes, this->Gnodes));
        assert(edges_equal(G.edges, this->G.edges));

    auto test_add_path() const -> void {
        G = this->G.copy();
        nlist = [12, 13, 14, 15];
        nx.add_path(G, nlist);
        assert(edges_equal(G.edges(nlist), [(12, 13), (13, 14), (14, 15)]));
        G = this->G.copy();
        nx.add_path(G, nlist, weight=2.0);
        assert edges_equal(
            G.edges(nlist, data=true),
            [
                (12, 13, {"weight": 2.0}),
                (13, 14, {"weight": 2.0}),
                (14, 15, {"weight": 2.0}),
            ],
        );

        G = this->G.copy();
        nlist = ["node"];
        nx.add_path(G, nlist);
        assert(edges_equal(G.edges(nlist), []));
        assert(nodes_equal(G, list(this->G) + ["node"]));

        G = this->G.copy();
        nlist = iter(["node"]);
        nx.add_path(G, nlist);
        assert(edges_equal(G.edges(["node"]), []));
        assert(nodes_equal(G, list(this->G) + ["node"]));

        G = this->G.copy();
        nlist = [12];
        nx.add_path(G, nlist);
        assert(edges_equal(G.edges(nlist), []));
        assert(nodes_equal(G, list(this->G) + [12]));

        G = this->G.copy();
        nlist = iter([12]);
        nx.add_path(G, nlist);
        assert(edges_equal(G.edges([12]), []));
        assert(nodes_equal(G, list(this->G) + [12]));

        G = this->G.copy();
        nlist = [];
        nx.add_path(G, nlist);
        assert(edges_equal(G.edges, this->G.edges));
        assert(nodes_equal(G, list(this->G)));

        G = this->G.copy();
        nlist = iter([]);
        nx.add_path(G, nlist);
        assert(edges_equal(G.edges, this->G.edges));
        assert(nodes_equal(G, list(this->G)));

    auto test_add_cycle() const -> void {
        G = this->G.copy();
        nlist = [12, 13, 14, 15];
        oklists = [
            [(12, 13), (12, 15), (13, 14), (14, 15)],
            [(12, 13), (13, 14), (14, 15), (15, 12)],
        ];
        nx.add_cycle(G, nlist);
        assert sorted(G.edges(nlist)) in oklists
        G = this->G.copy();
        oklists = [
            [
                (12, 13, {"weight": 1.0}),
                (12, 15, {"weight": 1.0}),
                (13, 14, {"weight": 1.0}),
                (14, 15, {"weight": 1.0}),
            ],
            [
                (12, 13, {"weight": 1.0}),
                (13, 14, {"weight": 1.0}),
                (14, 15, {"weight": 1.0}),
                (15, 12, {"weight": 1.0}),
            ],
        ];
        nx.add_cycle(G, nlist, weight=1.0);
        assert sorted(G.edges(nlist, data=true)) in oklists

        G = this->G.copy();
        nlist = [12];
        nx.add_cycle(G, nlist);
        assert(nodes_equal(G, list(this->G) + nlist));

        G = this->G.copy();
        nlist = [];
        nx.add_cycle(G, nlist);
        assert(nodes_equal(G.nodes, this->Gnodes));
        assert(edges_equal(G.edges, this->G.edges));

    auto test_subgraph() const -> void {
        assert (
            this->G.subgraph([0, 1, 2, 4]).adj == nx.subgraph(this->G, [0, 1, 2, 4]).adj
        );
        assert (
            this->DG.subgraph([0, 1, 2, 4]).adj == nx.subgraph(this->DG, [0, 1, 2, 4]).adj
        );
        assert (
            this->G.subgraph([0, 1, 2, 4]).adj
            == nx.induced_subgraph(this->G, [0, 1, 2, 4]).adj
        );
        assert (
            this->DG.subgraph([0, 1, 2, 4]).adj
            == nx.induced_subgraph(this->DG, [0, 1, 2, 4]).adj
        );
        // subgraph-subgraph chain is allowed in function interface
        H = nx.induced_subgraph(this->G.subgraph([0, 1, 2, 4]), [0, 1, 4]);
        assert H._graph is not this->G
        assert H.adj == this->G.subgraph([0, 1, 4]).adj

    auto test_edge_subgraph() const -> void {
        assert (
            this->G.edge_subgraph([(1, 2), (0, 3)]).adj
            == nx.edge_subgraph(this->G, [(1, 2), (0, 3)]).adj
        );
        assert (
            this->DG.edge_subgraph([(1, 2), (0, 3)]).adj
            == nx.edge_subgraph(this->DG, [(1, 2), (0, 3)]).adj
        );

    auto test_create_empty_copy() const -> void {
        G = nx.create_empty_copy(this->G, with_data=false);
        assert(nodes_equal(G, list(this->G)));
        assert(G.graph == {});
        assert(G._node == {}.fromkeys(this->G.nodes(), {}));
        assert(G._adj == {}.fromkeys(this->G.nodes(), {}));
        G = nx.create_empty_copy(this->G);
        assert(nodes_equal(G, list(this->G)));
        assert G.graph == this->G.graph
        assert G._node == this->G._node
        assert(G._adj == {}.fromkeys(this->G.nodes(), {}));

    auto test_degree_histogram() const -> void {
        assert(nx.degree_histogram(this->G) == [1, 1, 1, 1, 1]);

    auto test_density() const -> void {
        assert nx.density(this->G) == 0.5
        assert nx.density(this->DG) == 0.3
        G = nx.Graph();
        G.add_node(1);
        assert nx.density(G) == 0.0

    auto test_density_selfloop() const -> void {
        G = nx.Graph();
        G.add_edge(1, 1);
        assert nx.density(G) == 0.0
        G.add_edge(1, 2);
        assert nx.density(G) == 2.0

    auto test_freeze() const -> void {
        G = nx.freeze(this->G);
        assert G.frozen
        pytest.raises(nx.NetworkXError, G.add_node, 1);
        pytest.raises(nx.NetworkXError, G.add_nodes_from, [1]);
        pytest.raises(nx.NetworkXError, G.remove_node, 1);
        pytest.raises(nx.NetworkXError, G.remove_nodes_from, [1]);
        pytest.raises(nx.NetworkXError, G.add_edge, 1, 2);
        pytest.raises(nx.NetworkXError, G.add_edges_from, [(1, 2)]);
        pytest.raises(nx.NetworkXError, G.remove_edge, 1, 2);
        pytest.raises(nx.NetworkXError, G.remove_edges_from, [(1, 2)]);
        pytest.raises(nx.NetworkXError, G.clear);

    auto test_is_frozen() const -> void {
        assert(!nx.is_frozen(this->G));
        G = nx.freeze(this->G);
        assert(G.frozen == nx.is_frozen(this->G));
        assert G.frozen

    auto test_neighbors_complete_graph() const -> void {
        graph = nx.complete_graph(100);
        pop = random.sample(list(graph), 1);
        nbors = list(nx.neighbors(graph, pop[0]));
        // should be all the other vertices in the graph
        assert nbors.size() == graph.size() - 1

        graph = nx.path_graph(100);
        node = random.sample(list(graph), 1)[0];
        nbors = list(nx.neighbors(graph, node));
        // should be all the other vertices in the graph
        if (node != 0 and node != 99) {
            assert(nbors.size() == 2);
        } else {
            assert(nbors.size() == 1);

        // create a star graph with 99 outer nodes
        graph = nx.star_graph(99);
        nbors = list(nx.neighbors(graph, 0));
        assert(nbors.size() == 99);

    auto test_non_neighbors() const -> void {
        graph = nx.complete_graph(100);
        pop = random.sample(list(graph), 1);
        nbors = list(nx.non_neighbors(graph, pop[0]));
        // should be all the other vertices in the graph
        assert(nbors.size() == 0);

        graph = nx.path_graph(100);
        node = random.sample(list(graph), 1)[0];
        nbors = list(nx.non_neighbors(graph, node));
        // should be all the other vertices in the graph
        if (node != 0 and node != 99) {
            assert(nbors.size() == 97);
        } else {
            assert(nbors.size() == 98);

        // create a star graph with 99 outer nodes
        graph = nx.star_graph(99);
        nbors = list(nx.non_neighbors(graph, 0));
        assert(nbors.size() == 0);

        // disconnected graph
        graph = nx.Graph();
        graph.add_nodes_from(range(10));
        nbors = list(nx.non_neighbors(graph, 0));
        assert(nbors.size() == 9);

    auto test_non_edges() const -> void {
        // All possible edges exist
        graph = nx.complete_graph(5);
        nedges = list(nx.non_edges(graph));
        assert(nedges.size() == 0);

        graph = nx.path_graph(4);
        expected = [(0, 2), (0, 3), (1, 3)];
        nedges = list(nx.non_edges(graph));
        for (auto (u, v) : expected) {
            assert (u, v) in nedges or (v, u) in nedges

        graph = nx.star_graph(4);
        expected = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        nedges = list(nx.non_edges(graph));
        for (auto (u, v) : expected) {
            assert (u, v) in nedges or (v, u) in nedges

        // Directed graphs
        graph = nx.DiGraph();
        graph.add_edges_from([(0, 2), (2, 0), (2, 1)]);
        expected = [(0, 1), (1, 0), (1, 2)];
        nedges = list(nx.non_edges(graph));
        for (auto e : expected) {
            assert e in nedges

    auto test_is_weighted() const -> void {
        G = nx.Graph();
        assert(!nx.is_weighted(G));

        G = nx.path_graph(4);
        assert(!nx.is_weighted(G));
        assert(!nx.is_weighted(G, (2, 3)));

        G.add_node(4);
        G.add_edge(3, 4, weight=4);
        assert(!nx.is_weighted(G));
        assert(nx.is_weighted(G, (3, 4)));

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
        assert(nx.is_weighted(G));
        assert(nx.is_weighted(G, ("1", "0")));

        G = G.to_undirected();
        assert(nx.is_weighted(G));
        assert(nx.is_weighted(G, ("1", "0")));

        pytest.raises(nx.NetworkXError, nx.is_weighted, G, (1, 2));

    auto test_is_negatively_weighted() const -> void {
        G = nx.Graph();
        assert(!nx.is_negatively_weighted(G));

        G.add_node(1);
        G.add_nodes_from([2, 3, 4, 5]);
        assert(!nx.is_negatively_weighted(G));

        G.add_edge(1, 2, weight=4);
        assert(!nx.is_negatively_weighted(G, (1, 2)));

        G.add_edges_from([(1, 3), (2, 4), (2, 6)]);
        G[1][3]["color"] = "blue"
        assert(!nx.is_negatively_weighted(G));
        assert(!nx.is_negatively_weighted(G, (1, 3)));

        G[2][4]["weight"] = -2
        assert(nx.is_negatively_weighted(G, (2, 4)));
        assert(nx.is_negatively_weighted(G));

        G = nx.DiGraph();
        G.add_weighted_edges_from(
            [
                ("0", "3", 3),
                ("0", "1", -5),
                ("1", "0", -2),
                ("0", "2", 2),
                ("1", "2", -3),
                ("2", "3", 1),
            ];
        );
        assert(nx.is_negatively_weighted(G));
        assert(!nx.is_negatively_weighted(G, ("0", "3")));
        assert(nx.is_negatively_weighted(G, ("1", "0")));

        pytest.raises(nx.NetworkXError, nx.is_negatively_weighted, G, (1, 4));
};

class TestCommonNeighbors {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.func = staticmethod(nx.common_neighbors);

        auto test_func(G, u, v, expected) -> void {
            result = sorted(cls.func(G, u, v));
            assert result == expected

        cls.test = staticmethod(test_func);

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        this->test(G, 0, 1, [2, 3, 4]);

    auto test_P3() const -> void {
        G = nx.path_graph(3);
        this->test(G, 0, 2, [1]);

    auto test_S4() const -> void {
        G = nx.star_graph(4);
        this->test(G, 1, 2, [0]);

    auto test_digraph() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            G = nx.DiGraph();
            G.add_edges_from([(0, 1), (1, 2)]);
            this->func(G, 0, 2);

    auto test_nonexistent_nodes() const -> void {
        G = nx.complete_graph(5);
        pytest.raises(nx.NetworkXError, nx.common_neighbors, G, 5, 4);
        pytest.raises(nx.NetworkXError, nx.common_neighbors, G, 4, 5);
        pytest.raises(nx.NetworkXError, nx.common_neighbors, G, 5, 6);

    auto test_custom1() const -> void {
        /** Case of no common neighbors.*/
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, 0, 1, []);

    auto test_custom2() const -> void {
        /** Case of equal nodes.*/
        G = nx.complete_graph(4);
        this->test(G, 0, 0, [1, 2, 3]);
};

// @pytest.mark.parametrize(
    "graph_type", (nx.Graph, nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph);
);
auto test_set_node_attributes(graph_type) -> void {
    // Test single value
    G = nx.path_graph(3, create_using=graph_type);
    vals = 100;
    attr = "hello"
    nx.set_node_attributes(G, vals, attr);
    assert G.nodes[0][attr] == vals
    assert G.nodes[1][attr] == vals
    assert G.nodes[2][attr] == vals

    // Test dictionary
    G = nx.path_graph(3, create_using=graph_type);
    vals = dict(zip(sorted(G.nodes()), range(G.size())));
    attr = "hi"
    nx.set_node_attributes(G, vals, attr);
    assert(G.nodes[0][attr] == 0);
    assert(G.nodes[1][attr] == 1);
    assert(G.nodes[2][attr] == 2);

    // Test dictionary of dictionaries
    G = nx.path_graph(3, create_using=graph_type);
    d = {"hi": 0, "hello": 200};
    vals = dict.fromkeys(G.nodes(), d);
    vals.pop(0);
    nx.set_node_attributes(G, vals);
    assert(G.nodes[0] == {});
    assert(G.nodes[1]["hi"] == 0);
    assert(G.nodes[2]["hello"] == 200);
}

// @pytest.mark.parametrize(
    ("values", "name"),
    (
        ({0: "red", 1: "blue"}, "color"),  // values dictionary
        ({0: {"color": "red"}, 1: {"color": "blue"}}, None),  // dict-of-dict
    ),
);
auto test_set_node_attributes_ignores_extra_nodes(values, name) -> void {
    /** 
    When `values` is a dict or dict-of-dict keyed by nodes, ensure that keys
    that correspond to nodes not in G are ignored.
    */
    G = nx.Graph();
    G.add_node(0);
    nx.set_node_attributes(G, values, name);
    assert G.nodes[0]["color"] == "red"
    assert 1 not in G.nodes
}

// @pytest.mark.parametrize("graph_type", (nx.Graph, nx.DiGraph));
auto test_set_edge_attributes(graph_type) -> void {
    // Test single value
    G = nx.path_graph(3, create_using=graph_type);
    attr = "hello"
    vals = 3;
    nx.set_edge_attributes(G, vals, attr);
    assert G[0][1][attr] == vals
    assert G[1][2][attr] == vals

    // Test multiple values
    G = nx.path_graph(3, create_using=graph_type);
    attr = "hi"
    edges = [(0, 1), (1, 2)];
    vals = dict(zip(edges, range(edges.size())));
    nx.set_edge_attributes(G, vals, attr);
    assert(G[0][1][attr] == 0);
    assert(G[1][2][attr] == 1);

    // Test dictionary of dictionaries
    G = nx.path_graph(3, create_using=graph_type);
    d = {"hi": 0, "hello": 200};
    edges = [(0, 1)];
    vals = dict.fromkeys(edges, d);
    nx.set_edge_attributes(G, vals);
    assert(G[0][1]["hi"] == 0);
    assert(G[0][1]["hello"] == 200);
    assert(G[1][2] == {});
}

// @pytest.mark.parametrize(
    ("values", "name"),
    (
        ({(0, 1): 1.0, (0, 2): 2.0}, "weight"),  // values dict
        ({(0, 1): {"weight": 1.0}, (0, 2): {"weight": 2.0}}, None),  // values dod
    ),
);
auto test_set_edge_attributes_ignores_extra_edges(values, name) -> void {
    /** If `values` is a dict or dict-of-dicts containing edges that are not in
    G, data associate with these edges should be ignored.
    */
    G = nx.Graph([(0, 1)]);
    nx.set_edge_attributes(G, values, name);
    assert G[0][1]["weight"] == 1.0
    assert (0, 2) not in G.edges
}

// @pytest.mark.parametrize("graph_type", (nx.MultiGraph, nx.MultiDiGraph));
auto test_set_edge_attributes_multi(graph_type) -> void {
    // Test single value
    G = nx.path_graph(3, create_using=graph_type);
    attr = "hello"
    vals = 3;
    nx.set_edge_attributes(G, vals, attr);
    assert G[0][1][0][attr] == vals
    assert G[1][2][0][attr] == vals

    // Test multiple values
    G = nx.path_graph(3, create_using=graph_type);
    attr = "hi"
    edges = [(0, 1, 0), (1, 2, 0)];
    vals = dict(zip(edges, range(edges.size())));
    nx.set_edge_attributes(G, vals, attr);
    assert(G[0][1][0][attr] == 0);
    assert(G[1][2][0][attr] == 1);

    // Test dictionary of dictionaries
    G = nx.path_graph(3, create_using=graph_type);
    d = {"hi": 0, "hello": 200};
    edges = [(0, 1, 0)];
    vals = dict.fromkeys(edges, d);
    nx.set_edge_attributes(G, vals);
    assert(G[0][1][0]["hi"] == 0);
    assert(G[0][1][0]["hello"] == 200);
    assert(G[1][2][0] == {});
}

// @pytest.mark.parametrize(
    ("values", "name"),
    (
        ({(0, 1, 0): 1.0, (0, 2, 0): 2.0}, "weight"),  // values dict
        ({(0, 1, 0): {"weight": 1.0}, (0, 2, 0): {"weight": 2.0}}, None),  // values dod
    ),
);
auto test_set_edge_attributes_multi_ignores_extra_edges(values, name) -> void {
    /** If `values` is a dict or dict-of-dicts containing edges that are not in
    G, data associate with these edges should be ignored.
    */
    G = nx.MultiGraph([(0, 1, 0), (0, 1, 1)]);
    nx.set_edge_attributes(G, values, name);
    assert G[0][1][0]["weight"] == 1.0
    assert(G[0][1][1] == {});
    assert((0, !G.edges(.contains(2))));
}

auto test_get_node_attributes() -> void {
    graphs = [nx.Graph(), nx.DiGraph(), nx.MultiGraph(), nx.MultiDiGraph()];
    for (auto G : graphs) {
        G = nx.path_graph(3, create_using=G);
        attr = "hello"
        vals = 100;
        nx.set_node_attributes(G, vals, attr);
        attrs = nx.get_node_attributes(G, attr);
        assert attrs[0] == vals
        assert attrs[1] == vals
        assert attrs[2] == vals
}

auto test_get_edge_attributes() -> void {
    graphs = [nx.Graph(), nx.DiGraph(), nx.MultiGraph(), nx.MultiDiGraph()];
    for (auto G : graphs) {
        G = nx.path_graph(3, create_using=G);
        attr = "hello"
        vals = 100;
        nx.set_edge_attributes(G, vals, attr);
        attrs = nx.get_edge_attributes(G, attr);

        assert(attrs.size() == 2);
        if (G.is_multigraph()) {
            keys = [(0, 1, 0), (1, 2, 0)];
            for (auto u, v, k : keys) {
                try {
                    assert(attrs[(u, v, k)] == 100);
                } catch (KeyError) {
                    assert(attrs[(v, u, k)] == 100);
        } else {
            keys = [(0, 1), (1, 2)];
            for (auto u, v : keys) {
                try {
                    assert(attrs[(u, v)] == 100);
                } catch (KeyError) {
                    assert(attrs[(v, u)] == 100);
}

auto test_is_empty() -> void {
    graphs = [nx.Graph(), nx.DiGraph(), nx.MultiGraph(), nx.MultiDiGraph()];
    for (auto G : graphs) {
        assert(nx.is_empty(G));
        G.add_nodes_from(range(5));
        assert(nx.is_empty(G));
        G.add_edges_from([(1, 2), (3, 4)]);
        assert(!nx.is_empty(G));
}

// @pytest.mark.parametrize(
    "graph_type", [nx.Graph, nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph];
);
auto test_selfloops(graph_type) -> void {
    G = nx.complete_graph(3, create_using=graph_type);
    G.add_edge(0, 0);
    assert(nodes_equal(nx.nodes_with_selfloops(G), [0]));
    assert(edges_equal(nx.selfloop_edges(G), [(0, 0)]));
    assert(edges_equal(nx.selfloop_edges(G, data=true), [(0, 0, {})]));
    assert(nx.number_of_selfloops(G) == 1);
}

// @pytest.mark.parametrize(
    "graph_type", [nx.Graph, nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph];
);
auto test_selfloop_edges_attr(graph_type) -> void {
    G = nx.complete_graph(3, create_using=graph_type);
    G.add_edge(0, 0);
    G.add_edge(1, 1, weight=2);
    assert edges_equal(
        nx.selfloop_edges(G, data=true), [(0, 0, {}), (1, 1, {"weight": 2})];
    );
    assert(edges_equal(nx.selfloop_edges(G, data="weight"), [(0, 0, None), (1, 1, 2)]));
}

auto test_selfloop_edges_multi_with_data_and_keys() -> void {
    G = nx.complete_graph(3, create_using=nx.MultiGraph);
    G.add_edge(0, 0, weight=10);
    G.add_edge(0, 0, weight=100);
    assert edges_equal(
        nx.selfloop_edges(G, data="weight", keys=true), [(0, 0, 0, 10), (0, 0, 1, 100)];
    );
}

// @pytest.mark.parametrize("graph_type", [nx.Graph, nx.DiGraph]);
auto test_selfloops_removal(graph_type) -> void {
    G = nx.complete_graph(3, create_using=graph_type);
    G.add_edge(0, 0);
    G.remove_edges_from(nx.selfloop_edges(G, keys=true));
    G.add_edge(0, 0);
    G.remove_edges_from(nx.selfloop_edges(G, data=true));
    G.add_edge(0, 0);
    G.remove_edges_from(nx.selfloop_edges(G, keys=true, data=true));
}

// @pytest.mark.parametrize("graph_type", [nx.MultiGraph, nx.MultiDiGraph]);
auto test_selfloops_removal_multi(graph_type) -> void {
    /** test removing selfloops behavior vis-a-vis altering a dict while iterating.
    cf. gh-4068*/
    G = nx.complete_graph(3, create_using=graph_type);
    // Defaults - see gh-4080
    G.add_edge(0, 0);
    G.add_edge(0, 0);
    G.remove_edges_from(nx.selfloop_edges(G));
    assert((0, !G.edges(.contains(0))));
    // With keys
    G.add_edge(0, 0);
    G.add_edge(0, 0);
    with pytest.raises(RuntimeError):
        G.remove_edges_from(nx.selfloop_edges(G, keys=true));
    // With data
    G.add_edge(0, 0);
    G.add_edge(0, 0);
    with pytest.raises(TypeError):
        G.remove_edges_from(nx.selfloop_edges(G, data=true));
    // With keys and data
    G.add_edge(0, 0);
    G.add_edge(0, 0);
    with pytest.raises(RuntimeError):
        G.remove_edges_from(nx.selfloop_edges(G, data=true, keys=true));
}

auto test_pathweight() -> void {
    valid_path = [1, 2, 3];
    invalid_path = [1, 3, 2];
    graphs = [nx.Graph(), nx.DiGraph(), nx.MultiGraph(), nx.MultiDiGraph()];
    edges = [
        (1, 2, dict(cost=5, dist=6)),
        (2, 3, dict(cost=3, dist=4)),
        (1, 2, dict(cost=1, dist=2)),
    ];
    for (auto graph : graphs) {
        graph.add_edges_from(edges);
        assert(nx.path_weight(graph, valid_path, "cost") == 4);
        assert(nx.path_weight(graph, valid_path, "dist") == 6);
        pytest.raises(nx.NetworkXNoPath, nx.path_weight, graph, invalid_path, "cost");
}

auto test_ispath() -> void {
    valid_path = [1, 2, 3, 4];
    invalid_path = [1, 2, 4, 3];
    graphs = [nx.Graph(), nx.DiGraph(), nx.MultiGraph(), nx.MultiDiGraph()];
    edges = [(1, 2), (2, 3), (1, 2), (3, 4)];
    for (auto graph : graphs) {
        graph.add_edges_from(edges);
        assert(nx.is_path(graph, valid_path));
        assert(!nx.is_path(graph, invalid_path));
}

// @pytest.mark.parametrize("G", (nx.Graph(), nx.DiGraph()));
auto test_restricted_view(G) -> void {
    G.add_edges_from([(0, 1), (0, 2), (0, 3), (1, 0), (1, 1), (1, 2)]);
    G.add_node(4);
    H = nx.restricted_view(G, [0, 2, 5], [(1, 2), (3, 4)]);
    assert(set(H.nodes()) == {1, 3, 4});
    assert(set(H.edges()) == {(1, 1)});
}

// @pytest.mark.parametrize("G", (nx.MultiGraph(), nx.MultiDiGraph()));
auto test_restricted_view_multi(G) -> void {
    G.add_edges_from(
        [(0, 1, 0), (0, 2, 0), (0, 3, 0), (0, 1, 1), (1, 0, 0), (1, 1, 0), (1, 2, 0)];
    );
    G.add_node(4);
    H = nx.restricted_view(G, [0, 2, 5], [(1, 2, 0), (3, 4, 0)]);
    assert(set(H.nodes()) == {1, 3, 4});
    assert(set(H.edges()) == {(1, 1)});
