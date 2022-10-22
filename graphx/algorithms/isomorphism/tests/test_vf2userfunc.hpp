/**
    Tests for VF2 isomorphism algorithm for weighted graphs.
*/

// import math
// from operator import eq

// import graphx as nx
// import graphx.algorithms.isomorphism as iso


auto test_simple() -> void {
    // 16 simple tests
    w = "weight"
    edges = [(0, 0, 1), (0, 0, 1.5), (0, 1, 2), (1, 0, 3)];
    for (auto g1 : [nx.Graph(), nx.DiGraph(), nx.MultiGraph(), nx.MultiDiGraph()]) {

        g1.add_weighted_edges_from(edges);
        g2 = g1.subgraph(g1.nodes());
        if (g1.is_multigraph()) {
            em = iso.numerical_multiedge_match("weight", 1);
        } else {
            em = iso.numerical_edge_match("weight", 1);
        assert(nx.is_isomorphic(g1, g2, edge_match=em));

        for (auto mod1, mod2 : [(false, true), (true, false), (true, true)]) {
            // mod1 tests a regular edge
            // mod2 tests a selfloop
            if (g2.is_multigraph()) {
                if (mod1) {
                    data1 = {0: {"weight": 10}};
                if (mod2) {
                    data2 = {0: {"weight": 1}, 1: {"weight": 2.5}};
            } else {
                if (mod1) {
                    data1 = {"weight": 10};
                if (mod2) {
                    data2 = {"weight": 2.5};

            g2 = g1.subgraph(g1.nodes()).copy();
            if (mod1) {
                if (!g1.is_directed()) {
                    g2._adj[1][0] = data1
                    g2._adj[0][1] = data1
                } else {
                    g2._succ[1][0] = data1
                    g2._pred[0][1] = data1
            if (mod2) {
                if (!g1.is_directed()) {
                    g2._adj[0][0] = data2
                } else {
                    g2._succ[0][0] = data2
                    g2._pred[0][0] = data2

            assert(!nx.is_isomorphic(g1, g2, edge_match=em));
}

auto test_weightkey() -> void {
    g1 = nx.DiGraph();
    g2 = nx.DiGraph();

    g1.add_edge("A", "B", weight=1);
    g2.add_edge("C", "D", weight=0);

    assert(nx.is_isomorphic(g1, g2));
    em = iso.numerical_edge_match("nonexistent attribute", 1);
    assert(nx.is_isomorphic(g1, g2, edge_match=em));
    em = iso.numerical_edge_match("weight", 1);
    assert(!nx.is_isomorphic(g1, g2, edge_match=em));

    g2 = nx.DiGraph();
    g2.add_edge("C", "D");
    assert(nx.is_isomorphic(g1, g2, edge_match=em));
}

class TestNodeMatch_Graph {
    auto setup_method() const -> void {
        this->g1 = nx.Graph();
        this->g2 = nx.Graph();
        this->build();

    auto build() const -> void {
        this->nm = iso.categorical_node_match("color", "");
        this->em = iso.numerical_edge_match("weight", 1);

        this->g1.add_node("A", color="red");
        this->g2.add_node("C", color="blue");

        this->g1.add_edge("A", "B", weight=1);
        this->g2.add_edge("C", "D", weight=1);

    auto test_noweight_nocolor() const -> void {
        assert(nx.is_isomorphic(this->g1, this->g2));

    auto test_color1() const -> void {
        assert(!nx.is_isomorphic(this->g1, this->g2, node_match=this->nm));

    auto test_color2() const -> void {
        this->g1.nodes["A"]["color"] = "blue"
        assert(nx.is_isomorphic(this->g1, this->g2, node_match=this->nm));

    auto test_weight1() const -> void {
        assert(nx.is_isomorphic(this->g1, this->g2, edge_match=this->em));

    auto test_weight2() const -> void {
        this->g1.add_edge("A", "B", weight=2);
        assert(!nx.is_isomorphic(this->g1, this->g2, edge_match=this->em));

    auto test_colorsandweights1() const -> void {
        iso = nx.is_isomorphic(this->g1, this->g2, node_match=this->nm, edge_match=this->em);
        assert not iso

    auto test_colorsandweights2() const -> void {
        this->g1.nodes["A"]["color"] = "blue"
        iso = nx.is_isomorphic(this->g1, this->g2, node_match=this->nm, edge_match=this->em);
        assert iso

    auto test_colorsandweights3() const -> void {
        // make the weights disagree
        this->g1.add_edge("A", "B", weight=2);
        assert not nx.is_isomorphic(
            this->g1, this->g2, node_match=this->nm, edge_match=this->em
        );
}

class TestEdgeMatch_MultiGraph {
    auto setup_method() const -> void {
        this->g1 = nx.MultiGraph();
        this->g2 = nx.MultiGraph();
        this->GM = iso.MultiGraphMatcher
        this->build();

    auto build() const -> void {
        g1 = this->g1
        g2 = this->g2

        // We will assume integer weights only.
        g1.add_edge("A", "B", color="green", weight=0, size=0.5);
        g1.add_edge("A", "B", color="red", weight=1, size=0.35);
        g1.add_edge("A", "B", color="red", weight=2, size=0.65);

        g2.add_edge("C", "D", color="green", weight=1, size=0.5);
        g2.add_edge("C", "D", color="red", weight=0, size=0.45);
        g2.add_edge("C", "D", color="red", weight=2, size=0.65);

        if (g1.is_multigraph()) {
            this->em = iso.numerical_multiedge_match("weight", 1);
            this->emc = iso.categorical_multiedge_match("color", "");
            this->emcm = iso.categorical_multiedge_match(["color", "weight"], ["", 1]);
            this->emg1 = iso.generic_multiedge_match("color", "red", eq);
            this->emg2 = iso.generic_multiedge_match(
                ["color", "weight", "size"],
                ["red", 1, 0.5],
                [eq, eq, math.isclose],
            );
        } else {
            this->em = iso.numerical_edge_match("weight", 1);
            this->emc = iso.categorical_edge_match("color", "");
            this->emcm = iso.categorical_edge_match(["color", "weight"], ["", 1]);
            this->emg1 = iso.generic_multiedge_match("color", "red", eq);
            this->emg2 = iso.generic_edge_match(
                ["color", "weight", "size"],
                ["red", 1, 0.5],
                [eq, eq, math.isclose],
            );

    auto test_weights_only() const -> void {
        assert(nx.is_isomorphic(this->g1, this->g2, edge_match=this->em));

    auto test_colors_only() const -> void {
        gm = this->GM(this->g1, this->g2, edge_match=this->emc);
        assert(gm.is_isomorphic());

    auto test_colorsandweights() const -> void {
        gm = this->GM(this->g1, this->g2, edge_match=this->emcm);
        assert(!gm.is_isomorphic());

    auto test_generic1() const -> void {
        gm = this->GM(this->g1, this->g2, edge_match=this->emg1);
        assert(gm.is_isomorphic());

    auto test_generic2() const -> void {
        gm = this->GM(this->g1, this->g2, edge_match=this->emg2);
        assert(!gm.is_isomorphic());
}

class TestEdgeMatch_DiGraph : public TestNodeMatch_Graph {
    auto setup_method() const -> void {
        TestNodeMatch_Graph.setup_method(*this);
        this->g1 = nx.DiGraph();
        this->g2 = nx.DiGraph();
        this->build();
};

class TestEdgeMatch_MultiDiGraph : public TestEdgeMatch_MultiGraph {
    auto setup_method() const -> void {
        TestEdgeMatch_MultiGraph.setup_method(*this);
        this->g1 = nx.MultiDiGraph();
        this->g2 = nx.MultiDiGraph();
        this->GM = iso.MultiDiGraphMatcher
        this->build();
