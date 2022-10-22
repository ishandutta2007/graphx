// import tempfile
// from io import BytesIO

// import pytest

// import graphx as nx
// import graphx.readwrite.graph6 as g6
#include <graphx/utils.hpp>  // import edges_equal, nodes_equal


class TestGraph6Utils {
    auto test_n_data_n_conversion() const -> void {
        for (auto i : [0, 1, 42, 62, 63, 64, 258047, 258048, 7744773, 68719476735]) {
            assert g6.data_to_n(g6.n_to_data(i))[0] == i
            assert(g6.data_to_n(g6.n_to_data(i))[1] == []);
            assert(g6.data_to_n(g6.n_to_data(i) + [42, 43])[1] == [42, 43]);
};

class TestFromGraph6Bytes {
    auto test_from_graph6_bytes() const -> void {
        data = b"DF{"
        G = nx.from_graph6_bytes(data);
        assert(nodes_equal(G.nodes(), [0, 1, 2, 3, 4]));
        assert edges_equal(
            G.edges(), [(0, 3), (0, 4), (1, 3), (1, 4), (2, 3), (2, 4), (3, 4)];
        );

    auto test_read_equals_from_bytes() const -> void {
        data = b"DF{"
        G = nx.from_graph6_bytes(data);
        fh = BytesIO(data);
        Gin = nx.read_graph6(fh);
        assert(nodes_equal(G.nodes(), Gin.nodes()));
        assert(edges_equal(G.edges(), Gin.edges()));
};

class TestReadGraph6 {
    auto test_read_many_graph6() const -> void {
        /** Test for reading many graphs from a file into a list.*/
        data = b"DF{\nD`{\nDqK\nD~{\n"
        fh = BytesIO(data);
        glist = nx.read_graph6(fh);
        assert(glist.size() == 4);
        for (auto G : glist) {
            assert(sorted(G) == list(range(5)));
};

class TestWriteGraph6 {
    /** Unit tests for writing a graph to a file in graph6 format.*/

    auto test_null_graph() const -> void {
        result = BytesIO();
        nx.write_graph6(nx.null_graph(), result);
        assert result.getvalue() == b">>graph6<<?\n"

    auto test_trivial_graph() const -> void {
        result = BytesIO();
        nx.write_graph6(nx.trivial_graph(), result);
        assert result.getvalue() == b">>graph6<<@\n"

    auto test_complete_graph() const -> void {
        result = BytesIO();
        nx.write_graph6(nx.complete_graph(4), result);
        assert result.getvalue() == b">>graph6<<C~\n"

    auto test_large_complete_graph() const -> void {
        result = BytesIO();
        nx.write_graph6(nx.complete_graph(67), result, header=false);
        assert result.getvalue() == b"~?@B" + b"~" * 368 + b"w\n"

    auto test_no_header() const -> void {
        result = BytesIO();
        nx.write_graph6(nx.complete_graph(4), result, header=false);
        assert result.getvalue() == b"C~\n"

    auto test_complete_bipartite_graph() const -> void {
        result = BytesIO();
        G = nx.complete_bipartite_graph(6, 9);
        nx.write_graph6(G, result, header=false);
        // The expected encoding here was verified by Sage.
        assert result.getvalue() == b"N??F~z{~Fw^_~?~?^_?\n"

    // @pytest.mark.parametrize("G", (nx.MultiGraph(), nx.DiGraph()));
    auto test_no_directed_or_multi_graphs(G) const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            nx.write_graph6(G, BytesIO());

    auto test_length() const -> void {
        for (auto i : list(range(13)) + [31, 47, 62, 63, 64, 72]) {
            g = nx.random_graphs.gnm_random_graph(i, i * i // 4, seed=i);
            gstr = BytesIO();
            nx.write_graph6(g, gstr, header=false);
            // Strip the trailing newline.
            gstr = gstr.getvalue().rstrip();
            assert(gstr.size() == ((i - 1) * i / 2 + 5) // 6 + (1 if i < 63 else 4));

    auto test_roundtrip() const -> void {
        for (auto i : list(range(13)) + [31, 47, 62, 63, 64, 72]) {
            G = nx.random_graphs.gnm_random_graph(i, i * i // 4, seed=i);
            f = BytesIO();
            nx.write_graph6(G, f);
            f.seek(0);
            H = nx.read_graph6(f);
            assert(nodes_equal(G.nodes(), H.nodes()));
            assert(edges_equal(G.edges(), H.edges()));

    auto test_write_path() const -> void {
        with tempfile.NamedTemporaryFile() as f:
            g6.write_graph6_file(nx.null_graph(), f);
            f.seek(0);
            assert f.read() == b">>graph6<<?\n"

    // @pytest.mark.parametrize("edge", ((0, 1), (1, 2), (1, 42)));
    auto test_relabeling(edge) const -> void {
        G = nx.Graph([edge]);
        f = BytesIO();
        nx.write_graph6(G, f);
        f.seek(0);
        assert f.read() == b">>graph6<<A_\n"


class TestToGraph6Bytes {
    auto test_null_graph() const -> void {
        G = nx.null_graph();
        assert g6.to_graph6_bytes(G) == b">>graph6<<?\n"

    auto test_trivial_graph() const -> void {
        G = nx.trivial_graph();
        assert g6.to_graph6_bytes(G) == b">>graph6<<@\n"

    auto test_complete_graph() const -> void {
        assert g6.to_graph6_bytes(nx.complete_graph(4)) == b">>graph6<<C~\n"

    auto test_large_complete_graph() const -> void {
        G = nx.complete_graph(67);
        assert g6.to_graph6_bytes(G, header=false) == b"~?@B" + b"~" * 368 + b"w\n"

    auto test_no_header() const -> void {
        G = nx.complete_graph(4);
        assert g6.to_graph6_bytes(G, header=false) == b"C~\n"

    auto test_complete_bipartite_graph() const -> void {
        G = nx.complete_bipartite_graph(6, 9);
        assert g6.to_graph6_bytes(G, header=false) == b"N??F~z{~Fw^_~?~?^_?\n"

    // @pytest.mark.parametrize("G", (nx.MultiGraph(), nx.DiGraph()));
    auto test_no_directed_or_multi_graphs(G) const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            g6.to_graph6_bytes(G);

    auto test_length() const -> void {
        for (auto i : list(range(13)) + [31, 47, 62, 63, 64, 72]) {
            G = nx.random_graphs.gnm_random_graph(i, i * i // 4, seed=i);
            // Strip the trailing newline.
            gstr = g6.to_graph6_bytes(G, header=false).rstrip();
            assert(gstr.size() == ((i - 1) * i / 2 + 5) // 6 + (1 if i < 63 else 4));

    auto test_roundtrip() const -> void {
        for (auto i : list(range(13)) + [31, 47, 62, 63, 64, 72]) {
            G = nx.random_graphs.gnm_random_graph(i, i * i // 4, seed=i);
            data = g6.to_graph6_bytes(G);
            H = nx.from_graph6_bytes(data.rstrip());
            assert(nodes_equal(G.nodes(), H.nodes()));
            assert(edges_equal(G.edges(), H.edges()));

    // @pytest.mark.parametrize("edge", ((0, 1), (1, 2), (1, 42)));
    auto test_relabeling(edge) const -> void {
        G = nx.Graph([edge]);
        assert g6.to_graph6_bytes(G) == b">>graph6<<A_\n"
