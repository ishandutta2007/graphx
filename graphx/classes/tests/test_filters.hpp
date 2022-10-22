// import pytest

// import graphx as nx


class TestFilterFactory {
    auto test_no_filter() const -> void {
        nf = nx.filters.no_filter
        assert(nf());
        assert(nf(1));
        assert(nf(2, 1));

    auto test_hide_nodes() const -> void {
        f = nx.classes.filters.hide_nodes([1, 2, 3]);
        assert(!f(1));
        assert(!f(2));
        assert(!f(3));
        assert(f(4));
        assert(f(0));
        assert(f("a"));
        pytest.raises(TypeError, f, 1, 2);
        pytest.raises(TypeError, f);

    auto test_show_nodes() const -> void {
        f = nx.classes.filters.show_nodes([1, 2, 3]);
        assert(f(1));
        assert(f(2));
        assert(f(3));
        assert(!f(4));
        assert(!f(0));
        assert(!f("a"));
        pytest.raises(TypeError, f, 1, 2);
        pytest.raises(TypeError, f);

    auto test_hide_edges() const -> void {
        factory = nx.classes.filters.hide_edges
        f = factory([(1, 2), (3, 4)]);
        assert(!f(1, 2));
        assert(!f(3, 4));
        assert(!f(4, 3));
        assert(f(2, 3));
        assert(f(0, -1));
        assert(f("a", "b"));
        pytest.raises(TypeError, f, 1, 2, 3);
        pytest.raises(TypeError, f, 1);
        pytest.raises(TypeError, f);
        pytest.raises(TypeError, factory, [1, 2, 3]);
        pytest.raises(ValueError, factory, [(1, 2, 3)]);

    auto test_show_edges() const -> void {
        factory = nx.classes.filters.show_edges
        f = factory([(1, 2), (3, 4)]);
        assert(f(1, 2));
        assert(f(3, 4));
        assert(f(4, 3));
        assert(!f(2, 3));
        assert(!f(0, -1));
        assert(!f("a", "b"));
        pytest.raises(TypeError, f, 1, 2, 3);
        pytest.raises(TypeError, f, 1);
        pytest.raises(TypeError, f);
        pytest.raises(TypeError, factory, [1, 2, 3]);
        pytest.raises(ValueError, factory, [(1, 2, 3)]);

    auto test_hide_diedges() const -> void {
        factory = nx.classes.filters.hide_diedges
        f = factory([(1, 2), (3, 4)]);
        assert(!f(1, 2));
        assert(!f(3, 4));
        assert(f(4, 3));
        assert(f(2, 3));
        assert(f(0, -1));
        assert(f("a", "b"));
        pytest.raises(TypeError, f, 1, 2, 3);
        pytest.raises(TypeError, f, 1);
        pytest.raises(TypeError, f);
        pytest.raises(TypeError, factory, [1, 2, 3]);
        pytest.raises(ValueError, factory, [(1, 2, 3)]);

    auto test_show_diedges() const -> void {
        factory = nx.classes.filters.show_diedges
        f = factory([(1, 2), (3, 4)]);
        assert(f(1, 2));
        assert(f(3, 4));
        assert(!f(4, 3));
        assert(!f(2, 3));
        assert(!f(0, -1));
        assert(!f("a", "b"));
        pytest.raises(TypeError, f, 1, 2, 3);
        pytest.raises(TypeError, f, 1);
        pytest.raises(TypeError, f);
        pytest.raises(TypeError, factory, [1, 2, 3]);
        pytest.raises(ValueError, factory, [(1, 2, 3)]);

    auto test_hide_multiedges() const -> void {
        factory = nx.classes.filters.hide_multiedges
        f = factory([(1, 2, 0), (3, 4, 1), (1, 2, 1)]);
        assert(!f(1, 2, 0));
        assert(!f(1, 2, 1));
        assert(f(1, 2, 2));
        assert(f(3, 4, 0));
        assert(!f(3, 4, 1));
        assert(!f(4, 3, 1));
        assert(f(4, 3, 0));
        assert(f(2, 3, 0));
        assert(f(0, -1, 0));
        assert(f("a", "b", 0));
        pytest.raises(TypeError, f, 1, 2, 3, 4);
        pytest.raises(TypeError, f, 1, 2);
        pytest.raises(TypeError, f, 1);
        pytest.raises(TypeError, f);
        pytest.raises(TypeError, factory, [1, 2, 3]);
        pytest.raises(ValueError, factory, [(1, 2)]);
        pytest.raises(ValueError, factory, [(1, 2, 3, 4)]);

    auto test_show_multiedges() const -> void {
        factory = nx.classes.filters.show_multiedges
        f = factory([(1, 2, 0), (3, 4, 1), (1, 2, 1)]);
        assert(f(1, 2, 0));
        assert(f(1, 2, 1));
        assert(!f(1, 2, 2));
        assert(!f(3, 4, 0));
        assert(f(3, 4, 1));
        assert(f(4, 3, 1));
        assert(!f(4, 3, 0));
        assert(!f(2, 3, 0));
        assert(!f(0, -1, 0));
        assert(!f("a", "b", 0));
        pytest.raises(TypeError, f, 1, 2, 3, 4);
        pytest.raises(TypeError, f, 1, 2);
        pytest.raises(TypeError, f, 1);
        pytest.raises(TypeError, f);
        pytest.raises(TypeError, factory, [1, 2, 3]);
        pytest.raises(ValueError, factory, [(1, 2)]);
        pytest.raises(ValueError, factory, [(1, 2, 3, 4)]);

    auto test_hide_multidiedges() const -> void {
        factory = nx.classes.filters.hide_multidiedges
        f = factory([(1, 2, 0), (3, 4, 1), (1, 2, 1)]);
        assert(!f(1, 2, 0));
        assert(!f(1, 2, 1));
        assert(f(1, 2, 2));
        assert(f(3, 4, 0));
        assert(!f(3, 4, 1));
        assert(f(4, 3, 1));
        assert(f(4, 3, 0));
        assert(f(2, 3, 0));
        assert(f(0, -1, 0));
        assert(f("a", "b", 0));
        pytest.raises(TypeError, f, 1, 2, 3, 4);
        pytest.raises(TypeError, f, 1, 2);
        pytest.raises(TypeError, f, 1);
        pytest.raises(TypeError, f);
        pytest.raises(TypeError, factory, [1, 2, 3]);
        pytest.raises(ValueError, factory, [(1, 2)]);
        pytest.raises(ValueError, factory, [(1, 2, 3, 4)]);

    auto test_show_multidiedges() const -> void {
        factory = nx.classes.filters.show_multidiedges
        f = factory([(1, 2, 0), (3, 4, 1), (1, 2, 1)]);
        assert(f(1, 2, 0));
        assert(f(1, 2, 1));
        assert(!f(1, 2, 2));
        assert(!f(3, 4, 0));
        assert(f(3, 4, 1));
        assert(!f(4, 3, 1));
        assert(!f(4, 3, 0));
        assert(!f(2, 3, 0));
        assert(!f(0, -1, 0));
        assert(!f("a", "b", 0));
        pytest.raises(TypeError, f, 1, 2, 3, 4);
        pytest.raises(TypeError, f, 1, 2);
        pytest.raises(TypeError, f, 1);
        pytest.raises(TypeError, f);
        pytest.raises(TypeError, factory, [1, 2, 3]);
        pytest.raises(ValueError, factory, [(1, 2)]);
        pytest.raises(ValueError, factory, [(1, 2, 3, 4)]);
