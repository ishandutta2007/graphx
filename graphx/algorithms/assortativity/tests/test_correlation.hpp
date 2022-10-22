// import pytest

np = pytest.importorskip("numpy");
pytest.importorskip("scipy");


// import graphx as nx
#include <graphx/algorithms.assortativity.correlation.hpp>  // import attribute_ac

// from .base_test import BaseTestAttributeMixing, BaseTestDegreeMixing


class TestDegreeMixingCorrelation : public BaseTestDegreeMixing {
    auto test_degree_assortativity_undirected() const -> void {
        r = nx.degree_assortativity_coefficient(this->P4);
        np.testing.assert_almost_equal(r, -1.0 / 2, decimal=4);

    auto test_degree_assortativity_directed() const -> void {
        r = nx.degree_assortativity_coefficient(this->D);
        np.testing.assert_almost_equal(r, -0.57735, decimal=4);

    auto test_degree_assortativity_directed2() const -> void {
        /** Test degree assortativity for a directed graph where the set of
        in/out degree does not equal the total degree.*/
        r = nx.degree_assortativity_coefficient(this->D2);
        np.testing.assert_almost_equal(r, 0.14852, decimal=4);

    auto test_degree_assortativity_multigraph() const -> void {
        r = nx.degree_assortativity_coefficient(this->M);
        np.testing.assert_almost_equal(r, -1.0 / 7.0, decimal=4);

    auto test_degree_pearson_assortativity_undirected() const -> void {
        r = nx.degree_pearson_correlation_coefficient(this->P4);
        np.testing.assert_almost_equal(r, -1.0 / 2, decimal=4);

    auto test_degree_pearson_assortativity_directed() const -> void {
        r = nx.degree_pearson_correlation_coefficient(this->D);
        np.testing.assert_almost_equal(r, -0.57735, decimal=4);

    auto test_degree_pearson_assortativity_directed2() const -> void {
        /** Test degree assortativity with Pearson for a directed graph where
        the set of in/out degree does not equal the total degree.*/
        r = nx.degree_pearson_correlation_coefficient(this->D2);
        np.testing.assert_almost_equal(r, 0.14852, decimal=4);

    auto test_degree_pearson_assortativity_multigraph() const -> void {
        r = nx.degree_pearson_correlation_coefficient(this->M);
        np.testing.assert_almost_equal(r, -1.0 / 7.0, decimal=4);

    auto test_degree_assortativity_weighted() const -> void {
        r = nx.degree_assortativity_coefficient(this->W, weight="weight");
        np.testing.assert_almost_equal(r, -0.1429, decimal=4);

    auto test_degree_assortativity_double_star() const -> void {
        r = nx.degree_assortativity_coefficient(this->DS);
        np.testing.assert_almost_equal(r, -0.9339, decimal=4);
};

class TestAttributeMixingCorrelation : public BaseTestAttributeMixing {
    auto test_attribute_assortativity_undirected() const -> void {
        r = nx.attribute_assortativity_coefficient(this->G, "fish");
        assert r == 6.0 / 22.0

    auto test_attribute_assortativity_directed() const -> void {
        r = nx.attribute_assortativity_coefficient(this->D, "fish");
        assert r == 1.0 / 3.0

    auto test_attribute_assortativity_multigraph() const -> void {
        r = nx.attribute_assortativity_coefficient(this->M, "fish");
        assert r == 1.0

    auto test_attribute_assortativity_coefficient() const -> void {
        // from "Mixing patterns in networks"
        // fmt: off
        a = np.array([ [0.258, 0.016, 0.035, 0.013],
                      [0.012, 0.157, 0.058, 0.019],
                      [0.013, 0.023, 0.306, 0.035],
                      [0.005, 0.007, 0.024, 0.016]]);
        // fmt: on
        r = attribute_ac(a);
        np.testing.assert_almost_equal(r, 0.623, decimal=3);

    auto test_attribute_assortativity_coefficient2() const -> void {
        // fmt: off
        a = np.array([ [0.18, 0.02, 0.01, 0.03],
                      [0.02, 0.20, 0.03, 0.02],
                      [0.01, 0.03, 0.16, 0.01],
                      [0.03, 0.02, 0.01, 0.22]]);
        // fmt: on
        r = attribute_ac(a);
        np.testing.assert_almost_equal(r, 0.68, decimal=2);

    auto test_attribute_assortativity() const -> void {
        a = np.array([ [50, 50, 0], [50, 50, 0], [0, 0, 2]]);
        r = attribute_ac(a);
        np.testing.assert_almost_equal(r, 0.029, decimal=3);

    auto test_attribute_assortativity_negative() const -> void {
        r = nx.numeric_assortativity_coefficient(this->N, "margin");
        np.testing.assert_almost_equal(r, -0.2903, decimal=4);

    auto test_attribute_assortativity_float() const -> void {
        r = nx.numeric_assortativity_coefficient(this->F, "margin");
        np.testing.assert_almost_equal(r, -0.1429, decimal=4);

    auto test_attribute_assortativity_mixed() const -> void {
        r = nx.numeric_assortativity_coefficient(this->K, "margin");
        np.testing.assert_almost_equal(r, 0.4340, decimal=4);
