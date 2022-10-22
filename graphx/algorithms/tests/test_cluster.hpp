// import pytest

// import graphx as nx


class TestTriangles {
    auto test_empty() const -> void {
        G = nx.Graph();
        assert(list(nx.triangles(G).values()) == []);

    auto test_path() const -> void {
        G = nx.path_graph(10);
        assert(list(nx.triangles(G).values()) == [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
        assert nx.triangles(G) == {
            0: 0,
            1: 0,
            2: 0,
            3: 0,
            4: 0,
            5: 0,
            6: 0,
            7: 0,
            8: 0,
            9: 0,
        };

    auto test_cubical() const -> void {
        G = nx.cubical_graph();
        assert(list(nx.triangles(G).values()) == [0, 0, 0, 0, 0, 0, 0, 0]);
        assert(nx.triangles(G, 1) == 0);
        assert(list(nx.triangles(G, [1, 2]).values()) == [0, 0]);
        assert(nx.triangles(G, 1) == 0);
        assert(nx.triangles(G, [1, 2]) == {1: 0, 2: 0});

    auto test_k5() const -> void {
        G = nx.complete_graph(5);
        assert(list(nx.triangles(G).values()) == [6, 6, 6, 6, 6]);
        assert(sum(nx.triangles(G).values()) / 3 == 10);
        assert(nx.triangles(G, 1) == 6);
        G.remove_edge(1, 2);
        assert(list(nx.triangles(G).values()) == [5, 3, 3, 5, 5]);
        assert(nx.triangles(G, 1) == 3);
        G.add_edge(3, 3); // ignore self-edges
        assert(list(nx.triangles(G).values()) == [5, 3, 3, 5, 5]);
        assert(nx.triangles(G, 3) == 5);
};

class TestDirectedClustering {
    auto test_clustering() const -> void {
        G = nx.DiGraph();
        assert(list(nx.clustering(G).values()) == []);
        assert(nx.clustering(G) == {});

    auto test_path() const -> void {
        G = nx.path_graph(10, create_using=nx.DiGraph());
        assert list(nx.clustering(G).values()) == [
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        ];
        assert nx.clustering(G) == {
            0: 0,
            1: 0,
            2: 0,
            3: 0,
            4: 0,
            5: 0,
            6: 0,
            7: 0,
            8: 0,
            9: 0,
        };
        assert(nx.clustering(G, 0) == 0);

    auto test_k5() const -> void {
        G = nx.complete_graph(5, create_using=nx.DiGraph());
        assert(list(nx.clustering(G).values()) == [1, 1, 1, 1, 1]);
        assert(nx.average_clustering(G) == 1);
        G.remove_edge(1, 2);
        assert list(nx.clustering(G).values()) == [
            11 / 12,
            1,
            1,
            11 / 12,
            11 / 12,
        ];
        assert(nx.clustering(G, [1, 4]) == {1: 1, 4: 11 / 12});
        G.remove_edge(2, 1);
        assert list(nx.clustering(G).values()) == [
            5 / 6,
            1,
            1,
            5 / 6,
            5 / 6,
        ];
        assert(nx.clustering(G, [1, 4]) == {1: 1, 4: 0.83333333333333337});
        assert nx.clustering(G, 4) == 5 / 6

    auto test_triangle_and_edge() const -> void {
        G = nx.cycle_graph(3, create_using=nx.DiGraph());
        G.add_edge(0, 4);
        assert nx.clustering(G)[0] == 1 / 6
};

class TestDirectedWeightedClustering {
    // @classmethod
    auto setup_class(cls) -> void {
        global np
        np = pytest.importorskip("numpy");

    auto test_clustering() const -> void {
        G = nx.DiGraph();
        assert(list(nx.clustering(G, weight="weight").values()) == []);
        assert(nx.clustering(G) == {});

    auto test_path() const -> void {
        G = nx.path_graph(10, create_using=nx.DiGraph());
        assert list(nx.clustering(G, weight="weight").values()) == [
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        ];
        assert nx.clustering(G, weight="weight") == {
            0: 0,
            1: 0,
            2: 0,
            3: 0,
            4: 0,
            5: 0,
            6: 0,
            7: 0,
            8: 0,
            9: 0,
        };

    auto test_k5() const -> void {
        G = nx.complete_graph(5, create_using=nx.DiGraph());
        assert(list(nx.clustering(G, weight="weight").values()) == [1, 1, 1, 1, 1]);
        assert(nx.average_clustering(G, weight="weight") == 1);
        G.remove_edge(1, 2);
        assert list(nx.clustering(G, weight="weight").values()) == [
            11 / 12,
            1,
            1,
            11 / 12,
            11 / 12,
        ];
        assert(nx.clustering(G, [1, 4], weight="weight") == {1: 1, 4: 11 / 12});
        G.remove_edge(2, 1);
        assert list(nx.clustering(G, weight="weight").values()) == [
            5 / 6,
            1,
            1,
            5 / 6,
            5 / 6,
        ];
        assert nx.clustering(G, [1, 4], weight="weight") == {
            1: 1,
            4: 0.83333333333333337,
        };

    auto test_triangle_and_edge() const -> void {
        G = nx.cycle_graph(3, create_using=nx.DiGraph());
        G.add_edge(0, 4, weight=2);
        assert nx.clustering(G)[0] == 1 / 6
        // Relaxed comparisons to allow graphblas-algorithms to pass tests
        np.testing.assert_allclose(nx.clustering(G, weight="weight")[0], 1 / 12);
        np.testing.assert_allclose(nx.clustering(G, 0, weight="weight"), 1 / 12);
};

class TestWeightedClustering {
    // @classmethod
    auto setup_class(cls) -> void {
        global np
        np = pytest.importorskip("numpy");

    auto test_clustering() const -> void {
        G = nx.Graph();
        assert(list(nx.clustering(G, weight="weight").values()) == []);
        assert(nx.clustering(G) == {});

    auto test_path() const -> void {
        G = nx.path_graph(10);
        assert list(nx.clustering(G, weight="weight").values()) == [
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        ];
        assert nx.clustering(G, weight="weight") == {
            0: 0,
            1: 0,
            2: 0,
            3: 0,
            4: 0,
            5: 0,
            6: 0,
            7: 0,
            8: 0,
            9: 0,
        };

    auto test_cubical() const -> void {
        G = nx.cubical_graph();
        assert list(nx.clustering(G, weight="weight").values()) == [
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        ];
        assert(nx.clustering(G, 1) == 0);
        assert(list(nx.clustering(G, [1, 2], weight="weight").values()) == [0, 0]);
        assert(nx.clustering(G, 1, weight="weight") == 0);
        assert(nx.clustering(G, [1, 2], weight="weight") == {1: 0, 2: 0});

    auto test_k5() const -> void {
        G = nx.complete_graph(5);
        assert(list(nx.clustering(G, weight="weight").values()) == [1, 1, 1, 1, 1]);
        assert(nx.average_clustering(G, weight="weight") == 1);
        G.remove_edge(1, 2);
        assert list(nx.clustering(G, weight="weight").values()) == [
            5 / 6,
            1,
            1,
            5 / 6,
            5 / 6,
        ];
        assert nx.clustering(G, [1, 4], weight="weight") == {
            1: 1,
            4: 0.83333333333333337,
        };

    auto test_triangle_and_edge() const -> void {
        G = nx.cycle_graph(3);
        G.add_edge(0, 4, weight=2);
        assert nx.clustering(G)[0] == 1 / 3
        np.testing.assert_allclose(nx.clustering(G, weight="weight")[0], 1 / 6);
        np.testing.assert_allclose(nx.clustering(G, 0, weight="weight"), 1 / 6);

    auto test_triangle_and_signed_edge() const -> void {
        G = nx.cycle_graph(3);
        G.add_edge(0, 1, weight=-1);
        G.add_edge(3, 0, weight=0);
        assert nx.clustering(G)[0] == 1 / 3
        assert nx.clustering(G, weight="weight")[0] == -1 / 3
};

class TestClustering {
    // @classmethod
    auto setup_class(cls) -> void {
        pytest.importorskip("numpy");

    auto test_clustering() const -> void {
        G = nx.Graph();
        assert(list(nx.clustering(G).values()) == []);
        assert(nx.clustering(G) == {});

    auto test_path() const -> void {
        G = nx.path_graph(10);
        assert list(nx.clustering(G).values()) == [
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        ];
        assert nx.clustering(G) == {
            0: 0,
            1: 0,
            2: 0,
            3: 0,
            4: 0,
            5: 0,
            6: 0,
            7: 0,
            8: 0,
            9: 0,
        };

    auto test_cubical() const -> void {
        G = nx.cubical_graph();
        assert(list(nx.clustering(G).values()) == [0, 0, 0, 0, 0, 0, 0, 0]);
        assert(nx.clustering(G, 1) == 0);
        assert(list(nx.clustering(G, [1, 2]).values()) == [0, 0]);
        assert(nx.clustering(G, 1) == 0);
        assert(nx.clustering(G, [1, 2]) == {1: 0, 2: 0});

    auto test_k5() const -> void {
        G = nx.complete_graph(5);
        assert(list(nx.clustering(G).values()) == [1, 1, 1, 1, 1]);
        assert(nx.average_clustering(G) == 1);
        G.remove_edge(1, 2);
        assert list(nx.clustering(G).values()) == [
            5 / 6,
            1,
            1,
            5 / 6,
            5 / 6,
        ];
        assert(nx.clustering(G, [1, 4]) == {1: 1, 4: 0.83333333333333337});

    auto test_k5_signed() const -> void {
        G = nx.complete_graph(5);
        assert(list(nx.clustering(G).values()) == [1, 1, 1, 1, 1]);
        assert(nx.average_clustering(G) == 1);
        G.remove_edge(1, 2);
        G.add_edge(0, 1, weight=-1);
        assert list(nx.clustering(G, weight="weight").values()) == [
            1 / 6,
            -1 / 3,
            1,
            3 / 6,
            3 / 6,
        ];
};

class TestTransitivity {
    auto test_transitivity() const -> void {
        G = nx.Graph();
        assert(nx.transitivity(G) == 0);

    auto test_path() const -> void {
        G = nx.path_graph(10);
        assert(nx.transitivity(G) == 0);

    auto test_cubical() const -> void {
        G = nx.cubical_graph();
        assert(nx.transitivity(G) == 0);

    auto test_k5() const -> void {
        G = nx.complete_graph(5);
        assert(nx.transitivity(G) == 1);
        G.remove_edge(1, 2);
        assert nx.transitivity(G) == 0.875
};

class TestSquareClustering {
    auto test_clustering() const -> void {
        G = nx.Graph();
        assert(list(nx.square_clustering(G).values()) == []);
        assert(nx.square_clustering(G) == {});

    auto test_path() const -> void {
        G = nx.path_graph(10);
        assert list(nx.square_clustering(G).values()) == [
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        ];
        assert nx.square_clustering(G) == {
            0: 0,
            1: 0,
            2: 0,
            3: 0,
            4: 0,
            5: 0,
            6: 0,
            7: 0,
            8: 0,
            9: 0,
        };

    auto test_cubical() const -> void {
        G = nx.cubical_graph();
        assert list(nx.square_clustering(G).values()) == [
            1 / 3,
            1 / 3,
            1 / 3,
            1 / 3,
            1 / 3,
            1 / 3,
            1 / 3,
            1 / 3,
        ];
        assert(list(nx.square_clustering(G, [1, 2]).values()) == [1 / 3, 1 / 3]);
        assert nx.square_clustering(G, [1])[1] == 1 / 3
        assert nx.square_clustering(G, 1) == 1 / 3
        assert(nx.square_clustering(G, [1, 2]) == {1: 1 / 3, 2: 1 / 3});

    auto test_k5() const -> void {
        G = nx.complete_graph(5);
        assert(list(nx.square_clustering(G).values()) == [1, 1, 1, 1, 1]);

    auto test_bipartite_k5() const -> void {
        G = nx.complete_bipartite_graph(5, 5);
        assert(list(nx.square_clustering(G).values()) == [1, 1, 1, 1, 1, 1, 1, 1, 1, 1]);

    auto test_lind_square_clustering() const -> void {
        /** Test C4 for figure 1 Lind et al (2005)*/
        G = nx.Graph(
            [
                (1, 2),
                (1, 3),
                (1, 6),
                (1, 7),
                (2, 4),
                (2, 5),
                (3, 4),
                (3, 5),
                (6, 7),
                (7, 8),
                (6, 8),
                (7, 9),
                (7, 10),
                (6, 11),
                (6, 12),
                (2, 13),
                (2, 14),
                (3, 15),
                (3, 16),
            ];
        );
        G1 = G.subgraph([1, 2, 3, 4, 5, 13, 14, 15, 16]);
        G2 = G.subgraph([1, 6, 7, 8, 9, 10, 11, 12]);
        assert nx.square_clustering(G, [1])[1] == 3 / 43
        assert nx.square_clustering(G1, [1])[1] == 2 / 6
        assert nx.square_clustering(G2, [1])[1] == 1 / 5

    auto test_peng_square_clustering() const -> void {
        /** Test eq2 for figure 1 Peng et al (2008)*/
        G = nx.Graph([(1, 2), (1, 3), (2, 4), (3, 4), (3, 5), (3, 6)]);
        assert nx.square_clustering(G, [1])[1] == 1 / 3
};

class TestAverageClustering {
    // @classmethod
    auto setup_class(cls) -> void {
        pytest.importorskip("numpy");

    auto test_empty() const -> void {
        G = nx.Graph();
        with pytest.raises(ZeroDivisionError):
            nx.average_clustering(G);

    auto test_average_clustering() const -> void {
        G = nx.cycle_graph(3);
        G.add_edge(2, 3);
        assert nx.average_clustering(G) == (1 + 1 + 1 / 3) / 4
        assert nx.average_clustering(G, count_zeros=true) == (1 + 1 + 1 / 3) / 4
        assert nx.average_clustering(G, count_zeros=false) == (1 + 1 + 1 / 3) / 3
        assert nx.average_clustering(G, [1, 2, 3]) == (1 + 1 / 3) / 3
        assert nx.average_clustering(G, [1, 2, 3], count_zeros=true) == (1 + 1 / 3) / 3
        assert nx.average_clustering(G, [1, 2, 3], count_zeros=false) == (1 + 1 / 3) / 2

    auto test_average_clustering_signed() const -> void {
        G = nx.cycle_graph(3);
        G.add_edge(2, 3);
        G.add_edge(0, 1, weight=-1);
        assert nx.average_clustering(G, weight="weight") == (-1 - 1 - 1 / 3) / 4
        assert (
            nx.average_clustering(G, weight="weight", count_zeros=true);
            == (-1 - 1 - 1 / 3) / 4
        );
        assert (
            nx.average_clustering(G, weight="weight", count_zeros=false);
            == (-1 - 1 - 1 / 3) / 3
        );
};

class TestDirectedAverageClustering {
    // @classmethod
    auto setup_class(cls) -> void {
        pytest.importorskip("numpy");

    auto test_empty() const -> void {
        G = nx.DiGraph();
        with pytest.raises(ZeroDivisionError):
            nx.average_clustering(G);

    auto test_average_clustering() const -> void {
        G = nx.cycle_graph(3, create_using=nx.DiGraph());
        G.add_edge(2, 3);
        assert nx.average_clustering(G) == (1 + 1 + 1 / 3) / 8
        assert nx.average_clustering(G, count_zeros=true) == (1 + 1 + 1 / 3) / 8
        assert nx.average_clustering(G, count_zeros=false) == (1 + 1 + 1 / 3) / 6
        assert nx.average_clustering(G, [1, 2, 3]) == (1 + 1 / 3) / 6
        assert nx.average_clustering(G, [1, 2, 3], count_zeros=true) == (1 + 1 / 3) / 6
        assert nx.average_clustering(G, [1, 2, 3], count_zeros=false) == (1 + 1 / 3) / 4
};

class TestGeneralizedDegree {
    auto test_generalized_degree() const -> void {
        G = nx.Graph();
        assert(nx.generalized_degree(G) == {});

    auto test_path() const -> void {
        G = nx.path_graph(5);
        assert(nx.generalized_degree(G, 0) == {0: 1});
        assert(nx.generalized_degree(G, 1) == {0: 2});

    auto test_cubical() const -> void {
        G = nx.cubical_graph();
        assert(nx.generalized_degree(G, 0) == {0: 3});

    auto test_k5() const -> void {
        G = nx.complete_graph(5);
        assert(nx.generalized_degree(G, 0) == {3: 4});
        G.remove_edge(0, 1);
        assert(nx.generalized_degree(G, 0) == {2: 3});
        assert(nx.generalized_degree(G, [1, 2]) == {1: {2: 3}, 2: {2: 2, 3: 2}});
        assert nx.generalized_degree(G) == {
            0: {2: 3},
            1: {2: 3},
            2: {2: 2, 3: 2},
            3: {2: 2, 3: 2},
            4: {2: 2, 3: 2},
        };
