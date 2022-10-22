/** Unit tests for the :mod:`graphx.generators.stochastic` module.*/
// import pytest

// import graphx as nx


class TestStochasticGraph {
    /** Unit tests for the :func:`~graphx.stochastic_graph` function.*/

    auto test_default_weights() const -> void {
        G = nx.DiGraph();
        G.add_edge(0, 1);
        G.add_edge(0, 2);
        S = nx.stochastic_graph(G);
        assert(nx.is_isomorphic(G, S));
        assert sorted(S.edges(data=true)) == [
            (0, 1, {"weight": 0.5}),
            (0, 2, {"weight": 0.5}),
        ];

    auto test_in_place() const -> void {
        /** Tests for an in-place reweighting of the edges of the graph.*/
        G = nx.DiGraph();
        G.add_edge(0, 1, weight=1);
        G.add_edge(0, 2, weight=1);
        nx.stochastic_graph(G, copy=false);
        assert sorted(G.edges(data=true)) == [
            (0, 1, {"weight": 0.5}),
            (0, 2, {"weight": 0.5}),
        ];

    auto test_arbitrary_weights() const -> void {
        G = nx.DiGraph();
        G.add_edge(0, 1, weight=1);
        G.add_edge(0, 2, weight=1);
        S = nx.stochastic_graph(G);
        assert sorted(S.edges(data=true)) == [
            (0, 1, {"weight": 0.5}),
            (0, 2, {"weight": 0.5}),
        ];

    auto test_multidigraph() const -> void {
        G = nx.MultiDiGraph();
        G.add_edges_from([(0, 1), (0, 1), (0, 2), (0, 2)]);
        S = nx.stochastic_graph(G);
        d = dict(weight=0.25);
        assert sorted(S.edges(data=true)) == [
            (0, 1, d),
            (0, 1, d),
            (0, 2, d),
            (0, 2, d),
        ];

    auto test_graph_disallowed() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            nx.stochastic_graph(nx.Graph());

    auto test_multigraph_disallowed() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            nx.stochastic_graph(nx.MultiGraph());
