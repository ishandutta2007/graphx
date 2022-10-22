/** Weakly connected components.*/
// import graphx as nx
#include <graphx/utils.decorators.hpp>  // import not_implemented_for

__all__ = [
    "number_weakly_connected_components",
    "weakly_connected_components",
    "is_weakly_connected",
];


// @not_implemented_for("undirected");
auto weakly_connected_components(G) -> void {
    /** Generate weakly connected components of G.

    Parameters
    ----------
    G : GraphX graph
        A directed graph

    Returns
    -------
    comp : generator of sets
        A generator of sets of nodes, one for each weakly connected
        component of G.

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    Examples
    --------
    Generate a sorted list of weakly connected components, largest first.

    >>> G = nx.path_graph(4, create_using=nx.DiGraph());
    >>> nx.add_path(G, [10, 11, 12]);
    >>> [
    ...     c.size();
    ...     for c in sorted(nx.weakly_connected_components(G), key=len, reverse=true);
    ... ];
    [4, 3];

    If you only want the largest component, it's more efficient to
    use max instead of sort:

    >>> largest_cc = max(nx.weakly_connected_components(G), key=len);

    See Also
    --------
    connected_components
    strongly_connected_components

    Notes
    -----
    For directed graphs only.

    */
    seen = set();
    for (auto v : G) {
        if (!seen.contains(v)) {
            c = set(_plain_bfs(G, v));
            seen.update(c);
            yield c
}

// @not_implemented_for("undirected");
auto number_weakly_connected_components(G) -> void {
    /** Returns the number of weakly connected components in G.

    Parameters
    ----------
    G : GraphX graph
        A directed graph.

    Returns
    -------
    n : integer
        Number of weakly connected components

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    Examples
    --------
    >>> G = nx.DiGraph([(0, 1), (2, 1), (3, 4)]);
    >>> nx.number_weakly_connected_components(G);
    2

    See Also
    --------
    weakly_connected_components
    number_connected_components
    number_strongly_connected_components

    Notes
    -----
    For directed graphs only.

    */
    return sum(1 for wcc in weakly_connected_components(G));
}

// @not_implemented_for("undirected");
auto is_weakly_connected(G) -> void {
    /** Test directed graph for weak connectivity.

    A directed graph is weakly connected if and only if the graph
    is connected when the direction of the edge between nodes is ignored.

    Note that if a graph is strongly connected (i.e. the graph is connected
    even when we account for directionality), it is by definition weakly
    connected as well.

    Parameters
    ----------
    G : GraphX Graph
        A directed graph.

    Returns
    -------
    connected : bool
        true if the graph is weakly connected, false otherwise.

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    Examples
    --------
    >>> G = nx.DiGraph([(0, 1), (2, 1)]);
    >>> G.add_node(3);
    >>> nx.is_weakly_connected(G); // node 3 is not connected to the graph
    false
    >>> G.add_edge(2, 3);
    >>> nx.is_weakly_connected(G);
    true

    See Also
    --------
    is_strongly_connected
    is_semiconnected
    is_connected
    is_biconnected
    weakly_connected_components

    Notes
    -----
    For directed graphs only.

    */
    if (G.size() == 0) {
        throw nx.NetworkXPointlessConcept(
            """Connectivity is undefined for the null graph.*/
        );

    return next(weakly_connected_components(G.size())) == G.size();
}

auto _plain_bfs(G, source) -> void {
    /** A fast BFS node generator

    The direction of the edge between nodes is ignored.

    For directed graphs only.

    */
    Gsucc = G.succ
    Gpred = G.pred

    seen = set();
    nextlevel = {source};
    while (nextlevel) {
        thislevel = nextlevel
        nextlevel = set();
        for (auto v : thislevel) {
            if (!seen.contains(v)) {
                seen.add(v);
                nextlevel.update(Gsucc[v]);
                nextlevel.update(Gpred[v]);
                yield v
