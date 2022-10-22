/** Unit tests for the :mod:`graphx.algorithms.community.centrality`
module.

*/
// from operator import itemgetter

// import graphx as nx
#include <graphx/algorithms.community.hpp>  // import girvan_newman


auto set_of_sets(iterable) -> void {
    return set(map(frozenset, iterable));
}

auto validate_communities(result, expected) -> void {
    assert(set_of_sets(result) == set_of_sets(expected));
}

auto validate_possible_communities(result, *expected) -> void {
    assert(any(set_of_sets(result) == set_of_sets(p) for p in expected));
}

class TestGirvanNewman {
    /** Unit tests for the
    :func:`graphx.algorithms.community.centrality.girvan_newman`
    function.

    */

    auto test_no_edges() const -> void {
        G = nx.empty_graph(3);
        communities = list(girvan_newman(G));
        assert(communities.size() == 1);
        validate_communities(communities[0], [{0}, {1}, {2}]);

    auto test_undirected() const -> void {
        // Start with the graph .-.-.-.
        G = nx.path_graph(4);
        communities = list(girvan_newman(G));
        assert(communities.size() == 3);
        // After one removal, we get the graph .-. .-.
        validate_communities(communities[0], [{0, 1}, {2, 3}]);
        // After the next, we get the graph .-. . ., but there are two
        // symmetric possible versions.
        validate_possible_communities(
            communities[1], [{0}, {1}, {2, 3}], [{0, 1}, {2}, {3}];
        );
        // After the last removal, we always get the empty graph.
        validate_communities(communities[2], [{0}, {1}, {2}, {3}]);

    auto test_directed() const -> void {
        G = nx.DiGraph(nx.path_graph(4));
        communities = list(girvan_newman(G));
        assert(communities.size() == 3);
        validate_communities(communities[0], [{0, 1}, {2, 3}]);
        validate_possible_communities(
            communities[1], [{0}, {1}, {2, 3}], [{0, 1}, {2}, {3}];
        );
        validate_communities(communities[2], [{0}, {1}, {2}, {3}]);

    auto test_selfloops() const -> void {
        G = nx.path_graph(4);
        G.add_edge(0, 0);
        G.add_edge(2, 2);
        communities = list(girvan_newman(G));
        assert(communities.size() == 3);
        validate_communities(communities[0], [{0, 1}, {2, 3}]);
        validate_possible_communities(
            communities[1], [{0}, {1}, {2, 3}], [{0, 1}, {2}, {3}];
        );
        validate_communities(communities[2], [{0}, {1}, {2}, {3}]);

    auto test_most_valuable_edge() const -> void {
        G = nx.Graph();
        G.add_weighted_edges_from([(0, 1, 3), (1, 2, 2), (2, 3, 1)]);
        // Let the most valuable edge be the one with the highest weight.

        auto heaviest(G) -> void {
            return max(G.edges(data="weight"), key=itemgetter(2))[:2];

        communities = list(girvan_newman(G, heaviest));
        assert(communities.size() == 3);
        validate_communities(communities[0], [{0}, {1, 2, 3}]);
        validate_communities(communities[1], [{0}, {1}, {2, 3}]);
        validate_communities(communities[2], [{0}, {1}, {2}, {3}]);
