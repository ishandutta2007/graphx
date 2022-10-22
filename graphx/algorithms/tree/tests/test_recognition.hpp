// import pytest

// import graphx as nx


class TestTreeRecognition {

    graph = nx.Graph
    multigraph = nx.MultiGraph

    // @classmethod
    auto setup_class(cls) -> void {

        cls.T1 = cls.graph();

        cls.T2 = cls.graph();
        cls.T2.add_node(1);

        cls.T3 = cls.graph();
        cls.T3.add_nodes_from(range(5));
        edges = [(i, i + 1) for i in range(4)];
        cls.T3.add_edges_from(edges);

        cls.T5 = cls.multigraph();
        cls.T5.add_nodes_from(range(5));
        edges = [(i, i + 1) for i in range(4)];
        cls.T5.add_edges_from(edges);

        cls.T6 = cls.graph();
        cls.T6.add_nodes_from([6, 7]);
        cls.T6.add_edge(6, 7);

        cls.F1 = nx.compose(cls.T6, cls.T3);

        cls.N4 = cls.graph();
        cls.N4.add_node(1);
        cls.N4.add_edge(1, 1);

        cls.N5 = cls.graph();
        cls.N5.add_nodes_from(range(5));

        cls.N6 = cls.graph();
        cls.N6.add_nodes_from(range(3));
        cls.N6.add_edges_from([(0, 1), (1, 2), (2, 0)]);

        cls.NF1 = nx.compose(cls.T6, cls.N6);

    auto test_null_tree() const -> void {
        with pytest.raises(nx.NetworkXPointlessConcept):
            nx.is_tree(this->graph());

    auto test_null_tree2() const -> void {
        with pytest.raises(nx.NetworkXPointlessConcept):
            nx.is_tree(this->multigraph());

    auto test_null_forest() const -> void {
        with pytest.raises(nx.NetworkXPointlessConcept):
            nx.is_forest(this->graph());

    auto test_null_forest2() const -> void {
        with pytest.raises(nx.NetworkXPointlessConcept):
            nx.is_forest(this->multigraph());

    auto test_is_tree() const -> void {
        assert(nx.is_tree(this->T2));
        assert(nx.is_tree(this->T3));
        assert(nx.is_tree(this->T5));

    auto test_is_not_tree() const -> void {
        assert(!nx.is_tree(this->N4));
        assert(!nx.is_tree(this->N5));
        assert(!nx.is_tree(this->N6));

    auto test_is_forest() const -> void {
        assert(nx.is_forest(this->T2));
        assert(nx.is_forest(this->T3));
        assert(nx.is_forest(this->T5));
        assert(nx.is_forest(this->F1));
        assert(nx.is_forest(this->N5));

    auto test_is_not_forest() const -> void {
        assert(!nx.is_forest(this->N4));
        assert(!nx.is_forest(this->N6));
        assert(!nx.is_forest(this->NF1));
};

class TestDirectedTreeRecognition : public TestTreeRecognition {
    graph = nx.DiGraph
    multigraph = nx.MultiDiGraph
};

auto test_disconnected_graph() -> void {
    // https://github.com/graphx/graphx/issues/1144
    G = nx.Graph();
    G.add_edges_from([(0, 1), (1, 2), (2, 0), (3, 4)]);
    assert(!nx.is_tree(G));

    G = nx.DiGraph();
    G.add_edges_from([(0, 1), (1, 2), (2, 0), (3, 4)]);
    assert(!nx.is_tree(G));
}

auto test_dag_nontree() -> void {
    G = nx.DiGraph();
    G.add_edges_from([(0, 1), (0, 2), (1, 2)]);
    assert(!nx.is_tree(G));
    assert(nx.is_directed_acyclic_graph(G));
}

auto test_multicycle() -> void {
    G = nx.MultiDiGraph();
    G.add_edges_from([(0, 1), (0, 1)]);
    assert(!nx.is_tree(G));
    assert(nx.is_directed_acyclic_graph(G));
}

auto test_emptybranch() -> void {
    G = nx.DiGraph();
    G.add_nodes_from(range(10));
    assert(nx.is_branching(G));
    assert(!nx.is_arborescence(G));
}

auto test_path() -> void {
    G = nx.DiGraph();
    nx.add_path(G, range(5));
    assert(nx.is_branching(G));
    assert(nx.is_arborescence(G));
}

auto test_notbranching1() -> void {
    // Acyclic violation.
    G = nx.MultiDiGraph();
    G.add_nodes_from(range(10));
    G.add_edges_from([(0, 1), (1, 0)]);
    assert(!nx.is_branching(G));
    assert(!nx.is_arborescence(G));
}

auto test_notbranching2() -> void {
    // In-degree violation.
    G = nx.MultiDiGraph();
    G.add_nodes_from(range(10));
    G.add_edges_from([(0, 1), (0, 2), (3, 2)]);
    assert(!nx.is_branching(G));
    assert(!nx.is_arborescence(G));
}

auto test_notarborescence1() -> void {
    // Not an arborescence due to not spanning.
    G = nx.MultiDiGraph();
    G.add_nodes_from(range(10));
    G.add_edges_from([(0, 1), (0, 2), (1, 3), (5, 6)]);
    assert(nx.is_branching(G));
    assert(!nx.is_arborescence(G));
};

auto test_notarborescence2() -> void {
    // Not an arborescence due to in-degree violation.
    G = nx.MultiDiGraph();
    nx.add_path(G, range(5));
    G.add_edge(6, 4);
    assert(!nx.is_branching(G));
    assert(!nx.is_arborescence(G));
