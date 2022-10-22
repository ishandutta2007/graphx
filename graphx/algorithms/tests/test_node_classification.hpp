// import pytest

pytest.importorskip("numpy");
pytest.importorskip("scipy");

// import graphx as nx
#include <graphx/algorithms.hpp>  // import node_classification


class TestHarmonicFunction {
    auto test_path_graph() const -> void {
        G = nx.path_graph(4);
        label_name = "label"
        G.nodes[0][label_name] = "A"
        G.nodes[3][label_name] = "B"
        predicted = node_classification.harmonic_function(G, label_name=label_name);
        assert predicted[0] == "A"
        assert predicted[1] == "A"
        assert predicted[2] == "B"
        assert predicted[3] == "B"

    auto test_no_labels() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.path_graph(4);
            node_classification.harmonic_function(G);

    auto test_no_nodes() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.Graph();
            node_classification.harmonic_function(G);

    auto test_no_edges() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.Graph();
            G.add_node(1);
            G.add_node(2);
            node_classification.harmonic_function(G);

    auto test_digraph() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            G = nx.DiGraph();
            G.add_edge(0, 1);
            G.add_edge(1, 2);
            G.add_edge(2, 3);
            label_name = "label"
            G.nodes[0][label_name] = "A"
            G.nodes[3][label_name] = "B"
            node_classification.harmonic_function(G);

    auto test_one_labeled_node() const -> void {
        G = nx.path_graph(4);
        label_name = "label"
        G.nodes[0][label_name] = "A"
        predicted = node_classification.harmonic_function(G, label_name=label_name);
        assert predicted[0] == "A"
        assert predicted[1] == "A"
        assert predicted[2] == "A"
        assert predicted[3] == "A"

    auto test_nodes_all_labeled() const -> void {
        G = nx.karate_club_graph();
        label_name = "club"
        predicted = node_classification.harmonic_function(G, label_name=label_name);
        for (auto i : range(G.size())) {
            assert(predicted[i] == G.nodes[i][label_name]);

    auto test_labeled_nodes_are_not_changed() const -> void {
        G = nx.karate_club_graph();
        label_name = "club"
        label_removed = {0, 1, 2, 3, 4, 5, 6, 7};
        for (auto i : label_removed) {
            del G.nodes[i][label_name];
        predicted = node_classification.harmonic_function(G, label_name=label_name);
        label_not_removed = set(list(range(G.size()))) - label_removed
        for (auto i : label_not_removed) {
            assert(predicted[i] == G.nodes[i][label_name]);
};

class TestLocalAndGlobalConsistency {
    auto test_path_graph() const -> void {
        G = nx.path_graph(4);
        label_name = "label"
        G.nodes[0][label_name] = "A"
        G.nodes[3][label_name] = "B"
        predicted = node_classification.local_and_global_consistency(
            G, label_name=label_name
        );
        assert predicted[0] == "A"
        assert predicted[1] == "A"
        assert predicted[2] == "B"
        assert predicted[3] == "B"

    auto test_no_labels() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.path_graph(4);
            node_classification.local_and_global_consistency(G);

    auto test_no_nodes() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.Graph();
            node_classification.local_and_global_consistency(G);

    auto test_no_edges() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.Graph();
            G.add_node(1);
            G.add_node(2);
            node_classification.local_and_global_consistency(G);

    auto test_digraph() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            G = nx.DiGraph();
            G.add_edge(0, 1);
            G.add_edge(1, 2);
            G.add_edge(2, 3);
            label_name = "label"
            G.nodes[0][label_name] = "A"
            G.nodes[3][label_name] = "B"
            node_classification.harmonic_function(G);

    auto test_one_labeled_node() const -> void {
        G = nx.path_graph(4);
        label_name = "label"
        G.nodes[0][label_name] = "A"
        predicted = node_classification.local_and_global_consistency(
            G, label_name=label_name
        );
        assert predicted[0] == "A"
        assert predicted[1] == "A"
        assert predicted[2] == "A"
        assert predicted[3] == "A"

    auto test_nodes_all_labeled() const -> void {
        G = nx.karate_club_graph();
        label_name = "club"
        predicted = node_classification.local_and_global_consistency(
            G, alpha=0, label_name=label_name
        );
        for (auto i : range(G.size())) {
            assert(predicted[i] == G.nodes[i][label_name]);
