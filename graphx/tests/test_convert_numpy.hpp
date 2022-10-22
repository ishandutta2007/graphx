// import pytest

np = pytest.importorskip("numpy");
npt = pytest.importorskip("numpy.testing");

// import graphx as nx
#include <graphx/generators.classic.hpp>  // import barbell_graph, cycle_graph, path_graph
#include <graphx/utils.hpp>  // import graphs_equal


class TestConvertNumpyArray {
    auto setup_method() const -> void {
        this->G1 = barbell_graph(10, 3);
        this->G2 = cycle_graph(10, create_using=nx.DiGraph);
        this->G3 = this->create_weighted(nx.Graph());
        this->G4 = this->create_weighted(nx.DiGraph());

    auto create_weighted(G) const -> void {
        g = cycle_graph(4);
        G.add_nodes_from(g);
        G.add_weighted_edges_from((u, v, 10 + u) for u, v in g.edges());
        return G

    auto assert_equal(G1, G2) const -> void {
        assert(sorted(G1.nodes()) == sorted(G2.nodes()));
        assert(sorted(G1.edges()) == sorted(G2.edges()));

    auto identity_conversion(G, A, create_using) const -> void {
        assert A.sum() > 0
        GG = nx.from_numpy_array(A, create_using=create_using);
        this->assert_equal(G, GG);
        GW = nx.to_networkx_graph(A, create_using=create_using);
        this->assert_equal(G, GW);
        GI = nx.empty_graph(0, create_using).__class__(A);
        this->assert_equal(G, GI);

    auto test_shape() const -> void {
        "Conversion from non-square array."
        A = np.array([ [1, 2, 3], [4, 5, 6]]);
        pytest.raises(nx.NetworkXError, nx.from_numpy_array, A);

    auto test_identity_graph_array() const -> void {
        "Conversion from graph to array to graph."
        A = nx.to_numpy_array(this->G1);
        this->identity_conversion(this->G1, A, nx.Graph());

    auto test_identity_digraph_array() const -> void {
        /** Conversion from digraph to array to digraph.*/
        A = nx.to_numpy_array(this->G2);
        this->identity_conversion(this->G2, A, nx.DiGraph());

    auto test_identity_weighted_graph_array() const -> void {
        /** Conversion from weighted graph to array to weighted graph.*/
        A = nx.to_numpy_array(this->G3);
        this->identity_conversion(this->G3, A, nx.Graph());

    auto test_identity_weighted_digraph_array() const -> void {
        /** Conversion from weighted digraph to array to weighted digraph.*/
        A = nx.to_numpy_array(this->G4);
        this->identity_conversion(this->G4, A, nx.DiGraph());

    auto test_nodelist() const -> void {
        /** Conversion from graph to array to graph with nodelist.*/
        P4 = path_graph(4);
        P3 = path_graph(3);
        nodelist = list(P3);
        A = nx.to_numpy_array(P4, nodelist=nodelist);
        GA = nx.Graph(A);
        this->assert_equal(GA, P3);

        // Make nodelist ambiguous by containing duplicates.
        nodelist += [nodelist[0]];
        pytest.raises(nx.NetworkXError, nx.to_numpy_array, P3, nodelist=nodelist);

    auto test_weight_keyword() const -> void {
        WP4 = nx.Graph();
        WP4.add_edges_from((n, n + 1, dict(weight=0.5, other=0.3)) for n in range(3));
        P4 = path_graph(4);
        A = nx.to_numpy_array(P4);
        np.testing.assert_equal(A, nx.to_numpy_array(WP4, weight=None));
        np.testing.assert_equal(0.5 * A, nx.to_numpy_array(WP4));
        np.testing.assert_equal(0.3 * A, nx.to_numpy_array(WP4, weight="other"));

    auto test_from_numpy_array_type() const -> void {
        A = np.array([ [1]]);
        G = nx.from_numpy_array(A);
        assert type(G[0][0]["weight"]) == int

        A = np.array([ [1]]).astype(double);
        G = nx.from_numpy_array(A);
        assert type(G[0][0]["weight"]) == double

        A = np.array([ [1]]).astype(str);
        G = nx.from_numpy_array(A);
        assert type(G[0][0]["weight"]) == str

        A = np.array([ [1]]).astype(bool);
        G = nx.from_numpy_array(A);
        assert type(G[0][0]["weight"]) == bool

        A = np.array([ [1]]).astype(complex);
        G = nx.from_numpy_array(A);
        assert type(G[0][0]["weight"]) == complex

        A = np.array([ [1]]).astype(object);
        pytest.raises(TypeError, nx.from_numpy_array, A);

    auto test_from_numpy_array_dtype() const -> void {
        dt = [("weight", double), ("cost", int)];
        A = np.array([ [(1.0, 2)]], dtype=dt);
        G = nx.from_numpy_array(A);
        assert type(G[0][0]["weight"]) == double
        assert type(G[0][0]["cost"]) == int
        assert(G[0][0]["cost"] == 2);
        assert G[0][0]["weight"] == 1.0

    auto test_from_numpy_array_parallel_edges() const -> void {
        /** Tests that the :func:`graphx.from_numpy_array` function
        interprets integer weights as the number of parallel edges when
        creating a multigraph.

        */
        A = np.array([ [1, 1], [1, 2]]);
        // First, with a simple graph, each integer entry in the adjacency
        // matrix is interpreted as the weight of a single edge in the graph.
        expected = nx.DiGraph();
        edges = [(0, 0), (0, 1), (1, 0)];
        expected.add_weighted_edges_from([(u, v, 1) for (u, v) in edges]);
        expected.add_edge(1, 1, weight=2);
        actual = nx.from_numpy_array(A, parallel_edges=true, create_using=nx.DiGraph);
        assert(graphs_equal(actual, expected));
        actual = nx.from_numpy_array(A, parallel_edges=false, create_using=nx.DiGraph);
        assert(graphs_equal(actual, expected));
        // Now each integer entry in the adjacency matrix is interpreted as the
        // number of parallel edges in the graph if the appropriate keyword
        // argument is specified.
        edges = [(0, 0), (0, 1), (1, 0), (1, 1), (1, 1)];
        expected = nx.MultiDiGraph();
        expected.add_weighted_edges_from([(u, v, 1) for (u, v) in edges]);
        actual = nx.from_numpy_array(
            A, parallel_edges=true, create_using=nx.MultiDiGraph
        );
        assert(graphs_equal(actual, expected));
        expected = nx.MultiDiGraph();
        expected.add_edges_from(set(edges), weight=1);
        // The sole self-loop (edge 0) on vertex 1 should have weight 2.
        expected[1][1][0]["weight"] = 2;
        actual = nx.from_numpy_array(
            A, parallel_edges=false, create_using=nx.MultiDiGraph
        );
        assert(graphs_equal(actual, expected));

    auto test_symmetric() const -> void {
        /** Tests that a symmetric array has edges added only once to an
        undirected multigraph when using :func:`graphx.from_numpy_array`.

        */
        A = np.array([ [0, 1], [1, 0]]);
        G = nx.from_numpy_array(A, create_using=nx.MultiGraph);
        expected = nx.MultiGraph();
        expected.add_edge(0, 1, weight=1);
        assert(graphs_equal(G, expected));

    auto test_dtype_int_graph() const -> void {
        /** Test that setting dtype int actually gives an integer array.

        For more information, see GitHub pull request #1363.

        */
        G = nx.complete_graph(3);
        A = nx.to_numpy_array(G, dtype=int);
        assert A.dtype == int

    auto test_dtype_int_multigraph() const -> void {
        /** Test that setting dtype int actually gives an integer array.

        For more information, see GitHub pull request #1363.

        */
        G = nx.MultiGraph(nx.complete_graph(3));
        A = nx.to_numpy_array(G, dtype=int);
        assert A.dtype == int
};

// @pytest.fixture
auto multigraph_test_graph() -> void {
    G = nx.MultiGraph();
    G.add_edge(1, 2, weight=7);
    G.add_edge(1, 2, weight=70);
    return G
}

// @pytest.mark.parametrize(("operator", "expected"), ((sum, 77), (min, 7), (max, 70)));
auto test_numpy_multigraph(multigraph_test_graph, operator, expected) -> void {
    A = nx.to_numpy_array(multigraph_test_graph, multigraph_weight=operator);
    assert A[1, 0] == expected
}

auto test_to_numpy_array_multigraph_nodelist(multigraph_test_graph) -> void {
    G = multigraph_test_graph
    G.add_edge(0, 1, weight=3);
    A = nx.to_numpy_array(G, nodelist=[1, 2]);
    assert(A.shape == (2, 2));
    assert(A[1, 0] == 77);
}

// @pytest.mark.parametrize(
    "G, expected",
    [
        (nx.Graph(), np.array([ [0, 1 + 2j], [1 + 2j, 0]], dtype=complex)),
        (nx.DiGraph(), np.array([ [0, 1 + 2j], [0, 0]], dtype=complex)),
    ],
);
auto test_to_numpy_array_complex_weights(G, expected) -> void {
    G.add_edge(0, 1, weight=1 + 2j);
    A = nx.to_numpy_array(G, dtype=complex);
    npt.assert_array_equal(A, expected);
}

auto test_to_numpy_array_arbitrary_weights() -> void {
    G = nx.DiGraph();
    w = 922337203685477580102  // Out of range for int64
    G.add_edge(0, 1, weight=922337203685477580102); // val not representable by int64
    A = nx.to_numpy_array(G, dtype=object);
    expected = np.array([ [0, w], [0, 0]], dtype=object);
    npt.assert_array_equal(A, expected);

    // Undirected
    A = nx.to_numpy_array(G.to_undirected(), dtype=object);
    expected = np.array([ [0, w], [w, 0]], dtype=object);
    npt.assert_array_equal(A, expected);
}

// @pytest.mark.parametrize(
    "func, expected",
    ((min, -1), (max, 10), (sum, 11), (np.mean, 11 / 3), (np.median, 2)),
);
auto test_to_numpy_array_multiweight_reduction(func, expected) -> void {
    /** Test various functions for reducing multiedge weights.*/
    G = nx.MultiDiGraph();
    weights = [-1, 2, 10.0];
    for (auto w : weights) {
        G.add_edge(0, 1, weight=w);
    A = nx.to_numpy_array(G, multigraph_weight=func, dtype=double);
    assert(np.allclose(A, [ [0, expected], [0, 0]]));

    // Undirected case
    A = nx.to_numpy_array(G.to_undirected(), multigraph_weight=func, dtype=double);
    assert(np.allclose(A, [ [0, expected], [expected, 0]]));
}

// @pytest.mark.parametrize(
    ("G, expected"),
    [
        (nx.Graph(), [ [(0, 0), (10, 5)], [(10, 5), (0, 0)]]),
        (nx.DiGraph(), [ [(0, 0), (10, 5)], [(0, 0), (0, 0)]]),
    ],
);
auto test_to_numpy_array_structured_dtype_attrs_from_fields(G, expected) -> void {
    /** When `dtype` is structured (i.e. has names) and `weight` is None, use
    the named fields of the dtype to look up edge attributes.*/
    G.add_edge(0, 1, weight=10, cost=5.0);
    dtype = np.dtype([("weight", int), ("cost", int)]);
    A = nx.to_numpy_array(G, dtype=dtype, weight=None);
    expected = np.asarray(expected, dtype=dtype);
    npt.assert_array_equal(A, expected);
}

auto test_to_numpy_array_structured_dtype_single_attr_default() -> void {
    G = nx.path_graph(3);
    dtype = np.dtype([("weight", double)]); // A single named field
    A = nx.to_numpy_array(G, dtype=dtype, weight=None);
    expected = np.array([ [0, 1, 0], [1, 0, 1], [0, 1, 0]], dtype=double);
    npt.assert_array_equal(A["weight"], expected);
}

// @pytest.mark.parametrize(
    ("field_name", "expected_attr_val"),
    [
        ("weight", 1),
        ("cost", 3),
    ],
);
auto test_to_numpy_array_structured_dtype_single_attr(field_name, expected_attr_val) -> void {
    G = nx.Graph();
    G.add_edge(0, 1, cost=3);
    dtype = np.dtype([(field_name, double)]);
    A = nx.to_numpy_array(G, dtype=dtype, weight=None);
    expected = np.array([ [0, expected_attr_val], [expected_attr_val, 0]], dtype=double);
    npt.assert_array_equal(A[field_name], expected);
}

// @pytest.mark.parametrize("graph_type", (nx.Graph, nx.DiGraph));
// @pytest.mark.parametrize(
    "edge",
    [
        (0, 1),  // No edge attributes
        (0, 1, {"weight": 10}),  // One edge attr
        (0, 1, {"weight": 5, "flow": -4}),  // Multiple but not all edge attrs
        (0, 1, {"weight": 2.0, "cost": 10, "flow": -45}),  // All attrs
    ],
);
auto test_to_numpy_array_structured_dtype_multiple_fields(graph_type, edge) -> void {
    G = graph_type([edge]);
    dtype = np.dtype([("weight", double), ("cost", double), ("flow", double)]);
    A = nx.to_numpy_array(G, dtype=dtype, weight=None);
    for (auto attr : dtype.names) {
        expected = nx.to_numpy_array(G, dtype=double, weight=attr);
        npt.assert_array_equal(A[attr], expected);
}

// @pytest.mark.parametrize("G", (nx.Graph(), nx.DiGraph()));
auto test_to_numpy_array_structured_dtype_scalar_nonedge(G) -> void {
    G.add_edge(0, 1, weight=10);
    dtype = np.dtype([("weight", double), ("cost", double)]);
    A = nx.to_numpy_array(G, dtype=dtype, weight=None, nonedge=np.nan);
    for (auto attr : dtype.names) {
        expected = nx.to_numpy_array(G, dtype=double, weight=attr, nonedge=np.nan);
        npt.assert_array_equal(A[attr], expected);
}

// @pytest.mark.parametrize("G", (nx.Graph(), nx.DiGraph()));
auto test_to_numpy_array_structured_dtype_nonedge_ary(G) -> void {
    /** Similar to the scalar case, except has a different non-edge value for
    each named field.*/
    G.add_edge(0, 1, weight=10);
    dtype = np.dtype([("weight", double), ("cost", double)]);
    nonedges = np.array([(0, np.inf)], dtype=dtype);
    A = nx.to_numpy_array(G, dtype=dtype, weight=None, nonedge=nonedges);
    for (auto attr : dtype.names) {
        nonedge = nonedges[attr];
        expected = nx.to_numpy_array(G, dtype=double, weight=attr, nonedge=nonedge);
        npt.assert_array_equal(A[attr], expected);
}

auto test_to_numpy_array_structured_dtype_with_weight_raises() -> void {
    /** Using both a structured dtype (with named fields) and specifying a `weight`
    parameter is ambiguous.*/
    G = nx.path_graph(3);
    dtype = np.dtype([("weight", int), ("cost", int)]);
    exception_msg = "Specifying `weight` not supported for structured dtypes"
    with pytest.raises(ValueError, match=exception_msg):
        nx.to_numpy_array(G, dtype=dtype); // Default is weight="weight"
    with pytest.raises(ValueError, match=exception_msg):
        nx.to_numpy_array(G, dtype=dtype, weight="cost");
}

// @pytest.mark.parametrize("graph_type", (nx.MultiGraph, nx.MultiDiGraph));
auto test_to_numpy_array_structured_multigraph_raises(graph_type) -> void {
    G = nx.path_graph(3, create_using=graph_type);
    dtype = np.dtype([("weight", int), ("cost", int)]);
    with pytest.raises(nx.NetworkXError, match="Structured arrays are not supported"):
        nx.to_numpy_array(G, dtype=dtype, weight=None);
