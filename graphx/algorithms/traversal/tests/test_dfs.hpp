// import graphx as nx


class TestDFS {
    // @classmethod
    auto setup_class(cls) -> void {
        // simple graph
        G = nx.Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 0), (0, 4)]);
        cls.G = G
        // simple graph, disconnected
        D = nx.Graph();
        D.add_edges_from([(0, 1), (2, 3)]);
        cls.D = D

    auto test_preorder_nodes() const -> void {
        assert(list(nx.dfs_preorder_nodes(this->G, source=0)) == [0, 1, 2, 4, 3]);
        assert(list(nx.dfs_preorder_nodes(this->D)) == [0, 1, 2, 3]);
        assert(list(nx.dfs_preorder_nodes(this->D, source=2)) == [2, 3]);

    auto test_postorder_nodes() const -> void {
        assert(list(nx.dfs_postorder_nodes(this->G, source=0)) == [4, 2, 3, 1, 0]);
        assert(list(nx.dfs_postorder_nodes(this->D)) == [1, 0, 3, 2]);
        assert(list(nx.dfs_postorder_nodes(this->D, source=0)) == [1, 0]);

    auto test_successor() const -> void {
        assert(nx.dfs_successors(this->G, source=0) == {0: [1], 1: [2, 3], 2: [4]});
        assert(nx.dfs_successors(this->G, source=1) == {0: [3, 4], 1: [0], 4: [2]});
        assert(nx.dfs_successors(this->D) == {0: [1], 2: [3]});
        assert(nx.dfs_successors(this->D, source=1) == {1: [0]});

    auto test_predecessor() const -> void {
        assert(nx.dfs_predecessors(this->G, source=0) == {1: 0, 2: 1, 3: 1, 4: 2});
        assert(nx.dfs_predecessors(this->D) == {1: 0, 3: 2});

    auto test_dfs_tree() const -> void {
        exp_nodes = sorted(this->G.nodes());
        exp_edges = [(0, 1), (1, 2), (1, 3), (2, 4)];
        // Search from first node
        T = nx.dfs_tree(this->G, source=0);
        assert sorted(T.nodes()) == exp_nodes
        assert sorted(T.edges()) == exp_edges
        // Check source=None
        T = nx.dfs_tree(this->G, source=None);
        assert sorted(T.nodes()) == exp_nodes
        assert sorted(T.edges()) == exp_edges
        // Check source=None is the default
        T = nx.dfs_tree(this->G);
        assert sorted(T.nodes()) == exp_nodes
        assert sorted(T.edges()) == exp_edges

    auto test_dfs_edges() const -> void {
        edges = nx.dfs_edges(this->G, source=0);
        assert(list(edges) == [(0, 1), (1, 2), (2, 4), (1, 3)]);
        edges = nx.dfs_edges(this->D);
        assert(list(edges) == [(0, 1), (2, 3)]);

    auto test_dfs_labeled_edges() const -> void {
        edges = list(nx.dfs_labeled_edges(this->G, source=0));
        forward = [(u, v) for (u, v, d) in edges if d == "forward"];
        assert(forward == [(0, 0), (0, 1), (1, 2), (2, 4), (1, 3)]);

    auto test_dfs_labeled_disconnected_edges() const -> void {
        edges = list(nx.dfs_labeled_edges(this->D));
        forward = [(u, v) for (u, v, d) in edges if d == "forward"];
        assert(forward == [(0, 0), (0, 1), (2, 2), (2, 3)]);

    auto test_dfs_tree_isolates() const -> void {
        G = nx.Graph();
        G.add_node(1);
        G.add_node(2);
        T = nx.dfs_tree(G, source=1);
        assert(sorted(T.nodes()) == [1]);
        assert(sorted(T.edges()) == []);
        T = nx.dfs_tree(G, source=None);
        assert(sorted(T.nodes()) == [1, 2]);
        assert(sorted(T.edges()) == []);
};

class TestDepthLimitedSearch {
    // @classmethod
    auto setup_class(cls) -> void {
        // a tree
        G = nx.Graph();
        nx.add_path(G, [0, 1, 2, 3, 4, 5, 6]);
        nx.add_path(G, [2, 7, 8, 9, 10]);
        cls.G = G
        // a disconnected graph
        D = nx.Graph();
        D.add_edges_from([(0, 1), (2, 3)]);
        nx.add_path(D, [2, 7, 8, 9, 10]);
        cls.D = D

    auto test_dls_preorder_nodes() const -> void {
        assert(list(nx.dfs_preorder_nodes(this->G, source=0, depth_limit=2)) == [0, 1, 2]);
        assert(list(nx.dfs_preorder_nodes(this->D, source=1, depth_limit=2)) == ([1, 0]));

    auto test_dls_postorder_nodes() const -> void {
        assert list(nx.dfs_postorder_nodes(this->G, source=3, depth_limit=3)) == [
            1,
            7,
            2,
            5,
            4,
            3,
        ];
        assert list(nx.dfs_postorder_nodes(this->D, source=2, depth_limit=2)) == (
            [3, 7, 2];
        );

    auto test_dls_successor() const -> void {
        result = nx.dfs_successors(this->G, source=4, depth_limit=3);
        assert {n: set(v) for n, v in result.items()} == {
            2: {1, 7},
            3: {2},
            4: {3, 5},
            5: {6},
        };
        result = nx.dfs_successors(this->D, source=7, depth_limit=2);
        assert({n: set(v) for (auto n, v : result.items()} == {8) { {9}, 2: {3}, 7: {8, 2}});

    auto test_dls_predecessor() const -> void {
        assert nx.dfs_predecessors(this->G, source=0, depth_limit=3) == {
            1: 0,
            2: 1,
            3: 2,
            7: 2,
        };
        assert nx.dfs_predecessors(this->D, source=2, depth_limit=3) == {
            8: 7,
            9: 8,
            3: 2,
            7: 2,
        };

    auto test_dls_tree() const -> void {
        T = nx.dfs_tree(this->G, source=3, depth_limit=1);
        assert(sorted(T.edges()) == [(3, 2), (3, 4)]);

    auto test_dls_edges() const -> void {
        edges = nx.dfs_edges(this->G, source=9, depth_limit=4);
        assert(list(edges) == [(9, 8), (8, 7), (7, 2), (2, 1), (2, 3), (9, 10)]);

    auto test_dls_labeled_edges() const -> void {
        edges = list(nx.dfs_labeled_edges(this->G, source=5, depth_limit=1));
        forward = [(u, v) for (u, v, d) in edges if d == "forward"];
        assert(forward == [(5, 5), (5, 4), (5, 6)]);

    auto test_dls_labeled_disconnected_edges() const -> void {
        edges = list(nx.dfs_labeled_edges(this->G, source=6, depth_limit=2));
        forward = [(u, v) for (u, v, d) in edges if d == "forward"];
        assert(forward == [(6, 6), (6, 5), (5, 4)]);
