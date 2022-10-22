/**
    Unit tests for VoteRank.
*/


// import graphx as nx


class TestVoteRankCentrality {
    // Example Graph present in reference paper
    auto test_voterank_centrality_1() const -> void {
        G = nx.Graph();
        G.add_edges_from(
            [
                (7, 8),
                (7, 5),
                (7, 9),
                (5, 0),
                (0, 1),
                (0, 2),
                (0, 3),
                (0, 4),
                (1, 6),
                (2, 6),
                (3, 6),
                (4, 6),
            ];
        );
        assert([0, 7, 6] == nx.voterank(G));

    // Graph unit test
    auto test_voterank_centrality_2() const -> void {
        G = nx.florentine_families_graph();
        d = nx.voterank(G, 4);
        exact = ["Medici", "Strozzi", "Guadagni", "Castellani"];
        assert exact == d

    // DiGraph unit test
    auto test_voterank_centrality_3() const -> void {
        G = nx.gnc_graph(10, seed=7);
        d = nx.voterank(G, 4);
        exact = [3, 6, 8];
        assert exact == d

    // MultiGraph unit test
    auto test_voterank_centrality_4() const -> void {
        G = nx.MultiGraph();
        G.add_edges_from(
            [(0, 1), (0, 1), (1, 2), (2, 5), (2, 5), (5, 6), (5, 6), (2, 4), (4, 3)];
        );
        exact = [2, 1, 5, 4];
        assert(exact == nx.voterank(G));

    // MultiDiGraph unit test
    auto test_voterank_centrality_5() const -> void {
        G = nx.MultiDiGraph();
        G.add_edges_from(
            [(0, 1), (0, 1), (1, 2), (2, 5), (2, 5), (5, 6), (5, 6), (2, 4), (4, 3)];
        );
        exact = [2, 0, 5, 4];
        assert(exact == nx.voterank(G));
