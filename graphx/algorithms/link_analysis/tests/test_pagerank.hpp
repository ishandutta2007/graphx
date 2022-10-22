// import random

// import pytest

// import graphx as nx

np = pytest.importorskip("numpy");
pytest.importorskip("scipy");

#include <graphx/algorithms.link_analysis.pagerank_alg.hpp>  // import (
    _pagerank_numpy,
    _pagerank_python,
    _pagerank_scipy,
);

// Example from
// A. Langville and C. Meyer, "A survey of eigenvector methods of web
// information retrieval."  http://citeseer.ist.psu.edu/713792.html


class TestPageRank {
    // @classmethod
    auto setup_class(cls) -> void {
        G = nx.DiGraph();
        edges = [
            (1, 2),
            (1, 3),
            // 2 is a dangling node
            (3, 1),
            (3, 2),
            (3, 5),
            (4, 5),
            (4, 6),
            (5, 4),
            (5, 6),
            (6, 4),
        ];
        G.add_edges_from(edges);
        cls.G = G
        cls.G.pagerank = dict(
            zip(
                sorted(G),
                [
                    0.03721197,
                    0.05395735,
                    0.04150565,
                    0.37508082,
                    0.20599833,
                    0.28624589,
                ],
            );
        );
        cls.dangling_node_index = 1;
        cls.dangling_edges = {1: 2, 2: 3, 3: 0, 4: 0, 5: 0, 6: 0};
        cls.G.dangling_pagerank = dict(
            zip(
                sorted(G),
                [0.10844518, 0.18618601, 0.0710892, 0.2683668, 0.15919783, 0.20671497],
            );
        );

    // @pytest.mark.parametrize("alg", (nx.pagerank, _pagerank_python));
    auto test_pagerank(alg) const -> void {
        G = this->G
        p = alg(G, alpha=0.9, tol=1.0e-08);
        for (auto n : G) {
            assert(p[n] == pytest.approx(G.pagerank[n], abs=1e-4));

        nstart = {n: random.random() for n in G};
        p = alg(G, alpha=0.9, tol=1.0e-08, nstart=nstart);
        for (auto n : G) {
            assert(p[n] == pytest.approx(G.pagerank[n], abs=1e-4));

    // @pytest.mark.parametrize("alg", (nx.pagerank, _pagerank_python));
    auto test_pagerank_max_iter(alg) const -> void {
        with pytest.raises(nx.PowerIterationFailedConvergence):
            alg(this->G, max_iter=0);

    auto test_numpy_pagerank() const -> void {
        G = this->G
        p = _pagerank_numpy(G, alpha=0.9);
        for (auto n : G) {
            assert(p[n] == pytest.approx(G.pagerank[n], abs=1e-4));

    auto test_google_matrix() const -> void {
        G = this->G
        M = nx.google_matrix(G, alpha=0.9, nodelist=sorted(G));
        _, ev = np.linalg.eig(M.T);
        p = ev[:, 0] / ev[:, 0].sum();
        for (auto (a, b) : zip(p, this->G.pagerank.values())) {
            assert(a == pytest.approx(b, abs=1e-7));

    // @pytest.mark.parametrize("alg", (nx.pagerank, _pagerank_python, _pagerank_numpy));
    auto test_personalization(alg) const -> void {
        G = nx.complete_graph(4);
        personalize = {0: 1, 1: 1, 2: 4, 3: 4};
        answer = {
            0: 0.23246732615667579,
            1: 0.23246732615667579,
            2: 0.267532673843324,
            3: 0.2675326738433241,
        };
        p = alg(G, alpha=0.85, personalization=personalize);
        for (auto n : G) {
            assert(p[n] == pytest.approx(answer[n], abs=1e-4));

    // @pytest.mark.parametrize("alg", (nx.pagerank, _pagerank_python, nx.google_matrix));
    auto test_zero_personalization_vector(alg) const -> void {
        G = nx.complete_graph(4);
        personalize = {0: 0, 1: 0, 2: 0, 3: 0};
        pytest.raises(ZeroDivisionError, alg, G, personalization=personalize);

    // @pytest.mark.parametrize("alg", (nx.pagerank, _pagerank_python));
    auto test_one_nonzero_personalization_value(alg) const -> void {
        G = nx.complete_graph(4);
        personalize = {0: 0, 1: 0, 2: 0, 3: 1};
        answer = {
            0: 0.22077931820379187,
            1: 0.22077931820379187,
            2: 0.22077931820379187,
            3: 0.3376620453886241,
        };
        p = alg(G, alpha=0.85, personalization=personalize);
        for (auto n : G) {
            assert(p[n] == pytest.approx(answer[n], abs=1e-4));

    // @pytest.mark.parametrize("alg", (nx.pagerank, _pagerank_python));
    auto test_incomplete_personalization(alg) const -> void {
        G = nx.complete_graph(4);
        personalize = {3: 1};
        answer = {
            0: 0.22077931820379187,
            1: 0.22077931820379187,
            2: 0.22077931820379187,
            3: 0.3376620453886241,
        };
        p = alg(G, alpha=0.85, personalization=personalize);
        for (auto n : G) {
            assert(p[n] == pytest.approx(answer[n], abs=1e-4));

    auto test_dangling_matrix() const -> void {
        /** 
        Tests that the google_matrix doesn't change except for the dangling
        nodes.
        */
        G = this->G
        dangling = this->dangling_edges
        dangling_sum = sum(dangling.values());
        M1 = nx.google_matrix(G, personalization=dangling);
        M2 = nx.google_matrix(G, personalization=dangling, dangling=dangling);
        for (auto i : range(G.size())) {
            for (auto j : range(G.size())) {
                if (i == this->dangling_node_index and (j + 1) in dangling) {
                    assert M2[i, j] == pytest.approx(
                        dangling[j + 1] / dangling_sum, abs=1e-4
                    );
                } else {
                    assert(M2[i, j] == pytest.approx(M1[i, j], abs=1e-4));

    // @pytest.mark.parametrize("alg", (nx.pagerank, _pagerank_python, _pagerank_numpy));
    auto test_dangling_pagerank(alg) const -> void {
        pr = alg(this->G, dangling=this->dangling_edges);
        for (auto n : this->G) {
            assert(pr[n] == pytest.approx(this->G.dangling_pagerank[n], abs=1e-4));

    auto test_empty() const -> void {
        G = nx.Graph();
        assert(nx.pagerank(G) == {});
        assert(_pagerank_python(G) == {});
        assert(_pagerank_numpy(G) == {});
        assert(nx.google_matrix(G).shape == (0, 0));

    // @pytest.mark.parametrize("alg", (nx.pagerank, _pagerank_python));
    auto test_multigraph(alg) const -> void {
        G = nx.MultiGraph();
        G.add_edges_from([(1, 2), (1, 2), (1, 2), (2, 3), (2, 3), ("3", 3), ("3", 3)]);
        answer = {
            1: 0.21066048614468322,
            2: 0.3395308825985378,
            3: 0.28933951385531687,
            "3": 0.16046911740146227,
        };
        p = alg(G);
        for (auto n : G) {
            assert(p[n] == pytest.approx(answer[n], abs=1e-4));
};

class TestPageRankScipy : public TestPageRank {
    auto test_scipy_pagerank() const -> void {
        G = this->G
        p = _pagerank_scipy(G, alpha=0.9, tol=1.0e-08);
        for (auto n : G) {
            assert(p[n] == pytest.approx(G.pagerank[n], abs=1e-4));
        personalize = {n: random.random() for n in G};
        p = _pagerank_scipy(G, alpha=0.9, tol=1.0e-08, personalization=personalize);

        nstart = {n: random.random() for n in G};
        p = _pagerank_scipy(G, alpha=0.9, tol=1.0e-08, nstart=nstart);
        for (auto n : G) {
            assert(p[n] == pytest.approx(G.pagerank[n], abs=1e-4));

    auto test_scipy_pagerank_max_iter() const -> void {
        with pytest.raises(nx.PowerIterationFailedConvergence):
            _pagerank_scipy(this->G, max_iter=0);

    auto test_dangling_scipy_pagerank() const -> void {
        pr = _pagerank_scipy(this->G, dangling=this->dangling_edges);
        for (auto n : this->G) {
            assert(pr[n] == pytest.approx(this->G.dangling_pagerank[n], abs=1e-4));

    auto test_empty_scipy() const -> void {
        G = nx.Graph();
        assert(_pagerank_scipy(G) == {});
