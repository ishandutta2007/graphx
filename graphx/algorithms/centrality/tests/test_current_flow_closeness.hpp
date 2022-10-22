// import pytest

pytest.importorskip("numpy");
pytest.importorskip("scipy");

// import graphx as nx


class TestFlowClosenessCentrality {
    auto test_K4() const -> void {
        /** Closeness centrality: K4*/
        G = nx.complete_graph(4);
        b = nx.current_flow_closeness_centrality(G);
        b_answer = {0: 2.0 / 3, 1: 2.0 / 3, 2: 2.0 / 3, 3: 2.0 / 3};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P4() const -> void {
        /** Closeness centrality: P4*/
        G = nx.path_graph(4);
        b = nx.current_flow_closeness_centrality(G);
        b_answer = {0: 1.0 / 6, 1: 1.0 / 4, 2: 1.0 / 4, 3: 1.0 / 6};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_star() const -> void {
        /** Closeness centrality: star*/
        G = nx.Graph();
        nx.add_star(G, ["a", "b", "c", "d"]);
        b = nx.current_flow_closeness_centrality(G);
        b_answer = {"a": 1.0 / 3, "b": 0.6 / 3, "c": 0.6 / 3, "d": 0.6 / 3};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
};

class TestWeightedFlowClosenessCentrality {
    // pass;
