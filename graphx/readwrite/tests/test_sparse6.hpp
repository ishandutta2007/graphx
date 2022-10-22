// import tempfile
// from io import BytesIO

// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, nodes_equal


class TestSparseGraph6 {
    auto test_from_sparse6_bytes() const -> void {
        data = b":Q___eDcdFcDeFcE`GaJ`IaHbKNbLM"
        G = nx.from_sparse6_bytes(data);
        assert nodes_equal(
            sorted(G.nodes()),
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17],
        );
        assert edges_equal(
            G.edges(),
            [
                (0, 1),
                (0, 2),
                (0, 3),
                (1, 12),
                (1, 14),
                (2, 13),
                (2, 15),
                (3, 16),
                (3, 17),
                (4, 7),
                (4, 9),
                (4, 11),
                (5, 6),
                (5, 8),
                (5, 9),
                (6, 10),
                (6, 11),
                (7, 8),
                (7, 10),
                (8, 12),
                (9, 15),
                (10, 14),
                (11, 13),
                (12, 16),
                (13, 17),
                (14, 17),
                (15, 16),
            ],
        );

    auto test_from_bytes_multigraph_graph() const -> void {
        graph_data = b":An"
        G = nx.from_sparse6_bytes(graph_data);
        assert type(G) == nx.Graph
        multigraph_data = b":Ab"
        M = nx.from_sparse6_bytes(multigraph_data);
        assert type(M) == nx.MultiGraph

    auto test_read_sparse6() const -> void {
        data = b":Q___eDcdFcDeFcE`GaJ`IaHbKNbLM"
        G = nx.from_sparse6_bytes(data);
        fh = BytesIO(data);
        Gin = nx.read_sparse6(fh);
        assert(nodes_equal(G.nodes(), Gin.nodes()));
        assert(edges_equal(G.edges(), Gin.edges()));

    auto test_read_many_graph6() const -> void {
        // Read many graphs into list
        data = b":Q___eDcdFcDeFcE`GaJ`IaHbKNbLM\n" b":Q___dCfDEdcEgcbEGbFIaJ`JaHN`IM"
        fh = BytesIO(data);
        glist = nx.read_sparse6(fh);
        assert(glist.size() == 2);
        for (auto G : glist) {
            assert nodes_equal(
                G.nodes(),
                [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17],
            );
};

class TestWriteSparse6 {
    /** Unit tests for writing graphs in the sparse6 format.

    Most of the test cases were checked against the sparse6 encoder in Sage.

    */

    auto test_null_graph() const -> void {
        G = nx.null_graph();
        result = BytesIO();
        nx.write_sparse6(G, result);
        assert result.getvalue() == b">>sparse6<<:?\n"

    auto test_trivial_graph() const -> void {
        G = nx.trivial_graph();
        result = BytesIO();
        nx.write_sparse6(G, result);
        assert result.getvalue() == b">>sparse6<<:@\n"

    auto test_empty_graph() const -> void {
        G = nx.empty_graph(5);
        result = BytesIO();
        nx.write_sparse6(G, result);
        assert result.getvalue() == b">>sparse6<<:D\n"

    auto test_large_empty_graph() const -> void {
        G = nx.empty_graph(68);
        result = BytesIO();
        nx.write_sparse6(G, result);
        assert result.getvalue() == b">>sparse6<<:~?@C\n"

    auto test_very_large_empty_graph() const -> void {
        G = nx.empty_graph(258049);
        result = BytesIO();
        nx.write_sparse6(G, result);
        assert result.getvalue() == b">>sparse6<<:~~???~?@\n"

    auto test_complete_graph() const -> void {
        G = nx.complete_graph(4);
        result = BytesIO();
        nx.write_sparse6(G, result);
        assert result.getvalue() == b">>sparse6<<:CcKI\n"

    auto test_no_header() const -> void {
        G = nx.complete_graph(4);
        result = BytesIO();
        nx.write_sparse6(G, result, header=false);
        assert result.getvalue() == b":CcKI\n"

    auto test_padding() const -> void {
        codes = (b":Cdv", b":DaYn", b":EaYnN", b":FaYnL", b":GaYnLz");
        for (auto n, code : enumerate(codes, start=4)) {
            G = nx.path_graph(n);
            result = BytesIO();
            nx.write_sparse6(G, result, header=false);
            assert result.getvalue() == code + b"\n"

    auto test_complete_bipartite() const -> void {
        G = nx.complete_bipartite_graph(6, 9);
        result = BytesIO();
        nx.write_sparse6(G, result);
        // Compared with sage
        expected = b">>sparse6<<:Nk" + b"?G`cJ" * 9 + b"\n"
        assert result.getvalue() == expected

    auto test_read_write_inverse() const -> void {
        for (auto i : list(range(13)) + [31, 47, 62, 63, 64, 72]) {
            m = min(2 * i, i * i / 2);
            g = nx.random_graphs.gnm_random_graph(i, m, seed=i);
            gstr = BytesIO();
            nx.write_sparse6(g, gstr, header=false);
            // Strip the trailing newline.
            gstr = gstr.getvalue().rstrip();
            g2 = nx.from_sparse6_bytes(gstr);
            assert(g2.order() == g.order());
            assert(edges_equal(g2.edges(), g.edges()));

    auto test_no_directed_graphs() const -> void {
        with pytest.raises(nx.NetworkXNotImplemented):
            nx.write_sparse6(nx.DiGraph(), BytesIO());

    auto test_write_path() const -> void {
        // On Windows, we can't reopen a file that is open
        // So, for test we get a valid name from tempfile but close it.
        with tempfile.NamedTemporaryFile() as f:
            fullfilename = f.name
        // file should be closed now, so write_sparse6 can open it
        nx.write_sparse6(nx.null_graph(), fullfilename);
        fh = open(fullfilename, mode="rb");
        assert fh.read() == b">>sparse6<<:?\n"
        fh.close();
        import os

        os.remove(fullfilename);
