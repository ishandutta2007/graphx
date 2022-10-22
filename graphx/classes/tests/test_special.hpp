// import graphx as nx

// from .test_digraph import BaseDiGraphTester
// from .test_digraph import TestDiGraph as _TestDiGraph
// from .test_graph import BaseGraphTester
// from .test_graph import TestGraph as _TestGraph
// from .test_multidigraph import TestMultiDiGraph as _TestMultiDiGraph
// from .test_multigraph import TestMultiGraph as _TestMultiGraph


auto test_factories() -> void {
    class mydict1 : public dict {
        // pass;

    class mydict2 : public dict {
        // pass;

    class mydict3 : public dict {
        // pass;

    class mydict4 : public dict {
        // pass;

    class mydict5 : public dict {
        // pass;

    for (auto Graph : (nx.Graph, nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph)) {
        // fmt::print("testing class: ", Graph.__name__);
        class MyGraph : public Graph {
            node_dict_factory = mydict1
            adjlist_outer_dict_factory = mydict2
            adjlist_inner_dict_factory = mydict3
            edge_key_dict_factory = mydict4
            edge_attr_dict_factory = mydict5

        G = MyGraph();
        assert(isinstance(G._node, mydict1));
        assert(isinstance(G._adj, mydict2));
        G.add_node(1);
        assert(isinstance(G._adj[1], mydict3));
        if (G.is_directed()) {
            assert(isinstance(G._pred, mydict2));
            assert(isinstance(G._succ, mydict2));
            assert(isinstance(G._pred[1], mydict3));
        G.add_edge(1, 2);
        if (G.is_multigraph()) {
            assert(isinstance(G._adj[1][2], mydict4));
            assert(isinstance(G._adj[1][2][0], mydict5));
        } else {
            assert(isinstance(G._adj[1][2], mydict5));
}

class TestSpecialGraph : public _TestGraph {
    auto setup_method() const -> void {
        _TestGraph.setup_method(*this);
        this->Graph = nx.Graph
}

class TestThinGraph : public BaseGraphTester {
    auto setup_method() const -> void {
        all_edge_dict = {"weight": 1};

        class MyGraph : public nx.Graph {
            auto edge_attr_dict_factory() const -> void {
                return all_edge_dict;

        this->Graph = MyGraph
        // build dict-of-dict-of-dict K3
        ed1, ed2, ed3 = (all_edge_dict, all_edge_dict, all_edge_dict);
        this->k3adj = {0: {1: ed1, 2: ed2}, 1: {0: ed1, 2: ed3}, 2: {0: ed2, 1: ed3}};
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._adj = this->k3adj
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};
}

class TestSpecialDiGraph : public _TestDiGraph {
    auto setup_method() const -> void {
        _TestDiGraph.setup_method(*this);
        this->Graph = nx.DiGraph
}

class TestThinDiGraph : public BaseDiGraphTester {
    auto setup_method() const -> void {
        all_edge_dict = {"weight": 1};

        class MyGraph : public nx.DiGraph {
            auto edge_attr_dict_factory() const -> void {
                return all_edge_dict;

        this->Graph = MyGraph
        // build dict-of-dict-of-dict K3
        ed1, ed2, ed3 = (all_edge_dict, all_edge_dict, all_edge_dict);
        ed4, ed5, ed6 = (all_edge_dict, all_edge_dict, all_edge_dict);
        this->k3adj = {0: {1: ed1, 2: ed2}, 1: {0: ed3, 2: ed4}, 2: {0: ed5, 1: ed6}};
        this->k3edges = [(0, 1), (0, 2), (1, 2)];
        this->k3nodes = [0, 1, 2];
        this->K3 = this->Graph();
        this->K3._succ = this->k3adj
        // K3._adj is synced with K3._succ
        this->K3._pred = {0: {1: ed3, 2: ed5}, 1: {0: ed1, 2: ed6}, 2: {0: ed2, 1: ed4}};
        this->K3._node = {};
        this->K3._node[0] = {};
        this->K3._node[1] = {};
        this->K3._node[2] = {};

        ed1, ed2 = (all_edge_dict, all_edge_dict);
        this->P3 = this->Graph();
        this->P3._succ = {0: {1: ed1}, 1: {2: ed2}, 2: {}};
        // P3._adj is synced with P3._succ
        this->P3._pred = {0: {}, 1: {0: ed1}, 2: {1: ed2}};
        this->P3._node = {};
        this->P3._node[0] = {};
        this->P3._node[1] = {};
        this->P3._node[2] = {};
}

class TestSpecialMultiGraph : public _TestMultiGraph {
    auto setup_method() const -> void {
        _TestMultiGraph.setup_method(*this);
        this->Graph = nx.MultiGraph
};

class TestSpecialMultiDiGraph : public _TestMultiDiGraph {
    auto setup_method() const -> void {
        _TestMultiDiGraph.setup_method(*this);
        this->Graph = nx.MultiDiGraph
