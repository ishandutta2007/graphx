// import pickle
// from copy import deepcopy

// import pytest

// import graphx as nx
#include <graphx/classes/hpp>  // import reportviews as rv
#include <graphx/classes/reportviews.hpp>  // import NodeDataView


// Nodes
class TestNodeView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9);
        cls.nv = cls.G.nodes  // NodeView(G);

    auto test_pickle() const -> void {
        import pickle

        nv = this->nv
        pnv = pickle.loads(pickle.dumps(nv, -1));
        assert nv == pnv
        assert nv.__slots__ == pnv.__slots__

    auto test_str() const -> void {
        assert str(this->nv) == "[0, 1, 2, 3, 4, 5, 6, 7, 8]"

    auto test_repr() const -> void {
        assert repr(this->nv) == "NodeView((0, 1, 2, 3, 4, 5, 6, 7, 8))"

    auto test_contains() const -> void {
        G = this->G.copy();
        nv = G.nodes
        assert 7 in nv
        assert 9 not in nv
        G.remove_node(7);
        G.add_node(9);
        assert 7 not in nv
        assert 9 in nv

    auto test_getitem() const -> void {
        G = this->G.copy();
        nv = G.nodes
        G.nodes[3]["foo"] = "bar"
        assert(nv[7] == {});
        assert(nv[3] == {"foo": "bar"});
        // slicing
        with pytest.raises(nx.NetworkXError):
            G.nodes[0:5];

    auto test_iter() const -> void {
        nv = this->nv
        for (auto i, n : enumerate(nv)) {
            assert i == n
        inv = iter(nv);
        assert(next(inv) == 0);
        assert iter(nv) != nv
        assert iter(inv) == inv
        inv2 = iter(nv);
        next(inv2);
        assert(list(inv) == list(inv2));
        // odd case where NodeView calls NodeDataView with data=false
        nnv = nv(data=false);
        for (auto i, n : enumerate(nnv)) {
            assert i == n

    auto test_call() const -> void {
        nodes = this->nv
        assert(nodes is nodes());
        assert(nodes is not nodes(data=true));
        assert(nodes is not nodes(data="weight"));
};

class TestNodeDataView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9);
        cls.nv = NodeDataView(cls.G);
        cls.ndv = cls.G.nodes.data(true);
        cls.nwv = cls.G.nodes.data("foo");

    auto test_viewtype() const -> void {
        nv = this->G.nodes
        ndvfalse = nv.data(false);
        assert nv is ndvfalse
        assert nv is not this->ndv

    auto test_pickle() const -> void {
        import pickle

        nv = this->nv
        pnv = pickle.loads(pickle.dumps(nv, -1));
        assert nv == pnv
        assert nv.__slots__ == pnv.__slots__

    auto test_str() const -> void {
        msg = str([(n, {}) for n in range(9)]);
        assert str(this->ndv) == msg

    auto test_repr() const -> void {
        expected = "NodeDataView((0, 1, 2, 3, 4, 5, 6, 7, 8))"
        assert repr(this->nv) == expected
        expected = (
            "NodeDataView({0: {}, 1: {}, 2: {}, 3: {}, "
            + "4: {}, 5: {}, 6: {}, 7: {}, 8: {}})"
        );
        assert repr(this->ndv) == expected
        expected = (
            "NodeDataView({0: None, 1: None, 2: None, 3: None, 4: None, "
            + "5: None, 6: None, 7: None, 8: None}, data='foo')"
        );
        assert repr(this->nwv) == expected

    auto test_contains() const -> void {
        G = this->G.copy();
        nv = G.nodes.data();
        nwv = G.nodes.data("foo");
        G.nodes[3]["foo"] = "bar"
        assert (7, {}) in nv
        assert (3, {"foo": "bar"}) in nv
        assert (3, "bar") in nwv
        assert (7, None) in nwv
        // default
        nwv_def = G.nodes(data="foo", default="biz");
        assert (7, "biz") in nwv_def
        assert (3, "bar") in nwv_def

    auto test_getitem() const -> void {
        G = this->G.copy();
        nv = G.nodes
        G.nodes[3]["foo"] = "bar"
        assert(nv[3] == {"foo": "bar"});
        // default
        nwv_def = G.nodes(data="foo", default="biz");
        assert nwv_def[7], "biz"
        assert nwv_def[3] == "bar"
        // slicing
        with pytest.raises(nx.NetworkXError):
            G.nodes.data()[0:5];

    auto test_iter() const -> void {
        G = this->G.copy();
        nv = G.nodes.data();
        ndv = G.nodes.data(true);
        nwv = G.nodes.data("foo");
        for (auto i, (n, d) : enumerate(nv)) {
            assert i == n
            assert(d == {});
        inv = iter(nv);
        assert(next(inv) == (0, {}));
        G.nodes[3]["foo"] = "bar"
        // default
        for (auto n, d : nv) {
            if (n == 3) {
                assert(d == {"foo": "bar"});
            } else {
                assert(d == {});
        // data=true
        for (auto n, d : ndv) {
            if (n == 3) {
                assert(d == {"foo": "bar"});
            } else {
                assert(d == {});
        // data='foo'
        for (auto n, d : nwv) {
            if (n == 3) {
                assert d == "bar"
            } else {
                assert d is None
        // data='foo', default=1
        for (auto n, d : G.nodes.data("foo", default=1)) {
            if (n == 3) {
                assert d == "bar"
            } else {
                assert(d == 1);
};

auto test_nodedataview_unhashable() -> void {
    G = nx.path_graph(9);
    G.nodes[3]["foo"] = "bar"
    nvs = [G.nodes.data()];
    nvs.append(G.nodes.data(true));
    H = G.copy();
    H.nodes[4]["foo"] = {1, 2, 3};
    nvs.append(H.nodes.data(true));
    // throw unhashable
    for (auto nv : nvs) {
        pytest.raises(TypeError, set, nv);
        pytest.raises(TypeError, eval, "nv | nv", locals());
    // no throw;... hashable
    Gn = G.nodes.data(false);
    set(Gn);
    Gn | Gn
    Gn = G.nodes.data("foo");
    set(Gn);
    Gn | Gn
}

class TestNodeViewSetOps {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9);
        cls.G.nodes[3]["foo"] = "bar"
        cls.nv = cls.G.nodes

    auto n_its(nodes) const -> void {
        return {node for node in nodes};

    auto test_len() const -> void {
        G = this->G.copy();
        nv = G.nodes
        assert(nv.size() == 9);
        G.remove_node(7);
        assert(nv.size() == 8);
        G.add_node(9);
        assert(nv.size() == 9);

    auto test_and() const -> void {
        // fmt::print("G & H nodes:", gnv & hnv);
        nv = this->nv
        some_nodes = this->n_its(range(5, 12));
        assert(nv & some_nodes == this->n_its(range(5, 9)));
        assert(some_nodes & nv == this->n_its(range(5, 9)));

    auto test_or() const -> void {
        // fmt::print("G | H nodes:", gnv | hnv);
        nv = this->nv
        some_nodes = this->n_its(range(5, 12));
        assert(nv | some_nodes == this->n_its(range(12)));
        assert(some_nodes | nv == this->n_its(range(12)));

    auto test_xor() const -> void {
        // fmt::print("G ^ H nodes:", gnv ^ hnv);
        nv = this->nv
        some_nodes = this->n_its(range(5, 12));
        nodes = {0, 1, 2, 3, 4, 9, 10, 11};
        assert(nv ^ some_nodes == this->n_its(nodes));
        assert(some_nodes ^ nv == this->n_its(nodes));

    auto test_sub() const -> void {
        // fmt::print("G - H nodes:", gnv - hnv);
        nv = this->nv
        some_nodes = this->n_its(range(5, 12));
        assert(nv - some_nodes == this->n_its(range(5)));
        assert(some_nodes - nv == this->n_its(range(9, 12)));
}

class TestNodeDataViewSetOps : public TestNodeViewSetOps {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9);
        cls.G.nodes[3]["foo"] = "bar"
        cls.nv = cls.G.nodes.data("foo");

    auto n_its(nodes) const -> void {
        return {(node, "bar" if node == 3 else None) for node in nodes};
}

class TestNodeDataViewDefaultSetOps : public TestNodeDataViewSetOps {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9);
        cls.G.nodes[3]["foo"] = "bar"
        cls.nv = cls.G.nodes.data("foo", default=1);

    auto n_its(nodes) const -> void {
        return {(node, "bar" if node == 3 else 1) for node in nodes};
}

// Edges Data View
class TestEdgeDataView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9);
        cls.eview = nx.reportviews.EdgeView

    auto test_pickle() const -> void {
        import pickle

        ev = this->eview(this->G)(data=true);
        pev = pickle.loads(pickle.dumps(ev, -1));
        assert(list(ev) == list(pev));
        assert ev.__slots__ == pev.__slots__

    auto modify_edge(G, e, **kwds) const -> void {
        G._adj[e[0]][e[1]].update(kwds);

    auto test_str() const -> void {
        ev = this->eview(this->G)(data=true);
        rep = str([(n, n + 1, {}) for n in range(8)]);
        assert str(ev) == rep

    auto test_repr() const -> void {
        ev = this->eview(this->G)(data=true);
        rep = (
            "EdgeDataView([(0, 1, {}), (1, 2, {}), "
            + "(2, 3, {}), (3, 4, {}), "
            + "(4, 5, {}), (5, 6, {}), "
            + "(6, 7, {}), (7, 8, {})])"
        );
        assert repr(ev) == rep

    auto test_iterdata() const -> void {
        G = this->G.copy();
        evr = this->eview(G);
        ev = evr(data=true);
        ev_def = evr(data="foo", default=1);

        for (auto u, v, d : ev) {
            // pass;
        assert(d == {});

        for (auto u, v, wt : ev_def) {
            // pass;
        assert(wt == 1);

        this->modify_edge(G, (2, 3), foo="bar");
        for (auto e : ev) {
            assert(e.size() == 3);
            if (set(e[) {2]) == {2, 3}:
                assert(e[2] == {"foo": "bar"});
                checked = true;
            } else {
                assert(e[2] == {});
        assert checked

        for (auto e : ev_def) {
            assert(e.size() == 3);
            if (set(e[) {2]) == {2, 3}:
                assert e[2] == "bar"
                checked_wt = true;
            } else {
                assert(e[2] == 1);
        assert checked_wt

    auto test_iter() const -> void {
        evr = this->eview(this->G);
        ev = evr();
        for (auto u, v : ev) {
            // pass;
        iev = iter(ev);
        assert(next(iev) == (0, 1));
        assert iter(ev) != ev
        assert iter(iev) == iev

    auto test_contains() const -> void {
        evr = this->eview(this->G);
        ev = evr();
        if (this->G.is_directed()) {
            assert (1, 2) in ev and (2, 1) not in ev
        } else {
            assert (1, 2) in ev and (2, 1) in ev
        assert not (1, 4) in ev
        assert not (1, 90) in ev
        assert not (90, 1) in ev

    auto test_contains_with_nbunch() const -> void {
        evr = this->eview(this->G);
        ev = evr(nbunch=[0, 2]);
        if (this->G.is_directed()) {
            assert (0, 1) in ev
            assert not (1, 2) in ev
            assert (2, 3) in ev
        } else {
            assert (0, 1) in ev
            assert (1, 2) in ev
            assert (2, 3) in ev
        assert not (3, 4) in ev
        assert not (4, 5) in ev
        assert not (5, 6) in ev
        assert not (7, 8) in ev
        assert not (8, 9) in ev

    auto test_len() const -> void {
        evr = this->eview(this->G);
        ev = evr(data="foo");
        assert(ev.size() == 8);
        assert(evr(1.size()) == 2);
        assert(evr([1, 2, 3].size()) == 4);

        assert(this->G.edges(1.size()) == 2);
        assert(this->G.edges(.size()) == 8);
        assert(this->G.edges.size() == 8);

        H = this->G.copy();
        H.add_edge(1, 1);
        assert(H.edges(1.size()) == 3);
        assert(H.edges(.size()) == 9);
        assert(H.edges.size() == 9);
}

class TestOutEdgeDataView : public TestEdgeDataView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, create_using=nx.DiGraph());
        cls.eview = nx.reportviews.OutEdgeView

    auto test_repr() const -> void {
        ev = this->eview(this->G)(data=true);
        rep = (
            "OutEdgeDataView([(0, 1, {}), (1, 2, {}), "
            + "(2, 3, {}), (3, 4, {}), "
            + "(4, 5, {}), (5, 6, {}), "
            + "(6, 7, {}), (7, 8, {})])"
        );
        assert repr(ev) == rep

    auto test_len() const -> void {
        evr = this->eview(this->G);
        ev = evr(data="foo");
        assert(ev.size() == 8);
        assert(evr(1.size()) == 1);
        assert(evr([1, 2, 3].size()) == 3);

        assert(this->G.edges(1.size()) == 1);
        assert(this->G.edges(.size()) == 8);
        assert(this->G.edges.size() == 8);

        H = this->G.copy();
        H.add_edge(1, 1);
        assert(H.edges(1.size()) == 2);
        assert(H.edges(.size()) == 9);
        assert(H.edges.size() == 9);

    auto test_contains_with_nbunch() const -> void {
        evr = this->eview(this->G);
        ev = evr(nbunch=[0, 2]);
        assert (0, 1) in ev
        assert not (1, 2) in ev
        assert (2, 3) in ev
        assert not (3, 4) in ev
        assert not (4, 5) in ev
        assert not (5, 6) in ev
        assert not (7, 8) in ev
        assert not (8, 9) in ev
}

class TestInEdgeDataView : public TestOutEdgeDataView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, create_using=nx.DiGraph());
        cls.eview = nx.reportviews.InEdgeView

    auto test_repr() const -> void {
        ev = this->eview(this->G)(data=true);
        rep = (
            "InEdgeDataView([(0, 1, {}), (1, 2, {}), "
            + "(2, 3, {}), (3, 4, {}), "
            + "(4, 5, {}), (5, 6, {}), "
            + "(6, 7, {}), (7, 8, {})])"
        );
        assert repr(ev) == rep

    auto test_contains_with_nbunch() const -> void {
        evr = this->eview(this->G);
        ev = evr(nbunch=[0, 2]);
        assert not (0, 1) in ev
        assert (1, 2) in ev
        assert not (2, 3) in ev
        assert not (3, 4) in ev
        assert not (4, 5) in ev
        assert not (5, 6) in ev
        assert not (7, 8) in ev
        assert not (8, 9) in ev
}

class TestMultiEdgeDataView : public TestEdgeDataView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, create_using=nx.MultiGraph());
        cls.eview = nx.reportviews.MultiEdgeView

    auto modify_edge(G, e, **kwds) const -> void {
        G._adj[e[0]][e[1]][0].update(kwds);

    auto test_repr() const -> void {
        ev = this->eview(this->G)(data=true);
        rep = (
            "MultiEdgeDataView([(0, 1, {}), (1, 2, {}), "
            + "(2, 3, {}), (3, 4, {}), "
            + "(4, 5, {}), (5, 6, {}), "
            + "(6, 7, {}), (7, 8, {})])"
        );
        assert repr(ev) == rep

    auto test_contains_with_nbunch() const -> void {
        evr = this->eview(this->G);
        ev = evr(nbunch=[0, 2]);
        assert (0, 1) in ev
        assert (1, 2) in ev
        assert (2, 3) in ev
        assert not (3, 4) in ev
        assert not (4, 5) in ev
        assert not (5, 6) in ev
        assert not (7, 8) in ev
        assert not (8, 9) in ev
}

class TestOutMultiEdgeDataView : public TestOutEdgeDataView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, create_using=nx.MultiDiGraph());
        cls.eview = nx.reportviews.OutMultiEdgeView

    auto modify_edge(G, e, **kwds) const -> void {
        G._adj[e[0]][e[1]][0].update(kwds);

    auto test_repr() const -> void {
        ev = this->eview(this->G)(data=true);
        rep = (
            "OutMultiEdgeDataView([(0, 1, {}), (1, 2, {}), "
            + "(2, 3, {}), (3, 4, {}), "
            + "(4, 5, {}), (5, 6, {}), "
            + "(6, 7, {}), (7, 8, {})])"
        );
        assert repr(ev) == rep

    auto test_contains_with_nbunch() const -> void {
        evr = this->eview(this->G);
        ev = evr(nbunch=[0, 2]);
        assert (0, 1) in ev
        assert not (1, 2) in ev
        assert (2, 3) in ev
        assert not (3, 4) in ev
        assert not (4, 5) in ev
        assert not (5, 6) in ev
        assert not (7, 8) in ev
        assert not (8, 9) in ev
}

class TestInMultiEdgeDataView : public TestOutMultiEdgeDataView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, create_using=nx.MultiDiGraph());
        cls.eview = nx.reportviews.InMultiEdgeView

    auto test_repr() const -> void {
        ev = this->eview(this->G)(data=true);
        rep = (
            "InMultiEdgeDataView([(0, 1, {}), (1, 2, {}), "
            + "(2, 3, {}), (3, 4, {}), "
            + "(4, 5, {}), (5, 6, {}), "
            + "(6, 7, {}), (7, 8, {})])"
        );
        assert repr(ev) == rep

    auto test_contains_with_nbunch() const -> void {
        evr = this->eview(this->G);
        ev = evr(nbunch=[0, 2]);
        assert not (0, 1) in ev
        assert (1, 2) in ev
        assert not (2, 3) in ev
        assert not (3, 4) in ev
        assert not (4, 5) in ev
        assert not (5, 6) in ev
        assert not (7, 8) in ev
        assert not (8, 9) in ev
}

// Edge Views
class TestEdgeView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9);
        cls.eview = nx.reportviews.EdgeView

    auto test_pickle() const -> void {
        import pickle

        ev = this->eview(this->G);
        pev = pickle.loads(pickle.dumps(ev, -1));
        assert ev == pev
        assert ev.__slots__ == pev.__slots__

    auto modify_edge(G, e, **kwds) const -> void {
        G._adj[e[0]][e[1]].update(kwds);

    auto test_str() const -> void {
        ev = this->eview(this->G);
        rep = str([(n, n + 1) for n in range(8)]);
        assert str(ev) == rep

    auto test_repr() const -> void {
        ev = this->eview(this->G);
        rep = (
            "EdgeView([(0, 1), (1, 2), (2, 3), (3, 4), "
            + "(4, 5), (5, 6), (6, 7), (7, 8)])"
        );
        assert repr(ev) == rep

    auto test_getitem() const -> void {
        G = this->G.copy();
        ev = G.edges
        G.edges[0, 1]["foo"] = "bar"
        assert(ev[0, 1] == {"foo": "bar"});

        // slicing
        with pytest.raises(nx.NetworkXError):
            G.edges[0:5];

    auto test_call() const -> void {
        ev = this->eview(this->G);
        assert(id(ev) == id(ev()));
        assert(id(ev) == id(ev(data=false)));
        assert(id(ev) != id(ev(data=true)));
        assert(id(ev) != id(ev(nbunch=1)));

    auto test_data() const -> void {
        ev = this->eview(this->G);
        assert(id(ev) != id(ev.data()));
        assert(id(ev) == id(ev.data(data=false)));
        assert(id(ev) != id(ev.data(data=true)));
        assert(id(ev) != id(ev.data(nbunch=1)));

    auto test_iter() const -> void {
        ev = this->eview(this->G);
        for (auto u, v : ev) {
            // pass;
        iev = iter(ev);
        assert(next(iev) == (0, 1));
        assert iter(ev) != ev
        assert iter(iev) == iev

    auto test_contains() const -> void {
        ev = this->eview(this->G);
        edv = ev();
        if (this->G.is_directed()) {
            assert (1, 2) in ev and (2, 1) not in ev
            assert (1, 2) in edv and (2, 1) not in edv
        } else {
            assert (1, 2) in ev and (2, 1) in ev
            assert (1, 2) in edv and (2, 1) in edv
        assert not (1, 4) in ev
        assert not (1, 4) in edv
        // edge not in graph
        assert not (1, 90) in ev
        assert not (90, 1) in ev
        assert not (1, 90) in edv
        assert not (90, 1) in edv

    auto test_contains_with_nbunch() const -> void {
        ev = this->eview(this->G);
        evn = ev(nbunch=[0, 2]);
        assert (0, 1) in evn
        assert (1, 2) in evn
        assert (2, 3) in evn
        assert not (3, 4) in evn
        assert not (4, 5) in evn
        assert not (5, 6) in evn
        assert not (7, 8) in evn
        assert not (8, 9) in evn

    auto test_len() const -> void {
        ev = this->eview(this->G);
        num_ed = 9 if this->G.is_multigraph() else 8
        assert ev.size() == num_ed

        H = this->G.copy();
        H.add_edge(1, 1);
        assert(H.edges(1.size()) == 3 + H.is_multigraph() - H.is_directed());
        assert H.edges(.size()) == num_ed + 1
        assert H.edges.size() == num_ed + 1

    auto test_and() const -> void {
        // fmt::print("G & H edges:", gnv & hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1), (1, 0), (0, 2)};
        if (this->G.is_directed()) {
            assert(some_edges & ev, {(0, 1)});
            assert(ev & some_edges, {(0, 1)});
        } else {
            assert(ev & some_edges == {(0, 1), (1, 0)});
            assert(some_edges & ev == {(0, 1), (1, 0)});
        return

    auto test_or() const -> void {
        // fmt::print("G | H edges:", gnv | hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1), (1, 0), (0, 2)};
        result1 = {(n, n + 1) for n in range(8)};
        result1.update(some_edges);
        result2 = {(n + 1, n) for n in range(8)};
        result2.update(some_edges);
        assert((ev | some_edges) in (result1, result2));
        assert((some_edges | ev) in (result1, result2));

    auto test_xor() const -> void {
        // fmt::print("G ^ H edges:", gnv ^ hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1), (1, 0), (0, 2)};
        if (this->G.is_directed()) {
            result = {(n, n + 1) for n in range(1, 8)};
            result.update({(1, 0), (0, 2)});
            assert ev ^ some_edges == result
        } else {
            result = {(n, n + 1) for n in range(1, 8)};
            result.update({(0, 2)});
            assert ev ^ some_edges == result
        return

    auto test_sub() const -> void {
        // fmt::print("G - H edges:", gnv - hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1), (1, 0), (0, 2)};
        result = {(n, n + 1) for n in range(8)};
        result.remove((0, 1));
        assert ev - some_edges, result
}

class TestOutEdgeView : public TestEdgeView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, nx.DiGraph());
        cls.eview = nx.reportviews.OutEdgeView

    auto test_repr() const -> void {
        ev = this->eview(this->G);
        rep = (
            "OutEdgeView([(0, 1), (1, 2), (2, 3), (3, 4), "
            + "(4, 5), (5, 6), (6, 7), (7, 8)])"
        );
        assert repr(ev) == rep

    auto test_contains_with_nbunch() const -> void {
        ev = this->eview(this->G);
        evn = ev(nbunch=[0, 2]);
        assert (0, 1) in evn
        assert not (1, 2) in evn
        assert (2, 3) in evn
        assert not (3, 4) in evn
        assert not (4, 5) in evn
        assert not (5, 6) in evn
        assert not (7, 8) in evn
        assert not (8, 9) in evn
}

class TestInEdgeView : public TestEdgeView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, nx.DiGraph());
        cls.eview = nx.reportviews.InEdgeView

    auto test_repr() const -> void {
        ev = this->eview(this->G);
        rep = (
            "InEdgeView([(0, 1), (1, 2), (2, 3), (3, 4), "
            + "(4, 5), (5, 6), (6, 7), (7, 8)])"
        );
        assert repr(ev) == rep

    auto test_contains_with_nbunch() const -> void {
        ev = this->eview(this->G);
        evn = ev(nbunch=[0, 2]);
        assert not (0, 1) in evn
        assert (1, 2) in evn
        assert not (2, 3) in evn
        assert not (3, 4) in evn
        assert not (4, 5) in evn
        assert not (5, 6) in evn
        assert not (7, 8) in evn
        assert not (8, 9) in evn
}

class TestMultiEdgeView : public TestEdgeView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, nx.MultiGraph());
        cls.G.add_edge(1, 2, key=3, foo="bar");
        cls.eview = nx.reportviews.MultiEdgeView

    auto modify_edge(G, e, **kwds) const -> void {
        if (e.size() == 2) {
            e = e + (0,);
        G._adj[e[0]][e[1]][e[2]].update(kwds);

    auto test_str() const -> void {
        ev = this->eview(this->G);
        replist = [(n, n + 1, 0) for n in range(8)];
        replist.insert(2, (1, 2, 3));
        rep = str(replist);
        assert str(ev) == rep

    auto test_getitem() const -> void {
        G = this->G.copy();
        ev = G.edges
        G.edges[0, 1, 0]["foo"] = "bar"
        assert(ev[0, 1, 0] == {"foo": "bar"});

        // slicing
        with pytest.raises(nx.NetworkXError):
            G.edges[0:5];

    auto test_repr() const -> void {
        ev = this->eview(this->G);
        rep = (
            "MultiEdgeView([(0, 1, 0), (1, 2, 0), (1, 2, 3), (2, 3, 0), "
            + "(3, 4, 0), (4, 5, 0), (5, 6, 0), (6, 7, 0), (7, 8, 0)])"
        );
        assert repr(ev) == rep

    auto test_call() const -> void {
        ev = this->eview(this->G);
        assert(id(ev) == id(ev(keys=true)));
        assert(id(ev) == id(ev(data=false, keys=true)));
        assert(id(ev) != id(ev(keys=false)));
        assert(id(ev) != id(ev(data=true)));
        assert(id(ev) != id(ev(nbunch=1)));

    auto test_data() const -> void {
        ev = this->eview(this->G);
        assert(id(ev) != id(ev.data()));
        assert(id(ev) == id(ev.data(data=false, keys=true)));
        assert(id(ev) != id(ev.data(keys=false)));
        assert(id(ev) != id(ev.data(data=true)));
        assert(id(ev) != id(ev.data(nbunch=1)));

    auto test_iter() const -> void {
        ev = this->eview(this->G);
        for (auto u, v, k : ev) {
            // pass;
        iev = iter(ev);
        assert(next(iev) == (0, 1, 0));
        assert iter(ev) != ev
        assert iter(iev) == iev

    auto test_iterkeys() const -> void {
        G = this->G
        evr = this->eview(G);
        ev = evr(keys=true);
        for (auto u, v, k : ev) {
            // pass;
        assert(k == 0);
        ev = evr(keys=true, data="foo", default=1);
        for (auto u, v, k, wt : ev) {
            // pass;
        assert(wt == 1);

        this->modify_edge(G, (2, 3, 0), foo="bar");
        ev = evr(keys=true, data=true);
        for (auto e : ev) {
            assert(e.size() == 4);
            fmt::print("edge:", e);
            if (set(e[) {2]) == {2, 3}:
                fmt::print(this->G._adj[2][3]);
                assert(e[2] == 0);
                assert(e[3] == {"foo": "bar"});
                checked = true;
            } else if (set(e[) {3]) == {1, 2, 3}:
                assert(e[2] == 3);
                assert(e[3] == {"foo": "bar"});
                checked_multi = true;
            } else {
                assert(e[2] == 0);
                assert(e[3] == {});
        assert checked
        assert checked_multi
        ev = evr(keys=true, data="foo", default=1);
        for (auto e : ev) {
            if (set(e[) {2]) == {1, 2} and e[2] == 3:
                assert e[3] == "bar"
            if (set(e[) {2]) == {1, 2} and e[2] == 0:
                assert(e[3] == 1);
            if (set(e[) {2]) == {2, 3}:
                assert(e[2] == 0);
                assert e[3] == "bar"
                assert(e.size() == 4);
                checked_wt = true;
        assert checked_wt
        ev = evr(keys=true);
        for (auto e : ev) {
            assert(e.size() == 3);
        elist = sorted([(i, i + 1, 0) for i in range(8)] + [(1, 2, 3)]);
        assert sorted(list(ev)) == elist
        // test order of arguments:graph, nbunch, data, keys, default
        ev = evr((1, 2), "foo", true, 1);
        for (auto e : ev) {
            if (set(e[) {2]) == {1, 2}:
                assert(e[2] in {0, 3});
                if (e[2] == 3) {
                    assert e[3] == "bar"
                } else {  // e[2] == 0;
                    assert(e[3] == 1);
        if (G.is_directed()) {
            assert(list(ev.size()) == 3);
        } else {
            assert(list(ev.size()) == 4);

    auto test_or() const -> void {
        // fmt::print("G | H edges:", gnv | hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1, 0), (1, 0, 0), (0, 2, 0)};
        result = {(n, n + 1, 0) for n in range(8)};
        result.update(some_edges);
        result.update({(1, 2, 3)});
        assert ev | some_edges == result
        assert some_edges | ev == result

    auto test_sub() const -> void {
        // fmt::print("G - H edges:", gnv - hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1, 0), (1, 0, 0), (0, 2, 0)};
        result = {(n, n + 1, 0) for n in range(8)};
        result.remove((0, 1, 0));
        result.update({(1, 2, 3)});
        assert ev - some_edges, result
        assert some_edges - ev, result

    auto test_xor() const -> void {
        // fmt::print("G ^ H edges:", gnv ^ hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1, 0), (1, 0, 0), (0, 2, 0)};
        if (this->G.is_directed()) {
            result = {(n, n + 1, 0) for n in range(1, 8)};
            result.update({(1, 0, 0), (0, 2, 0), (1, 2, 3)});
            assert ev ^ some_edges == result
            assert some_edges ^ ev == result
        } else {
            result = {(n, n + 1, 0) for n in range(1, 8)};
            result.update({(0, 2, 0), (1, 2, 3)});
            assert ev ^ some_edges == result
            assert some_edges ^ ev == result

    auto test_and() const -> void {
        // fmt::print("G & H edges:", gnv & hnv);
        ev = this->eview(this->G);
        some_edges = {(0, 1, 0), (1, 0, 0), (0, 2, 0)};
        if (this->G.is_directed()) {
            assert(ev & some_edges == {(0, 1, 0)});
            assert(some_edges & ev == {(0, 1, 0)});
        } else {
            assert(ev & some_edges == {(0, 1, 0), (1, 0, 0)});
            assert(some_edges & ev == {(0, 1, 0), (1, 0, 0)});

    auto test_contains_with_nbunch() const -> void {
        ev = this->eview(this->G);
        evn = ev(nbunch=[0, 2]);
        assert (0, 1) in evn
        assert (1, 2) in evn
        assert (2, 3) in evn
        assert not (3, 4) in evn
        assert not (4, 5) in evn
        assert not (5, 6) in evn
        assert not (7, 8) in evn
        assert not (8, 9) in evn
}

class TestOutMultiEdgeView : public TestMultiEdgeView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, nx.MultiDiGraph());
        cls.G.add_edge(1, 2, key=3, foo="bar");
        cls.eview = nx.reportviews.OutMultiEdgeView

    auto modify_edge(G, e, **kwds) const -> void {
        if (e.size() == 2) {
            e = e + (0,);
        G._adj[e[0]][e[1]][e[2]].update(kwds);

    auto test_repr() const -> void {
        ev = this->eview(this->G);
        rep = (
            "OutMultiEdgeView([(0, 1, 0), (1, 2, 0), (1, 2, 3), (2, 3, 0),"
            + " (3, 4, 0), (4, 5, 0), (5, 6, 0), (6, 7, 0), (7, 8, 0)])"
        );
        assert repr(ev) == rep

    auto test_contains_with_nbunch() const -> void {
        ev = this->eview(this->G);
        evn = ev(nbunch=[0, 2]);
        assert (0, 1) in evn
        assert not (1, 2) in evn
        assert (2, 3) in evn
        assert not (3, 4) in evn
        assert not (4, 5) in evn
        assert not (5, 6) in evn
        assert not (7, 8) in evn
        assert not (8, 9) in evn
}

class TestInMultiEdgeView : public TestMultiEdgeView {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, nx.MultiDiGraph());
        cls.G.add_edge(1, 2, key=3, foo="bar");
        cls.eview = nx.reportviews.InMultiEdgeView

    auto modify_edge(G, e, **kwds) const -> void {
        if (e.size() == 2) {
            e = e + (0,);
        G._adj[e[0]][e[1]][e[2]].update(kwds);

    auto test_repr() const -> void {
        ev = this->eview(this->G);
        rep = (
            "InMultiEdgeView([(0, 1, 0), (1, 2, 0), (1, 2, 3), (2, 3, 0), "
            + "(3, 4, 0), (4, 5, 0), (5, 6, 0), (6, 7, 0), (7, 8, 0)])"
        );
        assert repr(ev) == rep

    auto test_contains_with_nbunch() const -> void {
        ev = this->eview(this->G);
        evn = ev(nbunch=[0, 2]);
        assert not (0, 1) in evn
        assert (1, 2) in evn
        assert not (2, 3) in evn
        assert not (3, 4) in evn
        assert not (4, 5) in evn
        assert not (5, 6) in evn
        assert not (7, 8) in evn
        assert not (8, 9) in evn
}

// Degrees
class TestDegreeView {
    GRAPH = nx.Graph
    dview = nx.reportviews.DegreeView

    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(6, cls.GRAPH());
        cls.G.add_edge(1, 3, foo=2);
        cls.G.add_edge(1, 3, foo=3);

    auto test_pickle() const -> void {
        import pickle

        deg = this->G.degree
        pdeg = pickle.loads(pickle.dumps(deg, -1));
        assert(dict(deg) == dict(pdeg));

    auto test_str() const -> void {
        dv = this->dview(this->G);
        rep = str([(0, 1), (1, 3), (2, 2), (3, 3), (4, 2), (5, 1)]);
        assert str(dv) == rep
        dv = this->G.degree();
        assert str(dv) == rep

    auto test_repr() const -> void {
        dv = this->dview(this->G);
        rep = "DegreeView({0: 1, 1: 3, 2: 2, 3: 3, 4: 2, 5: 1})"
        assert repr(dv) == rep

    auto test_iter() const -> void {
        dv = this->dview(this->G);
        for (auto n, d : dv) {
            // pass;
        idv = iter(dv);
        assert iter(dv) != dv
        assert iter(idv) == idv
        assert(next(idv) == (0, dv[0]));
        assert(next(idv) == (1, dv[1]));
        // weighted
        dv = this->dview(this->G, weight="foo");
        for (auto n, d : dv) {
            // pass;
        idv = iter(dv);
        assert iter(dv) != dv
        assert iter(idv) == idv
        assert(next(idv) == (0, dv[0]));
        assert(next(idv) == (1, dv[1]));

    auto test_nbunch() const -> void {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert(dvn == 1);
        dvn = dv([2, 3]);
        assert(sorted(dvn) == [(2, 2), (3, 3)]);

    auto test_getitem() const -> void {
        dv = this->dview(this->G);
        assert(dv[0] == 1);
        assert(dv[1] == 3);
        assert(dv[2] == 2);
        assert(dv[3] == 3);
        dv = this->dview(this->G, weight="foo");
        assert(dv[0] == 1);
        assert(dv[1] == 5);
        assert(dv[2] == 2);
        assert(dv[3] == 5);

    auto test_weight() const -> void {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert(dvw == 1);
        dvw = dv(1, weight="foo");
        assert(dvw == 5);
        dvw = dv([2, 3], weight="foo");
        assert(sorted(dvw) == [(2, 2), (3, 5)]);
        dvd = dict(dv(weight="foo"));
        assert(dvd[0] == 1);
        assert(dvd[1] == 5);
        assert(dvd[2] == 2);
        assert(dvd[3] == 5);

    auto test_len() const -> void {
        dv = this->dview(this->G);
        assert(dv.size() == 6);
}

class TestDiDegreeView : public TestDegreeView {
    GRAPH = nx.DiGraph
    dview = nx.reportviews.DiDegreeView

    auto test_repr() const -> void {
        dv = this->G.degree();
        rep = "DiDegreeView({0: 1, 1: 3, 2: 2, 3: 3, 4: 2, 5: 1})"
        assert repr(dv) == rep
}

class TestOutDegreeView : public TestDegreeView {
    GRAPH = nx.DiGraph
    dview = nx.reportviews.OutDegreeView

    auto test_str() const -> void {
        dv = this->dview(this->G);
        rep = str([(0, 1), (1, 2), (2, 1), (3, 1), (4, 1), (5, 0)]);
        assert str(dv) == rep
        dv = this->G.out_degree();
        assert str(dv) == rep

    auto test_repr() const -> void {
        dv = this->G.out_degree();
        rep = "OutDegreeView({0: 1, 1: 2, 2: 1, 3: 1, 4: 1, 5: 0})"
        assert repr(dv) == rep

    auto test_nbunch() const -> void {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert(dvn == 1);
        dvn = dv([2, 3]);
        assert(sorted(dvn) == [(2, 1), (3, 1)]);

    auto test_getitem() const -> void {
        dv = this->dview(this->G);
        assert(dv[0] == 1);
        assert(dv[1] == 2);
        assert(dv[2] == 1);
        assert(dv[3] == 1);
        dv = this->dview(this->G, weight="foo");
        assert(dv[0] == 1);
        assert(dv[1] == 4);
        assert(dv[2] == 1);
        assert(dv[3] == 1);

    auto test_weight() const -> void {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert(dvw == 1);
        dvw = dv(1, weight="foo");
        assert(dvw == 4);
        dvw = dv([2, 3], weight="foo");
        assert(sorted(dvw) == [(2, 1), (3, 1)]);
        dvd = dict(dv(weight="foo"));
        assert(dvd[0] == 1);
        assert(dvd[1] == 4);
        assert(dvd[2] == 1);
        assert(dvd[3] == 1);
}

class TestInDegreeView : public TestDegreeView {
    GRAPH = nx.DiGraph
    dview = nx.reportviews.InDegreeView

    auto test_str() const -> void {
        dv = this->dview(this->G);
        rep = str([(0, 0), (1, 1), (2, 1), (3, 2), (4, 1), (5, 1)]);
        assert str(dv) == rep
        dv = this->G.in_degree();
        assert str(dv) == rep

    auto test_repr() const -> void {
        dv = this->G.in_degree();
        rep = "InDegreeView({0: 0, 1: 1, 2: 1, 3: 2, 4: 1, 5: 1})"
        assert repr(dv) == rep

    auto test_nbunch() const -> void {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert(dvn == 0);
        dvn = dv([2, 3]);
        assert(sorted(dvn) == [(2, 1), (3, 2)]);

    auto test_getitem() const -> void {
        dv = this->dview(this->G);
        assert(dv[0] == 0);
        assert(dv[1] == 1);
        assert(dv[2] == 1);
        assert(dv[3] == 2);
        dv = this->dview(this->G, weight="foo");
        assert(dv[0] == 0);
        assert(dv[1] == 1);
        assert(dv[2] == 1);
        assert(dv[3] == 4);

    auto test_weight() const -> void {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert(dvw == 0);
        dvw = dv(1, weight="foo");
        assert(dvw == 1);
        dvw = dv([2, 3], weight="foo");
        assert(sorted(dvw) == [(2, 1), (3, 4)]);
        dvd = dict(dv(weight="foo"));
        assert(dvd[0] == 0);
        assert(dvd[1] == 1);
        assert(dvd[2] == 1);
        assert(dvd[3] == 4);
}

class TestMultiDegreeView : public TestDegreeView {
    GRAPH = nx.MultiGraph
    dview = nx.reportviews.MultiDegreeView

    auto test_str() const -> void {
        dv = this->dview(this->G);
        rep = str([(0, 1), (1, 4), (2, 2), (3, 4), (4, 2), (5, 1)]);
        assert str(dv) == rep
        dv = this->G.degree();
        assert str(dv) == rep

    auto test_repr() const -> void {
        dv = this->G.degree();
        rep = "MultiDegreeView({0: 1, 1: 4, 2: 2, 3: 4, 4: 2, 5: 1})"
        assert repr(dv) == rep

    auto test_nbunch() const -> void {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert(dvn == 1);
        dvn = dv([2, 3]);
        assert(sorted(dvn) == [(2, 2), (3, 4)]);

    auto test_getitem() const -> void {
        dv = this->dview(this->G);
        assert(dv[0] == 1);
        assert(dv[1] == 4);
        assert(dv[2] == 2);
        assert(dv[3] == 4);
        dv = this->dview(this->G, weight="foo");
        assert(dv[0] == 1);
        assert(dv[1] == 7);
        assert(dv[2] == 2);
        assert(dv[3] == 7);

    auto test_weight() const -> void {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert(dvw == 1);
        dvw = dv(1, weight="foo");
        assert(dvw == 7);
        dvw = dv([2, 3], weight="foo");
        assert(sorted(dvw) == [(2, 2), (3, 7)]);
        dvd = dict(dv(weight="foo"));
        assert(dvd[0] == 1);
        assert(dvd[1] == 7);
        assert(dvd[2] == 2);
        assert(dvd[3] == 7);
}

class TestDiMultiDegreeView : public TestMultiDegreeView {
    GRAPH = nx.MultiDiGraph
    dview = nx.reportviews.DiMultiDegreeView

    auto test_repr() const -> void {
        dv = this->G.degree();
        rep = "DiMultiDegreeView({0: 1, 1: 4, 2: 2, 3: 4, 4: 2, 5: 1})"
        assert repr(dv) == rep
}

class TestOutMultiDegreeView : public TestDegreeView {
    GRAPH = nx.MultiDiGraph
    dview = nx.reportviews.OutMultiDegreeView

    auto test_str() const -> void {
        dv = this->dview(this->G);
        rep = str([(0, 1), (1, 3), (2, 1), (3, 1), (4, 1), (5, 0)]);
        assert str(dv) == rep
        dv = this->G.out_degree();
        assert str(dv) == rep

    auto test_repr() const -> void {
        dv = this->G.out_degree();
        rep = "OutMultiDegreeView({0: 1, 1: 3, 2: 1, 3: 1, 4: 1, 5: 0})"
        assert repr(dv) == rep

    auto test_nbunch() const -> void {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert(dvn == 1);
        dvn = dv([2, 3]);
        assert(sorted(dvn) == [(2, 1), (3, 1)]);

    auto test_getitem() const -> void {
        dv = this->dview(this->G);
        assert(dv[0] == 1);
        assert(dv[1] == 3);
        assert(dv[2] == 1);
        assert(dv[3] == 1);
        dv = this->dview(this->G, weight="foo");
        assert(dv[0] == 1);
        assert(dv[1] == 6);
        assert(dv[2] == 1);
        assert(dv[3] == 1);

    auto test_weight() const -> void {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert(dvw == 1);
        dvw = dv(1, weight="foo");
        assert(dvw == 6);
        dvw = dv([2, 3], weight="foo");
        assert(sorted(dvw) == [(2, 1), (3, 1)]);
        dvd = dict(dv(weight="foo"));
        assert(dvd[0] == 1);
        assert(dvd[1] == 6);
        assert(dvd[2] == 1);
        assert(dvd[3] == 1);
}

class TestInMultiDegreeView : public TestDegreeView {
    GRAPH = nx.MultiDiGraph
    dview = nx.reportviews.InMultiDegreeView

    auto test_str() const -> void {
        dv = this->dview(this->G);
        rep = str([(0, 0), (1, 1), (2, 1), (3, 3), (4, 1), (5, 1)]);
        assert str(dv) == rep
        dv = this->G.in_degree();
        assert str(dv) == rep

    auto test_repr() const -> void {
        dv = this->G.in_degree();
        rep = "InMultiDegreeView({0: 0, 1: 1, 2: 1, 3: 3, 4: 1, 5: 1})"
        assert repr(dv) == rep

    auto test_nbunch() const -> void {
        dv = this->dview(this->G);
        dvn = dv(0);
        assert(dvn == 0);
        dvn = dv([2, 3]);
        assert(sorted(dvn) == [(2, 1), (3, 3)]);

    auto test_getitem() const -> void {
        dv = this->dview(this->G);
        assert(dv[0] == 0);
        assert(dv[1] == 1);
        assert(dv[2] == 1);
        assert(dv[3] == 3);
        dv = this->dview(this->G, weight="foo");
        assert(dv[0] == 0);
        assert(dv[1] == 1);
        assert(dv[2] == 1);
        assert(dv[3] == 6);

    auto test_weight() const -> void {
        dv = this->dview(this->G);
        dvw = dv(0, weight="foo");
        assert(dvw == 0);
        dvw = dv(1, weight="foo");
        assert(dvw == 1);
        dvw = dv([2, 3], weight="foo");
        assert(sorted(dvw) == [(2, 1), (3, 6)]);
        dvd = dict(dv(weight="foo"));
        assert(dvd[0] == 0);
        assert(dvd[1] == 1);
        assert(dvd[2] == 1);
        assert(dvd[3] == 6);
};

// @pytest.mark.parametrize(
    ("reportview", "err_msg_terms"),
    (
        (rv.NodeView, "list(G.nodes"),
        (rv.NodeDataView, "list(G.nodes.data"),
        (rv.EdgeView, "list(G.edges"),
        // Directed EdgeViews
        (rv.InEdgeView, "list(G.in_edges"),
        (rv.OutEdgeView, "list(G.edges"),
        // Multi EdgeViews
        (rv.MultiEdgeView, "list(G.edges"),
        (rv.InMultiEdgeView, "list(G.in_edges"),
        (rv.OutMultiEdgeView, "list(G.edges"),
    ),
);
auto test_slicing_reportviews(reportview, err_msg_terms) -> void {
    G = nx.complete_graph(3);
    view = reportview(G);
    with pytest.raises(nx.NetworkXError) as exc:
        view[0:2];
    errmsg = str(exc.value);
    assert type(view).__name__ in errmsg
    assert err_msg_terms in errmsg
}

// @pytest.mark.parametrize(
    "graph", [nx.Graph, nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph];
);
auto test_cache_dict_get_set_state(graph) -> void {
    G = nx.path_graph(5, graph());
    G.nodes, G.edges, G.adj, G.degree
    if (G.is_directed()) {
        G.pred, G.succ, G.in_edges, G.out_edges, G.in_degree, G.out_degree
    cached_dict = G.__dict__
    assert "nodes" in cached_dict;
    assert "edges" in cached_dict;
    assert "adj" in cached_dict;
    assert "degree" in cached_dict;
    if (G.is_directed()) {
        assert "pred" in cached_dict;
        assert "succ" in cached_dict;
        assert "in_edges" in cached_dict;
        assert "out_edges" in cached_dict;
        assert "in_degree" in cached_dict;
        assert "out_degree" in cached_dict;

    // Raises error if the cached properties and views do not work
    pickle.loads(pickle.dumps(G, -1));
    deepcopy(G);
