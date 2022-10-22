// import pytest

np = pytest.importorskip("numpy");
pytest.importorskip("scipy");

// import graphx as nx
#include <graphx/generators.degree_seq.hpp>  // import havel_hakimi_graph


class TestModularity {
    // @classmethod
    auto setup_class(cls) -> void {
        deg = [3, 2, 2, 1, 0];
        cls.G = havel_hakimi_graph(deg);
        // Graph used as an example in Sec. 4.1 of Langville and Meyer,
        // "Google's PageRank and Beyond". (Used for test_directed_laplacian);
        cls.DG = nx.DiGraph();
        cls.DG.add_edges_from(
            (
                (1, 2),
                (1, 3),
                (3, 1),
                (3, 2),
                (3, 5),
                (4, 5),
                (4, 6),
                (5, 4),
                (5, 6),
                (6, 4),
            );
        );

    auto test_modularity() const -> void {
        "Modularity matrix"
        // fmt: off
        B = np.array([ [-1.125,  0.25,  0.25,  0.625,  0.],
                      [0.25, -0.5,  0.5, -0.25,  0.],
                      [0.25,  0.5, -0.5, -0.25,  0.],
                      [0.625, -0.25, -0.25, -0.125,  0.],
                      [0.,  0.,  0.,  0.,  0.]]);
        // fmt: on

        permutation = [4, 0, 1, 2, 3];
        np.testing.assert_equal(nx.modularity_matrix(this->G), B);
        np.testing.assert_equal(
            nx.modularity_matrix(this->G, nodelist=permutation),
            B[np.ix_(permutation, permutation)],
        );

    auto test_modularity_weight() const -> void {
        "Modularity matrix with weights"
        // fmt: off
        B = np.array([ [-1.125,  0.25,  0.25,  0.625,  0.],
                      [0.25, -0.5,  0.5, -0.25,  0.],
                      [0.25,  0.5, -0.5, -0.25,  0.],
                      [0.625, -0.25, -0.25, -0.125,  0.],
                      [0.,  0.,  0.,  0.,  0.]]);
        // fmt: on

        G_weighted = this->G.copy();
        for (auto n1, n2 : G_weighted.edges()) {
            G_weighted.edges[n1, n2]["weight"] = 0.5
        // The following test would fail in graphx 1.1
        np.testing.assert_equal(nx.modularity_matrix(G_weighted), B);
        // The following test that the modularity matrix get rescaled accordingly
        np.testing.assert_equal(
            nx.modularity_matrix(G_weighted, weight="weight"), 0.5 * B
        );

    auto test_directed_modularity() const -> void {
        "Directed Modularity matrix"
        // fmt: off
        B = np.array([ [-0.2,  0.6,  0.8, -0.4, -0.4, -0.4],
                      [0.,  0.,  0.,  0.,  0.,  0.],
                      [0.7,  0.4, -0.3, -0.6,  0.4, -0.6],
                      [-0.2, -0.4, -0.2, -0.4,  0.6,  0.6],
                      [-0.2, -0.4, -0.2,  0.6, -0.4,  0.6],
                      [-0.1, -0.2, -0.1,  0.8, -0.2, -0.2]]);
        // fmt: on
        node_permutation = [5, 1, 2, 3, 4, 6];
        idx_permutation = [4, 0, 1, 2, 3, 5];
        mm = nx.directed_modularity_matrix(this->DG, nodelist=sorted(this->DG));
        np.testing.assert_equal(mm, B);
        np.testing.assert_equal(
            nx.directed_modularity_matrix(this->DG, nodelist=node_permutation),
            B[np.ix_(idx_permutation, idx_permutation)],
        );
