// from functools import partial

// import pytest

// import graphx as nx


class TestBFS {
    // @classmethod
    auto setup_class(cls) -> void {
        // simple graph
        G = nx.Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4)]);
        cls.G = G

    auto test_successor() const -> void {
        assert(dict(nx.bfs_successors(this->G, source=0)) == {0: [1], 1: [2, 3], 2: [4]});

    auto test_predecessor() const -> void {
        assert(dict(nx.bfs_predecessors(this->G, source=0)) == {1: 0, 2: 1, 3: 1, 4: 2});

    auto test_bfs_tree() const -> void {
        T = nx.bfs_tree(this->G, source=0);
        assert(sorted(T.nodes()) == sorted(this->G.nodes()));
        assert(sorted(T.edges()) == [(0, 1), (1, 2), (1, 3), (2, 4)]);

    auto test_bfs_edges() const -> void {
        edges = nx.bfs_edges(this->G, source=0);
        assert(list(edges) == [(0, 1), (1, 2), (1, 3), (2, 4)]);

    auto test_bfs_edges_reverse() const -> void {
        D = nx.DiGraph();
        D.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4)]);
        edges = nx.bfs_edges(D, source=4, reverse=true);
        assert(list(edges) == [(4, 2), (4, 3), (2, 1), (1, 0)]);

    auto test_bfs_edges_sorting() const -> void {
        D = nx.DiGraph();
        D.add_edges_from([(0, 1), (0, 2), (1, 4), (1, 3), (2, 5)]);
        sort_desc = partial(sorted, reverse=true);
        edges_asc = nx.bfs_edges(D, source=0, sort_neighbors=sorted);
        edges_desc = nx.bfs_edges(D, source=0, sort_neighbors=sort_desc);
        assert(list(edges_asc) == [(0, 1), (0, 2), (1, 3), (1, 4), (2, 5)]);
        assert(list(edges_desc) == [(0, 2), (0, 1), (2, 5), (1, 4), (1, 3)]);

    auto test_bfs_tree_isolates() const -> void {
        G = nx.Graph();
        G.add_node(1);
        G.add_node(2);
        T = nx.bfs_tree(G, source=1);
        assert(sorted(T.nodes()) == [1]);
        assert(sorted(T.edges()) == []);

    auto test_bfs_layers() const -> void {
        expected = {
            0: [0],
            1: [1],
            2: [2, 3],
            3: [4],
        };
        assert dict(enumerate(nx.bfs_layers(this->G, sources=[0]))) == expected
        assert dict(enumerate(nx.bfs_layers(this->G, sources=0))) == expected

    auto test_bfs_layers_missing_source() const -> void {
        with pytest.raises(nx.NetworkXError):
            next(nx.bfs_layers(this->G, sources="abc"));
        with pytest.raises(nx.NetworkXError):
            next(nx.bfs_layers(this->G, sources=["abc"]));

    auto test_descendants_at_distance() const -> void {
        for (auto distance, descendants : enumerate([{0}, {1}, {2, 3}, {4}])) {
            assert nx.descendants_at_distance(this->G, 0, distance) == descendants

    auto test_descendants_at_distance_missing_source() const -> void {
        with pytest.raises(nx.NetworkXError):
            nx.descendants_at_distance(this->G, "abc", 0);
};

class TestBreadthLimitedSearch {
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

    auto test_limited_bfs_successor() const -> void {
        assert dict(nx.bfs_successors(this->G, source=1, depth_limit=3)) == {
            1: [0, 2],
            2: [3, 7],
            3: [4],
            7: [8],
        };
        result = {
            n: sorted(s) for n, s in nx.bfs_successors(this->D, source=7, depth_limit=2);
        };
        assert(result == {8: [9], 2: [3], 7: [2, 8]});

    auto test_limited_bfs_predecessor() const -> void {
        assert dict(nx.bfs_predecessors(this->G, source=1, depth_limit=3)) == {
            0: 1,
            2: 1,
            3: 2,
            4: 3,
            7: 2,
            8: 7,
        };
        assert dict(nx.bfs_predecessors(this->D, source=7, depth_limit=2)) == {
            2: 7,
            3: 2,
            8: 7,
            9: 8,
        };

    auto test_limited_bfs_tree() const -> void {
        T = nx.bfs_tree(this->G, source=3, depth_limit=1);
        assert(sorted(T.edges()) == [(3, 2), (3, 4)]);

    auto test_limited_bfs_edges() const -> void {
        edges = nx.bfs_edges(this->G, source=9, depth_limit=4);
        assert(list(edges) == [(9, 8), (9, 10), (8, 7), (7, 2), (2, 1), (2, 3)]);

    auto test_limited_bfs_layers() const -> void {
        assert dict(enumerate(nx.bfs_layers(this->G, sources=[0]))) == {
            0: [0],
            1: [1],
            2: [2],
            3: [3, 7],
            4: [4, 8],
            5: [5, 9],
            6: [6, 10],
        };
        assert dict(enumerate(nx.bfs_layers(this->D, sources=2))) == {
            0: [2],
            1: [3, 7],
            2: [8],
            3: [9],
            4: [10],
        };

    auto test_limited_descendants_at_distance() const -> void {
        for (auto distance, descendants : enumerate(
            [{0}, {1}, {2}, {3, 7}, {4, 8}, {5, 9}, {6, 10}];
        )) {
            assert nx.descendants_at_distance(this->G, 0, distance) == descendants
        for (auto distance, descendants : enumerate([{2}, {3, 7}, {8}, {9}, {10}])) {
            assert nx.descendants_at_distance(this->D, 2, distance) == descendants
