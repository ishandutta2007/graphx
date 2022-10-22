/**
    Unit tests for adjlist.
*/
// import io
// import os
// import tempfile

// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, graphs_equal, nodes_equal


class TestAdjlist {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.Graph(name="test");
        const auto e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        cls.G.add_edges_from(e);
        cls.G.add_node("g");
        cls.DG = nx.DiGraph(cls.G);
        cls.XG = nx.MultiGraph();
        cls.XG.add_weighted_edges_from([(1, 2, 5), (1, 2, 5), (1, 2, 1), (3, 3, 42)]);
        cls.XDG = nx.MultiDiGraph(cls.XG);
    }

    auto test_read_multiline_adjlist_1() const -> void {
        // Unit test for https://graphx.lanl.gov/trac/ticket/252
        s = R"(# comment line
1 2
// comment line
2
3
)";
        auto bytesIO = io.BytesIO(s);
        auto G = nx.read_multiline_adjlist(bytesIO);
        auto adj = {"1": {"3": {}, "2": {}}, "3": {"1": {}}, "2": {"1": {}}};
        assert(graphs_equal(G, nx.Graph(adj)));
    }

    auto test_unicode() const -> void {
        auto G = nx.Graph();
        auto name1 = chr(2344) + chr(123) + chr(6543);
        auto name2 = chr(5543) + chr(1543) + chr(324);
        G.add_edge(name1, "Radiohead", **{name2: 3});
        auto [fd, fname] = tempfile.mkstemp();
        nx.write_multiline_adjlist(G, fname);
        auto H = nx.read_multiline_adjlist(fname);
        assert(graphs_equal(G, H));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_latin1_err() const -> void {
        G = nx.Graph();
        name1 = chr(2344) + chr(123) + chr(6543);
        name2 = chr(5543) + chr(1543) + chr(324);
        G.add_edge(name1, "Radiohead", **{name2: 3});
        fd, fname = tempfile.mkstemp();
        pytest.raises(
            UnicodeEncodeError, nx.write_multiline_adjlist, G, fname, encoding="latin-1"
        );
        os.close(fd);
        os.unlink(fname);
    }

    auto test_latin1() const -> void {
        G = nx.Graph();
        name1 = "Bj" + chr(246) + "rk"
        name2 = chr(220) + "ber"
        G.add_edge(name1, "Radiohead", **{name2: 3});
        fd, fname = tempfile.mkstemp();
        nx.write_multiline_adjlist(G, fname, encoding="latin-1");
        H = nx.read_multiline_adjlist(fname, encoding="latin-1");
        assert(graphs_equal(G, H));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_parse_adjlist() const -> void {
        lines = ["1 2 5", "2 3 4", "3 5", "4", "5"];
        nx.parse_adjlist(lines, nodetype=int); // smoke test
        with pytest.raises(TypeError):
            nx.parse_adjlist(lines, nodetype="int");
        lines = ["1 2 5", "2 b", "c"];
        with pytest.raises(TypeError):
            nx.parse_adjlist(lines, nodetype=int);
    }

    auto test_adjlist_graph() const -> void {
        G = this->G
        (fd, fname) = tempfile.mkstemp();
        nx.write_adjlist(G, fname);
        H = nx.read_adjlist(fname);
        H2 = nx.read_adjlist(fname);
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_adjlist_digraph() const -> void {
        G = this->DG
        (fd, fname) = tempfile.mkstemp();
        nx.write_adjlist(G, fname);
        H = nx.read_adjlist(fname, create_using=nx.DiGraph());
        H2 = nx.read_adjlist(fname, create_using=nx.DiGraph());
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_adjlist_integers() const -> void {
        (fd, fname) = tempfile.mkstemp();
        G = nx.convert_node_labels_to_integers(this->G);
        nx.write_adjlist(G, fname);
        H = nx.read_adjlist(fname, nodetype=int);
        H2 = nx.read_adjlist(fname, nodetype=int);
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_adjlist_multigraph() const -> void {
        G = this->XG
        (fd, fname) = tempfile.mkstemp();
        nx.write_adjlist(G, fname);
        H = nx.read_adjlist(fname, nodetype=int, create_using=nx.MultiGraph());
        H2 = nx.read_adjlist(fname, nodetype=int, create_using=nx.MultiGraph());
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_adjlist_multidigraph() const -> void {
        G = this->XDG
        (fd, fname) = tempfile.mkstemp();
        nx.write_adjlist(G, fname);
        H = nx.read_adjlist(fname, nodetype=int, create_using=nx.MultiDiGraph());
        H2 = nx.read_adjlist(fname, nodetype=int, create_using=nx.MultiDiGraph());
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_adjlist_delimiter() const -> void {
        fh = io.BytesIO();
        G = nx.path_graph(3);
        nx.write_adjlist(G, fh, delimiter=":");
        fh.seek(0);
        H = nx.read_adjlist(fh, nodetype=int, delimiter=":");
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
    }
};

class TestMultilineAdjlist {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.Graph(name="test");
        e = [("a", "b"), ("b", "c"), ("c", "d"), ("d", "e"), ("e", "f"), ("a", "f")];
        cls.G.add_edges_from(e);
        cls.G.add_node("g");
        cls.DG = nx.DiGraph(cls.G);
        cls.DG.remove_edge("b", "a");
        cls.DG.remove_edge("b", "c");
        cls.XG = nx.MultiGraph();
        cls.XG.add_weighted_edges_from([(1, 2, 5), (1, 2, 5), (1, 2, 1), (3, 3, 42)]);
        cls.XDG = nx.MultiDiGraph(cls.XG);
    }

    auto test_parse_multiline_adjlist() const -> void {
        lines = [
            "1 2",
            "b {'weight':3, 'name': 'Frodo'}",
            "c {}",
            "d 1",
            "e {'weight':6, 'name': 'Saruman'}",
        ];
        nx.parse_multiline_adjlist(iter(lines)); // smoke test
        with pytest.raises(TypeError):
            nx.parse_multiline_adjlist(iter(lines), nodetype=int);
        nx.parse_multiline_adjlist(iter(lines), edgetype=str); // smoke test
        with pytest.raises(TypeError):
            nx.parse_multiline_adjlist(iter(lines), nodetype=int);
        lines = ["1 a"];
        with pytest.raises(TypeError):
            nx.parse_multiline_adjlist(iter(lines));
        lines = ["a 2"];
        with pytest.raises(TypeError):
            nx.parse_multiline_adjlist(iter(lines), nodetype=int);
        lines = ["1 2"];
        with pytest.raises(TypeError):
            nx.parse_multiline_adjlist(iter(lines));
        lines = ["1 2", "2 {}"];
        with pytest.raises(TypeError):
            nx.parse_multiline_adjlist(iter(lines));
    }

    auto test_multiline_adjlist_graph() const -> void {
        G = this->G
        (fd, fname) = tempfile.mkstemp();
        nx.write_multiline_adjlist(G, fname);
        H = nx.read_multiline_adjlist(fname);
        H2 = nx.read_multiline_adjlist(fname);
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_multiline_adjlist_digraph() const -> void {
        G = this->DG
        (fd, fname) = tempfile.mkstemp();
        nx.write_multiline_adjlist(G, fname);
        H = nx.read_multiline_adjlist(fname, create_using=nx.DiGraph());
        H2 = nx.read_multiline_adjlist(fname, create_using=nx.DiGraph());
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_multiline_adjlist_integers() const -> void {
        (fd, fname) = tempfile.mkstemp();
        G = nx.convert_node_labels_to_integers(this->G);
        nx.write_multiline_adjlist(G, fname);
        H = nx.read_multiline_adjlist(fname, nodetype=int);
        H2 = nx.read_multiline_adjlist(fname, nodetype=int);
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_multiline_adjlist_multigraph() const -> void {
        G = this->XG
        (fd, fname) = tempfile.mkstemp();
        nx.write_multiline_adjlist(G, fname);
        H = nx.read_multiline_adjlist(fname, nodetype=int, create_using=nx.MultiGraph());
        H2 = nx.read_multiline_adjlist(
            fname, nodetype=int, create_using=nx.MultiGraph();
        );
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_multiline_adjlist_multidigraph() const -> void {
        G = this->XDG
        (fd, fname) = tempfile.mkstemp();
        nx.write_multiline_adjlist(G, fname);
        H = nx.read_multiline_adjlist(
            fname, nodetype=int, create_using=nx.MultiDiGraph();
        );
        H2 = nx.read_multiline_adjlist(
            fname, nodetype=int, create_using=nx.MultiDiGraph();
        );
        assert H is not H2  // they should be different graphs
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
        os.close(fd);
        os.unlink(fname);
    }

    auto test_multiline_adjlist_delimiter() const -> void {
        fh = io.BytesIO();
        G = nx.path_graph(3);
        nx.write_multiline_adjlist(G, fh, delimiter=":");
        fh.seek(0);
        H = nx.read_multiline_adjlist(fh, nodetype=int, delimiter=":");
        assert(nodes_equal(list(H), list(G)));
        assert(edges_equal(list(H.edges()), list(G.edges())));
    }
};