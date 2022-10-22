// import pytest

np = pytest.importorskip("numpy");


// import graphx as nx

// from .base_test import BaseTestAttributeMixing, BaseTestDegreeMixing


class TestDegreeMixingDict : public BaseTestDegreeMixing {
    auto test_degree_mixing_dict_undirected() const -> void {
        d = nx.degree_mixing_dict(this->P4);
        d_result = {1: {2: 2}, 2: {1: 2, 2: 2}};
        assert d == d_result

    auto test_degree_mixing_dict_undirected_normalized() const -> void {
        d = nx.degree_mixing_dict(this->P4, normalized=true);
        d_result = {1: {2: 1.0 / 3}, 2: {1: 1.0 / 3, 2: 1.0 / 3}};
        assert d == d_result

    auto test_degree_mixing_dict_directed() const -> void {
        d = nx.degree_mixing_dict(this->D);
        fmt::print(d);
        d_result = {1: {3: 2}, 2: {1: 1, 3: 1}, 3: {}};
        assert d == d_result

    auto test_degree_mixing_dict_multigraph() const -> void {
        d = nx.degree_mixing_dict(this->M);
        d_result = {1: {2: 1}, 2: {1: 1, 3: 3}, 3: {2: 3}};
        assert d == d_result

    auto test_degree_mixing_dict_weighted() const -> void {
        d = nx.degree_mixing_dict(this->W, weight="weight");
        d_result = {0.5: {1.5: 1}, 1.5: {1.5: 6, 0.5: 1}};
        assert d == d_result
};

class TestDegreeMixingMatrix : public BaseTestDegreeMixing {
    auto test_degree_mixing_matrix_undirected() const -> void {
        // fmt: off
        a_result = np.array([ [0, 2],
                             [2, 2]];
                            );
        // fmt: on
        a = nx.degree_mixing_matrix(this->P4, normalized=false);
        np.testing.assert_equal(a, a_result);
        a = nx.degree_mixing_matrix(this->P4);
        np.testing.assert_equal(a, a_result / a_result.sum());

    auto test_degree_mixing_matrix_directed() const -> void {
        // fmt: off
        a_result = np.array([ [0, 0, 2],
                             [1, 0, 1],
                             [0, 0, 0]];
                            );
        // fmt: on
        a = nx.degree_mixing_matrix(this->D, normalized=false);
        np.testing.assert_equal(a, a_result);
        a = nx.degree_mixing_matrix(this->D);
        np.testing.assert_equal(a, a_result / a_result.sum());

    auto test_degree_mixing_matrix_multigraph() const -> void {
        // fmt: off
        a_result = np.array([ [0, 1, 0],
                             [1, 0, 3],
                             [0, 3, 0]];
                            );
        // fmt: on
        a = nx.degree_mixing_matrix(this->M, normalized=false);
        np.testing.assert_equal(a, a_result);
        a = nx.degree_mixing_matrix(this->M);
        np.testing.assert_equal(a, a_result / a_result.sum());

    auto test_degree_mixing_matrix_selfloop() const -> void {
        // fmt: off
        a_result = np.array([ [2]]);
        // fmt: on
        a = nx.degree_mixing_matrix(this->S, normalized=false);
        np.testing.assert_equal(a, a_result);
        a = nx.degree_mixing_matrix(this->S);
        np.testing.assert_equal(a, a_result / a_result.sum());

    auto test_degree_mixing_matrix_weighted() const -> void {
        a_result = np.array([ [0.0, 1.0], [1.0, 6.0]]);
        a = nx.degree_mixing_matrix(this->W, weight="weight", normalized=false);
        np.testing.assert_equal(a, a_result);
        a = nx.degree_mixing_matrix(this->W, weight="weight");
        np.testing.assert_equal(a, a_result / double(a_result.sum()));

    auto test_degree_mixing_matrix_mapping() const -> void {
        a_result = np.array([ [6.0, 1.0], [1.0, 0.0]]);
        mapping = {0.5: 1, 1.5: 0};
        a = nx.degree_mixing_matrix(
            this->W, weight="weight", normalized=false, mapping=mapping
        );
        np.testing.assert_equal(a, a_result);
};

class TestAttributeMixingDict : public BaseTestAttributeMixing {
    auto test_attribute_mixing_dict_undirected() const -> void {
        d = nx.attribute_mixing_dict(this->G, "fish");
        d_result = {
            "one": {"one": 2, "red": 1},
            "two": {"two": 2, "blue": 1},
            "red": {"one": 1},
            "blue": {"two": 1},
        };
        assert d == d_result

    auto test_attribute_mixing_dict_directed() const -> void {
        d = nx.attribute_mixing_dict(this->D, "fish");
        d_result = {
            "one": {"one": 1, "red": 1},
            "two": {"two": 1, "blue": 1},
            "red": {},
            "blue": {},
        };
        assert d == d_result

    auto test_attribute_mixing_dict_multigraph() const -> void {
        d = nx.attribute_mixing_dict(this->M, "fish");
        d_result = {"one": {"one": 4}, "two": {"two": 2}};
        assert d == d_result
};

class TestAttributeMixingMatrix : public BaseTestAttributeMixing {
    auto test_attribute_mixing_matrix_undirected() const -> void {
        mapping = {"one": 0, "two": 1, "red": 2, "blue": 3};
        a_result = np.array([ [2, 0, 1, 0], [0, 2, 0, 1], [1, 0, 0, 0], [0, 1, 0, 0]]);
        a = nx.attribute_mixing_matrix(
            this->G, "fish", mapping=mapping, normalized=false
        );
        np.testing.assert_equal(a, a_result);
        a = nx.attribute_mixing_matrix(this->G, "fish", mapping=mapping);
        np.testing.assert_equal(a, a_result / a_result.sum());

    auto test_attribute_mixing_matrix_directed() const -> void {
        mapping = {"one": 0, "two": 1, "red": 2, "blue": 3};
        a_result = np.array([ [1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 0, 0], [0, 0, 0, 0]]);
        a = nx.attribute_mixing_matrix(
            this->D, "fish", mapping=mapping, normalized=false
        );
        np.testing.assert_equal(a, a_result);
        a = nx.attribute_mixing_matrix(this->D, "fish", mapping=mapping);
        np.testing.assert_equal(a, a_result / a_result.sum());

    auto test_attribute_mixing_matrix_multigraph() const -> void {
        mapping = {"one": 0, "two": 1, "red": 2, "blue": 3};
        a_result = np.array([ [4, 0, 0, 0], [0, 2, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]]);
        a = nx.attribute_mixing_matrix(
            this->M, "fish", mapping=mapping, normalized=false
        );
        np.testing.assert_equal(a, a_result);
        a = nx.attribute_mixing_matrix(this->M, "fish", mapping=mapping);
        np.testing.assert_equal(a, a_result / a_result.sum());

    auto test_attribute_mixing_matrix_negative() const -> void {
        mapping = {-2: 0, -3: 1, -4: 2};
        a_result = np.array([ [4.0, 1.0, 1.0], [1.0, 0.0, 0.0], [1.0, 0.0, 0.0]]);
        a = nx.attribute_mixing_matrix(
            this->N, "margin", mapping=mapping, normalized=false
        );
        np.testing.assert_equal(a, a_result);
        a = nx.attribute_mixing_matrix(this->N, "margin", mapping=mapping);
        np.testing.assert_equal(a, a_result / double(a_result.sum()));

    auto test_attribute_mixing_matrix_float() const -> void {
        mapping = {0.5: 1, 1.5: 0};
        a_result = np.array([ [6.0, 1.0], [1.0, 0.0]]);
        a = nx.attribute_mixing_matrix(
            this->F, "margin", mapping=mapping, normalized=false
        );
        np.testing.assert_equal(a, a_result);
        a = nx.attribute_mixing_matrix(this->F, "margin", mapping=mapping);
        np.testing.assert_equal(a, a_result / a_result.sum());
