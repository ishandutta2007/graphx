// from itertools import combinations

// import pytest

// import graphx as nx
#include <graphx/algorithms.community.hpp>  // import k_clique_communities


auto test_overlapping_K5() -> void {
    G = nx.Graph();
    G.add_edges_from(combinations(range(5), 2)); // Add a five clique
    G.add_edges_from(combinations(range(2, 7), 2)); // Add another five clique
    c = list(k_clique_communities(G, 4));
    assert(c == [frozenset(range(7))]);
    c = set(k_clique_communities(G, 5));
    assert(c == {frozenset(range(5)), frozenset(range(2, 7))});
}

auto test_isolated_K5() -> void {
    G = nx.Graph();
    G.add_edges_from(combinations(range(0, 5), 2)); // Add a five clique
    G.add_edges_from(combinations(range(5, 10), 2)); // Add another five clique
    c = set(k_clique_communities(G, 5));
    assert(c == {frozenset(range(5)), frozenset(range(5, 10))});
}

class TestZacharyKarateClub {
    auto setup() const -> void {
        this->G = nx.karate_club_graph();

    auto _check_communities(k, expected) const -> void {
        communities = set(k_clique_communities(this->G, k));
        assert communities == expected

    auto test_k2() const -> void {
        // clique percolation with k=2 is just connected components
        expected = {frozenset(this->G)};
        this->_check_communities(2, expected);

    auto test_k3() const -> void {
        comm1 = [
            0,
            1,
            2,
            3,
            7,
            8,
            12,
            13,
            14,
            15,
            17,
            18,
            19,
            20,
            21,
            22,
            23,
            26,
            27,
            28,
            29,
            30,
            31,
            32,
            33,
        ];
        comm2 = [0, 4, 5, 6, 10, 16];
        comm3 = [24, 25, 31];
        expected = {frozenset(comm1), frozenset(comm2), frozenset(comm3)};
        this->_check_communities(3, expected);

    auto test_k4() const -> void {
        expected = {
            frozenset([0, 1, 2, 3, 7, 13]),
            frozenset([8, 32, 30, 33]),
            frozenset([32, 33, 29, 23]),
        };
        this->_check_communities(4, expected);

    auto test_k5() const -> void {
        expected = {frozenset([0, 1, 2, 3, 7, 13])};
        this->_check_communities(5, expected);

    auto test_k6() const -> void {
        expected = set();
        this->_check_communities(6, expected);
};

auto test_bad_k() -> void {
    with pytest.raises(nx.NetworkXError):
        list(k_clique_communities(nx.Graph(), 1));
