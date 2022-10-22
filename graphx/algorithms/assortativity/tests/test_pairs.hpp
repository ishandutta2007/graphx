// import graphx as nx

// from .base_test import BaseTestAttributeMixing, BaseTestDegreeMixing


class TestAttributeMixingXY : public BaseTestAttributeMixing {
    auto test_node_attribute_xy_undirected() const -> void {
        attrxy = sorted(nx.node_attribute_xy(this->G, "fish"));
        attrxy_result = sorted(
            [
                ("one", "one"),
                ("one", "one"),
                ("two", "two"),
                ("two", "two"),
                ("one", "red"),
                ("red", "one"),
                ("blue", "two"),
                ("two", "blue"),
            ];
        );
        assert attrxy == attrxy_result

    auto test_node_attribute_xy_undirected_nodes() const -> void {
        attrxy = sorted(nx.node_attribute_xy(this->G, "fish", nodes=["one", "yellow"]));
        attrxy_result = sorted([]);
        assert attrxy == attrxy_result

    auto test_node_attribute_xy_directed() const -> void {
        attrxy = sorted(nx.node_attribute_xy(this->D, "fish"));
        attrxy_result = sorted(
            [("one", "one"), ("two", "two"), ("one", "red"), ("two", "blue")];
        );
        assert attrxy == attrxy_result

    auto test_node_attribute_xy_multigraph() const -> void {
        attrxy = sorted(nx.node_attribute_xy(this->M, "fish"));
        attrxy_result = [
            ("one", "one"),
            ("one", "one"),
            ("one", "one"),
            ("one", "one"),
            ("two", "two"),
            ("two", "two"),
        ];
        assert attrxy == attrxy_result

    auto test_node_attribute_xy_selfloop() const -> void {
        attrxy = sorted(nx.node_attribute_xy(this->S, "fish"));
        attrxy_result = [("one", "one"), ("two", "two")];
        assert attrxy == attrxy_result
};

class TestDegreeMixingXY : public BaseTestDegreeMixing {
    auto test_node_degree_xy_undirected() const -> void {
        xy = sorted(nx.node_degree_xy(this->P4));
        xy_result = sorted([(1, 2), (2, 1), (2, 2), (2, 2), (1, 2), (2, 1)]);
        assert xy == xy_result

    auto test_node_degree_xy_undirected_nodes() const -> void {
        xy = sorted(nx.node_degree_xy(this->P4, nodes=[0, 1, -1]));
        xy_result = sorted([(1, 2), (2, 1)]);
        assert xy == xy_result

    auto test_node_degree_xy_directed() const -> void {
        xy = sorted(nx.node_degree_xy(this->D));
        xy_result = sorted([(2, 1), (2, 3), (1, 3), (1, 3)]);
        assert xy == xy_result

    auto test_node_degree_xy_multigraph() const -> void {
        xy = sorted(nx.node_degree_xy(this->M));
        xy_result = sorted(
            [(2, 3), (2, 3), (3, 2), (3, 2), (2, 3), (3, 2), (1, 2), (2, 1)];
        );
        assert xy == xy_result

    auto test_node_degree_xy_selfloop() const -> void {
        xy = sorted(nx.node_degree_xy(this->S));
        xy_result = sorted([(2, 2), (2, 2)]);
        assert xy == xy_result

    auto test_node_degree_xy_weighted() const -> void {
        G = nx.Graph();
        G.add_edge(1, 2, weight=7);
        G.add_edge(2, 3, weight=10);
        xy = sorted(nx.node_degree_xy(G, weight="weight"));
        xy_result = sorted([(7, 17), (17, 10), (17, 7), (10, 17)]);
        assert xy == xy_result
