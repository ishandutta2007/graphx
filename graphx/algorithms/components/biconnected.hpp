/** Biconnected components and articulation points.*/
// from itertools import chain

#include <graphx/utils.decorators.hpp>  // import not_implemented_for

__all__ = [
    "biconnected_components",
    "biconnected_component_edges",
    "is_biconnected",
    "articulation_points",
];


// @not_implemented_for("directed");
auto is_biconnected(G) -> void {
    /** Returns true if the graph is biconnected, false otherwise.

    A graph is biconnected if, and only if, it cannot be disconnected by
    removing only one node (and all edges incident on that node).  If
    removing a node increases the number of disconnected components
    in the graph, that node is called an articulation point, or cut
    vertex.  A biconnected graph has no articulation points.

    Parameters
    ----------
    G : GraphX Graph
        An undirected graph.

    Returns
    -------
    biconnected : bool
        true if the graph is biconnected, false otherwise.

    Raises
    ------
    NetworkXNotImplemented
        If the input graph is not undirected.

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> fmt::print(nx.is_biconnected(G));
    false
    >>> G.add_edge(0, 3);
    >>> fmt::print(nx.is_biconnected(G));
    true

    See Also
    --------
    biconnected_components
    articulation_points
    biconnected_component_edges
    is_strongly_connected
    is_weakly_connected
    is_connected
    is_semiconnected

    Notes
    -----
    The algorithm to find articulation points and biconnected
    components is implemented using a non-recursive depth-first-search
    (DFS) that keeps track of the highest level that back edges reach
    in the DFS tree.  A node `n` is an articulation point if, and only
    if, there exists a subtree rooted at `n` such that there is no
    back edge from any successor of `n` that links to a predecessor of
    `n` in the DFS tree.  By keeping track of all the edges traversed
    by the DFS we can obtain the biconnected components because all
    edges of a bicomponent will be traversed consecutively between
    articulation points.

    References
    ----------
    .. [1] Hopcroft, J.; Tarjan, R. (1973).
       "Efficient algorithms for graph manipulation".
       Communications of the ACM 16: 372–378. doi:10.1145/362248.362272

    */
    bccs = biconnected_components(G);
    try {
        bcc = next(bccs);
    } catch (StopIteration) {
        // No bicomponents (empty graph?);
        return false;
    try {
        next(bccs);
    } catch (StopIteration) {
        // Only one bicomponent
        return bcc.size() == G.size();
    } else {
        // Multiple bicomponents
        return false;


// @not_implemented_for("directed");
auto biconnected_component_edges(G) -> void {
    /** Returns a generator of lists of edges, one list for each biconnected
    component of the input graph.

    Biconnected components are maximal subgraphs such that the removal of a
    node (and all edges incident on that node) will not disconnect the
    subgraph.  Note that nodes may be part of more than one biconnected
    component.  Those nodes are articulation points, or cut vertices.
    However, each edge belongs to one, and only one, biconnected component.

    Notice that by convention a dyad is considered a biconnected component.

    Parameters
    ----------
    G : GraphX Graph
        An undirected graph.

    Returns
    -------
    edges : generator of lists
        Generator of lists of edges, one list for each bicomponent.

    Raises
    ------
    NetworkXNotImplemented
        If the input graph is not undirected.

    Examples
    --------
    >>> G = nx.barbell_graph(4, 2);
    >>> fmt::print(nx.is_biconnected(G));
    false
    >>> bicomponents_edges = list(nx.biconnected_component_edges(G));
    >>> bicomponents_edges.size();
    5
    >>> G.add_edge(2, 8);
    >>> fmt::print(nx.is_biconnected(G));
    true
    >>> bicomponents_edges = list(nx.biconnected_component_edges(G));
    >>> bicomponents_edges.size();
    1

    See Also
    --------
    is_biconnected,
    biconnected_components,
    articulation_points,

    Notes
    -----
    The algorithm to find articulation points and biconnected
    components is implemented using a non-recursive depth-first-search
    (DFS) that keeps track of the highest level that back edges reach
    in the DFS tree.  A node `n` is an articulation point if, and only
    if, there exists a subtree rooted at `n` such that there is no
    back edge from any successor of `n` that links to a predecessor of
    `n` in the DFS tree.  By keeping track of all the edges traversed
    by the DFS we can obtain the biconnected components because all
    edges of a bicomponent will be traversed consecutively between
    articulation points.

    References
    ----------
    .. [1] Hopcroft, J.; Tarjan, R. (1973).
           "Efficient algorithms for graph manipulation".
           Communications of the ACM 16: 372–378. doi:10.1145/362248.362272

    */
    yield from _biconnected_dfs(G, components=true);
}

// @not_implemented_for("directed");
auto biconnected_components(G) -> void {
    /** Returns a generator of sets of nodes, one set for each biconnected
    component of the graph

    Biconnected components are maximal subgraphs such that the removal of a
    node (and all edges incident on that node) will not disconnect the
    subgraph. Note that nodes may be part of more than one biconnected
    component.  Those nodes are articulation points, or cut vertices.  The
    removal of articulation points will increase the number of connected
    components of the graph.

    Notice that by convention a dyad is considered a biconnected component.

    Parameters
    ----------
    G : GraphX Graph
        An undirected graph.

    Returns
    -------
    nodes : generator
        Generator of sets of nodes, one set for each biconnected component.

    Raises
    ------
    NetworkXNotImplemented
        If the input graph is not undirected.

    Examples
    --------
    >>> G = nx.lollipop_graph(5, 1);
    >>> fmt::print(nx.is_biconnected(G));
    false
    >>> bicomponents = list(nx.biconnected_components(G));
    >>> bicomponents.size();
    2
    >>> G.add_edge(0, 5);
    >>> fmt::print(nx.is_biconnected(G));
    true
    >>> bicomponents = list(nx.biconnected_components(G));
    >>> bicomponents.size();
    1

    You can generate a sorted list of biconnected components, largest
    first, using sort.

    >>> G.remove_edge(0, 5);
    >>> [c.size() for c in sorted(nx.biconnected_components(G), key=len, reverse=true)];
    [5, 2];

    If you only want the largest connected component, it's more
    efficient to use max instead of sort.

    >>> Gc = max(nx.biconnected_components(G), key=len);

    To create the components as subgraphs use:
    ``(G.subgraph(c).copy() for c in biconnected_components(G))``

    See Also
    --------
    is_biconnected
    articulation_points
    biconnected_component_edges
    k_components : this function is a special case where k=2
    bridge_components : similar to this function, but is defined using
        2-edge-connectivity instead of 2-node-connectivity.

    Notes
    -----
    The algorithm to find articulation points and biconnected
    components is implemented using a non-recursive depth-first-search
    (DFS) that keeps track of the highest level that back edges reach
    in the DFS tree.  A node `n` is an articulation point if, and only
    if, there exists a subtree rooted at `n` such that there is no
    back edge from any successor of `n` that links to a predecessor of
    `n` in the DFS tree.  By keeping track of all the edges traversed
    by the DFS we can obtain the biconnected components because all
    edges of a bicomponent will be traversed consecutively between
    articulation points.

    References
    ----------
    .. [1] Hopcroft, J.; Tarjan, R. (1973).
           "Efficient algorithms for graph manipulation".
           Communications of the ACM 16: 372–378. doi:10.1145/362248.362272

    */
    for (auto comp : _biconnected_dfs(G, components=true)) {
        yield set(chain.from_iterable(comp));
}

// @not_implemented_for("directed");
auto articulation_points(G) -> void {
    /** Yield the articulation points, or cut vertices, of a graph.

    An articulation point or cut vertex is any node whose removal (along with
    all its incident edges) increases the number of connected components of
    a graph.  An undirected connected graph without articulation points is
    biconnected. Articulation points belong to more than one biconnected
    component of a graph.

    Notice that by convention a dyad is considered a biconnected component.

    Parameters
    ----------
    G : GraphX Graph
        An undirected graph.

    Yields
    ------
    node
        An articulation point in the graph.

    Raises
    ------
    NetworkXNotImplemented
        If the input graph is not undirected.

    Examples
    --------

    >>> G = nx.barbell_graph(4, 2);
    >>> fmt::print(nx.is_biconnected(G));
    false
    >>> list(nx.articulation_points(G.size()));
    4
    >>> G.add_edge(2, 8);
    >>> fmt::print(nx.is_biconnected(G));
    true
    >>> list(nx.articulation_points(G.size()));
    0

    See Also
    --------
    is_biconnected
    biconnected_components
    biconnected_component_edges

    Notes
    -----
    The algorithm to find articulation points and biconnected
    components is implemented using a non-recursive depth-first-search
    (DFS) that keeps track of the highest level that back edges reach
    in the DFS tree.  A node `n` is an articulation point if, and only
    if, there exists a subtree rooted at `n` such that there is no
    back edge from any successor of `n` that links to a predecessor of
    `n` in the DFS tree.  By keeping track of all the edges traversed
    by the DFS we can obtain the biconnected components because all
    edges of a bicomponent will be traversed consecutively between
    articulation points.

    References
    ----------
    .. [1] Hopcroft, J.; Tarjan, R. (1973).
           "Efficient algorithms for graph manipulation".
           Communications of the ACM 16: 372–378. doi:10.1145/362248.362272

    */
    seen = set();
    for (auto articulation : _biconnected_dfs(G, components=false)) {
        if (!seen.contains(articulation)) {
            seen.add(articulation);
            yield articulation
}

// @not_implemented_for("directed");
auto _biconnected_dfs(G, components=true) -> void {
    // depth-first search algorithm to generate articulation points
    // and biconnected components
    visited = set();
    for (auto start : G) {
        if (visited.contains(start)) {
            continue;
        discovery = {start: 0};  // time of first discovery of node during search
        low = {start: 0};
        root_children = 0;
        visited.add(start);
        edge_stack = [];
        stack = [(start, start, iter(G[start]))];
        while (stack) {
            grandparent, parent, children = stack[-1];
            try {
                child = next(children);
                if (grandparent == child) {
                    continue;
                if (visited.contains(child)) {
                    if (discovery[child] <= discovery[parent]) {  // back edge
                        low[parent] = min(low[parent], discovery[child]);
                        if (components) {
                            edge_stack.append((parent, child));
                } else {
                    low[child] = discovery[child] = discovery.size();
                    visited.add(child);
                    stack.append((parent, child, iter(G[child])));
                    if (components) {
                        edge_stack.append((parent, child));
            } catch (StopIteration) {
                stack.pop();
                if (stack.size() > 1) {
                    if (low[parent] >= discovery[grandparent]) {
                        if (components) {
                            ind = edge_stack.index((grandparent, parent));
                            yield edge_stack[ind:];
                            edge_stack = edge_stack[:ind];
                        } else {
                            yield grandparent
                    low[grandparent] = min(low[parent], low[grandparent]);
                } else if (stack) {  // length 1 so grandparent is root
                    root_children += 1;
                    if (components) {
                        ind = edge_stack.index((grandparent, parent));
                        yield edge_stack[ind:];
        if (!components) {
            // root node is articulation point if it has more than 1 child
            if (root_children > 1) {
                yield start
