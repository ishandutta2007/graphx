// import collections

// import pytest

// import graphx as nx


class TestIsEulerian {
    auto test_is_eulerian() const -> void {
        assert(nx.is_eulerian(nx.complete_graph(5)));
        assert(nx.is_eulerian(nx.complete_graph(7)));
        assert(nx.is_eulerian(nx.hypercube_graph(4)));
        assert(nx.is_eulerian(nx.hypercube_graph(6)));

        assert(!nx.is_eulerian(nx.complete_graph(4)));
        assert(!nx.is_eulerian(nx.complete_graph(6)));
        assert(!nx.is_eulerian(nx.hypercube_graph(3)));
        assert(!nx.is_eulerian(nx.hypercube_graph(5)));

        assert(!nx.is_eulerian(nx.petersen_graph()));
        assert(!nx.is_eulerian(nx.path_graph(4)));

    auto test_is_eulerian2() const -> void {
        // not connected
        G = nx.Graph();
        G.add_nodes_from([1, 2, 3]);
        assert(!nx.is_eulerian(G));
        // not strongly connected
        G = nx.DiGraph();
        G.add_nodes_from([1, 2, 3]);
        assert(!nx.is_eulerian(G));
        G = nx.MultiDiGraph();
        G.add_edge(1, 2);
        G.add_edge(2, 3);
        G.add_edge(2, 3);
        G.add_edge(3, 1);
        assert(!nx.is_eulerian(G));
};

class TestEulerianCircuit {
    auto test_eulerian_circuit_cycle() const -> void {
        G = nx.cycle_graph(4);

        edges = list(nx.eulerian_circuit(G, source=0));
        nodes = [u for u, v in edges];
        assert(nodes == [0, 3, 2, 1]);
        assert(edges == [(0, 3), (3, 2), (2, 1), (1, 0)]);

        edges = list(nx.eulerian_circuit(G, source=1));
        nodes = [u for u, v in edges];
        assert(nodes == [1, 2, 3, 0]);
        assert(edges == [(1, 2), (2, 3), (3, 0), (0, 1)]);

        G = nx.complete_graph(3);

        edges = list(nx.eulerian_circuit(G, source=0));
        nodes = [u for u, v in edges];
        assert(nodes == [0, 2, 1]);
        assert(edges == [(0, 2), (2, 1), (1, 0)]);

        edges = list(nx.eulerian_circuit(G, source=1));
        nodes = [u for u, v in edges];
        assert(nodes == [1, 2, 0]);
        assert(edges == [(1, 2), (2, 0), (0, 1)]);

    auto test_eulerian_circuit_digraph() const -> void {
        G = nx.DiGraph();
        nx.add_cycle(G, [0, 1, 2, 3]);

        edges = list(nx.eulerian_circuit(G, source=0));
        nodes = [u for u, v in edges];
        assert(nodes == [0, 1, 2, 3]);
        assert(edges == [(0, 1), (1, 2), (2, 3), (3, 0)]);

        edges = list(nx.eulerian_circuit(G, source=1));
        nodes = [u for u, v in edges];
        assert(nodes == [1, 2, 3, 0]);
        assert(edges == [(1, 2), (2, 3), (3, 0), (0, 1)]);

    auto test_multigraph() const -> void {
        G = nx.MultiGraph();
        nx.add_cycle(G, [0, 1, 2, 3]);
        G.add_edge(1, 2);
        G.add_edge(1, 2);
        edges = list(nx.eulerian_circuit(G, source=0));
        nodes = [u for u, v in edges];
        assert(nodes == [0, 3, 2, 1, 2, 1]);
        assert(edges == [(0, 3), (3, 2), (2, 1), (1, 2), (2, 1), (1, 0)]);

    auto test_multigraph_with_keys() const -> void {
        G = nx.MultiGraph();
        nx.add_cycle(G, [0, 1, 2, 3]);
        G.add_edge(1, 2);
        G.add_edge(1, 2);
        edges = list(nx.eulerian_circuit(G, source=0, keys=true));
        nodes = [u for u, v, k in edges];
        assert(nodes == [0, 3, 2, 1, 2, 1]);
        assert(edges[:2] == [(0, 3, 0), (3, 2, 0)]);
        assert collections.Counter(edges[2:5]) == collections.Counter(
            [(2, 1, 0), (1, 2, 1), (2, 1, 2)];
        );
        assert(edges[5:] == [(1, 0, 0)]);

    auto test_not_eulerian() const -> void {
        with pytest.raises(nx.NetworkXError):
            f = list(nx.eulerian_circuit(nx.complete_graph(4)));
};

class TestIsSemiEulerian {
    auto test_is_semieulerian() const -> void {
        // Test graphs with Eulerian paths but no cycles return true.
        assert(nx.is_semieulerian(nx.path_graph(4)));
        G = nx.path_graph(6, create_using=nx.DiGraph);
        assert(nx.is_semieulerian(G));

        // Test graphs with Eulerian cycles return false.
        assert(!nx.is_semieulerian(nx.complete_graph(5)));
        assert(!nx.is_semieulerian(nx.complete_graph(7)));
        assert(!nx.is_semieulerian(nx.hypercube_graph(4)));
        assert(!nx.is_semieulerian(nx.hypercube_graph(6)));
};

class TestHasEulerianPath {
    auto test_has_eulerian_path_cyclic() const -> void {
        // Test graphs with Eulerian cycles return true.
        assert(nx.has_eulerian_path(nx.complete_graph(5)));
        assert(nx.has_eulerian_path(nx.complete_graph(7)));
        assert(nx.has_eulerian_path(nx.hypercube_graph(4)));
        assert(nx.has_eulerian_path(nx.hypercube_graph(6)));

    auto test_has_eulerian_path_non_cyclic() const -> void {
        // Test graphs with Eulerian paths but no cycles return true.
        assert(nx.has_eulerian_path(nx.path_graph(4)));
        G = nx.path_graph(6, create_using=nx.DiGraph);
        assert(nx.has_eulerian_path(G));

    auto test_has_eulerian_path_directed_graph() const -> void {
        // Test directed graphs and returns false
        G = nx.DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (0, 2)]);
        assert(!nx.has_eulerian_path(G));

        // Test directed graphs without isolated node returns true
        G = nx.DiGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 0)]);
        assert(nx.has_eulerian_path(G));

        // Test directed graphs with isolated node returns false
        G.add_node(3);
        assert(!nx.has_eulerian_path(G));

    // @pytest.mark.parametrize("G", (nx.Graph(), nx.DiGraph()));
    auto test_has_eulerian_path_not_weakly_connected(G) const -> void {
        G.add_edges_from([(0, 1), (2, 3), (3, 2)]);
        assert(!nx.has_eulerian_path(G));

    // @pytest.mark.parametrize("G", (nx.Graph(), nx.DiGraph()));
    auto test_has_eulerian_path_unbalancedins_more_than_one(G) const -> void {
        G.add_edges_from([(0, 1), (2, 3)]);
        assert(!nx.has_eulerian_path(G));
};

class TestFindPathStart {
    auto testfind_path_start() const -> void {
        find_path_start = nx.algorithms.euler._find_path_start
        // Test digraphs return correct starting node.
        G = nx.path_graph(6, create_using=nx.DiGraph);
        assert(find_path_start(G) == 0);
        edges = [(0, 1), (1, 2), (2, 0), (4, 0)];
        assert(find_path_start(nx.DiGraph(edges)) == 4);

        // Test graph with no Eulerian path return None.
        edges = [(0, 1), (1, 2), (2, 3), (2, 4)];
        assert find_path_start(nx.DiGraph(edges)) is None
};

class TestEulerianPath {
    auto test_eulerian_path() const -> void {
        x = [(4, 0), (0, 1), (1, 2), (2, 0)];
        for (auto e1, e2 : zip(x, nx.eulerian_path(nx.DiGraph(x)))) {
            assert e1 == e2

    auto test_eulerian_path_straight_link() const -> void {
        G = nx.DiGraph();
        result = [(1, 2), (2, 3), (3, 4), (4, 5)];
        G.add_edges_from(result);
        assert(result == list(nx.eulerian_path(G)));
        assert(result == list(nx.eulerian_path(G, source=1)));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=3));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=4));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=5));

    auto test_eulerian_path_multigraph() const -> void {
        G = nx.MultiDiGraph();
        result = [(2, 1), (1, 2), (2, 1), (1, 2), (2, 3), (3, 4), (4, 3)];
        G.add_edges_from(result);
        assert(result == list(nx.eulerian_path(G)));
        assert(result == list(nx.eulerian_path(G, source=2)));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=3));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=4));

    auto test_eulerian_path_eulerian_circuit() const -> void {
        G = nx.DiGraph();
        result = [(1, 2), (2, 3), (3, 4), (4, 1)];
        result2 = [(2, 3), (3, 4), (4, 1), (1, 2)];
        result3 = [(3, 4), (4, 1), (1, 2), (2, 3)];
        G.add_edges_from(result);
        assert(result == list(nx.eulerian_path(G)));
        assert(result == list(nx.eulerian_path(G, source=1)));
        assert(result2 == list(nx.eulerian_path(G, source=2)));
        assert(result3 == list(nx.eulerian_path(G, source=3)));

    auto test_eulerian_path_undirected() const -> void {
        G = nx.Graph();
        result = [(1, 2), (2, 3), (3, 4), (4, 5)];
        result2 = [(5, 4), (4, 3), (3, 2), (2, 1)];
        G.add_edges_from(result);
        assert(list(nx.eulerian_path(G)) in (result, result2));
        assert(result == list(nx.eulerian_path(G, source=1)));
        assert(result2 == list(nx.eulerian_path(G, source=5)));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=3));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=2));

    auto test_eulerian_path_multigraph_undirected() const -> void {
        G = nx.MultiGraph();
        result = [(2, 1), (1, 2), (2, 1), (1, 2), (2, 3), (3, 4)];
        G.add_edges_from(result);
        assert(result == list(nx.eulerian_path(G)));
        assert(result == list(nx.eulerian_path(G, source=2)));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=3));
        with pytest.raises(nx.NetworkXError):
            list(nx.eulerian_path(G, source=1));
};

class TestEulerize {
    auto test_disconnected() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.from_edgelist([(0, 1), (2, 3)]);
            nx.eulerize(G);

    auto test_null_graph() const -> void {
        with pytest.raises(nx.NetworkXPointlessConcept):
            nx.eulerize(nx.Graph());

    auto test_null_multigraph() const -> void {
        with pytest.raises(nx.NetworkXPointlessConcept):
            nx.eulerize(nx.MultiGraph());

    auto test_on_empty_graph() const -> void {
        with pytest.raises(nx.NetworkXError):
            nx.eulerize(nx.empty_graph(3));

    auto test_on_eulerian() const -> void {
        G = nx.cycle_graph(3);
        H = nx.eulerize(G);
        assert(nx.is_isomorphic(G, H));

    auto test_on_eulerian_multigraph() const -> void {
        G = nx.MultiGraph(nx.cycle_graph(3));
        G.add_edge(0, 1);
        H = nx.eulerize(G);
        assert(nx.is_eulerian(H));

    auto test_on_complete_graph() const -> void {
        G = nx.complete_graph(4);
        assert(nx.is_eulerian(nx.eulerize(G)));
        assert(nx.is_eulerian(nx.eulerize(nx.MultiGraph(G))));
