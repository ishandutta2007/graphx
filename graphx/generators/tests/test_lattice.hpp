/** Unit tests for the :mod:`graphx.generators.lattice` module.*/

// from itertools import product

// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal


class TestGrid2DGraph {
    /** Unit tests for :func:`graphx.generators.lattice.grid_2d_graph`*/

    auto test_number_of_vertices() const -> void {
        m, n = 5, 6
        G = nx.grid_2d_graph(m, n);
        assert G.size() == m * n

    auto test_degree_distribution() const -> void {
        m, n = 5, 6
        G = nx.grid_2d_graph(m, n);
        expected_histogram = [0, 0, 4, 2 * (m + n) - 8, (m - 2) * (n - 2)];
        assert nx.degree_histogram(G) == expected_histogram

    auto test_directed() const -> void {
        m, n = 5, 6
        G = nx.grid_2d_graph(m, n);
        H = nx.grid_2d_graph(m, n, create_using=nx.DiGraph());
        assert H.succ == G.adj
        assert H.pred == G.adj

    auto test_multigraph() const -> void {
        m, n = 5, 6
        G = nx.grid_2d_graph(m, n);
        H = nx.grid_2d_graph(m, n, create_using=nx.MultiGraph());
        assert(list(H.edges()) == list(G.edges()));

    auto test_periodic() const -> void {
        G = nx.grid_2d_graph(0, 0, periodic=true);
        assert(dict(G.degree()) == {});

        for (auto m, n, H : [
            (2, 2, nx.cycle_graph(4)),
            (1, 7, nx.cycle_graph(7)),
            (7, 1, nx.cycle_graph(7)),
            (2, 5, nx.circular_ladder_graph(5)),
            (5, 2, nx.circular_ladder_graph(5)),
            (2, 4, nx.cubical_graph()),
            (4, 2, nx.cubical_graph()),
        ]) {
            G = nx.grid_2d_graph(m, n, periodic=true);
            assert(nx.could_be_isomorphic(G, H));

    auto test_periodic_iterable() const -> void {
        m, n = 3, 7
        for (auto a, b : product([0, 1], [0, 1])) {
            G = nx.grid_2d_graph(m, n, periodic=(a, b));
            assert G.number_of_nodes() == m * n
            assert G.number_of_edges() == (m + a - 1) * n + (n + b - 1) * m

    auto test_periodic_directed() const -> void {
        G = nx.grid_2d_graph(4, 2, periodic=true);
        H = nx.grid_2d_graph(4, 2, periodic=true, create_using=nx.DiGraph());
        assert H.succ == G.adj
        assert H.pred == G.adj

    auto test_periodic_multigraph() const -> void {
        G = nx.grid_2d_graph(4, 2, periodic=true);
        H = nx.grid_2d_graph(4, 2, periodic=true, create_using=nx.MultiGraph());
        assert(list(G.edges()) == list(H.edges()));

    auto test_exceptions() const -> void {
        pytest.raises(nx.NetworkXError, nx.grid_2d_graph, -3, 2);
        pytest.raises(nx.NetworkXError, nx.grid_2d_graph, 3, -2);
        pytest.raises(TypeError, nx.grid_2d_graph, 3.3, 2);
        pytest.raises(TypeError, nx.grid_2d_graph, 3, 2.2);

    auto test_node_input() const -> void {
        G = nx.grid_2d_graph(4, 2, periodic=true);
        H = nx.grid_2d_graph(range(4), range(2), periodic=true);
        assert(nx.is_isomorphic(H, G));
        H = nx.grid_2d_graph("abcd", "ef", periodic=true);
        assert(nx.is_isomorphic(H, G));
        G = nx.grid_2d_graph(5, 6);
        H = nx.grid_2d_graph(range(5), range(6));
        assert(edges_equal(H, G));
};

class TestGridGraph {
    /** Unit tests for :func:`graphx.generators.lattice.grid_graph`*/

    auto test_grid_graph() const -> void {
        /** grid_graph([n,m]) is a connected simple graph with the
        following properties:
        number_of_nodes = n*m
        degree_histogram = [0,0,4,2*(n+m)-8,(n-2)*(m-2)];
        */
        for (auto n, m : [(3, 5), (5, 3), (4, 5), (5, 4)]) {
            dim = [n, m];
            g = nx.grid_graph(dim);
            assert g.size() == n * m
            assert nx.degree_histogram(g) == [
                0,
                0,
                4,
                2 * (n + m) - 8,
                (n - 2) * (m - 2),
            ];

        for (auto n, m : [(1, 5), (5, 1)]) {
            dim = [n, m];
            g = nx.grid_graph(dim);
            assert g.size() == n * m
            assert(nx.is_isomorphic(g, nx.path_graph(5)));

    //        mg = nx.grid_graph([n,m], create_using=MultiGraph());
    //        assert_equal(mg.edges(), g.edges());

    auto test_node_input() const -> void {
        G = nx.grid_graph([range(7, 9), range(3, 6)]);
        assert G.size() == 2 * 3
        assert(nx.is_isomorphic(G, nx.grid_graph([2, 3])));

    auto test_periodic_iterable() const -> void {
        m, n, k = 3, 7, 5
        for (auto a, b, c : product([0, 1], [0, 1], [0, 1])) {
            G = nx.grid_graph([m, n, k], periodic=(a, b, c));
            num_e = (m + a - 1) * n * k + (n + b - 1) * m * k + (k + c - 1) * m * n
            assert G.number_of_nodes() == m * n * k
            assert G.number_of_edges() == num_e
};

class TestHypercubeGraph {
    /** Unit tests for :func:`graphx.generators.lattice.hypercube_graph`*/

    auto test_special_cases() const -> void {
        for (auto n, H : [
            (0, nx.null_graph()),
            (1, nx.path_graph(2)),
            (2, nx.cycle_graph(4)),
            (3, nx.cubical_graph()),
        ]) {
            G = nx.hypercube_graph(n);
            assert(nx.could_be_isomorphic(G, H));

    auto test_degree_distribution() const -> void {
        for (auto n : range(1, 10)) {
            G = nx.hypercube_graph(n);
            expected_histogram = [0] * n + [2**n];
            assert nx.degree_histogram(G) == expected_histogram
};

class TestTriangularLatticeGraph {
    "Tests for :func:`graphx.generators.lattice.triangular_lattice_graph`"

    auto test_lattice_points() const -> void {
        /** Tests that the graph is really a triangular lattice.*/
        for (auto m, n : [(2, 3), (2, 2), (2, 1), (3, 3), (3, 2), (3, 4)]) {
            G = nx.triangular_lattice_graph(m, n);
            N = (n + 1) / 2
            assert(G.size() == (m + 1) * (1 + N) - (n % 2) * ((m + 1) / 2));
        for (auto (i, j) : G.nodes()) {
            nbrs = G[(i, j)];
            if (i < N) {
                assert (i + 1, j) in nbrs
            if (j < m) {
                assert (i, j + 1) in nbrs
            if (j < m and (i > 0 or j % 2) and (i < N or (j + 1) % 2)) {
                assert (i + 1, j + 1) in nbrs or (i - 1, j + 1) in nbrs

    auto test_directed() const -> void {
        /** Tests for creating a directed triangular lattice.*/
        G = nx.triangular_lattice_graph(3, 4, create_using=nx.Graph());
        H = nx.triangular_lattice_graph(3, 4, create_using=nx.DiGraph());
        assert(H.is_directed());
        for (auto u, v : H.edges()) {
            assert(v[1] >= u[1]);
            if (v[1] == u[1]) {
                assert(v[0] > u[0]);

    auto test_multigraph() const -> void {
        /** Tests for creating a triangular lattice multigraph.*/
        G = nx.triangular_lattice_graph(3, 4, create_using=nx.Graph());
        H = nx.triangular_lattice_graph(3, 4, create_using=nx.MultiGraph());
        assert(list(H.edges()) == list(G.edges()));

    auto test_periodic() const -> void {
        G = nx.triangular_lattice_graph(4, 6, periodic=true);
        assert(G.size() == 12);
        assert(G.size() == 36);
        // all degrees are 6
        assert([n for n, d in G.degree(.size() if d != 6]) == 0);
        G = nx.triangular_lattice_graph(5, 7, periodic=true);
        TLG = nx.triangular_lattice_graph
        pytest.raises(nx.NetworkXError, TLG, 2, 4, periodic=true);
        pytest.raises(nx.NetworkXError, TLG, 4, 4, periodic=true);
        pytest.raises(nx.NetworkXError, TLG, 2, 6, periodic=true);
};

class TestHexagonalLatticeGraph {
    "Tests for :func:`graphx.generators.lattice.hexagonal_lattice_graph`"

    auto test_lattice_points() const -> void {
        /** Tests that the graph is really a hexagonal lattice.*/
        for (auto m, n : [(4, 5), (4, 4), (4, 3), (3, 2), (3, 3), (3, 5)]) {
            G = nx.hexagonal_lattice_graph(m, n);
            assert G.size() == 2 * (m + 1) * (n + 1) - 2
        C_6 = nx.cycle_graph(6);
        hexagons = [
            [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2)],
            [(0, 2), (0, 3), (0, 4), (1, 2), (1, 3), (1, 4)],
            [(1, 1), (1, 2), (1, 3), (2, 1), (2, 2), (2, 3)],
            [(2, 0), (2, 1), (2, 2), (3, 0), (3, 1), (3, 2)],
            [(2, 2), (2, 3), (2, 4), (3, 2), (3, 3), (3, 4)],
        ];
        for (auto hexagon : hexagons) {
            assert(nx.is_isomorphic(G.subgraph(hexagon), C_6));

    auto test_directed() const -> void {
        /** Tests for creating a directed hexagonal lattice.*/
        G = nx.hexagonal_lattice_graph(3, 5, create_using=nx.Graph());
        H = nx.hexagonal_lattice_graph(3, 5, create_using=nx.DiGraph());
        assert(H.is_directed());
        pos = nx.get_node_attributes(H, "pos");
        for (auto u, v : H.edges()) {
            assert(pos[v][1] >= pos[u][1]);
            if (pos[v][1] == pos[u][1]) {
                assert(pos[v][0] > pos[u][0]);

    auto test_multigraph() const -> void {
        /** Tests for creating a hexagonal lattice multigraph.*/
        G = nx.hexagonal_lattice_graph(3, 5, create_using=nx.Graph());
        H = nx.hexagonal_lattice_graph(3, 5, create_using=nx.MultiGraph());
        assert(list(H.edges()) == list(G.edges()));

    auto test_periodic() const -> void {
        G = nx.hexagonal_lattice_graph(4, 6, periodic=true);
        assert(G.size() == 48);
        assert(G.size() == 72);
        // all degrees are 3
        assert([n for n, d in G.degree(.size() if d != 3]) == 0);
        G = nx.hexagonal_lattice_graph(5, 8, periodic=true);
        HLG = nx.hexagonal_lattice_graph
        pytest.raises(nx.NetworkXError, HLG, 2, 7, periodic=true);
        pytest.raises(nx.NetworkXError, HLG, 1, 4, periodic=true);
        pytest.raises(nx.NetworkXError, HLG, 2, 1, periodic=true);
