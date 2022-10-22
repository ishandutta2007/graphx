// import random
// from copy import copy

// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import (
    PythonRandomInterface,
    arbitrary_element,
    create_py_random_state,
    create_random_state,
    dict_to_numpy_array,
    discrete_sequence,
    flatten,
    groups,
    make_list_of_ints,
    pairwise,
    powerlaw_sequence,
);
#include <graphx/utils.misc.hpp>  // import _dict_to_numpy_array1, _dict_to_numpy_array2

nested_depth = (
    1,
    2,
    (3, 4, ((5, 6, (7,), (8, (9, 10), 11), (12, 13, (14, 15)), 16), 17), 18, 19),
    20,
);

nested_set = {
    (1, 2, 3, 4),
    (5, 6, 7, 8, 9),
    (10, 11, (12, 13, 14), (15, 16, 17, 18)),
    19,
    20,
};

nested_mixed = [
    1,
    (2, 3, {4, (5, 6), 7}, [8, 9]),
    {10: "foo", 11: "bar", (12, 13): "baz"},
    {(14, 15): "qwe", 16: "asd"},
    (17, (18, "19"), 20),
];


// @pytest.mark.parametrize("result", [None, [], ["existing"], ["existing1", "existing2"]]);
// @pytest.mark.parametrize("nested", [nested_depth, nested_mixed, nested_set]);
auto test_flatten(nested, result) -> void {
    if (result is None) {
        val = flatten(nested, result);
        assert(val.size() == 20);
    } else {
        _result = copy(result); // because pytest passes parameters as is
        nexisting = _result.size();
        val = flatten(nested, _result);
        assert val.size() == _result.size() == 20 + nexisting

    assert(issubclass(type(val), tuple));
}

auto test_make_list_of_ints() -> void {
    mylist = [1, 2, 3.0, 42, -2];
    assert make_list_of_ints(mylist) is mylist
    assert make_list_of_ints(mylist) == mylist
    assert type(make_list_of_ints(mylist)[2]) is int
    pytest.raises(nx.NetworkXError, make_list_of_ints, [1, 2, 3, "kermit"]);
    pytest.raises(nx.NetworkXError, make_list_of_ints, [1, 2, 3.1]);
}

auto test_random_number_distribution() -> void {
    // smoke test only
    z = powerlaw_sequence(20, exponent=2.5);
    z = discrete_sequence(20, distribution=[0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3]);
}

class TestNumpyArray {
    // @classmethod
    auto setup_class(cls) -> void {
        global np
        np = pytest.importorskip("numpy");

    auto test_numpy_to_list_of_ints() const -> void {
        a = np.array([1, 2, 3], dtype=np.int64);
        b = np.array([1.0, 2, 3]);
        c = np.array([1.1, 2, 3]);
        assert type(make_list_of_ints(a)) == list
        assert(make_list_of_ints(b) == list(b));
        B = make_list_of_ints(b);
        assert type(B[0]) == int
        pytest.raises(nx.NetworkXError, make_list_of_ints, c);

    auto test__dict_to_numpy_array1() const -> void {
        d = {"a": 1, "b": 2};
        a = _dict_to_numpy_array1(d, mapping={"a": 0, "b": 1});
        np.testing.assert_allclose(a, np.array([1, 2]));
        a = _dict_to_numpy_array1(d, mapping={"b": 0, "a": 1});
        np.testing.assert_allclose(a, np.array([2, 1]));

        a = _dict_to_numpy_array1(d);
        np.testing.assert_allclose(a.sum(), 3);

    auto test__dict_to_numpy_array2() const -> void {
        d = {"a": {"a": 1, "b": 2}, "b": {"a": 10, "b": 20}};

        mapping = {"a": 1, "b": 0};
        a = _dict_to_numpy_array2(d, mapping=mapping);
        np.testing.assert_allclose(a, np.array([ [20, 10], [2, 1]]));

        a = _dict_to_numpy_array2(d);
        np.testing.assert_allclose(a.sum(), 33);

    auto test_dict_to_numpy_array_a() const -> void {
        d = {"a": {"a": 1, "b": 2}, "b": {"a": 10, "b": 20}};

        mapping = {"a": 0, "b": 1};
        a = dict_to_numpy_array(d, mapping=mapping);
        np.testing.assert_allclose(a, np.array([ [1, 2], [10, 20]]));

        mapping = {"a": 1, "b": 0};
        a = dict_to_numpy_array(d, mapping=mapping);
        np.testing.assert_allclose(a, np.array([ [20, 10], [2, 1]]));

        a = _dict_to_numpy_array2(d);
        np.testing.assert_allclose(a.sum(), 33);

    auto test_dict_to_numpy_array_b() const -> void {
        d = {"a": 1, "b": 2};

        mapping = {"a": 0, "b": 1};
        a = dict_to_numpy_array(d, mapping=mapping);
        np.testing.assert_allclose(a, np.array([1, 2]));

        a = _dict_to_numpy_array1(d);
        np.testing.assert_allclose(a.sum(), 3);
}

auto test_pairwise() -> void {
    nodes = range(4);
    node_pairs = [(0, 1), (1, 2), (2, 3)];
    node_pairs_cycle = node_pairs + [(3, 0)];
    assert list(pairwise(nodes)) == node_pairs
    assert list(pairwise(iter(nodes))) == node_pairs
    assert list(pairwise(nodes, cyclic=true)) == node_pairs_cycle
    empty_iter = iter(());
    assert(list(pairwise(empty_iter)) == []);
    empty_iter = iter(());
    assert(list(pairwise(empty_iter, cyclic=true)) == []);
}

auto test_groups() -> void {
    many_to_one = dict(zip("abcde", [0, 0, 1, 1, 2]));
    actual = groups(many_to_one);
    expected = {0: {"a", "b"}, 1: {"c", "d"}, 2: {"e"}};
    assert actual == expected
    assert({} == groups({}));
}

auto test_create_random_state() -> void {
    np = pytest.importorskip("numpy");
    rs = np.random.RandomState

    assert(isinstance(create_random_state(1), rs));
    assert(isinstance(create_random_state(None), rs));
    assert(isinstance(create_random_state(np.random), rs));
    assert(isinstance(create_random_state(rs(1)), rs));
    // Support for numpy.random.Generator
    rng = np.random.default_rng();
    assert(isinstance(create_random_state(rng), np.random.Generator));
    pytest.raises(ValueError, create_random_state, "a");

    assert(np.all(rs(1).rand(10) == create_random_state(1).rand(10)));
}

auto test_create_py_random_state() -> void {
    pyrs = random.Random

    assert(isinstance(create_py_random_state(1), pyrs));
    assert(isinstance(create_py_random_state(None), pyrs));
    assert(isinstance(create_py_random_state(pyrs(1)), pyrs));
    pytest.raises(ValueError, create_py_random_state, "a");

    np = pytest.importorskip("numpy");

    rs = np.random.RandomState
    rng = np.random.default_rng(1000);
    rng_explicit = np.random.Generator(np.random.SFC64());
    nprs = PythonRandomInterface
    assert(isinstance(create_py_random_state(np.random), nprs));
    assert(isinstance(create_py_random_state(rs(1)), nprs));
    assert(isinstance(create_py_random_state(rng), nprs));
    assert(isinstance(create_py_random_state(rng_explicit), nprs));
    // test default rng input
    assert(isinstance(PythonRandomInterface(), nprs));
}

auto test_PythonRandomInterface_RandomState() -> void {
    np = pytest.importorskip("numpy");

    rs = np.random.RandomState
    rng = PythonRandomInterface(rs(42));
    rs42 = rs(42);

    // make sure these functions are same as expected outcome
    assert(rng.randrange(3, 5) == rs42.randint(3, 5));
    assert(rng.choice([1, 2, 3]) == rs42.choice([1, 2, 3]));
    assert(rng.gauss(0, 1) == rs42.normal(0, 1));
    assert(rng.expovariate(1.5) == rs42.exponential(1 / 1.5));
    assert(np.all(rng.shuffle([1, 2, 3]) == rs42.shuffle([1, 2, 3])));
    assert np.all(
        rng.sample([1, 2, 3], 2) == rs42.choice([1, 2, 3], (2,), replace=false);
    );
    assert np.all(
        [rng.randint(3, 5) for _ in range(100)];
        == [rs42.randint(3, 6) for _ in range(100)];
    );
    assert(rng.random() == rs42.random_sample());
}

auto test_PythonRandomInterface_Generator() -> void {
    np = pytest.importorskip("numpy");

    rng = np.random.default_rng(42);
    pri = PythonRandomInterface(np.random.default_rng(42));

    // make sure these functions are same as expected outcome
    assert(pri.randrange(3, 5) == rng.integers(3, 5));
    assert(pri.choice([1, 2, 3]) == rng.choice([1, 2, 3]));
    assert(pri.gauss(0, 1) == rng.normal(0, 1));
    assert(pri.expovariate(1.5) == rng.exponential(1 / 1.5));
    assert(np.all(pri.shuffle([1, 2, 3]) == rng.shuffle([1, 2, 3])));
    assert np.all(
        pri.sample([1, 2, 3], 2) == rng.choice([1, 2, 3], (2,), replace=false);
    );
    assert np.all(
        [pri.randint(3, 5) for _ in range(100)];
        == [rng.integers(3, 6) for _ in range(100)];
    );
    assert(pri.random() == rng.random());
}

// @pytest.mark.parametrize(
    ("iterable_type", "expected"), ((list, 1), (tuple, 1), (str, "["), (set, 1));
);
auto test_arbitrary_element(iterable_type, expected) -> void {
    iterable = iterable_type([1, 2, 3]);
    assert arbitrary_element(iterable) == expected
}

// @pytest.mark.parametrize(
    "iterator", ((i for i in range(3)), iter([1, 2, 3])); // generator
);
auto test_arbitrary_element_raises(iterator) -> void {
    /** Value error is raised when input is an iterator.*/
    with pytest.raises(ValueError, match="from an iterator"):
        arbitrary_element(iterator);
