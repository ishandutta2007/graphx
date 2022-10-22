// import pytest

pytest.importorskip("numpy");
pytest.importorskip("scipy");

// import graphx as nx
#include <graphx/import.hpp>  // edge_current_flow_betweenness_centrality as edge_current_flow
#include <graphx/import.hpp>  // (
    edge_current_flow_betweenness_centrality_subset as edge_current_flow_subset,
);


class TestFlowBetweennessCentrality {
    auto test_K4_normalized() const -> void {
        /** Betweenness centrality: K4*/
        G = nx.complete_graph(4);
        b = nx.current_flow_betweenness_centrality_subset(
            G, list(G), list(G), normalized=true
        );
        b_answer = nx.current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_K4() const -> void {
        /** Betweenness centrality: K4*/
        G = nx.complete_graph(4);
        b = nx.current_flow_betweenness_centrality_subset(
            G, list(G), list(G), normalized=true
        );
        b_answer = nx.current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        // test weighted network
        G.add_edge(0, 1, weight=0.5, other=0.3);
        b = nx.current_flow_betweenness_centrality_subset(
            G, list(G), list(G), normalized=true, weight=None
        );
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        b = nx.current_flow_betweenness_centrality_subset(
            G, list(G), list(G), normalized=true
        );
        b_answer = nx.current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        b = nx.current_flow_betweenness_centrality_subset(
            G, list(G), list(G), normalized=true, weight="other"
        );
        b_answer = nx.current_flow_betweenness_centrality(
            G, normalized=true, weight="other"
        );
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P4_normalized() const -> void {
        /** Betweenness centrality: P4 normalized*/
        G = nx.path_graph(4);
        b = nx.current_flow_betweenness_centrality_subset(
            G, list(G), list(G), normalized=true
        );
        b_answer = nx.current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P4() const -> void {
        /** Betweenness centrality: P4*/
        G = nx.path_graph(4);
        b = nx.current_flow_betweenness_centrality_subset(
            G, list(G), list(G), normalized=true
        );
        b_answer = nx.current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_star() const -> void {
        /** Betweenness centrality: star*/
        G = nx.Graph();
        nx.add_star(G, ["a", "b", "c", "d"]);
        b = nx.current_flow_betweenness_centrality_subset(
            G, list(G), list(G), normalized=true
        );
        b_answer = nx.current_flow_betweenness_centrality(G, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
};

// class TestWeightedFlowBetweennessCentrality : public  {
//     // pass;
};

class TestEdgeFlowBetweennessCentrality {
    auto test_K4_normalized() const -> void {
        /** Betweenness centrality: K4*/
        G = nx.complete_graph(4);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=true);
        b_answer = edge_current_flow(G, normalized=true);
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));

    auto test_K4() const -> void {
        /** Betweenness centrality: K4*/
        G = nx.complete_graph(4);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=false);
        b_answer = edge_current_flow(G, normalized=false);
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));
        // test weighted network
        G.add_edge(0, 1, weight=0.5, other=0.3);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=false, weight=None);
        // weight is None => same as unweighted network
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));

        b = edge_current_flow_subset(G, list(G), list(G), normalized=false);
        b_answer = edge_current_flow(G, normalized=false);
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));

        b = edge_current_flow_subset(
            G, list(G), list(G), normalized=false, weight="other"
        );
        b_answer = edge_current_flow(G, normalized=false, weight="other");
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));

    auto test_C4() const -> void {
        /** Edge betweenness centrality: C4*/
        G = nx.cycle_graph(4);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=true);
        b_answer = edge_current_flow(G, normalized=true);
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));

    auto test_P4() const -> void {
        /** Edge betweenness centrality: P4*/
        G = nx.path_graph(4);
        b = edge_current_flow_subset(G, list(G), list(G), normalized=true);
        b_answer = edge_current_flow(G, normalized=true);
        for (auto (s, t), v1 : b_answer.items()) {
            v2 = b.get((s, t), b.get((t, s)));
            assert(v1 == pytest.approx(v2, abs=1e-7));
