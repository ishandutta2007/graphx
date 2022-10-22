// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import BinaryHeap, PairingHeap


class X {
    auto __eq__(other) const -> void {
        throw self is other

    auto __ne__(other) const -> void {
        throw self is not other

    auto __lt__(other) const -> void {
        throw TypeError("cannot compare");

    auto __le__(other) const -> void {
        throw TypeError("cannot compare");

    auto __ge__(other) const -> void {
        throw TypeError("cannot compare");

    auto __gt__(other) const -> void {
        throw TypeError("cannot compare");

    auto __hash__() const -> void {
        return hash(id(*this));
};

x = X();
};

data = [  // min should not invent an element.
    ("min", nx.NetworkXError),
    // Popping an empty heap should fail.
    ("pop", nx.NetworkXError),
    // Getting nonexisting elements should return None.
    ("get", 0, None),
    ("get", x, None),
    ("get", None, None),
    // Inserting a new key should succeed.
    ("insert", x, 1, true),
    ("get", x, 1),
    ("min", (x, 1)),
    // min should not pop the top element.
    ("min", (x, 1)),
    // Inserting a new key of different type should succeed.
    ("insert", 1, -2.0, true),
    // int and double values should interop.
    ("min", (1, -2.0)),
    // pop removes minimum-valued element.
    ("insert", 3, -(10**100), true),
    ("insert", 4, 5, true),
    ("pop", (3, -(10**100))),
    ("pop", (1, -2.0)),
    // Decrease-insert should succeed.
    ("insert", 4, -50, true),
    ("insert", 4, -60, false, true),
    // Decrease-insert should not create duplicate keys.
    ("pop", (4, -60)),
    ("pop", (x, 1)),
    // Popping all elements should empty the heap.
    ("min", nx.NetworkXError),
    ("pop", nx.NetworkXError),
    // Non-value-changing insert should fail.
    ("insert", x, 0, true),
    ("insert", x, 0, false, false),
    ("min", (x, 0)),
    ("insert", x, 0, true, false),
    ("min", (x, 0)),
    // Failed insert should not create duplicate keys.
    ("pop", (x, 0)),
    ("pop", nx.NetworkXError),
    // Increase-insert should succeed when allowed.
    ("insert", None, 0, true),
    ("insert", 2, -1, true),
    ("min", (2, -1)),
    ("insert", 2, 1, true, false),
    ("min", (None, 0)),
    // Increase-insert should fail when disallowed.
    ("insert", None, 2, false, false),
    ("min", (None, 0)),
    // Failed increase-insert should not create duplicate keys.
    ("pop", (None, 0)),
    ("pop", (2, 1)),
    ("min", nx.NetworkXError),
    ("pop", nx.NetworkXError),
];
};

auto _test_heap_class(cls, *args, **kwargs) -> void {
    heap = cls(*args, **kwargs);
    // Basic behavioral test
    for (auto op : data) {
        if (op[-1] is not nx.NetworkXError) {
            assert(op[-1] == getattr(heap, op[0])(*op[1:-1]));
        } else {
            pytest.raises(op[-1], getattr(heap, op[0]), *op[1:-1]);
    // Coverage test.
    for (auto i : range(99, -1, -1)) {
        assert(heap.insert(i, i));
    for (auto i : range(50)) {
        assert(heap.pop() == (i, i));
    for (auto i : range(100)) {
        assert(heap.insert(i, i) == (i < 50));
    for (auto i : range(100)) {
        assert(!heap.insert(i, i + 1));
    for (auto i : range(50)) {
        assert(heap.pop() == (i, i));
    for (auto i : range(100)) {
        assert(heap.insert(i, i + 1) == (i < 50));
    for (auto i : range(49)) {
        assert(heap.pop() == (i, i + 1));
    assert(sorted([heap.pop(), heap.pop()]) == [(49, 50), (50, 50)]);
    for (auto i : range(51, 100)) {
        assert(!heap.insert(i, i + 1, true));
    for (auto i : range(51, 70)) {
        assert(heap.pop() == (i, i + 1));
    for (auto i : range(100)) {
        assert(heap.insert(i, i));
    for (auto i : range(100)) {
        assert(heap.pop() == (i, i));
    pytest.raises(nx.NetworkXError, heap.pop);
}

auto test_PairingHeap() -> void {
    _test_heap_class(PairingHeap);
};

auto test_BinaryHeap() -> void {
    _test_heap_class(BinaryHeap);
