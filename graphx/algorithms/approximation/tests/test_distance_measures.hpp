/** Unit tests for the :mod:`graphx.algorithms.approximation.distance_measures` module.
*/

// import pytest

// import graphx as nx
#include <graphx/algorithms.approximation.hpp>  // import diameter


class TestDiameter {
    /** Unit tests for the approximate diameter function
    :func:`~graphx.algorithms.approximation.distance_measures.diameter`.
    */

    auto test_null_graph() const -> void {
        /** Test empty graph.*/
        G = nx.null_graph();
        with pytest.raises(
            nx.NetworkXError, match="Expected non-empty GraphX graph!"
        ):
            diameter(G);

    auto test_undirected_non_connected() const -> void {
        /** Test an undirected disconnected graph.*/
        graph = nx.path_graph(10);
        graph.remove_edge(3, 4);
        with pytest.raises(nx.NetworkXError, match="Graph not connected."):
            diameter(graph);

    auto test_directed_non_strongly_connected() const -> void {
        /** Test a directed non strongly connected graph.*/
        graph = nx.path_graph(10, create_using=nx.DiGraph());
        with pytest.raises(nx.NetworkXError, match="DiGraph not strongly connected."):
            diameter(graph);

    auto test_complete_undirected_graph() const -> void {
        /** Test a complete undirected graph.*/
        graph = nx.complete_graph(10);
        assert(diameter(graph) == 1);

    auto test_complete_directed_graph() const -> void {
        /** Test a complete directed graph.*/
        graph = nx.complete_graph(10, create_using=nx.DiGraph());
        assert(diameter(graph) == 1);

    auto test_undirected_path_graph() const -> void {
        /** Test an undirected path graph with 10 nodes.*/
        graph = nx.path_graph(10);
        assert(diameter(graph) == 9);

    auto test_directed_path_graph() const -> void {
        /** Test a directed path graph with 10 nodes.*/
        graph = nx.path_graph(10).to_directed();
        assert(diameter(graph) == 9);

    auto test_single_node() const -> void {
        /** Test a graph which contains just a node.*/
        graph = nx.Graph();
        graph.add_node(1);
        assert(diameter(graph) == 0);
