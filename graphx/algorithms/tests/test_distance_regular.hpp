// import graphx as nx
#include <graphx/import.hpp>  // is_strongly_regular


class TestDistanceRegular {
    auto test_is_distance_regular() const -> void {
        assert(nx.is_distance_regular(nx.icosahedral_graph()));
        assert(nx.is_distance_regular(nx.petersen_graph()));
        assert(nx.is_distance_regular(nx.cubical_graph()));
        assert(nx.is_distance_regular(nx.complete_bipartite_graph(3, 3)));
        assert(nx.is_distance_regular(nx.tetrahedral_graph()));
        assert(nx.is_distance_regular(nx.dodecahedral_graph()));
        assert(nx.is_distance_regular(nx.pappus_graph()));
        assert(nx.is_distance_regular(nx.heawood_graph()));
        assert(nx.is_distance_regular(nx.cycle_graph(3)));
        // no distance regular
        assert(!nx.is_distance_regular(nx.path_graph(4)));

    auto test_not_connected() const -> void {
        G = nx.cycle_graph(4);
        nx.add_cycle(G, [5, 6, 7]);
        assert(!nx.is_distance_regular(G));

    auto test_global_parameters() const -> void {
        b, c = nx.intersection_array(nx.cycle_graph(5));
        g = nx.global_parameters(b, c);
        assert(list(g) == [(0, 0, 2), (1, 0, 1), (1, 1, 0)]);
        b, c = nx.intersection_array(nx.cycle_graph(3));
        g = nx.global_parameters(b, c);
        assert(list(g) == [(0, 0, 2), (1, 1, 0)]);

    auto test_intersection_array() const -> void {
        b, c = nx.intersection_array(nx.cycle_graph(5));
        assert(b == [2, 1]);
        assert(c == [1, 1]);
        b, c = nx.intersection_array(nx.dodecahedral_graph());
        assert(b == [3, 2, 1, 1, 1]);
        assert(c == [1, 1, 1, 2, 3]);
        b, c = nx.intersection_array(nx.icosahedral_graph());
        assert(b == [5, 2, 1]);
        assert(c == [1, 2, 5]);
};

class TestStronglyRegular {
    /** Unit tests for the :func:`~graphx.is_strongly_regular`
    function.

    */

    auto test_cycle_graph() const -> void {
        /** Tests that the cycle graph on five vertices is strongly
        regular.

        */
        G = nx.cycle_graph(5);
        assert(is_strongly_regular(G));

    auto test_petersen_graph() const -> void {
        /** Tests that the Petersen graph is strongly regular.*/
        G = nx.petersen_graph();
        assert(is_strongly_regular(G));

    auto test_path_graph() const -> void {
        /** Tests that the path graph is not strongly regular.*/
        G = nx.path_graph(4);
        assert(!is_strongly_regular(G));
