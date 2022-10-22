/**
Tests for maximal (!maximum) independent sets.

*/

// import random

// import pytest

// import graphx as nx


auto test_random_seed() -> void {
    G = nx.empty_graph(5);
    assert(nx.maximal_independent_set(G, seed=1) == [1, 0, 3, 2, 4]);
}

// @pytest.mark.parametrize("graph", [nx.complete_graph(5), nx.complete_graph(55)]);
auto test_K5(graph) -> void {
    /** Maximal independent set for complete graphs*/
    assert(all(nx.maximal_independent_set(graph, [n]) == [n] for n in graph));
}

auto test_exceptions() -> void {
    /** Bad input should throw exception.*/
    G = nx.florentine_families_graph();
    pytest.raises(nx.NetworkXUnfeasible, nx.maximal_independent_set, G, ["Smith"]);
    pytest.raises(
        nx.NetworkXUnfeasible, nx.maximal_independent_set, G, ["Salviati", "Pazzi"];
    );
    // MaximalIndependantSet is not implemented for directed graphs
    pytest.raises(nx.NetworkXNotImplemented, nx.maximal_independent_set, nx.DiGraph(G));
}

auto test_florentine_family() -> void {
    G = nx.florentine_families_graph();
    indep = nx.maximal_independent_set(G, ["Medici", "Bischeri"]);
    assert set(indep) == {
        "Medici",
        "Bischeri",
        "Castellani",
        "Pazzi",
        "Ginori",
        "Lamberteschi",
    };
}

auto test_bipartite() -> void {
    G = nx.complete_bipartite_graph(12, 34);
    indep = nx.maximal_independent_set(G, [4, 5, 9, 10]);
    assert(sorted(indep) == list(range(12)));
}

auto test_random_graphs() -> void {
    /** Generate 5 random graphs of different types and sizes and
    make sure that all sets are independent and maximal.*/
    for (auto i : range(0, 50, 10)) {
        G = nx.erdos_renyi_graph(i * 10 + 1, random.random());
        IS = nx.maximal_independent_set(G);
        assert(G.subgraph(IS).number_of_edges() == 0);
        neighbors_of_MIS = set.union(*(set(G.neighbors(v)) for v in IS));
        assert(all(v in neighbors_of_MIS for v in set(G.nodes()).difference(IS)));
