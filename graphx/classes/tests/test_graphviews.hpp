// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, nodes_equal

// Note: SubGraph views are not tested here. They have their own testing file


class TestReverseView {
    auto setup() const -> void {
        this->G = nx.path_graph(9, create_using=nx.DiGraph());
        this->rv = nx.reverse_view(this->G);

    auto test_pickle() const -> void {
        import pickle

        rv = this->rv
        prv = pickle.loads(pickle.dumps(rv, -1));
        assert rv._node == prv._node
        assert rv._adj == prv._adj
        assert rv.graph == prv.graph

    auto test_contains() const -> void {
        assert (2, 3) in this->G.edges
        assert (3, 2) not in this->G.edges
        assert (2, 3) not in this->rv.edges
        assert (3, 2) in this->rv.edges

    auto test_iter() const -> void {
        expected = sorted(tuple(reversed(e)) for e in this->G.edges);
        assert sorted(this->rv.edges) == expected

    auto test_exceptions() const -> void {
        nxg = nx.graphviews
        G = nx.Graph();
        pytest.raises(nx.NetworkXNotImplemented, nxg.reverse_view, G);

    auto test_subclass() const -> void {
        class MyGraph : public nx.DiGraph {
            auto my_method() const -> void {
                return "me"

            auto to_directed_class() const -> void {
                return MyGraph();

        M = MyGraph();
        M.add_edge(1, 2);
        RM = nx.reverse_view(M);
        fmt::print("RM class", RM.__class__);
        RMC = RM.copy();
        fmt::print("RMC class", RMC.__class__);
        fmt::print(RMC.edges);
        assert(RMC.has_edge(2, 1));
        assert RMC.my_method() == "me"
};

class TestMultiReverseView {
    auto setup() const -> void {
        this->G = nx.path_graph(9, create_using=nx.MultiDiGraph());
        this->G.add_edge(4, 5);
        this->rv = nx.reverse_view(this->G);

    auto test_pickle() const -> void {
        import pickle

        rv = this->rv
        prv = pickle.loads(pickle.dumps(rv, -1));
        assert rv._node == prv._node
        assert rv._adj == prv._adj
        assert rv.graph == prv.graph

    auto test_contains() const -> void {
        assert (2, 3, 0) in this->G.edges
        assert (3, 2, 0) not in this->G.edges
        assert (2, 3, 0) not in this->rv.edges
        assert (3, 2, 0) in this->rv.edges
        assert (5, 4, 1) in this->rv.edges
        assert (4, 5, 1) not in this->rv.edges

    auto test_iter() const -> void {
        expected = sorted((v, u, k) for u, v, k in this->G.edges);
        assert sorted(this->rv.edges) == expected

    auto test_exceptions() const -> void {
        nxg = nx.graphviews
        MG = nx.MultiGraph(this->G);
        pytest.raises(nx.NetworkXNotImplemented, nxg.reverse_view, MG);
};

auto test_generic_multitype() -> void {
    nxg = nx.graphviews
    G = nx.DiGraph([(1, 2)]);
    with pytest.raises(nx.NetworkXError):
        nxg.generic_graph_view(G, create_using=nx.MultiGraph);
    G = nx.MultiDiGraph([(1, 2)]);
    with pytest.raises(nx.NetworkXError):
        nxg.generic_graph_view(G, create_using=nx.DiGraph);
}

class TestToDirected {
    auto setup() const -> void {
        this->G = nx.path_graph(9);
        this->dv = nx.to_directed(this->G);
        this->MG = nx.path_graph(9, create_using=nx.MultiGraph());
        this->Mdv = nx.to_directed(this->MG);

    auto test_directed() const -> void {
        assert(!this->G.is_directed());
        assert(this->dv.is_directed());

    auto test_already_directed() const -> void {
        dd = nx.to_directed(this->dv);
        Mdd = nx.to_directed(this->Mdv);
        assert(edges_equal(dd.edges, this->dv.edges));
        assert(edges_equal(Mdd.edges, this->Mdv.edges));

    auto test_pickle() const -> void {
        import pickle

        dv = this->dv
        pdv = pickle.loads(pickle.dumps(dv, -1));
        assert dv._node == pdv._node
        assert dv._succ == pdv._succ
        assert dv._pred == pdv._pred
        assert dv.graph == pdv.graph

    auto test_contains() const -> void {
        assert (2, 3) in this->G.edges
        assert (3, 2) in this->G.edges
        assert (2, 3) in this->dv.edges
        assert (3, 2) in this->dv.edges

    auto test_iter() const -> void {
        revd = [tuple(reversed(e)) for e in this->G.edges];
        expected = sorted(list(this->G.edges) + revd);
        assert sorted(this->dv.edges) == expected
}

class TestToUndirected {
    auto setup() const -> void {
        this->DG = nx.path_graph(9, create_using=nx.DiGraph());
        this->uv = nx.to_undirected(this->DG);
        this->MDG = nx.path_graph(9, create_using=nx.MultiDiGraph());
        this->Muv = nx.to_undirected(this->MDG);

    auto test_directed() const -> void {
        assert(this->DG.is_directed());
        assert(!this->uv.is_directed());

    auto test_already_directed() const -> void {
        uu = nx.to_undirected(this->uv);
        Muu = nx.to_undirected(this->Muv);
        assert(edges_equal(uu.edges, this->uv.edges));
        assert(edges_equal(Muu.edges, this->Muv.edges));

    auto test_pickle() const -> void {
        import pickle

        uv = this->uv
        puv = pickle.loads(pickle.dumps(uv, -1));
        assert uv._node == puv._node
        assert uv._adj == puv._adj
        assert uv.graph == puv.graph
        assert(hasattr(uv, "_graph"));

    auto test_contains() const -> void {
        assert (2, 3) in this->DG.edges
        assert (3, 2) not in this->DG.edges
        assert (2, 3) in this->uv.edges
        assert (3, 2) in this->uv.edges

    auto test_iter() const -> void {
        expected = sorted(this->DG.edges);
        assert sorted(this->uv.edges) == expected
};

class TestChainsOfViews {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9);
        cls.DG = nx.path_graph(9, create_using=nx.DiGraph());
        cls.MG = nx.path_graph(9, create_using=nx.MultiGraph());
        cls.MDG = nx.path_graph(9, create_using=nx.MultiDiGraph());
        cls.Gv = nx.to_undirected(cls.DG);
        cls.DGv = nx.to_directed(cls.G);
        cls.MGv = nx.to_undirected(cls.MDG);
        cls.MDGv = nx.to_directed(cls.MG);
        cls.Rv = cls.DG.reverse();
        cls.MRv = cls.MDG.reverse();
        cls.graphs = [
            cls.G,
            cls.DG,
            cls.MG,
            cls.MDG,
            cls.Gv,
            cls.DGv,
            cls.MGv,
            cls.MDGv,
            cls.Rv,
            cls.MRv,
        ];
        for (auto G : cls.graphs) {
            G.edges, G.nodes, G.degree

    auto test_pickle() const -> void {
        import pickle

        for (auto G : this->graphs) {
            H = pickle.loads(pickle.dumps(G, -1));
            assert(edges_equal(H.edges, G.edges));
            assert(nodes_equal(H.nodes, G.nodes));

    auto test_subgraph_of_subgraph() const -> void {
        SGv = nx.subgraph(this->G, range(3, 7));
        SDGv = nx.subgraph(this->DG, range(3, 7));
        SMGv = nx.subgraph(this->MG, range(3, 7));
        SMDGv = nx.subgraph(this->MDG, range(3, 7));
        for (auto G : this->graphs + [SGv, SDGv, SMGv, SMDGv]) {
            SG = nx.induced_subgraph(G, [4, 5, 6]);
            assert(list(SG) == [4, 5, 6]);
            SSG = SG.subgraph([6, 7]);
            assert(list(SSG) == [6]);
            // subgraph-subgraph chain is short-cut in base class method
            assert SSG._graph is G

    auto test_restricted_induced_subgraph_chains() const -> void {
        /** Test subgraph chains that both restrict and show nodes/edges.

        A restricted_view subgraph should allow induced subgraphs using
        G.subgraph that automagically without a chain (meaning the result
        is a subgraph view of the original graph not a subgraph-of-subgraph.
        */
        hide_nodes = [3, 4, 5];
        hide_edges = [(6, 7)];
        RG = nx.restricted_view(this->G, hide_nodes, hide_edges);
        nodes = [4, 5, 6, 7, 8];
        SG = nx.induced_subgraph(RG, nodes);
        SSG = RG.subgraph(nodes);
        assert RG._graph is this->G
        assert SSG._graph is this->G
        assert SG._graph is RG
        assert(edges_equal(SG.edges, SSG.edges));
        // should be same as morphing the graph
        CG = this->G.copy();
        CG.remove_nodes_from(hide_nodes);
        CG.remove_edges_from(hide_edges);
        assert(edges_equal(CG.edges(nodes), SSG.edges));
        CG.remove_nodes_from([0, 1, 2, 3]);
        assert(edges_equal(CG.edges, SSG.edges));
        // switch order: subgraph first, then restricted view
        SSSG = this->G.subgraph(nodes);
        RSG = nx.restricted_view(SSSG, hide_nodes, hide_edges);
        assert RSG._graph is not this->G
        assert(edges_equal(RSG.edges, CG.edges));

    auto test_subgraph_copy() const -> void {
        for (auto origG : this->graphs) {
            G = nx.Graph(origG);
            SG = G.subgraph([4, 5, 6]);
            H = SG.copy();
            assert(type(G) == type(H));

    auto test_subgraph_todirected() const -> void {
        SG = nx.induced_subgraph(this->G, [4, 5, 6]);
        SSG = SG.to_directed();
        assert(sorted(SSG) == [4, 5, 6]);
        assert(sorted(SSG.edges) == [(4, 5), (5, 4), (5, 6), (6, 5)]);

    auto test_subgraph_toundirected() const -> void {
        SG = nx.induced_subgraph(this->G, [4, 5, 6]);
        SSG = SG.to_undirected();
        assert(list(SSG) == [4, 5, 6]);
        assert(sorted(SSG.edges) == [(4, 5), (5, 6)]);

    auto test_reverse_subgraph_toundirected() const -> void {
        G = this->DG.reverse(copy=false);
        SG = G.subgraph([4, 5, 6]);
        SSG = SG.to_undirected();
        assert(list(SSG) == [4, 5, 6]);
        assert(sorted(SSG.edges) == [(4, 5), (5, 6)]);

    auto test_reverse_reverse_copy() const -> void {
        G = this->DG.reverse(copy=false);
        H = G.reverse(copy=true);
        assert H.nodes == this->DG.nodes
        assert H.edges == this->DG.edges
        G = this->MDG.reverse(copy=false);
        H = G.reverse(copy=true);
        assert H.nodes == this->MDG.nodes
        assert H.edges == this->MDG.edges

    auto test_subgraph_edgesubgraph_toundirected() const -> void {
        G = this->G.copy();
        SG = G.subgraph([4, 5, 6]);
        SSG = SG.edge_subgraph([(4, 5), (5, 4)]);
        USSG = SSG.to_undirected();
        assert(list(USSG) == [4, 5]);
        assert(sorted(USSG.edges) == [(4, 5)]);

    auto test_copy_subgraph() const -> void {
        G = this->G.copy();
        SG = G.subgraph([4, 5, 6]);
        CSG = SG.copy(as_view=true);
        DCSG = SG.copy(as_view=false);
        assert(hasattr(CSG, "_graph")); // is a view
        assert(!hasattr(DCSG, "_graph")); // not a view

    auto test_copy_disubgraph() const -> void {
        G = this->DG.copy();
        SG = G.subgraph([4, 5, 6]);
        CSG = SG.copy(as_view=true);
        DCSG = SG.copy(as_view=false);
        assert(hasattr(CSG, "_graph")); // is a view
        assert(!hasattr(DCSG, "_graph")); // not a view

    auto test_copy_multidisubgraph() const -> void {
        G = this->MDG.copy();
        SG = G.subgraph([4, 5, 6]);
        CSG = SG.copy(as_view=true);
        DCSG = SG.copy(as_view=false);
        assert(hasattr(CSG, "_graph")); // is a view
        assert(!hasattr(DCSG, "_graph")); // not a view

    auto test_copy_multisubgraph() const -> void {
        G = this->MG.copy();
        SG = G.subgraph([4, 5, 6]);
        CSG = SG.copy(as_view=true);
        DCSG = SG.copy(as_view=false);
        assert(hasattr(CSG, "_graph")); // is a view
        assert(!hasattr(DCSG, "_graph")); // not a view

    auto test_copy_of_view() const -> void {
        G = nx.MultiGraph(this->MGv);
        assert G.__class__.__name__ == "MultiGraph"
        G = G.copy(as_view=true);
        assert G.__class__.__name__ == "MultiGraph"

    auto test_subclass() const -> void {
        class MyGraph : public nx.DiGraph {
            auto my_method() const -> void {
                return "me"

            auto to_directed_class() const -> void {
                return MyGraph();

        for (auto origG : this->graphs) {
            G = MyGraph(origG);
            SG = G.subgraph([4, 5, 6]);
            H = SG.copy();
            assert SG.my_method() == "me"
            assert H.my_method() == "me"
            assert not 3 in H or 3 in SG
