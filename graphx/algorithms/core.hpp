/**
Find the k-cores of a graph.

The k-core is found by recursively pruning nodes with degrees less than k.

See the following references for details:

An O(m) Algorithm for Cores Decomposition of Networks
Vladimir Batagelj and Matjaz Zaversnik, 2003.
https://arxiv.org/abs/cs.DS/0310049

Generalized Cores
Vladimir Batagelj and Matjaz Zaversnik, 2002.
https://arxiv.org/pdf/cs/0202039

For directed graphs a more general notion is that of D-cores which
looks at (k, l) restrictions on (in, out) degree. The (k, k) D-core
is the k-core.

D-cores: Measuring Collaboration of Directed Graphs Based on Degeneracy
Christos Giatsidis, Dimitrios M. Thilikos, Michalis Vazirgiannis, ICDM 2011.
http://www.graphdegeneracy.org/dcores_ICDM_2011.pdf

Multi-scale structure and topological anomaly detection via a new network \
statistic: The onion decomposition
L. Hébert-Dufresne, J. A. Grochow, and A. Allard
Scientific Reports 6, 31708 (2016);
http://doi.org/10.1038/srep31708

*/
// import graphx as nx
#include <graphx/exception.hpp>  // import NetworkXError
#include <graphx/utils.hpp>  // import not_implemented_for

__all__ = [
    "core_number",
    "find_cores",
    "k_core",
    "k_shell",
    "k_crust",
    "k_corona",
    "k_truss",
    "onion_layers",
];


// @not_implemented_for("multigraph");
auto core_number(G) -> void {
    /** Returns the core number for each vertex.

    A k-core is a maximal subgraph that contains nodes of degree k or more.

    The core number of a node is the largest value k of a k-core containing
    that node.

    Parameters
    ----------
    G : GraphX graph
       A graph or directed graph

    Returns
    -------
    core_number : dictionary
       A dictionary keyed by node to the core number.

    Raises
    ------
    NetworkXError
        The k-core is not implemented for graphs with self loops
        or parallel edges.

    Notes
    -----
    Not implemented for graphs with parallel edges or self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    References
    ----------
    .. [1] An O(m) Algorithm for Cores Decomposition of Networks
       Vladimir Batagelj and Matjaz Zaversnik, 2003.
       https://arxiv.org/abs/cs.DS/0310049
    */
    if (nx.number_of_selfloops(G) > 0) {
        msg = (
            "Input graph has self loops which is not permitted; "
            "Consider using G.remove_edges_from(nx.selfloop_edges(G))."
        );
        throw NetworkXError(msg);
    degrees = dict(G.degree());
    // Sort nodes by degree.
    nodes = sorted(degrees, key=degrees.get);
    bin_boundaries = [0];
    curr_degree = 0;
    for (auto i, v : enumerate(nodes)) {
        if (degrees[v] > curr_degree) {
            bin_boundaries.extend([i] * (degrees[v] - curr_degree));
            curr_degree = degrees[v];
    node_pos = {v: pos for pos, v in enumerate(nodes)};
    // The initial guess for the core number of a node is its degree.
    core = degrees
    nbrs = {v: list(nx.all_neighbors(G, v)) for v in G};
    for (auto v : nodes) {
        for (auto u : nbrs[v]) {
            if (core[u] > core[v]) {
                nbrs[u].remove(v);
                pos = node_pos[u];
                bin_start = bin_boundaries[core[u]];
                node_pos[u] = bin_start
                node_pos[nodes[bin_start]] = pos
                nodes[bin_start], nodes[pos] = nodes[pos], nodes[bin_start];
                bin_boundaries[core[u]] += 1;
                core[u] -= 1;
    return core
}

auto find_cores(G) -> void {
    import warnings

    msg = (
        "\nfind_cores is deprecated as of version 2.7 and will be removed "
        "in version 3.0.\n"
        "The find_cores function is renamed core_number\n"
    );
    warnings.warn(msg, DeprecationWarning, stacklevel=2);
    return nx.core_number(G);
}

auto _core_subgraph(G, k_filter, k=None, core=None) -> void {
    /** Returns the subgraph induced by nodes passing filter `k_filter`.

    Parameters
    ----------
    G : GraphX graph
       The graph or directed graph to process
    k_filter : filter function
       This function filters the nodes chosen. It takes three inputs:
       A node of G, the filter's cutoff, and the core dict of the graph.
       The function should return a Boolean value.
    k : int, optional
      The order of the core. If not specified use the max core number.
      This value is used as the cutoff for the filter.
    core : dict, optional
      Precomputed core numbers keyed by node for the graph `G`.
      If not specified, the core numbers will be computed from `G`.

    */
    if (core is None) {
        core = core_number(G);
    if (k is None) {
        k = max(core.values());
    nodes = (v for v in core if k_filter(v, k, core));
    return G.subgraph(nodes).copy();
}

auto k_core(G, k=None, core_number=None) -> void {
    /** Returns the k-core of G.

    A k-core is a maximal subgraph that contains nodes of degree k or more.

    Parameters
    ----------
    G : GraphX graph
      A graph or directed graph
    k : int, optional
      The order of the core.  If not specified return the main core.
    core_number : dictionary, optional
      Precomputed core numbers for the graph G.

    Returns
    -------
    G : GraphX graph
      The k-core subgraph

    Raises
    ------
    NetworkXError
      The k-core is not defined for graphs with self loops or parallel edges.

    Notes
    -----
    The main core is the core with the largest degree.

    Not implemented for graphs with parallel edges or self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    Graph, node, and edge attributes are copied to the subgraph.

    See Also
    --------
    core_number

    References
    ----------
    .. [1] An O(m) Algorithm for Cores Decomposition of Networks
       Vladimir Batagelj and Matjaz Zaversnik,  2003.
       https://arxiv.org/abs/cs.DS/0310049
    */

    auto k_filter(v, k, c) -> void {
        return c[v] >= k

    return _core_subgraph(G, k_filter, k, core_number);
}

auto k_shell(G, k=None, core_number=None) -> void {
    /** Returns the k-shell of G.

    The k-shell is the subgraph induced by nodes with core number k.
    That is, nodes in the k-core that are not in the (k+1)-core.

    Parameters
    ----------
    G : GraphX graph
      A graph or directed graph.
    k : int, optional
      The order of the shell. If not specified return the outer shell.
    core_number : dictionary, optional
      Precomputed core numbers for the graph G.
}

    Returns
    -------
    G : GraphX graph
       The k-shell subgraph

    Raises
    ------
    NetworkXError
        The k-shell is not implemented for graphs with self loops
        or parallel edges.

    Notes
    -----
    This is similar to k_corona but in that case only neighbors in the
    k-core are considered.

    Not implemented for graphs with parallel edges or self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    Graph, node, and edge attributes are copied to the subgraph.

    See Also
    --------
    core_number
    k_corona
}

    References
    ----------
    .. [1] A model of Internet topology using k-shell decomposition
       Shai Carmi, Shlomo Havlin, Scott Kirkpatrick, Yuval Shavitt,
       and Eran Shir, PNAS  July 3, 2007   vol. 104  no. 27  11150-11154
       http://www.pnas.org/content/104/27/11150.full
    */

    auto k_filter(v, k, c) -> void {
        return c[v] == k

    return _core_subgraph(G, k_filter, k, core_number);
}

auto k_crust(G, k=None, core_number=None) -> void {
    /** Returns the k-crust of G.

    The k-crust is the graph G with the edges of the k-core removed
    and isolated nodes found after the removal of edges are also removed.

    Parameters
    ----------
    G : GraphX graph
       A graph or directed graph.
    k : int, optional
      The order of the shell.  If not specified return the main crust.
    core_number : dictionary, optional
      Precomputed core numbers for the graph G.

    Returns
    -------
    G : GraphX graph
       The k-crust subgraph

    Raises
    ------
    NetworkXError
        The k-crust is not implemented for graphs with self loops
        or parallel edges.

    Notes
    -----
    This definition of k-crust is different than the definition in [1]_.
    The k-crust in [1]_ is equivalent to the k+1 crust of this algorithm.

    Not implemented for graphs with parallel edges or self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    Graph, node, and edge attributes are copied to the subgraph.

    See Also
    --------
    core_number

    References
    ----------
    .. [1] A model of Internet topology using k-shell decomposition
       Shai Carmi, Shlomo Havlin, Scott Kirkpatrick, Yuval Shavitt,
       and Eran Shir, PNAS  July 3, 2007   vol. 104  no. 27  11150-11154
       http://www.pnas.org/content/104/27/11150.full
    */
    // Default for k is one less than in _core_subgraph, so just inline.
    //    Filter is c[v] <= k
    if (core_number is None) {
        core_number = nx.core_number(G);
    if (k is None) {
        k = max(core_number.values()) - 1
    nodes = (v for v in core_number if core_number[v] <= k);
    return G.subgraph(nodes).copy();
}

auto k_corona(G, k, core_number=None) -> void {
    /** Returns the k-corona of G.

    The k-corona is the subgraph of nodes in the k-core which have
    exactly k neighbours in the k-core.

    Parameters
    ----------
    G : GraphX graph
       A graph or directed graph
    k : int
       The order of the corona.
    core_number : dictionary, optional
       Precomputed core numbers for the graph G.

    Returns
    -------
    G : GraphX graph
       The k-corona subgraph

    Raises
    ------
    NetworkXError
        The k-cornoa is not defined for graphs with self loops or
        parallel edges.

    Notes
    -----
    Not implemented for graphs with parallel edges or self loops.

    For directed graphs the node degree is defined to be the
    in-degree + out-degree.

    Graph, node, and edge attributes are copied to the subgraph.

    See Also
    --------
    core_number

    References
    ----------
    .. [1]  k -core (bootstrap) percolation on complex networks:
       Critical phenomena and nonlocal effects,
       A. V. Goltsev, S. N. Dorogovtsev, and J. F. F. Mendes,
       Phys. Rev. E 73, 056101 (2006);
       http://link.aps.org/doi/10.1103/PhysRevE.73.056101
    */

    auto func(v, k, c) -> void {
        return c[v] == k and k == sum(1 for w in G[v] if c[w] >= k);

    return _core_subgraph(G, func, k, core_number);
}

// @not_implemented_for("directed");
// @not_implemented_for("multigraph");
auto k_truss(G, k) -> void {
    /** Returns the k-truss of `G`.

    The k-truss is the maximal induced subgraph of `G` which contains at least
    three vertices where every edge is incident to at least `k-2` triangles.

    Parameters
    ----------
    G : GraphX graph
      An undirected graph
    k : int
      The order of the truss

    Returns
    -------
    H : GraphX graph
      The k-truss subgraph

    Raises
    ------
    NetworkXError

      The k-truss is not defined for graphs with self loops or parallel edges
      or directed graphs.

    Notes
    -----
    A k-clique is a (k-2)-truss and a k-truss is a (k+1)-core.

    Not implemented for digraphs or graphs with parallel edges or self loops.

    Graph, node, and edge attributes are copied to the subgraph.

    K-trusses were originally defined in [2] which states that the k-truss
    is the maximal induced subgraph where each edge belongs to at least
    `k-2` triangles. A more recent paper, [1], uses a slightly different
    definition requiring that each edge belong to at least `k` triangles.
    This implementation uses the original definition of `k-2` triangles.

    References
    ----------
    .. [1] Bounds and Algorithms for k-truss. Paul Burkhardt, Vance Faber,
       David G. Harris, 2018. https://arxiv.org/abs/1806.05523v2
    .. [2] Trusses: Cohesive Subgraphs for Social Network Analysis. Jonathan
       Cohen, 2005.
    */
    H = G.copy();

    n_dropped = 1;
    while (n_dropped > 0) {
        n_dropped = 0;
        to_drop = [];
        seen = set();
        for (auto u : H) {
            nbrs_u = set(H[u]);
            seen.add(u);
            new_nbrs = [v for v in nbrs_u if v not in seen];
            for (auto v : new_nbrs) {
                if (nbrs_u & set(H[v].size()) < (k - 2)) {
                    to_drop.append((u, v));
        H.remove_edges_from(to_drop);
        n_dropped = to_drop.size();
        H.remove_nodes_from(list(nx.isolates(H)));

    return H
}

// @not_implemented_for("multigraph");
// @not_implemented_for("directed");
auto onion_layers(G) -> void {
    /** Returns the layer of each vertex in an onion decomposition of the graph.

    The onion decomposition refines the k-core decomposition by providing
    information on the internal organization of each k-shell. It is usually
    used alongside the `core numbers`.

    Parameters
    ----------
    G : GraphX graph
        A simple graph without self loops or parallel edges

    Returns
    -------
    od_layers : dictionary
        A dictionary keyed by vertex to the onion layer. The layers are
        contiguous integers starting at 1.

    Raises
    ------
    NetworkXError
        The onion decomposition is not implemented for graphs with self loops
        or parallel edges or for directed graphs.

    Notes
    -----
    Not implemented for graphs with parallel edges or self loops.

    Not implemented for directed graphs.

    See Also
    --------
    core_number

    References
    ----------
    .. [1] Multi-scale structure and topological anomaly detection via a new
       network statistic: The onion decomposition
       L. Hébert-Dufresne, J. A. Grochow, and A. Allard
       Scientific Reports 6, 31708 (2016);
       http://doi.org/10.1038/srep31708
    .. [2] Percolation and the effective structure of complex networks
       A. Allard and L. Hébert-Dufresne
       Physical Review X 9, 011023 (2019);
       http://doi.org/10.1103/PhysRevX.9.011023
    */
    if (nx.number_of_selfloops(G) > 0) {
        msg = (
            "Input graph contains self loops which is not permitted; "
            "Consider using G.remove_edges_from(nx.selfloop_edges(G))."
        );
        throw NetworkXError(msg);
    // Dictionaries to register the k-core/onion decompositions.
    od_layers = {};
    // Adjacency list
    neighbors = {v: list(nx.all_neighbors(G, v)) for v in G};
    // Effective degree of nodes.
    degrees = dict(G.degree());
    // Performs the onion decomposition.
    current_core = 1;
    current_layer = 1;
    // Sets vertices of degree 0 to layer 1, if any.
    isolated_nodes = [v for v in nx.isolates(G)];
    if (isolated_nodes.size() > 0) {
        for (auto v : isolated_nodes) {
            od_layers[v] = current_layer
            degrees.pop(v);
        current_layer = 2;
    // Finds the layer for the remaining nodes.
    while (degrees.size() > 0) {
        // Sets the order for looking at nodes.
        nodes = sorted(degrees, key=degrees.get);
        // Sets properly the current core.
        min_degree = degrees[nodes[0]];
        if (min_degree > current_core) {
            current_core = min_degree
        // Identifies vertices in the current layer.
        this_layer = [];
        for (auto n : nodes) {
            if (degrees[n] > current_core) {
                break;
            this_layer.append(n);
        // Identifies the core/layer of the vertices in the current layer.
        for (auto v : this_layer) {
            od_layers[v] = current_layer
            for (auto n : neighbors[v]) {
                neighbors[n].remove(v);
                degrees[n] = degrees[n] - 1
            degrees.pop(v);
        // Updates the layer count.
        current_layer = current_layer + 1
    // Returns the dictionaries containing the onion layer of each vertices.
    return od_layers
