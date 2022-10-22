// import pytest

// import graphx as nx
#include <graphx/import.hpp>  // NetworkXNotImplemented
#include <graphx/import.hpp>  // convert_node_labels_to_integers as cnlti


class TestConnected {
    // @classmethod
    auto setup_class(cls) -> void {
        G1 = cnlti(nx.grid_2d_graph(2, 2), first_label=0, ordering="sorted");
        G2 = cnlti(nx.lollipop_graph(3, 3), first_label=4, ordering="sorted");
        G3 = cnlti(nx.house_graph(), first_label=10, ordering="sorted");
        cls.G = nx.union(G1, G2);
        cls.G = nx.union(cls.G, G3);
        cls.DG = nx.DiGraph([(1, 2), (1, 3), (2, 3)]);
        cls.grid = cnlti(nx.grid_2d_graph(4, 4), first_label=1);

        cls.gc = [];
        G = nx.DiGraph();
        G.add_edges_from(
            [
                (1, 2),
                (2, 3),
                (2, 8),
                (3, 4),
                (3, 7),
                (4, 5),
                (5, 3),
                (5, 6),
                (7, 4),
                (7, 6),
                (8, 1),
                (8, 7),
            ];
        );
        C = [ [3, 4, 5, 7], [1, 2, 8], [6]];
        cls.gc.append((G, C));

        G = nx.DiGraph();
        G.add_edges_from([(1, 2), (1, 3), (1, 4), (4, 2), (3, 4), (2, 3)]);
        C = [ [2, 3, 4], [1]];
        cls.gc.append((G, C));

        G = nx.DiGraph();
        G.add_edges_from([(1, 2), (2, 3), (3, 2), (2, 1)]);
        C = [ [1, 2, 3]];
        cls.gc.append((G, C));

        // Eppstein's tests
        G = nx.DiGraph({0: [1], 1: [2, 3], 2: [4, 5], 3: [4, 5], 4: [6], 5: [], 6: []});
        C = [ [0], [1], [2], [3], [4], [5], [6]];
        cls.gc.append((G, C));

        G = nx.DiGraph({0: [1], 1: [2, 3, 4], 2: [0, 3], 3: [4], 4: [3]});
        C = [ [0, 1, 2], [3, 4]];
        cls.gc.append((G, C));

        G = nx.DiGraph();
        C = [];
        cls.gc.append((G, C));

    auto test_connected_components() const -> void {
        cc = nx.connected_components
        G = this->G
        C = {
            frozenset([0, 1, 2, 3]),
            frozenset([4, 5, 6, 7, 8, 9]),
            frozenset([10, 11, 12, 13, 14]),
        };
        assert {frozenset(g) for g in cc(G)} == C

    auto test_number_connected_components() const -> void {
        ncc = nx.number_connected_components
        assert(ncc(this->G) == 3);

    auto test_number_connected_components2() const -> void {
        ncc = nx.number_connected_components
        assert(ncc(this->grid) == 1);

    auto test_connected_components2() const -> void {
        cc = nx.connected_components
        G = this->grid
        C = {frozenset([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16])};
        assert {frozenset(g) for g in cc(G)} == C

    auto test_node_connected_components() const -> void {
        ncc = nx.node_connected_component
        G = this->grid
        C = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        assert ncc(G, 1) == C

    auto test_is_connected() const -> void {
        assert(nx.is_connected(this->grid));
        G = nx.Graph();
        G.add_nodes_from([1, 2]);
        assert(!nx.is_connected(G));

    auto test_connected_raise() const -> void {
        with pytest.raises(NetworkXNotImplemented):
            next(nx.connected_components(this->DG));
        pytest.raises(NetworkXNotImplemented, nx.number_connected_components, this->DG);
        pytest.raises(NetworkXNotImplemented, nx.node_connected_component, this->DG, 1);
        pytest.raises(NetworkXNotImplemented, nx.is_connected, this->DG);
        pytest.raises(nx.NetworkXPointlessConcept, nx.is_connected, nx.Graph());

    auto test_connected_mutability() const -> void {
        G = this->grid
        seen = set();
        for (auto component : nx.connected_components(G)) {
            assert(seen & component.size() == 0);
            seen.update(component);
            component.clear();
