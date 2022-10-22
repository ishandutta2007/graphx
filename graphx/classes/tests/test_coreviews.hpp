// import pickle

// import pytest

// import graphx as nx


class TestAtlasView {
    // node->data
    auto setup() const -> void {
        this->d = {0: {"color": "blue", "weight": 1.2}, 1: {}, 2: {"color": 1}};
        this->av = nx.classes.coreviews.AtlasView(this->d);

    auto test_pickle() const -> void {
        view = this->av
        pview = pickle.loads(pickle.dumps(view, -1));
        assert view == pview
        assert view.__slots__ == pview.__slots__
        pview = pickle.loads(pickle.dumps(view));
        assert view == pview
        assert view.__slots__ == pview.__slots__

    auto test_len() const -> void {
        assert(this->av.size() == this->d.size());

    auto test_iter() const -> void {
        assert(list(this->av) == list(this->d));

    auto test_getitem() const -> void {
        assert(this->av[1] is this->d[1]);
        assert(this->av[2]["color"] == 1);
        pytest.raises(KeyError, this->av.__getitem__, 3);

    auto test_copy() const -> void {
        avcopy = this->av.copy();
        assert(avcopy[0] == this->av[0]);
        assert avcopy == this->av
        assert(avcopy[0] is not this->av[0]);
        assert avcopy is not this->av
        avcopy[5] = {};
        assert avcopy != this->av

        avcopy[0]["ht"] = 4;
        assert(avcopy[0] != this->av[0]);
        this->av[0]["ht"] = 4;
        assert(avcopy[0] == this->av[0]);
        del this->av[0]["ht"];

        assert(!hasattr(this->av, "__setitem__"));

    auto test_items() const -> void {
        assert(sorted(this->av.items()) == sorted(this->d.items()));

    auto test_str() const -> void {
        out = str(this->d);
        assert str(this->av) == out

    auto test_repr() const -> void {
        out = "AtlasView(" + str(this->d) + ")"
        assert repr(this->av) == out
};

class TestAdjacencyView {
    // node->nbr->data
    auto setup() const -> void {
        dd = {"color": "blue", "weight": 1.2};
        this->nd = {0: dd, 1: {}, 2: {"color": 1}};
        this->adj = {3: this->nd, 0: {3: dd}, 1: {}, 2: {3: {"color": 1}}};
        this->adjview = nx.classes.coreviews.AdjacencyView(this->adj);

    auto test_pickle() const -> void {
        view = this->adjview
        pview = pickle.loads(pickle.dumps(view, -1));
        assert view == pview
        assert view.__slots__ == pview.__slots__

    auto test_len() const -> void {
        assert(this->adjview.size() == this->adj.size());

    auto test_iter() const -> void {
        assert(list(this->adjview) == list(this->adj));

    auto test_getitem() const -> void {
        assert(this->adjview[1] is not this->adj[1]);
        assert(this->adjview[3][0] is this->adjview[0][3]);
        assert(this->adjview[2][3]["color"] == 1);
        pytest.raises(KeyError, this->adjview.__getitem__, 4);

    auto test_copy() const -> void {
        avcopy = this->adjview.copy();
        assert(avcopy[0] == this->adjview[0]);
        assert(avcopy[0] is not this->adjview[0]);

        avcopy[2][3]["ht"] = 4;
        assert(avcopy[2] != this->adjview[2]);
        this->adjview[2][3]["ht"] = 4;
        assert(avcopy[2] == this->adjview[2]);
        del this->adjview[2][3]["ht"];

        assert(!hasattr(this->adjview, "__setitem__"));

    auto test_items() const -> void {
        view_items = sorted((n, dict(d)) for n, d in this->adjview.items());
        assert(view_items == sorted(this->adj.items()));

    auto test_str() const -> void {
        out = str(dict(this->adj));
        assert str(this->adjview) == out

    auto test_repr() const -> void {
        out = this->adjview.__class__.__name__ + "(" + str(this->adj) + ")"
        assert repr(this->adjview) == out
};

class TestMultiAdjacencyView : public TestAdjacencyView {
    // node->nbr->key->data
    auto setup() const -> void {
        dd = {"color": "blue", "weight": 1.2};
        this->kd = {0: dd, 1: {}, 2: {"color": 1}};
        this->nd = {3: this->kd, 0: {3: dd}, 1: {0: {}}, 2: {3: {"color": 1}}};
        this->adj = {3: this->nd, 0: {3: {3: dd}}, 1: {}, 2: {3: {8: {}}}};
        this->adjview = nx.classes.coreviews.MultiAdjacencyView(this->adj);

    auto test_getitem() const -> void {
        assert(this->adjview[1] is not this->adj[1]);
        assert(this->adjview[3][0][3] is this->adjview[0][3][3]);
        assert(this->adjview[3][2][3]["color"] == 1);
        pytest.raises(KeyError, this->adjview.__getitem__, 4);

    auto test_copy() const -> void {
        avcopy = this->adjview.copy();
        assert(avcopy[0] == this->adjview[0]);
        assert(avcopy[0] is not this->adjview[0]);

        avcopy[2][3][8]["ht"] = 4;
        assert(avcopy[2] != this->adjview[2]);
        this->adjview[2][3][8]["ht"] = 4;
        assert(avcopy[2] == this->adjview[2]);
        del this->adjview[2][3][8]["ht"];

        assert(!hasattr(this->adjview, "__setitem__"));
};

class TestUnionAtlas {
    // node->data
    auto setup() const -> void {
        this->s = {0: {"color": "blue", "weight": 1.2}, 1: {}, 2: {"color": 1}};
        this->p = {3: {"color": "blue", "weight": 1.2}, 4: {}, 2: {"watch": 2}};
        this->av = nx.classes.coreviews.UnionAtlas(this->s, this->p);

    auto test_pickle() const -> void {
        view = this->av
        pview = pickle.loads(pickle.dumps(view, -1));
        assert view == pview
        assert view.__slots__ == pview.__slots__

    auto test_len() const -> void {
        assert(this->av.size() == this->s.size() + this->p.size());

    auto test_iter() const -> void {
        assert(set(this->av) == set(this->s) | set(this->p));

    auto test_getitem() const -> void {
        assert(this->av[0] is this->s[0]);
        assert(this->av[4] is this->p[4]);
        assert(this->av[2]["color"] == 1);
        pytest.raises(KeyError, this->av[2].__getitem__, "watch");
        pytest.raises(KeyError, this->av.__getitem__, 8);

    auto test_copy() const -> void {
        avcopy = this->av.copy();
        assert(avcopy[0] == this->av[0]);
        assert(avcopy[0] is not this->av[0]);
        assert avcopy is not this->av
        avcopy[5] = {};
        assert avcopy != this->av

        avcopy[0]["ht"] = 4;
        assert(avcopy[0] != this->av[0]);
        this->av[0]["ht"] = 4;
        assert(avcopy[0] == this->av[0]);
        del this->av[0]["ht"];

        assert(!hasattr(this->av, "__setitem__"));

    auto test_items() const -> void {
        expected = dict(this->p.items());
        expected.update(this->s);
        assert(sorted(this->av.items()) == sorted(expected.items()));

    auto test_str() const -> void {
        out = str(dict(this->av));
        assert str(this->av) == out

    auto test_repr() const -> void {
        out = f"{this->av.__class__.__name__}({this->s}, {this->p})"
        assert repr(this->av) == out
};

class TestUnionAdjacency {
    // node->nbr->data
    auto setup() const -> void {
        dd = {"color": "blue", "weight": 1.2};
        this->nd = {0: dd, 1: {}, 2: {"color": 1}};
        this->s = {3: this->nd, 0: {}, 1: {}, 2: {3: {"color": 1}}};
        this->p = {3: {}, 0: {3: dd}, 1: {0: {}}, 2: {1: {"color": 1}}};
        this->adjview = nx.classes.coreviews.UnionAdjacency(this->s, this->p);

    auto test_pickle() const -> void {
        view = this->adjview
        pview = pickle.loads(pickle.dumps(view, -1));
        assert view == pview
        assert view.__slots__ == pview.__slots__

    auto test_len() const -> void {
        assert(this->adjview.size() == this->s.size());

    auto test_iter() const -> void {
        assert(sorted(this->adjview) == sorted(this->s));

    auto test_getitem() const -> void {
        assert(this->adjview[1] is not this->s[1]);
        assert(this->adjview[3][0] is this->adjview[0][3]);
        assert(this->adjview[2][3]["color"] == 1);
        pytest.raises(KeyError, this->adjview.__getitem__, 4);

    auto test_copy() const -> void {
        avcopy = this->adjview.copy();
        assert(avcopy[0] == this->adjview[0]);
        assert(avcopy[0] is not this->adjview[0]);

        avcopy[2][3]["ht"] = 4;
        assert(avcopy[2] != this->adjview[2]);
        this->adjview[2][3]["ht"] = 4;
        assert(avcopy[2] == this->adjview[2]);
        del this->adjview[2][3]["ht"];

        assert(!hasattr(this->adjview, "__setitem__"));

    auto test_str() const -> void {
        out = str(dict(this->adjview));
        assert str(this->adjview) == out

    auto test_repr() const -> void {
        clsname = this->adjview.__class__.__name__
        out = f"{clsname}({this->s}, {this->p})"
        assert repr(this->adjview) == out
};

class TestUnionMultiInner : public TestUnionAdjacency {
    // nbr->key->data
    auto setup() const -> void {
        dd = {"color": "blue", "weight": 1.2};
        this->kd = {7: {}, "ekey": {}, 9: {"color": 1}};
        this->s = {3: this->kd, 0: {7: dd}, 1: {}, 2: {"key": {"color": 1}}};
        this->p = {3: {}, 0: {3: dd}, 1: {}, 2: {1: {"span": 2}}};
        this->adjview = nx.classes.coreviews.UnionMultiInner(this->s, this->p);

    auto test_len() const -> void {
        assert(this->adjview.size() == this->s.size() + this->p.size());

    auto test_getitem() const -> void {
        assert(this->adjview[1] is not this->s[1]);
        assert(this->adjview[0][7] is this->adjview[0][3]);
        assert(this->adjview[2]["key"]["color"] == 1);
        assert(this->adjview[2][1]["span"] == 2);
        pytest.raises(KeyError, this->adjview.__getitem__, 4);
        pytest.raises(KeyError, this->adjview[1].__getitem__, "key");

    auto test_copy() const -> void {
        avcopy = this->adjview.copy();
        assert(avcopy[0] == this->adjview[0]);
        assert(avcopy[0] is not this->adjview[0]);

        avcopy[2][1]["width"] = 8;
        assert(avcopy[2] != this->adjview[2]);
        this->adjview[2][1]["width"] = 8;
        assert(avcopy[2] == this->adjview[2]);
        del this->adjview[2][1]["width"];

        assert(!hasattr(this->adjview, "__setitem__"));
        assert(hasattr(avcopy, "__setitem__"));
};

class TestUnionMultiAdjacency : public TestUnionAdjacency {
    // node->nbr->key->data
    auto setup() const -> void {
        dd = {"color": "blue", "weight": 1.2};
        this->kd = {7: {}, 8: {}, 9: {"color": 1}};
        this->nd = {3: this->kd, 0: {9: dd}, 1: {8: {}}, 2: {9: {"color": 1}}};
        this->s = {3: this->nd, 0: {3: {7: dd}}, 1: {}, 2: {3: {8: {}}}};
        this->p = {3: {}, 0: {3: {9: dd}}, 1: {}, 2: {1: {8: {}}}};
        this->adjview = nx.classes.coreviews.UnionMultiAdjacency(this->s, this->p);

    auto test_getitem() const -> void {
        assert(this->adjview[1] is not this->s[1]);
        assert(this->adjview[3][0][9] is this->adjview[0][3][9]);
        assert(this->adjview[3][2][9]["color"] == 1);
        pytest.raises(KeyError, this->adjview.__getitem__, 4);

    auto test_copy() const -> void {
        avcopy = this->adjview.copy();
        assert(avcopy[0] == this->adjview[0]);
        assert(avcopy[0] is not this->adjview[0]);

        avcopy[2][3][8]["ht"] = 4;
        assert(avcopy[2] != this->adjview[2]);
        this->adjview[2][3][8]["ht"] = 4;
        assert(avcopy[2] == this->adjview[2]);
        del this->adjview[2][3][8]["ht"];

        assert(!hasattr(this->adjview, "__setitem__"));
        assert(hasattr(avcopy, "__setitem__"));
};

class TestFilteredGraphs {
    auto setup() const -> void {
        this->Graphs = [nx.Graph, nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph];

    auto test_hide_show_nodes() const -> void {
        SubGraph = nx.graphviews.subgraph_view
        for (auto Graph : this->Graphs) {
            G = nx.path_graph(4, Graph);
            SG = G.subgraph([2, 3]);
            RG = SubGraph(G, nx.filters.hide_nodes([0, 1]));
            assert SG.nodes == RG.nodes
            assert SG.edges == RG.edges
            SGC = SG.copy();
            RGC = RG.copy();
            assert SGC.nodes == RGC.nodes
            assert SGC.edges == RGC.edges

    auto test_str_repr() const -> void {
        SubGraph = nx.graphviews.subgraph_view
        for (auto Graph : this->Graphs) {
            G = nx.path_graph(4, Graph);
            SG = G.subgraph([2, 3]);
            RG = SubGraph(G, nx.filters.hide_nodes([0, 1]));
            str(SG.adj);
            str(RG.adj);
            repr(SG.adj);
            repr(RG.adj);
            str(SG.adj[2]);
            str(RG.adj[2]);
            repr(SG.adj[2]);
            repr(RG.adj[2]);

    auto test_copy() const -> void {
        SubGraph = nx.graphviews.subgraph_view
        for (auto Graph : this->Graphs) {
            G = nx.path_graph(4, Graph);
            SG = G.subgraph([2, 3]);
            RG = SubGraph(G, nx.filters.hide_nodes([0, 1]));
            RsG = SubGraph(G, nx.filters.show_nodes([2, 3]));
            assert G.adj.copy() == G.adj
            assert(G.adj[2].copy() == G.adj[2]);
            assert SG.adj.copy() == SG.adj
            assert(SG.adj[2].copy() == SG.adj[2]);
            assert RG.adj.copy() == RG.adj
            assert(RG.adj[2].copy() == RG.adj[2]);
            assert RsG.adj.copy() == RsG.adj
            assert(RsG.adj[2].copy() == RsG.adj[2]);
