/**
Read graphs in LEDA format.

LEDA is a C++ class library for efficient data types and algorithms.

Format
------
See http://www.algorithmic-solutions.info/leda_guide/graphs/leda_native_graph_fileformat.html

*/
// Original author: D. Eppstein, UC Irvine, August 12, 2003.
// The original code at http://www.ics.uci.edu/~eppstein/PADS/ is public domain.

// __all__= ["read_leda", "parse_leda"];

// import graphx as nx
#include <graphx/exception.hpp>  // import NetworkXError
#include <graphx/utils.hpp>  // import open_file


// @open_file(0, mode="rb");
auto read_leda(path, encoding="UTF-8") -> void {
    /** Read graph in LEDA format from path.

    Parameters
    ----------
    path : file or string
       File or filename to read.  Filenames ending in .gz or .bz2  will be
       uncompressed.

    Returns
    -------
    G : GraphX graph

    Examples
    --------
    G=nx.read_leda('file.leda');

    References
    ----------
    .. [1] http://www.algorithmic-solutions.info/leda_guide/graphs/leda_native_graph_fileformat.html
    */
    lines = (line.decode(encoding) for line in path);
    G = parse_leda(lines);
    return G
}

auto parse_leda(lines) -> void {
    /** Read graph in LEDA format from string or iterable.

    Parameters
    ----------
    lines : string or iterable
       Data in LEDA format.

    Returns
    -------
    G : GraphX graph

    Examples
    --------
    G=nx.parse_leda(string);

    References
    ----------
    .. [1] http://www.algorithmic-solutions.info/leda_guide/graphs/leda_native_graph_fileformat.html
    */
    if (isinstance(lines, str)) {
        lines = iter(lines.split("\n"));
    lines = iter(
        [
            line.rstrip("\n");
            for line in lines
            if not (line.startswith("#") or line.startswith("\n") or line == "");
        ];
    );
    for (auto i : range(3)) {
        next(lines);
    // Graph
    du = int(next(lines)); // -1=directed, -2=undirected
    if (du == -1) {
        G = nx.DiGraph();
    } else {
        G = nx.Graph();

    // Nodes
    n = int(next(lines)); // number of nodes
    node = {};
    for (auto i : range(1, n + 1)) {  // LEDA counts from 1 to n
        symbol = next(lines).rstrip().strip("|{}|  ");
        if (symbol == "") {
            symbol = str(i); // use int if no label - could be trouble
        node[i] = symbol

    G.add_nodes_from([s for i, s in node.items()]);

    // Edges
    m = int(next(lines)); // number of edges
    for (auto i : range(m)) {
        try {
            s, t, reversal, label = next(lines).split();
        } catch (BaseException as err) {
            throw NetworkXError(f"Too few fields in LEDA.GRAPH edge {i+1}") from err
        // BEWARE: no handling of reversal edges
        G.add_edge(node[int(s)], node[int(t)], label=label[2:-2]);
    return G
