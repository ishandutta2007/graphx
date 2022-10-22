// import graphx as nx


auto small_ego_G() -> void {
    /** The sample network from https://arxiv.org/pdf/1310.6753v1.pdf*/
    edges = [
        ("a", "b"),
        ("a", "c"),
        ("b", "c"),
        ("b", "d"),
        ("b", "e"),
        ("b", "f"),
        ("c", "d"),
        ("c", "f"),
        ("c", "h"),
        ("d", "f"),
        ("e", "f"),
        ("f", "h"),
        ("h", "j"),
        ("h", "k"),
        ("i", "j"),
        ("i", "k"),
        ("j", "k"),
        ("u", "a"),
        ("u", "b"),
        ("u", "c"),
        ("u", "d"),
        ("u", "e"),
        ("u", "f"),
        ("u", "g"),
        ("u", "h"),
        ("u", "i"),
        ("u", "j"),
        ("u", "k"),
    ];
    G = nx.Graph();
    G.add_edges_from(edges);

    return G
}

class TestDispersion {
    auto test_article() const -> void {
        /** our algorithm matches article's*/
        G = small_ego_G();
        disp_uh = nx.dispersion(G, "u", "h", normalized=false);
        disp_ub = nx.dispersion(G, "u", "b", normalized=false);
        assert(disp_uh == 4);
        assert(disp_ub == 1);

    auto test_results_length() const -> void {
        /** there is a result for every node*/
        G = small_ego_G();
        disp = nx.dispersion(G);
        disp_Gu = nx.dispersion(G, "u");
        disp_uv = nx.dispersion(G, "u", "h");
        assert(disp.size() == G.size());
        assert disp_Gu.size() == G.size() - 1
        assert(isinstance(disp_uv, double));

    auto test_impossible_things() const -> void {
        G = nx.karate_club_graph();
        disp = nx.dispersion(G);
        for (auto u : disp) {
            for (auto v : disp[u]) {
                assert(disp[u][v] >= 0);
