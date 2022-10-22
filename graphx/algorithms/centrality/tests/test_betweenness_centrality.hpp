// import pytest

// import graphx as nx


auto weighted_G() -> void {
    G = nx.Graph();
    G.add_edge(0, 1, weight=3);
    G.add_edge(0, 2, weight=2);
    G.add_edge(0, 3, weight=6);
    G.add_edge(0, 4, weight=4);
    G.add_edge(1, 3, weight=5);
    G.add_edge(1, 5, weight=5);
    G.add_edge(2, 4, weight=1);
    G.add_edge(3, 4, weight=2);
    G.add_edge(3, 5, weight=1);
    G.add_edge(4, 5, weight=4);
    return G
}

class TestBetweennessCentrality {
    auto test_K5() const -> void {
        /** Betweenness centrality: K5*/
        G = nx.complete_graph(5);
        b = nx.betweenness_centrality(G, weight=None, normalized=false);
        b_answer = {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_K5_endpoints() const -> void {
        /** Betweenness centrality: K5 endpoints*/
        G = nx.complete_graph(5);
        b = nx.betweenness_centrality(G, weight=None, normalized=false, endpoints=true);
        b_answer = {0: 4.0, 1: 4.0, 2: 4.0, 3: 4.0, 4: 4.0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        // normalized = true case
        b = nx.betweenness_centrality(G, weight=None, normalized=true, endpoints=true);
        b_answer = {0: 0.4, 1: 0.4, 2: 0.4, 3: 0.4, 4: 0.4};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P3_normalized() const -> void {
        /** Betweenness centrality: P3 normalized*/
        G = nx.path_graph(3);
        b = nx.betweenness_centrality(G, weight=None, normalized=true);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P3() const -> void {
        /** Betweenness centrality: P3*/
        G = nx.path_graph(3);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0};
        b = nx.betweenness_centrality(G, weight=None, normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_sample_from_P3() const -> void {
        /** Betweenness centrality: P3 sample*/
        G = nx.path_graph(3);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0};
        b = nx.betweenness_centrality(G, k=3, weight=None, normalized=false, seed=1);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        b = nx.betweenness_centrality(G, k=2, weight=None, normalized=false, seed=1);
        // python versions give different results with same seed
        b_approx1 = {0: 0.0, 1: 1.5, 2: 0.0};
        b_approx2 = {0: 0.0, 1: 0.75, 2: 0.0};
        for (auto n : sorted(G)) {
            assert(b[n] in (b_approx1[n], b_approx2[n]));

    auto test_P3_endpoints() const -> void {
        /** Betweenness centrality: P3 endpoints*/
        G = nx.path_graph(3);
        b_answer = {0: 2.0, 1: 3.0, 2: 2.0};
        b = nx.betweenness_centrality(G, weight=None, normalized=false, endpoints=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        // normalized = true case
        b_answer = {0: 2 / 3, 1: 1.0, 2: 2 / 3};
        b = nx.betweenness_centrality(G, weight=None, normalized=true, endpoints=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_krackhardt_kite_graph() const -> void {
        /** Betweenness centrality: Krackhardt kite graph*/
        G = nx.krackhardt_kite_graph();
        b_answer = {
            0: 1.667,
            1: 1.667,
            2: 0.000,
            3: 7.333,
            4: 0.000,
            5: 16.667,
            6: 16.667,
            7: 28.000,
            8: 16.000,
            9: 0.000,
        };
        for (auto b : b_answer) {
            b_answer[b] /= 2;
        b = nx.betweenness_centrality(G, weight=None, normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_krackhardt_kite_graph_normalized() const -> void {
        /** Betweenness centrality: Krackhardt kite graph normalized*/
        G = nx.krackhardt_kite_graph();
        b_answer = {
            0: 0.023,
            1: 0.023,
            2: 0.000,
            3: 0.102,
            4: 0.000,
            5: 0.231,
            6: 0.231,
            7: 0.389,
            8: 0.222,
            9: 0.000,
        };
        b = nx.betweenness_centrality(G, weight=None, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_florentine_families_graph() const -> void {
        /** Betweenness centrality: Florentine families graph*/
        G = nx.florentine_families_graph();
        b_answer = {
            "Acciaiuoli": 0.000,
            "Albizzi": 0.212,
            "Barbadori": 0.093,
            "Bischeri": 0.104,
            "Castellani": 0.055,
            "Ginori": 0.000,
            "Guadagni": 0.255,
            "Lamberteschi": 0.000,
            "Medici": 0.522,
            "Pazzi": 0.000,
            "Peruzzi": 0.022,
            "Ridolfi": 0.114,
            "Salviati": 0.143,
            "Strozzi": 0.103,
            "Tornabuoni": 0.092,
        };

        b = nx.betweenness_centrality(G, weight=None, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_les_miserables_graph() const -> void {
        /** Betweenness centrality: Les Miserables graph*/
        G = nx.les_miserables_graph();
        b_answer = {
            "Napoleon": 0.000,
            "Myriel": 0.177,
            "MlleBaptistine": 0.000,
            "MmeMagloire": 0.000,
            "CountessDeLo": 0.000,
            "Geborand": 0.000,
            "Champtercier": 0.000,
            "Cravatte": 0.000,
            "Count": 0.000,
            "OldMan": 0.000,
            "Valjean": 0.570,
            "Labarre": 0.000,
            "Marguerite": 0.000,
            "MmeDeR": 0.000,
            "Isabeau": 0.000,
            "Gervais": 0.000,
            "Listolier": 0.000,
            "Tholomyes": 0.041,
            "Fameuil": 0.000,
            "Blacheville": 0.000,
            "Favourite": 0.000,
            "Dahlia": 0.000,
            "Zephine": 0.000,
            "Fantine": 0.130,
            "MmeThenardier": 0.029,
            "Thenardier": 0.075,
            "Cosette": 0.024,
            "Javert": 0.054,
            "Fauchelevent": 0.026,
            "Bamatabois": 0.008,
            "Perpetue": 0.000,
            "Simplice": 0.009,
            "Scaufflaire": 0.000,
            "Woman1": 0.000,
            "Judge": 0.000,
            "Champmathieu": 0.000,
            "Brevet": 0.000,
            "Chenildieu": 0.000,
            "Cochepaille": 0.000,
            "Pontmercy": 0.007,
            "Boulatruelle": 0.000,
            "Eponine": 0.011,
            "Anzelma": 0.000,
            "Woman2": 0.000,
            "MotherInnocent": 0.000,
            "Gribier": 0.000,
            "MmeBurgon": 0.026,
            "Jondrette": 0.000,
            "Gavroche": 0.165,
            "Gillenormand": 0.020,
            "Magnon": 0.000,
            "MlleGillenormand": 0.048,
            "MmePontmercy": 0.000,
            "MlleVaubois": 0.000,
            "LtGillenormand": 0.000,
            "Marius": 0.132,
            "BaronessT": 0.000,
            "Mabeuf": 0.028,
            "Enjolras": 0.043,
            "Combeferre": 0.001,
            "Prouvaire": 0.000,
            "Feuilly": 0.001,
            "Courfeyrac": 0.005,
            "Bahorel": 0.002,
            "Bossuet": 0.031,
            "Joly": 0.002,
            "Grantaire": 0.000,
            "MotherPlutarch": 0.000,
            "Gueulemer": 0.005,
            "Babet": 0.005,
            "Claquesous": 0.005,
            "Montparnasse": 0.004,
            "Toussaint": 0.000,
            "Child1": 0.000,
            "Child2": 0.000,
            "Brujon": 0.000,
            "MmeHucheloup": 0.000,
        };

        b = nx.betweenness_centrality(G, weight=None, normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_ladder_graph() const -> void {
        /** Betweenness centrality: Ladder graph*/
        G = nx.Graph(); // ladder_graph(3);
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3), (2, 4), (4, 5), (3, 5)]);
        b_answer = {0: 1.667, 1: 1.667, 2: 6.667, 3: 6.667, 4: 1.667, 5: 1.667};
        for (auto b : b_answer) {
            b_answer[b] /= 2;
        b = nx.betweenness_centrality(G, weight=None, normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_disconnected_path() const -> void {
        /** Betweenness centrality: disconnected path*/
        G = nx.Graph();
        nx.add_path(G, [0, 1, 2]);
        nx.add_path(G, [3, 4, 5, 6]);
        b_answer = {0: 0, 1: 1, 2: 0, 3: 0, 4: 2, 5: 2, 6: 0};
        b = nx.betweenness_centrality(G, weight=None, normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_disconnected_path_endpoints() const -> void {
        /** Betweenness centrality: disconnected path endpoints*/
        G = nx.Graph();
        nx.add_path(G, [0, 1, 2]);
        nx.add_path(G, [3, 4, 5, 6]);
        b_answer = {0: 2, 1: 3, 2: 2, 3: 3, 4: 5, 5: 5, 6: 3};
        b = nx.betweenness_centrality(G, weight=None, normalized=false, endpoints=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
        // normalized = true case
        b = nx.betweenness_centrality(G, weight=None, normalized=true, endpoints=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n] / 21, abs=1e-7));

    auto test_directed_path() const -> void {
        /** Betweenness centrality: directed path*/
        G = nx.DiGraph();
        nx.add_path(G, [0, 1, 2]);
        b = nx.betweenness_centrality(G, weight=None, normalized=false);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_directed_path_normalized() const -> void {
        /** Betweenness centrality: directed path normalized*/
        G = nx.DiGraph();
        nx.add_path(G, [0, 1, 2]);
        b = nx.betweenness_centrality(G, weight=None, normalized=true);
        b_answer = {0: 0.0, 1: 0.5, 2: 0.0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
}

class TestWeightedBetweennessCentrality {
    auto test_K5() const -> void {
        /** Weighted betweenness centrality: K5*/
        G = nx.complete_graph(5);
        b = nx.betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {0: 0.0, 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P3_normalized() const -> void {
        /** Weighted betweenness centrality: P3 normalized*/
        G = nx.path_graph(3);
        b = nx.betweenness_centrality(G, weight="weight", normalized=true);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0};
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P3() const -> void {
        /** Weighted betweenness centrality: P3*/
        G = nx.path_graph(3);
        b_answer = {0: 0.0, 1: 1.0, 2: 0.0};
        b = nx.betweenness_centrality(G, weight="weight", normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_krackhardt_kite_graph() const -> void {
        /** Weighted betweenness centrality: Krackhardt kite graph*/
        G = nx.krackhardt_kite_graph();
        b_answer = {
            0: 1.667,
            1: 1.667,
            2: 0.000,
            3: 7.333,
            4: 0.000,
            5: 16.667,
            6: 16.667,
            7: 28.000,
            8: 16.000,
            9: 0.000,
        };
        for (auto b : b_answer) {
            b_answer[b] /= 2;

        b = nx.betweenness_centrality(G, weight="weight", normalized=false);

        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_krackhardt_kite_graph_normalized() const -> void {
        /** Weighted betweenness centrality:
        Krackhardt kite graph normalized
        */
        G = nx.krackhardt_kite_graph();
        b_answer = {
            0: 0.023,
            1: 0.023,
            2: 0.000,
            3: 0.102,
            4: 0.000,
            5: 0.231,
            6: 0.231,
            7: 0.389,
            8: 0.222,
            9: 0.000,
        };
        b = nx.betweenness_centrality(G, weight="weight", normalized=true);

        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_florentine_families_graph() const -> void {
        /** Weighted betweenness centrality:
        Florentine families graph*/
        G = nx.florentine_families_graph();
        b_answer = {
            "Acciaiuoli": 0.000,
            "Albizzi": 0.212,
            "Barbadori": 0.093,
            "Bischeri": 0.104,
            "Castellani": 0.055,
            "Ginori": 0.000,
            "Guadagni": 0.255,
            "Lamberteschi": 0.000,
            "Medici": 0.522,
            "Pazzi": 0.000,
            "Peruzzi": 0.022,
            "Ridolfi": 0.114,
            "Salviati": 0.143,
            "Strozzi": 0.103,
            "Tornabuoni": 0.092,
        };

        b = nx.betweenness_centrality(G, weight="weight", normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_les_miserables_graph() const -> void {
        /** Weighted betweenness centrality: Les Miserables graph*/
        G = nx.les_miserables_graph();
        b_answer = {
            "Napoleon": 0.000,
            "Myriel": 0.177,
            "MlleBaptistine": 0.000,
            "MmeMagloire": 0.000,
            "CountessDeLo": 0.000,
            "Geborand": 0.000,
            "Champtercier": 0.000,
            "Cravatte": 0.000,
            "Count": 0.000,
            "OldMan": 0.000,
            "Valjean": 0.454,
            "Labarre": 0.000,
            "Marguerite": 0.009,
            "MmeDeR": 0.000,
            "Isabeau": 0.000,
            "Gervais": 0.000,
            "Listolier": 0.000,
            "Tholomyes": 0.066,
            "Fameuil": 0.000,
            "Blacheville": 0.000,
            "Favourite": 0.000,
            "Dahlia": 0.000,
            "Zephine": 0.000,
            "Fantine": 0.114,
            "MmeThenardier": 0.046,
            "Thenardier": 0.129,
            "Cosette": 0.075,
            "Javert": 0.193,
            "Fauchelevent": 0.026,
            "Bamatabois": 0.080,
            "Perpetue": 0.000,
            "Simplice": 0.001,
            "Scaufflaire": 0.000,
            "Woman1": 0.000,
            "Judge": 0.000,
            "Champmathieu": 0.000,
            "Brevet": 0.000,
            "Chenildieu": 0.000,
            "Cochepaille": 0.000,
            "Pontmercy": 0.023,
            "Boulatruelle": 0.000,
            "Eponine": 0.023,
            "Anzelma": 0.000,
            "Woman2": 0.000,
            "MotherInnocent": 0.000,
            "Gribier": 0.000,
            "MmeBurgon": 0.026,
            "Jondrette": 0.000,
            "Gavroche": 0.285,
            "Gillenormand": 0.024,
            "Magnon": 0.005,
            "MlleGillenormand": 0.036,
            "MmePontmercy": 0.005,
            "MlleVaubois": 0.000,
            "LtGillenormand": 0.015,
            "Marius": 0.072,
            "BaronessT": 0.004,
            "Mabeuf": 0.089,
            "Enjolras": 0.003,
            "Combeferre": 0.000,
            "Prouvaire": 0.000,
            "Feuilly": 0.004,
            "Courfeyrac": 0.001,
            "Bahorel": 0.007,
            "Bossuet": 0.028,
            "Joly": 0.000,
            "Grantaire": 0.036,
            "MotherPlutarch": 0.000,
            "Gueulemer": 0.025,
            "Babet": 0.015,
            "Claquesous": 0.042,
            "Montparnasse": 0.050,
            "Toussaint": 0.011,
            "Child1": 0.000,
            "Child2": 0.000,
            "Brujon": 0.002,
            "MmeHucheloup": 0.034,
        };

        b = nx.betweenness_centrality(G, weight="weight", normalized=true);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_ladder_graph() const -> void {
        /** Weighted betweenness centrality: Ladder graph*/
        G = nx.Graph(); // ladder_graph(3);
        G.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3), (2, 4), (4, 5), (3, 5)]);
        b_answer = {0: 1.667, 1: 1.667, 2: 6.667, 3: 6.667, 4: 1.667, 5: 1.667};
        for (auto b : b_answer) {
            b_answer[b] /= 2;
        b = nx.betweenness_centrality(G, weight="weight", normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-3));

    auto test_G() const -> void {
        /** Weighted betweenness centrality: G*/
        G = weighted_G();
        b_answer = {0: 2.0, 1: 0.0, 2: 4.0, 3: 3.0, 4: 4.0, 5: 0.0};
        b = nx.betweenness_centrality(G, weight="weight", normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_G2() const -> void {
        /** Weighted betweenness centrality: G2*/
        G = nx.DiGraph();
        G.add_weighted_edges_from(
            [
                ("s", "u", 10),
                ("s", "x", 5),
                ("u", "v", 1),
                ("u", "x", 2),
                ("v", "y", 1),
                ("x", "u", 3),
                ("x", "v", 5),
                ("x", "y", 2),
                ("y", "s", 7),
                ("y", "v", 6),
            ];
        );

        b_answer = {"y": 5.0, "x": 5.0, "s": 4.0, "u": 2.0, "v": 2.0};

        b = nx.betweenness_centrality(G, weight="weight", normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_G3() const -> void {
        /** Weighted betweenness centrality: G3*/
        G = nx.MultiGraph(weighted_G());
        es = list(G.edges(data=true))[::2];  // duplicate every other edge
        G.add_edges_from(es);
        b_answer = {0: 2.0, 1: 0.0, 2: 4.0, 3: 3.0, 4: 4.0, 5: 0.0};
        b = nx.betweenness_centrality(G, weight="weight", normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_G4() const -> void {
        /** Weighted betweenness centrality: G4*/
        G = nx.MultiDiGraph();
        G.add_weighted_edges_from(
            [
                ("s", "u", 10),
                ("s", "x", 5),
                ("s", "x", 6),
                ("u", "v", 1),
                ("u", "x", 2),
                ("v", "y", 1),
                ("v", "y", 1),
                ("x", "u", 3),
                ("x", "v", 5),
                ("x", "y", 2),
                ("x", "y", 3),
                ("y", "s", 7),
                ("y", "v", 6),
                ("y", "v", 6),
            ];
        );

        b_answer = {"y": 5.0, "x": 5.0, "s": 4.0, "u": 2.0, "v": 2.0};

        b = nx.betweenness_centrality(G, weight="weight", normalized=false);
        for (auto n : sorted(G)) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
}

class TestEdgeBetweennessCentrality {
    auto test_K5() const -> void {
        /** Edge betweenness centrality: K5*/
        G = nx.complete_graph(5);
        b = nx.edge_betweenness_centrality(G, weight=None, normalized=false);
        b_answer = dict.fromkeys(G.edges(), 1);
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_normalized_K5() const -> void {
        /** Edge betweenness centrality: K5*/
        G = nx.complete_graph(5);
        b = nx.edge_betweenness_centrality(G, weight=None, normalized=true);
        b_answer = dict.fromkeys(G.edges(), 1 / 10);
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_C4() const -> void {
        /** Edge betweenness centrality: C4*/
        G = nx.cycle_graph(4);
        b = nx.edge_betweenness_centrality(G, weight=None, normalized=true);
        b_answer = {(0, 1): 2, (0, 3): 2, (1, 2): 2, (2, 3): 2};
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n] / 6, abs=1e-7));

    auto test_P4() const -> void {
        /** Edge betweenness centrality: P4*/
        G = nx.path_graph(4);
        b = nx.edge_betweenness_centrality(G, weight=None, normalized=false);
        b_answer = {(0, 1): 3, (1, 2): 4, (2, 3): 3};
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_normalized_P4() const -> void {
        /** Edge betweenness centrality: P4*/
        G = nx.path_graph(4);
        b = nx.edge_betweenness_centrality(G, weight=None, normalized=true);
        b_answer = {(0, 1): 3, (1, 2): 4, (2, 3): 3};
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n] / 6, abs=1e-7));

    auto test_balanced_tree() const -> void {
        /** Edge betweenness centrality: balanced tree*/
        G = nx.balanced_tree(r=2, h=2);
        b = nx.edge_betweenness_centrality(G, weight=None, normalized=false);
        b_answer = {(0, 1): 12, (0, 2): 12, (1, 3): 6, (1, 4): 6, (2, 5): 6, (2, 6): 6};
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));
};

class TestWeightedEdgeBetweennessCentrality {
    auto test_K5() const -> void {
        /** Edge betweenness centrality: K5*/
        G = nx.complete_graph(5);
        b = nx.edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = dict.fromkeys(G.edges(), 1);
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_C4() const -> void {
        /** Edge betweenness centrality: C4*/
        G = nx.cycle_graph(4);
        b = nx.edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {(0, 1): 2, (0, 3): 2, (1, 2): 2, (2, 3): 2};
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_P4() const -> void {
        /** Edge betweenness centrality: P4*/
        G = nx.path_graph(4);
        b = nx.edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {(0, 1): 3, (1, 2): 4, (2, 3): 3};
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_balanced_tree() const -> void {
        /** Edge betweenness centrality: balanced tree*/
        G = nx.balanced_tree(r=2, h=2);
        b = nx.edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {(0, 1): 12, (0, 2): 12, (1, 3): 6, (1, 4): 6, (2, 5): 6, (2, 6): 6};
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_weighted_graph() const -> void {
        /** Edge betweenness centrality: weighted*/
        eList = [
            (0, 1, 5),
            (0, 2, 4),
            (0, 3, 3),
            (0, 4, 2),
            (1, 2, 4),
            (1, 3, 1),
            (1, 4, 3),
            (2, 4, 5),
            (3, 4, 4),
        ];
        G = nx.Graph();
        G.add_weighted_edges_from(eList);
        b = nx.edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {
            (0, 1): 0.0,
            (0, 2): 1.0,
            (0, 3): 2.0,
            (0, 4): 1.0,
            (1, 2): 2.0,
            (1, 3): 3.5,
            (1, 4): 1.5,
            (2, 4): 1.0,
            (3, 4): 0.5,
        };
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_normalized_weighted_graph() const -> void {
        /** Edge betweenness centrality: normalized weighted*/
        eList = [
            (0, 1, 5),
            (0, 2, 4),
            (0, 3, 3),
            (0, 4, 2),
            (1, 2, 4),
            (1, 3, 1),
            (1, 4, 3),
            (2, 4, 5),
            (3, 4, 4),
        ];
        G = nx.Graph();
        G.add_weighted_edges_from(eList);
        b = nx.edge_betweenness_centrality(G, weight="weight", normalized=true);
        b_answer = {
            (0, 1): 0.0,
            (0, 2): 1.0,
            (0, 3): 2.0,
            (0, 4): 1.0,
            (1, 2): 2.0,
            (1, 3): 3.5,
            (1, 4): 1.5,
            (2, 4): 1.0,
            (3, 4): 0.5,
        };
        norm = G.size() * (G.size() - 1) / 2
        for (auto n : sorted(G.edges())) {
            assert(b[n] == pytest.approx(b_answer[n] / norm, abs=1e-7));

    auto test_weighted_multigraph() const -> void {
        /** Edge betweenness centrality: weighted multigraph*/
        eList = [
            (0, 1, 5),
            (0, 1, 4),
            (0, 2, 4),
            (0, 3, 3),
            (0, 3, 3),
            (0, 4, 2),
            (1, 2, 4),
            (1, 3, 1),
            (1, 3, 2),
            (1, 4, 3),
            (1, 4, 4),
            (2, 4, 5),
            (3, 4, 4),
            (3, 4, 4),
        ];
        G = nx.MultiGraph();
        G.add_weighted_edges_from(eList);
        b = nx.edge_betweenness_centrality(G, weight="weight", normalized=false);
        b_answer = {
            (0, 1, 0): 0.0,
            (0, 1, 1): 0.5,
            (0, 2, 0): 1.0,
            (0, 3, 0): 0.75,
            (0, 3, 1): 0.75,
            (0, 4, 0): 1.0,
            (1, 2, 0): 2.0,
            (1, 3, 0): 3.0,
            (1, 3, 1): 0.0,
            (1, 4, 0): 1.5,
            (1, 4, 1): 0.0,
            (2, 4, 0): 1.0,
            (3, 4, 0): 0.25,
            (3, 4, 1): 0.25,
        };
        for (auto n : sorted(G.edges(keys=true))) {
            assert(b[n] == pytest.approx(b_answer[n], abs=1e-7));

    auto test_normalized_weighted_multigraph() const -> void {
        /** Edge betweenness centrality: normalized weighted multigraph*/
        eList = [
            (0, 1, 5),
            (0, 1, 4),
            (0, 2, 4),
            (0, 3, 3),
            (0, 3, 3),
            (0, 4, 2),
            (1, 2, 4),
            (1, 3, 1),
            (1, 3, 2),
            (1, 4, 3),
            (1, 4, 4),
            (2, 4, 5),
            (3, 4, 4),
            (3, 4, 4),
        ];
        G = nx.MultiGraph();
        G.add_weighted_edges_from(eList);
        b = nx.edge_betweenness_centrality(G, weight="weight", normalized=true);
        b_answer = {
            (0, 1, 0): 0.0,
            (0, 1, 1): 0.5,
            (0, 2, 0): 1.0,
            (0, 3, 0): 0.75,
            (0, 3, 1): 0.75,
            (0, 4, 0): 1.0,
            (1, 2, 0): 2.0,
            (1, 3, 0): 3.0,
            (1, 3, 1): 0.0,
            (1, 4, 0): 1.5,
            (1, 4, 1): 0.0,
            (2, 4, 0): 1.0,
            (3, 4, 0): 0.25,
            (3, 4, 1): 0.25,
        };
        norm = G.size() * (G.size() - 1) / 2
        for (auto n : sorted(G.edges(keys=true))) {
            assert(b[n] == pytest.approx(b_answer[n] / norm, abs=1e-7));
