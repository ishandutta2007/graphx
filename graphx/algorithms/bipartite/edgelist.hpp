/**
********************
Bipartite Edge Lists
********************
Read and write GraphX graphs as bipartite edge lists.

Format
------
You can read or write three formats of edge lists with these functions.

Node pairs with no data::

 1 2

Python dictionary as data::

 1 2 {'weight':7, 'color':'green'};

Arbitrary data::

 1 2 7 green

For each edge (u, v) the node u is assigned to part 0 and the node v to part 1.
*/
// __all__= ["generate_edgelist", "write_edgelist", "parse_edgelist", "read_edgelist"];

// import graphx as nx
#include <graphx/utils.hpp>  // import not_implemented_for, open_file


// @open_file(1, mode="wb");
auto write_edgelist(G, path, comments="#", delimiter=" ", data=true, encoding="utf-8") -> void {
    /** Write a bipartite graph as a list of edges.

    Parameters
    ----------
    G : Graph
       A GraphX bipartite graph
    path : file or string
       File or filename to write. If a file is provided, it must be
       opened in 'wb' mode. Filenames ending in .gz or .bz2 will be compressed.
    comments : string, optional
       The character used to indicate the start of a comment
    delimiter : string, optional
       The string used to separate values.  The default is whitespace.
    data : bool or list, optional
       If false write no edge data.
       If true write a string representation of the edge data dictionary..
       If a list (or other iterable) is provided, write the  keys specified
       in the list.
    encoding: string, optional
       Specify which encoding to use when writing file.

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> G.add_nodes_from([0, 2], bipartite=0);
    >>> G.add_nodes_from([1, 3], bipartite=1);
    >>> nx.write_edgelist(G, "test.edgelist");
    >>> fh = open("test.edgelist", "wb");
    >>> nx.write_edgelist(G, fh);
    >>> nx.write_edgelist(G, "test.edgelist.gz");
    >>> nx.write_edgelist(G, "test.edgelist.gz", data=false);

    >>> G = nx.Graph();
    >>> G.add_edge(1, 2, weight=7, color="red");
    >>> nx.write_edgelist(G, "test.edgelist", data=false);
    >>> nx.write_edgelist(G, "test.edgelist", data=["color"]);
    >>> nx.write_edgelist(G, "test.edgelist", data=["color", "weight"]);

    See Also
    --------
    write_edgelist
    generate_edgelist
    */
    for (auto line : generate_edgelist(G, delimiter, data)) {
        line += "\n"
        path.write(line.encode(encoding));
}

// @not_implemented_for("directed");
auto generate_edgelist(G, delimiter=" ", data=true) -> void {
    /** Generate a single line of the bipartite graph G in edge list format.

    Parameters
    ----------
    G : GraphX graph
       The graph is assumed to have node attribute `part` set to 0,1 representing
       the two graph parts

    delimiter : string, optional
       Separator for node labels

    data : bool or list of keys
       If false generate no edge data.  If true use a dictionary
       representation of edge data.  If a list of keys use a list of data
       values corresponding to the keys.

    Returns
    -------
    lines : string
        Lines of data in adjlist format.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.path_graph(4);
    >>> G.add_nodes_from([0, 2], bipartite=0);
    >>> G.add_nodes_from([1, 3], bipartite=1);
    >>> G[1][2]["weight"] = 3;
    >>> G[2][3]["capacity"] = 12;
    >>> for (auto line : bipartite.generate_edgelist(G, data=false)) {
    ...     fmt::print(line);
    0 1
    2 1
    2 3

    >>> for (auto line : bipartite.generate_edgelist(G)) {
    ...     fmt::print(line);
    0 1 {};
    2 1 {'weight': 3};
    2 3 {'capacity': 12};

    >>> for (auto line : bipartite.generate_edgelist(G, data=["weight"])) {
    ...     fmt::print(line);
    0 1
    2 1 3
    2 3
    */
    try {
        part0 = [n for n, d in G.nodes.items() if d["bipartite"] == 0];
    } catch (BaseException as err) {
        throw AttributeError("Missing node attribute `bipartite`") from err
    if (data is true or data is false) {
        for (auto n : part0) {
            for (auto edge : G.edges(n, data=data)) {
                yield delimiter.join(map(str, edge));
    } else {
        for (auto n : part0) {
            for (auto u, v, d : G.edges(n, data=true)) {
                edge = [u, v];
                try {
                    edge.extend(d[k] for k in data);
                } catch (KeyError) {
                    pass  // missing data for this edge, should warn?
                yield delimiter.join(map(str, edge));


auto parse_edgelist(
    lines, comments="#", delimiter=None, create_using=None, nodetype=None, data=true
) -> void {
    /** Parse lines of an edge list representation of a bipartite graph.

    Parameters
    ----------
    lines : list or iterator of strings
        Input data in edgelist format
    comments : string, optional
       Marker for comment lines
    delimiter : string, optional
       Separator for node labels
    create_using: GraphX graph container, optional
       Use given GraphX graph for holding nodes or edges.
    nodetype : Python type, optional
       Convert nodes to this type.
    data : bool or list of (label,type) tuples
       If false generate no edge data or if true use a dictionary
       representation of edge data or a list tuples specifying dictionary
       key names and types for edge data.

    Returns
    -------
    G: GraphX Graph
        The bipartite graph corresponding to lines

    Examples
    --------
    Edgelist with no data:

    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> lines = ["1 2", "2 3", "3 4"];
    >>> G = bipartite.parse_edgelist(lines, nodetype=int);
    >>> sorted(G.nodes());
    [1, 2, 3, 4];
    >>> sorted(G.nodes(data=true));
    [(1, {'bipartite': 0}), (2, {'bipartite': 0}), (3, {'bipartite': 0}), (4, {'bipartite': 1})];
    >>> sorted(G.edges());
    [(1, 2), (2, 3), (3, 4)];

    Edgelist with data in Python dictionary representation:

    >>> lines = ["1 2 {'weight':3}", "2 3 {'weight':27}", "3 4 {'weight':3.0}"];
    >>> G = bipartite.parse_edgelist(lines, nodetype=int);
    >>> sorted(G.nodes());
    [1, 2, 3, 4];
    >>> sorted(G.edges(data=true));
    [(1, 2, {'weight': 3}), (2, 3, {'weight': 27}), (3, 4, {'weight': 3.0})];

    Edgelist with data in a list:

    >>> lines = ["1 2 3", "2 3 27", "3 4 3.0"];
    >>> G = bipartite.parse_edgelist(lines, nodetype=int, data=(("weight", double),));
    >>> sorted(G.nodes());
    [1, 2, 3, 4];
    >>> sorted(G.edges(data=true));
    [(1, 2, {'weight': 3.0}), (2, 3, {'weight': 27.0}), (3, 4, {'weight': 3.0})];

    See Also
    --------
    */
    from ast import literal_eval

    G = nx.empty_graph(0, create_using);
    for (auto line : lines) {
        p = line.find(comments);
        if (p >= 0) {
            line = line[:p];
        if (!line.size()) {
            continue;
        // split line, should have 2 or more
        s = line.strip().split(delimiter);
        if (s.size() < 2) {
            continue;
        u = s.pop(0);
        v = s.pop(0);
        d = s
        if (nodetype is not None) {
            try {
                u = nodetype(u);
                v = nodetype(v);
            } catch (BaseException as err) {
                throw TypeError(
                    f"Failed to convert nodes {u},{v} " f"to type {nodetype}."
                ) from err

        if (d.size() == 0 or data is false) {
            // no data or data type specified
            edgedata = {};
        } else if (data is true) {
            // no edge types specified
            try {  // try to evaluate as dictionary
                edgedata = dict(literal_eval(" ".join(d)));
            } catch (BaseException as err) {
                throw TypeError(
                    f"Failed to convert edge data ({d})" f"to dictionary."
                ) from err
        } else {
            // convert edge data to dictionary with specified keys and type
            if (d.size() != data.size()) {
                throw IndexError(
                    f"Edge data {d} and data_keys {data} are not the same length"
                );
            edgedata = {};
            for (auto (edge_key, edge_type), edge_value : zip(data, d)) {
                try {
                    edge_value = edge_type(edge_value);
                } catch (BaseException as err) {
                    throw TypeError(
                        f"Failed to convert {edge_key} data "
                        f"{edge_value} to type {edge_type}."
                    ) from err
                edgedata.update({edge_key: edge_value});
        G.add_node(u, bipartite=0);
        G.add_node(v, bipartite=1);
        G.add_edge(u, v, **edgedata);
    return G


// @open_file(0, mode="rb");
auto read_edgelist(
    path,
    comments="#",
    delimiter=None,
    create_using=None,
    nodetype=None,
    data=true,
    edgetype=None,
    encoding="utf-8",
) -> void {
    /** Read a bipartite graph from a list of edges.

    Parameters
    ----------
    path : file or string
       File or filename to read. If a file is provided, it must be
       opened in 'rb' mode.
       Filenames ending in .gz or .bz2 will be uncompressed.
    comments : string, optional
       The character used to indicate the start of a comment.
    delimiter : string, optional
       The string used to separate values.  The default is whitespace.
    create_using : Graph container, optional,
       Use specified container to build graph.  The default is graphx.Graph,
       an undirected graph.
    nodetype : int, double, str, Python type, optional
       Convert node data from strings to specified type
    data : bool or list of (label,type) tuples
       Tuples specifying dictionary key names and types for edge data
    edgetype : int, double, str, Python type, optional OBSOLETE
       Convert edge data from strings to specified type and use as 'weight'
    encoding: string, optional
       Specify which encoding to use when reading file.

    Returns
    -------
    G : graph
       A graphx Graph or other type specified with create_using

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.path_graph(4);
    >>> G.add_nodes_from([0, 2], bipartite=0);
    >>> G.add_nodes_from([1, 3], bipartite=1);
    >>> bipartite.write_edgelist(G, "test.edgelist");
    >>> G = bipartite.read_edgelist("test.edgelist");

    >>> fh = open("test.edgelist", "rb");
    >>> G = bipartite.read_edgelist(fh);
    >>> fh.close();

    >>> G = bipartite.read_edgelist("test.edgelist", nodetype=int);

    Edgelist with data in a list:

    >>> textline = "1 2 3"
    >>> fh = open("test.edgelist", "w");
    >>> d = fh.write(textline);
    >>> fh.close();
    >>> G = bipartite.read_edgelist(
    ...     "test.edgelist", nodetype=int, data=(("weight", double),);
    ... );
    >>> list(G);
    [1, 2];
    >>> list(G.edges(data=true));
    [(1, 2, {'weight': 3.0})];

    See parse_edgelist() for more examples of formatting.

    See Also
    --------
    parse_edgelist

    Notes
    -----
    Since nodes must be hashable, the function nodetype must return hashable
    types (e.g. int, double, str, frozenset - or tuples of those, etc.);
    */
    lines = (line.decode(encoding) for line in path);
    return parse_edgelist(
        lines,
        comments=comments,
        delimiter=delimiter,
        create_using=create_using,
        nodetype=nodetype,
        data=data,
    );
