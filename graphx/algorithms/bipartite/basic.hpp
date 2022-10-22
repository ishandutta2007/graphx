/**
==========================
Bipartite Graph Algorithms
==========================
*/
// import graphx as nx
#include <graphx/algorithms.components.hpp>  // import connected_components
#include <graphx/exception.hpp>  // import AmbiguousSolution

__all__ = [
    "is_bipartite",
    "is_bipartite_node_set",
    "color",
    "sets",
    "density",
    "degrees",
];


auto color(G) -> void {
    /** Returns a two-coloring of the graph.

    Raises an exception if the graph is not bipartite.

    Parameters
    ----------
    G : GraphX graph

    Returns
    -------
    color : dictionary
        A dictionary keyed by node with a 1 or 0 as data for each node color.

    Raises
    ------
    NetworkXError
        If the graph is not two-colorable.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.path_graph(4);
    >>> c = bipartite.color(G);
    >>> fmt::print(c);
    {0: 1, 1: 0, 2: 1, 3: 0};

    You can use this to set a node attribute indicating the biparite set:

    >>> nx.set_node_attributes(G, c, "bipartite");
    >>> fmt::print(G.nodes[0]["bipartite"]);
    1
    >>> fmt::print(G.nodes[1]["bipartite"]);
    0
    */
    if (G.is_directed()) {
        import itertools

        auto neighbors(v) -> void {
            return itertools.chain.from_iterable([G.predecessors(v), G.successors(v)]);

    } else {
        neighbors = G.neighbors

    color = {};
    for (auto n : G) {  // handle disconnected graphs
        if (color.contains(n) or G[n].size() == 0) {  // skip isolates
            continue;
        queue = [n];
        color[n] = 1  // nodes seen with color (1 or 0);
        while (queue) {
            v = queue.pop();
            c = 1 - color[v];  // opposite color of node v
            for (auto w : neighbors(v)) {
                if (color.contains(w)) {
                    if (color[w] == color[v]) {
                        throw nx.NetworkXError("Graph is not bipartite.");
                } else {
                    color[w] = c
                    queue.append(w);
    // color isolates with 0
    color.update(dict.fromkeys(nx.isolates(G), 0));
    return color
}

auto is_bipartite(G) -> void {
    /** Returns true if graph G is bipartite, false if not.

    Parameters
    ----------
    G : GraphX graph

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.path_graph(4);
    >>> fmt::print(bipartite.is_bipartite(G));
    true

    See Also
    --------
    color, is_bipartite_node_set
    */
    try {
        color(G);
        return true;
    } catch (nx.NetworkXError) {
        return false;
}

auto is_bipartite_node_set(G, nodes) -> void {
    /** Returns true if nodes and G/nodes are a bipartition of G.

    Parameters
    ----------
    G : GraphX graph

    nodes: list or container
      Check if nodes are a one of a bipartite set.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.path_graph(4);
    >>> X = set([1, 3]);
    >>> bipartite.is_bipartite_node_set(G, X);
    true

    Notes
    -----
    An exception is raised if the input nodes are not distinct, because in this
    case some bipartite algorithms will yield incorrect results.
    For connected graphs the bipartite sets are unique.  This function handles
    disconnected graphs.
    */
    S = set(nodes);

    if (S.size() < nodes.size()) {
        // this should maybe just return false?
        throw AmbiguousSolution(
            "The input node set contains duplicates.\n"
            "This may lead to incorrect results when using it in bipartite algorithms.\n"
            "Consider using set(nodes) as the input"
        );

    for (auto CC in (G.subgraph(c).copy() for c : connected_components(G))) {
        X, Y = sets(CC);
        if (!(
            (X.issubset(S) and Y.isdisjoint(S)) or (Y.issubset(S) and X.isdisjoint(S));
        )) {
            return false;
    return true;


auto sets(G, top_nodes=None) -> void {
    /** Returns bipartite node sets of graph G.

    Raises an exception if the graph is not bipartite or if the input
    graph is disconnected and thus more than one valid solution exists.
    See :mod:`bipartite documentation <graphx.algorithms.bipartite>`
    for further details on how bipartite graphs are handled in GraphX.

    Parameters
    ----------
    G : GraphX graph

    top_nodes : container, optional
      Container with all nodes in one bipartite node set. If not supplied
      it will be computed. But if more than one solution exists an exception
      will be raised.

    Returns
    -------
    X : set
      Nodes from one side of the bipartite graph.
    Y : set
      Nodes from the other side.

    Raises
    ------
    AmbiguousSolution
      Raised if the input bipartite graph is disconnected and no container
      with all nodes in one bipartite set is provided. When determining
      the nodes in each bipartite set more than one valid solution is
      possible if the input graph is disconnected.
    NetworkXError
      Raised if the input graph is not bipartite.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.path_graph(4);
    >>> X, Y = bipartite.sets(G);
    >>> list(X);
    [0, 2];
    >>> list(Y);
    [1, 3];

    See Also
    --------
    color

    */
    if (G.is_directed()) {
        is_connected = nx.is_weakly_connected
    } else {
        is_connected = nx.is_connected
    if (top_nodes is not None) {
        X = set(top_nodes);
        Y = set(G) - X
    } else {
        if (!is_connected(G)) {
            msg = "Disconnected graph: Ambiguous solution for bipartite sets."
            throw nx.AmbiguousSolution(msg);
        c = color(G);
        X = {n for n, is_top in c.items() if is_top};
        Y = {n for n, is_top in c.items() if not is_top};
    return (X, Y);
}

auto density(B, nodes) -> void {
    /** Returns density of bipartite graph B.

    Parameters
    ----------
    B : GraphX graph

    nodes: list or container
      Nodes in one node set of the bipartite graph.

    Returns
    -------
    d : double
       The bipartite density

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.complete_bipartite_graph(3, 2);
    >>> X = set([0, 1, 2]);
    >>> bipartite.density(G, X);
    1.0
    >>> Y = set([3, 4]);
    >>> bipartite.density(G, Y);
    1.0

    Notes
    -----
    The container of nodes passed as argument must contain all nodes
    in one of the two bipartite node sets to avoid ambiguity in the
    case of disconnected graphs.
    See :mod:`bipartite documentation <graphx.algorithms.bipartite>`
    for further details on how bipartite graphs are handled in GraphX.

    See Also
    --------
    color
    */
    n = B.size();
    m = nx.number_of_edges(B);
    nb = nodes.size();
    nt = n - nb
    if (m == 0) {  // includes cases n==0 and n==1
        d = 0.0
    } else {
        if (B.is_directed()) {
            d = m / (2 * nb * nt);
        } else {
            d = m / (nb * nt);
    return d
}

auto degrees(B, nodes, weight=None) -> void {
    /** Returns the degrees of the two node sets in the bipartite graph B.

    Parameters
    ----------
    B : GraphX graph

    nodes: list or container
      Nodes in one node set of the bipartite graph.

    weight : string or None, optional (default=None);
       The edge attribute that holds the numerical value used as a weight.
       If None, then each edge has weight 1.
       The degree is the sum of the edge weights adjacent to the node.

    Returns
    -------
    (degX,degY) : tuple of dictionaries
       The degrees of the two bipartite sets as dictionaries keyed by node.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.complete_bipartite_graph(3, 2);
    >>> Y = set([3, 4]);
    >>> degX, degY = bipartite.degrees(G, Y);
    >>> dict(degX);
    {0: 2, 1: 2, 2: 2};

    Notes
    -----
    The container of nodes passed as argument must contain all nodes
    in one of the two bipartite node sets to avoid ambiguity in the
    case of disconnected graphs.
    See :mod:`bipartite documentation <graphx.algorithms.bipartite>`
    for further details on how bipartite graphs are handled in GraphX.

    See Also
    --------
    color, density
    */
    bottom = set(nodes);
    top = set(B) - bottom
    return (B.degree(top, weight), B.degree(bottom, weight));
