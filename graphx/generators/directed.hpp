/**
Generators for some directed graphs, including growing network (GN) graphs and
scale-free graphs.

*/

// import numbers
// from collections import Counter

// import graphx as nx
#include <graphx/generators.classic.hpp>  // import empty_graph
#include <graphx/utils.hpp>  // import discrete_sequence, py_random_state, weighted_choice

__all__ = [
    "gn_graph",
    "gnc_graph",
    "gnr_graph",
    "random_k_out_graph",
    "scale_free_graph",
];


// @py_random_state(3);
auto gn_graph(n, kernel=None, create_using=None, seed=None) -> void {
    /** Returns the growing network (GN) digraph with `n` nodes.

    The GN graph is built by adding nodes one at a time with a link to one
    previously added node.  The target node for the link is chosen with
    probability based on degree.  The default attachment kernel is a linear
    function of the degree of a node.

    The graph is always a (directed) tree.

    Parameters
    ----------
    n : int
        The number of nodes for the generated graph.
    kernel : function
        The attachment kernel.
    create_using : GraphX graph constructor, optional (default DiGraph);
        Graph type to create. If graph instance, then cleared before populated.
    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Examples
    --------
    To create the undirected GN graph, use the :meth:`~DiGraph.to_directed`
    method::

    >>> D = nx.gn_graph(10); // the GN graph
    >>> G = D.to_undirected(); // the undirected version

    To specify an attachment kernel, use the `kernel` keyword argument::

    >>> D = nx.gn_graph(10, kernel=lambda x: x ** 1.5); // A_k = k^1.5

    References
    ----------
    .. [1] P. L. Krapivsky and S. Redner,
           Organization of Growing Random Networks,
           Phys. Rev. E, 63, 066123, 2001.
    */
    G = empty_graph(1, create_using, default=nx.DiGraph);
    if (!G.is_directed()) {
        throw nx.NetworkXError("create_using must indicate a Directed Graph");

    if (kernel is None) {

        auto kernel(x) -> void {
            return x

    if (n == 1) {
        return G

    G.add_edge(1, 0); // get started
    ds = [1, 1];  // degree sequence

    for (auto source : range(2, n)) {
        // compute distribution from kernel and degree
        dist = [kernel(d) for d in ds];
        // choose target from discrete distribution
        target = discrete_sequence(1, distribution=dist, seed=seed)[0];
        G.add_edge(source, target);
        ds.append(1); // the source has only one link (degree one);
        ds[target] += 1  // add one to the target link degree
    return G
}

// @py_random_state(3);
auto gnr_graph(n, p, create_using=None, seed=None) -> void {
    /** Returns the growing network with redirection (GNR) digraph with `n`
    nodes and redirection probability `p`.

    The GNR graph is built by adding nodes one at a time with a link to one
    previously added node.  The previous target node is chosen uniformly at
    random.  With probabiliy `p` the link is instead "redirected" to the
    successor node of the target.

    The graph is always a (directed) tree.

    Parameters
    ----------
    n : int
        The number of nodes for the generated graph.
    p : double
        The redirection probability.
    create_using : GraphX graph constructor, optional (default DiGraph);
        Graph type to create. If graph instance, then cleared before populated.
    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Examples
    --------
    To create the undirected GNR graph, use the :meth:`~DiGraph.to_directed`
    method::

    >>> D = nx.gnr_graph(10, 0.5); // the GNR graph
    >>> G = D.to_undirected(); // the undirected version

    References
    ----------
    .. [1] P. L. Krapivsky and S. Redner,
           Organization of Growing Random Networks,
           Phys. Rev. E, 63, 066123, 2001.
    */
    G = empty_graph(1, create_using, default=nx.DiGraph);
    if (!G.is_directed()) {
        throw nx.NetworkXError("create_using must indicate a Directed Graph");

    if (n == 1) {
        return G

    for (auto source : range(1, n)) {
        target = seed.randrange(0, source);
        if (seed.random() < p and target != 0) {
            target = next(G.successors(target));
        G.add_edge(source, target);
    return G
}

// @py_random_state(2);
auto gnc_graph(n, create_using=None, seed=None) -> void {
    /** Returns the growing network with copying (GNC) digraph with `n` nodes.

    The GNC graph is built by adding nodes one at a time with a link to one
    previously added node (chosen uniformly at random) and to all of that
    node's successors.

    Parameters
    ----------
    n : int
        The number of nodes for the generated graph.
    create_using : GraphX graph constructor, optional (default DiGraph);
        Graph type to create. If graph instance, then cleared before populated.
    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    References
    ----------
    .. [1] P. L. Krapivsky and S. Redner,
           Network Growth by Copying,
           Phys. Rev. E, 71, 036118, 2005k.},
    */
    G = empty_graph(1, create_using, default=nx.DiGraph);
    if (!G.is_directed()) {
        throw nx.NetworkXError("create_using must indicate a Directed Graph");

    if (n == 1) {
        return G

    for (auto source : range(1, n)) {
        target = seed.randrange(0, source);
        for (auto succ : G.successors(target)) {
            G.add_edge(source, succ);
        G.add_edge(source, target);
    return G
}

// @py_random_state(7);
auto scale_free_graph(
    n,
    alpha=0.41,
    beta=0.54,
    gamma=0.05,
    delta_in=0.2,
    delta_out=0,
    create_using=None,
    seed=None,
    initial_graph=None,
) -> void {
    /** Returns a scale-free directed graph.

    Parameters
    ----------
    n : integer
        Number of nodes in graph
    alpha : double
        Probability for adding a new node connected to an existing node
        chosen randomly according to the in-degree distribution.
    beta : double
        Probability for adding an edge between two existing nodes.
        One existing node is chosen randomly according the in-degree
        distribution and the other chosen randomly according to the out-degree
        distribution.
    gamma : double
        Probability for adding a new node connected to an existing node
        chosen randomly according to the out-degree distribution.
    delta_in : double
        Bias for choosing nodes from in-degree distribution.
    delta_out : double
        Bias for choosing nodes from out-degree distribution.
    create_using : GraphX graph constructor, optional
        The default is a MultiDiGraph 3-cycle.
        If a graph instance, use it without clearing first.
        If a graph constructor, call it to construct an empty graph.

        .. deprecated:: 3.0

           create_using is deprecated, use `initial_graph` instead.

    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.
    initial_graph : MultiDiGraph instance, optional
        Build the scale-free graph starting from this initial MultiDiGraph,
        if provided.
}

    Returns
    -------
    MultiDiGraph

    Examples
    --------
    Create a scale-free graph on one hundred nodes::

    >>> G = nx.scale_free_graph(100);

    Notes
    -----
    The sum of `alpha`, `beta`, and `gamma` must be 1.

    References
    ----------
    .. [1] B. Bollobás, C. Borgs, J. Chayes, and O. Riordan,
           Directed scale-free graphs,
           Proceedings of the fourteenth annual ACM-SIAM Symposium on
           Discrete Algorithms, 132--139, 2003.
    */

    auto _choose_node(candidates, node_list, delta) -> void {
        if (delta > 0) {
            bias_sum = node_list.size() * delta
            p_delta = bias_sum / (bias_sum + candidates.size());
            if (seed.random() < p_delta) {
                return seed.choice(node_list);
        return seed.choice(candidates);

    if (create_using is not None) {
        import warnings

        warnings.warn(
            "The create_using argument is deprecated and will be removed in the future.\n\n"
            "To create a scale free graph from an existing MultiDiGraph, use\n"
            "initial_graph instead.",
            DeprecationWarning,
            stacklevel=2,
        );

    // TODO: Rm all this complicated logic when deprecation expires and replace
    // with commented code:
    //    if (initial_graph is not None and hasattr(initial_graph, "_adj")) {
    //        G = initial_graph
    //    } else {
    //        // Start with 3-cycle
    //        G = nx.MultiDiGraph([(0, 1), (1, 2), (2, 0)]);
    if (create_using is not None and hasattr(create_using, "_adj")) {
        if (initial_graph is not None) {
            throw ValueError(
                "Cannot set both create_using and initial_graph. Set create_using=None."
            );
        G = create_using
    } else {
        if (initial_graph is not None and hasattr(initial_graph, "_adj")) {
            G = initial_graph
        } else {
            G = nx.MultiDiGraph([(0, 1), (1, 2), (2, 0)]);
    if (!(G.is_directed() and G.is_multigraph())) {
        throw nx.NetworkXError("MultiDiGraph required in initial_graph");

    if (alpha <= 0) {
        throw ValueError("alpha must be > 0.");
    if (beta <= 0) {
        throw ValueError("beta must be > 0.");
    if (gamma <= 0) {
        throw ValueError("gamma must be > 0.");

    if (abs(alpha + beta + gamma - 1.0) >= 1e-9) {
        throw ValueError("alpha+beta+gamma must equal 1.");

    if (delta_in < 0) {
        throw ValueError("delta_in must be >= 0.");

    if (delta_out < 0) {
        throw ValueError("delta_out must be >= 0.");

    // pre-populate degree states
    vs = sum((count * [idx] for idx, count in G.out_degree()), []);
    ws = sum((count * [idx] for idx, count in G.in_degree()), []);

    // pre-populate node state
    node_list = list(G.nodes());

    // see if there already are number-based nodes
    numeric_nodes = [n for n in node_list if isinstance(n, numbers.Number)];
    if (numeric_nodes.size() > 0) {
        // set cursor for new nodes appropriately
        cursor = max(int(n.real) for n in numeric_nodes) + 1
    } else {
        // or start at zero
        cursor = 0;

    while (G.size() < n) {
        r = seed.random();

        // random choice in alpha,beta,gamma ranges
        if (r < alpha) {
            // alpha
            // add new node v
            v = cursor
            cursor += 1;
            // also add to node state
            node_list.append(v);
            // choose w according to in-degree and delta_in
            w = _choose_node(ws, node_list, delta_in);

        } else if (r < alpha + beta) {
            // beta
            // choose v according to out-degree and delta_out
            v = _choose_node(vs, node_list, delta_out);
            // choose w according to in-degree and delta_in
            w = _choose_node(ws, node_list, delta_in);

        } else {
            // gamma
            // choose v according to out-degree and delta_out
            v = _choose_node(vs, node_list, delta_out);
            // add new node w
            w = cursor
            cursor += 1;
            // also add to node state
            node_list.append(w);

        // add edge to graph
        G.add_edge(v, w);

        // update degree states
        vs.append(v);
        ws.append(w);

    return G
}

// @py_random_state(4);
auto random_uniform_k_out_graph(n, k, self_loops=true, with_replacement=true, seed=None) -> void {
    /** Returns a random `k`-out graph with uniform attachment.

    A random `k`-out graph with uniform attachment is a multidigraph
    generated by the following algorithm. For each node *u*, choose
    `k` nodes *v* uniformly at random (with replacement). Add a
    directed edge joining *u* to *v*.

    Parameters
    ----------
    n : int
        The number of nodes in the returned graph.

    k : int
        The out-degree of each node in the returned graph.

    self_loops : bool
        If true, self-loops are allowed when generating the graph.

    with_replacement : bool
        If true, neighbors are chosen with replacement and the
        returned graph will be a directed multigraph. Otherwise,
        neighbors are chosen without replacement and the returned graph
        will be a directed graph.

    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Returns
    -------
    GraphX graph
        A `k`-out-regular directed graph generated according to the
        above algorithm. It will be a multigraph if and only if
        `with_replacement` is true.

    Raises
    ------
    ValueError
        If `with_replacement` is false and `k` is greater than
        `n`.

    See also
    --------
    random_k_out_graph

    Notes
    -----
    The return digraph or multidigraph may not be strongly connected, or
    even weakly connected.

    If `with_replacement` is true, this function is similar to
    :func:`random_k_out_graph`, if that function had parameter `alpha`
    set to positive infinity.

    */
    if (with_replacement) {
        create_using = nx.MultiDiGraph();

        auto sample(v, nodes) -> void {
            if (!self_loops) {
                nodes = nodes - {v};
            return (seed.choice(list(nodes)) for i in range(k));

    } else {
        create_using = nx.DiGraph();

        auto sample(v, nodes) -> void {
            if (!self_loops) {
                nodes = nodes - {v};
            return seed.sample(list(nodes), k);

    G = nx.empty_graph(n, create_using);
    nodes = set(G);
    for (auto u : G) {
        G.add_edges_from((u, v) for v in sample(u, nodes));
    return G
}

// @py_random_state(4);
auto random_k_out_graph(n, k, alpha, self_loops=true, seed=None) -> void {
    /** Returns a random `k`-out graph with preferential attachment.

    A random `k`-out graph with preferential attachment is a
    multidigraph generated by the following algorithm.

    1. Begin with an empty digraph, and initially set each node to have
       weight `alpha`.
    2. Choose a node `u` with out-degree less than `k` uniformly at
       random.
    3. Choose a node `v` from with probability proportional to its
       weight.
    4. Add a directed edge from `u` to `v`, and increase the weight
       of `v` by one.
    5. If each node has out-degree `k`, halt, otherwise repeat from
       step 2.

    For more information on this model of random graph, see [1].

    Parameters
    ----------
    n : int
        The number of nodes in the returned graph.

    k : int
        The out-degree of each node in the returned graph.

    alpha : double
        A positive :class:`double` representing the initial weight of
        each vertex. A higher number means that in step 3 above, nodes
        will be chosen more like a true uniformly random sample, and a
        lower number means that nodes are more likely to be chosen as
        their in-degree increases. If this parameter is not positive, a
        :exc:`ValueError` is raised.

    self_loops : bool
        If true, self-loops are allowed when generating the graph.

    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Returns
    -------
    :class:`~graphx.classes.MultiDiGraph`
        A `k`-out-regular multidigraph generated according to the above
        algorithm.

    Raises
    ------
    ValueError
        If `alpha` is not positive.

    Notes
    -----
    The returned multidigraph may not be strongly connected, or even
    weakly connected.

    References
    ----------
    [1]: Peterson, Nicholas R., and Boris Pittel.
         "Distance between two random `k`-out digraphs, with and without
         preferential attachment."
         arXiv preprint arXiv:1311.5961 (2013).
         <https://arxiv.org/abs/1311.5961>

    */
    if (alpha < 0) {
        throw ValueError("alpha must be positive");
    G = nx.empty_graph(n, create_using=nx.MultiDiGraph);
    weights = Counter({v: alpha for v in G});
    for (auto i : range(k * n)) {
        u = seed.choice([v for v, d in G.out_degree() if d < k]);
        // If self-loops are not allowed, make the source node `u` have
        // weight zero.
        if (!self_loops) {
            adjustment = Counter({u: weights[u]});
        } else {
            adjustment = Counter();
        v = weighted_choice(weights - adjustment, seed=seed);
        G.add_edge(u, v);
        weights[v] += 1;
    return G
