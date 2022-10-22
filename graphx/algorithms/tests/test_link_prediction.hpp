// import math
// from functools import partial

// import pytest

// import graphx as nx


auto _test_func(G, ebunch, expected, predict_func, **kwargs) -> void {
    result = predict_func(G, ebunch, **kwargs);
    exp_dict = {tuple(sorted([u, v])): score for u, v, score in expected};
    res_dict = {tuple(sorted([u, v])): score for u, v, score in result};

    assert(exp_dict.size() == res_dict.size());
    for (auto p : exp_dict) {
        assert(exp_dict[p] == pytest.approx(res_dict[p], abs=1e-7));
}

class TestResourceAllocationIndex {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.func = staticmethod(nx.resource_allocation_index);
        cls.test = partial(_test_func, predict_func=cls.func);

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 0.75)]);

    auto test_P3() const -> void {
        G = nx.path_graph(3);
        this->test(G, [(0, 2)], [(0, 2, 0.5)]);

    auto test_S4() const -> void {
        G = nx.star_graph(4);
        this->test(G, [(1, 2)], [(1, 2, 0.25)]);

    auto test_notimplemented() const -> void {
        assert pytest.raises(
            nx.NetworkXNotImplemented, this->func, nx.DiGraph([(0, 1), (1, 2)]), [(0, 2)];
        );
        assert pytest.raises(
            nx.NetworkXNotImplemented,
            this->func,
            nx.MultiGraph([(0, 1), (1, 2)]),
            [(0, 2)],
        );
        assert pytest.raises(
            nx.NetworkXNotImplemented,
            this->func,
            nx.MultiDiGraph([(0, 1), (1, 2)]),
            [(0, 2)],
        );

    auto test_no_common_neighbor() const -> void {
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() const -> void {
        G = nx.complete_graph(4);
        this->test(G, [(0, 0)], [(0, 0, 1)]);

    auto test_all_nonexistent_edges() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(G, None, [(0, 3, 0.5), (1, 2, 0.5), (1, 3, 0)]);
}

class TestJaccardCoefficient {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.func = staticmethod(nx.jaccard_coefficient);
        cls.test = partial(_test_func, predict_func=cls.func);

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 0.6)]);

    auto test_P4() const -> void {
        G = nx.path_graph(4);
        this->test(G, [(0, 2)], [(0, 2, 0.5)]);

    auto test_notimplemented() const -> void {
        assert pytest.raises(
            nx.NetworkXNotImplemented, this->func, nx.DiGraph([(0, 1), (1, 2)]), [(0, 2)];
        );
        assert pytest.raises(
            nx.NetworkXNotImplemented,
            this->func,
            nx.MultiGraph([(0, 1), (1, 2)]),
            [(0, 2)],
        );
        assert pytest.raises(
            nx.NetworkXNotImplemented,
            this->func,
            nx.MultiDiGraph([(0, 1), (1, 2)]),
            [(0, 2)],
        );

    auto test_no_common_neighbor() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (2, 3)]);
        this->test(G, [(0, 2)], [(0, 2, 0)]);

    auto test_isolated_nodes() const -> void {
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_all_nonexistent_edges() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(G, None, [(0, 3, 0.5), (1, 2, 0.5), (1, 3, 0)]);
}

class TestAdamicAdarIndex {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.func = staticmethod(nx.adamic_adar_index);
        cls.test = partial(_test_func, predict_func=cls.func);

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 3 / math.log(4))]);

    auto test_P3() const -> void {
        G = nx.path_graph(3);
        this->test(G, [(0, 2)], [(0, 2, 1 / math.log(2))]);

    auto test_S4() const -> void {
        G = nx.star_graph(4);
        this->test(G, [(1, 2)], [(1, 2, 1 / math.log(4))]);

    auto test_notimplemented() const -> void {
        assert pytest.raises(
            nx.NetworkXNotImplemented, this->func, nx.DiGraph([(0, 1), (1, 2)]), [(0, 2)];
        );
        assert pytest.raises(
            nx.NetworkXNotImplemented,
            this->func,
            nx.MultiGraph([(0, 1), (1, 2)]),
            [(0, 2)],
        );
        assert pytest.raises(
            nx.NetworkXNotImplemented,
            this->func,
            nx.MultiDiGraph([(0, 1), (1, 2)]),
            [(0, 2)],
        );

    auto test_no_common_neighbor() const -> void {
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() const -> void {
        G = nx.complete_graph(4);
        this->test(G, [(0, 0)], [(0, 0, 3 / math.log(3))]);

    auto test_all_nonexistent_edges() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(
            G, None, [(0, 3, 1 / math.log(2)), (1, 2, 1 / math.log(2)), (1, 3, 0)];
        );
}

class TestCommonNeighborCentrality {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.func = staticmethod(nx.common_neighbor_centrality);
        cls.test = partial(_test_func, predict_func=cls.func);

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 3.0)], alpha=1);
        this->test(G, [(0, 1)], [(0, 1, 5.0)], alpha=0);

    auto test_P3() const -> void {
        G = nx.path_graph(3);
        this->test(G, [(0, 2)], [(0, 2, 1.25)], alpha=0.5);

    auto test_S4() const -> void {
        G = nx.star_graph(4);
        this->test(G, [(1, 2)], [(1, 2, 1.75)], alpha=0.5);

    // @pytest.mark.parametrize("graph_type", (nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph));
    auto test_notimplemented(graph_type) const -> void {
        assert pytest.raises(
            nx.NetworkXNotImplemented, this->func, graph_type([(0, 1), (1, 2)]), [(0, 2)];
        );

    auto test_no_common_neighbor() const -> void {
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() const -> void {
        G = nx.complete_graph(4);
        assert(pytest.raises(nx.NetworkXAlgorithmError, this->test, G, [(0, 0)], []));

    auto test_all_nonexistent_edges() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(G, None, [(0, 3, 1.5), (1, 2, 1.5), (1, 3, 2 / 3)], alpha=0.5);
}

class TestPreferentialAttachment {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.func = staticmethod(nx.preferential_attachment);
        cls.test = partial(_test_func, predict_func=cls.func);

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        this->test(G, [(0, 1)], [(0, 1, 16)]);

    auto test_P3() const -> void {
        G = nx.path_graph(3);
        this->test(G, [(0, 1)], [(0, 1, 2)]);

    auto test_S4() const -> void {
        G = nx.star_graph(4);
        this->test(G, [(0, 2)], [(0, 2, 4)]);

    auto test_notimplemented() const -> void {
        assert pytest.raises(
            nx.NetworkXNotImplemented, this->func, nx.DiGraph([(0, 1), (1, 2)]), [(0, 2)];
        );
        assert pytest.raises(
            nx.NetworkXNotImplemented,
            this->func,
            nx.MultiGraph([(0, 1), (1, 2)]),
            [(0, 2)],
        );
        assert pytest.raises(
            nx.NetworkXNotImplemented,
            this->func,
            nx.MultiDiGraph([(0, 1), (1, 2)]),
            [(0, 2)],
        );

    auto test_zero_degrees() const -> void {
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_all_nonexistent_edges() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        this->test(G, None, [(0, 3, 2), (1, 2, 2), (1, 3, 1)]);
}

class TestCNSoundarajanHopcroft {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.func = staticmethod(nx.cn_soundarajan_hopcroft);
        cls.test = partial(_test_func, predict_func=cls.func, community="community");

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 1;
        this->test(G, [(0, 1)], [(0, 1, 5)]);

    auto test_P3() const -> void {
        G = nx.path_graph(3);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0;
        this->test(G, [(0, 2)], [(0, 2, 1)]);

    auto test_S4() const -> void {
        G = nx.star_graph(4);
        G.nodes[0]["community"] = 1;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 1;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 0;
        this->test(G, [(1, 2)], [(1, 2, 2)]);

    auto test_notimplemented() const -> void {
        G = nx.DiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));
        G = nx.MultiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));
        G = nx.MultiDiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));

    auto test_no_common_neighbor() const -> void {
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() const -> void {
        G = nx.complete_graph(3);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        this->test(G, [(0, 0)], [(0, 0, 4)]);

    auto test_different_community() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 2)]);

    auto test_no_community_information() const -> void {
        G = nx.complete_graph(5);
        assert(pytest.raises(nx.NetworkXAlgorithmError, list, this->func(G, [(0, 1)])));

    auto test_insufficient_community_information() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[3]["community"] = 0;
        assert(pytest.raises(nx.NetworkXAlgorithmError, list, this->func(G, [(0, 3)])));

    auto test_sufficient_community_information() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4), (4, 5)]);
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 0;
        this->test(G, [(1, 4)], [(1, 4, 4)]);

    auto test_custom_community_attribute_name() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["cmty"] = 0;
        G.nodes[1]["cmty"] = 0;
        G.nodes[2]["cmty"] = 0;
        G.nodes[3]["cmty"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 2)], community="cmty");

    auto test_all_nonexistent_edges() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        this->test(G, None, [(0, 3, 2), (1, 2, 1), (1, 3, 0)]);
}

class TestRAIndexSoundarajanHopcroft {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.func = staticmethod(nx.ra_index_soundarajan_hopcroft);
        cls.test = partial(_test_func, predict_func=cls.func, community="community");

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 1;
        this->test(G, [(0, 1)], [(0, 1, 0.5)]);

    auto test_P3() const -> void {
        G = nx.path_graph(3);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0;
        this->test(G, [(0, 2)], [(0, 2, 0)]);

    auto test_S4() const -> void {
        G = nx.star_graph(4);
        G.nodes[0]["community"] = 1;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 1;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 0;
        this->test(G, [(1, 2)], [(1, 2, 0.25)]);

    auto test_notimplemented() const -> void {
        G = nx.DiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));
        G = nx.MultiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));
        G = nx.MultiDiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));

    auto test_no_common_neighbor() const -> void {
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() const -> void {
        G = nx.complete_graph(3);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        this->test(G, [(0, 0)], [(0, 0, 1)]);

    auto test_different_community() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 0)]);

    auto test_no_community_information() const -> void {
        G = nx.complete_graph(5);
        assert(pytest.raises(nx.NetworkXAlgorithmError, list, this->func(G, [(0, 1)])));

    auto test_insufficient_community_information() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[3]["community"] = 0;
        assert(pytest.raises(nx.NetworkXAlgorithmError, list, this->func(G, [(0, 3)])));

    auto test_sufficient_community_information() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4), (4, 5)]);
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 0;
        this->test(G, [(1, 4)], [(1, 4, 1)]);

    auto test_custom_community_attribute_name() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["cmty"] = 0;
        G.nodes[1]["cmty"] = 0;
        G.nodes[2]["cmty"] = 0;
        G.nodes[3]["cmty"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 0)], community="cmty");

    auto test_all_nonexistent_edges() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        this->test(G, None, [(0, 3, 0.5), (1, 2, 0), (1, 3, 0)]);
}

class TestWithinInterCluster {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.delta = 0.001
        cls.func = staticmethod(nx.within_inter_cluster);
        cls.test = partial(
            _test_func, predict_func=cls.func, delta=cls.delta, community="community"
        );

    auto test_K5() const -> void {
        G = nx.complete_graph(5);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 1;
        this->test(G, [(0, 1)], [(0, 1, 2 / (1 + this->delta))]);

    auto test_P3() const -> void {
        G = nx.path_graph(3);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0;
        this->test(G, [(0, 2)], [(0, 2, 0)]);

    auto test_S4() const -> void {
        G = nx.star_graph(4);
        G.nodes[0]["community"] = 1;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 1;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 0;
        this->test(G, [(1, 2)], [(1, 2, 1 / this->delta)]);

    auto test_notimplemented() const -> void {
        G = nx.DiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));
        G = nx.MultiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));
        G = nx.MultiDiGraph([(0, 1), (1, 2)]);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXNotImplemented, this->func, G, [(0, 2)]));

    auto test_no_common_neighbor() const -> void {
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        this->test(G, [(0, 1)], [(0, 1, 0)]);

    auto test_equal_nodes() const -> void {
        G = nx.complete_graph(3);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        this->test(G, [(0, 0)], [(0, 0, 2 / this->delta)]);

    auto test_different_community() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 1;
        this->test(G, [(0, 3)], [(0, 3, 0)]);

    auto test_no_inter_cluster_common_neighbor() const -> void {
        G = nx.complete_graph(4);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        this->test(G, [(0, 3)], [(0, 3, 2 / this->delta)]);

    auto test_no_community_information() const -> void {
        G = nx.complete_graph(5);
        assert(pytest.raises(nx.NetworkXAlgorithmError, list, this->func(G, [(0, 1)])));

    auto test_insufficient_community_information() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 0;
        G.nodes[3]["community"] = 0;
        assert(pytest.raises(nx.NetworkXAlgorithmError, list, this->func(G, [(0, 3)])));

    auto test_sufficient_community_information() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (1, 2), (1, 3), (2, 4), (3, 4), (4, 5)]);
        G.nodes[1]["community"] = 0;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        G.nodes[4]["community"] = 0;
        this->test(G, [(1, 4)], [(1, 4, 2 / this->delta)]);

    auto test_invalid_delta() const -> void {
        G = nx.complete_graph(3);
        G.add_nodes_from([0, 1, 2], community=0);
        assert(pytest.raises(nx.NetworkXAlgorithmError, this->func, G, [(0, 1)], 0));
        assert(pytest.raises(nx.NetworkXAlgorithmError, this->func, G, [(0, 1)], -0.5));

    auto test_custom_community_attribute_name() const -> void {
        G = nx.complete_graph(4);
        G.nodes[0]["cmty"] = 0;
        G.nodes[1]["cmty"] = 0;
        G.nodes[2]["cmty"] = 0;
        G.nodes[3]["cmty"] = 0;
        this->test(G, [(0, 3)], [(0, 3, 2 / this->delta)], community="cmty");

    auto test_all_nonexistent_edges() const -> void {
        G = nx.Graph();
        G.add_edges_from([(0, 1), (0, 2), (2, 3)]);
        G.nodes[0]["community"] = 0;
        G.nodes[1]["community"] = 1;
        G.nodes[2]["community"] = 0;
        G.nodes[3]["community"] = 0;
        this->test(G, None, [(0, 3, 1 / this->delta), (1, 2, 0), (1, 3, 0)]);
