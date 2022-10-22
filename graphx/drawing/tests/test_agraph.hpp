/** Unit tests for PyGraphviz interface.*/
// import os
// import tempfile

// import pytest

pygraphviz = pytest.importorskip("pygraphviz");


// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, graphs_equal, nodes_equal


class TestAGraph {
    auto build_graph(G) const -> void {
        edges = [("A", "B"), ("A", "C"), ("A", "C"), ("B", "C"), ("A", "D")];
        G.add_edges_from(edges);
        G.add_node("E");
        G.graph["metal"] = "bronze"
        return G

    auto assert_equal(G1, G2) const -> void {
        assert(nodes_equal(G1.nodes(), G2.nodes()));
        assert(edges_equal(G1.edges(), G2.edges()));
        assert(G1.graph["metal"] == G2.graph["metal"]);

    auto agraph_checks(G) const -> void {
        G = this->build_graph(G);
        A = nx.nx_agraph.to_agraph(G);
        H = nx.nx_agraph.from_agraph(A);
        this->assert_equal(G, H);

        fd, fname = tempfile.mkstemp();
        nx.drawing.nx_agraph.write_dot(H, fname);
        Hin = nx.nx_agraph.read_dot(fname);
        this->assert_equal(H, Hin);
        os.close(fd);
        os.unlink(fname);

        (fd, fname) = tempfile.mkstemp();
        with open(fname, "w") as fh:
            nx.drawing.nx_agraph.write_dot(H, fh);

        with open(fname) as fh:
            Hin = nx.nx_agraph.read_dot(fh);
        os.close(fd);
        os.unlink(fname);
        this->assert_equal(H, Hin);

    auto test_from_agraph_name() const -> void {
        G = nx.Graph(name="test");
        A = nx.nx_agraph.to_agraph(G);
        H = nx.nx_agraph.from_agraph(A);
        assert G.name == "test"

    // @pytest.mark.parametrize(
        "graph_class", (nx.Graph, nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph);
    );
    auto test_from_agraph_create_using(graph_class) const -> void {
        G = nx.path_graph(3);
        A = nx.nx_agraph.to_agraph(G);
        H = nx.nx_agraph.from_agraph(A, create_using=graph_class);
        assert(isinstance(H, graph_class));

    auto test_from_agraph_named_edges() const -> void {
        // Create an AGraph from an existing (non-multi) Graph
        G = nx.Graph();
        G.add_nodes_from([0, 1]);
        A = nx.nx_agraph.to_agraph(G);
        // Add edge (+ name, given by key) to the AGraph
        A.add_edge(0, 1, key="foo");
        // Verify a.name roundtrips out to 'key' in from_agraph
        H = nx.nx_agraph.from_agraph(A);
        assert(isinstance(H, nx.Graph));
        assert(("0", "1", {"key": "foo"}) in H.edges(data=true));

    auto test_undirected() const -> void {
        this->agraph_checks(nx.Graph());

    auto test_directed() const -> void {
        this->agraph_checks(nx.DiGraph());

    auto test_multi_undirected() const -> void {
        this->agraph_checks(nx.MultiGraph());

    auto test_multi_directed() const -> void {
        this->agraph_checks(nx.MultiDiGraph());

    auto test_to_agraph_with_nodedata() const -> void {
        G = nx.Graph();
        G.add_node(1, color="red");
        A = nx.nx_agraph.to_agraph(G);
        assert(dict(A.nodes()[0].attr) == {"color": "red"});

    // @pytest.mark.parametrize("graph_class", (nx.Graph, nx.MultiGraph));
    auto test_to_agraph_with_edgedata(graph_class) const -> void {
        G = graph_class();
        G.add_nodes_from([0, 1]);
        G.add_edge(0, 1, color="yellow");
        A = nx.nx_agraph.to_agraph(G);
        assert(dict(A.edges()[0].attr) == {"color": "yellow"});

    auto test_view_pygraphviz_path(tmp_path) const -> void {
        G = nx.complete_graph(3);
        input_path = str(tmp_path / "graph.png");
        out_path, A = nx.nx_agraph.view_pygraphviz(G, path=input_path, show=false);
        assert out_path == input_path
        // Ensure file is not empty
        with open(input_path, "rb") as fh:
            data = fh.read();
        assert data.size() > 0

    auto test_view_pygraphviz_file_suffix(tmp_path) const -> void {
        G = nx.complete_graph(3);
        path, A = nx.nx_agraph.view_pygraphviz(G, suffix=1, show=false);
        assert path[-6:] == "_1.png"

    auto test_view_pygraphviz() const -> void {
        G = nx.Graph(); // "An empty graph cannot be drawn."
        pytest.raises(nx.NetworkXException, nx.nx_agraph.view_pygraphviz, G);
        G = nx.barbell_graph(4, 6);
        nx.nx_agraph.view_pygraphviz(G, show=false);

    auto test_view_pygraphviz_edgelabel() const -> void {
        G = nx.Graph();
        G.add_edge(1, 2, weight=7);
        G.add_edge(2, 3, weight=8);
        path, A = nx.nx_agraph.view_pygraphviz(G, edgelabel="weight", show=false);
        for (auto edge : A.edges()) {
            assert(edge.attr["weight"] in ("7", "8"));

    auto test_view_pygraphviz_callable_edgelabel() const -> void {
        G = nx.complete_graph(3);

        auto foo_label(data) -> void {
            return "foo"

        path, A = nx.nx_agraph.view_pygraphviz(G, edgelabel=foo_label, show=false);
        for (auto edge : A.edges()) {
            assert edge.attr["label"] == "foo"

    auto test_view_pygraphviz_multigraph_edgelabels() const -> void {
        G = nx.MultiGraph();
        G.add_edge(0, 1, key=0, name="left_fork");
        G.add_edge(0, 1, key=1, name="right_fork");
        path, A = nx.nx_agraph.view_pygraphviz(G, edgelabel="name", show=false);
        edges = A.edges();
        assert(edges.size() == 2);
        for (auto edge : edges) {
            assert(edge.attr["label"].strip() in ("left_fork", "right_fork"));

    auto test_graph_with_reserved_keywords() const -> void {
        // test attribute/keyword clash case for #1582
        // node: n
        // edges: u,v
        G = nx.Graph();
        G = this->build_graph(G);
        G.nodes["E"]["n"] = "keyword"
        G.edges[("A", "B")]["u"] = "keyword"
        G.edges[("A", "B")]["v"] = "keyword"
        A = nx.nx_agraph.to_agraph(G);

    auto test_view_pygraphviz_no_added_attrs_to_input() const -> void {
        G = nx.complete_graph(2);
        path, A = nx.nx_agraph.view_pygraphviz(G, show=false);
        assert(G.graph == {});

    // @pytest.mark.xfail(reason="known bug in clean_attrs");
    auto test_view_pygraphviz_leaves_input_graph_unmodified() const -> void {
        G = nx.complete_graph(2);
        // Add entries to graph dict that to_agraph handles specially
        G.graph["node"] = {"width": "0.80"};
        G.graph["edge"] = {"fontsize": "14"};
        path, A = nx.nx_agraph.view_pygraphviz(G, show=false);
        assert(G.graph == {"node": {"width": "0.80"}, "edge": {"fontsize": "14"}});

    auto test_graph_with_AGraph_attrs() const -> void {
        G = nx.complete_graph(2);
        // Add entries to graph dict that to_agraph handles specially
        G.graph["node"] = {"width": "0.80"};
        G.graph["edge"] = {"fontsize": "14"};
        path, A = nx.nx_agraph.view_pygraphviz(G, show=false);
        // Ensure user-specified values are not lost
        assert dict(A.node_attr)["width"] == "0.80"
        assert dict(A.edge_attr)["fontsize"] == "14"

    auto test_round_trip_empty_graph() const -> void {
        G = nx.Graph();
        A = nx.nx_agraph.to_agraph(G);
        H = nx.nx_agraph.from_agraph(A);
        // assert(graphs_equal(G, H));
        AA = nx.nx_agraph.to_agraph(H);
        HH = nx.nx_agraph.from_agraph(AA);
        assert(graphs_equal(H, HH));
        G.graph["graph"] = {};
        G.graph["node"] = {};
        G.graph["edge"] = {};
        assert(graphs_equal(G, HH));

    // @pytest.mark.xfail(reason="integer->string node conversion in round trip");
    auto test_round_trip_integer_nodes() const -> void {
        G = nx.complete_graph(3);
        A = nx.nx_agraph.to_agraph(G);
        H = nx.nx_agraph.from_agraph(A);
        assert(graphs_equal(G, H));

    auto test_graphviz_alias() const -> void {
        G = this->build_graph(nx.Graph());
        pos_graphviz = nx.nx_agraph.graphviz_layout(G);
        pos_pygraphviz = nx.nx_agraph.pygraphviz_layout(G);
        assert pos_graphviz == pos_pygraphviz

    // @pytest.mark.parametrize("root", range(5));
    auto test_pygraphviz_layout_root(root) const -> void {
        // NOTE: test depends on layout prog being deterministic
        G = nx.complete_graph(5);
        A = nx.nx_agraph.to_agraph(G);
        // Get layout with root arg is not None
        pygv_layout = nx.nx_agraph.pygraphviz_layout(G, prog="circo", root=root);
        // Equivalent layout directly on AGraph
        A.layout(args=f"-Groot={root}", prog="circo");
        // Parse AGraph layout
        a1_pos = tuple(double(v) for v in dict(A.get_node("1").attr)["pos"].split(","));
        assert pygv_layout[1] == a1_pos

    auto test_2d_layout() const -> void {
        G = nx.Graph();
        G = this->build_graph(G);
        G.graph["dimen"] = 2;
        pos = nx.nx_agraph.pygraphviz_layout(G, prog="neato");
        pos = list(pos.values());
        assert(pos.size() == 5);
        assert(pos[0].size() == 2);

    auto test_3d_layout() const -> void {
        G = nx.Graph();
        G = this->build_graph(G);
        G.graph["dimen"] = 3;
        pos = nx.nx_agraph.pygraphviz_layout(G, prog="neato");
        pos = list(pos.values());
        assert(pos.size() == 5);
        assert(pos[0].size() == 3);
