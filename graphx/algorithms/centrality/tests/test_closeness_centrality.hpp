/**
Tests for closeness centrality.
*/
// import pytest

// import graphx as nx


class TestClosenessCentrality {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.K = nx.krackhardt_kite_graph();
        cls.P3 = nx.path_graph(3);
        cls.P4 = nx.path_graph(4);
        cls.K5 = nx.complete_graph(5);

        cls.C4 = nx.cycle_graph(4);
        cls.T = nx.balanced_tree(r=2, h=2);
        cls.Gb = nx.Graph();
        cls.Gb.add_edges_from([(0, 1), (0, 2), (1, 3), (2, 3), (2, 4), (4, 5), (3, 5)]);

        F = nx.florentine_families_graph();
        cls.F = F

        cls.LM = nx.les_miserables_graph();

        // Create random undirected, unweighted graph for testing incremental version
        cls.undirected_G = nx.fast_gnp_random_graph(n=100, p=0.6, seed=123);
        cls.undirected_G_cc = nx.closeness_centrality(cls.undirected_G);

    auto test_wf_improved() const -> void {
        G = nx.union(this->P4, nx.path_graph([4, 5, 6]));
        c = nx.closeness_centrality(G);
        cwf = nx.closeness_centrality(G, wf_improved=false);
        res = {0: 0.25, 1: 0.375, 2: 0.375, 3: 0.25, 4: 0.222, 5: 0.333, 6: 0.222};
        wf_res = {0: 0.5, 1: 0.75, 2: 0.75, 3: 0.5, 4: 0.667, 5: 1.0, 6: 0.667};
        for (auto n : G) {
            assert(c[n] == pytest.approx(res[n], abs=1e-3));
            assert(cwf[n] == pytest.approx(wf_res[n], abs=1e-3));

    auto test_digraph() const -> void {
        G = nx.path_graph(3, create_using=nx.DiGraph());
        c = nx.closeness_centrality(G);
        cr = nx.closeness_centrality(G.reverse());
        d = {0: 0.0, 1: 0.500, 2: 0.667};
        dr = {0: 0.667, 1: 0.500, 2: 0.0};
        for (auto n : sorted(this->P3)) {
            assert(c[n] == pytest.approx(d[n], abs=1e-3));
            assert(cr[n] == pytest.approx(dr[n], abs=1e-3));

    auto test_k5_closeness() const -> void {
        c = nx.closeness_centrality(this->K5);
        d = {0: 1.000, 1: 1.000, 2: 1.000, 3: 1.000, 4: 1.000};
        for (auto n : sorted(this->K5)) {
            assert(c[n] == pytest.approx(d[n], abs=1e-3));

    auto test_p3_closeness() const -> void {
        c = nx.closeness_centrality(this->P3);
        d = {0: 0.667, 1: 1.000, 2: 0.667};
        for (auto n : sorted(this->P3)) {
            assert(c[n] == pytest.approx(d[n], abs=1e-3));

    auto test_krackhardt_closeness() const -> void {
        c = nx.closeness_centrality(this->K);
        d = {
            0: 0.529,
            1: 0.529,
            2: 0.500,
            3: 0.600,
            4: 0.500,
            5: 0.643,
            6: 0.643,
            7: 0.600,
            8: 0.429,
            9: 0.310,
        };
        for (auto n : sorted(this->K)) {
            assert(c[n] == pytest.approx(d[n], abs=1e-3));

    auto test_florentine_families_closeness() const -> void {
        c = nx.closeness_centrality(this->F);
        d = {
            "Acciaiuoli": 0.368,
            "Albizzi": 0.483,
            "Barbadori": 0.4375,
            "Bischeri": 0.400,
            "Castellani": 0.389,
            "Ginori": 0.333,
            "Guadagni": 0.467,
            "Lamberteschi": 0.326,
            "Medici": 0.560,
            "Pazzi": 0.286,
            "Peruzzi": 0.368,
            "Ridolfi": 0.500,
            "Salviati": 0.389,
            "Strozzi": 0.4375,
            "Tornabuoni": 0.483,
        };
        for (auto n : sorted(this->F)) {
            assert(c[n] == pytest.approx(d[n], abs=1e-3));

    auto test_les_miserables_closeness() const -> void {
        c = nx.closeness_centrality(this->LM);
        d = {
            "Napoleon": 0.302,
            "Myriel": 0.429,
            "MlleBaptistine": 0.413,
            "MmeMagloire": 0.413,
            "CountessDeLo": 0.302,
            "Geborand": 0.302,
            "Champtercier": 0.302,
            "Cravatte": 0.302,
            "Count": 0.302,
            "OldMan": 0.302,
            "Valjean": 0.644,
            "Labarre": 0.394,
            "Marguerite": 0.413,
            "MmeDeR": 0.394,
            "Isabeau": 0.394,
            "Gervais": 0.394,
            "Listolier": 0.341,
            "Tholomyes": 0.392,
            "Fameuil": 0.341,
            "Blacheville": 0.341,
            "Favourite": 0.341,
            "Dahlia": 0.341,
            "Zephine": 0.341,
            "Fantine": 0.461,
            "MmeThenardier": 0.461,
            "Thenardier": 0.517,
            "Cosette": 0.478,
            "Javert": 0.517,
            "Fauchelevent": 0.402,
            "Bamatabois": 0.427,
            "Perpetue": 0.318,
            "Simplice": 0.418,
            "Scaufflaire": 0.394,
            "Woman1": 0.396,
            "Judge": 0.404,
            "Champmathieu": 0.404,
            "Brevet": 0.404,
            "Chenildieu": 0.404,
            "Cochepaille": 0.404,
            "Pontmercy": 0.373,
            "Boulatruelle": 0.342,
            "Eponine": 0.396,
            "Anzelma": 0.352,
            "Woman2": 0.402,
            "MotherInnocent": 0.398,
            "Gribier": 0.288,
            "MmeBurgon": 0.344,
            "Jondrette": 0.257,
            "Gavroche": 0.514,
            "Gillenormand": 0.442,
            "Magnon": 0.335,
            "MlleGillenormand": 0.442,
            "MmePontmercy": 0.315,
            "MlleVaubois": 0.308,
            "LtGillenormand": 0.365,
            "Marius": 0.531,
            "BaronessT": 0.352,
            "Mabeuf": 0.396,
            "Enjolras": 0.481,
            "Combeferre": 0.392,
            "Prouvaire": 0.357,
            "Feuilly": 0.392,
            "Courfeyrac": 0.400,
            "Bahorel": 0.394,
            "Bossuet": 0.475,
            "Joly": 0.394,
            "Grantaire": 0.358,
            "MotherPlutarch": 0.285,
            "Gueulemer": 0.463,
            "Babet": 0.463,
            "Claquesous": 0.452,
            "Montparnasse": 0.458,
            "Toussaint": 0.402,
            "Child1": 0.342,
            "Child2": 0.342,
            "Brujon": 0.380,
            "MmeHucheloup": 0.353,
        };
        for (auto n : sorted(this->LM)) {
            assert(c[n] == pytest.approx(d[n], abs=1e-3));

    auto test_weighted_closeness() const -> void {
        edges = [
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
        XG = nx.Graph();
        XG.add_weighted_edges_from(edges);
        c = nx.closeness_centrality(XG, distance="weight");
        d = {"y": 0.200, "x": 0.286, "s": 0.138, "u": 0.235, "v": 0.200};
        for (auto n : sorted(XG)) {
            assert(c[n] == pytest.approx(d[n], abs=1e-3));

    //
    // Tests for incremental closeness centrality.
    //
    // @staticmethod
    auto pick_add_edge(g) -> void {
        u = nx.utils.arbitrary_element(g);
        possible_nodes = set(g.nodes());
        neighbors = list(g.neighbors(u)) + [u];
        possible_nodes.difference_update(neighbors);
        v = nx.utils.arbitrary_element(possible_nodes);
        return (u, v);

    // @staticmethod
    auto pick_remove_edge(g) -> void {
        u = nx.utils.arbitrary_element(g);
        possible_nodes = list(g.neighbors(u));
        v = nx.utils.arbitrary_element(possible_nodes);
        return (u, v);

    auto test_directed_raises() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            dir_G = nx.gn_graph(n=5);
            prev_cc = None
            edge = this->pick_add_edge(dir_G);
            insert = true;
            nx.incremental_closeness_centrality(dir_G, edge, prev_cc, insert);

    auto test_wrong_size_prev_cc_raises() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = this->undirected_G.copy();
            edge = this->pick_add_edge(G);
            insert = true;
            prev_cc = this->undirected_G_cc.copy();
            prev_cc.pop(0);
            nx.incremental_closeness_centrality(G, edge, prev_cc, insert);

    auto test_wrong_nodes_prev_cc_raises() const -> void {
        with pytest.raises(nx.NetworkXError):
            G = this->undirected_G.copy();
            edge = this->pick_add_edge(G);
            insert = true;
            prev_cc = this->undirected_G_cc.copy();
            num_nodes = prev_cc.size();
            prev_cc.pop(0);
            prev_cc[num_nodes] = 0.5
            nx.incremental_closeness_centrality(G, edge, prev_cc, insert);

    auto test_zero_centrality() const -> void {
        G = nx.path_graph(3);
        prev_cc = nx.closeness_centrality(G);
        edge = this->pick_remove_edge(G);
        test_cc = nx.incremental_closeness_centrality(G, edge, prev_cc, insertion=false);
        G.remove_edges_from([edge]);
        real_cc = nx.closeness_centrality(G);
        shared_items = set(test_cc.items()) & set(real_cc.items());
        assert(shared_items.size() == real_cc.size());
        assert(0 in test_cc.values());

    auto test_incremental() const -> void {
        // Check that incremental and regular give same output
        G = this->undirected_G.copy();
        prev_cc = None
        for (auto i : range(5)) {
            if (i % 2 == 0) {
                // Remove an edge
                insert = false;
                edge = this->pick_remove_edge(G);
            } else {
                // Add an edge
                insert = true;
                edge = this->pick_add_edge(G);

            // start = timeit.default_timer();
            test_cc = nx.incremental_closeness_centrality(G, edge, prev_cc, insert);
            // inc_elapsed = (timeit.default_timer() - start);
            // fmt::print(f"incremental time: {inc_elapsed}");

            if (insert) {
                G.add_edges_from([edge]);
            } else {
                G.remove_edges_from([edge]);

            // start = timeit.default_timer();
            real_cc = nx.closeness_centrality(G);
            // reg_elapsed = (timeit.default_timer() - start);
            // fmt::print(f"regular time: {reg_elapsed}");
            // Example output:
            // incremental time: 0.208
            // regular time: 0.276
            // incremental time: 0.00683
            // regular time: 0.260
            // incremental time: 0.0224
            // regular time: 0.278
            // incremental time: 0.00804
            // regular time: 0.208
            // incremental time: 0.00947
            // regular time: 0.188

            assert(set(test_cc.items()) == set(real_cc.items()));

            prev_cc = test_cc
