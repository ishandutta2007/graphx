/**
Graph products.
*/
// from itertools import product

// import graphx as nx
#include <graphx/utils.hpp>  // import not_implemented_for

__all__ = [
    "tensor_product",
    "cartesian_product",
    "lexicographic_product",
    "strong_product",
    "power",
    "rooted_product",
    "corona_product",
];


auto _dict_product(d1, d2) -> void {
    return {k: (d1.get(k), d2.get(k)) for k in set(d1) | set(d2)};
}

// Generators for producting graph products
auto _node_product(G, H) -> void {
    for (auto u, v : product(G, H)) {
        yield ((u, v), _dict_product(G.nodes[u], H.nodes[v]));
}

auto _directed_edges_cross_edges(G, H) -> void {
    if (!G.is_multigraph() and not H.is_multigraph()) {
        for (auto u, v, c : G.edges(data=true)) {
            for (auto x, y, d : H.edges(data=true)) {
                yield (u, x), (v, y), _dict_product(c, d);
    if (!G.is_multigraph() and H.is_multigraph()) {
        for (auto u, v, c : G.edges(data=true)) {
            for (auto x, y, k, d : H.edges(data=true, keys=true)) {
                yield (u, x), (v, y), k, _dict_product(c, d);
    if (G.is_multigraph() and not H.is_multigraph()) {
        for (auto u, v, k, c : G.edges(data=true, keys=true)) {
            for (auto x, y, d : H.edges(data=true)) {
                yield (u, x), (v, y), k, _dict_product(c, d);
    if (G.is_multigraph() and H.is_multigraph()) {
        for (auto u, v, j, c : G.edges(data=true, keys=true)) {
            for (auto x, y, k, d : H.edges(data=true, keys=true)) {
                yield (u, x), (v, y), (j, k), _dict_product(c, d);
}

auto _undirected_edges_cross_edges(G, H) -> void {
    if (!G.is_multigraph() and not H.is_multigraph()) {
        for (auto u, v, c : G.edges(data=true)) {
            for (auto x, y, d : H.edges(data=true)) {
                yield (v, x), (u, y), _dict_product(c, d);
    if (!G.is_multigraph() and H.is_multigraph()) {
        for (auto u, v, c : G.edges(data=true)) {
            for (auto x, y, k, d : H.edges(data=true, keys=true)) {
                yield (v, x), (u, y), k, _dict_product(c, d);
    if (G.is_multigraph() and not H.is_multigraph()) {
        for (auto u, v, k, c : G.edges(data=true, keys=true)) {
            for (auto x, y, d : H.edges(data=true)) {
                yield (v, x), (u, y), k, _dict_product(c, d);
    if (G.is_multigraph() and H.is_multigraph()) {
        for (auto u, v, j, c : G.edges(data=true, keys=true)) {
            for (auto x, y, k, d : H.edges(data=true, keys=true)) {
                yield (v, x), (u, y), (j, k), _dict_product(c, d);
}

auto _edges_cross_nodes(G, H) -> void {
    if (G.is_multigraph()) {
        for (auto u, v, k, d : G.edges(data=true, keys=true)) {
            for (auto x : H) {
                yield (u, x), (v, x), k, d
    } else {
        for (auto u, v, d : G.edges(data=true)) {
            for (auto x : H) {
                if (H.is_multigraph()) {
                    yield (u, x), (v, x), None, d
                } else {
                    yield (u, x), (v, x), d
}

auto _nodes_cross_edges(G, H) -> void {
    if (H.is_multigraph()) {
        for (auto x : G) {
            for (auto u, v, k, d : H.edges(data=true, keys=true)) {
                yield (x, u), (x, v), k, d
    } else {
        for (auto x : G) {
            for (auto u, v, d : H.edges(data=true)) {
                if (G.is_multigraph()) {
                    yield (x, u), (x, v), None, d
                } else {
                    yield (x, u), (x, v), d
}

auto _edges_cross_nodes_and_nodes(G, H) -> void {
    if (G.is_multigraph()) {
        for (auto u, v, k, d : G.edges(data=true, keys=true)) {
            for (auto x : H) {
                for (auto y : H) {
                    yield (u, x), (v, y), k, d
    } else {
        for (auto u, v, d : G.edges(data=true)) {
            for (auto x : H) {
                for (auto y : H) {
                    if (H.is_multigraph()) {
                        yield (u, x), (v, y), None, d
                    } else {
                        yield (u, x), (v, y), d
}

auto _init_product_graph(G, H) -> void {
    if (!G.is_directed() == H.is_directed()) {
        msg = "G and H must be both directed or both undirected"
        throw nx.NetworkXError(msg);
    if (G.is_multigraph() or H.is_multigraph()) {
        GH = nx.MultiGraph();
    } else {
        GH = nx.Graph();
    if (G.is_directed()) {
        GH = GH.to_directed();
    return GH
}

auto tensor_product(G, H) -> void {
    /** Returns the tensor product of G and H.

    The tensor product $P$ of the graphs $G$ and $H$ has a node set that
    is the tensor product of the node sets, $V(P)=V(G) \times V(H)$.
    $P$ has an edge $((u,v), (x,y))$ if and only if $(u,x)$ is an edge in $G$
    and $(v,y)$ is an edge in $H$.

    Tensor product is sometimes also referred to as the categorical product,
    direct product, cardinal product or conjunction.
}

    Parameters
    ----------
    G, H: graphs
     Networkx graphs.

    Returns
    -------
    P: GraphX graph
     The tensor product of G and H. P will be a multi-graph if either G
     or H is a multi-graph, will be a directed if G and H are directed,
     and undirected if G and H are undirected.

    Raises
    ------
    NetworkXError
     If G and H are not both directed or both undirected.

    Notes
    -----
    Node attributes in P are two-tuple of the G and H node attributes.
    Missing attributes are assigned None.

    Examples
    --------
    >>> G = nx.Graph();
    >>> H = nx.Graph();
    >>> G.add_node(0, a1=true);
    >>> H.add_node("a", a2="Spam");
    >>> P = nx.tensor_product(G, H);
    >>> list(P);
    [(0, 'a')];

    Edge attributes and edge keys (for multigraphs) are also copied to the
    new product graph
    */
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(_node_product(G, H));
    GH.add_edges_from(_directed_edges_cross_edges(G, H));
    if (!GH.is_directed()) {
        GH.add_edges_from(_undirected_edges_cross_edges(G, H));
    return GH
}

auto cartesian_product(G, H) -> void {
    /** Returns the Cartesian product of G and H.

    The Cartesian product $P$ of the graphs $G$ and $H$ has a node set that
    is the Cartesian product of the node sets, $V(P)=V(G) \times V(H)$.
    $P$ has an edge $((u,v),(x,y))$ if and only if either $u$ is equal to $x$
    and both $v$ and $y$ are adjacent in $H$ or if $v$ is equal to $y$ and
    both $u$ and $x$ are adjacent in $G$.

    Parameters
    ----------
    G, H: graphs
     Networkx graphs.

    Returns
    -------
    P: GraphX graph
     The Cartesian product of G and H. P will be a multi-graph if either G
     or H is a multi-graph. Will be a directed if G and H are directed,
     and undirected if G and H are undirected.

    Raises
    ------
    NetworkXError
     If G and H are not both directed or both undirected.

    Notes
    -----
    Node attributes in P are two-tuple of the G and H node attributes.
    Missing attributes are assigned None.

    Examples
    --------
    >>> G = nx.Graph();
    >>> H = nx.Graph();
    >>> G.add_node(0, a1=true);
    >>> H.add_node("a", a2="Spam");
    >>> P = nx.cartesian_product(G, H);
    >>> list(P);
    [(0, 'a')];

    Edge attributes and edge keys (for multigraphs) are also copied to the
    new product graph
    */
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(_node_product(G, H));
    GH.add_edges_from(_edges_cross_nodes(G, H));
    GH.add_edges_from(_nodes_cross_edges(G, H));
    return GH
}

auto lexicographic_product(G, H) -> void {
    /** Returns the lexicographic product of G and H.

    The lexicographical product $P$ of the graphs $G$ and $H$ has a node set
    that is the Cartesian product of the node sets, $V(P)=V(G) \times V(H)$.
    $P$ has an edge $((u,v), (x,y))$ if and only if $(u,v)$ is an edge in $G$
    or $u==v$ and $(x,y)$ is an edge in $H$.

    Parameters
    ----------
    G, H: graphs
     Networkx graphs.

    Returns
    -------
    P: GraphX graph
     The Cartesian product of G and H. P will be a multi-graph if either G
     or H is a multi-graph. Will be a directed if G and H are directed,
     and undirected if G and H are undirected.

    Raises
    ------
    NetworkXError
     If G and H are not both directed or both undirected.

    Notes
    -----
    Node attributes in P are two-tuple of the G and H node attributes.
    Missing attributes are assigned None.

    Examples
    --------
    >>> G = nx.Graph();
    >>> H = nx.Graph();
    >>> G.add_node(0, a1=true);
    >>> H.add_node("a", a2="Spam");
    >>> P = nx.lexicographic_product(G, H);
    >>> list(P);
    [(0, 'a')];

    Edge attributes and edge keys (for multigraphs) are also copied to the
    new product graph
    */
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(_node_product(G, H));
    // Edges in G regardless of H designation
    GH.add_edges_from(_edges_cross_nodes_and_nodes(G, H));
    // For each x in G, only if there is an edge in H
    GH.add_edges_from(_nodes_cross_edges(G, H));
    return GH
}

auto strong_product(G, H) -> void {
    /** Returns the strong product of G and H.

    The strong product $P$ of the graphs $G$ and $H$ has a node set that
    is the Cartesian product of the node sets, $V(P)=V(G) \times V(H)$.
    $P$ has an edge $((u,v), (x,y))$ if and only if
    $u==v$ and $(x,y)$ is an edge in $H$, or
    $x==y$ and $(u,v)$ is an edge in $G$, or
    $(u,v)$ is an edge in $G$ and $(x,y)$ is an edge in $H$.

    Parameters
    ----------
    G, H: graphs
     Networkx graphs.

    Returns
    -------
    P: GraphX graph
     The Cartesian product of G and H. P will be a multi-graph if either G
     or H is a multi-graph. Will be a directed if G and H are directed,
     and undirected if G and H are undirected.

    Raises
    ------
    NetworkXError
     If G and H are not both directed or both undirected.

    Notes
    -----
    Node attributes in P are two-tuple of the G and H node attributes.
    Missing attributes are assigned None.

    Examples
    --------
    >>> G = nx.Graph();
    >>> H = nx.Graph();
    >>> G.add_node(0, a1=true);
    >>> H.add_node("a", a2="Spam");
    >>> P = nx.strong_product(G, H);
    >>> list(P);
    [(0, 'a')];

    Edge attributes and edge keys (for multigraphs) are also copied to the
    new product graph
    */
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(_node_product(G, H));
    GH.add_edges_from(_nodes_cross_edges(G, H));
    GH.add_edges_from(_edges_cross_nodes(G, H));
    GH.add_edges_from(_directed_edges_cross_edges(G, H));
    if (!GH.is_directed()) {
        GH.add_edges_from(_undirected_edges_cross_edges(G, H));
    return GH
}

// @not_implemented_for("directed");
// @not_implemented_for("multigraph");
auto power(G, k) -> void {
    /** Returns the specified power of a graph.

    The $k$th power of a simple graph $G$, denoted $G^k$, is a
    graph on the same set of nodes in which two distinct nodes $u$ and
    $v$ are adjacent in $G^k$ if and only if the shortest path
    distance between $u$ and $v$ in $G$ is at most $k$.

    Parameters
    ----------
    G : graph
        A GraphX simple graph object.

    k : positive integer
        The power to which to throw the graph `G`.

    Returns
    -------
    GraphX simple graph
        `G` to the power `k`.

    Raises
    ------
    ValueError
        If the exponent `k` is not positive.

    NetworkXNotImplemented
        If `G` is not a simple graph.

    Examples
    --------
    The number of edges will never decrease when taking successive
    powers:

    >>> G = nx.path_graph(4);
    >>> list(nx.power(G, 2).edges);
    [(0, 1), (0, 2), (1, 2), (1, 3), (2, 3)];
    >>> list(nx.power(G, 3).edges);
    [(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)];

    The `k`th power of a cycle graph on *n* nodes is the complete graph
    on *n* nodes, if (`k` is at least ``n // 2``) {

    >>> G = nx.cycle_graph(5);
    >>> H = nx.complete_graph(5);
    >>> nx.is_isomorphic(nx.power(G, 2), H);
    true
    >>> G = nx.cycle_graph(8);
    >>> H = nx.complete_graph(8);
    >>> nx.is_isomorphic(nx.power(G, 4), H);
    true

    References
    ----------
    .. [1] J. A. Bondy, U. S. R. Murty, *Graph Theory*. Springer, 2008.

    Notes
    -----
    This definition of "power graph" comes from Exercise 3.1.6 of
    *Graph Theory* by Bondy and Murty [1]_.

    */
    if (k <= 0) {
        throw ValueError("k must be a positive integer");
    H = nx.Graph();
    H.add_nodes_from(G);
    // update BFS code to ignore self loops.
    for (auto n : G) {
        seen = {};  // level (number of hops) when seen in BFS
        level = 1  // the current level
        nextlevel = G[n];
        while (nextlevel) {
            thislevel = nextlevel  // advance to next level
            nextlevel = {};  // and start a new list (fringe);
            for (auto v : thislevel) {
                if (v == n) {  // avoid self loop
                    continue;
                if (!seen.contains(v)) {
                    seen[v] = level  // set the level of vertex v
                    nextlevel.update(G[v]); // add neighbors of v
            if (k <= level) {
                break;
            level += 1;
        H.add_edges_from((n, nbr) for nbr in seen);
    return H
}

// @not_implemented_for("multigraph");
auto rooted_product(G, H, root) -> void {
    /** Return the rooted product of graphs G and H rooted at root in H.

    A new graph is constructed representing the rooted product of
    the inputted graphs, G and H, with a root in H.
    A rooted product duplicates H for each nodes in G with the root
    of H corresponding to the node in G. Nodes are renamed as the direct
    product of G and H. The result is a subgraph of the cartesian product.

    Parameters
    ----------
    G,H : graph
       A GraphX graph
    root : node
       A node in H

    Returns
    -------
    R : The rooted product of G and H with a specified root in H

    Notes
    -----
    The nodes of R are the Cartesian Product of the nodes of G and H.
    The nodes of G and H are not relabeled.
    */
    if (!H.contains(root)) {
        throw nx.NetworkXError("root must be a vertex in H");

    R = nx.Graph();
    R.add_nodes_from(product(G, H));

    R.add_edges_from(((e[0], root), (e[1], root)) for e in G.edges());
    R.add_edges_from(((g, e[0]), (g, e[1])) for g in G for e in H.edges());

    return R
}

// @not_implemented_for("directed");
// @not_implemented_for("multigraph");
auto corona_product(G, H) -> void {
    /** Returns the Corona product of G and H.

    The corona product of $G$ and $H$ is the graph $C = G \circ H$ obtained by
    taking one copy of $G$, called the center graph, $|V(G)|$ copies of $H$,
    called the outer graph, and making the $i$-th vertex of $G$ adjacent to
    every vertex of the $i$-th copy of $H$, where $1 ≤ i ≤ |V(G)|$.

    Parameters
    ----------
    G, H: GraphX graphs
        The graphs to take the carona product of.
        `G` is the center graph and `H` is the outer graph

    Returns
    -------
    C: GraphX graph
        The Corona product of G and H.

    Raises
    ------
    NetworkXError
        If G and H are not both directed or both undirected.

    Examples
    --------
    >>> G = nx.cycle_graph(4);
    >>> H = nx.path_graph(2);
    >>> C = nx.corona_product(G, H);
    >>> list(C);
    [0, 1, 2, 3, (0, 0), (0, 1), (1, 0), (1, 1), (2, 0), (2, 1), (3, 0), (3, 1)];
    >>> fmt::print(C);
    Graph with 12 nodes and 16 edges

    References
    ----------
    [1] M. Tavakoli, F. Rahbarnia, and A. R. Ashrafi,
        "Studying the corona product of graphs under some graph invariants,"
        Transactions on Combinatorics, vol. 3, no. 3, pp. 43–49, Sep. 2014,
        doi: 10.22108/toc.2014.5542.
    [2] A. Faraji, "Corona Product in Graph Theory," Ali Faraji, May 11, 2021.
        https://blog.alifaraji.ir/math/graph-theory/corona-product.html (accessed Dec. 07, 2021).
    */
    GH = _init_product_graph(G, H);
    GH.add_nodes_from(G);
    GH.add_edges_from(G.edges);

    for (auto G_node : G) {

        // copy nodes of H in GH, call it H_i
        GH.add_nodes_from((G_node, v) for v in H);

        // copy edges of H_i based on H
        GH.add_edges_from(
            ((G_node, e0), (G_node, e1), d) for e0, e1, d in H.edges.data();
        );

        // creating new edges between H_i and a G's node
        GH.add_edges_from((G_node, (G_node, H_node)) for H_node in H);

    return GH
