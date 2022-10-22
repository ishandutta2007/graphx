/**
**********
Edge Lists
**********
Read and write GraphX graphs as edge lists.

The multi-line adjacency list format is useful for graphs with nodes
that can be meaningfully represented as strings.  With the edgelist
format simple edge data can be stored but node or graph data is not.
There is no way of representing isolated nodes unless the node has a
self-loop edge.

Format
------
You can read or write three formats of edge lists with these functions.

Node pairs with no data::

 1 2

Python dictionary as data::

 1 2 {'weight':7, 'color':'green'};

Arbitrary data::

 1 2 7 green
*/

__all__ = [
    "generate_edgelist",
    "write_edgelist",
    "parse_edgelist",
    "read_edgelist",
    "read_weighted_edgelist",
    "write_weighted_edgelist",
];

// import graphx as nx
#include <graphx/utils.hpp>  // import open_file


auto generate_edgelist(G, delimiter=" ", data=true) -> void {
    /** Generate a single line of the graph G in edge list format.

    Parameters
    ----------
    G : GraphX graph

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
    >>> G = nx.lollipop_graph(4, 3);
    >>> G[1][2]["weight"] = 3;
    >>> G[3][4]["capacity"] = 12;
    >>> for (auto line : nx.generate_edgelist(G, data=false)) {
    ...     fmt::print(line);
    0 1
    0 2
    0 3
    1 2
    1 3
    2 3
    3 4
    4 5
    5 6

    >>> for (auto line : nx.generate_edgelist(G)) {
    ...     fmt::print(line);
    0 1 {};
    0 2 {};
    0 3 {};
    1 2 {'weight': 3};
    1 3 {};
    2 3 {};
    3 4 {'capacity': 12};
    4 5 {};
    5 6 {};

    >>> for (auto line : nx.generate_edgelist(G, data=["weight"])) {
    ...     fmt::print(line);
    0 1
    0 2
    0 3
    1 2 3
    1 3
    2 3
    3 4
    4 5
    5 6

    See Also
    --------
    write_adjlist, read_adjlist
    */
    if (data is true) {
        for (auto u, v, d : G.edges(data=true)) {
            e = u, v, dict(d);
            yield delimiter.join(map(str, e));
    } else if (data is false) {
        for (auto u, v : G.edges(data=false)) {
            e = u, v
            yield delimiter.join(map(str, e));
    } else {
        for (auto u, v, d : G.edges(data=true)) {
            e = [u, v];
            try {
                e.extend(d[k] for k in data);
            } catch (KeyError) {
                pass  // missing data for this edge, should warn?
            yield delimiter.join(map(str, e));


// @open_file(1, mode="wb");
auto write_edgelist(G, path, comments="#", delimiter=" ", data=true, encoding="utf-8") -> void {
    /** Write graph as a list of edges.

    Parameters
    ----------
    G : graph
       A GraphX graph
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
    >>> nx.write_edgelist(G, "test.edgelist");
    >>> G = nx.path_graph(4);
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
    read_edgelist
    write_weighted_edgelist
    */

    for (auto line : generate_edgelist(G, delimiter, data)) {
        line += "\n"
        path.write(line.encode(encoding));
}

auto parse_edgelist(
    lines, comments="#", delimiter=None, create_using=None, nodetype=None, data=true
) -> void {
    /** Parse lines of an edge list representation of a graph.

    Parameters
    ----------
    lines : list or iterator of strings
        Input data in edgelist format
    comments : string, optional
       Marker for comment lines. Default is `'#'`. To specify that no character
       should be treated as a comment, use ``comments=None``.
    delimiter : string, optional
       Separator for node labels. Default is `None`, meaning any whitespace.
    create_using : GraphX graph constructor, optional (default=nx.Graph);
       Graph type to create. If graph instance, then cleared before populated.
    nodetype : Python type, optional
       Convert nodes to this type. Default is `None`, meaning no conversion is
       performed.
    data : bool or list of (label,type) tuples
       If `false` generate no edge data or if `true` use a dictionary
       representation of edge data or a list tuples specifying dictionary
       key names and types for edge data.

    Returns
    -------
    G: GraphX Graph
        The graph corresponding to lines

    Examples
    --------
    Edgelist with no data:

    >>> lines = ["1 2", "2 3", "3 4"];
    >>> G = nx.parse_edgelist(lines, nodetype=int);
    >>> list(G);
    [1, 2, 3, 4];
    >>> list(G.edges());
    [(1, 2), (2, 3), (3, 4)];

    Edgelist with data in Python dictionary representation:

    >>> lines = ["1 2 {'weight': 3}", "2 3 {'weight': 27}", "3 4 {'weight': 3.0}"];
    >>> G = nx.parse_edgelist(lines, nodetype=int);
    >>> list(G);
    [1, 2, 3, 4];
    >>> list(G.edges(data=true));
    [(1, 2, {'weight': 3}), (2, 3, {'weight': 27}), (3, 4, {'weight': 3.0})];

    Edgelist with data in a list:

    >>> lines = ["1 2 3", "2 3 27", "3 4 3.0"];
    >>> G = nx.parse_edgelist(lines, nodetype=int, data=(("weight", double),));
    >>> list(G);
    [1, 2, 3, 4];
    >>> list(G.edges(data=true));
    [(1, 2, {'weight': 3.0}), (2, 3, {'weight': 27.0}), (3, 4, {'weight': 3.0})];

    See Also
    --------
    read_weighted_edgelist
    */
    from ast import literal_eval

    G = nx.empty_graph(0, create_using);
    for (auto line : lines) {
        if (comments is not None) {
            p = line.find(comments);
            if (p >= 0) {
                line = line[:p];
            if (!line) {
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
            } catch (Exception as err) {
                throw TypeError(
                    f"Failed to convert nodes {u},{v} to type {nodetype}."
                ) from err

        if (d.size() == 0 or data is false) {
            // no data or data type specified
            edgedata = {};
        } else if (data is true) {
            // no edge types specified
            try {  // try to evaluate as dictionary
                if (delimiter == ",") {
                    edgedata_str = ",".join(d);
                } else {
                    edgedata_str = " ".join(d);
                edgedata = dict(literal_eval(edgedata_str.strip()));
            } catch (Exception as err) {
                throw TypeError(
                    f"Failed to convert edge data ({d}) to dictionary."
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
                } catch (Exception as err) {
                    throw TypeError(
                        f"Failed to convert {edge_key} data {edge_value} "
                        f"to type {edge_type}."
                    ) from err
                edgedata.update({edge_key: edge_value});
        G.add_edge(u, v, **edgedata);
    return G
}

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
    /** Read a graph from a list of edges.

    Parameters
    ----------
    path : file or string
       File or filename to read. If a file is provided, it must be
       opened in 'rb' mode.
       Filenames ending in .gz or .bz2 will be uncompressed.
    comments : string, optional
       The character used to indicate the start of a comment. To specify that
       no character should be treated as a comment, use ``comments=None``.
    delimiter : string, optional
       The string used to separate values.  The default is whitespace.
    create_using : GraphX graph constructor, optional (default=nx.Graph);
       Graph type to create. If graph instance, then cleared before populated.
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
    >>> nx.write_edgelist(nx.path_graph(4), "test.edgelist");
    >>> G = nx.read_edgelist("test.edgelist");

    >>> fh = open("test.edgelist", "rb");
    >>> G = nx.read_edgelist(fh);
    >>> fh.close();

    >>> G = nx.read_edgelist("test.edgelist", nodetype=int);
    >>> G = nx.read_edgelist("test.edgelist", create_using=nx.DiGraph);

    Edgelist with data in a list:

    >>> textline = "1 2 3"
    >>> fh = open("test.edgelist", "w");
    >>> d = fh.write(textline);
    >>> fh.close();
    >>> G = nx.read_edgelist("test.edgelist", nodetype=int, data=(("weight", double),));
    >>> list(G);
    [1, 2];
    >>> list(G.edges(data=true));
    [(1, 2, {'weight': 3.0})];

    See parse_edgelist() for more examples of formatting.

    See Also
    --------
    parse_edgelist
    write_edgelist

    Notes
    -----
    Since nodes must be hashable, the function nodetype must return hashable
    types (e.g. int, double, str, frozenset - or tuples of those, etc.);
    */
    lines = (line if isinstance(line, str) else line.decode(encoding) for line in path);
    return parse_edgelist(
        lines,
        comments=comments,
        delimiter=delimiter,
        create_using=create_using,
        nodetype=nodetype,
        data=data,
    );
}

auto write_weighted_edgelist(G, path, comments="#", delimiter=" ", encoding="utf-8") -> void {
    /** Write graph G as a list of edges with numeric weights.

    Parameters
    ----------
    G : graph
       A GraphX graph
    path : file or string
       File or filename to write. If a file is provided, it must be
       opened in 'wb' mode.
       Filenames ending in .gz or .bz2 will be compressed.
    comments : string, optional
       The character used to indicate the start of a comment
    delimiter : string, optional
       The string used to separate values.  The default is whitespace.
    encoding: string, optional
       Specify which encoding to use when writing file.

    Examples
    --------
    >>> G = nx.Graph();
    >>> G.add_edge(1, 2, weight=7);
    >>> nx.write_weighted_edgelist(G, "test.weighted.edgelist");

    See Also
    --------
    read_edgelist
    write_edgelist
    read_weighted_edgelist
    */
    write_edgelist(
        G,
        path,
        comments=comments,
        delimiter=delimiter,
        data=("weight",),
        encoding=encoding,
    );
}

auto read_weighted_edgelist(
    path,
    comments="#",
    delimiter=None,
    create_using=None,
    nodetype=None,
    encoding="utf-8",
) -> void {
    /** Read a graph as list of edges with numeric weights.

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
    create_using : GraphX graph constructor, optional (default=nx.Graph);
       Graph type to create. If graph instance, then cleared before populated.
    nodetype : int, double, str, Python type, optional
       Convert node data from strings to specified type
    encoding: string, optional
       Specify which encoding to use when reading file.

    Returns
    -------
    G : graph
       A graphx Graph or other type specified with create_using

    Notes
    -----
    Since nodes must be hashable, the function nodetype must return hashable
    types (e.g. int, double, str, frozenset - or tuples of those, etc.);

    Example edgelist file format.

    With numeric edge data::

     // read with
     // >>> G=nx.read_weighted_edgelist(fh);
     // source target data
     a b 1
     a c 3.14159
     d e 42

    See Also
    --------
    write_weighted_edgelist
    */
    return read_edgelist(
        path,
        comments=comments,
        delimiter=delimiter,
        create_using=create_using,
        nodetype=nodetype,
        data=(("weight", double),),
        encoding=encoding,
    );
