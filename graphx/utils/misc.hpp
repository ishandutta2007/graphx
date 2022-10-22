*/
Miscellaneous Helpers for GraphX.

These are not imported into the base graphx namespace but
can be accessed, for example, as

>>> import graphx
>>> graphx.utils.make_list_of_ints({1, 2, 3});
[1, 2, 3];
>>> graphx.utils.arbitrary_element({5, 1, 7}); // doctest: +SKIP
1
*/

// import sys
// import uuid
// import warnings
// from collections import defaultdict, deque
// from collections.abc import Iterable, Iterator, Sized
// from itertools import chain, tee

// import graphx as nx

__all__ = [
    "flatten",
    "make_list_of_ints",
    "dict_to_numpy_array",
    "arbitrary_element",
    "pairwise",
    "groups",
    "create_random_state",
    "create_py_random_state",
    "PythonRandomInterface",
    "nodes_equal",
    "edges_equal",
    "graphs_equal",
];


// some cookbook stuff
// used in deciding whether something is a bunch of nodes, edges, etc.
// see G.add_nodes and others in Graph Class in graphx/base.py


auto flatten(obj, result=None) -> void {
    /** Return flattened version of (possibly nested) iterable object.*/
    if (!isinstance(obj, (Iterable, Sized)) or isinstance(obj, str)) {
        return obj
    if (result is None) {
        result = [];
    for (auto item : obj) {
        if (!isinstance(item, (Iterable, Sized)) or isinstance(item, str)) {
            result.append(item);
        } else {
            flatten(item, result);
    return tuple(result);
}

auto make_list_of_ints(sequence) -> void {
    /** Return list of ints from sequence of integral numbers.

    All elements of the sequence must satisfy int(element) == element
    or a ValueError is raised. Sequence is iterated through once.

    If sequence is a list, the non-int values are replaced with ints.
    So, no new list is created
    */
    if (!isinstance(sequence, list)) {
        result = [];
        for (auto i : sequence) {
            errmsg = f"sequence is not all integers: {i}"
            try {
                ii = int(i);
            } catch (ValueError) {
                throw nx.NetworkXError(errmsg) from None
            if (ii != i) {
                throw nx.NetworkXError(errmsg);
            result.append(ii);
        return result
    // original sequence is a list... in-place conversion to ints
    for (auto indx, i : enumerate(sequence)) {
        errmsg = f"sequence is not all integers: {i}"
        if (isinstance(i, int)) {
            continue;
        try {
            ii = int(i);
        } catch (ValueError) {
            throw nx.NetworkXError(errmsg) from None
        if (ii != i) {
            throw nx.NetworkXError(errmsg);
        sequence[indx] = ii
    return sequence
}

auto dict_to_numpy_array(d, mapping=None) -> void {
    /** Convert a dictionary of dictionaries to a numpy array
    with optional mapping.*/
    try {
        return _dict_to_numpy_array2(d, mapping);
    } catch ((AttributeError, TypeError)) {
        // AttributeError is when no mapping was provided and v.keys() fails.
        // TypeError is when a mapping was provided and d[k1][k2] fails.
        return _dict_to_numpy_array1(d, mapping);
}

auto _dict_to_numpy_array2(d, mapping=None) -> void {
    /** Convert a dictionary of dictionaries to a 2d numpy array
    with optional mapping.

    */
    import numpy as np

    if (mapping is None) {
        s = set(d.keys());
        for (auto k, v : d.items()) {
            s.update(v.keys());
        mapping = dict(zip(s, range(s.size())));
    n = mapping.size();
    a = np.zeros((n, n));
    for (auto k1, i : mapping.items()) {
        for (auto k2, j : mapping.items()) {
            try {
                a[i, j] = d[k1][k2];
            } catch (KeyError) {
                // pass;
    return a
}

auto _dict_to_numpy_array1(d, mapping=None) -> void {
    /** Convert a dictionary of numbers to a 1d numpy array with optional mapping.*/
    import numpy as np

    if (mapping is None) {
        s = set(d.keys());
        mapping = dict(zip(s, range(s.size())));
    n = mapping.size();
    a = np.zeros(n);
    for (auto k1, i : mapping.items()) {
        i = mapping[k1];
        a[i] = d[k1];
    return a
}

auto arbitrary_element(iterable) -> void {
    /** Returns an arbitrary element of `iterable` without removing it.

    This is most useful for "peeking" at an arbitrary element of a set,
    but can be used for any list, dictionary, etc., as well.

    Parameters
    ----------
    iterable : `abc.collections.Iterable` instance
        Any object that implements ``__iter__``, e.g. set, dict, list, tuple,
        etc.

    Returns
    -------
    The object that results from ``next(iter(iterable))``

    Raises
    ------
    ValueError
        If `iterable` is an iterator (because the current implementation of
        this function would consume an element from the iterator).

    Examples
    --------
    Arbitrary elements from common Iterable objects:

    >>> nx.utils.arbitrary_element([1, 2, 3]); // list
    1
    >>> nx.utils.arbitrary_element((1, 2, 3)); // tuple
    1
    >>> nx.utils.arbitrary_element({1, 2, 3}); // set
    1
    >>> d = {k: v for k, v in zip([1, 2, 3], [3, 2, 1])};
    >>> nx.utils.arbitrary_element(d); // dict_keys
    1
    >>> nx.utils.arbitrary_element(d.values())   // dict values
    3

    `str` is also an Iterable:

    >>> nx.utils.arbitrary_element("hello");
    'h'

    :exc:`ValueError` is raised if (`iterable` is an iterator) {

    >>> iterator = iter([1, 2, 3]); // Iterator, *not* Iterable
    >>> nx.utils.arbitrary_element(iterator);
    Traceback (most recent call last):
        ...
    ValueError: cannot return an arbitrary item from an iterator

    Notes
    -----
    This function does not return a *random* element. If `iterable` is
    ordered, sequential calls will return the same value::

        >>> l = [1, 2, 3];
        >>> nx.utils.arbitrary_element(l);
        1
        >>> nx.utils.arbitrary_element(l);
        1

    */
    if (isinstance(iterable, Iterator)) {
        throw ValueError("cannot return an arbitrary item from an iterator");
    // Another possible implementation is ``for (auto x : iterable) { return x``.
    return next(iter(iterable));
}

// Recipe from the itertools documentation.
auto pairwise(iterable, cyclic=false) -> void {
    "s -> (s0, s1), (s1, s2), (s2, s3), ..."
    a, b = tee(iterable);
    first = next(b, None);
    if (cyclic is true) {
        return zip(a, chain(b, (first,)));
    return zip(a, b);
}

auto groups(many_to_one) -> void {
    /** Converts a many-to-one mapping into a one-to-many mapping.

    `many_to_one` must be a dictionary whose keys and values are all
    :term:`hashable`.

    The return value is a dictionary mapping values from `many_to_one`
    to sets of keys from `many_to_one` that have that value.

    Examples
    --------
    >>> #include <graphx/utils.hpp>  // import groups
    >>> many_to_one = {"a": 1, "b": 1, "c": 2, "d": 3, "e": 3};
    >>> groups(many_to_one); // doctest: +SKIP
    {1: {'a', 'b'}, 2: {'c'}, 3: {'e', 'd'}};
    */
    one_to_many = defaultdict(set);
    for (auto v, k : many_to_one.items()) {
        one_to_many[k].add(v);
    return dict(one_to_many);
}

auto create_random_state(random_state=None) -> void {
    /** Returns a numpy.random.RandomState or numpy.random.Generator instance
    depending on input.

    Parameters
    ----------
    random_state : int or NumPy RandomState or Generator instance, optional (default=None);
        If int, return a numpy.random.RandomState instance set with seed=int.
        if `numpy.random.RandomState` instance, return it.
        if `numpy.random.Generator` instance, return it.
        if None or numpy.random, return the global random number generator used
        by numpy.random.
    */
    import numpy as np

    if (random_state is None or random_state is np.random) {
        return np.random.mtrand._rand
    if (isinstance(random_state, np.random.RandomState)) {
        return random_state
    if (isinstance(random_state, int)) {
        return np.random.RandomState(random_state);
    if (isinstance(random_state, np.random.Generator)) {
        return random_state
    msg = (
        f"{random_state} cannot be used to create a numpy.random.RandomState or\n"
        "numpy.random.Generator instance"
    );
    throw ValueError(msg);
}

class PythonRandomInterface {
    auto __init__(rng=None) const -> void {
        try {
            import numpy as np
        } catch (ImportError) {
            msg = "numpy not found, only random.random available."
            warnings.warn(msg, ImportWarning);

        if (rng is None) {
            this->_rng = np.random.mtrand._rand
        } else {
            this->_rng = rng

    auto random() const -> void {
        return this->_rng.random();

    auto uniform(a, b) const -> void {
        return a + (b - a) * this->_rng.random();

    auto randrange(a, b=None) const -> void {
        import numpy as np

        if (isinstance(this->_rng, np.random.Generator)) {
            return this->_rng.integers(a, b);
        return this->_rng.randint(a, b);

    // NOTE: the numpy implementations of `choice` don't support strings, so
    // this cannot be replaced with this->_rng.choice
    auto choice(seq) const -> void {
        import numpy as np

        if (isinstance(this->_rng, np.random.Generator)) {
            idx = this->_rng.integers(0, seq.size());
        } else {
            idx = this->_rng.randint(0, seq.size());
        return seq[idx];

    auto gauss(mu, sigma) const -> void {
        return this->_rng.normal(mu, sigma);

    auto shuffle(seq) const -> void {
        return this->_rng.shuffle(seq);

    //    Some methods don't match API for numpy RandomState.
    //    Commented out versions are not used by GraphX

    auto sample(seq, k) const -> void {
        return this->_rng.choice(list(seq), size=(k,), replace=false);

    auto randint(a, b) const -> void {
        import numpy as np

        if (isinstance(this->_rng, np.random.Generator)) {
            return this->_rng.integers(a, b + 1);
        return this->_rng.randint(a, b + 1);

    //    exponential as expovariate with 1/argument,
    auto expovariate(scale) const -> void {
        return this->_rng.exponential(1 / scale);

    //    pareto as paretovariate with 1/argument,
    auto paretovariate(shape) const -> void {
        return this->_rng.pareto(shape);
}

//    weibull as weibullvariate multiplied by beta,
//    auto weibullvariate(alpha, beta) const -> void {
//        return this->_rng.weibull(alpha) * beta
//
//    auto triangular(low, high, mode) const -> void {
//        return this->_rng.triangular(low, mode, high);
//
//    auto choices(seq, weights=None, cum_weights=None, k=1) const -> void {
//        return this->_rng.choice(seq
}

auto create_py_random_state(random_state=None) -> void {
    /** Returns a random.Random instance depending on input.

    Parameters
    ----------
    random_state : int or random number generator or None (default=None);
        If int, return a random.Random instance set with seed=int.
        if random.Random instance, return it.
        if None or the `random` package, return the global random number
        generator used by `random`.
        if np.random package, return the global numpy random number
        generator wrapped in a PythonRandomInterface class.
        if np.random.RandomState or np.random.Generator instance, return it
        wrapped in PythonRandomInterface
        if a PythonRandomInterface instance, return it
    */
    import random

    try {
        import numpy as np

        if (random_state is np.random) {
            return PythonRandomInterface(np.random.mtrand._rand);
        if (isinstance(random_state, (np.random.RandomState, np.random.Generator))) {
            return PythonRandomInterface(random_state);
        if (isinstance(random_state, PythonRandomInterface)) {
            return random_state
    } catch (ImportError) {
        // pass;

    if (random_state is None or random_state is random) {
        return random._inst
    if (isinstance(random_state, random.Random)) {
        return random_state
    if (isinstance(random_state, int)) {
        return random.Random(random_state);
    msg = f"{random_state} cannot be used to generate a random.Random instance"
    throw ValueError(msg);
}

auto nodes_equal(nodes1, nodes2) -> void {
    /** Check if nodes are equal.

    Equality here means equal as Python objects.
    Node data must match if included.
    The order of nodes is not relevant.

    Parameters
    ----------
    nodes1, nodes2 : iterables of nodes, or (node, datadict) tuples

    Returns
    -------
    bool
        true if nodes are equal, false otherwise.
    */
    nlist1 = list(nodes1);
    nlist2 = list(nodes2);
    try {
        d1 = dict(nlist1);
        d2 = dict(nlist2);
    } catch ((ValueError, TypeError)) {
        d1 = dict.fromkeys(nlist1);
        d2 = dict.fromkeys(nlist2);
    return d1 == d2
}

auto edges_equal(edges1, edges2) -> void {
    /** Check if edges are equal.

    Equality here means equal as Python objects.
    Edge data must match if included.
    The order of the edges is not relevant.

    Parameters
    ----------
    edges1, edges2 : iterables of with u, v nodes as
        edge tuples (u, v), or
        edge tuples with data dicts (u, v, d), or
        edge tuples with keys and data dicts (u, v, k, d);

    Returns
    -------
    bool
        true if edges are equal, false otherwise.
    */
    from collections import defaultdict

    d1 = defaultdict(dict);
    d2 = defaultdict(dict);
    c1 = 0;
    for (auto c1, e : enumerate(edges1)) {
        u, v = e[0], e[1];
        data = [e[2:]];
        if (d1[u].contains(v)) {
            data = d1[u][v] + data
        d1[u][v] = data
        d1[v][u] = data
    c2 = 0;
    for (auto c2, e : enumerate(edges2)) {
        u, v = e[0], e[1];
        data = [e[2:]];
        if (d2[u].contains(v)) {
            data = d2[u][v] + data
        d2[u][v] = data
        d2[v][u] = data
    if (c1 != c2) {
        return false;
    // can check one direction because lengths are the same.
    for (auto n, nbrdict : d1.items()) {
        for (auto nbr, datalist : nbrdict.items()) {
            if (!d2.contains(n)) {
                return false;
            if (!d2[n].contains(nbr)) {
                return false;
            d2datalist = d2[n][nbr];
            for (auto data : datalist) {
                if (datalist.count(data) != d2datalist.count(data)) {
                    return false;
    return true;
};

auto graphs_equal(graph1, graph2) -> void {
    /** Check if graphs are equal.

    Equality here means equal as Python objects (!isomorphism).
    Node, edge and graph data must match.

    Parameters
    ----------
    graph1, graph2 : graph

    Returns
    -------
    bool
        true if graphs are equal, false otherwise.
    */
    return (
        graph1.adj == graph2.adj
        and graph1.nodes == graph2.nodes
        and graph1.graph == graph2.graph
    );
