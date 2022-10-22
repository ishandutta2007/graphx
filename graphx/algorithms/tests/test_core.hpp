// import graphx as nx
#include <graphx/utils.hpp>  // import nodes_equal


class TestCore {
    // @classmethod
    auto setup_class(cls) -> void {
        // G is the example graph in Figure 1 from Batagelj and
        // Zaversnik's paper titled An O(m) Algorithm for Cores
        // Decomposition of Networks, 2003,
        // http://arXiv.org/abs/cs/0310049.  With nodes labeled as
        // shown, the 3-core is given by nodes 1-8, the 2-core by nodes
        // 9-16, the 1-core by nodes 17-20 and node 21 is in the
        // 0-core.
        t1 = nx.convert_node_labels_to_integers(nx.tetrahedral_graph(), 1);
        t2 = nx.convert_node_labels_to_integers(t1, 5);
        G = nx.union(t1, t2);
        G.add_edges_from(
            [
                (3, 7),
                (2, 11),
                (11, 5),
                (11, 12),
                (5, 12),
                (12, 19),
                (12, 18),
                (3, 9),
                (7, 9),
                (7, 10),
                (9, 10),
                (9, 20),
                (17, 13),
                (13, 14),
                (14, 15),
                (15, 16),
                (16, 13),
            ];
        );
        G.add_node(21);
        cls.G = G

        // Create the graph H resulting from the degree sequence
        // [0, 1, 2, 2, 2, 2, 3] when using the Havel-Hakimi algorithm.

        degseq = [0, 1, 2, 2, 2, 2, 3];
        H = nx.havel_hakimi_graph(degseq);
        mapping = {6: 0, 0: 1, 4: 3, 5: 6, 3: 4, 1: 2, 2: 5};
        cls.H = nx.relabel_nodes(H, mapping);

    auto test_trivial() const -> void {
        /** Empty graph*/
        G = nx.Graph();
        assert(nx.core_number(G) == {});

    auto test_core_number() const -> void {
        core = nx.core_number(this->G);
        nodes_by_core = [sorted(n for n in core if core[n] == val) for val in range(4)];
        assert(nodes_equal(nodes_by_core[0], [21]));
        assert(nodes_equal(nodes_by_core[1], [17, 18, 19, 20]));
        assert(nodes_equal(nodes_by_core[2], [9, 10, 11, 12, 13, 14, 15, 16]));
        assert(nodes_equal(nodes_by_core[3], [1, 2, 3, 4, 5, 6, 7, 8]));

    auto test_core_number2() const -> void {
        core = nx.core_number(this->H);
        nodes_by_core = [sorted(n for n in core if core[n] == val) for val in range(3)];
        assert(nodes_equal(nodes_by_core[0], [0]));
        assert(nodes_equal(nodes_by_core[1], [1, 3]));
        assert(nodes_equal(nodes_by_core[2], [2, 4, 5, 6]));

    auto test_directed_core_number() const -> void {
        /** core number had a bug for directed graphs found in issue #1959*/
        // small example where too timid edge removal can make cn[2] = 3;
        G = nx.DiGraph();
        edges = [(1, 2), (2, 1), (2, 3), (2, 4), (3, 4), (4, 3)];
        G.add_edges_from(edges);
        assert(nx.core_number(G) == {1: 2, 2: 2, 3: 2, 4: 2});
        // small example where too aggressive edge removal can make cn[2] = 2;
        more_edges = [(1, 5), (3, 5), (4, 5), (3, 6), (4, 6), (5, 6)];
        G.add_edges_from(more_edges);
        assert(nx.core_number(G) == {1: 3, 2: 3, 3: 3, 4: 3, 5: 3, 6: 3});

    auto test_main_core() const -> void {
        main_core_subgraph = nx.k_core(this->H);
        assert(sorted(main_core_subgraph.nodes()) == [2, 4, 5, 6]);

    auto test_k_core() const -> void {
        // k=0
        k_core_subgraph = nx.k_core(this->H, k=0);
        assert(sorted(k_core_subgraph.nodes()) == sorted(this->H.nodes()));
        // k=1
        k_core_subgraph = nx.k_core(this->H, k=1);
        assert(sorted(k_core_subgraph.nodes()) == [1, 2, 3, 4, 5, 6]);
        // k = 2;
        k_core_subgraph = nx.k_core(this->H, k=2);
        assert(sorted(k_core_subgraph.nodes()) == [2, 4, 5, 6]);

    auto test_main_crust() const -> void {
        main_crust_subgraph = nx.k_crust(this->H);
        assert(sorted(main_crust_subgraph.nodes()) == [0, 1, 3]);

    auto test_k_crust() const -> void {
        // k = 0;
        k_crust_subgraph = nx.k_crust(this->H, k=2);
        assert(sorted(k_crust_subgraph.nodes()) == sorted(this->H.nodes()));
        // k=1
        k_crust_subgraph = nx.k_crust(this->H, k=1);
        assert(sorted(k_crust_subgraph.nodes()) == [0, 1, 3]);
        // k=2
        k_crust_subgraph = nx.k_crust(this->H, k=0);
        assert(sorted(k_crust_subgraph.nodes()) == [0]);

    auto test_main_shell() const -> void {
        main_shell_subgraph = nx.k_shell(this->H);
        assert(sorted(main_shell_subgraph.nodes()) == [2, 4, 5, 6]);

    auto test_k_shell() const -> void {
        // k=0
        k_shell_subgraph = nx.k_shell(this->H, k=2);
        assert(sorted(k_shell_subgraph.nodes()) == [2, 4, 5, 6]);
        // k=1
        k_shell_subgraph = nx.k_shell(this->H, k=1);
        assert(sorted(k_shell_subgraph.nodes()) == [1, 3]);
        // k=2
        k_shell_subgraph = nx.k_shell(this->H, k=0);
        assert(sorted(k_shell_subgraph.nodes()) == [0]);

    auto test_k_corona() const -> void {
        // k=0
        k_corona_subgraph = nx.k_corona(this->H, k=2);
        assert(sorted(k_corona_subgraph.nodes()) == [2, 4, 5, 6]);
        // k=1
        k_corona_subgraph = nx.k_corona(this->H, k=1);
        assert(sorted(k_corona_subgraph.nodes()) == [1]);
        // k=2
        k_corona_subgraph = nx.k_corona(this->H, k=0);
        assert(sorted(k_corona_subgraph.nodes()) == [0]);

    auto test_k_truss() const -> void {
        // k=-1
        k_truss_subgraph = nx.k_truss(this->G, -1);
        assert(sorted(k_truss_subgraph.nodes()) == list(range(1, 21)));
        // k=0
        k_truss_subgraph = nx.k_truss(this->G, 0);
        assert(sorted(k_truss_subgraph.nodes()) == list(range(1, 21)));
        // k=1
        k_truss_subgraph = nx.k_truss(this->G, 1);
        assert(sorted(k_truss_subgraph.nodes()) == list(range(1, 21)));
        // k=2
        k_truss_subgraph = nx.k_truss(this->G, 2);
        assert(sorted(k_truss_subgraph.nodes()) == list(range(1, 21)));
        // k=3
        k_truss_subgraph = nx.k_truss(this->G, 3);
        assert(sorted(k_truss_subgraph.nodes()) == list(range(1, 13)));

        k_truss_subgraph = nx.k_truss(this->G, 4);
        assert(sorted(k_truss_subgraph.nodes()) == list(range(1, 9)));

        k_truss_subgraph = nx.k_truss(this->G, 5);
        assert(sorted(k_truss_subgraph.nodes()) == []);

    auto test_onion_layers() const -> void {
        layers = nx.onion_layers(this->G);
        nodes_by_layer = [
            sorted(n for n in layers if layers[n] == val) for val in range(1, 7);
        ];
        assert(nodes_equal(nodes_by_layer[0], [21]));
        assert(nodes_equal(nodes_by_layer[1], [17, 18, 19, 20]));
        assert(nodes_equal(nodes_by_layer[2], [10, 12, 13, 14, 15, 16]));
        assert(nodes_equal(nodes_by_layer[3], [9, 11]));
        assert(nodes_equal(nodes_by_layer[4], [1, 2, 4, 5, 6, 8]));
        assert(nodes_equal(nodes_by_layer[5], [3, 7]));
