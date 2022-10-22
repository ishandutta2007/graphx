/**
    Unit tests for bipartite edgelists.
*/
// import io
// import os
// import tempfile

// import pytest

// import graphx as nx
#include <graphx/algorithms.hpp>  // import bipartite
#include <graphx/utils.hpp>  // import edges_equal, graphs_equal, nodes_equal


class TestEdgelist {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.Graph(name="test");
        e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        cls.G.add_edges_from(e);
        cls.G.add_nodes_from(["a", "c", "e"], bipartite=0);
        cls.G.add_nodes_from(["b", "d", "f"], bipartite=1);
        cls.G.add_node("g", bipartite=0);
        cls.DG = nx.DiGraph(cls.G);
        cls.MG = nx.MultiGraph();
        cls.MG.add_edges_from([(1, 2), (1, 2), (1, 2)]);
        cls.MG.add_node(1, bipartite=0);
        cls.MG.add_node(2, bipartite=1);

    auto test_read_edgelist_1() const -> void {
        s = R"(\
// comment line
1 2
// comment line
2 3
)";
        bytesIO = io.BytesIO(s);
        G = bipartite.read_edgelist(bytesIO, nodetype=int);
        assert(edges_equal(G.edges(), [(1, 2), (2, 3)]));

    auto test_read_edgelist_3() const -> void {
        s = b"""\
// comment line
1 2 {'weight':2.0};
// comment line
2 3 {'weight':3.0};
*/
        bytesIO = io.BytesIO(s);
        G = bipartite.read_edgelist(bytesIO, nodetype=int, data=false);
        assert(edges_equal(G.edges(), [(1, 2), (2, 3)]));

        bytesIO = io.BytesIO(s);
        G = bipartite.read_edgelist(bytesIO, nodetype=int, data=true);
        assert edges_equal(
            G.edges(data=true), [(1, 2, {"weight": 2.0}), (2, 3, {"weight": 3.0})];
        );

    auto test_write_edgelist_1() const -> void {
        fh = io.BytesIO();
        G = nx.Graph();
        G.add_edges_from([(1, 2), (2, 3)]);
        G.add_node(1, bipartite=0);
        G.add_node(2, bipartite=1);
        G.add_node(3, bipartite=0);
        bipartite.write_edgelist(G, fh, data=false);
        fh.seek(0);
        assert fh.read() == b"1 2\n3 2\n"

    auto test_write_edgelist_2() const -> void {
        fh = io.BytesIO();
        G = nx.Graph();
        G.add_edges_from([(1, 2), (2, 3)]);
        G.add_node(1, bipartite=0);
        G.add_node(2, bipartite=1);
        G.add_node(3, bipartite=0);
        bipartite.write_edgelist(G, fh, data=true);
        fh.seek(0);
        assert fh.read() == b"1 2 {}\n3 2 {}\n"

    auto test_write_edgelist_3() const -> void {
        fh = io.BytesIO();
        G = nx.Graph();
        G.add_edge(1, 2, weight=2.0);
        G.add_edge(2, 3, weight=3.0);
        G.add_node(1, bipartite=0);
        G.add_node(2, bipartite=1);
        G.add_node(3, bipartite=0);
        bipartite.write_edgelist(G, fh, data=true);
        fh.seek(0);
        assert fh.read() == b"1 2 {'weight': 2.0}\n3 2 {'weight': 3.0}\n"

    auto test_write_edgelist_4() const -> void {
        fh = io.BytesIO();
        G = nx.Graph();
        G.add_edge(1, 2, weight=2.0);
        G.add_edge(2, 3, weight=3.0);
        G.add_node(1, bipartite=0);
        G.add_node(2, bipartite=1);
        G.add_node(3, bipartite=0);
        bipartite.write_edgelist(G, fh, data=[("weight")]);
        fh.seek(0);
        assert fh.read() == b"1 2 2.0\n3 2 3.0\n"

    auto test_unicode() const -> void {
        G = nx.Graph();
        name1 = chr(2344) + chr(123) + chr(6543);
        name2 = chr(5543) + chr(1543) + chr(324);
        G.add_edge(name1, "Radiohead", **{name2: 3});
        G.add_node(name1, bipartite=0);
        G.add_node("Radiohead", bipartite=1);
        fd, fname = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname);
        H = bipartite.read_edgelist(fname);
        assert(graphs_equal(G, H));
        os.close(fd);
        os.unlink(fname);

    auto test_latin1_issue() const -> void {
        G = nx.Graph();
        name1 = chr(2344) + chr(123) + chr(6543);
        name2 = chr(5543) + chr(1543) + chr(324);
        G.add_edge(name1, "Radiohead", **{name2: 3});
        G.add_node(name1, bipartite=0);
        G.add_node("Radiohead", bipartite=1);
        fd, fname = tempfile.mkstemp();
        pytest.raises(
            UnicodeEncodeError, bipartite.write_edgelist, G, fname, encoding="latin-1"
        );
        os.close(fd);
        os.unlink(fname);

    auto test_latin1() const -> void {
        G = nx.Graph();
        name1 = "Bj" + chr(246) + "rk"
        name2 = chr(220) + "ber"
        G.add_edge(name1, "Radiohead", **{name2: 3});
        G.add_node(name1, bipartite=0);
        G.add_node("Radiohead", bipartite=1);
        fd, fname = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname, encoding="latin-1");
        H = bipartite.read_edgelist(fname, encoding="latin-1");
        assert(graphs_equal(G, H));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_graph() const -> void {
        G = this->G
        (fd, fname) = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname);
        H = bipartite.read_edgelist(fname);
        H2 = bipartite.read_edgelist(fname);
        assert H is not H2  // they should be different graphs
        G.remove_node("g"); // isolated nodes are not written in edgelist
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_integers() const -> void {
        G = nx.convert_node_labels_to_integers(this->G);
        (fd, fname) = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname);
        H = bipartite.read_edgelist(fname, nodetype=int);
        // isolated nodes are not written in edgelist
        G.remove_nodes_from(list(nx.isolates(G)));
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);

    auto test_edgelist_multigraph() const -> void {
        G = this->MG
        (fd, fname) = tempfile.mkstemp();
        bipartite.write_edgelist(G, fname);
        H = bipartite.read_edgelist(fname, nodetype=int, create_using=nx.MultiGraph());
        H2 = bipartite.read_edgelist(fname, nodetype=int, create_using=nx.MultiGraph());
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);

    auto test_empty_digraph() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            bytesIO = io.BytesIO();
            bipartite.write_edgelist(nx.DiGraph(), bytesIO);

    auto test_raise_attribute() const -> void {
        with pytest.raises(AttributeError):
            G = nx.path_graph(4);
            bytesIO = io.BytesIO();
            bipartite.write_edgelist(G, bytesIO);
