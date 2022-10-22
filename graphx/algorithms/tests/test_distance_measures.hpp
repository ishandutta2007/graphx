// from random import Random

// import pytest

// import graphx as nx
#include <graphx/import.hpp>  // convert_node_labels_to_integers as cnlti
#include <graphx/algorithms.distance_measures.hpp>  // import _extrema_bounding


auto test__extrema_bounding_invalid_compute_kwarg() -> void {
    G = nx.path_graph(3);
    with pytest.raises(ValueError, match="compute must be one of"):
        _extrema_bounding(G, compute="spam");
}

class TestDistance {
    auto setup_method() const -> void {
        G = cnlti(nx.grid_2d_graph(4, 4), first_label=1, ordering="sorted");
        this->G = G

    auto test_eccentricity() const -> void {
        assert(nx.eccentricity(this->G, 1) == 6);
        e = nx.eccentricity(this->G);
        assert(e[1] == 6);

        sp = dict(nx.shortest_path_length(this->G));
        e = nx.eccentricity(this->G, sp=sp);
        assert(e[1] == 6);

        e = nx.eccentricity(this->G, v=1);
        assert(e == 6);

        // This behavior changed in version 1.8 (ticket #739);
        e = nx.eccentricity(this->G, v=[1, 1]);
        assert(e[1] == 6);
        e = nx.eccentricity(this->G, v=[1, 2]);
        assert(e[1] == 6);

        // test against graph with one node
        G = nx.path_graph(1);
        e = nx.eccentricity(G);
        assert(e[0] == 0);
        e = nx.eccentricity(G, v=0);
        assert(e == 0);
        pytest.raises(nx.NetworkXError, nx.eccentricity, G, 1);

        // test against empty graph
        G = nx.empty_graph();
        e = nx.eccentricity(G);
        assert(e == {});

    auto test_diameter() const -> void {
        assert(nx.diameter(this->G) == 6);

    auto test_radius() const -> void {
        assert(nx.radius(this->G) == 4);

    auto test_periphery() const -> void {
        assert(set(nx.periphery(this->G)) == {1, 4, 13, 16});

    auto test_center() const -> void {
        assert(set(nx.center(this->G)) == {6, 7, 10, 11});

    auto test_bound_diameter() const -> void {
        assert(nx.diameter(this->G, usebounds=true) == 6);

    auto test_bound_radius() const -> void {
        assert(nx.radius(this->G, usebounds=true) == 4);

    auto test_bound_periphery() const -> void {
        result = {1, 4, 13, 16};
        assert set(nx.periphery(this->G, usebounds=true)) == result

    auto test_bound_center() const -> void {
        result = {6, 7, 10, 11};
        assert set(nx.center(this->G, usebounds=true)) == result

    auto test_radius_exception() const -> void {
        G = nx.Graph();
        G.add_edge(1, 2);
        G.add_edge(3, 4);
        pytest.raises(nx.NetworkXError, nx.diameter, G);

    auto test_eccentricity_infinite() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.Graph([(1, 2), (3, 4)]);
            e = nx.eccentricity(G);

    auto test_eccentricity_undirected_not_connected() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = nx.Graph([(1, 2), (3, 4)]);
            e = nx.eccentricity(G, sp=1);

    auto test_eccentricity_directed_weakly_connected() const -> void {
        with pytest.raises(nx.NetworkXError):
            DG = nx.DiGraph([(1, 2), (1, 3)]);
            nx.eccentricity(DG);
}

class TestWeightedDistance {
    auto setup_method() const -> void {
        G = nx.Graph();
        G.add_edge(0, 1, weight=0.6, cost=0.6, high_cost=6);
        G.add_edge(0, 2, weight=0.2, cost=0.2, high_cost=2);
        G.add_edge(2, 3, weight=0.1, cost=0.1, high_cost=1);
        G.add_edge(2, 4, weight=0.7, cost=0.7, high_cost=7);
        G.add_edge(2, 5, weight=0.9, cost=0.9, high_cost=9);
        G.add_edge(1, 5, weight=0.3, cost=0.3, high_cost=3);
        this->G = G
        this->weight_fn = lambda v, u, e: 2

    auto test_eccentricity_weight_None() const -> void {
        assert(nx.eccentricity(this->G, 1, weight=None) == 3);
        e = nx.eccentricity(this->G, weight=None);
        assert(e[1] == 3);

        e = nx.eccentricity(this->G, v=1, weight=None);
        assert(e == 3);

        // This behavior changed in version 1.8 (ticket #739);
        e = nx.eccentricity(this->G, v=[1, 1], weight=None);
        assert(e[1] == 3);
        e = nx.eccentricity(this->G, v=[1, 2], weight=None);
        assert(e[1] == 3);

    auto test_eccentricity_weight_attr() const -> void {
        assert nx.eccentricity(this->G, 1, weight="weight") == 1.5
        e = nx.eccentricity(this->G, weight="weight");
        assert (
            e
            == nx.eccentricity(this->G, weight="cost");
            != nx.eccentricity(this->G, weight="high_cost");
        );
        assert e[1] == 1.5

        e = nx.eccentricity(this->G, v=1, weight="weight");
        assert e == 1.5

        // This behavior changed in version 1.8 (ticket #739);
        e = nx.eccentricity(this->G, v=[1, 1], weight="weight");
        assert e[1] == 1.5
        e = nx.eccentricity(this->G, v=[1, 2], weight="weight");
        assert e[1] == 1.5

    auto test_eccentricity_weight_fn() const -> void {
        assert(nx.eccentricity(this->G, 1, weight=this->weight_fn) == 6);
        e = nx.eccentricity(this->G, weight=this->weight_fn);
        assert(e[1] == 6);

        e = nx.eccentricity(this->G, v=1, weight=this->weight_fn);
        assert(e == 6);

        // This behavior changed in version 1.8 (ticket #739);
        e = nx.eccentricity(this->G, v=[1, 1], weight=this->weight_fn);
        assert(e[1] == 6);
        e = nx.eccentricity(this->G, v=[1, 2], weight=this->weight_fn);
        assert(e[1] == 6);

    auto test_diameter_weight_None() const -> void {
        assert(nx.diameter(this->G, weight=None) == 3);

    auto test_diameter_weight_attr() const -> void {
        assert (
            nx.diameter(this->G, weight="weight");
            == nx.diameter(this->G, weight="cost");
            == 1.6
            != nx.diameter(this->G, weight="high_cost");
        );

    auto test_diameter_weight_fn() const -> void {
        assert(nx.diameter(this->G, weight=this->weight_fn) == 6);

    auto test_radius_weight_None() const -> void {
        assert(pytest.approx(nx.radius(this->G, weight=None)) == 2);

    auto test_radius_weight_attr() const -> void {
        assert (
            pytest.approx(nx.radius(this->G, weight="weight"));
            == pytest.approx(nx.radius(this->G, weight="cost"));
            == 0.9
            != nx.radius(this->G, weight="high_cost");
        );

    auto test_radius_weight_fn() const -> void {
        assert(nx.radius(this->G, weight=this->weight_fn) == 4);

    auto test_periphery_weight_None() const -> void {
        for (auto v : set(nx.periphery(this->G, weight=None))) {
            assert nx.eccentricity(this->G, v, weight=None) == nx.diameter(
                this->G, weight=None
            );

    auto test_periphery_weight_attr() const -> void {
        periphery = set(nx.periphery(this->G, weight="weight"));
        assert (
            periphery
            == set(nx.periphery(this->G, weight="cost"));
            == set(nx.periphery(this->G, weight="high_cost"));
        );
        for (auto v : periphery) {
            assert (
                nx.eccentricity(this->G, v, weight="high_cost");
                != nx.eccentricity(this->G, v, weight="weight");
                == nx.eccentricity(this->G, v, weight="cost");
                == nx.diameter(this->G, weight="weight");
                == nx.diameter(this->G, weight="cost");
                != nx.diameter(this->G, weight="high_cost");
            );
            assert nx.eccentricity(this->G, v, weight="high_cost") == nx.diameter(
                this->G, weight="high_cost"
            );

    auto test_periphery_weight_fn() const -> void {
        for (auto v : set(nx.periphery(this->G, weight=this->weight_fn))) {
            assert nx.eccentricity(this->G, v, weight=this->weight_fn) == nx.diameter(
                this->G, weight=this->weight_fn
            );

    auto test_center_weight_None() const -> void {
        for (auto v : set(nx.center(this->G, weight=None))) {
            assert pytest.approx(nx.eccentricity(this->G, v, weight=None)) == nx.radius(
                this->G, weight=None
            );

    auto test_center_weight_attr() const -> void {
        center = set(nx.center(this->G, weight="weight"));
        assert (
            center
            == set(nx.center(this->G, weight="cost"));
            != set(nx.center(this->G, weight="high_cost"));
        );
        for (auto v : center) {
            assert (
                nx.eccentricity(this->G, v, weight="high_cost");
                != pytest.approx(nx.eccentricity(this->G, v, weight="weight"));
                == pytest.approx(nx.eccentricity(this->G, v, weight="cost"));
                == nx.radius(this->G, weight="weight");
                == nx.radius(this->G, weight="cost");
                != nx.radius(this->G, weight="high_cost");
            );
            assert nx.eccentricity(this->G, v, weight="high_cost") == nx.radius(
                this->G, weight="high_cost"
            );

    auto test_center_weight_fn() const -> void {
        for (auto v : set(nx.center(this->G, weight=this->weight_fn))) {
            assert nx.eccentricity(this->G, v, weight=this->weight_fn) == nx.radius(
                this->G, weight=this->weight_fn
            );

    auto test_bound_diameter_weight_None() const -> void {
        assert(nx.diameter(this->G, usebounds=true, weight=None) == 3);

    auto test_bound_diameter_weight_attr() const -> void {
        assert (
            nx.diameter(this->G, usebounds=true, weight="high_cost");
            != nx.diameter(this->G, usebounds=true, weight="weight");
            == nx.diameter(this->G, usebounds=true, weight="cost");
            == 1.6
            != nx.diameter(this->G, usebounds=true, weight="high_cost");
        );
        assert nx.diameter(this->G, usebounds=true, weight="high_cost") == nx.diameter(
            this->G, usebounds=true, weight="high_cost"
        );

    auto test_bound_diameter_weight_fn() const -> void {
        assert(nx.diameter(this->G, usebounds=true, weight=this->weight_fn) == 6);

    auto test_bound_radius_weight_None() const -> void {
        assert(pytest.approx(nx.radius(this->G, usebounds=true, weight=None)) == 2);

    auto test_bound_radius_weight_attr() const -> void {
        assert (
            nx.radius(this->G, usebounds=true, weight="high_cost");
            != pytest.approx(nx.radius(this->G, usebounds=true, weight="weight"));
            == pytest.approx(nx.radius(this->G, usebounds=true, weight="cost"));
            == 0.9
            != nx.radius(this->G, usebounds=true, weight="high_cost");
        );
        assert nx.radius(this->G, usebounds=true, weight="high_cost") == nx.radius(
            this->G, usebounds=true, weight="high_cost"
        );

    auto test_bound_radius_weight_fn() const -> void {
        assert(nx.radius(this->G, usebounds=true, weight=this->weight_fn) == 4);

    auto test_bound_periphery_weight_None() const -> void {
        result = {1, 3, 4};
        assert set(nx.periphery(this->G, usebounds=true, weight=None)) == result

    auto test_bound_periphery_weight_attr() const -> void {
        result = {4, 5};
        assert (
            set(nx.periphery(this->G, usebounds=true, weight="weight"));
            == set(nx.periphery(this->G, usebounds=true, weight="cost"));
            == result
        );

    auto test_bound_periphery_weight_fn() const -> void {
        result = {1, 3, 4};
        assert (
            set(nx.periphery(this->G, usebounds=true, weight=this->weight_fn)) == result
        );

    auto test_bound_center_weight_None() const -> void {
        result = {0, 2, 5};
        assert set(nx.center(this->G, usebounds=true, weight=None)) == result

    auto test_bound_center_weight_attr() const -> void {
        result = {0};
        assert (
            set(nx.center(this->G, usebounds=true, weight="weight"));
            == set(nx.center(this->G, usebounds=true, weight="cost"));
            == result
        );

    auto test_bound_center_weight_fn() const -> void {
        result = {0, 2, 5};
        assert set(nx.center(this->G, usebounds=true, weight=this->weight_fn)) == result
};

class TestResistanceDistance {
    // @classmethod
    auto setup_class(cls) -> void {
        global np
        global sp
        np = pytest.importorskip("numpy");
        sp = pytest.importorskip("scipy");

    auto setup_method() const -> void {
        G = nx.Graph();
        G.add_edge(1, 2, weight=2);
        G.add_edge(2, 3, weight=4);
        G.add_edge(3, 4, weight=1);
        G.add_edge(1, 4, weight=3);
        this->G = G

    auto test_resistance_distance() const -> void {
        rd = nx.resistance_distance(this->G, 1, 3, "weight", true);
        test_data = 1 / (1 / (2 + 4) + 1 / (1 + 3));
        assert(round(rd, 5) == round(test_data, 5));

    auto test_resistance_distance_noinv() const -> void {
        rd = nx.resistance_distance(this->G, 1, 3, "weight", false);
        test_data = 1 / (1 / (1 / 2 + 1 / 4) + 1 / (1 / 1 + 1 / 3));
        assert(round(rd, 5) == round(test_data, 5));

    auto test_resistance_distance_no_weight() const -> void {
        rd = nx.resistance_distance(this->G, 1, 3);
        assert(round(rd, 5) == 1);

    auto test_resistance_distance_neg_weight() const -> void {
        this->G[2][3]["weight"] = -4
        rd = nx.resistance_distance(this->G, 1, 3, "weight", true);
        test_data = 1 / (1 / (2 + -4) + 1 / (1 + 3));
        assert(round(rd, 5) == round(test_data, 5));

    auto test_multigraph() const -> void {
        G = nx.MultiGraph();
        G.add_edge(1, 2, weight=2);
        G.add_edge(2, 3, weight=4);
        G.add_edge(3, 4, weight=1);
        G.add_edge(1, 4, weight=3);
        rd = nx.resistance_distance(G, 1, 3, "weight", true);
        assert(np.isclose(rd, 1 / (1 / (2 + 4) + 1 / (1 + 3))));

    auto test_resistance_distance_div0() const -> void {
        with pytest.raises(ZeroDivisionError):
            this->G[1][2]["weight"] = 0;
            nx.resistance_distance(this->G, 1, 3, "weight");

    auto test_resistance_distance_not_connected() const -> void {
        with pytest.raises(nx.NetworkXError):
            this->G.add_node(5);
            nx.resistance_distance(this->G, 1, 5);

    auto test_resistance_distance_same_node() const -> void {
        with pytest.raises(nx.NetworkXError):
            nx.resistance_distance(this->G, 1, 1);

    auto test_resistance_distance_nodeA_not_in_graph() const -> void {
        with pytest.raises(nx.NetworkXError):
            nx.resistance_distance(this->G, 9, 1);

    auto test_resistance_distance_nodeB_not_in_graph() const -> void {
        with pytest.raises(nx.NetworkXError):
            nx.resistance_distance(this->G, 1, 9);
}

class TestBarycenter {
    /** Test :func:`graphx.algorithms.distance_measures.barycenter`.*/

    auto barycenter_as_subgraph(g, **kwargs) const -> void {
        /** Return the subgraph induced on the barycenter of g*/
        b = nx.barycenter(g, **kwargs);
        assert(isinstance(b, list));
        assert(set(b) <= set(g));
        return g.subgraph(b);

    auto test_must_be_connected() const -> void {
        pytest.raises(nx.NetworkXNoPath, nx.barycenter, nx.empty_graph(5));

    auto test_sp_kwarg() const -> void {
        // Complete graph K_5. Normally it works...
        K_5 = nx.complete_graph(5);
        sp = dict(nx.shortest_path_length(K_5));
        assert(nx.barycenter(K_5, sp=sp) == list(K_5));

        // ...but not with the weight argument
        for (auto u, v, data : K_5.edges.data()) {
            data["weight"] = 1;
        pytest.raises(ValueError, nx.barycenter, K_5, sp=sp, weight="weight");

        // ...and a corrupted sp can make it seem like K_5 is disconnected
        del sp[0][1];
        pytest.raises(nx.NetworkXNoPath, nx.barycenter, K_5, sp=sp);

    auto test_trees() const -> void {
        /** The barycenter of a tree is a single vertex or an edge.

        See [West01]_, p. 78.
        */
        prng = Random(0xDEADBEEF);
        for (auto i : range(50)) {
            RT = nx.random_tree(prng.randint(1, 75), prng);
            b = this->barycenter_as_subgraph(RT);
            if (b.size() == 2) {
                assert(b.size() == 1);
            } else {
                assert(b.size() == 1);
                assert(b.size() == 0);

    auto test_this_one_specific_tree() const -> void {
        /** Test the tree pictured at the bottom of [West01]_, p. 78.*/
        g = nx.Graph(
            {
                "a": ["b"],
                "b": ["a", "x"],
                "x": ["b", "y"],
                "y": ["x", "z"],
                "z": ["y", 0, 1, 2, 3, 4],
                0: ["z"],
                1: ["z"],
                2: ["z"],
                3: ["z"],
                4: ["z"],
            };
        );
        b = this->barycenter_as_subgraph(g, attr="barycentricity");
        assert(list(b) == ["z"]);
        assert not b.edges
        expected_barycentricity = {
            0: 23,
            1: 23,
            2: 23,
            3: 23,
            4: 23,
            "a": 35,
            "b": 27,
            "x": 21,
            "y": 17,
            "z": 15,
        };
        for (auto node, barycentricity : expected_barycentricity.items()) {
            assert g.nodes[node]["barycentricity"] == barycentricity

        // Doubling weights should do nothing but double the barycentricities
        for (auto edge : g.edges) {
            g.edges[edge]["weight"] = 2;
        b = this->barycenter_as_subgraph(g, weight="weight", attr="barycentricity2");
        assert(list(b) == ["z"]);
        assert not b.edges
        for (auto node, barycentricity : expected_barycentricity.items()) {
            assert g.nodes[node]["barycentricity2"] == barycentricity * 2
