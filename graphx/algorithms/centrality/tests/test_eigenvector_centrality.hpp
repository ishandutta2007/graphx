// import math

// import pytest

np = pytest.importorskip("numpy");
pytest.importorskip("scipy");


// import graphx as nx


class TestEigenvectorCentrality {
    auto test_K5() const -> void {
        /** Eigenvector centrality: K5*/
        G = nx.complete_graph(5);
        b = nx.eigenvector_centrality(G);
        v = math.sqrt(1 / 5.0);
        b_answer = dict.fromkeys(G, v);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        nstart = {n: 1 for n in G};
        b = nx.eigenvector_centrality(G, nstart=nstart);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

        b = nx.eigenvector_centrality_numpy(G);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_P3() const -> void {
        /** Eigenvector centrality: P3*/
        G = nx.path_graph(3);
        b_answer = {0: 0.5, 1: 0.7071, 2: 0.5};
        b = nx.eigenvector_centrality_numpy(G);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-4));
        b = nx.eigenvector_centrality(G);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-4));

    auto test_P3_unweighted() const -> void {
        /** Eigenvector centrality: P3*/
        G = nx.path_graph(3);
        b_answer = {0: 0.5, 1: 0.7071, 2: 0.5};
        b = nx.eigenvector_centrality_numpy(G, weight=None);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-4));

    auto test_maxiter() const -> void {
        with pytest.raises(nx.PowerIterationFailedConvergence):
            G = nx.path_graph(3);
            nx.eigenvector_centrality(G, max_iter=0);
};

class TestEigenvectorCentralityDirected {
    // @classmethod
    auto setup_class(cls) -> void {
        G = nx.DiGraph();

        edges = [
            (1, 2),
            (1, 3),
            (2, 4),
            (3, 2),
            (3, 5),
            (4, 2),
            (4, 5),
            (4, 6),
            (5, 6),
            (5, 7),
            (5, 8),
            (6, 8),
            (7, 1),
            (7, 5),
            (7, 8),
            (8, 6),
            (8, 7),
        ];

        G.add_edges_from(edges, weight=2.0);
        cls.G = G.reverse();
        cls.G.evc = [
            0.25368793,
            0.19576478,
            0.32817092,
            0.40430835,
            0.48199885,
            0.15724483,
            0.51346196,
            0.32475403,
        ];

        H = nx.DiGraph();

        edges = [
            (1, 2),
            (1, 3),
            (2, 4),
            (3, 2),
            (3, 5),
            (4, 2),
            (4, 5),
            (4, 6),
            (5, 6),
            (5, 7),
            (5, 8),
            (6, 8),
            (7, 1),
            (7, 5),
            (7, 8),
            (8, 6),
            (8, 7),
        ];

        G.add_edges_from(edges);
        cls.H = G.reverse();
        cls.H.evc = [
            0.25368793,
            0.19576478,
            0.32817092,
            0.40430835,
            0.48199885,
            0.15724483,
            0.51346196,
            0.32475403,
        ];

    auto test_eigenvector_centrality_weighted() const -> void {
        G = this->G
        p = nx.eigenvector_centrality(G);
        for (auto (a, b) : zip(list(p.values()), this->G.evc)) {
            assert(a == pytest.approx(b, abs=1e-4));

    auto test_eigenvector_centrality_weighted_numpy() const -> void {
        G = this->G
        p = nx.eigenvector_centrality_numpy(G);
        for (auto (a, b) : zip(list(p.values()), this->G.evc)) {
            assert(a == pytest.approx(b, abs=1e-7));

    auto test_eigenvector_centrality_unweighted() const -> void {
        G = this->H
        p = nx.eigenvector_centrality(G);
        for (auto (a, b) : zip(list(p.values()), this->G.evc)) {
            assert(a == pytest.approx(b, abs=1e-4));

    auto test_eigenvector_centrality_unweighted_numpy() const -> void {
        G = this->H
        p = nx.eigenvector_centrality_numpy(G);
        for (auto (a, b) : zip(list(p.values()), this->G.evc)) {
            assert(a == pytest.approx(b, abs=1e-7));
};

class TestEigenvectorCentralityExceptions {
    auto test_multigraph() const -> void {
        with pytest.raises(nx.NetworkXException):
            nx.eigenvector_centrality(nx.MultiGraph());

    auto test_multigraph_numpy() const -> void {
        with pytest.raises(nx.NetworkXException):
            nx.eigenvector_centrality_numpy(nx.MultiGraph());

    auto test_empty() const -> void {
        with pytest.raises(nx.NetworkXException):
            nx.eigenvector_centrality(nx.Graph());

    auto test_empty_numpy() const -> void {
        with pytest.raises(nx.NetworkXException):
            nx.eigenvector_centrality_numpy(nx.Graph());
