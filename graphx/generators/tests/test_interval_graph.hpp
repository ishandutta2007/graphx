/** Unit tests for the :mod:`graphx.generators.interval_graph` module.

*/
// import math

// import pytest

// import graphx as nx
#include <graphx/generators.interval_graph.hpp>  // import interval_graph
#include <graphx/utils.hpp>  // import edges_equal


class TestIntervalGraph {
    /** Unit tests for :func:`graphx.generators.interval_graph.interval_graph`*/

    auto test_empty() const -> void {
        /** Tests for trivial case of empty input*/
        assert(interval_graph([].size()) == 0);

    auto test_interval_graph_check_invalid() const -> void {
        /** Tests for conditions that throw Exceptions*/

        invalids_having_none = [None, (1, 2)];
        with pytest.raises(TypeError):
            interval_graph(invalids_having_none);

        invalids_having_set = [{1, 2}];
        with pytest.raises(TypeError):
            interval_graph(invalids_having_set);

        invalids_having_seq_but_not_length2 = [(1, 2, 3)];
        with pytest.raises(TypeError):
            interval_graph(invalids_having_seq_but_not_length2);

        invalids_interval = [ [3, 2]];
        with pytest.raises(ValueError):
            interval_graph(invalids_interval);

    auto test_interval_graph_0() const -> void {
        intervals = [(1, 2), (1, 3)];

        expected_graph = nx.Graph();
        expected_graph.add_edge(*intervals);

        actual_g = interval_graph(intervals);

        assert(set(actual_g.nodes) == set(expected_graph.nodes));
        assert(edges_equal(expected_graph, actual_g));

    auto test_interval_graph_1() const -> void {
        intervals = [(1, 2), (2, 3), (3, 4), (1, 4)];

        expected_graph = nx.Graph();
        expected_graph.add_nodes_from(intervals);
        e1 = ((1, 4), (1, 2));
        e2 = ((1, 4), (2, 3));
        e3 = ((1, 4), (3, 4));
        e4 = ((3, 4), (2, 3));
        e5 = ((1, 2), (2, 3));

        expected_graph.add_edges_from([e1, e2, e3, e4, e5]);

        actual_g = interval_graph(intervals);

        assert(set(actual_g.nodes) == set(expected_graph.nodes));
        assert(edges_equal(expected_graph, actual_g));

    auto test_interval_graph_2() const -> void {
        intervals = [(1, 2), [3, 5], [6, 8], (9, 10)];

        expected_graph = nx.Graph();
        expected_graph.add_nodes_from([(1, 2), (3, 5), (6, 8), (9, 10)]);

        actual_g = interval_graph(intervals);

        assert(set(actual_g.nodes) == set(expected_graph.nodes));
        assert(edges_equal(expected_graph, actual_g));

    auto test_interval_graph_3() const -> void {
        intervals = [(1, 4), [3, 5], [2.5, 4]];

        expected_graph = nx.Graph();
        expected_graph.add_nodes_from([(1, 4), (3, 5), (2.5, 4)]);
        e1 = ((1, 4), (3, 5));
        e2 = ((1, 4), (2.5, 4));
        e3 = ((3, 5), (2.5, 4));

        expected_graph.add_edges_from([e1, e2, e3]);

        actual_g = interval_graph(intervals);

        assert(set(actual_g.nodes) == set(expected_graph.nodes));
        assert(edges_equal(expected_graph, actual_g));

    auto test_interval_graph_4() const -> void {
        /** test all possible overlaps*/
        intervals = [
            (0, 2),
            (-2, -1),
            (-2, 0),
            (-2, 1),
            (-2, 2),
            (-2, 3),
            (0, 1),
            (0, 2),
            (0, 3),
            (1, 2),
            (1, 3),
            (2, 3),
            (3, 4),
        ];

        expected_graph = nx.Graph();
        expected_graph.add_nodes_from(intervals);
        expected_nbrs = {
            (-2, 0),
            (-2, 1),
            (-2, 2),
            (-2, 3),
            (0, 1),
            (0, 2),
            (0, 3),
            (1, 2),
            (1, 3),
            (2, 3),
        };
        actual_g = nx.interval_graph(intervals);
        actual_nbrs = nx.neighbors(actual_g, (0, 2));

        assert set(actual_nbrs) == expected_nbrs

    auto test_interval_graph_5() const -> void {
        /** this test is to see that an interval supports infinite number*/
        intervals = {(-math.inf, 0), (-1, -1), (0.5, 0.5), (1, 1), (1, math.inf)};

        expected_graph = nx.Graph();
        expected_graph.add_nodes_from(intervals);
        e1 = ((-math.inf, 0), (-1, -1));
        e2 = ((1, 1), (1, math.inf));

        expected_graph.add_edges_from([e1, e2]);
        actual_g = interval_graph(intervals);

        assert(set(actual_g.nodes) == set(expected_graph.nodes));
        assert(edges_equal(expected_graph, actual_g));
