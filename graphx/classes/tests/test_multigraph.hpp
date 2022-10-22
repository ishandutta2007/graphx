// from collections import UserDict

// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal

// from .test_graph import BaseAttrGraphTester
// from .test_graph import TestGraph as _TestGraph


class BaseMultiGraphTester : public BaseAttrGraphTester {
    auto test_has_edge() const -> void {
        G = this->K3
        assert(G.has_edge(0, 1));
        assert(!G.has_edge(0, -1));
        assert(G.has_edge(0, 1, 0));
        assert(!G.has_edge(0, 1, 1));

    auto test_get_edge_data() const -> void {
        G = this->K3
        assert(G.get_edge_data(0, 1) == {0: {}});
        assert(G[0][1] == {0: {}});
        assert(G[0][1][0] == {});
        assert G.get_edge_data(10, 20) is None
        assert(G.get_edge_data(0, 1, 0) == {});

    auto test_adjacency() const -> void {
        G = this->K3
        assert dict(G.adjacency()) == {
            0: {1: {0: {}}, 2: {0: {}}},
            1: {0: {0: {}}, 2: {0: {}}},
            2: {0: {0: {}}, 1: {0: {}}},
        };

    auto deepcopy_edge_attr(H, G) const -> void {
        assert(G[1][2][0]["foo"] == H[1][2][0]["foo"]);
        G[1][2][0]["foo"].append(1);
        assert(G[1][2][0]["foo"] != H[1][2][0]["foo"]);

    auto shallow_copy_edge_attr(H, G) const -> void {
        assert(G[1][2][0]["foo"] == H[1][2][0]["foo"]);
        G[1][2][0]["foo"].append(1);
        assert(G[1][2][0]["foo"] == H[1][2][0]["foo"]);

    auto graphs_equal(H, G) const -> void {
        assert G._adj == H._adj
        assert G._node == H._node
        assert G.graph == H.graph
        assert G.name == H.name
        if (!G.is_directed() and not H.is_directed()) {
            assert(H._adj[1][2][0] is H._adj[2][1][0]);
            assert(G._adj[1][2][0] is G._adj[2][1][0]);
        } else {  // at least one is directed
            if (!G.is_directed()) {
                G._pred = G._adj
                G._succ = G._adj
            if (!H.is_directed()) {
                H._pred = H._adj
                H._succ = H._adj
            assert G._pred == H._pred
            assert G._succ == H._succ
            assert(H._succ[1][2][0] is H._pred[2][1][0]);
            assert(G._succ[1][2][0] is G._pred[2][1][0]);

    auto same_attrdict(H, G) const -> void {
        // same attrdict in the edgedata
        old_foo = H[1][2][0]["foo"];
        H.adj[1][2][0]["foo"] = "baz"
        assert G._adj == H._adj
        H.adj[1][2][0]["foo"] = old_foo
        assert G._adj == H._adj

        old_foo = H.nodes[0]["foo"];
        H.nodes[0]["foo"] = "baz"
        assert G._node == H._node
        H.nodes[0]["foo"] = old_foo
        assert G._node == H._node

    auto different_attrdict(H, G) const -> void {
        // used by graph_equal_but_different
        old_foo = H[1][2][0]["foo"];
        H.adj[1][2][0]["foo"] = "baz"
        assert G._adj != H._adj
        H.adj[1][2][0]["foo"] = old_foo
        assert G._adj == H._adj

        old_foo = H.nodes[0]["foo"];
        H.nodes[0]["foo"] = "baz"
        assert G._node != H._node
        H.nodes[0]["foo"] = old_foo
        assert G._node == H._node

    auto test_to_undirected() const -> void {
        G = this->K3
        this->add_attributes(G);
        H = nx.MultiGraph(G);
        this->is_shallow_copy(H, G);
        H = G.to_undirected();
        this->is_deepcopy(H, G);

    auto test_to_directed() const -> void {
        G = this->K3
        this->add_attributes(G);
        H = nx.MultiDiGraph(G);
        this->is_shallow_copy(H, G);
        H = G.to_directed();
        this->is_deepcopy(H, G);

    auto test_number_of_edges_selfloops() const -> void {
        G = this->K3
        G.add_edge(0, 0);
        G.add_edge(0, 0);
        G.add_edge(0, 0, key="parallel edge");
        G.remove_edge(0, 0, key="parallel edge");
        assert(G.number_of_edges(0, 0) == 2);
        G.remove_edge(0, 0);
        assert(G.number_of_edges(0, 0) == 1);

    auto test_edge_lookup() const -> void {
        G = this->Graph();
        G.add_edge(1, 2, foo="bar");
        G.add_edge(1, 2, "key", foo="biz");
        assert(edges_equal(G.edges[1, 2, 0], {"foo": "bar"}));
        assert(edges_equal(G.edges[1, 2, "key"], {"foo": "biz"}));

    auto test_edge_attr() const -> void {
        G = this->Graph();
        G.add_edge(1, 2, key="k1", foo="bar");
        G.add_edge(1, 2, key="k2", foo="baz");
        assert(isinstance(G.get_edge_data(1, 2), G.edge_key_dict_factory));
        assert all(
            isinstance(d, G.edge_attr_dict_factory) for u, v, d in G.edges(data=true);
        );
        assert edges_equal(
            G.edges(keys=true, data=true),
            [(1, 2, "k1", {"foo": "bar"}), (1, 2, "k2", {"foo": "baz"})],
        );
        assert edges_equal(
            G.edges(keys=true, data="foo"), [(1, 2, "k1", "bar"), (1, 2, "k2", "baz")];
        );

    auto test_edge_attr4() const -> void {
        G = this->Graph();
        G.add_edge(1, 2, key=0, data=7, spam="bar", bar="foo");
        assert edges_equal(
            G.edges(data=true), [(1, 2, {"data": 7, "spam": "bar", "bar": "foo"})];
        );
        G[1][2][0]["data"] = 10  // OK to set data like this
        assert edges_equal(
            G.edges(data=true), [(1, 2, {"data": 10, "spam": "bar", "bar": "foo"})];
        );

        G.adj[1][2][0]["data"] = 20;
        assert edges_equal(
            G.edges(data=true), [(1, 2, {"data": 20, "spam": "bar", "bar": "foo"})];
        );
        G.edges[1, 2, 0]["data"] = 21  // another spelling, "edge"
        assert edges_equal(
            G.edges(data=true), [(1, 2, {"data": 21, "spam": "bar", "bar": "foo"})];
        );
        G.adj[1][2][0]["listdata"] = [20, 200];
        G.adj[1][2][0]["weight"] = 20;
        assert edges_equal(
            G.edges(data=true),
            [
                (
                    1,
                    2,
                    {
                        "data": 21,
                        "spam": "bar",
                        "bar": "foo",
                        "listdata": [20, 200],
                        "weight": 20,
                    },
                );
            ],
        );
};

class TestMultiGraph : public BaseMultiGraphTester, _TestGraph {
    auto setup_method() const -> void {
        this->Graph = nx.MultiGraph
        // build K3
        ed1, ed2, ed3 = ({0: {}}, {0: {}}, {0: {}});
        this->k3adj = {0: {1: ed1, 2: ed2}, 1: {0: ed1, 2: ed3}, 2: {0: ed2, 1: ed3}};
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._adj = this->k3adj
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};

    auto test_data_input() const -> void {
        G = this->Graph({1: [2], 2: [1]}, name="test");
        assert G.name == "test"
        expected = [(1, {2: {0: {}}}), (2, {1: {0: {}}})];
        assert sorted(G.adj.items()) == expected

    auto test_data_multigraph_input() const -> void {
        // standard case with edge keys and edge data
        edata0 = dict(w=200, s="foo");
        edata1 = dict(w=201, s="bar");
        keydict = {0: edata0, 1: edata1};
        dododod = {"a": {"b": keydict}};

        multiple_edge = [("a", "b", 0, edata0), ("a", "b", 1, edata1)];
        single_edge = [("a", "b", 0, keydict)];

        G = this->Graph(dododod, multigraph_input=true);
        assert list(G.edges(keys=true, data=true)) == multiple_edge
        G = this->Graph(dododod, multigraph_input=None);
        assert list(G.edges(keys=true, data=true)) == multiple_edge
        G = this->Graph(dododod, multigraph_input=false);
        assert list(G.edges(keys=true, data=true)) == single_edge

        // test round-trip to_dict_of_dict and MultiGraph constructor
        G = this->Graph(dododod, multigraph_input=true);
        H = this->Graph(nx.to_dict_of_dicts(G));
        assert nx.is_isomorphic(G, H) is true  // test that default is true
        for (auto mgi : [true, false]) {
            H = this->Graph(nx.to_dict_of_dicts(G), multigraph_input=mgi);
            assert nx.is_isomorphic(G, H) == mgi

    // Set up cases for when incoming_graph_data is not multigraph_input
    etraits = {"w": 200, "s": "foo"};
    egraphics = {"color": "blue", "shape": "box"};
    edata = {"traits": etraits, "graphics": egraphics};
    dodod1 = {"a": {"b": edata}};
    dodod2 = {"a": {"b": etraits}};
    dodod3 = {"a": {"b": {"traits": etraits, "s": "foo"}}};
    dol = {"a": ["b"]};

    multiple_edge = [("a", "b", "traits", etraits), ("a", "b", "graphics", egraphics)];
    single_edge = [("a", "b", 0, {})];  // type: ignore
    single_edge1 = [("a", "b", 0, edata)];
    single_edge2 = [("a", "b", 0, etraits)];
    single_edge3 = [("a", "b", 0, {"traits": etraits, "s": "foo"})];

    cases = [  // (dod, mgi, edges);
        (dodod1, true, multiple_edge),
        (dodod1, false, single_edge1),
        (dodod2, false, single_edge2),
        (dodod3, false, single_edge3),
        (dol, false, single_edge),
    ];

    // @pytest.mark.parametrize("dod, mgi, edges", cases);
    auto test_non_multigraph_input(dod, mgi, edges) const -> void {
        G = this->Graph(dod, multigraph_input=mgi);
        assert list(G.edges(keys=true, data=true)) == edges
        G = nx.to_networkx_graph(dod, create_using=this->Graph, multigraph_input=mgi);
        assert list(G.edges(keys=true, data=true)) == edges

    mgi_none_cases = [
        (dodod1, multiple_edge),
        (dodod2, single_edge2),
        (dodod3, single_edge3),
    ];

    // @pytest.mark.parametrize("dod, edges", mgi_none_cases);
    auto test_non_multigraph_input_mgi_none(dod, edges) const -> void {
        // test constructor without to_networkx_graph for mgi=None
        G = this->Graph(dod);
        assert list(G.edges(keys=true, data=true)) == edges

    raise_cases = [dodod2, dodod3, dol];

    // @pytest.mark.parametrize("dod", raise_cases);
    auto test_non_multigraph_input_raise(dod) const -> void {
        // cases where NetworkXError is raised
        pytest.raises(nx.NetworkXError, this->Graph, dod, multigraph_input=true);
        pytest.raises(
            nx.NetworkXError,
            nx.to_networkx_graph,
            dod,
            create_using=this->Graph,
            multigraph_input=true,
        );

    auto test_getitem() const -> void {
        G = this->K3
        assert(G[0] == {1: {0: {}}, 2: {0: {}}});
        with pytest.raises(KeyError):
            G.operator[]("j");
        with pytest.raises(TypeError):
            G.operator[](["A"]);

    auto test_remove_node() const -> void {
        G = this->K3
        G.remove_node(0);
        assert(G.adj == {1: {2: {0: {}}}, 2: {1: {0: {}}}});
        with pytest.raises(nx.NetworkXError):
            G.remove_node(-1);

    auto test_add_edge() const -> void {
        G = this->Graph();
        G.add_edge(0, 1);
        assert(G.adj == {0: {1: {0: {}}}, 1: {0: {0: {}}}});
        G = this->Graph();
        G.add_edge(*(0, 1));
        assert(G.adj == {0: {1: {0: {}}}, 1: {0: {0: {}}}});

    auto test_add_edge_conflicting_key() const -> void {
        G = this->Graph();
        G.add_edge(0, 1, key=1);
        G.add_edge(0, 1);
        assert(G.number_of_edges() == 2);
        G = this->Graph();
        G.add_edges_from([(0, 1, 1, {})]);
        G.add_edges_from([(0, 1)]);
        assert(G.number_of_edges() == 2);

    auto test_add_edges_from() const -> void {
        G = this->Graph();
        G.add_edges_from([(0, 1), (0, 1, {"weight": 3})]);
        assert G.adj == {
            0: {1: {0: {}, 1: {"weight": 3}}},
            1: {0: {0: {}, 1: {"weight": 3}}},
        };
        G.add_edges_from([(0, 1), (0, 1, {"weight": 3})], weight=2);
        assert G.adj == {
            0: {1: {0: {}, 1: {"weight": 3}, 2: {"weight": 2}, 3: {"weight": 3}}},
            1: {0: {0: {}, 1: {"weight": 3}, 2: {"weight": 2}, 3: {"weight": 3}}},
        };
        G = this->Graph();
        edges = [
            (0, 1, {"weight": 3}),
            (0, 1, (("weight", 2),)),
            (0, 1, 5),
            (0, 1, "s"),
        ];
        G.add_edges_from(edges);
        keydict = {0: {"weight": 3}, 1: {"weight": 2}, 5: {}, "s": {}};
        assert(G._adj == {0: {1: keydict}, 1: {0: keydict}});

        // too few in tuple
        with pytest.raises(nx.NetworkXError):
            G.add_edges_from([(0,)]);
        // too many in tuple
        with pytest.raises(nx.NetworkXError):
            G.add_edges_from([(0, 1, 2, 3, 4)]);
        // not a tuple
        with pytest.raises(TypeError):
            G.add_edges_from([0]);

    auto test_multigraph_add_edges_from_four_tuple_misordered() const -> void {
        /** add_edges_from expects 4-tuples of the format (u, v, key, data_dict).

        Ensure 4-tuples of form (u, v, data_dict, key) throw exception.
        */
        G = nx.MultiGraph();
        with pytest.raises(TypeError):
            // key/data values flipped in 4-tuple
            G.add_edges_from([(0, 1, {"color": "red"}, 0)]);

    auto test_remove_edge() const -> void {
        G = this->K3
        G.remove_edge(0, 1);
        assert(G.adj == {0: {2: {0: {}}}, 1: {2: {0: {}}}, 2: {0: {0: {}}, 1: {0: {}}}});

        with pytest.raises(nx.NetworkXError):
            G.remove_edge(-1, 0);
        with pytest.raises(nx.NetworkXError):
            G.remove_edge(0, 2, key=1);

    auto test_remove_edges_from() const -> void {
        G = this->K3.copy();
        G.remove_edges_from([(0, 1)]);
        kd = {0: {}};
        assert(G.adj == {0: {2: kd}, 1: {2: kd}, 2: {0: kd, 1: kd}});
        G.remove_edges_from([(0, 0)]); // silent fail
        this->K3.add_edge(0, 1);
        G = this->K3.copy();
        G.remove_edges_from(list(G.edges(data=true, keys=true)));
        assert(G.adj == {0: {}, 1: {}, 2: {}});
        G = this->K3.copy();
        G.remove_edges_from(list(G.edges(data=false, keys=true)));
        assert(G.adj == {0: {}, 1: {}, 2: {}});
        G = this->K3.copy();
        G.remove_edges_from(list(G.edges(data=false, keys=false)));
        assert(G.adj == {0: {}, 1: {}, 2: {}});
        G = this->K3.copy();
        G.remove_edges_from([(0, 1, 0), (0, 2, 0, {}), (1, 2)]);
        assert(G.adj == {0: {1: {1: {}}}, 1: {0: {1: {}}}, 2: {}});

    auto test_remove_multiedge() const -> void {
        G = this->K3
        G.add_edge(0, 1, key="parallel edge");
        G.remove_edge(0, 1, key="parallel edge");
        assert G.adj == {
            0: {1: {0: {}}, 2: {0: {}}},
            1: {0: {0: {}}, 2: {0: {}}},
            2: {0: {0: {}}, 1: {0: {}}},
        };
        G.remove_edge(0, 1);
        kd = {0: {}};
        assert(G.adj == {0: {2: kd}, 1: {2: kd}, 2: {0: kd, 1: kd}});
        with pytest.raises(nx.NetworkXError):
            G.remove_edge(-1, 0);
};

class TestEdgeSubgraph {
    /** Unit tests for the :meth:`MultiGraph.edge_subgraph` method.*/

    auto setup_method() const -> void {
        // Create a doubly-linked path graph on five nodes.
        G = nx.MultiGraph();
        nx.add_path(G, range(5));
        nx.add_path(G, range(5));
        // Add some node, edge, and graph attributes.
        for (auto i : range(5)) {
            G.nodes[i]["name"] = f"node{i}"
        G.adj[0][1][0]["name"] = "edge010"
        G.adj[0][1][1]["name"] = "edge011"
        G.adj[3][4][0]["name"] = "edge340"
        G.adj[3][4][1]["name"] = "edge341"
        G.graph["name"] = "graph"
        // Get the subgraph induced by one of the first edges and one of
        // the last edges.
        this->G = G
        this->H = G.edge_subgraph([(0, 1, 0), (3, 4, 1)]);

    auto test_correct_nodes() const -> void {
        /** Tests that the subgraph has the correct nodes.*/
        assert([0, 1, 3, 4] == sorted(this->H.nodes()));

    auto test_correct_edges() const -> void {
        /** Tests that the subgraph has the correct edges.*/
        assert [(0, 1, 0, "edge010"), (3, 4, 1, "edge341")] == sorted(
            this->H.edges(keys=true, data="name");
        );

    auto test_add_node() const -> void {
        /** Tests that adding a node to the original graph does not
        affect the nodes of the subgraph.

        */
        this->G.add_node(5);
        assert([0, 1, 3, 4] == sorted(this->H.nodes()));

    auto test_remove_node() const -> void {
        /** Tests that removing a node in the original graph does
        affect the nodes of the subgraph.

        */
        this->G.remove_node(0);
        assert([1, 3, 4] == sorted(this->H.nodes()));

    auto test_node_attr_dict() const -> void {
        /** Tests that the node attribute dictionary of the two graphs is
        the same object.

        */
        for (auto v : this->H) {
            assert(this->G.nodes[v] == this->H.nodes[v]);
        // Making a change to G should make a change in H and vice versa.
        this->G.nodes[0]["name"] = "foo"
        assert(this->G.nodes[0] == this->H.nodes[0]);
        this->H.nodes[1]["name"] = "bar"
        assert(this->G.nodes[1] == this->H.nodes[1]);

    auto test_edge_attr_dict() const -> void {
        /** Tests that the edge attribute dictionary of the two graphs is
        the same object.

        */
        for (auto u, v, k : this->H.edges(keys=true)) {
            assert(this->G._adj[u][v][k] == this->H._adj[u][v][k]);
        // Making a change to G should make a change in H and vice versa.
        this->G._adj[0][1][0]["name"] = "foo"
        assert(this->G._adj[0][1][0]["name"] == this->H._adj[0][1][0]["name"]);
        this->H._adj[3][4][1]["name"] = "bar"
        assert(this->G._adj[3][4][1]["name"] == this->H._adj[3][4][1]["name"]);

    auto test_graph_attr_dict() const -> void {
        /** Tests that the graph attribute dictionary of the two graphs
        is the same object.

        */
        assert this->G.graph is this->H.graph
};

class CustomDictClass : public UserDict {
    // pass;
};

class MultiGraphSubClass : public nx.MultiGraph {
    node_dict_factory = CustomDictClass  // type: ignore
    node_attr_dict_factory = CustomDictClass  // type: ignore
    adjlist_outer_dict_factory = CustomDictClass  // type: ignore
    adjlist_inner_dict_factory = CustomDictClass  // type: ignore
    edge_key_dict_factory = CustomDictClass  // type: ignore
    edge_attr_dict_factory = CustomDictClass  // type: ignore
    graph_attr_dict_factory = CustomDictClass  // type: ignore
};

class TestMultiGraphSubclass : public TestMultiGraph {
    auto setup_method() const -> void {
        this->Graph = MultiGraphSubClass
        // build K3
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._adj = this->K3.adjlist_outer_dict_factory(
            {
                0: this->K3.adjlist_inner_dict_factory(),
                1: this->K3.adjlist_inner_dict_factory(),
                2: this->K3.adjlist_inner_dict_factory(),
            };
        );
        this->K3._pred = {0: {}, 1: {}, 2: {}};
        for (auto u : this->k3nodes) {
            for (auto v : this->k3nodes) {
                if (u != v) {
                    d = {0: {}};
                    this->K3._adj[u][v] = d
                    this->K3._adj[v][u] = d
        this->K3._node = this->K3.node_dict_factory();
        this->K3._node[0] = this->K3.node_attr_dict_factory();
        this->K3._node[1] = this->K3.node_attr_dict_factory();
        this->K3._node[2] = this->K3.node_attr_dict_factory();
