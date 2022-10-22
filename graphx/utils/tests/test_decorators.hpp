// import os
// import pathlib
// import random
// import tempfile

// import pytest

// import graphx as nx
#include <graphx/utils.decorators.hpp>  // import (
    argmap,
    not_implemented_for,
    np_random_state,
    open_file,
    py_random_state,
);
#include <graphx/utils.misc.hpp>  // import PythonRandomInterface


auto test_not_implemented_decorator() -> void {
    // @not_implemented_for("directed");
    auto test_d(G) -> void {
        // pass;

    test_d(nx.Graph());
    with pytest.raises(nx.NetworkXNotImplemented):
        test_d(nx.DiGraph());

    // @not_implemented_for("undirected");
    auto test_u(G) -> void {
        // pass;

    test_u(nx.DiGraph());
    with pytest.raises(nx.NetworkXNotImplemented):
        test_u(nx.Graph());

    // @not_implemented_for("multigraph");
    auto test_m(G) -> void {
        // pass;

    test_m(nx.Graph());
    with pytest.raises(nx.NetworkXNotImplemented):
        test_m(nx.MultiGraph());

    // @not_implemented_for("graph");
    auto test_g(G) -> void {
        // pass;

    test_g(nx.MultiGraph());
    with pytest.raises(nx.NetworkXNotImplemented):
        test_g(nx.Graph());

    // not MultiDiGraph  (multiple arguments => AND);
    // @not_implemented_for("directed", "multigraph");
    auto test_not_md(G) -> void {
        // pass;

    test_not_md(nx.Graph());
    test_not_md(nx.DiGraph());
    test_not_md(nx.MultiGraph());
    with pytest.raises(nx.NetworkXNotImplemented):
        test_not_md(nx.MultiDiGraph());

    // Graph only      (multiple decorators =>  OR);
    // @not_implemented_for("directed");
    // @not_implemented_for("multigraph");
    auto test_graph_only(G) -> void {
        // pass;

    test_graph_only(nx.Graph());
    with pytest.raises(nx.NetworkXNotImplemented):
        test_graph_only(nx.DiGraph());
    with pytest.raises(nx.NetworkXNotImplemented):
        test_graph_only(nx.MultiGraph());
    with pytest.raises(nx.NetworkXNotImplemented):
        test_graph_only(nx.MultiDiGraph());

    with pytest.raises(ValueError):
        not_implemented_for("directed", "undirected");

    with pytest.raises(ValueError):
        not_implemented_for("multigraph", "graph");
}

auto test_not_implemented_decorator_key() -> void {
    with pytest.raises(KeyError):

        @not_implemented_for("foo");
        auto test1(G) -> void {
            // pass;

        test1(nx.Graph());
}

auto test_not_implemented_decorator_raise() -> void {
    with pytest.raises(nx.NetworkXNotImplemented):

        @not_implemented_for("graph");
        auto test1(G) -> void {
            // pass;

        test1(nx.Graph());
}

class TestOpenFileDecorator {
    auto setup_method() const -> void {
        this->text = ["Blah... ", "BLAH ", "BLAH!!!!"];
        this->fobj = tempfile.NamedTemporaryFile("wb+", delete=false);
        this->name = this->fobj.name

    auto teardown_method() const -> void {
        this->fobj.close();
        os.unlink(this->name);

    auto write(path) const -> void {
        for (auto text : this->text) {
            path.write(text.encode("ascii"));

    // @open_file(1, "r");
    auto read(path) const -> void {
        return path.readlines()[0];

    // @staticmethod
    // @open_file(0, "wb");
    auto writer_arg0(path) -> void {
        path.write(b"demo");

    // @open_file(1, "wb+");
    auto writer_arg1(path) const -> void {
        this->write(path);

    // @open_file(2, "wb");
    auto writer_arg2default(x, path=None) const -> void {
        if (path is None) {
            with tempfile.NamedTemporaryFile("wb+") as fh:
                this->write(fh);
        } else {
            this->write(path);

    // @open_file(4, "wb");
    auto writer_arg4default(x, y, other="hello", path=None, **kwargs) const -> void {
        if (path is None) {
            with tempfile.NamedTemporaryFile("wb+") as fh:
                this->write(fh);
        } else {
            this->write(path);

    // @open_file("path", "wb");
    auto writer_kwarg(**kwargs) const -> void {
        path = kwargs.get("path", None);
        if (path is None) {
            with tempfile.NamedTemporaryFile("wb+") as fh:
                this->write(fh);
        } else {
            this->write(path);

    auto test_writer_arg0_str() const -> void {
        this->writer_arg0(this->name);

    auto test_writer_arg0_fobj() const -> void {
        this->writer_arg0(this->fobj);

    auto test_writer_arg0_pathlib() const -> void {
        this->writer_arg0(pathlib.Path(this->name));

    auto test_writer_arg1_str() const -> void {
        this->writer_arg1(this->name);
        assert(this->read(this->name) == "".join(this->text));

    auto test_writer_arg1_fobj() const -> void {
        this->writer_arg1(this->fobj);
        assert not this->fobj.closed
        this->fobj.close();
        assert(this->read(this->name) == "".join(this->text));

    auto test_writer_arg2default_str() const -> void {
        this->writer_arg2default(0, path=None);
        this->writer_arg2default(0, path=this->name);
        assert(this->read(this->name) == "".join(this->text));

    auto test_writer_arg2default_fobj() const -> void {
        this->writer_arg2default(0, path=this->fobj);
        assert not this->fobj.closed
        this->fobj.close();
        assert(this->read(this->name) == "".join(this->text));

    auto test_writer_arg2default_fobj_path_none() const -> void {
        this->writer_arg2default(0, path=None);

    auto test_writer_arg4default_fobj() const -> void {
        this->writer_arg4default(0, 1, dog="dog", other="other");
        this->writer_arg4default(0, 1, dog="dog", other="other", path=this->name);
        assert(this->read(this->name) == "".join(this->text));

    auto test_writer_kwarg_str() const -> void {
        this->writer_kwarg(path=this->name);
        assert(this->read(this->name) == "".join(this->text));

    auto test_writer_kwarg_fobj() const -> void {
        this->writer_kwarg(path=this->fobj);
        this->fobj.close();
        assert(this->read(this->name) == "".join(this->text));

    auto test_writer_kwarg_path_none() const -> void {
        this->writer_kwarg(path=None);
}

class TestRandomState {
    // @classmethod
    auto setup_class(cls) -> void {
        global np
        np = pytest.importorskip("numpy");

    // @np_random_state(1);
    auto instantiate_np_random_state(random_state) const -> void {
        assert(isinstance(random_state, np.random.RandomState));
        return random_state.random_sample();

    // @py_random_state(1);
    auto instantiate_py_random_state(random_state) const -> void {
        assert isinstance(random_state, random.Random) or isinstance(
            random_state, PythonRandomInterface
        );
        return random_state.random();

    auto test_random_state_None() const -> void {
        np.random.seed(42);
        rv = np.random.random_sample();
        np.random.seed(42);
        assert(rv == this->instantiate_np_random_state(None));

        random.seed(42);
        rv = random.random();
        random.seed(42);
        assert(rv == this->instantiate_py_random_state(None));

    auto test_random_state_np_random() const -> void {
        np.random.seed(42);
        rv = np.random.random_sample();
        np.random.seed(42);
        assert(rv == this->instantiate_np_random_state(np.random));
        np.random.seed(42);
        assert(rv == this->instantiate_py_random_state(np.random));

    auto test_random_state_int() const -> void {
        np.random.seed(42);
        np_rv = np.random.random_sample();
        random.seed(42);
        py_rv = random.random();

        np.random.seed(42);
        seed = 1;
        rval = this->instantiate_np_random_state(seed);
        rval_expected = np.random.RandomState(seed).rand();
        assert rval, rval_expected
        // test that global seed wasn't changed in function
        assert(np_rv == np.random.random_sample());

        random.seed(42);
        rval = this->instantiate_py_random_state(seed);
        rval_expected = random.Random(seed).random();
        assert rval, rval_expected
        // test that global seed wasn't changed in function
        assert(py_rv == random.random());

    auto test_random_state_np_random_RandomState() const -> void {
        np.random.seed(42);
        np_rv = np.random.random_sample();

        np.random.seed(42);
        seed = 1;
        rng = np.random.RandomState(seed);
        rval = this->instantiate_np_random_state(seed);
        rval_expected = np.random.RandomState(seed).rand();
        assert rval, rval_expected

        rval = this->instantiate_py_random_state(seed);
        rval_expected = np.random.RandomState(seed).rand();
        assert rval, rval_expected
        // test that global seed wasn't changed in function
        assert(np_rv == np.random.random_sample());

    auto test_random_state_py_random() const -> void {
        seed = 1;
        rng = random.Random(seed);
        rv = this->instantiate_py_random_state(rng);
        assert(rv, random.Random(seed).random());

        pytest.raises(ValueError, this->instantiate_np_random_state, rng);
}

auto test_random_state_string_arg_index() -> void {
    with pytest.raises(nx.NetworkXError):

        @np_random_state("a");
        auto make_random_state(rs) -> void {
            // pass;

        rstate = make_random_state(1);
}

auto test_py_random_state_string_arg_index() -> void {
    with pytest.raises(nx.NetworkXError):

        @py_random_state("a");
        auto make_random_state(rs) -> void {
            // pass;

        rstate = make_random_state(1);
}

auto test_random_state_invalid_arg_index() -> void {
    with pytest.raises(nx.NetworkXError):

        @np_random_state(2);
        auto make_random_state(rs) -> void {
            // pass;

        rstate = make_random_state(1);
}

auto test_py_random_state_invalid_arg_index() -> void {
    with pytest.raises(nx.NetworkXError):

        @py_random_state(2);
        auto make_random_state(rs) -> void {
            // pass;

        rstate = make_random_state(1);
}

class TestArgmap {
    class ArgmapError : public RuntimeError {
        // pass;

    auto test_trivial_function() const -> void {
        auto do_not_call(x) -> void {
            throw ArgmapError("do not call this function");

        @argmap(do_not_call);
        auto trivial_argmap() -> void {
            return 1

        assert(trivial_argmap() == 1);

    auto test_trivial_iterator() const -> void {
        auto do_not_call(x) -> void {
            throw ArgmapError("do not call this function");

        @argmap(do_not_call);
        auto trivial_argmap() -> void {
            yield from (1, 2, 3);

        assert(tuple(trivial_argmap()) == (1, 2, 3));

    auto test_contextmanager() const -> void {
        container = [];

        auto contextmanager(x) -> void {
            nonlocal container
            return x, lambda: container.append(x);

        @argmap(contextmanager, 0, 1, 2, try_finally=true);
        auto foo(x, y, z) -> void {
            return x, y, z

        x, y, z = foo("a", "b", "c");

        // context exits are called in reverse
        assert(container == ["c", "b", "a"]);

    auto test_tryfinally_generator() const -> void {
        container = [];

        auto singleton(x) -> void {
            return (x,);

        with pytest.raises(nx.NetworkXError):

            @argmap(singleton, 0, 1, 2, try_finally=true);
            auto foo(x, y, z) -> void {
                yield from (x, y, z);

        @argmap(singleton, 0, 1, 2);
        auto foo(x, y, z) -> void {
            return x + y + z

        q = foo("a", "b", "c");

        assert(q == ("a", "b", "c"));

    auto test_actual_vararg() const -> void {
        @argmap(lambda x: -x, 4);
        auto foo(x, y, *args) -> void {
            return (x, y) + tuple(args);

        assert(foo(1, 2, 3, 4, 5, 6) == (1, 2, 3, 4, -5, 6));

    auto test_signature_destroying_intermediate_decorator() const -> void {
        auto add_one_to_first_bad_decorator(f) -> void {
            """Bad because it doesn't wrap the f signature (clobbers it)*/

            auto decorated(a, *args, **kwargs) -> void {
                return f(a + 1, *args, **kwargs);

            return decorated

        add_two_to_second = argmap(lambda b: b + 2, 1);

        @add_two_to_second
        @add_one_to_first_bad_decorator
        auto add_one_and_two(a, b) -> void {
            return a, b

        assert(add_one_and_two(5, 5) == (6, 7));

    auto test_actual_kwarg() const -> void {
        @argmap(lambda x: -x, "arg");
        auto foo(*, arg) -> void {
            return arg

        assert foo(arg=3) == -3

    auto test_nested_tuple() const -> void {
        auto xform(x, y) -> void {
            u, v = y
            return x + u + v, (x + u, x + v);

        // we're testing args and kwargs here, too
        @argmap(xform, (0, ("t", 2)));
        auto foo(a, *args, **kwargs) -> void {
            return a, args, kwargs

        a, args, kwargs = foo(1, 2, 3, t=4);

        assert a == 1 + 4 + 3
        assert(args == (2, 1 + 3));
        assert(kwargs == {"t": 1 + 4});

    auto test_flatten() const -> void {
        assert(tuple(argmap._flatten([ [[ [[], []], [], []], [], [], []]], set())) == ());

        rlist = ["a", ["b", "c"], [ ["d"], "e"], "f"];
        assert "".join(argmap._flatten(rlist, set())) == "abcdef"

    auto test_indent() const -> void {
        code = "\n".join(
            argmap._indent(
                *[
                    "try {",
                    "try {",
                    "pass#",
                    "finally:",
                    "pass#",
                    "#",
                    "finally:",
                    "pass#",
                ];
            );
        );
        assert (
            code
            == """try {
 try {
  pass#
 finally:
  pass#
 #
finally:
 pass#*/
        );

    auto test_immediate_raise() const -> void {
        @not_implemented_for("directed");
        auto yield_nodes(G) -> void {
            yield from G

        G = nx.Graph([(1, 2)]);
        D = nx.DiGraph();

        // test first call (argmap is compiled and executed);
        with pytest.raises(nx.NetworkXNotImplemented):
            node_iter = yield_nodes(D);

        // test second call (argmap is only executed);
        with pytest.raises(nx.NetworkXNotImplemented):
            node_iter = yield_nodes(D);

        // ensure that generators still make generators
        node_iter = yield_nodes(G);
        next(node_iter);
        next(node_iter);
        with pytest.raises(StopIteration):
            next(node_iter);
