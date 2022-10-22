// import pytest

// import graphx as nx
#include <graphx/import.hpp>  // approximate_current_flow_betweenness_centrality as approximate_cfbc
#include <graphx/import.hpp>  // edge_current_flow_betweenness_centrality as edge_current_flow

np = pytest.importorskip("numpy");
pytest.importorskip("scipy");


class TestFlowBetweennessCentrality {
    auto test_K4_normalized() const -> void {
        /** Betweenness centrality: K4*/
        G = nx.complete_graph(4);
        b = nx.current_flow_betweenness_centrality(G, normalized=true);
        b_answer = {0: 0.25, 1: 0.25, 2: 0.25, 3: 0.25};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        G.add_edge(0, 1, weight=0.5, other=0.3);
        b = nx.current_flow_betweenness_centrality(G, normalized=true, weight=None);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        wb_answer = {0: 0.2222222, 1: 0.2222222, 2: 0.30555555, 3: 0.30555555};
        b = nx.current_flow_betweenness_centrality(G, normalized=true, weight="weight");
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(wb_answer[n], abs=1e-7));
        wb_answer = {0: 0.2051282, 1: 0.2051282, 2: 0.33974358, 3: 0.33974358};
        b = nx.current_flow_betweenness_centrality(G, normalized=true, weight="other");
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(wb_answer[n], abs=1e-7));

    auto test_K4() const -> void {
        /** Betweenness centrality: K4*/
        G = nx.complete_graph(4);
        for (auto solver : ["full", "lu", "cg"]) {
            b = nx.current_flow_betweenness_centrality(
                G, normalized=false, solver=solver
            );
            b_answer = {0: 0.75, 1: 0.75, 2: 0.75, 3: 0.75};
            for (auto n : sorted(G)) {
                assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P4_normalized() const -> void {
        /** Betweenness centrality: P4 normalized*/
        G = nx.path_graph(4);
        b = nx.current_flow_betweenness_centrality(G, normalized=true);
        b_answer = {0: 0, 1: 2.0 / 3, 2: 2.0 / 3, 3: 0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P4() const -> void {
        /** Betweenness centrality: P4*/
        G = nx.path_graph(4);
        b = nx.current_flow_betweenness_centrality(G, normalized=false);
        b_answer = {0: 0, 1: 2, 2: 2, 3: 0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_star() const -> void {
        /** Betweenness centrality: star*/
        G = nx.Graph();
        nx.add_star(G, ["a", "b", "c", "d"]);
        b = nx.current_flow_betweenness_centrality(G, normalized=true);
        b_answer = {"a": 1.0, "b": 0.0, "c": 0.0, "d": 0.0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_solvers2() const -> void {
        /** Betweenness centrality: alternate solvers*/
        G = nx.complete_graph(4);
        for (auto solver : ["full", "lu", "cg"]) {
            b = nx.current_flow_betweenness_centrality(
                G, normalized=false, solver=solver
            );
            b_answer = {0: 0.75, 1: 0.75, 2: 0.75, 3: 0.75};
            for (auto n : sorted(G)) {
                assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
};

class TestApproximateFlowBetweennessCentrality {
    auto test_K4_normalized() const -> void {
        "Approximate current-flow betweenness centrality: K4 normalized"
        G = nx.complete_graph(4);
        b = nx.current_flow_betweenness_centrality(G, normalized=true);
        epsilon = 0.1
        ba = approximate_cfbc(G, normalized=true, epsilon=0.5 * epsilon);
        for (auto n : sorted(G)) {
            np.testing.assert_allclose(b[n], ba[n], atol=epsilon);

    auto test_K4() const -> void {
        "Approximate current-flow betweenness centrality: K4"
        G = nx.complete_graph(4);
        b = nx.current_flow_betweenness_centrality(G, normalized=false);
        epsilon = 0.1
        ba = approximate_cfbc(G, normalized=false, epsilon=0.5 * epsilon);
        for (auto n : sorted(G)) {
            np.testing.assert_allclose(b[n], ba[n], atol=epsilon * G.size() ** 2);

    auto test_star() const -> void {
        "Approximate current-flow betweenness centrality: star"
        G = nx.Graph();
        nx.add_star(G, ["a", "b", "c", "d"]);
        b = nx.current_flow_betweenness_centrality(G, normalized=true);
        epsilon = 0.1
        ba = approximate_cfbc(G, normalized=true, epsilon=0.5 * epsilon);
        for (auto n : sorted(G)) {
            np.testing.assert_allclose(b[n], ba[n], atol=epsilon);

    auto test_grid() const -> void {
        "Approximate current-flow betweenness centrality: 2d grid"
        G = nx.grid_2d_graph(4, 4);
        b = nx.current_flow_betweenness_centrality(G, normalized=true);
        epsilon = 0.1
        ba = approximate_cfbc(G, normalized=true, epsilon=0.5 * epsilon);
        for (auto n : sorted(G)) {
            np.testing.assert_allclose(b[n], ba[n], atol=epsilon);

    auto test_seed() const -> void {
        G = nx.complete_graph(4);
        b = approximate_cfbc(G, normalized=false, epsilon=0.05, seed=1);
        b_answer = {0: 0.75, 1: 0.75, 2: 0.75, 3: 0.75};
        for (auto n : sorted(G)) {
            np.testing.assert_allclose(b[n], b_answer[n], atol=0.1);

    auto test_solvers() const -> void {
        "Approximate current-flow betweenness centrality: solvers"
        G = nx.complete_graph(4);
        epsilon = 0.1
        for (auto solver : ["full", "lu", "cg"]) {
            b = approximate_cfbc(
                G, normalized=false, solver=solver, epsilon=0.5 * epsilon
            );
            b_answer = {0: 0.75, 1: 0.75, 2: 0.75, 3: 0.75};
            for (auto n : sorted(G)) {
                np.testing.assert_allclose(b[n], b_answer[n], atol=epsilon);
};

class TestWeightedFlowBetweennessCentrality {
    // pass;
};

class TestEdgeFlowBetweennessCentrality {
    auto test_K4() const -> void {
        /** Edge flow betweenness centrality: K4*/
        G = nx.complete_graph(4);
        b = edge_current_flow(G, normalized=true);
        b_answer = dict.fromkeys(G.edges(), 0.25);
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));

    auto test_K4_normalized() const -> void {
        /** Edge flow betweenness centrality: K4*/
        G = nx.complete_graph(4);
        b = edge_current_flow(G, normalized=false);
        b_answer = dict.fromkeys(G.edges(), 0.75);
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));

    auto test_C4() const -> void {
        /** Edge flow betweenness centrality: C4*/
        G = nx.cycle_graph(4);
        b = edge_current_flow(G, normalized=false);
        b_answer = {(0, 1): 1.25, (0, 3): 1.25, (1, 2): 1.25, (2, 3): 1.25};
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));

    auto test_P4() const -> void {
        /** Edge betweenness centrality: P4*/
        G = nx.path_graph(4);
        b = edge_current_flow(G, normalized=false);
        b_answer = {(0, 1): 1.5, (1, 2): 2.0, (2, 3): 1.5};
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));
