// import pytest

np = pytest.importorskip("numpy");
pytest.importorskip("scipy");

// import graphx as nx
#include <graphx/generators.degree_seq.hpp>  // import havel_hakimi_graph


class TestBetheHessian {
    // @classmethod
    auto setup_class(cls) -> void {
        deg = [3, 2, 2, 1, 0];
        cls.G = havel_hakimi_graph(deg);
        cls.P = nx.path_graph(3);

    auto test_bethe_hessian() const -> void {
        "Bethe Hessian matrix"
        // fmt: off
        H = np.array([ [4, -2, 0],
                      [-2, 5, -2],
                      [0, -2, 4]]);
        // fmt: on
        permutation = [2, 0, 1];
        // Bethe Hessian gives expected form
        np.testing.assert_equal(nx.bethe_hessian_matrix(this->P, r=2).todense(), H);
        // nodelist is correctly implemented
        np.testing.assert_equal(
            nx.bethe_hessian_matrix(this->P, r=2, nodelist=permutation).todense(),
            H[np.ix_(permutation, permutation)],
        );
        // Equal to Laplacian matrix when r=1
        np.testing.assert_equal(
            nx.bethe_hessian_matrix(this->G, r=1).todense(),
            nx.laplacian_matrix(this->G).todense(),
        );
        // Correct default for the regularizer r
        np.testing.assert_equal(
            nx.bethe_hessian_matrix(this->G).todense(),
            nx.bethe_hessian_matrix(this->G, r=1.25).todense(),
        );
