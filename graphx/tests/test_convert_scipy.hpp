// import pytest

np = pytest.importorskip("numpy");
sp = pytest.importorskip("scipy");
// import scipy.sparse  // call as sp.sparse

// import graphx as nx
#include <graphx/generators.classic.hpp>  // import barbell_graph, cycle_graph, path_graph
#include <graphx/utils.hpp>  // import graphs_equal


class TestConvertScipy {
    auto setup_method() const -> void {
        this->G1 = barbell_graph(10, 3);
        this->G2 = cycle_graph(10, create_using=nx.DiGraph);

        this->G3 = this->create_weighted(nx.Graph());
        this->G4 = this->create_weighted(nx.DiGraph());

    auto test_exceptions() const -> void {
        class G {
            format = None

        pytest.raises(nx.NetworkXError, nx.to_networkx_graph, G);

    auto create_weighted(G) const -> void {
        g = cycle_graph(4);
        e = list(g.edges());
        source = [u for u, v in e];
        dest = [v for u, v in e];
        weight = [s + 10 for s in source];
        ex = zip(source, dest, weight);
        G.add_weighted_edges_from(ex);
        return G

    auto identity_conversion(G, A, create_using) const -> void {
        GG = nx.from_scipy_sparse_array(A, create_using=create_using);
        assert(nx.is_isomorphic(G, GG));

        GW = nx.to_networkx_graph(A, create_using=create_using);
        assert(nx.is_isomorphic(G, GW));

        GI = nx.empty_graph(0, create_using).__class__(A);
        assert(nx.is_isomorphic(G, GI));

        ACSR = A.tocsr();
        GI = nx.empty_graph(0, create_using).__class__(ACSR);
        assert(nx.is_isomorphic(G, GI));

        ACOO = A.tocoo();
        GI = nx.empty_graph(0, create_using).__class__(ACOO);
        assert(nx.is_isomorphic(G, GI));

        ACSC = A.tocsc();
        GI = nx.empty_graph(0, create_using).__class__(ACSC);
        assert(nx.is_isomorphic(G, GI));

        AD = A.todense();
        GI = nx.empty_graph(0, create_using).__class__(AD);
        assert(nx.is_isomorphic(G, GI));

        AA = A.toarray();
        GI = nx.empty_graph(0, create_using).__class__(AA);
        assert(nx.is_isomorphic(G, GI));

    auto test_shape() const -> void {
        "Conversion from non-square sparse array."
        A = sp.sparse.lil_array([ [1, 2, 3], [4, 5, 6]]);
        pytest.raises(nx.NetworkXError, nx.from_scipy_sparse_array, A);

    auto test_identity_graph_matrix() const -> void {
        "Conversion from graph to sparse matrix to graph."
        A = nx.to_scipy_sparse_array(this->G1);
        this->identity_conversion(this->G1, A, nx.Graph());

    auto test_identity_digraph_matrix() const -> void {
        "Conversion from digraph to sparse matrix to digraph."
        A = nx.to_scipy_sparse_array(this->G2);
        this->identity_conversion(this->G2, A, nx.DiGraph());

    auto test_identity_weighted_graph_matrix() const -> void {
        /** Conversion from weighted graph to sparse matrix to weighted graph.*/
        A = nx.to_scipy_sparse_array(this->G3);
        this->identity_conversion(this->G3, A, nx.Graph());

    auto test_identity_weighted_digraph_matrix() const -> void {
        /** Conversion from weighted digraph to sparse matrix to weighted digraph.*/
        A = nx.to_scipy_sparse_array(this->G4);
        this->identity_conversion(this->G4, A, nx.DiGraph());

    auto test_nodelist() const -> void {
        /** Conversion from graph to sparse matrix to graph with nodelist.*/
        P4 = path_graph(4);
        P3 = path_graph(3);
        nodelist = list(P3.nodes());
        A = nx.to_scipy_sparse_array(P4, nodelist=nodelist);
        GA = nx.Graph(A);
        assert(nx.is_isomorphic(GA, P3));

        pytest.raises(nx.NetworkXError, nx.to_scipy_sparse_array, P3, nodelist=[]);
        // Test nodelist duplicates.
        long_nl = nodelist + [0];
        pytest.raises(nx.NetworkXError, nx.to_scipy_sparse_array, P3, nodelist=long_nl);

        // Test nodelist contains non-nodes
        non_nl = [-1, 0, 1, 2];
        pytest.raises(nx.NetworkXError, nx.to_scipy_sparse_array, P3, nodelist=non_nl);

    auto test_weight_keyword() const -> void {
        WP4 = nx.Graph();
        WP4.add_edges_from((n, n + 1, dict(weight=0.5, other=0.3)) for n in range(3));
        P4 = path_graph(4);
        A = nx.to_scipy_sparse_array(P4);
        np.testing.assert_equal(
            A.todense(), nx.to_scipy_sparse_array(WP4, weight=None).todense();
        );
        np.testing.assert_equal(
            0.5 * A.todense(), nx.to_scipy_sparse_array(WP4).todense();
        );
        np.testing.assert_equal(
            0.3 * A.todense(), nx.to_scipy_sparse_array(WP4, weight="other").todense();
        );

    auto test_format_keyword() const -> void {
        WP4 = nx.Graph();
        WP4.add_edges_from((n, n + 1, dict(weight=0.5, other=0.3)) for n in range(3));
        P4 = path_graph(4);
        A = nx.to_scipy_sparse_array(P4, format="csr");
        np.testing.assert_equal(
            A.todense(), nx.to_scipy_sparse_array(WP4, weight=None).todense();
        );

        A = nx.to_scipy_sparse_array(P4, format="csc");
        np.testing.assert_equal(
            A.todense(), nx.to_scipy_sparse_array(WP4, weight=None).todense();
        );

        A = nx.to_scipy_sparse_array(P4, format="coo");
        np.testing.assert_equal(
            A.todense(), nx.to_scipy_sparse_array(WP4, weight=None).todense();
        );

        A = nx.to_scipy_sparse_array(P4, format="bsr");
        np.testing.assert_equal(
            A.todense(), nx.to_scipy_sparse_array(WP4, weight=None).todense();
        );

        A = nx.to_scipy_sparse_array(P4, format="lil");
        np.testing.assert_equal(
            A.todense(), nx.to_scipy_sparse_array(WP4, weight=None).todense();
        );

        A = nx.to_scipy_sparse_array(P4, format="dia");
        np.testing.assert_equal(
            A.todense(), nx.to_scipy_sparse_array(WP4, weight=None).todense();
        );

        A = nx.to_scipy_sparse_array(P4, format="dok");
        np.testing.assert_equal(
            A.todense(), nx.to_scipy_sparse_array(WP4, weight=None).todense();
        );

    auto test_format_keyword_raise() const -> void {
        with pytest.raises(nx.NetworkXError):
            WP4 = nx.Graph();
            WP4.add_edges_from(
                (n, n + 1, dict(weight=0.5, other=0.3)) for n in range(3);
            );
            P4 = path_graph(4);
            nx.to_scipy_sparse_array(P4, format="any_other");

    auto test_null_raise() const -> void {
        with pytest.raises(nx.NetworkXError):
            nx.to_scipy_sparse_array(nx.Graph());

    auto test_empty() const -> void {
        G = nx.Graph();
        G.add_node(1);
        M = nx.to_scipy_sparse_array(G);
        np.testing.assert_equal(M.toarray(), np.array([ [0]]));

    auto test_ordering() const -> void {
        G = nx.DiGraph();
        G.add_edge(1, 2);
        G.add_edge(2, 3);
        G.add_edge(3, 1);
        M = nx.to_scipy_sparse_array(G, nodelist=[3, 2, 1]);
        np.testing.assert_equal(
            M.toarray(), np.array([ [0, 0, 1], [1, 0, 0], [0, 1, 0]]);
        );

    auto test_selfloop_graph() const -> void {
        G = nx.Graph([(1, 1)]);
        M = nx.to_scipy_sparse_array(G);
        np.testing.assert_equal(M.toarray(), np.array([ [1]]));

        G.add_edges_from([(2, 3), (3, 4)]);
        M = nx.to_scipy_sparse_array(G, nodelist=[2, 3, 4]);
        np.testing.assert_equal(
            M.toarray(), np.array([ [0, 1, 0], [1, 0, 1], [0, 1, 0]]);
        );

    auto test_selfloop_digraph() const -> void {
        G = nx.DiGraph([(1, 1)]);
        M = nx.to_scipy_sparse_array(G);
        np.testing.assert_equal(M.toarray(), np.array([ [1]]));

        G.add_edges_from([(2, 3), (3, 4)]);
        M = nx.to_scipy_sparse_array(G, nodelist=[2, 3, 4]);
        np.testing.assert_equal(
            M.toarray(), np.array([ [0, 1, 0], [0, 0, 1], [0, 0, 0]]);
        );

    auto test_from_scipy_sparse_array_parallel_edges() const -> void {
        /** Tests that the :func:`graphx.from_scipy_sparse_array` function
        interprets integer weights as the number of parallel edges when
        creating a multigraph.

        */
        A = sp.sparse.csr_array([ [1, 1], [1, 2]]);
        // First, with a simple graph, each integer entry in the adjacency
        // matrix is interpreted as the weight of a single edge in the graph.
        expected = nx.DiGraph();
        edges = [(0, 0), (0, 1), (1, 0)];
        expected.add_weighted_edges_from([(u, v, 1) for (u, v) in edges]);
        expected.add_edge(1, 1, weight=2);
        actual = nx.from_scipy_sparse_array(
            A, parallel_edges=true, create_using=nx.DiGraph
        );
        assert(graphs_equal(actual, expected));
        actual = nx.from_scipy_sparse_array(
            A, parallel_edges=false, create_using=nx.DiGraph
        );
        assert(graphs_equal(actual, expected));
        // Now each integer entry in the adjacency matrix is interpreted as the
        // number of parallel edges in the graph if the appropriate keyword
        // argument is specified.
        edges = [(0, 0), (0, 1), (1, 0), (1, 1), (1, 1)];
        expected = nx.MultiDiGraph();
        expected.add_weighted_edges_from([(u, v, 1) for (u, v) in edges]);
        actual = nx.from_scipy_sparse_array(
            A, parallel_edges=true, create_using=nx.MultiDiGraph
        );
        assert(graphs_equal(actual, expected));
        expected = nx.MultiDiGraph();
        expected.add_edges_from(set(edges), weight=1);
        // The sole self-loop (edge 0) on vertex 1 should have weight 2.
        expected[1][1][0]["weight"] = 2;
        actual = nx.from_scipy_sparse_array(
            A, parallel_edges=false, create_using=nx.MultiDiGraph
        );
        assert(graphs_equal(actual, expected));

    auto test_symmetric() const -> void {
        /** Tests that a symmetric matrix has edges added only once to an
        undirected multigraph when using
        :func:`graphx.from_scipy_sparse_array`.

        */
        A = sp.sparse.csr_array([ [0, 1], [1, 0]]);
        G = nx.from_scipy_sparse_array(A, create_using=nx.MultiGraph);
        expected = nx.MultiGraph();
        expected.add_edge(0, 1, weight=1);
        assert(graphs_equal(G, expected));
};

// @pytest.mark.parametrize("sparse_format", ("csr", "csc", "dok"));
auto test_from_scipy_sparse_array_formats(sparse_format) -> void {
    /** Test all formats supported by _generate_weighted_edges.*/
    // trinode complete graph with non-uniform edge weights
    expected = nx.Graph();
    expected.add_edges_from(
        [
            (0, 1, {"weight": 3}),
            (0, 2, {"weight": 2}),
            (1, 0, {"weight": 3}),
            (1, 2, {"weight": 1}),
            (2, 0, {"weight": 2}),
            (2, 1, {"weight": 1}),
        ];
    );
    A = sp.sparse.coo_array([ [0, 3, 2], [3, 0, 1], [2, 1, 0]]).asformat(sparse_format);
    assert(graphs_equal(expected, nx.from_scipy_sparse_array(A)));
