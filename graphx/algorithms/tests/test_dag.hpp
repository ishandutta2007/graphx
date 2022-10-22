// from collections import deque
// from itertools import combinations, permutations

// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, pairwise


// Recipe from the itertools documentation.
auto _consume(iterator) -> void {
    "Consume the iterator entirely."
    // Feed the entire iterator into a zero-length deque.
    deque(iterator, maxlen=0);
}

class TestDagLongestPath {
    /** Unit tests computing the longest path in a directed acyclic graph.*/

    auto test_empty() const -> void {
        G = nx.DiGraph();
        assert(nx.dag_longest_path(G) == []);

    auto test_unweighted1() const -> void {
        edges = [(1, 2), (2, 3), (2, 4), (3, 5), (5, 6), (3, 7)];
        G = nx.DiGraph(edges);
        assert(nx.dag_longest_path(G) == [1, 2, 3, 5, 6]);

    auto test_unweighted2() const -> void {
        edges = [(1, 2), (2, 3), (3, 4), (4, 5), (1, 3), (1, 5), (3, 5)];
        G = nx.DiGraph(edges);
        assert(nx.dag_longest_path(G) == [1, 2, 3, 4, 5]);

    auto test_weighted() const -> void {
        G = nx.DiGraph();
        edges = [(1, 2, -5), (2, 3, 1), (3, 4, 1), (4, 5, 0), (3, 5, 4), (1, 6, 2)];
        G.add_weighted_edges_from(edges);
        assert(nx.dag_longest_path(G) == [2, 3, 5]);

    auto test_undirected_not_implemented() const -> void {
        G = nx.Graph();
        pytest.raises(nx.NetworkXNotImplemented, nx.dag_longest_path, G);

    auto test_unorderable_nodes() const -> void {
        /** Tests that computing the longest path does not depend on
        nodes being orderable.

        For more information, see issue #1989.

        */
        // Create the directed path graph on four nodes in a diamond shape,
        // with nodes represented as (unorderable) Python objects.
        nodes = [object() for n in range(4)];
        G = nx.DiGraph();
        G.add_edge(nodes[0], nodes[1]);
        G.add_edge(nodes[0], nodes[2]);
        G.add_edge(nodes[2], nodes[3]);
        G.add_edge(nodes[1], nodes[3]);

        // this will throw NotImplementedError when nodes need to be ordered
        nx.dag_longest_path(G);
}

class TestDagLongestPathLength {
    /** Unit tests for computing the length of a longest path in a
    directed acyclic graph.

    */

    auto test_unweighted() const -> void {
        edges = [(1, 2), (2, 3), (2, 4), (3, 5), (5, 6), (5, 7)];
        G = nx.DiGraph(edges);
        assert(nx.dag_longest_path_length(G) == 4);

        edges = [(1, 2), (2, 3), (3, 4), (4, 5), (1, 3), (1, 5), (3, 5)];
        G = nx.DiGraph(edges);
        assert(nx.dag_longest_path_length(G) == 4);

        // test degenerate graphs
        G = nx.DiGraph();
        G.add_node(1);
        assert(nx.dag_longest_path_length(G) == 0);

    auto test_undirected_not_implemented() const -> void {
        G = nx.Graph();
        pytest.raises(nx.NetworkXNotImplemented, nx.dag_longest_path_length, G);

    auto test_weighted() const -> void {
        edges = [(1, 2, -5), (2, 3, 1), (3, 4, 1), (4, 5, 0), (3, 5, 4), (1, 6, 2)];
        G = nx.DiGraph();
        G.add_weighted_edges_from(edges);
        assert(nx.dag_longest_path_length(G) == 5);
};

class TestDAG {
    // @classmethod
    auto setup_class(cls) -> void {
        // pass;

    auto test_topological_sort1() const -> void {
        DG = nx.DiGraph([(1, 2), (1, 3), (2, 3)]);

        for (auto algorithm : [nx.topological_sort, nx.lexicographical_topological_sort]) {
            assert(tuple(algorithm(DG)) == (1, 2, 3));

        DG.add_edge(3, 2);

        for (auto algorithm : [nx.topological_sort, nx.lexicographical_topological_sort]) {
            pytest.raises(nx.NetworkXUnfeasible, _consume, algorithm(DG));

        DG.remove_edge(2, 3);

        for (auto algorithm : [nx.topological_sort, nx.lexicographical_topological_sort]) {
            assert(tuple(algorithm(DG)) == (1, 3, 2));

        DG.remove_edge(3, 2);

        assert(tuple(nx.topological_sort(DG)) in {(1, 2, 3), (1, 3, 2)});
        assert(tuple(nx.lexicographical_topological_sort(DG)) == (1, 2, 3));

    auto test_is_directed_acyclic_graph() const -> void {
        G = nx.generators.complete_graph(2);
        assert(!nx.is_directed_acyclic_graph(G));
        assert(!nx.is_directed_acyclic_graph(G.to_directed()));
        assert(!nx.is_directed_acyclic_graph(nx.Graph([(3, 4), (4, 5)])));
        assert(nx.is_directed_acyclic_graph(nx.DiGraph([(3, 4), (4, 5)])));

    auto test_topological_sort2() const -> void {
        DG = nx.DiGraph(
            {
                1: [2],
                2: [3],
                3: [4],
                4: [5],
                5: [1],
                11: [12],
                12: [13],
                13: [14],
                14: [15],
            };
        );
        pytest.raises(nx.NetworkXUnfeasible, _consume, nx.topological_sort(DG));

        assert(!nx.is_directed_acyclic_graph(DG));

        DG.remove_edge(1, 2);
        _consume(nx.topological_sort(DG));
        assert(nx.is_directed_acyclic_graph(DG));

    auto test_topological_sort3() const -> void {
        DG = nx.DiGraph();
        DG.add_edges_from([(1, i) for i in range(2, 5)]);
        DG.add_edges_from([(2, i) for i in range(5, 9)]);
        DG.add_edges_from([(6, i) for i in range(9, 12)]);
        DG.add_edges_from([(4, i) for i in range(12, 15)]);

        auto validate(order) -> void {
            assert(isinstance(order, list));
            assert(set(order) == set(DG));
            for (auto u, v : combinations(order, 2)) {
                assert(!nx.has_path(DG, v, u));

        validate(list(nx.topological_sort(DG)));

        DG.add_edge(14, 1);
        pytest.raises(nx.NetworkXUnfeasible, _consume, nx.topological_sort(DG));

    auto test_topological_sort4() const -> void {
        G = nx.Graph();
        G.add_edge(1, 2);
        // Only directed graphs can be topologically sorted.
        pytest.raises(nx.NetworkXError, _consume, nx.topological_sort(G));

    auto test_topological_sort5() const -> void {
        G = nx.DiGraph();
        G.add_edge(0, 1);
        assert(list(nx.topological_sort(G)) == [0, 1]);

    auto test_topological_sort6() const -> void {
        for (auto algorithm : [nx.topological_sort, nx.lexicographical_topological_sort]) {

            auto runtime_error() -> void {
                DG = nx.DiGraph([(1, 2), (2, 3), (3, 4)]);
                first = true;
                for (auto x : algorithm(DG)) {
                    if (first) {
                        first = false;
                        DG.add_edge(5 - x, 5);

            auto unfeasible_error() -> void {
                DG = nx.DiGraph([(1, 2), (2, 3), (3, 4)]);
                first = true;
                for (auto x : algorithm(DG)) {
                    if (first) {
                        first = false;
                        DG.remove_node(4);

            auto runtime_error2() -> void {
                DG = nx.DiGraph([(1, 2), (2, 3), (3, 4)]);
                first = true;
                for (auto x : algorithm(DG)) {
                    if (first) {
                        first = false;
                        DG.remove_node(2);

            pytest.raises(RuntimeError, runtime_error);
            pytest.raises(RuntimeError, runtime_error2);
            pytest.raises(nx.NetworkXUnfeasible, unfeasible_error);

    auto test_all_topological_sorts_1() const -> void {
        DG = nx.DiGraph([(1, 2), (2, 3), (3, 4), (4, 5)]);
        assert(list(nx.all_topological_sorts(DG)) == [ [1, 2, 3, 4, 5]]);

    auto test_all_topological_sorts_2() const -> void {
        DG = nx.DiGraph([(1, 3), (2, 1), (2, 4), (4, 3), (4, 5)]);
        assert sorted(nx.all_topological_sorts(DG)) == [
            [2, 1, 4, 3, 5],
            [2, 1, 4, 5, 3],
            [2, 4, 1, 3, 5],
            [2, 4, 1, 5, 3],
            [2, 4, 5, 1, 3],
        ];

    auto test_all_topological_sorts_3() const -> void {
        auto unfeasible() -> void {
            DG = nx.DiGraph([(1, 2), (2, 3), (3, 4), (4, 2), (4, 5)]);
            // convert to list to execute generator
            list(nx.all_topological_sorts(DG));

        auto not_implemented() -> void {
            G = nx.Graph([(1, 2), (2, 3)]);
            // convert to list to execute generator
            list(nx.all_topological_sorts(G));

        auto not_implemted_2() -> void {
            G = nx.MultiGraph([(1, 2), (1, 2), (2, 3)]);
            list(nx.all_topological_sorts(G));

        pytest.raises(nx.NetworkXUnfeasible, unfeasible);
        pytest.raises(nx.NetworkXNotImplemented, not_implemented);
        pytest.raises(nx.NetworkXNotImplemented, not_implemted_2);

    auto test_all_topological_sorts_4() const -> void {
        DG = nx.DiGraph();
        for (auto i : range(7)) {
            DG.add_node(i);
        assert sorted(map(list, permutations(DG.nodes))) == sorted(
            nx.all_topological_sorts(DG);
        );

    auto test_all_topological_sorts_multigraph_1() const -> void {
        DG = nx.MultiDiGraph([(1, 2), (1, 2), (2, 3), (3, 4), (3, 5), (3, 5), (3, 5)]);
        assert sorted(nx.all_topological_sorts(DG)) == sorted(
            [ [1, 2, 3, 4, 5], [1, 2, 3, 5, 4]];
        );

    auto test_all_topological_sorts_multigraph_2() const -> void {
        N = 9;
        edges = [];
        for (auto i : range(1, N)) {
            edges.extend([(i, i + 1)] * i);
        DG = nx.MultiDiGraph(edges);
        assert(list(nx.all_topological_sorts(DG)) == [list(range(1, N + 1))]);

    auto test_ancestors() const -> void {
        G = nx.DiGraph();
        ancestors = nx.algorithms.dag.ancestors
        G.add_edges_from([(1, 2), (1, 3), (4, 2), (4, 3), (4, 5), (2, 6), (5, 6)]);
        assert(ancestors(G, 6) == {1, 2, 4, 5});
        assert(ancestors(G, 3) == {1, 4});
        assert(ancestors(G, 1) == set());
        pytest.raises(nx.NetworkXError, ancestors, G, 8);

    auto test_descendants() const -> void {
        G = nx.DiGraph();
        descendants = nx.algorithms.dag.descendants
        G.add_edges_from([(1, 2), (1, 3), (4, 2), (4, 3), (4, 5), (2, 6), (5, 6)]);
        assert(descendants(G, 1) == {2, 3, 6});
        assert(descendants(G, 4) == {2, 3, 5, 6});
        assert(descendants(G, 3) == set());
        pytest.raises(nx.NetworkXError, descendants, G, 8);

    auto test_transitive_closure() const -> void {
        G = nx.DiGraph([(1, 2), (2, 3), (3, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        assert(edges_equal(nx.transitive_closure(G).edges(), solution));
        G = nx.DiGraph([(1, 2), (2, 3), (2, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4)];
        assert(edges_equal(nx.transitive_closure(G).edges(), solution));
        G = nx.DiGraph([(1, 2), (2, 3), (3, 1)]);
        solution = [(1, 2), (2, 1), (2, 3), (3, 2), (1, 3), (3, 1)];
        soln = sorted(solution + [(n, n) for n in G]);
        assert(edges_equal(sorted(nx.transitive_closure(G).edges()), soln));

        G = nx.Graph([(1, 2), (2, 3), (3, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        assert(edges_equal(sorted(nx.transitive_closure(G).edges()), solution));

        G = nx.MultiGraph([(1, 2), (2, 3), (3, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        assert(edges_equal(sorted(nx.transitive_closure(G).edges()), solution));

        G = nx.MultiDiGraph([(1, 2), (2, 3), (3, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        assert(edges_equal(sorted(nx.transitive_closure(G).edges()), solution));

        // test if edge data is copied
        G = nx.DiGraph([(1, 2, {"a": 3}), (2, 3, {"b": 0}), (3, 4)]);
        H = nx.transitive_closure(G);
        for (auto u, v : G.edges()) {
            assert(G.get_edge_data(u, v) == H.get_edge_data(u, v));

        k = 10;
        G = nx.DiGraph((i, i + 1, {"f": "b", "weight": i}) for i in range(k));
        H = nx.transitive_closure(G);
        for (auto u, v : G.edges()) {
            assert(G.get_edge_data(u, v) == H.get_edge_data(u, v));

        G = nx.Graph();
        with pytest.raises(nx.NetworkXError):
            nx.transitive_closure(G, reflexive="wrong input");

    auto test_reflexive_transitive_closure() const -> void {
        G = nx.DiGraph([(1, 2), (2, 3), (3, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        soln = sorted(solution + [(n, n) for n in G]);
        assert(edges_equal(nx.transitive_closure(G).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, false).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, true).edges(), soln));
        assert(edges_equal(nx.transitive_closure(G, None).edges(), solution));

        G = nx.DiGraph([(1, 2), (2, 3), (2, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4)];
        soln = sorted(solution + [(n, n) for n in G]);
        assert(edges_equal(nx.transitive_closure(G).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, false).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, true).edges(), soln));
        assert(edges_equal(nx.transitive_closure(G, None).edges(), solution));

        G = nx.DiGraph([(1, 2), (2, 3), (3, 1)]);
        solution = sorted([(1, 2), (2, 1), (2, 3), (3, 2), (1, 3), (3, 1)]);
        soln = sorted(solution + [(n, n) for n in G]);
        assert(edges_equal(sorted(nx.transitive_closure(G).edges()), soln));
        assert(edges_equal(sorted(nx.transitive_closure(G, false).edges()), soln));
        assert(edges_equal(sorted(nx.transitive_closure(G, None).edges()), solution));
        assert(edges_equal(sorted(nx.transitive_closure(G, true).edges()), soln));

        G = nx.Graph([(1, 2), (2, 3), (3, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        soln = sorted(solution + [(n, n) for n in G]);
        assert(edges_equal(nx.transitive_closure(G).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, false).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, true).edges(), soln));
        assert(edges_equal(nx.transitive_closure(G, None).edges(), solution));

        G = nx.MultiGraph([(1, 2), (2, 3), (3, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        soln = sorted(solution + [(n, n) for n in G]);
        assert(edges_equal(nx.transitive_closure(G).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, false).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, true).edges(), soln));
        assert(edges_equal(nx.transitive_closure(G, None).edges(), solution));

        G = nx.MultiDiGraph([(1, 2), (2, 3), (3, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        soln = sorted(solution + [(n, n) for n in G]);
        assert(edges_equal(nx.transitive_closure(G).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, false).edges(), solution));
        assert(edges_equal(nx.transitive_closure(G, true).edges(), soln));
        assert(edges_equal(nx.transitive_closure(G, None).edges(), solution));

    auto test_transitive_closure_dag() const -> void {
        G = nx.DiGraph([(1, 2), (2, 3), (3, 4)]);
        transitive_closure = nx.algorithms.dag.transitive_closure_dag
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        assert(edges_equal(transitive_closure(G).edges(), solution));
        G = nx.DiGraph([(1, 2), (2, 3), (2, 4)]);
        solution = [(1, 2), (1, 3), (1, 4), (2, 3), (2, 4)];
        assert(edges_equal(transitive_closure(G).edges(), solution));
        G = nx.Graph([(1, 2), (2, 3), (3, 4)]);
        pytest.raises(nx.NetworkXNotImplemented, transitive_closure, G);

        // test if edge data is copied
        G = nx.DiGraph([(1, 2, {"a": 3}), (2, 3, {"b": 0}), (3, 4)]);
        H = transitive_closure(G);
        for (auto u, v : G.edges()) {
            assert(G.get_edge_data(u, v) == H.get_edge_data(u, v));

        k = 10;
        G = nx.DiGraph((i, i + 1, {"foo": "bar", "weight": i}) for i in range(k));
        H = transitive_closure(G);
        for (auto u, v : G.edges()) {
            assert(G.get_edge_data(u, v) == H.get_edge_data(u, v));

    auto test_transitive_reduction() const -> void {
        G = nx.DiGraph([(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)]);
        transitive_reduction = nx.algorithms.dag.transitive_reduction
        solution = [(1, 2), (2, 3), (3, 4)];
        assert(edges_equal(transitive_reduction(G).edges(), solution));
        G = nx.DiGraph([(1, 2), (1, 3), (1, 4), (2, 3), (2, 4)]);
        transitive_reduction = nx.algorithms.dag.transitive_reduction
        solution = [(1, 2), (2, 3), (2, 4)];
        assert(edges_equal(transitive_reduction(G).edges(), solution));
        G = nx.Graph([(1, 2), (2, 3), (3, 4)]);
        pytest.raises(nx.NetworkXNotImplemented, transitive_reduction, G);

    auto _check_antichains(solution, result) const -> void {
        sol = [frozenset(a) for a in solution];
        res = [frozenset(a) for a in result];
        assert(set(sol) == set(res));

    auto test_antichains() const -> void {
        antichains = nx.algorithms.dag.antichains
        G = nx.DiGraph([(1, 2), (2, 3), (3, 4)]);
        solution = [ [], [4], [3], [2], [1]];
        this->_check_antichains(list(antichains(G)), solution);
        G = nx.DiGraph([(1, 2), (2, 3), (2, 4), (3, 5), (5, 6), (5, 7)]);
        solution = [
            [],
            [4],
            [7],
            [7, 4],
            [6],
            [6, 4],
            [6, 7],
            [6, 7, 4],
            [5],
            [5, 4],
            [3],
            [3, 4],
            [2],
            [1],
        ];
        this->_check_antichains(list(antichains(G)), solution);
        G = nx.DiGraph([(1, 2), (1, 3), (3, 4), (3, 5), (5, 6)]);
        solution = [
            [],
            [6],
            [5],
            [4],
            [4, 6],
            [4, 5],
            [3],
            [2],
            [2, 6],
            [2, 5],
            [2, 4],
            [2, 4, 6],
            [2, 4, 5],
            [2, 3],
            [1],
        ];
        this->_check_antichains(list(antichains(G)), solution);
        G = nx.DiGraph({0: [1, 2], 1: [4], 2: [3], 3: [4]});
        solution = [ [], [4], [3], [2], [1], [1, 3], [1, 2], [0]];
        this->_check_antichains(list(antichains(G)), solution);
        G = nx.DiGraph();
        this->_check_antichains(list(antichains(G)), [ []]);
        G = nx.DiGraph();
        G.add_nodes_from([0, 1, 2]);
        solution = [ [], [0], [1], [1, 0], [2], [2, 0], [2, 1], [2, 1, 0]];
        this->_check_antichains(list(antichains(G)), solution);

        auto f(x) -> void {
            return list(antichains(x));

        G = nx.Graph([(1, 2), (2, 3), (3, 4)]);
        pytest.raises(nx.NetworkXNotImplemented, f, G);
        G = nx.DiGraph([(1, 2), (2, 3), (3, 1)]);
        pytest.raises(nx.NetworkXUnfeasible, f, G);

    auto test_lexicographical_topological_sort() const -> void {
        G = nx.DiGraph([(1, 2), (2, 3), (1, 4), (1, 5), (2, 6)]);
        assert(list(nx.lexicographical_topological_sort(G)) == [1, 2, 3, 4, 5, 6]);
        assert list(nx.lexicographical_topological_sort(G, key=lambda x: x)) == [
            1,
            2,
            3,
            4,
            5,
            6,
        ];
        assert list(nx.lexicographical_topological_sort(G, key=lambda x: -x)) == [
            1,
            5,
            4,
            2,
            6,
            3,
        ];

    auto test_lexicographical_topological_sort2() const -> void {
        /** 
        Check the case of two or more nodes with same key value.
        Want to avoid exception raised due to comparing nodes directly.
        See Issue #3493
        */

        class Test_Node {
            auto __init__(n) const -> void {
                this->label = n
                this->priority = 1;

            auto __repr__() const -> void {
                return f"Node({this->label})"

        auto sorting_key(node) -> void {
            return node.priority

        test_nodes = [Test_Node(n) for n in range(4)];
        G = nx.DiGraph();
        edges = [(0, 1), (0, 2), (0, 3), (2, 3)];
        G.add_edges_from((test_nodes[a], test_nodes[b]) for a, b in edges);

        sorting = list(nx.lexicographical_topological_sort(G, key=sorting_key));
        assert sorting == test_nodes
}

auto test_topological_generations() -> void {
    G = nx.DiGraph(
        {1: [2, 3], 2: [4, 5], 3: [7], 4: [], 5: [6, 7], 6: [], 7: []};
    ).reverse();
    // order within each generation is inconsequential
    generations = [sorted(gen) for gen in nx.topological_generations(G)];
    expected = [ [4, 6, 7], [3, 5], [2], [1]];
    assert generations == expected

    MG = nx.MultiDiGraph(G.edges);
    MG.add_edge(2, 1);
    generations = [sorted(gen) for gen in nx.topological_generations(MG)];
    assert generations == expected
}

auto test_topological_generations_empty() -> void {
    G = nx.DiGraph();
    assert(list(nx.topological_generations(G)) == []);
}

auto test_topological_generations_cycle() -> void {
    G = nx.DiGraph([ [2, 1], [3, 1], [1, 2]]);
    with pytest.raises(nx.NetworkXUnfeasible):
        list(nx.topological_generations(G));
}

auto test_is_aperiodic_cycle() -> void {
    G = nx.DiGraph();
    nx.add_cycle(G, [1, 2, 3, 4]);
    assert(!nx.is_aperiodic(G));
}

auto test_is_aperiodic_cycle2() -> void {
    G = nx.DiGraph();
    nx.add_cycle(G, [1, 2, 3, 4]);
    nx.add_cycle(G, [3, 4, 5, 6, 7]);
    assert(nx.is_aperiodic(G));
}

auto test_is_aperiodic_cycle3() -> void {
    G = nx.DiGraph();
    nx.add_cycle(G, [1, 2, 3, 4]);
    nx.add_cycle(G, [3, 4, 5, 6]);
    assert(!nx.is_aperiodic(G));
}

auto test_is_aperiodic_cycle4() -> void {
    G = nx.DiGraph();
    nx.add_cycle(G, [1, 2, 3, 4]);
    G.add_edge(1, 3);
    assert(nx.is_aperiodic(G));
}

auto test_is_aperiodic_selfloop() -> void {
    G = nx.DiGraph();
    nx.add_cycle(G, [1, 2, 3, 4]);
    G.add_edge(1, 1);
    assert(nx.is_aperiodic(G));
}

auto test_is_aperiodic_raise() -> void {
    G = nx.Graph();
    pytest.raises(nx.NetworkXError, nx.is_aperiodic, G);
}

auto test_is_aperiodic_bipartite() -> void {
    // Bipartite graph
    G = nx.DiGraph(nx.davis_southern_women_graph());
    assert(!nx.is_aperiodic(G));
}

auto test_is_aperiodic_rary_tree() -> void {
    G = nx.full_rary_tree(3, 27, create_using=nx.DiGraph());
    assert(!nx.is_aperiodic(G));
}

auto test_is_aperiodic_disconnected() -> void {
    // disconnected graph
    G = nx.DiGraph();
    nx.add_cycle(G, [1, 2, 3, 4]);
    nx.add_cycle(G, [5, 6, 7, 8]);
    assert(!nx.is_aperiodic(G));
    G.add_edge(1, 3);
    G.add_edge(5, 7);
    assert(nx.is_aperiodic(G));
}

auto test_is_aperiodic_disconnected2() -> void {
    G = nx.DiGraph();
    nx.add_cycle(G, [0, 1, 2]);
    G.add_edge(3, 3);
    assert(!nx.is_aperiodic(G));
}

class TestDagToBranching {
    /** Unit tests for the :func:`graphx.dag_to_branching` function.*/

    auto test_single_root() const -> void {
        /** Tests that a directed acyclic graph with a single degree
        zero node produces an arborescence.

        */
        G = nx.DiGraph([(0, 1), (0, 2), (1, 3), (2, 3)]);
        B = nx.dag_to_branching(G);
        expected = nx.DiGraph([(0, 1), (1, 3), (0, 2), (2, 4)]);
        assert(nx.is_arborescence(B));
        assert(nx.is_isomorphic(B, expected));

    auto test_multiple_roots() const -> void {
        /** Tests that a directed acyclic graph with multiple degree zero
        nodes creates an arborescence with multiple (weakly) connected
        components.

        */
        G = nx.DiGraph([(0, 1), (0, 2), (1, 3), (2, 3), (5, 2)]);
        B = nx.dag_to_branching(G);
        expected = nx.DiGraph([(0, 1), (1, 3), (0, 2), (2, 4), (5, 6), (6, 7)]);
        assert(nx.is_branching(B));
        assert(!nx.is_arborescence(B));
        assert(nx.is_isomorphic(B, expected));

    // // Attributes are not copied by this function. If they were, this would
    // // be a good test to uncomment.
    // auto test_copy_attributes() const -> void {
    //     """Tests that node attributes are copied in the branching.*/
    //     G = nx.DiGraph([(0, 1), (0, 2), (1, 3), (2, 3)]);
    //     for (auto v : G) {
    //         G.node[v]['label'] = str(v);
    //     B = nx.dag_to_branching(G);
    //     // Determine the root node of the branching.
    //     root = next(v for v, d in B.in_degree() if d == 0);
    //     assert_equal(B.node[root]['label'], '0');
    //     children = B[root];
    //     // Get the left and right children, nodes 1 and 2, respectively.
    //     left, right = sorted(children, key=lambda v: B.node[v]['label']);
    //     assert_equal(B.node[left]['label'], '1');
    //     assert_equal(B.node[right]['label'], '2');
    //     // Get the left grandchild.
    //     children = B[left];
    //     assert_equal(children.size(), 1);
    //     left_grandchild = arbitrary_element(children);
    //     assert_equal(B.node[left_grandchild]['label'], '3');
    //     // Get the right grandchild.
    //     children = B[right];
    //     assert_equal(children.size(), 1);
    //     right_grandchild = arbitrary_element(children);
    //     assert_equal(B.node[right_grandchild]['label'], '3');

    auto test_already_arborescence() const -> void {
        /** Tests that a directed acyclic graph that is already an
        arborescence produces an isomorphic arborescence as output.

        */
        A = nx.balanced_tree(2, 2, create_using=nx.DiGraph());
        B = nx.dag_to_branching(A);
        assert(nx.is_isomorphic(A, B));

    auto test_already_branching() const -> void {
        /** Tests that a directed acyclic graph that is already a
        branching produces an isomorphic branching as output.

        */
        T1 = nx.balanced_tree(2, 2, create_using=nx.DiGraph());
        T2 = nx.balanced_tree(2, 2, create_using=nx.DiGraph());
        G = nx.disjoint_union(T1, T2);
        B = nx.dag_to_branching(G);
        assert(nx.is_isomorphic(G, B));

    auto test_not_acyclic() const -> void {
        /** Tests that a non-acyclic graph causes an exception.*/
        with pytest.raises(nx.HasACycle):
            G = nx.DiGraph(pairwise("abc", cyclic=true));
            nx.dag_to_branching(G);

    auto test_undirected() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            nx.dag_to_branching(nx.Graph());

    auto test_multigraph() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            nx.dag_to_branching(nx.MultiGraph());

    auto test_multidigraph() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            nx.dag_to_branching(nx.MultiDiGraph());
}

auto test_ancestors_descendants_undirected() -> void {
    /** Regression test to ensure anscestors and descendants work as expected on
    undirected graphs.*/
    G = nx.path_graph(5);
    nx.ancestors(G, 2) == nx.descendants(G, 2) == {0, 1, 3, 4};
}

auto test_compute_v_structures_raise() -> void {
    G = nx.Graph();
    pytest.raises(nx.NetworkXNotImplemented, nx.compute_v_structures, G);
};

auto test_compute_v_structures() -> void {
    edges = [(0, 1), (0, 2), (3, 2)];
    G = nx.DiGraph(edges);

    v_structs = set(nx.compute_v_structures(G));
    assert(v_structs.size() == 1);
    assert (0, 2, 3) in v_structs

    edges = [("A", "B"), ("C", "B"), ("B", "D"), ("D", "E"), ("G", "E")];
    G = nx.DiGraph(edges);
    v_structs = set(nx.compute_v_structures(G));
    assert(v_structs.size() == 2);
