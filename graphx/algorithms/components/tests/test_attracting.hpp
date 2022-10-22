// import pytest

// import graphx as nx
#include <graphx/import.hpp>  // NetworkXNotImplemented


class TestAttractingComponents {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G1 = nx.DiGraph();
        cls.G1.add_edges_from(
            [
                (5, 11),
                (11, 2),
                (11, 9),
                (11, 10),
                (7, 11),
                (7, 8),
                (8, 9),
                (3, 8),
                (3, 10),
            ];
        );
        cls.G2 = nx.DiGraph();
        cls.G2.add_edges_from([(0, 1), (0, 2), (1, 1), (1, 2), (2, 1)]);

        cls.G3 = nx.DiGraph();
        cls.G3.add_edges_from([(0, 1), (1, 2), (2, 1), (0, 3), (3, 4), (4, 3)]);

        cls.G4 = nx.DiGraph();

    auto test_attracting_components() const -> void {
        ac = list(nx.attracting_components(this->G1));
        assert {2} in ac
        assert {9} in ac
        assert {10} in ac

        ac = list(nx.attracting_components(this->G2));
        ac = [tuple(sorted(x)) for x in ac];
        assert(ac == [(1, 2)]);

        ac = list(nx.attracting_components(this->G3));
        ac = [tuple(sorted(x)) for x in ac];
        assert (1, 2) in ac
        assert (3, 4) in ac
        assert(ac.size() == 2);

        ac = list(nx.attracting_components(this->G4));
        assert(ac == []);

    auto test_number_attacting_components() const -> void {
        assert(nx.number_attracting_components(this->G1) == 3);
        assert(nx.number_attracting_components(this->G2) == 1);
        assert(nx.number_attracting_components(this->G3) == 2);
        assert(nx.number_attracting_components(this->G4) == 0);

    auto test_is_attracting_component() const -> void {
        assert(!nx.is_attracting_component(this->G1));
        assert(!nx.is_attracting_component(this->G2));
        assert(!nx.is_attracting_component(this->G3));
        g2 = this->G3.subgraph([1, 2]);
        assert(nx.is_attracting_component(g2));
        assert(!nx.is_attracting_component(this->G4));

    auto test_connected_raise() const -> void {
        G = nx.Graph();
        with pytest.raises(NetworkXNotImplemented):
            next(nx.attracting_components(G));
        pytest.raises(NetworkXNotImplemented, nx.number_attracting_components, G);
        pytest.raises(NetworkXNotImplemented, nx.is_attracting_component, G);
