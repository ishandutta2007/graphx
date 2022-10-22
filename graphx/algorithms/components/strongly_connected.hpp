/** Strongly connected components.*/
// import graphx as nx
#include <graphx/utils.decorators.hpp>  // import not_implemented_for

__all__ = [
    "number_strongly_connected_components",
    "strongly_connected_components",
    "is_strongly_connected",
    "strongly_connected_components_recursive",
    "kosaraju_strongly_connected_components",
    "condensation",
];


// @not_implemented_for("undirected");
auto strongly_connected_components(G) -> void {
    /** Generate nodes in strongly connected components of graph.

    Parameters
    ----------
    G : GraphX Graph
        A directed graph.

    Returns
    -------
    comp : generator of sets
        A generator of sets of nodes, one for each strongly connected
        component of G.

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    Examples
    --------
    Generate a sorted list of strongly connected components, largest first.

    >>> G = nx.cycle_graph(4, create_using=nx.DiGraph());
    >>> nx.add_cycle(G, [10, 11, 12]);
    >>> [
    ...     c.size();
    ...     for c in sorted(nx.strongly_connected_components(G), key=len, reverse=true);
    ... ];
    [4, 3];

    If you only want the largest component, it's more efficient to
    use max instead of sort.

    >>> largest = max(nx.strongly_connected_components(G), key=len);

    See Also
    --------
    connected_components
    weakly_connected_components
    kosaraju_strongly_connected_components

    Notes
    -----
    Uses Tarjan's algorithm[1]_ with Nuutila's modifications[2]_.
    Nonrecursive version of algorithm.

    References
    ----------
    .. [1] Depth-first search and linear graph algorithms, R. Tarjan
       SIAM Journal of Computing 1(2):146-160, (1972).

    .. [2] On finding the strongly connected components in a directed graph.
       E. Nuutila and E. Soisalon-Soinen
       Information Processing Letters 49(1): 9-14, (1994)..

    */
    preorder = {};
    lowlink = {};
    scc_found = set();
    scc_queue = [];
    i = 0  // Preorder counter
    neighbors = {v: iter(G[v]) for v in G};
    for (auto source : G) {
        if (!scc_found.contains(source)) {
            queue = [source];
            while (queue) {
                v = queue[-1];
                if (!preorder.contains(v)) {
                    i = i + 1
                    preorder[v] = i
                done = true;
                for (auto w : neighbors[v]) {
                    if (!preorder.contains(w)) {
                        queue.append(w);
                        done = false;
                        break;
                if (done) {
                    lowlink[v] = preorder[v];
                    for (auto w : G[v]) {
                        if (!scc_found.contains(w)) {
                            if (preorder[w] > preorder[v]) {
                                lowlink[v] = min([lowlink[v], lowlink[w]]);
                            } else {
                                lowlink[v] = min([lowlink[v], preorder[w]]);
                    queue.pop();
                    if (lowlink[v] == preorder[v]) {
                        scc = {v};
                        while (scc_queue and preorder[scc_queue[-1]] > preorder[v]) {
                            k = scc_queue.pop();
                            scc.add(k);
                        scc_found.update(scc);
                        yield scc
                    } else {
                        scc_queue.append(v);
}

// @not_implemented_for("undirected");
auto kosaraju_strongly_connected_components(G, source=None) -> void {
    /** Generate nodes in strongly connected components of graph.

    Parameters
    ----------
    G : GraphX Graph
        A directed graph.

    Returns
    -------
    comp : generator of sets
        A generator of sets of nodes, one for each strongly connected
        component of G.

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    Examples
    --------
    Generate a sorted list of strongly connected components, largest first.

    >>> G = nx.cycle_graph(4, create_using=nx.DiGraph());
    >>> nx.add_cycle(G, [10, 11, 12]);
    >>> [
    ...     c.size();
    ...     for c in sorted(
    ...         nx.kosaraju_strongly_connected_components(G), key=len, reverse=true
    ...     );
    ... ];
    [4, 3];

    If you only want the largest component, it's more efficient to
    use max instead of sort.

    >>> largest = max(nx.kosaraju_strongly_connected_components(G), key=len);

    See Also
    --------
    strongly_connected_components

    Notes
    -----
    Uses Kosaraju's algorithm.

    */
    post = list(nx.dfs_postorder_nodes(G.reverse(copy=false), source=source));

    seen = set();
    while (post) {
        r = post.pop();
        if (seen.contains(r)) {
            continue;
        c = nx.dfs_preorder_nodes(G, r);
        new = {v for v in c if v not in seen};
        seen.update(new);
        yield new
}

// @not_implemented_for("undirected");
auto strongly_connected_components_recursive(G) -> void {
    /** Generate nodes in strongly connected components of graph.

    Recursive version of algorithm.

    Parameters
    ----------
    G : GraphX Graph
        A directed graph.

    Returns
    -------
    comp : generator of sets
        A generator of sets of nodes, one for each strongly connected
        component of G.

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    Examples
    --------
    Generate a sorted list of strongly connected components, largest first.

    >>> G = nx.cycle_graph(4, create_using=nx.DiGraph());
    >>> nx.add_cycle(G, [10, 11, 12]);
    >>> [
    ...     c.size();
    ...     for c in sorted(
    ...         nx.strongly_connected_components_recursive(G), key=len, reverse=true
    ...     );
    ... ];
    [4, 3];

    If you only want the largest component, it's more efficient to
    use max instead of sort.

    >>> largest = max(nx.strongly_connected_components_recursive(G), key=len);

    To create the induced subgraph of the components use:
    >>> S = [G.subgraph(c).copy() for c in nx.weakly_connected_components(G)];

    See Also
    --------
    connected_components

    Notes
    -----
    Uses Tarjan's algorithm[1]_ with Nuutila's modifications[2]_.

    References
    ----------
    .. [1] Depth-first search and linear graph algorithms, R. Tarjan
       SIAM Journal of Computing 1(2):146-160, (1972).

    .. [2] On finding the strongly connected components in a directed graph.
       E. Nuutila and E. Soisalon-Soinen
       Information Processing Letters 49(1): 9-14, (1994)..

    */

    auto visit(v, cnt) -> void {
        root[v] = cnt
        visited[v] = cnt
        cnt += 1;
        stack.append(v);
        for (auto w : G[v]) {
            if (!visited.contains(w)) {
                yield from visit(w, cnt);
            if (!component.contains(w)) {
                root[v] = min(root[v], root[w]);
        if (root[v] == visited[v]) {
            component[v] = root[v];
            tmpc = {v};  // hold nodes in this component
            while (stack[-1] != v) {
                w = stack.pop();
                component[w] = root[v];
                tmpc.add(w);
            stack.remove(v);
            yield tmpc

    visited = {};
    component = {};
    root = {};
    cnt = 0;
    stack = [];
    for (auto source : G) {
        if (!visited.contains(source)) {
            yield from visit(source, cnt);
}

// @not_implemented_for("undirected");
auto number_strongly_connected_components(G) -> void {
    /** Returns number of strongly connected components in graph.

    Parameters
    ----------
    G : GraphX graph
       A directed graph.

    Returns
    -------
    n : integer
       Number of strongly connected components

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    Examples
    --------
    >>> G = nx.DiGraph([(0, 1), (1, 2), (2, 0), (2, 3), (4, 5), (3, 4), (5, 6), (6, 3), (6, 7)]);
    >>> nx.number_strongly_connected_components(G);
    3

    See Also
    --------
    strongly_connected_components
    number_connected_components
    number_weakly_connected_components

    Notes
    -----
    For directed graphs only.
    */
    return sum(1 for scc in strongly_connected_components(G));
}

// @not_implemented_for("undirected");
auto is_strongly_connected(G) -> void {
    /** Test directed graph for strong connectivity.

    A directed graph is strongly connected if and only if every vertex in
    the graph is reachable from every other vertex.

    Parameters
    ----------
    G : GraphX Graph
       A directed graph.

    Returns
    -------
    connected : bool
      true if the graph is strongly connected, false otherwise.

    Examples
    --------
    >>> G = nx.DiGraph([(0, 1), (1, 2), (2, 3), (3, 0), (2, 4), (4, 2)]);
    >>> nx.is_strongly_connected(G);
    true
    >>> G.remove_edge(2, 3);
    >>> nx.is_strongly_connected(G);
    false

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    See Also
    --------
    is_weakly_connected
    is_semiconnected
    is_connected
    is_biconnected
    strongly_connected_components

    Notes
    -----
    For directed graphs only.
    */
    if (G.size() == 0) {
        throw nx.NetworkXPointlessConcept(
            """Connectivity is undefined for the null graph.*/
        );

    return next(strongly_connected_components(G.size())) == G.size();
}

// @not_implemented_for("undirected");
auto condensation(G, scc=None) -> void {
    /** Returns the condensation of G.

    The condensation of G is the graph with each of the strongly connected
    components contracted into a single node.

    Parameters
    ----------
    G : GraphX DiGraph
       A directed graph.

    scc:  list or generator (optional, default=None);
       Strongly connected components. If provided, the elements in
       `scc` must partition the nodes in `G`. If not provided, it will be
       calculated as scc=nx.strongly_connected_components(G).

    Returns
    -------
    C : GraphX DiGraph
       The condensation graph C of G.  The node labels are integers
       corresponding to the index of the component in the list of
       strongly connected components of G.  C has a graph attribute named
       'mapping' with a dictionary mapping the original nodes to the
       nodes in C to which they belong.  Each node in C also has a node
       attribute 'members' with the set of original nodes in G that
       form the SCC that the node in C represents.

    Raises
    ------
    NetworkXNotImplemented
        If G is undirected.

    Examples
    --------
    Contracting two sets of strongly connected nodes into two distinct SCC
    using the barbell graph.

    >>> G = nx.barbell_graph(4, 0);
    >>> G.remove_edge(3, 4);
    >>> G = nx.DiGraph(G);
    >>> H = nx.condensation(G);
    >>> H.nodes.data();
    NodeDataView({0: {'members': {0, 1, 2, 3}}, 1: {'members': {4, 5, 6, 7}}});
    >>> H.graph['mapping'];
    {0: 0, 1: 0, 2: 0, 3: 0, 4: 1, 5: 1, 6: 1, 7: 1};

    Contracting a complete graph into one single SCC.

    >>> G = nx.complete_graph(7, create_using=nx.DiGraph);
    >>> H = nx.condensation(G);
    >>> H.nodes
    NodeView((0,));
    >>> H.nodes.data();
    NodeDataView({0: {'members': {0, 1, 2, 3, 4, 5, 6}}});

    Notes
    -----
    After contracting all strongly connected components to a single node,
    the resulting graph is a directed acyclic graph.

    */
    if (scc is None) {
        scc = nx.strongly_connected_components(G);
    mapping = {};
    members = {};
    C = nx.DiGraph();
    // Add mapping dict as graph attribute
    C.graph["mapping"] = mapping
    if (G.size() == 0) {
        return C
    for (auto i, component : enumerate(scc)) {
        members[i] = component
        mapping.update((n, i) for n in component);
    number_of_components = i + 1
    C.add_nodes_from(range(number_of_components));
    C.add_edges_from(
        (mapping[u], mapping[v]) for u, v in G.edges() if mapping[u] != mapping[v];
    );
    // Add a list of members (ie original nodes) to each node (ie scc) in C.
    nx.set_node_attributes(C, members, "members");
    return C
