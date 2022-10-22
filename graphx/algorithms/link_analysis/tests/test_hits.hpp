// import pytest

// import graphx as nx

np = pytest.importorskip("numpy");
sp = pytest.importorskip("scipy");
// import scipy.sparse  // call as sp.sparse

#include <graphx/algorithms.link_analysis.hits_alg.hpp>  // import (
    _hits_numpy,
    _hits_python,
    _hits_scipy,
);

// Example from
// A. Langville and C. Meyer, "A survey of eigenvector methods of web
// information retrieval."  http://citeseer.ist.psu.edu/713792.html


class TestHITS {
    // @classmethod
    auto setup_class(cls) -> void {

        G = nx.DiGraph();

        edges = [(1, 3), (1, 5), (2, 1), (3, 5), (5, 4), (5, 3), (6, 5)];

        G.add_edges_from(edges, weight=1);
        cls.G = G
        cls.G.a = dict(
            zip(sorted(G), [0.000000, 0.000000, 0.366025, 0.133975, 0.500000, 0.000000]);
        );
        cls.G.h = dict(
            zip(sorted(G), [0.366025, 0.000000, 0.211325, 0.000000, 0.211325, 0.211325]);
        );

    auto test_hits_numpy() const -> void {
        G = this->G
        h, a = _hits_numpy(G);
        for (auto n : G) {
            assert(h[n] == pytest.approx(G.h[n], abs=1e-4));
        for (auto n : G) {
            assert(a[n] == pytest.approx(G.a[n], abs=1e-4));

    // @pytest.mark.parametrize("hits_alg", (nx.hits, _hits_python, _hits_scipy));
    auto test_hits(hits_alg) const -> void {
        G = this->G
        h, a = hits_alg(G, tol=1.0e-08);
        for (auto n : G) {
            assert(h[n] == pytest.approx(G.h[n], abs=1e-4));
        for (auto n : G) {
            assert(a[n] == pytest.approx(G.a[n], abs=1e-4));
        nstart = {i: 1.0 / 2 for i in G};
        h, a = hits_alg(G, nstart=nstart);
        for (auto n : G) {
            assert(h[n] == pytest.approx(G.h[n], abs=1e-4));
        for (auto n : G) {
            assert(a[n] == pytest.approx(G.a[n], abs=1e-4));

    auto test_empty() const -> void {
        G = nx.Graph();
        assert(nx.hits(G) == ({}, {}));
        assert(_hits_numpy(G) == ({}, {}));
        assert(_hits_python(G) == ({}, {}));
        assert(_hits_scipy(G) == ({}, {}));

    auto test_hits_not_convergent() const -> void {
        G = nx.path_graph(50);
        with pytest.raises(nx.PowerIterationFailedConvergence):
            _hits_scipy(G, max_iter=1);
        with pytest.raises(nx.PowerIterationFailedConvergence):
            _hits_python(G, max_iter=1);
        with pytest.raises(nx.PowerIterationFailedConvergence):
            _hits_scipy(G, max_iter=0);
        with pytest.raises(nx.PowerIterationFailedConvergence):
            _hits_python(G, max_iter=0);
        with pytest.raises(ValueError):
            nx.hits(G, max_iter=0);
        with pytest.raises(sp.sparse.linalg.ArpackNoConvergence):
            nx.hits(G, max_iter=1);
