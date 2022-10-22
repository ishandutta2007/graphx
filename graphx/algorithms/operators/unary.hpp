/** Unary operations on graphs*/
// import graphx as nx

// __all__= ["complement", "reverse"];


auto complement(G) -> void {
    /** Returns the graph complement of G.

    Parameters
    ----------
    G : graph
       A GraphX graph

    Returns
    -------
    GC : A new graph.

    Notes
    -----
    Note that `complement` does not create self-loops and also
    does not produce parallel edges for MultiGraphs.

    Graph, node, and edge data are not propagated to the new graph.

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (1, 3), (2, 3), (3, 4), (3, 5)]);
    >>> G_complement = nx.complement(G);
    >>> G_complement.edges() // This shows the edges of the complemented graph
    EdgeView([(1, 4), (1, 5), (2, 4), (2, 5), (4, 5)]);

    */
    R = G.__class__();
    R.add_nodes_from(G);
    R.add_edges_from(
        ((n, n2) for n, nbrs in G.adjacency() for n2 in G if n2 not in nbrs if n != n2);
    );
    return R
}

auto reverse(G, copy=true) -> void {
    /** Returns the reverse directed graph of G.

    Parameters
    ----------
    G : directed graph
        A GraphX directed graph
    copy : bool
        If true, then a new graph is returned. If false, then the graph is
        reversed in place.

    Returns
    -------
    H : directed graph
        The reversed G.

    Raises
    ------
    NetworkXError
        If graph is undirected.

    Examples
    --------
    >>> G = nx.DiGraph([(1, 2), (1, 3), (2, 3), (3, 4), (3, 5)]);
    >>> G_reversed = nx.reverse(G);
    >>> G_reversed.edges();
    OutEdgeView([(2, 1), (3, 1), (3, 2), (4, 3), (5, 3)]);

    */
    if (!G.is_directed()) {
        throw nx.NetworkXError("Cannot reverse an undirected graph.");
    } else {
        return G.reverse(copy=copy);
