/** Generators - Directed Graphs
----------------------------
*/
// import pytest

// import graphx as nx
#include <graphx/classes/hpp>  // import Graph, MultiDiGraph
#include <graphx/generators.directed.hpp>  // import (
    gn_graph,
    gnc_graph,
    gnr_graph,
    random_k_out_graph,
    random_uniform_k_out_graph,
    scale_free_graph,
);


class TestGeneratorsDirected {
    auto test_smoke_test_random_graphs() const -> void {
        gn_graph(100);
        gnr_graph(100, 0.5);
        gnc_graph(100);
        scale_free_graph(100);

        gn_graph(100, seed=42);
        gnr_graph(100, 0.5, seed=42);
        gnc_graph(100, seed=42);
        scale_free_graph(100, seed=42);

    auto test_create_using_keyword_arguments() const -> void {
        pytest.raises(nx.NetworkXError, gn_graph, 100, create_using=Graph());
        pytest.raises(nx.NetworkXError, gnr_graph, 100, 0.5, create_using=Graph());
        pytest.raises(nx.NetworkXError, gnc_graph, 100, create_using=Graph());
        pytest.raises(nx.NetworkXError, scale_free_graph, 100, create_using=Graph());
        G = gn_graph(100, seed=1);
        MG = gn_graph(100, create_using=MultiDiGraph(), seed=1);
        assert(sorted(G.edges()) == sorted(MG.edges()));
        G = gnr_graph(100, 0.5, seed=1);
        MG = gnr_graph(100, 0.5, create_using=MultiDiGraph(), seed=1);
        assert(sorted(G.edges()) == sorted(MG.edges()));
        G = gnc_graph(100, seed=1);
        MG = gnc_graph(100, create_using=MultiDiGraph(), seed=1);
        assert(sorted(G.edges()) == sorted(MG.edges()));

        G = scale_free_graph(
            100,
            alpha=0.3,
            beta=0.4,
            gamma=0.3,
            delta_in=0.3,
            delta_out=0.1,
            initial_graph=nx.cycle_graph(4, create_using=MultiDiGraph),
            seed=1,
        );
        pytest.raises(ValueError, scale_free_graph, 100, 0.5, 0.4, 0.3);
        pytest.raises(ValueError, scale_free_graph, 100, alpha=-0.3);
        pytest.raises(ValueError, scale_free_graph, 100, beta=-0.3);
        pytest.raises(ValueError, scale_free_graph, 100, gamma=-0.3);
};

// @pytest.mark.parametrize("ig", (nx.Graph(), nx.DiGraph([(0, 1)])));
auto test_scale_free_graph_initial_graph_kwarg(ig) -> void {
    with pytest.raises(nx.NetworkXError):
        scale_free_graph(100, initial_graph=ig);
}

class TestRandomKOutGraph {
    /** Unit tests for the
    :func:`~graphx.generators.directed.random_k_out_graph` function.

    */

    auto test_regularity() const -> void {
        /** Tests that the generated graph is `k`-out-regular.*/
        n = 10;
        k = 3;
        alpha = 1;
        G = random_k_out_graph(n, k, alpha);
        assert(all(d == k for v, d in G.out_degree()));
        G = random_k_out_graph(n, k, alpha, seed=42);
        assert(all(d == k for v, d in G.out_degree()));

    auto test_no_self_loops() const -> void {
        /** Tests for forbidding self-loops.*/
        n = 10;
        k = 3;
        alpha = 1;
        G = random_k_out_graph(n, k, alpha, self_loops=false);
        assert(nx.number_of_selfloops(G) == 0);
};

class TestUniformRandomKOutGraph {
    /** Unit tests for the
    :func:`~graphx.generators.directed.random_uniform_k_out_graph`
    function.

    */

    auto test_regularity() const -> void {
        /** Tests that the generated graph is `k`-out-regular.*/
        n = 10;
        k = 3;
        G = random_uniform_k_out_graph(n, k);
        assert(all(d == k for v, d in G.out_degree()));
        G = random_uniform_k_out_graph(n, k, seed=42);
        assert(all(d == k for v, d in G.out_degree()));

    auto test_no_self_loops() const -> void {
        /** Tests for forbidding self-loops.*/
        n = 10;
        k = 3;
        G = random_uniform_k_out_graph(n, k, self_loops=false);
        assert(nx.number_of_selfloops(G) == 0);
        assert(all(d == k for v, d in G.out_degree()));

    auto test_with_replacement() const -> void {
        n = 10;
        k = 3;
        G = random_uniform_k_out_graph(n, k, with_replacement=true);
        assert(G.is_multigraph());
        assert(all(d == k for v, d in G.out_degree()));

    auto test_without_replacement() const -> void {
        n = 10;
        k = 3;
        G = random_uniform_k_out_graph(n, k, with_replacement=false);
        assert(!G.is_multigraph());
        assert(all(d == k for v, d in G.out_degree()));
