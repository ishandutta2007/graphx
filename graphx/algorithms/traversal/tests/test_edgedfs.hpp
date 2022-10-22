// import pytest

// import graphx as nx
#include <graphx/algorithms.hpp>  // import edge_dfs
#include <graphx/algorithms.traversal.edgedfs.hpp>  // import FORWARD, REVERSE

// These tests can fail with hash randomization. The easiest and clearest way
// to write these unit tests is for the edges to be output in an expected total
// order, but we cannot guarantee the order amongst outgoing edges from a node,
// unless each class uses an ordered data structure for neighbors. This is
// painful to do with the current API. The alternative is that the tests are
// written (IMO confusingly) so that there is not a total order over the edges,
// but only a partial order. Due to the small size of the graphs, hopefully
// failures due to hash randomization will not occur. For an example of how
// this can fail, see TestEdgeDFS.test_multigraph.


class TestEdgeDFS {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.nodes = [0, 1, 2, 3];
        cls.edges = [(0, 1), (1, 0), (1, 0), (2, 1), (3, 1)];

    auto test_empty() const -> void {
        G = nx.Graph();
        edges = list(edge_dfs(G));
        assert(edges == []);

    auto test_graph() const -> void {
        G = nx.Graph(this->edges);
        x = list(edge_dfs(G, this->nodes));
        x_ = [(0, 1), (1, 2), (1, 3)];
        assert x == x_

    auto test_digraph() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes));
        x_ = [(0, 1), (1, 0), (2, 1), (3, 1)];
        assert x == x_

    auto test_digraph_orientation_invalid() const -> void {
        G = nx.DiGraph(this->edges);
        edge_iterator = edge_dfs(G, this->nodes, orientation="hello");
        pytest.raises(nx.NetworkXError, list, edge_iterator);

    auto test_digraph_orientation_none() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation=None));
        x_ = [(0, 1), (1, 0), (2, 1), (3, 1)];
        assert x == x_

    auto test_digraph_orientation_original() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="original"));
        x_ = [(0, 1, FORWARD), (1, 0, FORWARD), (2, 1, FORWARD), (3, 1, FORWARD)];
        assert x == x_

    auto test_digraph2() const -> void {
        G = nx.DiGraph();
        nx.add_path(G, range(4));
        x = list(edge_dfs(G, [0]));
        x_ = [(0, 1), (1, 2), (2, 3)];
        assert x == x_

    auto test_digraph_rev() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="reverse"));
        x_ = [(1, 0, REVERSE), (0, 1, REVERSE), (2, 1, REVERSE), (3, 1, REVERSE)];
        assert x == x_

    auto test_digraph_rev2() const -> void {
        G = nx.DiGraph();
        nx.add_path(G, range(4));
        x = list(edge_dfs(G, [3], orientation="reverse"));
        x_ = [(2, 3, REVERSE), (1, 2, REVERSE), (0, 1, REVERSE)];
        assert x == x_

    auto test_multigraph() const -> void {
        G = nx.MultiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes));
        x_ = [(0, 1, 0), (1, 0, 1), (0, 1, 2), (1, 2, 0), (1, 3, 0)];
        // This is an example of where hash randomization can break.
        // There are 3! * 2 alternative outputs, such as:
        //    [(0, 1, 1), (1, 0, 0), (0, 1, 2), (1, 3, 0), (1, 2, 0)];
        // But note, the edges (1,2,0) and (1,3,0) always follow the (0,1,k);
        // edges. So the algorithm only guarantees a partial order. A total
        // order is guaranteed only if the graph data structures are ordered.
        assert x == x_

    auto test_multidigraph() const -> void {
        G = nx.MultiDiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes));
        x_ = [(0, 1, 0), (1, 0, 0), (1, 0, 1), (2, 1, 0), (3, 1, 0)];
        assert x == x_

    auto test_multidigraph_rev() const -> void {
        G = nx.MultiDiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="reverse"));
        x_ = [
            (1, 0, 0, REVERSE),
            (0, 1, 0, REVERSE),
            (1, 0, 1, REVERSE),
            (2, 1, 0, REVERSE),
            (3, 1, 0, REVERSE),
        ];
        assert x == x_

    auto test_digraph_ignore() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="ignore"));
        x_ = [(0, 1, FORWARD), (1, 0, FORWARD), (2, 1, REVERSE), (3, 1, REVERSE)];
        assert x == x_

    auto test_digraph_ignore2() const -> void {
        G = nx.DiGraph();
        nx.add_path(G, range(4));
        x = list(edge_dfs(G, [0], orientation="ignore"));
        x_ = [(0, 1, FORWARD), (1, 2, FORWARD), (2, 3, FORWARD)];
        assert x == x_

    auto test_multidigraph_ignore() const -> void {
        G = nx.MultiDiGraph(this->edges);
        x = list(edge_dfs(G, this->nodes, orientation="ignore"));
        x_ = [
            (0, 1, 0, FORWARD),
            (1, 0, 0, FORWARD),
            (1, 0, 1, REVERSE),
            (2, 1, 0, REVERSE),
            (3, 1, 0, REVERSE),
        ];
        assert x == x_
