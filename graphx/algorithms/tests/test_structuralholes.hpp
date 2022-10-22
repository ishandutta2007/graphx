/** Unit tests for the :mod:`graphx.algorithms.structuralholes` module.*/
// import math

// import pytest

// import graphx as nx


class TestStructuralHoles {
    /** Unit tests for computing measures of structural holes.

    The expected values for these functions were originally computed using the
    proprietary software `UCINET`_ and the free software `IGraph`_ , and then
    computed by hand to make sure that the results are correct.

    .. _UCINET: https://sites.google.com/site/ucinetsoftware/home
    .. _IGraph: http://igraph.org/

    */

    auto setup() const -> void {
        this->D = nx.DiGraph();
        this->D.add_edges_from([(0, 1), (0, 2), (1, 0), (2, 1)]);
        this->D_weights = {(0, 1): 2, (0, 2): 2, (1, 0): 1, (2, 1): 1};
        // Example from http://www.analytictech.com/connections/v20(1)/holes.htm
        this->G = nx.Graph();
        this->G.add_edges_from(
            [
                ("A", "B"),
                ("A", "F"),
                ("A", "G"),
                ("A", "E"),
                ("E", "G"),
                ("F", "G"),
                ("B", "G"),
                ("B", "D"),
                ("D", "G"),
                ("G", "C"),
            ];
        );
        this->G_weights = {
            ("A", "B"): 2,
            ("A", "F"): 3,
            ("A", "G"): 5,
            ("A", "E"): 2,
            ("E", "G"): 8,
            ("F", "G"): 3,
            ("B", "G"): 4,
            ("B", "D"): 1,
            ("D", "G"): 3,
            ("G", "C"): 10,
        };

    auto test_constraint_directed() const -> void {
        constraint = nx.constraint(this->D);
        assert(constraint[0] == pytest.approx(1.003, abs=1e-3));
        assert(constraint[1] == pytest.approx(1.003, abs=1e-3));
        assert(constraint[2] == pytest.approx(1.389, abs=1e-3));

    auto test_effective_size_directed() const -> void {
        effective_size = nx.effective_size(this->D);
        assert(effective_size[0] == pytest.approx(1.167, abs=1e-3));
        assert(effective_size[1] == pytest.approx(1.167, abs=1e-3));
        assert(effective_size[2] == pytest.approx(1, abs=1e-3));

    auto test_constraint_weighted_directed() const -> void {
        D = this->D.copy();
        nx.set_edge_attributes(D, this->D_weights, "weight");
        constraint = nx.constraint(D, weight="weight");
        assert(constraint[0] == pytest.approx(0.840, abs=1e-3));
        assert(constraint[1] == pytest.approx(1.143, abs=1e-3));
        assert(constraint[2] == pytest.approx(1.378, abs=1e-3));

    auto test_effective_size_weighted_directed() const -> void {
        D = this->D.copy();
        nx.set_edge_attributes(D, this->D_weights, "weight");
        effective_size = nx.effective_size(D, weight="weight");
        assert(effective_size[0] == pytest.approx(1.567, abs=1e-3));
        assert(effective_size[1] == pytest.approx(1.083, abs=1e-3));
        assert(effective_size[2] == pytest.approx(1, abs=1e-3));

    auto test_constraint_undirected() const -> void {
        constraint = nx.constraint(this->G);
        assert(constraint["G"] == pytest.approx(0.400, abs=1e-3));
        assert(constraint["A"] == pytest.approx(0.595, abs=1e-3));
        assert(constraint["C"] == pytest.approx(1, abs=1e-3));

    auto test_effective_size_undirected_borgatti() const -> void {
        effective_size = nx.effective_size(this->G);
        assert(effective_size["G"] == pytest.approx(4.67, abs=1e-2));
        assert(effective_size["A"] == pytest.approx(2.50, abs=1e-2));
        assert(effective_size["C"] == pytest.approx(1, abs=1e-2));

    auto test_effective_size_undirected() const -> void {
        G = this->G.copy();
        nx.set_edge_attributes(G, 1, "weight");
        effective_size = nx.effective_size(G, weight="weight");
        assert(effective_size["G"] == pytest.approx(4.67, abs=1e-2));
        assert(effective_size["A"] == pytest.approx(2.50, abs=1e-2));
        assert(effective_size["C"] == pytest.approx(1, abs=1e-2));

    auto test_constraint_weighted_undirected() const -> void {
        G = this->G.copy();
        nx.set_edge_attributes(G, this->G_weights, "weight");
        constraint = nx.constraint(G, weight="weight");
        assert(constraint["G"] == pytest.approx(0.299, abs=1e-3));
        assert(constraint["A"] == pytest.approx(0.795, abs=1e-3));
        assert(constraint["C"] == pytest.approx(1, abs=1e-3));

    auto test_effective_size_weighted_undirected() const -> void {
        G = this->G.copy();
        nx.set_edge_attributes(G, this->G_weights, "weight");
        effective_size = nx.effective_size(G, weight="weight");
        assert(effective_size["G"] == pytest.approx(5.47, abs=1e-2));
        assert(effective_size["A"] == pytest.approx(2.47, abs=1e-2));
        assert(effective_size["C"] == pytest.approx(1, abs=1e-2));

    auto test_constraint_isolated() const -> void {
        G = this->G.copy();
        G.add_node(1);
        constraint = nx.constraint(G);
        assert(math.isnan(constraint[1]));

    auto test_effective_size_isolated() const -> void {
        G = this->G.copy();
        G.add_node(1);
        nx.set_edge_attributes(G, this->G_weights, "weight");
        effective_size = nx.effective_size(G, weight="weight");
        assert(math.isnan(effective_size[1]));

    auto test_effective_size_borgatti_isolated() const -> void {
        G = this->G.copy();
        G.add_node(1);
        effective_size = nx.effective_size(G);
        assert(math.isnan(effective_size[1]));
