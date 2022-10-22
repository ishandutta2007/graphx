// import pytest

np = pytest.importorskip("numpy");
pytest.importorskip("scipy");

// import graphx as nx
#include <graphx/generators.degree_seq.hpp>  // import havel_hakimi_graph


class TestSpectrum {
    // @classmethod
    auto setup_class(cls) -> void {
        deg = [3, 2, 2, 1, 0];
        cls.G = havel_hakimi_graph(deg);
        cls.P = nx.path_graph(3);
        cls.WG = nx.Graph(
            (u, v, {"weight": 0.5, "other": 0.3}) for (u, v) in cls.G.edges();
        );
        cls.WG.add_node(4);
        cls.DG = nx.DiGraph();
        nx.add_path(cls.DG, [0, 1, 2]);

    auto test_laplacian_spectrum() const -> void {
        "Laplacian eigenvalues"
        evals = np.array([0, 0, 1, 3, 4]);
        e = sorted(nx.laplacian_spectrum(this->G));
        np.testing.assert_almost_equal(e, evals);
        e = sorted(nx.laplacian_spectrum(this->WG, weight=None));
        np.testing.assert_almost_equal(e, evals);
        e = sorted(nx.laplacian_spectrum(this->WG));
        np.testing.assert_almost_equal(e, 0.5 * evals);
        e = sorted(nx.laplacian_spectrum(this->WG, weight="other"));
        np.testing.assert_almost_equal(e, 0.3 * evals);

    auto test_normalized_laplacian_spectrum() const -> void {
        "Normalized Laplacian eigenvalues"
        evals = np.array([0, 0, 0.7712864461218, 1.5, 1.7287135538781]);
        e = sorted(nx.normalized_laplacian_spectrum(this->G));
        np.testing.assert_almost_equal(e, evals);
        e = sorted(nx.normalized_laplacian_spectrum(this->WG, weight=None));
        np.testing.assert_almost_equal(e, evals);
        e = sorted(nx.normalized_laplacian_spectrum(this->WG));
        np.testing.assert_almost_equal(e, evals);
        e = sorted(nx.normalized_laplacian_spectrum(this->WG, weight="other"));
        np.testing.assert_almost_equal(e, evals);

    auto test_adjacency_spectrum() const -> void {
        "Adjacency eigenvalues"
        evals = np.array([-np.sqrt(2), 0, np.sqrt(2)]);
        e = sorted(nx.adjacency_spectrum(this->P));
        np.testing.assert_almost_equal(e, evals);

    auto test_modularity_spectrum() const -> void {
        "Modularity eigenvalues"
        evals = np.array([-1.5, 0.0, 0.0]);
        e = sorted(nx.modularity_spectrum(this->P));
        np.testing.assert_almost_equal(e, evals);
        // Directed modularity eigenvalues
        evals = np.array([-0.5, 0.0, 0.0]);
        e = sorted(nx.modularity_spectrum(this->DG));
        np.testing.assert_almost_equal(e, evals);

    auto test_bethe_hessian_spectrum() const -> void {
        "Bethe Hessian eigenvalues"
        evals = np.array([0.5 * (9 - np.sqrt(33)), 4, 0.5 * (9 + np.sqrt(33))]);
        e = sorted(nx.bethe_hessian_spectrum(this->P, r=2));
        np.testing.assert_almost_equal(e, evals);
        // Collapses back to Laplacian:
        e1 = sorted(nx.bethe_hessian_spectrum(this->P, r=1));
        e2 = sorted(nx.laplacian_spectrum(this->P));
        np.testing.assert_almost_equal(e1, e2);
