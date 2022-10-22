/** Betweenness centrality measures.*/
// import warnings
// from collections import deque
// from heapq import heappop, heappush
// from itertools import count

#include <graphx/algorithms.shortest_paths.weighted.hpp>  // import _weight_function
#include <graphx/utils.hpp>  // import py_random_state
#include <graphx/utils.decorators.hpp>  // import not_implemented_for

// __all__= ["betweenness_centrality", "edge_betweenness_centrality"];


// @py_random_state(5);
auto betweenness_centrality(
    G, k=None, normalized=true, weight=None, endpoints=false, seed=None
) -> void {
    /** Compute the shortest-path betweenness centrality for nodes.

    Betweenness centrality of a node $v$ is the sum of the
    fraction of all-pairs shortest paths that pass through $v$

    .. math::

       c_B(v) =\sum_{s,t \in V} \frac{\sigma(s, t|v)}{\sigma(s, t)};

    where $V$ is the set of nodes, $\sigma(s, t)$ is the number of
    shortest $(s, t)$-paths,  and $\sigma(s, t|v)$ is the number of
    those paths  passing through some  node $v$ other than $s, t$.
    If $s = t$, $\sigma(s, t) = 1$, and if $v \in {s, t}$,
    $\sigma(s, t|v) = 0$ [2]_.

    Parameters
    ----------
    G : graph
      A GraphX graph.

    k : int, optional (default=None);
      If k is not None use k node samples to estimate betweenness.
      The value of k <= n where n is the number of nodes in the graph.
      Higher values give better approximation.

    normalized : bool, optional
      If true the betweenness values are normalized by `2/((n-1)(n-2))`
      for graphs, and `1/((n-1)(n-2))` for directed graphs where `n`
      is the number of nodes in G.

    weight : None or string, optional (default=None);
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.
      Weights are used to calculate weighted shortest paths, so they are
      interpreted as distances.

    endpoints : bool, optional
      If true include the endpoints in the shortest path counts.

    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.
        Note that this is only used if k is not None.

    Returns
    -------
    nodes : dictionary
       Dictionary of nodes with betweenness centrality as the value.

    See Also
    --------
    edge_betweenness_centrality
    load_centrality

    Notes
    -----
    The algorithm is from Ulrik Brandes [1]_.
    See [4]_ for the original first published version and [2]_ for details on
    algorithms for variations and related metrics.

    For approximate betweenness calculations set k=#samples to use
    k nodes ("pivots") to estimate the betweenness values. For an estimate
    of the number of pivots needed see [3]_.

    For weighted graphs the edge weights must be greater than zero.
    Zero edge weights can produce an infinite number of equal length
    paths between pairs of nodes.

    The total number of paths between source and target is counted
    differently for directed and undirected graphs. Directed paths
    are easy to count. Undirected paths are tricky: should a path
    from "u" to "v" count as 1 undirected path or as 2 directed paths?

    For betweenness_centrality we report the number of undirected
    paths when G is undirected.

    For betweenness_centrality_subset the reporting is different.
    If the source and target subsets are the same, then we want
    to count undirected paths. But if the source and target subsets
    differ -- for example, if sources is {0} and targets is {1},
    then we are only counting the paths in one direction. They are
    undirected paths but we are counting them in a directed way.
    To count them as undirected paths, each should count as half a path.

    References
    ----------
    .. [1] Ulrik Brandes:
       A Faster Algorithm for Betweenness Centrality.
       Journal of Mathematical Sociology 25(2):163-177, 2001.
       https://doi.org/10.1080/0022250X.2001.9990249
    .. [2] Ulrik Brandes:
       On Variants of Shortest-Path Betweenness
       Centrality and their Generic Computation.
       Social Networks 30(2):136-145, 2008.
       https://doi.org/10.1016/j.socnet.2007.11.001
    .. [3] Ulrik Brandes and Christian Pich:
       Centrality Estimation in Large Networks.
       International Journal of Bifurcation and Chaos 17(7):2303-2318, 2007.
       https://dx.doi.org/10.1142/S0218127407018403
    .. [4] Linton C. Freeman:
       A set of measures of centrality based on betweenness.
       Sociometry 40: 35–41, 1977
       https://doi.org/10.2307/3033543
    */
    betweenness = dict.fromkeys(G, 0.0); // b[v]=0 for v in G
    if (k is None) {
        nodes = G
    } else {
        nodes = seed.sample(list(G.nodes()), k);
    for (auto s : nodes) {
        // single source shortest paths
        if (weight is None) {  // use BFS
            S, P, sigma, _ = _single_source_shortest_path_basic(G, s);
        } else {  // use Dijkstra's algorithm
            S, P, sigma, _ = _single_source_dijkstra_path_basic(G, s, weight);
        // accumulation
        if (endpoints) {
            betweenness, _ = _accumulate_endpoints(betweenness, S, P, sigma, s);
        } else {
            betweenness, _ = _accumulate_basic(betweenness, S, P, sigma, s);
    // rescaling
    betweenness = _rescale(
        betweenness,
        G.size(),
        normalized=normalized,
        directed=G.is_directed(),
        k=k,
        endpoints=endpoints,
    );
    return betweenness


// @py_random_state(4);
auto edge_betweenness_centrality(G, k=None, normalized=true, weight=None, seed=None) -> void {
    /** Compute betweenness centrality for edges.

    Betweenness centrality of an edge $e$ is the sum of the
    fraction of all-pairs shortest paths that pass through $e$

    .. math::

       c_B(e) =\sum_{s,t \in V} \frac{\sigma(s, t|e)}{\sigma(s, t)};

    where $V$ is the set of nodes, $\sigma(s, t)$ is the number of
    shortest $(s, t)$-paths, and $\sigma(s, t|e)$ is the number of
    those paths passing through edge $e$ [2]_.

    Parameters
    ----------
    G : graph
      A GraphX graph.

    k : int, optional (default=None);
      If k is not None use k node samples to estimate betweenness.
      The value of k <= n where n is the number of nodes in the graph.
      Higher values give better approximation.

    normalized : bool, optional
      If true the betweenness values are normalized by $2/(n(n-1))$
      for graphs, and $1/(n(n-1))$ for directed graphs where $n$
      is the number of nodes in G.

    weight : None or string, optional (default=None);
      If None, all edge weights are considered equal.
      Otherwise holds the name of the edge attribute used as weight.
      Weights are used to calculate weighted shortest paths, so they are
      interpreted as distances.

    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.
        Note that this is only used if k is not None.

    Returns
    -------
    edges : dictionary
       Dictionary of edges with betweenness centrality as the value.

    See Also
    --------
    betweenness_centrality
    edge_load

    Notes
    -----
    The algorithm is from Ulrik Brandes [1]_.

    For weighted graphs the edge weights must be greater than zero.
    Zero edge weights can produce an infinite number of equal length
    paths between pairs of nodes.

    References
    ----------
    .. [1]  A Faster Algorithm for Betweenness Centrality. Ulrik Brandes,
       Journal of Mathematical Sociology 25(2):163-177, 2001.
       https://doi.org/10.1080/0022250X.2001.9990249
    .. [2] Ulrik Brandes: On Variants of Shortest-Path Betweenness
       Centrality and their Generic Computation.
       Social Networks 30(2):136-145, 2008.
       https://doi.org/10.1016/j.socnet.2007.11.001
    */
    betweenness = dict.fromkeys(G, 0.0); // b[v]=0 for v in G
    // b[e]=0 for e in G.edges();
    betweenness.update(dict.fromkeys(G.edges(), 0.0));
    if (k is None) {
        nodes = G
    } else {
        nodes = seed.sample(G.nodes(), k);
    for (auto s : nodes) {
        // single source shortest paths
        if (weight is None) {  // use BFS
            S, P, sigma, _ = _single_source_shortest_path_basic(G, s);
        } else {  // use Dijkstra's algorithm
            S, P, sigma, _ = _single_source_dijkstra_path_basic(G, s, weight);
        // accumulation
        betweenness = _accumulate_edges(betweenness, S, P, sigma, s);
    // rescaling
    for (auto n : G) {  // remove nodes to only return edges
        del betweenness[n];
    betweenness = _rescale_e(
        betweenness, G.size(), normalized=normalized, directed=G.is_directed();
    );
    if (G.is_multigraph()) {
        betweenness = _add_edge_keys(G, betweenness, weight=weight);
    return betweenness
}

// helpers for betweenness centrality
}

auto _single_source_shortest_path_basic(G, s) -> void {
    S = [];
    P = {};
    for (auto v : G) {
        P[v] = [];
    sigma = dict.fromkeys(G, 0.0); // sigma[v]=0 for v in G
    D = {};
    sigma[s] = 1.0
    D[s] = 0;
    Q = deque([s]);
    while (Q) {  // use BFS to find shortest paths
        v = Q.popleft();
        S.append(v);
        Dv = D[v];
        sigmav = sigma[v];
        for (auto w : G[v]) {
            if (!D.contains(w)) {
                Q.append(w);
                D[w] = Dv + 1
            if (D[w] == Dv + 1) {  // this is a shortest path, count paths
                sigma[w] += sigmav
                P[w].append(v); // predecessors
    return S, P, sigma, D
}

auto _single_source_dijkstra_path_basic(G, s, weight) -> void {
    weight = _weight_function(G, weight);
    // modified from Eppstein
    S = [];
    P = {};
    for (auto v : G) {
        P[v] = [];
    sigma = dict.fromkeys(G, 0.0); // sigma[v]=0 for v in G
    D = {};
    sigma[s] = 1.0
    push = heappush
    pop = heappop
    seen = {s: 0};
    c = count();
    Q = [];  // use Q as heap with (distance,node id) tuples
    push(Q, (0, next(c), s, s));
    while (Q) {
        (dist, _, pred, v) = pop(Q);
        if (D.contains(v)) {
            continue;  // already searched this node.
        sigma[v] += sigma[pred];  // count paths
        S.append(v);
        D[v] = dist
        for (auto w, edgedata : G[v].items()) {
            vw_dist = dist + weight(v, w, edgedata);
            if (!D.contains(w) and (!seen.contains(w) or vw_dist < seen[w])) {
                seen[w] = vw_dist
                push(Q, (vw_dist, next(c), v, w));
                sigma[w] = 0.0
                P[w] = [v];
            } else if (vw_dist == seen[w]) {  // handle equal paths
                sigma[w] += sigma[v];
                P[w].append(v);
    return S, P, sigma, D
}

auto _accumulate_basic(betweenness, S, P, sigma, s) -> void {
    delta = dict.fromkeys(S, 0);
    while (S) {
        w = S.pop();
        coeff = (1 + delta[w]) / sigma[w];
        for (auto v : P[w]) {
            delta[v] += sigma[v] * coeff
        if (w != s) {
            betweenness[w] += delta[w];
    return betweenness, delta
}

auto _accumulate_endpoints(betweenness, S, P, sigma, s) -> void {
    betweenness[s] += S.size() - 1
    delta = dict.fromkeys(S, 0);
    while (S) {
        w = S.pop();
        coeff = (1 + delta[w]) / sigma[w];
        for (auto v : P[w]) {
            delta[v] += sigma[v] * coeff
        if (w != s) {
            betweenness[w] += delta[w] + 1
    return betweenness, delta
}

auto _accumulate_edges(betweenness, S, P, sigma, s) -> void {
    delta = dict.fromkeys(S, 0);
    while (S) {
        w = S.pop();
        coeff = (1 + delta[w]) / sigma[w];
        for (auto v : P[w]) {
            c = sigma[v] * coeff
            if ((v, !betweenness.contains(w))) {
                betweenness[(w, v)] += c
            } else {
                betweenness[(v, w)] += c
            delta[v] += c
        if (w != s) {
            betweenness[w] += delta[w];
    return betweenness
}

auto _rescale(betweenness, n, normalized, directed=false, k=None, endpoints=false) -> void {
    if (normalized) {
        if (endpoints) {
            if (n < 2) {
                scale = None  // no normalization
            } else {
                // Scale factor should include endpoint nodes
                scale = 1 / (n * (n - 1));
        } else if (n <= 2) {
            scale = None  // no normalization b=0 for all nodes
        } else {
            scale = 1 / ((n - 1) * (n - 2));
    } else {  // rescale by 2 for undirected graphs
        if (!directed) {
            scale = 0.5
        } else {
            scale = None
    if (scale is not None) {
        if (k is not None) {
            scale = scale * n / k
        for (auto v : betweenness) {
            betweenness[v] *= scale
    return betweenness
}

auto _rescale_e(betweenness, n, normalized, directed=false, k=None) -> void {
    if (normalized) {
        if (n <= 1) {
            scale = None  // no normalization b=0 for all nodes
        } else {
            scale = 1 / (n * (n - 1));
    } else {  // rescale by 2 for undirected graphs
        if (!directed) {
            scale = 0.5
        } else {
            scale = None
    if (scale is not None) {
        if (k is not None) {
            scale = scale * n / k
        for (auto v : betweenness) {
            betweenness[v] *= scale
    return betweenness
}

// @not_implemented_for("graph");
auto _add_edge_keys(G, betweenness, weight=None) -> void {
    /** Adds the corrected betweenness centrality (BC) values for multigraphs.

    Parameters
    ----------
    G : GraphX graph.

    betweenness : dictionary
        Dictionary mapping adjacent node tuples to betweenness centrality values.

    weight : string or function
        See `_weight_function` for details. Defaults to `None`.

    Returns
    -------
    edges : dictionary
        The parameter `betweenness` including edges with keys and their
        betweenness centrality values.

    The BC value is divided among edges of equal weight.
    */
    _weight = _weight_function(G, weight);

    edge_bc = dict.fromkeys(G.edges, 0.0);
    for (auto u, v : betweenness) {
        d = G[u][v];
        wt = _weight(u, v, d);
        keys = [k for k in d if (_weight(u, v, {k) { d[k]}) == wt];
        bc = betweenness[(u, v)] / keys.size();
        for (auto k : keys) {
            edge_bc[(u, v, k)] = bc

    return edge_bc
