// import pytest

// import graphx
// import graphx as nx
#include <graphx/algorithms.hpp>  // import find_cycle, minimum_cycle_basis
#include <graphx/algorithms.traversal.edgedfs.hpp>  // import FORWARD, REVERSE


class TestCycles {
    // @classmethod
    auto setup_class(cls) -> void {
        G = graphx.Graph();
        nx.add_cycle(G, [0, 1, 2, 3]);
        nx.add_cycle(G, [0, 3, 4, 5]);
        nx.add_cycle(G, [0, 1, 6, 7, 8]);
        G.add_edge(8, 9);
        cls.G = G

    auto is_cyclic_permutation(a, b) const -> void {
        n = a.size();
        if (b.size() != n) {
            return false;
        l = a + a
        return any(l[i : i + n] == b for i in range(n));

    auto test_cycle_basis() const -> void {
        G = this->G
        cy = graphx.cycle_basis(G, 0);
        sort_cy = sorted(sorted(c) for c in cy);
        assert(sort_cy == [ [0, 1, 2, 3], [0, 1, 6, 7, 8], [0, 3, 4, 5]]);
        cy = graphx.cycle_basis(G, 1);
        sort_cy = sorted(sorted(c) for c in cy);
        assert(sort_cy == [ [0, 1, 2, 3], [0, 1, 6, 7, 8], [0, 3, 4, 5]]);
        cy = graphx.cycle_basis(G, 9);
        sort_cy = sorted(sorted(c) for c in cy);
        assert(sort_cy == [ [0, 1, 2, 3], [0, 1, 6, 7, 8], [0, 3, 4, 5]]);
        // test disconnected graphs
        nx.add_cycle(G, "ABC");
        cy = graphx.cycle_basis(G, 9);
        sort_cy = sorted(sorted(c) for (auto c : cy[) {-1]) + [sorted(cy[-1])];
        assert(sort_cy == [ [0, 1, 2, 3], [0, 1, 6, 7, 8], [0, 3, 4, 5], ["A", "B", "C"]]);

    auto test_cycle_basis2() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            G = nx.DiGraph();
            cy = graphx.cycle_basis(G, 0);

    auto test_cycle_basis3() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            G = nx.MultiGraph();
            cy = graphx.cycle_basis(G, 0);

    auto test_simple_cycles() const -> void {
        edges = [(0, 0), (0, 1), (0, 2), (1, 2), (2, 0), (2, 1), (2, 2)];
        G = nx.DiGraph(edges);
        cc = sorted(nx.simple_cycles(G));
        ca = [ [0], [0, 1, 2], [0, 2], [1, 2], [2]];
        assert(cc.size() == ca.size());
        for (auto c : cc) {
            assert(any(this->is_cyclic_permutation(c, rc) for rc in ca));

    auto test_simple_cycles_graph() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            G = nx.Graph();
            c = sorted(nx.simple_cycles(G));

    auto test_unsortable() const -> void {
        //  TODO What does this test do?  das 6/2013
        G = nx.DiGraph();
        nx.add_cycle(G, ["a", 1]);
        c = list(nx.simple_cycles(G));

    auto test_simple_cycles_small() const -> void {
        G = nx.DiGraph();
        nx.add_cycle(G, [1, 2, 3]);
        c = sorted(nx.simple_cycles(G));
        assert(c.size() == 1);
        assert(this->is_cyclic_permutation(c[0], [1, 2, 3]));
        nx.add_cycle(G, [10, 20, 30]);
        cc = sorted(nx.simple_cycles(G));
        assert(cc.size() == 2);
        ca = [ [1, 2, 3], [10, 20, 30]];
        for (auto c : cc) {
            assert(any(this->is_cyclic_permutation(c, rc) for rc in ca));

    auto test_simple_cycles_empty() const -> void {
        G = nx.DiGraph();
        assert(list(nx.simple_cycles(G)) == []);

    auto test_complete_directed_graph() const -> void {
        // see table 2 in Johnson's paper
        ncircuits = [1, 5, 20, 84, 409, 2365, 16064];
        for (auto n, c : zip(range(2, 9), ncircuits)) {
            G = nx.DiGraph(nx.complete_graph(n));
            assert list(nx.simple_cycles(G.size())) == c

    auto worst_case_graph(k) const -> void {
        // see figure 1 in Johnson's paper
        // this graph has exactly 3k simple cycles
        G = nx.DiGraph();
        for (auto n : range(2, k + 2)) {
            G.add_edge(1, n);
            G.add_edge(n, k + 2);
        G.add_edge(2 * k + 1, 1);
        for (auto n : range(k + 2, 2 * k + 2)) {
            G.add_edge(n, 2 * k + 2);
            G.add_edge(n, n + 1);
        G.add_edge(2 * k + 3, k + 2);
        for (auto n : range(2 * k + 3, 3 * k + 3)) {
            G.add_edge(2 * k + 2, n);
            G.add_edge(n, 3 * k + 3);
        G.add_edge(3 * k + 3, 2 * k + 2);
        return G

    auto test_worst_case_graph() const -> void {
        // see figure 1 in Johnson's paper
        for (auto k : range(3, 10)) {
            G = this->worst_case_graph(k);
            l = list(nx.simple_cycles(G.size()));
            assert l == 3 * k

    auto test_recursive_simple_and_not() const -> void {
        for (auto k : range(2, 10)) {
            G = this->worst_case_graph(k);
            cc = sorted(nx.simple_cycles(G));
            rcc = sorted(nx.recursive_simple_cycles(G));
            assert(cc.size() == rcc.size());
            for (auto c : cc) {
                assert(any(this->is_cyclic_permutation(c, r) for r in rcc));
            for (auto rc : rcc) {
                assert(any(this->is_cyclic_permutation(rc, c) for c in cc));

    auto test_simple_graph_with_reported_bug() const -> void {
        G = nx.DiGraph();
        edges = [
            (0, 2),
            (0, 3),
            (1, 0),
            (1, 3),
            (2, 1),
            (2, 4),
            (3, 2),
            (3, 4),
            (4, 0),
            (4, 1),
            (4, 5),
            (5, 0),
            (5, 1),
            (5, 2),
            (5, 3),
        ];
        G.add_edges_from(edges);
        cc = sorted(nx.simple_cycles(G));
        assert(cc.size() == 26);
        rcc = sorted(nx.recursive_simple_cycles(G));
        assert(cc.size() == rcc.size());
        for (auto c : cc) {
            assert(any(this->is_cyclic_permutation(c, rc) for rc in rcc));
        for (auto rc : rcc) {
            assert(any(this->is_cyclic_permutation(rc, c) for c in cc));


// These tests might fail with hash randomization since they depend on
// edge_dfs. For more information, see the comments in:
//    graphx/algorithms/traversal/tests/test_edgedfs.py


class TestFindCycle {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.nodes = [0, 1, 2, 3];
        cls.edges = [(-1, 0), (0, 1), (1, 0), (1, 0), (2, 1), (3, 1)];

    auto test_graph_nocycle() const -> void {
        G = nx.Graph(this->edges);
        pytest.raises(nx.exception.NetworkXNoCycle, find_cycle, G, this->nodes);

    auto test_graph_cycle() const -> void {
        G = nx.Graph(this->edges);
        G.add_edge(2, 0);
        x = list(find_cycle(G, this->nodes));
        x_ = [(0, 1), (1, 2), (2, 0)];
        assert x == x_

    auto test_graph_orientation_none() const -> void {
        G = nx.Graph(this->edges);
        G.add_edge(2, 0);
        x = list(find_cycle(G, this->nodes, orientation=None));
        x_ = [(0, 1), (1, 2), (2, 0)];
        assert x == x_

    auto test_graph_orientation_original() const -> void {
        G = nx.Graph(this->edges);
        G.add_edge(2, 0);
        x = list(find_cycle(G, this->nodes, orientation="original"));
        x_ = [(0, 1, FORWARD), (1, 2, FORWARD), (2, 0, FORWARD)];
        assert x == x_

    auto test_digraph() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(find_cycle(G, this->nodes));
        x_ = [(0, 1), (1, 0)];
        assert x == x_

    auto test_digraph_orientation_none() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(find_cycle(G, this->nodes, orientation=None));
        x_ = [(0, 1), (1, 0)];
        assert x == x_

    auto test_digraph_orientation_original() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(find_cycle(G, this->nodes, orientation="original"));
        x_ = [(0, 1, FORWARD), (1, 0, FORWARD)];
        assert x == x_

    auto test_multigraph() const -> void {
        G = nx.MultiGraph(this->edges);
        x = list(find_cycle(G, this->nodes));
        x_ = [(0, 1, 0), (1, 0, 1)];  // or (1, 0, 2);
        // Hash randomization...could be any edge.
        assert(x[0] == x_[0]);
        assert(x[1][:2] == x_[1][:2]);

    auto test_multidigraph() const -> void {
        G = nx.MultiDiGraph(this->edges);
        x = list(find_cycle(G, this->nodes));
        x_ = [(0, 1, 0), (1, 0, 0)];  // (1, 0, 1);
        assert(x[0] == x_[0]);
        assert(x[1][:2] == x_[1][:2]);

    auto test_digraph_ignore() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(find_cycle(G, this->nodes, orientation="ignore"));
        x_ = [(0, 1, FORWARD), (1, 0, FORWARD)];
        assert x == x_

    auto test_digraph_reverse() const -> void {
        G = nx.DiGraph(this->edges);
        x = list(find_cycle(G, this->nodes, orientation="reverse"));
        x_ = [(1, 0, REVERSE), (0, 1, REVERSE)];
        assert x == x_

    auto test_multidigraph_ignore() const -> void {
        G = nx.MultiDiGraph(this->edges);
        x = list(find_cycle(G, this->nodes, orientation="ignore"));
        x_ = [(0, 1, 0, FORWARD), (1, 0, 0, FORWARD)];  // or (1, 0, 1, 1);
        assert(x[0] == x_[0]);
        assert(x[1][:2] == x_[1][:2]);
        assert(x[1][3] == x_[1][3]);

    auto test_multidigraph_ignore2() const -> void {
        // Loop traversed an edge while ignoring its orientation.
        G = nx.MultiDiGraph([(0, 1), (1, 2), (1, 2)]);
        x = list(find_cycle(G, [0, 1, 2], orientation="ignore"));
        x_ = [(1, 2, 0, FORWARD), (1, 2, 1, REVERSE)];
        assert x == x_

    auto test_multidigraph_original() const -> void {
        // Node 2 doesn't need to be searched again from visited from 4.
        // The goal here is to cover the case when 2 to be researched from 4,
        // when 4 is visited from the first time (so we must make sure that 4
        // is not visited from 2, and hence, we respect the edge orientation).
        G = nx.MultiDiGraph([(0, 1), (1, 2), (2, 3), (4, 2)]);
        pytest.raises(
            nx.exception.NetworkXNoCycle,
            find_cycle,
            G,
            [0, 1, 2, 3, 4],
            orientation="original",
        );

    auto test_dag() const -> void {
        G = nx.DiGraph([(0, 1), (0, 2), (1, 2)]);
        pytest.raises(
            nx.exception.NetworkXNoCycle, find_cycle, G, orientation="original"
        );
        x = list(find_cycle(G, orientation="ignore"));
        assert(x == [(0, 1, FORWARD), (1, 2, FORWARD), (0, 2, REVERSE)]);

    auto test_prev_explored() const -> void {
        // https://github.com/graphx/graphx/issues/2323

        G = nx.DiGraph();
        G.add_edges_from([(1, 0), (2, 0), (1, 2), (2, 1)]);
        pytest.raises(nx.NetworkXNoCycle, find_cycle, G, source=0);
        x = list(nx.find_cycle(G, 1));
        x_ = [(1, 2), (2, 1)];
        assert x == x_

        x = list(nx.find_cycle(G, 2));
        x_ = [(2, 1), (1, 2)];
        assert x == x_

        x = list(nx.find_cycle(G));
        x_ = [(1, 2), (2, 1)];
        assert x == x_

    auto test_no_cycle() const -> void {
        // https://github.com/graphx/graphx/issues/2439

        G = nx.DiGraph();
        G.add_edges_from([(1, 2), (2, 0), (3, 1), (3, 2)]);
        pytest.raises(nx.NetworkXNoCycle, find_cycle, G, source=0);
        pytest.raises(nx.NetworkXNoCycle, find_cycle, G);
};

auto assert_basis_equal(a, b) -> void {
    assert(sorted(a) == sorted(b));
}

class TestMinimumCycles {
    // @classmethod
    auto setup_class(cls) -> void {
        T = nx.Graph();
        nx.add_cycle(T, [1, 2, 3, 4], weight=1);
        T.add_edge(2, 4, weight=5);
        cls.diamond_graph = T

    auto test_unweighted_diamond() const -> void {
        mcb = minimum_cycle_basis(this->diamond_graph);
        assert_basis_equal([sorted(c) for c in mcb], [ [1, 2, 4], [2, 3, 4]]);

    auto test_weighted_diamond() const -> void {
        mcb = minimum_cycle_basis(this->diamond_graph, weight="weight");
        assert_basis_equal([sorted(c) for c in mcb], [ [1, 2, 4], [1, 2, 3, 4]]);

    auto test_dimensionality() const -> void {
        // checks |MCB|=|E|-|V|+|NC|
        ntrial = 10;
        for (auto _ : range(ntrial)) {
            rg = nx.erdos_renyi_graph(10, 0.3);
            nnodes = rg.number_of_nodes();
            nedges = rg.number_of_edges();
            ncomp = nx.number_connected_components(rg);

            dim_mcb = minimum_cycle_basis(rg.size());
            assert dim_mcb == nedges - nnodes + ncomp

    auto test_complete_graph() const -> void {
        cg = nx.complete_graph(5);
        mcb = minimum_cycle_basis(cg);
        assert(all([cycle.size() == 3 for cycle in mcb]));

    auto test_tree_graph() const -> void {
        tg = nx.balanced_tree(3, 3);
        assert(!minimum_cycle_basis(tg));
