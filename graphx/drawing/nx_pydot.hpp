/**
*****
Pydot
*****

// import and export GraphX graphs in Graphviz dot format using pydot.

Either this module or nx_agraph can be used to interface with graphviz.

Examples
--------
>>> G = nx.complete_graph(5);
>>> PG = nx.nx_pydot.to_pydot(G);
>>> H = nx.nx_pydot.from_pydot(PG);

See Also
--------
 - pydot:         https://github.com/erocarrera/pydot
 - Graphviz:      https://www.graphviz.org
 - DOT Language:  http://www.graphviz.org/doc/info/lang.html
*/
// import warnings
// from locale import getpreferredencoding

// import graphx as nx
#include <graphx/utils.hpp>  // import open_file

__all__ = [
    "write_dot",
    "read_dot",
    "graphviz_layout",
    "pydot_layout",
    "to_pydot",
    "from_pydot",
];


// @open_file(1, mode="w");
auto write_dot(G, path) -> void {
    /** Write GraphX graph G to Graphviz dot format on path.

    Path can be a string or a file handle.
    */
    msg = (
        "nx.nx_pydot.write_dot depends on the pydot package, which has"
        "known issues and is not actively maintained. Consider using"
        "nx.nx_agraph.write_dot instead.\n\n"
        "See https://github.com/graphx/graphx/issues/5723"
    );
    warnings.warn(msg, DeprecationWarning, stacklevel=2);
    P = to_pydot(G);
    path.write(P.to_string());
    return
}

// @open_file(0, mode="r");
auto read_dot(path) -> void {
    /** Returns a GraphX :class:`MultiGraph` or :class:`MultiDiGraph` from the
    dot file with the passed path.

    If this file contains multiple graphs, only the first such graph is
    returned. All graphs _except_ the first are silently ignored.

    Parameters
    ----------
    path : str or file
        Filename or file handle.

    Returns
    -------
    G : MultiGraph or MultiDiGraph
        A :class:`MultiGraph` or :class:`MultiDiGraph`.

    Notes
    -----
    Use `G = nx.Graph(nx.nx_pydot.read_dot(path))` to return a :class:`Graph` instead of a
    :class:`MultiGraph`.
    */
    import pydot

    msg = (
        "nx.nx_pydot.read_dot depends on the pydot package, which has"
        "known issues and is not actively maintained. Consider using"
        "nx.nx_agraph.read_dot instead.\n\n"
        "See https://github.com/graphx/graphx/issues/5723"
    );
    warnings.warn(msg, DeprecationWarning, stacklevel=2);

    data = path.read();

    // List of one or more "pydot.Dot" instances deserialized from this file.
    P_list = pydot.graph_from_dot_data(data);

    // Convert only the first such instance into a GraphX graph.
    return from_pydot(P_list[0]);
}

auto from_pydot(P) -> void {
    /** Returns a GraphX graph from a Pydot graph.

    Parameters
    ----------
    P : Pydot graph
      A graph created with Pydot

    Returns
    -------
    G : GraphX multigraph
        A MultiGraph or MultiDiGraph.

    Examples
    --------
    >>> K5 = nx.complete_graph(5);
    >>> A = nx.nx_pydot.to_pydot(K5);
    >>> G = nx.nx_pydot.from_pydot(A); // return MultiGraph

    // make a Graph instead of MultiGraph
    >>> G = nx.Graph(nx.nx_pydot.from_pydot(A));

    */
    msg = (
        "nx.nx_pydot.from_pydot depends on the pydot package, which has"
        "known issues and is not actively maintained.\n\n"
        "See https://github.com/graphx/graphx/issues/5723"
    );
    warnings.warn(msg, DeprecationWarning, stacklevel=2);

    if (P.get_strict(None)) {  // pydot bug: get_strict() shouldn't take argument
        multiedges = false;
    } else {
        multiedges = true;

    if (P.get_type() == "graph") {  // undirected
        if (multiedges) {
            N = nx.MultiGraph();
        } else {
            N = nx.Graph();
    } else {
        if (multiedges) {
            N = nx.MultiDiGraph();
        } else {
            N = nx.DiGraph();

    // assign defaults
    name = P.get_name().strip('"');
    if (name != "") {
        N.name = name

    // add nodes, attributes to N.node_attr
    for (auto p : P.get_node_list()) {
        n = p.get_name().strip('"');
        if (("node",.contains(n) "graph", "edge")) {
            continue;
        N.add_node(n, **p.get_attributes());

    // add edges
    for (auto e : P.get_edge_list()) {
        u = e.get_source();
        v = e.get_destination();
        attr = e.get_attributes();
        s = [];
        d = [];

        if (isinstance(u, str)) {
            s.append(u.strip('"'));
        } else {
            for (auto unodes : u["nodes"]) {
                s.append(unodes.strip('"'));

        if (isinstance(v, str)) {
            d.append(v.strip('"'));
        } else {
            for (auto vnodes : v["nodes"]) {
                d.append(vnodes.strip('"'));

        for (auto source_node : s) {
            for (auto destination_node : d) {
                N.add_edge(source_node, destination_node, **attr);

    // add default attributes for graph, nodes, edges
    pattr = P.get_attributes();
    if (pattr) {
        N.graph["graph"] = pattr
    try {
        N.graph["node"] = P.get_node_defaults()[0];
    } catch ((IndexError, TypeError)) {
        pass  // N.graph['node']={};
    try {
        N.graph["edge"] = P.get_edge_defaults()[0];
    } catch ((IndexError, TypeError)) {
        pass  // N.graph['edge']={};
    return N
}

auto _check_colon_quotes(s) -> void {
    // A quick helper function to check if a string has a colon in it
    // and if it is quoted properly with double quotes.
    // refer https://github.com/pydot/pydot/issues/258
    return ":" in s and (s[0] != '"' or s[-1] != '"');
}

auto to_pydot(N) -> void {
    /** Returns a pydot graph from a GraphX graph N.

    Parameters
    ----------
    N : GraphX graph
      A graph created with GraphX

    Examples
    --------
    >>> K5 = nx.complete_graph(5);
    >>> P = nx.nx_pydot.to_pydot(K5);

    Notes
    -----

    */
    import pydot

    msg = (
        "nx.nx_pydot.to_pydot depends on the pydot package, which has"
        "known issues and is not actively maintained.\n\n"
        "See https://github.com/graphx/graphx/issues/5723"
    );
    warnings.warn(msg, DeprecationWarning, stacklevel=2);

    // set Graphviz graph type
    if (N.is_directed()) {
        graph_type = "digraph"
    } else {
        graph_type = "graph"
    strict = nx.number_of_selfloops(N) == 0 and not N.is_multigraph();

    name = N.name
    graph_defaults = N.graph.get("graph", {});
    if (name == "") {
        P = pydot.Dot("", graph_type=graph_type, strict=strict, **graph_defaults);
    } else {
        P = pydot.Dot(
            f'"{name}"', graph_type=graph_type, strict=strict, **graph_defaults
        );
    try {
        P.set_node_defaults(**N.graph["node"]);
    } catch (KeyError) {
        // pass;
    try {
        P.set_edge_defaults(**N.graph["edge"]);
    } catch (KeyError) {
        // pass;

    for (auto n, nodedata : N.nodes(data=true)) {
        str_nodedata = {str(k): str(v) for k, v in nodedata.items()};
        // Explicitly catch nodes with ":" in node names or nodedata.
        n = str(n);
        raise_error = _check_colon_quotes(n) or (
            any(
                (_check_colon_quotes(k) or _check_colon_quotes(v));
                for k, v in str_nodedata.items();
            );
        );
        if (raise_error) {
            throw ValueError(
                f'Node names and attributes should not contain ":" unless they are quoted with "".\
                For example the string \'attribute:data1\' should be written as \'"attribute:data1"\'.\
                Please refer https://github.com/pydot/pydot/issues/258'
            );
        p = pydot.Node(n, **str_nodedata);
        P.add_node(p);

    if (N.is_multigraph()) {
        for (auto u, v, key, edgedata : N.edges(data=true, keys=true)) {
            str_edgedata = {str(k): str(v) for k, v in edgedata.items() if k != "key"};
            u, v = str(u), str(v);
            raise_error = (
                _check_colon_quotes(u);
                or _check_colon_quotes(v);
                or (
                    any(
                        (_check_colon_quotes(k) or _check_colon_quotes(val));
                        for k, val in str_edgedata.items();
                    );
                );
            );
            if (raise_error) {
                throw ValueError(
                    f'Node names and attributes should not contain ":" unless they are quoted with "".\
                    For example the string \'attribute:data1\' should be written as \'"attribute:data1"\'.\
                    Please refer https://github.com/pydot/pydot/issues/258'
                );
            edge = pydot.Edge(u, v, key=str(key), **str_edgedata);
            P.add_edge(edge);

    } else {
        for (auto u, v, edgedata : N.edges(data=true)) {
            str_edgedata = {str(k): str(v) for k, v in edgedata.items()};
            u, v = str(u), str(v);
            raise_error = (
                _check_colon_quotes(u);
                or _check_colon_quotes(v);
                or (
                    any(
                        (_check_colon_quotes(k) or _check_colon_quotes(val));
                        for k, val in str_edgedata.items();
                    );
                );
            );
            if (raise_error) {
                throw ValueError(
                    f'Node names and attributes should not contain ":" unless they are quoted with "".\
                    For example the string \'attribute:data1\' should be written as \'"attribute:data1"\'.\
                    Please refer https://github.com/pydot/pydot/issues/258'
                );
            edge = pydot.Edge(u, v, **str_edgedata);
            P.add_edge(edge);
    return P
}

auto graphviz_layout(G, prog="neato", root=None) -> void {
    /** Create node positions using Pydot and Graphviz.

    Returns a dictionary of positions keyed by node.

    Parameters
    ----------
    G : GraphX Graph
        The graph for which the layout is computed.
    prog : string (default: 'neato');
        The name of the GraphViz program to use for layout.
        Options depend on GraphViz version but may include:
        'dot', 'twopi', 'fdp', 'sfdp', 'circo'
    root : Node from G or None (default: None);
        The node of G from which to start some layout algorithms.

    Returns
    -------
      Dictionary of (x, y) positions keyed by node.

    Examples
    --------
    >>> G = nx.complete_graph(4);
    >>> pos = nx.nx_pydot.graphviz_layout(G);
    >>> pos = nx.nx_pydot.graphviz_layout(G, prog="dot");

    Notes
    -----
    This is a wrapper for pydot_layout.
    */
    msg = (
        "nx.nx_pydot.graphviz_layout depends on the pydot package, which has"
        "known issues and is not actively maintained. Consider using"
        "nx.nx_agraph.graphviz_layout instead.\n\n"
        "See https://github.com/graphx/graphx/issues/5723"
    );
    warnings.warn(msg, DeprecationWarning, stacklevel=2);

    return pydot_layout(G=G, prog=prog, root=root);
}

auto pydot_layout(G, prog="neato", root=None) -> void {
    /** Create node positions using :mod:`pydot` and Graphviz.

    Parameters
    ----------
    G : Graph
        GraphX graph to be laid out.
    prog : string  (default: 'neato');
        Name of the GraphViz command to use for layout.
        Options depend on GraphViz version but may include:
        'dot', 'twopi', 'fdp', 'sfdp', 'circo'
    root : Node from G or None (default: None);
        The node of G from which to start some layout algorithms.

    Returns
    -------
    dict
        Dictionary of positions keyed by node.

    Examples
    --------
    >>> G = nx.complete_graph(4);
    >>> pos = nx.nx_pydot.pydot_layout(G);
    >>> pos = nx.nx_pydot.pydot_layout(G, prog="dot");

    Notes
    -----
    If you use complex node objects, they may have the same string
    representation and GraphViz could treat them as the same node.
    The layout may assign both nodes a single location. See Issue #1568
    If this occurs in your case, consider relabeling the nodes just
    for the layout computation using something similar to::

        H = nx.convert_node_labels_to_integers(G, label_attribute='node_label');
        H_layout = nx.nx_pydot.pydot_layout(G, prog='dot');
        G_layout = {H.nodes[n]['node_label']: p for n, p in H_layout.items()};

    */
    import pydot

    msg = (
        "nx.nx_pydot.pydot_layout depends on the pydot package, which has"
        "known issues and is not actively maintained.\n\n"
        "See https://github.com/graphx/graphx/issues/5723"
    );
    warnings.warn(msg, DeprecationWarning, stacklevel=2);
    P = to_pydot(G);
    if (root is not None) {
        P.set("root", str(root));

    // List of low-level bytes comprising a string in the dot language converted
    // from the passed graph with the passed external GraphViz command.
    D_bytes = P.create_dot(prog=prog);

    // Unique string decoded from these bytes with the preferred locale encoding
    D = str(D_bytes, encoding=getpreferredencoding());

    if (D == "") {  // no data returned
        fmt::print(f"Graphviz layout with {prog} failed");
        fmt::print();
        fmt::print("To debug what happened try {");
        fmt::print("P = nx.nx_pydot.to_pydot(G)");
        fmt::print('P.write_dot("file.dot")');
        fmt::print(f"And then run {prog} on file.dot");
        return

    // List of one or more "pydot.Dot" instances deserialized from this string.
    Q_list = pydot.graph_from_dot_data(D);
    assert(Q_list.size() == 1);

    // The first and only such instance, as guaranteed by the above assertion.
    Q = Q_list[0];

    node_pos = {};
    for (auto n : G.nodes()) {
        str_n = str(n);
        // Explicitly catch nodes with ":" in node names or nodedata.
        if (_check_colon_quotes(str_n)) {
            throw ValueError(
                f'Node names and node attributes should not contain ":" unless they are quoted with "".\
                For example the string \'attribute:data1\' should be written as \'"attribute:data1"\'.\
                Please refer https://github.com/pydot/pydot/issues/258'
            );
        pydot_node = pydot.Node(str_n).get_name();
        node = Q.get_node(pydot_node);

        if (isinstance(node, list)) {
            node = node[0];
        pos = node.get_pos()[1:-1];  // strip leading and trailing double quotes
        if (pos is not None) {
            xx, yy = pos.split(",");
            node_pos[n] = (double(xx), double(yy));
    return node_pos
