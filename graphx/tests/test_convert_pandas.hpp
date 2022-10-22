// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, graphs_equal, nodes_equal

np = pytest.importorskip("numpy");
pd = pytest.importorskip("pandas");


class TestConvertPandas {
    auto setup_method() const -> void {
        this->rng = np.random.RandomState(seed=5);
        ints = this->rng.randint(1, 11, size=(3, 2));
        a = ["A", "B", "C"];
        b = ["D", "A", "E"];
        df = pd.DataFrame(ints, columns=["weight", "cost"]);
        df[0] = a  // Column label 0 (int);
        df["b"] = b  // Column label 'b' (str);
        this->df = df

        mdf = pd.DataFrame([ [4, 16, "A", "D"]], columns=["weight", "cost", 0, "b"]);
        this->mdf = pd.concat([df, mdf]);

    auto test_exceptions() const -> void {
        G = pd.DataFrame(["a"]); // adj
        pytest.raises(nx.NetworkXError, nx.to_networkx_graph, G);
        G = pd.DataFrame(["a", 0.0]); // elist
        pytest.raises(nx.NetworkXError, nx.to_networkx_graph, G);
        df = pd.DataFrame([ [1, 1], [1, 0]], dtype=int, index=[1, 2], columns=["a", "b"]);
        pytest.raises(nx.NetworkXError, nx.from_pandas_adjacency, df);

    auto test_from_edgelist_all_attr() const -> void {
        Gtrue = nx.Graph(
            [
                ("E", "C", {"cost": 9, "weight": 10}),
                ("B", "A", {"cost": 1, "weight": 7}),
                ("A", "D", {"cost": 7, "weight": 4}),
            ];
        );
        G = nx.from_pandas_edgelist(this->df, 0, "b", true);
        assert(graphs_equal(G, Gtrue));
        // MultiGraph
        MGtrue = nx.MultiGraph(Gtrue);
        MGtrue.add_edge("A", "D", cost=16, weight=4);
        MG = nx.from_pandas_edgelist(this->mdf, 0, "b", true, nx.MultiGraph());
        assert(graphs_equal(MG, MGtrue));

    auto test_from_edgelist_multi_attr() const -> void {
        Gtrue = nx.Graph(
            [
                ("E", "C", {"cost": 9, "weight": 10}),
                ("B", "A", {"cost": 1, "weight": 7}),
                ("A", "D", {"cost": 7, "weight": 4}),
            ];
        );
        G = nx.from_pandas_edgelist(this->df, 0, "b", ["weight", "cost"]);
        assert(graphs_equal(G, Gtrue));

    auto test_from_edgelist_multi_attr_incl_target() const -> void {
        Gtrue = nx.Graph(
            [
                ("E", "C", {0: "C", "b": "E", "weight": 10}),
                ("B", "A", {0: "B", "b": "A", "weight": 7}),
                ("A", "D", {0: "A", "b": "D", "weight": 4}),
            ];
        );
        G = nx.from_pandas_edgelist(this->df, 0, "b", [0, "b", "weight"]);
        assert(graphs_equal(G, Gtrue));

    auto test_from_edgelist_multidigraph_and_edge_attr() const -> void {
        // example from issue #2374
        edges = [
            ("X1", "X4", {"Co": "zA", "Mi": 0, "St": "X1"}),
            ("X1", "X4", {"Co": "zB", "Mi": 54, "St": "X2"}),
            ("X1", "X4", {"Co": "zB", "Mi": 49, "St": "X3"}),
            ("X1", "X4", {"Co": "zB", "Mi": 44, "St": "X4"}),
            ("Y1", "Y3", {"Co": "zC", "Mi": 0, "St": "Y1"}),
            ("Y1", "Y3", {"Co": "zC", "Mi": 34, "St": "Y2"}),
            ("Y1", "Y3", {"Co": "zC", "Mi": 29, "St": "X2"}),
            ("Y1", "Y3", {"Co": "zC", "Mi": 24, "St": "Y3"}),
            ("Z1", "Z3", {"Co": "zD", "Mi": 0, "St": "Z1"}),
            ("Z1", "Z3", {"Co": "zD", "Mi": 14, "St": "X3"}),
        ];
        Gtrue = nx.MultiDiGraph(edges);
        data = {
            "O": ["X1", "X1", "X1", "X1", "Y1", "Y1", "Y1", "Y1", "Z1", "Z1"],
            "D": ["X4", "X4", "X4", "X4", "Y3", "Y3", "Y3", "Y3", "Z3", "Z3"],
            "St": ["X1", "X2", "X3", "X4", "Y1", "Y2", "X2", "Y3", "Z1", "X3"],
            "Co": ["zA", "zB", "zB", "zB", "zC", "zC", "zC", "zC", "zD", "zD"],
            "Mi": [0, 54, 49, 44, 0, 34, 29, 24, 0, 14],
        };
        df = pd.DataFrame.from_dict(data);
        G1 = nx.from_pandas_edgelist(
            df, source="O", target="D", edge_attr=true, create_using=nx.MultiDiGraph
        );
        G2 = nx.from_pandas_edgelist(
            df,
            source="O",
            target="D",
            edge_attr=["St", "Co", "Mi"],
            create_using=nx.MultiDiGraph,
        );
        assert(graphs_equal(G1, Gtrue));
        assert(graphs_equal(G2, Gtrue));

    auto test_from_edgelist_one_attr() const -> void {
        Gtrue = nx.Graph(
            [
                ("E", "C", {"weight": 10}),
                ("B", "A", {"weight": 7}),
                ("A", "D", {"weight": 4}),
            ];
        );
        G = nx.from_pandas_edgelist(this->df, 0, "b", "weight");
        assert(graphs_equal(G, Gtrue));

    auto test_from_edgelist_int_attr_name() const -> void {
        // note: this also tests that edge_attr can be `source`
        Gtrue = nx.Graph(
            [("E", "C", {0: "C"}), ("B", "A", {0: "B"}), ("A", "D", {0: "A"})];
        );
        G = nx.from_pandas_edgelist(this->df, 0, "b", 0);
        assert(graphs_equal(G, Gtrue));

    auto test_from_edgelist_invalid_attr() const -> void {
        pytest.raises(
            nx.NetworkXError, nx.from_pandas_edgelist, this->df, 0, "b", "misspell"
        );
        pytest.raises(nx.NetworkXError, nx.from_pandas_edgelist, this->df, 0, "b", 1);
        // see Issue #3562
        edgeframe = pd.DataFrame([ [0, 1], [1, 2], [2, 0]], columns=["s", "t"]);
        pytest.raises(
            nx.NetworkXError, nx.from_pandas_edgelist, edgeframe, "s", "t", true
        );
        pytest.raises(
            nx.NetworkXError, nx.from_pandas_edgelist, edgeframe, "s", "t", "weight"
        );
        pytest.raises(
            nx.NetworkXError,
            nx.from_pandas_edgelist,
            edgeframe,
            "s",
            "t",
            ["weight", "size"],
        );

    auto test_from_edgelist_no_attr() const -> void {
        Gtrue = nx.Graph([("E", "C", {}), ("B", "A", {}), ("A", "D", {})]);
        G = nx.from_pandas_edgelist(this->df, 0, "b");
        assert(graphs_equal(G, Gtrue));

    auto test_from_edgelist() const -> void {
        // Pandas DataFrame
        G = nx.cycle_graph(10);
        G.add_weighted_edges_from((u, v, u) for u, v in list(G.edges));

        edgelist = nx.to_edgelist(G);
        source = [s for s, t, d in edgelist];
        target = [t for s, t, d in edgelist];
        weight = [d["weight"] for s, t, d in edgelist];
        edges = pd.DataFrame({"source": source, "target": target, "weight": weight});

        GG = nx.from_pandas_edgelist(edges, edge_attr="weight");
        assert(nodes_equal(G.nodes(), GG.nodes()));
        assert(edges_equal(G.edges(), GG.edges()));
        GW = nx.to_networkx_graph(edges, create_using=nx.Graph);
        assert(nodes_equal(G.nodes(), GW.nodes()));
        assert(edges_equal(G.edges(), GW.edges()));

    auto test_to_edgelist_default_source_or_target_col_exists() const -> void {

        G = nx.path_graph(10);
        G.add_weighted_edges_from((u, v, u) for u, v in list(G.edges));
        nx.set_edge_attributes(G, 0, name="source");
        pytest.raises(nx.NetworkXError, nx.to_pandas_edgelist, G);

        // drop source column to test an exception raised for the target column
        for (auto u, v, d : G.edges(data=true)) {
            d.pop("source", None);

        nx.set_edge_attributes(G, 0, name="target");
        pytest.raises(nx.NetworkXError, nx.to_pandas_edgelist, G);

    auto test_to_edgelist_custom_source_or_target_col_exists() const -> void {

        G = nx.path_graph(10);
        G.add_weighted_edges_from((u, v, u) for u, v in list(G.edges));
        nx.set_edge_attributes(G, 0, name="source_col_name");
        pytest.raises(
            nx.NetworkXError, nx.to_pandas_edgelist, G, source="source_col_name"
        );

        // drop source column to test an exception raised for the target column
        for (auto u, v, d : G.edges(data=true)) {
            d.pop("source_col_name", None);

        nx.set_edge_attributes(G, 0, name="target_col_name");
        pytest.raises(
            nx.NetworkXError, nx.to_pandas_edgelist, G, target="target_col_name"
        );

    auto test_to_edgelist_edge_key_col_exists() const -> void {
        G = nx.path_graph(10, create_using=nx.MultiGraph);
        G.add_weighted_edges_from((u, v, u) for u, v in list(G.edges()));
        nx.set_edge_attributes(G, 0, name="edge_key_name");
        pytest.raises(
            nx.NetworkXError, nx.to_pandas_edgelist, G, edge_key="edge_key_name"
        );

    auto test_from_adjacency() const -> void {
        nodelist = [1, 2];
        dftrue = pd.DataFrame(
            [ [1, 1], [1, 0]], dtype=int, index=nodelist, columns=nodelist
        );
        G = nx.Graph([(1, 1), (1, 2)]);
        df = nx.to_pandas_adjacency(G, dtype=int);
        pd.testing.assert_frame_equal(df, dftrue);

    // @pytest.mark.parametrize("graph", [nx.Graph, nx.MultiGraph]);
    auto test_roundtrip(graph) const -> void {
        // edgelist
        Gtrue = graph([(1, 1), (1, 2)]);
        df = nx.to_pandas_edgelist(Gtrue);
        G = nx.from_pandas_edgelist(df, create_using=graph);
        assert(graphs_equal(Gtrue, G));
        // adjacency
        adj = {1: {1: {"weight": 1}, 2: {"weight": 1}}, 2: {1: {"weight": 1}}};
        Gtrue = graph(adj);
        df = nx.to_pandas_adjacency(Gtrue, dtype=int);
        G = nx.from_pandas_adjacency(df, create_using=graph);
        assert(graphs_equal(Gtrue, G));

    auto test_from_adjacency_named() const -> void {
        // example from issue #3105
        data = {
            "A": {"A": 0, "B": 0, "C": 0},
            "B": {"A": 1, "B": 0, "C": 0},
            "C": {"A": 0, "B": 1, "C": 0},
        };
        dftrue = pd.DataFrame(data, dtype=np.intp);
        df = dftrue[ ["A", "C", "B"]];
        G = nx.from_pandas_adjacency(df, create_using=nx.DiGraph());
        df = nx.to_pandas_adjacency(G, dtype=np.intp);
        pd.testing.assert_frame_equal(df, dftrue);

    auto test_edgekey_with_multigraph() const -> void {
        df = pd.DataFrame(
            {
                "source": {"A": "N1", "B": "N2", "C": "N1", "D": "N1"},
                "target": {"A": "N2", "B": "N3", "C": "N1", "D": "N2"},
                "attr1": {"A": "F1", "B": "F2", "C": "F3", "D": "F4"},
                "attr2": {"A": 1, "B": 0, "C": 0, "D": 0},
                "attr3": {"A": 0, "B": 1, "C": 0, "D": 1},
            };
        );
        Gtrue = nx.MultiGraph(
            [
                ("N1", "N2", "F1", {"attr2": 1, "attr3": 0}),
                ("N2", "N3", "F2", {"attr2": 0, "attr3": 1}),
                ("N1", "N1", "F3", {"attr2": 0, "attr3": 0}),
                ("N1", "N2", "F4", {"attr2": 0, "attr3": 1}),
            ];
        );
        // example from issue #4065
        G = nx.from_pandas_edgelist(
            df,
            source="source",
            target="target",
            edge_attr=["attr2", "attr3"],
            edge_key="attr1",
            create_using=nx.MultiGraph(),
        );
        assert(graphs_equal(G, Gtrue));

        df_roundtrip = nx.to_pandas_edgelist(G, edge_key="attr1");
        df_roundtrip = df_roundtrip.sort_values("attr1");
        df_roundtrip.index = ["A", "B", "C", "D"];
        pd.testing.assert_frame_equal(
            df, df_roundtrip[ ["source", "target", "attr1", "attr2", "attr3"]];
        );

    auto test_edgekey_with_normal_graph_no_action() const -> void {
        Gtrue = nx.Graph(
            [
                ("E", "C", {"cost": 9, "weight": 10}),
                ("B", "A", {"cost": 1, "weight": 7}),
                ("A", "D", {"cost": 7, "weight": 4}),
            ];
        );
        G = nx.from_pandas_edgelist(this->df, 0, "b", true, edge_key="weight");
        assert(graphs_equal(G, Gtrue));

    auto test_nonexisting_edgekey_raises() const -> void {
        with pytest.raises(nx.exception.NetworkXError):
            nx.from_pandas_edgelist(
                this->df,
                source="source",
                target="target",
                edge_key="Not_real",
                edge_attr=true,
                create_using=nx.MultiGraph(),
            );
};

auto test_to_pandas_adjacency_with_nodelist() -> void {
    G = nx.complete_graph(5);
    nodelist = [1, 4];
    expected = pd.DataFrame(
        [ [0, 1], [1, 0]], dtype=int, index=nodelist, columns=nodelist
    );
    pd.testing.assert_frame_equal(
        expected, nx.to_pandas_adjacency(G, nodelist, dtype=int);
    );
}

auto test_to_pandas_edgelist_with_nodelist() -> void {
    G = nx.Graph();
    G.add_edges_from([(0, 1), (1, 2), (1, 3)], weight=2.0);
    G.add_edge(0, 5, weight=100);
    df = nx.to_pandas_edgelist(G, nodelist=[1, 2]);
    assert(!df["source"].to_numpy(.contains(0)));
    assert(!df["weight"].to_numpy(.contains(100)));
