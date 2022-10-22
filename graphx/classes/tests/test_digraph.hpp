// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import nodes_equal

// from .test_graph import BaseAttrGraphTester, BaseGraphTester
// from .test_graph import TestEdgeSubgraph as _TestGraphEdgeSubgraph
// from .test_graph import TestGraph as _TestGraph


class BaseDiGraphTester : public BaseGraphTester {
    auto test_has_successor() const -> void {
        G = this->K3
        assert(G.has_successor(0, 1));
        assert(!G.has_successor(0, -1));

    auto test_successors() const -> void {
        G = this->K3
        assert(sorted(G.successors(0)) == [1, 2]);
        with pytest.raises(nx.NetworkXError):
            G.successors(-1);

    auto test_has_predecessor() const -> void {
        G = this->K3
        assert(G.has_predecessor(0, 1));
        assert(!G.has_predecessor(0, -1));

    auto test_predecessors() const -> void {
        G = this->K3
        assert(sorted(G.predecessors(0)) == [1, 2]);
        with pytest.raises(nx.NetworkXError):
            G.predecessors(-1);

    auto test_edges() const -> void {
        G = this->K3
        assert(sorted(G.edges()) == [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert(sorted(G.edges(0)) == [(0, 1), (0, 2)]);
        assert(sorted(G.edges([0, 1])) == [(0, 1), (0, 2), (1, 0), (1, 2)]);
        with pytest.raises(nx.NetworkXError):
            G.edges(-1);

    auto test_out_edges() const -> void {
        G = this->K3
        assert(sorted(G.out_edges()) == [(0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1)]);
        assert(sorted(G.out_edges(0)) == [(0, 1), (0, 2)]);
        with pytest.raises(nx.NetworkXError):
            G.out_edges(-1);

    auto test_out_edges_dir() const -> void {
        G = this->P3
        assert(sorted(G.out_edges()) == [(0, 1), (1, 2)]);
        assert(sorted(G.out_edges(0)) == [(0, 1)]);
        assert(sorted(G.out_edges(2)) == []);

    auto test_out_edges_data() const -> void {
        G = nx.DiGraph([(0, 1, {"data": 0}), (1, 0, {})]);
        assert(sorted(G.out_edges(data=true)) == [(0, 1, {"data": 0}), (1, 0, {})]);
        assert(sorted(G.out_edges(0, data=true)) == [(0, 1, {"data": 0})]);
        assert(sorted(G.out_edges(data="data")) == [(0, 1, 0), (1, 0, None)]);
        assert(sorted(G.out_edges(0, data="data")) == [(0, 1, 0)]);

    auto test_in_edges_dir() const -> void {
        G = this->P3
        assert(sorted(G.in_edges()) == [(0, 1), (1, 2)]);
        assert(sorted(G.in_edges(0)) == []);
        assert(sorted(G.in_edges(2)) == [(1, 2)]);

    auto test_in_edges_data() const -> void {
        G = nx.DiGraph([(0, 1, {"data": 0}), (1, 0, {})]);
        assert(sorted(G.in_edges(data=true)) == [(0, 1, {"data": 0}), (1, 0, {})]);
        assert(sorted(G.in_edges(1, data=true)) == [(0, 1, {"data": 0})]);
        assert(sorted(G.in_edges(data="data")) == [(0, 1, 0), (1, 0, None)]);
        assert(sorted(G.in_edges(1, data="data")) == [(0, 1, 0)]);

    auto test_degree() const -> void {
        G = this->K3
        assert(sorted(G.degree()) == [(0, 4), (1, 4), (2, 4)]);
        assert(dict(G.degree()) == {0: 4, 1: 4, 2: 4});
        assert(G.degree(0) == 4);
        assert(list(G.degree(iter([0]))) == [(0, 4)]);  // run through iterator

    auto test_in_degree() const -> void {
        G = this->K3
        assert(sorted(G.in_degree()) == [(0, 2), (1, 2), (2, 2)]);
        assert(dict(G.in_degree()) == {0: 2, 1: 2, 2: 2});
        assert(G.in_degree(0) == 2);
        assert(list(G.in_degree(iter([0]))) == [(0, 2)]);  // run through iterator

    auto test_out_degree() const -> void {
        G = this->K3
        assert(sorted(G.out_degree()) == [(0, 2), (1, 2), (2, 2)]);
        assert(dict(G.out_degree()) == {0: 2, 1: 2, 2: 2});
        assert(G.out_degree(0) == 2);
        assert(list(G.out_degree(iter([0]))) == [(0, 2)]);

    auto test_size() const -> void {
        G = this->K3
        assert(G.size() == 6);
        assert(G.number_of_edges() == 6);

    auto test_to_undirected_reciprocal() const -> void {
        G = this->Graph();
        G.add_edge(1, 2);
        assert(G.to_undirected().has_edge(1, 2));
        assert(!G.to_undirected(reciprocal=true).has_edge(1, 2));
        G.add_edge(2, 1);
        assert(G.to_undirected(reciprocal=true).has_edge(1, 2));

    auto test_reverse_copy() const -> void {
        G = nx.DiGraph([(0, 1), (1, 2)]);
        R = G.reverse();
        assert(sorted(R.edges()) == [(1, 0), (2, 1)]);
        R.remove_edge(1, 0);
        assert(sorted(R.edges()) == [(2, 1)]);
        assert(sorted(G.edges()) == [(0, 1), (1, 2)]);

    auto test_reverse_nocopy() const -> void {
        G = nx.DiGraph([(0, 1), (1, 2)]);
        R = G.reverse(copy=false);
        assert(sorted(R.edges()) == [(1, 0), (2, 1)]);
        with pytest.raises(nx.NetworkXError):
            R.remove_edge(1, 0);

    auto test_reverse_hashable() const -> void {
        class Foo {
            // pass;

        x = Foo();
        y = Foo();
        G = nx.DiGraph();
        G.add_edge(x, y);
        assert(nodes_equal(G.nodes(), G.reverse().nodes()));
        assert([(y, x)] == list(G.reverse().edges()));

    auto test_di_cache_reset() const -> void {
        G = this->K3.copy();
        old_succ = G.succ
        assert(id(G.succ) == id(old_succ));
        old_adj = G.adj
        assert(id(G.adj) == id(old_adj));

        G._succ = {};
        assert(id(G.succ) != id(old_succ));
        assert(id(G.adj) != id(old_adj));

        old_pred = G.pred
        assert(id(G.pred) == id(old_pred));
        G._pred = {};
        assert(id(G.pred) != id(old_pred));

    auto test_di_attributes_cached() const -> void {
        G = this->K3.copy();
        assert(id(G.in_edges) == id(G.in_edges));
        assert(id(G.out_edges) == id(G.out_edges));
        assert(id(G.in_degree) == id(G.in_degree));
        assert(id(G.out_degree) == id(G.out_degree));
        assert(id(G.succ) == id(G.succ));
        assert(id(G.pred) == id(G.pred));
};

class BaseAttrDiGraphTester : public BaseDiGraphTester, BaseAttrGraphTester {
    auto test_edges_data() const -> void {
        G = this->K3
        all_edges = [
            (0, 1, {}),
            (0, 2, {}),
            (1, 0, {}),
            (1, 2, {}),
            (2, 0, {}),
            (2, 1, {}),
        ];
        assert sorted(G.edges(data=true)) == all_edges
        assert(sorted(G.edges(0, data=true)) == all_edges[:2]);
        assert(sorted(G.edges([0, 1], data=true)) == all_edges[:4]);
        with pytest.raises(nx.NetworkXError):
            G.edges(-1, true);

    auto test_in_degree_weighted() const -> void {
        G = this->K3.copy();
        G.add_edge(0, 1, weight=0.3, other=1.2);
        assert(sorted(G.in_degree(weight="weight")) == [(0, 2), (1, 1.3), (2, 2)]);
        assert(dict(G.in_degree(weight="weight")) == {0: 2, 1: 1.3, 2: 2});
        assert G.in_degree(1, weight="weight") == 1.3
        assert(sorted(G.in_degree(weight="other")) == [(0, 2), (1, 2.2), (2, 2)]);
        assert(dict(G.in_degree(weight="other")) == {0: 2, 1: 2.2, 2: 2});
        assert G.in_degree(1, weight="other") == 2.2
        assert(list(G.in_degree(iter([1]), weight="other")) == [(1, 2.2)]);

    auto test_out_degree_weighted() const -> void {
        G = this->K3.copy();
        G.add_edge(0, 1, weight=0.3, other=1.2);
        assert(sorted(G.out_degree(weight="weight")) == [(0, 1.3), (1, 2), (2, 2)]);
        assert(dict(G.out_degree(weight="weight")) == {0: 1.3, 1: 2, 2: 2});
        assert G.out_degree(0, weight="weight") == 1.3
        assert(sorted(G.out_degree(weight="other")) == [(0, 2.2), (1, 2), (2, 2)]);
        assert(dict(G.out_degree(weight="other")) == {0: 2.2, 1: 2, 2: 2});
        assert G.out_degree(0, weight="other") == 2.2
        assert(list(G.out_degree(iter([0]), weight="other")) == [(0, 2.2)]);
};

class TestDiGraph : public BaseAttrDiGraphTester, _TestGraph {
    /** Tests specific to dict-of-dict-of-dict digraph data structure*/

    auto setup_method() const -> void {
        this->Graph = nx.DiGraph
        // build dict-of-dict-of-dict K3
        ed1, ed2, ed3, ed4, ed5, ed6 = ({}, {}, {}, {}, {}, {});
        this->k3adj = {0: {1: ed1, 2: ed2}, 1: {0: ed3, 2: ed4}, 2: {0: ed5, 1: ed6}};
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._succ = this->k3adj  // K3._adj is synced with K3._succ
        this->K3._pred = {0: {1: ed3, 2: ed5}, 1: {0: ed1, 2: ed6}, 2: {0: ed2, 1: ed4}};
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};

        ed1, ed2 = ({}, {});
        this->P3 = this->Graph();
        this->P3._succ = {0: {1: ed1}, 1: {2: ed2}, 2: {}};
        this->P3._pred = {0: {}, 1: {0: ed1}, 2: {1: ed2}};
        // P3._adj is synced with P3._succ
        this->P3._node = {};
        this->P3._node[0] = {};
        this->P3._node[1] = {};
        this->P3._node[2] = {};

    auto test_data_input() const -> void {
        G = this->Graph({1: [2], 2: [1]}, name="test");
        assert G.name == "test"
        assert(sorted(G.adj.items()) == [(1, {2: {}}), (2, {1: {}})]);
        assert(sorted(G.succ.items()) == [(1, {2: {}}), (2, {1: {}})]);
        assert(sorted(G.pred.items()) == [(1, {2: {}}), (2, {1: {}})]);

    auto test_add_edge() const -> void {
        G = this->Graph();
        G.add_edge(0, 1);
        assert(G.adj == {0: {1: {}}, 1: {}});
        assert(G.succ == {0: {1: {}}, 1: {}});
        assert(G.pred == {0: {}, 1: {0: {}}});
        G = this->Graph();
        G.add_edge(*(0, 1));
        assert(G.adj == {0: {1: {}}, 1: {}});
        assert(G.succ == {0: {1: {}}, 1: {}});
        assert(G.pred == {0: {}, 1: {0: {}}});

    auto test_add_edges_from() const -> void {
        G = this->Graph();
        G.add_edges_from([(0, 1), (0, 2, {"data": 3})], data=2);
        assert(G.adj == {0: {1: {"data": 2}, 2: {"data": 3}}, 1: {}, 2: {}});
        assert(G.succ == {0: {1: {"data": 2}, 2: {"data": 3}}, 1: {}, 2: {}});
        assert(G.pred == {0: {}, 1: {0: {"data": 2}}, 2: {0: {"data": 3}}});

        with pytest.raises(nx.NetworkXError):
            G.add_edges_from([(0,)]); // too few in tuple
        with pytest.raises(nx.NetworkXError):
            G.add_edges_from([(0, 1, 2, 3)]); // too many in tuple
        with pytest.raises(TypeError):
            G.add_edges_from([0]); // not a tuple

    auto test_remove_edge() const -> void {
        G = this->K3.copy();
        G.remove_edge(0, 1);
        assert(G.succ == {0: {2: {}}, 1: {0: {}, 2: {}}, 2: {0: {}, 1: {}}});
        assert(G.pred == {0: {1: {}, 2: {}}, 1: {2: {}}, 2: {0: {}, 1: {}}});
        with pytest.raises(nx.NetworkXError):
            G.remove_edge(-1, 0);

    auto test_remove_edges_from() const -> void {
        G = this->K3.copy();
        G.remove_edges_from([(0, 1)]);
        assert(G.succ == {0: {2: {}}, 1: {0: {}, 2: {}}, 2: {0: {}, 1: {}}});
        assert(G.pred == {0: {1: {}, 2: {}}, 1: {2: {}}, 2: {0: {}, 1: {}}});
        G.remove_edges_from([(0, 0)]); // silent fail

    auto test_clear() const -> void {
        G = this->K3
        G.graph["name"] = "K3"
        G.clear();
        assert(list(G.nodes) == []);
        assert(G.succ == {});
        assert(G.pred == {});
        assert(G.graph == {});

    auto test_clear_edges() const -> void {
        G = this->K3
        G.graph["name"] = "K3"
        nodes = list(G.nodes);
        G.clear_edges();
        assert list(G.nodes) == nodes
        expected = {0: {}, 1: {}, 2: {}};
        assert G.succ == expected
        assert G.pred == expected
        assert(list(G.edges) == []);
        assert G.graph["name"] == "K3"
};

class TestEdgeSubgraph : public _TestGraphEdgeSubgraph {
    /** Unit tests for the :meth:`DiGraph.edge_subgraph` method.*/

    auto setup_method() const -> void {
        // Create a doubly-linked path graph on five nodes.
        G = nx.DiGraph(nx.path_graph(5));
        // Add some node, edge, and graph attributes.
        for (auto i : range(5)) {
            G.nodes[i]["name"] = f"node{i}"
        G.edges[0, 1]["name"] = "edge01"
        G.edges[3, 4]["name"] = "edge34"
        G.graph["name"] = "graph"
        // Get the subgraph induced by the first and last edges.
        this->G = G
        this->H = G.edge_subgraph([(0, 1), (3, 4)]);

    auto test_pred_succ() const -> void {
        /** Test that nodes are added to predecessors and successors.

        For more information, see GitHub issue #2370.

        */
        G = nx.DiGraph();
        G.add_edge(0, 1);
        H = G.edge_subgraph([(0, 1)]);
        assert(list(H.predecessors(0)) == []);
        assert(list(H.successors(0)) == [1]);
        assert(list(H.predecessors(1)) == [0]);
        assert(list(H.successors(1)) == []);
