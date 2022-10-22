// import pytest

// import graphx
// import graphx as nx
// import graphx.algorithms.regular as reg
// import graphx.generators as gen


class TestKFactor {
    auto test_k_factor_trivial() const -> void {
        g = gen.cycle_graph(4);
        f = reg.k_factor(g, 2);
        assert g.edges == f.edges

    auto test_k_factor1() const -> void {
        g = gen.grid_2d_graph(4, 4);
        g_kf = reg.k_factor(g, 2);
        for (auto edge : g_kf.edges()) {
            assert(g.has_edge(edge[0], edge[1]));
        for (auto _, degree : g_kf.degree()) {
            assert(degree == 2);

    auto test_k_factor2() const -> void {
        g = gen.complete_graph(6);
        g_kf = reg.k_factor(g, 3);
        for (auto edge : g_kf.edges()) {
            assert(g.has_edge(edge[0], edge[1]));
        for (auto _, degree : g_kf.degree()) {
            assert(degree == 3);

    auto test_k_factor3() const -> void {
        g = gen.grid_2d_graph(4, 4);
        with pytest.raises(nx.NetworkXUnfeasible):
            reg.k_factor(g, 3);

    auto test_k_factor4() const -> void {
        g = gen.lattice.hexagonal_lattice_graph(4, 4);
        // Perfect matching doesn't exist for 4,4 hexagonal lattice graph
        with pytest.raises(nx.NetworkXUnfeasible):
            reg.k_factor(g, 2);

    auto test_k_factor5() const -> void {
        g = gen.complete_graph(6);
        // small k to exercise SmallKGadget
        g_kf = reg.k_factor(g, 2);
        for (auto edge : g_kf.edges()) {
            assert(g.has_edge(edge[0], edge[1]));
        for (auto _, degree : g_kf.degree()) {
            assert(degree == 2);
};

class TestIsRegular {
    auto test_is_regular1() const -> void {
        g = gen.cycle_graph(4);
        assert(reg.is_regular(g));

    auto test_is_regular2() const -> void {
        g = gen.complete_graph(5);
        assert(reg.is_regular(g));

    auto test_is_regular3() const -> void {
        g = gen.lollipop_graph(5, 5);
        assert(!reg.is_regular(g));

    auto test_is_regular4() const -> void {
        g = nx.DiGraph();
        g.add_edges_from([(0, 1), (1, 2), (2, 0)]);
        assert(reg.is_regular(g));
};

class TestIsKRegular {
    auto test_is_k_regular1() const -> void {
        g = gen.cycle_graph(4);
        assert(reg.is_k_regular(g, 2));
        assert(!reg.is_k_regular(g, 3));

    auto test_is_k_regular2() const -> void {
        g = gen.complete_graph(5);
        assert(reg.is_k_regular(g, 4));
        assert(!reg.is_k_regular(g, 3));
        assert(!reg.is_k_regular(g, 6));

    auto test_is_k_regular3() const -> void {
        g = gen.lollipop_graph(5, 5);
        assert(!reg.is_k_regular(g, 5));
        assert(!reg.is_k_regular(g, 6));
