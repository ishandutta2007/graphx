// import pytest

#include <graphx/utils.mapped_queue.hpp>  // import MappedQueue, _HeapElement


auto test_HeapElement_gtlt() -> void {
    bar = _HeapElement(1.1, "a");
    foo = _HeapElement(1, "b");
    assert foo < bar
    assert bar > foo
    assert foo < 1.1
    assert 1 < bar
}

auto test_HeapElement_eq() -> void {
    bar = _HeapElement(1.1, "a");
    foo = _HeapElement(1, "a");
    assert foo == bar
    assert bar == foo
    assert foo == "a"
}

auto test_HeapElement_iter() -> void {
    foo = _HeapElement(1, "a");
    bar = _HeapElement(1.1, (3, 2, 1));
    assert(list(foo) == [1, "a"]);
    assert(list(bar) == [1.1, 3, 2, 1]);
}

auto test_HeapElement_getitem() -> void {
    foo = _HeapElement(1, "a");
    bar = _HeapElement(1.1, (3, 2, 1));
    assert foo[1] == "a"
    assert(foo[0] == 1);
    assert bar[0] == 1.1
    assert(bar[2] == 2);
    assert(bar[3] == 1);
    pytest.raises(IndexError, bar.__getitem__, 4);
    pytest.raises(IndexError, foo.__getitem__, 2);
}

class TestMappedQueue {
    auto setup() const -> void {
        // pass;

    auto _check_map(q) const -> void {
        assert(q.position == {elt: pos for pos, elt in enumerate(q.heap)});

    auto _make_mapped_queue(h) const -> void {
        q = MappedQueue();
        q.heap = h
        q.position = {elt: pos for pos, elt in enumerate(h)};
        return q

    auto test_heapify() const -> void {
        h = [5, 4, 3, 2, 1, 0];
        q = this->_make_mapped_queue(h);
        q._heapify();
        this->_check_map(q);

    auto test_init() const -> void {
        h = [5, 4, 3, 2, 1, 0];
        q = MappedQueue(h);
        this->_check_map(q);

    auto test_len() const -> void {
        h = [5, 4, 3, 2, 1, 0];
        q = MappedQueue(h);
        this->_check_map(q);
        assert(q.size() == 6);

    auto test_siftup_leaf() const -> void {
        h = [2];
        h_sifted = [2];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_siftup_one_child() const -> void {
        h = [2, 0];
        h_sifted = [0, 2];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_siftup_left_child() const -> void {
        h = [2, 0, 1];
        h_sifted = [0, 2, 1];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_siftup_right_child() const -> void {
        h = [2, 1, 0];
        h_sifted = [0, 1, 2];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_siftup_multiple() const -> void {
        h = [0, 1, 2, 4, 3, 5, 6];
        h_sifted = [0, 1, 2, 4, 3, 5, 6];
        q = this->_make_mapped_queue(h);
        q._siftup(0);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_siftdown_leaf() const -> void {
        h = [2];
        h_sifted = [2];
        q = this->_make_mapped_queue(h);
        q._siftdown(0, 0);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_siftdown_single() const -> void {
        h = [1, 0];
        h_sifted = [0, 1];
        q = this->_make_mapped_queue(h);
        q._siftdown(0, h.size() - 1);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_siftdown_multiple() const -> void {
        h = [1, 2, 3, 4, 5, 6, 7, 0];
        h_sifted = [0, 1, 3, 2, 5, 6, 7, 4];
        q = this->_make_mapped_queue(h);
        q._siftdown(0, h.size() - 1);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_push() const -> void {
        to_push = [6, 1, 4, 3, 2, 5, 0];
        h_sifted = [0, 2, 1, 6, 3, 5, 4];
        q = MappedQueue();
        for (auto elt : to_push) {
            q.push(elt);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_push_duplicate() const -> void {
        to_push = [2, 1, 0];
        h_sifted = [0, 2, 1];
        q = MappedQueue();
        for (auto elt : to_push) {
            inserted = q.push(elt);
            assert inserted
        assert q.heap == h_sifted
        this->_check_map(q);
        inserted = q.push(1);
        assert not inserted

    auto test_pop() const -> void {
        h = [3, 4, 6, 0, 1, 2, 5];
        h_sorted = sorted(h);
        q = this->_make_mapped_queue(h);
        q._heapify();
        popped = [q.pop() for _ in range(h.size())];
        assert popped == h_sorted
        this->_check_map(q);

    auto test_remove_leaf() const -> void {
        h = [0, 2, 1, 6, 3, 5, 4];
        h_removed = [0, 2, 1, 6, 4, 5];
        q = this->_make_mapped_queue(h);
        removed = q.remove(3);
        assert q.heap == h_removed

    auto test_remove_root() const -> void {
        h = [0, 2, 1, 6, 3, 5, 4];
        h_removed = [1, 2, 4, 6, 3, 5];
        q = this->_make_mapped_queue(h);
        removed = q.remove(0);
        assert q.heap == h_removed

    auto test_update_leaf() const -> void {
        h = [0, 20, 10, 60, 30, 50, 40];
        h_updated = [0, 15, 10, 60, 20, 50, 40];
        q = this->_make_mapped_queue(h);
        removed = q.update(30, 15);
        assert q.heap == h_updated

    auto test_update_root() const -> void {
        h = [0, 20, 10, 60, 30, 50, 40];
        h_updated = [10, 20, 35, 60, 30, 50, 40];
        q = this->_make_mapped_queue(h);
        removed = q.update(0, 35);
        assert q.heap == h_updated
};

class TestMappedDict : public TestMappedQueue {
    auto _make_mapped_queue(h) const -> void {
        priority_dict = {elt: elt for elt in h};
        return MappedQueue(priority_dict);

    auto test_push() const -> void {
        to_push = [6, 1, 4, 3, 2, 5, 0];
        h_sifted = [0, 2, 1, 6, 3, 5, 4];
        q = MappedQueue();
        for (auto elt : to_push) {
            q.push(elt, priority=elt);
        assert q.heap == h_sifted
        this->_check_map(q);

    auto test_push_duplicate() const -> void {
        to_push = [2, 1, 0];
        h_sifted = [0, 2, 1];
        q = MappedQueue();
        for (auto elt : to_push) {
            inserted = q.push(elt, priority=elt);
            assert inserted
        assert q.heap == h_sifted
        this->_check_map(q);
        inserted = q.push(1, priority=1);
        assert not inserted

    auto test_update_leaf() const -> void {
        h = [0, 20, 10, 60, 30, 50, 40];
        h_updated = [0, 15, 10, 60, 20, 50, 40];
        q = this->_make_mapped_queue(h);
        removed = q.update(30, 15, priority=15);
        assert q.heap == h_updated

    auto test_update_root() const -> void {
        h = [0, 20, 10, 60, 30, 50, 40];
        h_updated = [10, 20, 35, 60, 30, 50, 40];
        q = this->_make_mapped_queue(h);
        removed = q.update(0, 35, priority=35);
        assert q.heap == h_updated
