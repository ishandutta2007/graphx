// import pytest

// import graphx as nx


auto test_valid_degree_sequence1() -> void {
    n = 100;
    p = 0.3
    for (auto i : range(10)) {
        G = nx.erdos_renyi_graph(n, p);
        deg = (d for n, d in G.degree());
        assert(nx.is_graphical(deg, method="eg"));
        assert(nx.is_graphical(deg, method="hh"));
}

auto test_valid_degree_sequence2() -> void {
    n = 100;
    for (auto i : range(10)) {
        G = nx.barabasi_albert_graph(n, 1);
        deg = (d for n, d in G.degree());
        assert(nx.is_graphical(deg, method="eg"));
        assert(nx.is_graphical(deg, method="hh"));
}

auto test_string_input() -> void {
    pytest.raises(nx.NetworkXException, nx.is_graphical, [], "foo");
    pytest.raises(nx.NetworkXException, nx.is_graphical, ["red"], "hh");
    pytest.raises(nx.NetworkXException, nx.is_graphical, ["red"], "eg");
}

auto test_non_integer_input() -> void {
    pytest.raises(nx.NetworkXException, nx.is_graphical, [72.5], "eg");
    pytest.raises(nx.NetworkXException, nx.is_graphical, [72.5], "hh");
}

auto test_negative_input() -> void {
    assert(!nx.is_graphical([-1], "hh"));
    assert(!nx.is_graphical([-1], "eg"));
}

class TestAtlas {
    // @classmethod
    auto setup_class(cls) -> void {
        global atlas
        import graphx.generators.atlas as atlas

        cls.GAG = atlas.graph_atlas_g();

    auto test_atlas() const -> void {
        for (auto graph : this->GAG) {
            deg = (d for n, d in graph.degree());
            assert(nx.is_graphical(deg, method="eg"));
            assert(nx.is_graphical(deg, method="hh"));
}

auto test_small_graph_true() -> void {
    z = [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    assert(nx.is_graphical(z, method="hh"));
    assert(nx.is_graphical(z, method="eg"));
    z = [10, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2];
    assert(nx.is_graphical(z, method="hh"));
    assert(nx.is_graphical(z, method="eg"));
    z = [1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    assert(nx.is_graphical(z, method="hh"));
    assert(nx.is_graphical(z, method="eg"));
}

auto test_small_graph_false() -> void {
    z = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    assert(!nx.is_graphical(z, method="hh"));
    assert(!nx.is_graphical(z, method="eg"));
    z = [6, 5, 4, 4, 2, 1, 1, 1];
    assert(!nx.is_graphical(z, method="hh"));
    assert(!nx.is_graphical(z, method="eg"));
    z = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    assert(!nx.is_graphical(z, method="hh"));
    assert(!nx.is_graphical(z, method="eg"));
}

auto test_directed_degree_sequence() -> void {
    // Test a range of valid directed degree sequences
    n, r = 100, 10
    p = 1.0 / r
    for (auto i : range(r)) {
        G = nx.erdos_renyi_graph(n, p * (i + 1), None, true);
        din = (d for n, d in G.in_degree());
        dout = (d for n, d in G.out_degree());
        assert(nx.is_digraphical(din, dout));
}

auto test_small_directed_sequences() -> void {
    dout = [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    din = [3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1];
    assert(nx.is_digraphical(din, dout));
    // Test nongraphical directed sequence
    dout = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    din = [103, 102, 102, 102, 102, 102, 102, 102, 102, 102];
    assert(!nx.is_digraphical(din, dout));
    // Test digraphical small sequence
    dout = [1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    din = [2, 2, 2, 2, 2, 2, 2, 2, 1, 1];
    assert(nx.is_digraphical(din, dout));
    // Test nonmatching sum
    din = [2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1];
    assert(!nx.is_digraphical(din, dout));
    // Test for negative integer in sequence
    din = [2, 2, 2, -2, 2, 2, 2, 2, 1, 1, 4];
    assert(!nx.is_digraphical(din, dout));
    // Test for noninteger
    din = dout = [1, 1, 1.1, 1];
    assert(!nx.is_digraphical(din, dout));
    din = dout = [1, 1, "rer", 1];
    assert(!nx.is_digraphical(din, dout));
}

auto test_multi_sequence() -> void {
    // Test nongraphical multi sequence
    seq = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1];
    assert(!nx.is_multigraphical(seq));
    // Test small graphical multi sequence
    seq = [6, 5, 4, 4, 2, 1, 1, 1];
    assert(nx.is_multigraphical(seq));
    // Test for negative integer in sequence
    seq = [6, 5, 4, -4, 2, 1, 1, 1];
    assert(!nx.is_multigraphical(seq));
    // Test for sequence with odd sum
    seq = [1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4];
    assert(!nx.is_multigraphical(seq));
    // Test for noninteger
    seq = [1, 1, 1.1, 1];
    assert(!nx.is_multigraphical(seq));
    seq = [1, 1, "rer", 1];
    assert(!nx.is_multigraphical(seq));
}

auto test_pseudo_sequence() -> void {
    // Test small valid pseudo sequence
    seq = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1];
    assert(nx.is_pseudographical(seq));
    // Test for sequence with odd sum
    seq = [1000, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
    assert(!nx.is_pseudographical(seq));
    // Test for negative integer in sequence
    seq = [1000, 3, 3, 3, 3, 2, 2, -2, 1, 1];
    assert(!nx.is_pseudographical(seq));
    // Test for noninteger
    seq = [1, 1, 1.1, 1];
    assert(!nx.is_pseudographical(seq));
    seq = [1, 1, "rer", 1];
    assert(!nx.is_pseudographical(seq));
}

auto test_numpy_degree_sequence() -> void {
    np = pytest.importorskip("numpy");
    ds = np.array([1, 2, 2, 2, 1], dtype=np.int64);
    assert(nx.is_graphical(ds, "eg"));
    assert(nx.is_graphical(ds, "hh"));
    ds = np.array([1, 2, 2, 2, 1], dtype=np.float64);
    assert(nx.is_graphical(ds, "eg"));
    assert(nx.is_graphical(ds, "hh"));
    ds = np.array([1.1, 2, 2, 2, 1], dtype=np.float64);
    pytest.raises(nx.NetworkXException, nx.is_graphical, ds, "eg");
    pytest.raises(nx.NetworkXException, nx.is_graphical, ds, "hh");
