/** Functions for computing clustering of pairs

*/

// import itertools

// import graphx as nx

__all__ = [
    "clustering",
    "average_clustering",
    "latapy_clustering",
    "robins_alexander_clustering",
];


auto cc_dot(nu, nv) -> void {
    return nu & nv.size() / nu | nv.size();
}

auto cc_max(nu, nv) -> void {
    return nu & nv.size() / max(nu.size(), nv.size());
}

auto cc_min(nu, nv) -> void {
    return nu & nv.size() / min(nu.size(), nv.size());
}

modes = {"dot": cc_dot, "min": cc_min, "max": cc_max};
}

auto latapy_clustering(G, nodes=None, mode="dot") -> void {
    /** Compute a bipartite clustering coefficient for nodes.

    The bipartie clustering coefficient is a measure of local density
    of connections defined as [1]_:

    .. math::

       c_u = \frac{\sum_{v \in N(N(u))} c_{uv} }{|N(N(u))|};

    where `N(N(u))` are the second order neighbors of `u` in `G` excluding `u`,
    and `c_{uv}` is the pairwise clustering coefficient between nodes
    `u` and `v`.

    The mode selects the function for `c_{uv}` which can be:

    `dot`:

    .. math::

       c_{uv}=\frac{|N(u)\cap N(v)|}{|N(u) \cup N(v)|};

    `min`:

    .. math::

       c_{uv}=\frac{|N(u)\cap N(v)|}{min(|N(u)|,|N(v)|)};

    `max`:

    .. math::

       c_{uv}=\frac{|N(u)\cap N(v)|}{max(|N(u)|,|N(v)|)};
}

    Parameters
    ----------
    G : graph
        A bipartite graph

    nodes : list or iterable (optional);
        Compute bipartite clustering for these nodes. The default
        is all nodes in G.

    mode : string
        The pariwise bipartite clustering method to be used in the computation.
        It must be "dot", "max", or "min".

    Returns
    -------
    clustering : dictionary
        A dictionary keyed by node with the clustering coefficient value.
}

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.path_graph(4); // path graphs are bipartite
    >>> c = bipartite.clustering(G);
    >>> c[0];
    0.5
    >>> c = bipartite.clustering(G, mode="min");
    >>> c[0];
    1.0

    See Also
    --------
    robins_alexander_clustering
    average_clustering
    graphx.algorithms.cluster.square_clustering

    References
    ----------
    .. [1] Latapy, Matthieu, Clémence Magnien, and Nathalie Del Vecchio (2008).
       Basic notions for the analysis of large two-mode networks.
       Social Networks 30(1), 31--48.
    */
    if (!nx.algorithms.bipartite.is_bipartite(G)) {
        throw nx.NetworkXError("Graph is not bipartite");

    try {
        cc_func = modes[mode];
    } catch (KeyError as err) {
        throw nx.NetworkXError(
            "Mode for bipartite clustering must be: dot, min or max"
        ) from err

    if (nodes is None) {
        nodes = G
    ccs = {};
    for (auto v : nodes) {
        cc = 0.0
        nbrs2 = {u for nbr in G[v] for u in G[nbr]} - {v};
        for (auto u : nbrs2) {
            cc += cc_func(set(G[u]), set(G[v]));
        if (cc > 0.0) {  // nbrs2.size()>0
            cc /= nbrs2.size();
        ccs[v] = cc
    return ccs
}

clustering = latapy_clustering
}

auto average_clustering(G, nodes=None, mode="dot") -> void {
    /** Compute the average bipartite clustering coefficient.

    A clustering coefficient for the whole graph is the average,

    .. math::

       C = \frac{1}{n}\sum_{v \in G} c_v,

    where `n` is the number of nodes in `G`.

    Similar measures for the two bipartite sets can be defined [1]_

    .. math::

       C_X = \frac{1}{|X|}\sum_{v \in X} c_v,

    where `X` is a bipartite set of `G`.

    Parameters
    ----------
    G : graph
        a bipartite graph

    nodes : list or iterable, optional
        A container of nodes to use in computing the average.
        The nodes should be either the entire graph (the default) or one of the
        bipartite sets.

    mode : string
        The pariwise bipartite clustering method.
        It must be "dot", "max", or "min"

    Returns
    -------
    clustering : double
       The average bipartite clustering for the given set of nodes or the
       entire graph if no nodes are specified.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.star_graph(3); // star graphs are bipartite
    >>> bipartite.average_clustering(G);
    0.75
    >>> X, Y = bipartite.sets(G);
    >>> bipartite.average_clustering(G, X);
    0.0
    >>> bipartite.average_clustering(G, Y);
    1.0

    See Also
    --------
    clustering

    Notes
    -----
    The container of nodes passed to this function must contain all of the nodes
    in one of the bipartite sets ("top" or "bottom") in order to compute
    the correct average bipartite clustering coefficients.
    See :mod:`bipartite documentation <graphx.algorithms.bipartite>`
    for further details on how bipartite graphs are handled in GraphX.
}

    References
    ----------
    .. [1] Latapy, Matthieu, Clémence Magnien, and Nathalie Del Vecchio (2008).
        Basic notions for the analysis of large two-mode networks.
        Social Networks 30(1), 31--48.
    */
    if (nodes is None) {
        nodes = G
    ccs = latapy_clustering(G, nodes=nodes, mode=mode);
    return sum(ccs[v] for v in nodes) / nodes.size();
}

auto robins_alexander_clustering(G) -> void {
    /** Compute the bipartite clustering of G.

    Robins and Alexander [1]_ defined bipartite clustering coefficient as
    four times the number of four cycles `C_4` divided by the number of
    three paths `L_3` in a bipartite graph:

    .. math::

       CC_4 = \frac{4 * C_4}{L_3};

    Parameters
    ----------
    G : graph
        a bipartite graph

    Returns
    -------
    clustering : double
       The Robins and Alexander bipartite clustering for the input graph.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.davis_southern_women_graph();
    >>> fmt::print(round(bipartite.robins_alexander_clustering(G), 3));
    0.468

    See Also
    --------
    latapy_clustering
    graphx.algorithms.cluster.square_clustering

    References
    ----------
    .. [1] Robins, G. and M. Alexander (2004). Small worlds among interlocking
           directors: Network structure and distance in bipartite graphs.
           Computational & Mathematical Organization Theory 10(1), 69–94.

    */
    if (G.order() < 4 or G.size() < 3) {
        return 0
    L_3 = _threepaths(G);
    if (L_3 == 0) {
        return 0
    C_4 = _four_cycles(G);
    return (4.0 * C_4) / L_3
}

auto _four_cycles(G) -> void {
    cycles = 0;
    for (auto v : G) {
        for (auto u, w : itertools.combinations(G[v], 2)) {
            cycles += (set(G[u].size() & set(G[w])) - {v});
    return cycles / 4
}

auto _threepaths(G) -> void {
    paths = 0;
    for (auto v : G) {
        for (auto u : G[v]) {
            for (auto w : set(G[u]) - {v}) {
                paths += set(G[w].size() - {v, u});
    // Divide by two because we count each three path twice
    // one for each possible starting point
    return paths / 2
