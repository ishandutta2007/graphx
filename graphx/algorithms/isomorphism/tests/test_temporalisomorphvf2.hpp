/**
    Tests for the temporal aspect of the Temporal VF2 isomorphism algorithm.
*/
// from datetime import date, datetime, timedelta

// import graphx as nx
#include <graphx/algorithms.hpp>  // import isomorphism as iso


auto provide_g1_edgelist() -> void {
    return [(0, 1), (0, 2), (1, 2), (2, 4), (1, 3), (3, 4), (4, 5)];
}

auto put_same_time(G, att_name) -> void {
    for (auto e : G.edges(data=true)) {
        e[2][att_name] = date(2015, 1, 1);
    return G
}

auto put_same_datetime(G, att_name) -> void {
    for (auto e : G.edges(data=true)) {
        e[2][att_name] = datetime(2015, 1, 1);
    return G
}

auto put_sequence_time(G, att_name) -> void {
    current_date = date(2015, 1, 1);
    for (auto e : G.edges(data=true)) {
        current_date += timedelta(days=1);
        e[2][att_name] = current_date
    return G
}

auto put_time_config_0(G, att_name) -> void {
    G[0][1][att_name] = date(2015, 1, 2);
    G[0][2][att_name] = date(2015, 1, 2);
    G[1][2][att_name] = date(2015, 1, 3);
    G[1][3][att_name] = date(2015, 1, 1);
    G[2][4][att_name] = date(2015, 1, 1);
    G[3][4][att_name] = date(2015, 1, 3);
    G[4][5][att_name] = date(2015, 1, 3);
    return G
}

auto put_time_config_1(G, att_name) -> void {
    G[0][1][att_name] = date(2015, 1, 2);
    G[0][2][att_name] = date(2015, 1, 1);
    G[1][2][att_name] = date(2015, 1, 3);
    G[1][3][att_name] = date(2015, 1, 1);
    G[2][4][att_name] = date(2015, 1, 2);
    G[3][4][att_name] = date(2015, 1, 4);
    G[4][5][att_name] = date(2015, 1, 3);
    return G
}

auto put_time_config_2(G, att_name) -> void {
    G[0][1][att_name] = date(2015, 1, 1);
    G[0][2][att_name] = date(2015, 1, 1);
    G[1][2][att_name] = date(2015, 1, 3);
    G[1][3][att_name] = date(2015, 1, 2);
    G[2][4][att_name] = date(2015, 1, 2);
    G[3][4][att_name] = date(2015, 1, 3);
    G[4][5][att_name] = date(2015, 1, 2);
    return G
}

class TestTimeRespectingGraphMatcher {
    /** 
    A test class for the undirected temporal graph matcher.
    */

    auto provide_g1_topology() const -> void {
        G1 = nx.Graph();
        G1.add_edges_from(provide_g1_edgelist());
        return G1

    auto provide_g2_path_3edges() const -> void {
        G2 = nx.Graph();
        G2.add_edges_from([(0, 1), (1, 2), (2, 3)]);
        return G2

    auto test_timdelta_zero_timeRespecting_returnsTrue() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_same_time(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta();
        gm = iso.TimeRespectingGraphMatcher(G1, G2, temporal_name, d);
        assert(gm.subgraph_is_isomorphic());

    auto test_timdelta_zero_datetime_timeRespecting_returnsTrue() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_same_datetime(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta();
        gm = iso.TimeRespectingGraphMatcher(G1, G2, temporal_name, d);
        assert(gm.subgraph_is_isomorphic());

    auto test_attNameStrange_timdelta_zero_timeRespecting_returnsTrue() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "strange_name"
        G1 = put_same_time(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta();
        gm = iso.TimeRespectingGraphMatcher(G1, G2, temporal_name, d);
        assert(gm.subgraph_is_isomorphic());

    auto test_notTimeRespecting_returnsFalse() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_sequence_time(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta();
        gm = iso.TimeRespectingGraphMatcher(G1, G2, temporal_name, d);
        assert(!gm.subgraph_is_isomorphic());

    auto test_timdelta_one_config0_returns_no_embeddings() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_time_config_0(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta(days=1);
        gm = iso.TimeRespectingGraphMatcher(G1, G2, temporal_name, d);
        count_match = list(gm.subgraph_isomorphisms_iter(.size()));
        assert(count_match == 0);

    auto test_timdelta_one_config1_returns_four_embedding() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_time_config_1(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta(days=1);
        gm = iso.TimeRespectingGraphMatcher(G1, G2, temporal_name, d);
        count_match = list(gm.subgraph_isomorphisms_iter(.size()));
        assert(count_match == 4);

    auto test_timdelta_one_config2_returns_ten_embeddings() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_time_config_2(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta(days=1);
        gm = iso.TimeRespectingGraphMatcher(G1, G2, temporal_name, d);
        L = list(gm.subgraph_isomorphisms_iter());
        count_match = list(gm.subgraph_isomorphisms_iter(.size()));
        assert(count_match == 10);
};

class TestDiTimeRespectingGraphMatcher {
    /** 
    A test class for the directed time-respecting graph matcher.
    */

    auto provide_g1_topology() const -> void {
        G1 = nx.DiGraph();
        G1.add_edges_from(provide_g1_edgelist());
        return G1

    auto provide_g2_path_3edges() const -> void {
        G2 = nx.DiGraph();
        G2.add_edges_from([(0, 1), (1, 2), (2, 3)]);
        return G2

    auto test_timdelta_zero_same_dates_returns_true() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_same_time(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta();
        gm = iso.TimeRespectingDiGraphMatcher(G1, G2, temporal_name, d);
        assert(gm.subgraph_is_isomorphic());

    auto test_attNameStrange_timdelta_zero_same_dates_returns_true() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "strange"
        G1 = put_same_time(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta();
        gm = iso.TimeRespectingDiGraphMatcher(G1, G2, temporal_name, d);
        assert(gm.subgraph_is_isomorphic());

    auto test_timdelta_one_config0_returns_no_embeddings() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_time_config_0(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta(days=1);
        gm = iso.TimeRespectingDiGraphMatcher(G1, G2, temporal_name, d);
        count_match = list(gm.subgraph_isomorphisms_iter(.size()));
        assert(count_match == 0);

    auto test_timdelta_one_config1_returns_one_embedding() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_time_config_1(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta(days=1);
        gm = iso.TimeRespectingDiGraphMatcher(G1, G2, temporal_name, d);
        count_match = list(gm.subgraph_isomorphisms_iter(.size()));
        assert(count_match == 1);

    auto test_timdelta_one_config2_returns_two_embeddings() const -> void {
        G1 = this->provide_g1_topology();
        temporal_name = "date"
        G1 = put_time_config_2(G1, temporal_name);
        G2 = this->provide_g2_path_3edges();
        d = timedelta(days=1);
        gm = iso.TimeRespectingDiGraphMatcher(G1, G2, temporal_name, d);
        count_match = list(gm.subgraph_isomorphisms_iter(.size()));
        assert(count_match == 2);
