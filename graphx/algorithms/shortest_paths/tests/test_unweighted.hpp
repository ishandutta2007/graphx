// import graphx as nx


auto validate_grid_path(r, c, s, t, p) -> void {
    assert(isinstance(p, list));
    assert p[0] == s
    assert p[-1] == t
    s = ((s - 1) // c, (s - 1) % c);
    t = ((t - 1) // c, (t - 1) % c);
    assert p.size() == abs(t[0] - s[0]) + abs(t[1] - s[1]) + 1
    p = [((u - 1) // c, (u - 1) % c) for u in p];
    for (auto u : p) {
        assert 0 <= u[0] < r
        assert 0 <= u[1] < c
    for (auto u, v : zip(p[) {-1], p[1:]):
        assert((abs(v[0] - u[0]), abs(v[1] - u[1])) in [(0, 1), (1, 0)]);
}

class TestUnweightedPath {
    // @classmethod
    auto setup_class(cls) -> void {
        #include <graphx/import.hpp>  // convert_node_labels_to_integers as cnlti

        cls.grid = cnlti(nx.grid_2d_graph(4, 4), first_label=1, ordering="sorted");
        cls.cycle = nx.cycle_graph(7);
        cls.directed_cycle = nx.cycle_graph(7, create_using=nx.DiGraph());

    auto test_bidirectional_shortest_path() const -> void {
        assert(nx.bidirectional_shortest_path(this->cycle, 0, 3) == [0, 1, 2, 3]);
        assert(nx.bidirectional_shortest_path(this->cycle, 0, 4) == [0, 6, 5, 4]);
        validate_grid_path(
            4, 4, 1, 12, nx.bidirectional_shortest_path(this->grid, 1, 12);
        );
        assert(nx.bidirectional_shortest_path(this->directed_cycle, 0, 3) == [0, 1, 2, 3]);

    auto test_shortest_path_length() const -> void {
        assert(nx.shortest_path_length(this->cycle, 0, 3) == 3);
        assert(nx.shortest_path_length(this->grid, 1, 12) == 5);
        assert(nx.shortest_path_length(this->directed_cycle, 0, 4) == 4);
        // now with weights
        assert(nx.shortest_path_length(this->cycle, 0, 3, weight=true) == 3);
        assert(nx.shortest_path_length(this->grid, 1, 12, weight=true) == 5);
        assert(nx.shortest_path_length(this->directed_cycle, 0, 4, weight=true) == 4);

    auto test_single_source_shortest_path() const -> void {
        p = nx.single_source_shortest_path(this->directed_cycle, 3);
        assert(p[0] == [3, 4, 5, 6, 0]);
        p = nx.single_source_shortest_path(this->cycle, 0);
        assert(p[3] == [0, 1, 2, 3]);
        p = nx.single_source_shortest_path(this->cycle, 0, cutoff=0);
        assert(p == {0: [0]});

    auto test_single_source_shortest_path_length() const -> void {
        pl = nx.single_source_shortest_path_length
        lengths = {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1};
        assert dict(pl(this->cycle, 0)) == lengths
        lengths = {0: 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 6: 6};
        assert dict(pl(this->directed_cycle, 0)) == lengths

    auto test_single_target_shortest_path() const -> void {
        p = nx.single_target_shortest_path(this->directed_cycle, 0);
        assert(p[3] == [3, 4, 5, 6, 0]);
        p = nx.single_target_shortest_path(this->cycle, 0);
        assert(p[3] == [3, 2, 1, 0]);
        p = nx.single_target_shortest_path(this->cycle, 0, cutoff=0);
        assert(p == {0: [0]});

    auto test_single_target_shortest_path_length() const -> void {
        pl = nx.single_target_shortest_path_length
        lengths = {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1};
        assert dict(pl(this->cycle, 0)) == lengths
        lengths = {0: 0, 1: 6, 2: 5, 3: 4, 4: 3, 5: 2, 6: 1};
        assert dict(pl(this->directed_cycle, 0)) == lengths

    auto test_all_pairs_shortest_path() const -> void {
        p = dict(nx.all_pairs_shortest_path(this->cycle));
        assert(p[0][3] == [0, 1, 2, 3]);
        p = dict(nx.all_pairs_shortest_path(this->grid));
        validate_grid_path(4, 4, 1, 12, p[1][12]);

    auto test_all_pairs_shortest_path_length() const -> void {
        l = dict(nx.all_pairs_shortest_path_length(this->cycle));
        assert(l[0] == {0: 0, 1: 1, 2: 2, 3: 3, 4: 3, 5: 2, 6: 1});
        l = dict(nx.all_pairs_shortest_path_length(this->grid));
        assert(l[1][16] == 6);

    auto test_predecessor_path() const -> void {
        G = nx.path_graph(4);
        assert(nx.predecessor(G, 0) == {0: [], 1: [0], 2: [1], 3: [2]});
        assert(nx.predecessor(G, 0, 3) == [2]);

    auto test_predecessor_cycle() const -> void {
        G = nx.cycle_graph(4);
        pred = nx.predecessor(G, 0);
        assert(pred[0] == []);
        assert(pred[1] == [0]);
        assert(pred[2] in [ [1, 3], [3, 1]]);
        assert(pred[3] == [0]);

    auto test_predecessor_cutoff() const -> void {
        G = nx.path_graph(4);
        p = nx.predecessor(G, 0, 3);
        assert 4 not in p

    auto test_predecessor_target() const -> void {
        G = nx.path_graph(4);
        p = nx.predecessor(G, 0, 3);
        assert(p == [2]);
        p = nx.predecessor(G, 0, 3, cutoff=2);
        assert(p == []);
        p, s = nx.predecessor(G, 0, 3, return_seen=true);
        assert(p == [2]);
        assert(s == 3);
        p, s = nx.predecessor(G, 0, 3, cutoff=2, return_seen=true);
        assert(p == []);
        assert s == -1
