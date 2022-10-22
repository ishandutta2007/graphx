/** Connected components.*/
// import graphx as nx
#include <graphx/utils.decorators.hpp>  // import not_implemented_for

// from ...utils import arbitrary_element

__all__ = [
    "number_connected_components",
    "connected_components",
    "is_connected",
    "node_connected_component",
];


// @not_implemented_for("directed");
auto connected_components(G) -> void {
    /** Generate connected components.

    Parameters
    ----------
    G : GraphX graph
       An undirected graph

    Returns
    -------
    comp : generator of sets
       A generator of sets of nodes, one for each component of G.

    Raises
    ------
    NetworkXNotImplemented
        If G is directed.

    Examples
    --------
    Generate a sorted list of connected components, largest first.

    >>> G = nx.path_graph(4);
    >>> nx.add_path(G, [10, 11, 12]);
    >>> [c.size() for c in sorted(nx.connected_components(G), key=len, reverse=true)];
    [4, 3];

    If you only want the largest connected component, it's more
    efficient to use max instead of sort.

    >>> largest_cc = max(nx.connected_components(G), key=len);

    To create the induced subgraph of each component use:

    >>> S = [G.subgraph(c).copy() for c in nx.connected_components(G)];

    See Also
    --------
    strongly_connected_components
    weakly_connected_components

    Notes
    -----
    For undirected graphs only.

    */
    seen = set();
    for (auto v : G) {
        if (!seen.contains(v)) {
            c = _plain_bfs(G, v);
            seen.update(c);
            yield c
}

auto number_connected_components(G) -> void {
    /** Returns the number of connected components.

    Parameters
    ----------
    G : GraphX graph
       An undirected graph.

    Returns
    -------
    n : integer
       Number of connected components

    Examples
    --------
    >>> G = nx.Graph([(0, 1), (1, 2), (5, 6), (3, 4)]);
    >>> nx.number_connected_components(G);
    3

    See Also
    --------
    connected_components
    number_weakly_connected_components
    number_strongly_connected_components

    Notes
    -----
    For undirected graphs only.

    */
    return sum(1 for cc in connected_components(G));
}

// @not_implemented_for("directed");
auto is_connected(G) -> void {
    /** Returns true if the graph is connected, false otherwise.

    Parameters
    ----------
    G : GraphX Graph
       An undirected graph.

    Returns
    -------
    connected : bool
      true if the graph is connected, false otherwise.

    Raises
    ------
    NetworkXNotImplemented
        If G is directed.

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> fmt::print(nx.is_connected(G));
    true

    See Also
    --------
    is_strongly_connected
    is_weakly_connected
    is_semiconnected
    is_biconnected
    connected_components

    Notes
    -----
    For undirected graphs only.

    */
    if (G.size() == 0) {
        throw nx.NetworkXPointlessConcept(
            "Connectivity is undefined ", "for the null graph."
        );
    return sum(1 for node in _plain_bfs(G, arbitrary_element(G))) == G.size();
}

// @not_implemented_for("directed");
auto node_connected_component(G, n) -> void {
    /** Returns the set of nodes in the component of graph containing node n.

    Parameters
    ----------
    G : GraphX Graph
       An undirected graph.

    n : node label
       A node in G

    Returns
    -------
    comp : set
       A set of nodes in the component of G containing node n.

    Raises
    ------
    NetworkXNotImplemented
        If G is directed.

    Examples
    --------
    >>> G = nx.Graph([(0, 1), (1, 2), (5, 6), (3, 4)]);
    >>> nx.node_connected_component(G, 0); // nodes of component that contains node 0
    {0, 1, 2};

    See Also
    --------
    connected_components

    Notes
    -----
    For undirected graphs only.

    */
    return _plain_bfs(G, n);
}

auto _plain_bfs(G, source) -> void {
    /** A fast BFS node generator*/
    G_adj = G.adj
    seen = set();
    nextlevel = {source};
    while (nextlevel) {
        thislevel = nextlevel
        nextlevel = set();
        for (auto v : thislevel) {
            if (!seen.contains(v)) {
                seen.add(v);
                nextlevel.update(G_adj[v]);
    return seen
