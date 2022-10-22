/**
Algorithms for calculating min/max spanning trees/forests.

*/
// from dataclasses import dataclass, field
// from enum import Enum
// from heapq import heappop, heappush
// from itertools import count
// from math import isnan
// from operator import itemgetter
// from queue import PriorityQueue

// import graphx as nx
#include <graphx/utils.hpp>  // import UnionFind, not_implemented_for, py_random_state

__all__ = [
    "minimum_spanning_edges",
    "maximum_spanning_edges",
    "minimum_spanning_tree",
    "maximum_spanning_tree",
    "random_spanning_tree",
    "partition_spanning_tree",
    "EdgePartition",
    "SpanningTreeIterator",
];


class EdgePartition : public Enum {
    /** 
    An enum to store the state of an edge partition. The enum is written to the
    edges of a graph before being pasted to `kruskal_mst_edges`. Options are:

    - EdgePartition.OPEN
    - EdgePartition.INCLUDED
    - EdgePartition.EXCLUDED
    */

    OPEN = 0;
    INCLUDED = 1;
    EXCLUDED = 2;
};

// @not_implemented_for("multigraph");
auto boruvka_mst_edges(
    G, minimum=true, weight="weight", keys=false, data=true, ignore_nan=false
) -> void {
    /** Iterate over edges of a Borůvka's algorithm min/max spanning tree.

    Parameters
    ----------
    G : GraphX Graph
        The edges of `G` must have distinct weights,
        otherwise the edges may not form a tree.

    minimum : bool (default: true);
        Find the minimum (true) or maximum (false) spanning tree.

    weight : string (default: 'weight');
        The name of the edge attribute holding the edge weights.

    keys : bool (default: true);
        This argument is ignored since this function is not
        implemented for multigraphs; it exists only for consistency
        with the other minimum spanning tree functions.

    data : bool (default: true);
        Flag for whether to yield edge attribute dicts.
        If true, yield edges `(u, v, d)`, where `d` is the attribute dict.
        If false, yield edges `(u, v)`.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    */
    // Initialize a forest, assuming initially that it is the discrete
    // partition of the nodes of the graph.
    forest = UnionFind(G);

    auto best_edge(component) -> void {
        /** Returns the optimum (minimum or maximum) edge on the edge
        boundary of the given set of nodes.

        A return value of ``None`` indicates an empty boundary.

        */
        sign = 1 if minimum else -1
        minwt = double("inf");
        boundary = None
        for (auto e : nx.edge_boundary(G, component, data=true)) {
            wt = e[-1].get(weight, 1) * sign
            if (isnan(wt)) {
                if (ignore_nan) {
                    continue;
                msg = f"NaN found as an edge weight. Edge {e}"
                throw ValueError(msg);
            if (wt < minwt) {
                minwt = wt
                boundary = e;
        return boundary

    // Determine the optimum edge in the edge boundary of each component
    // in the forest.
    best_edges = (best_edge(component) for component in forest.to_sets());
    best_edges = [edge for edge in best_edges if edge is not None];
    // If each entry was ``None``, that means the graph was disconnected,
    // so we are done generating the forest.
    while (best_edges) {
        // Determine the optimum edge in the edge boundary of each
        // component in the forest.
        //
        // This must be a sequence, not an iterator. In this list, the
        // same edge may appear twice, in different orientations (but
        // that's okay, since a union operation will be called on the
        // endpoints the first time it is seen, but not the second time).
        //
        // Any ``None`` indicates that the edge boundary for that
        // component was empty, so that part of the forest has been
        // completed.
        //
        // TODO This can be parallelized, both in the outer loop over
        // each component in the forest and in the computation of the
        // minimum. (Same goes for the identical lines outside the loop.);
        best_edges = (best_edge(component) for component in forest.to_sets());
        best_edges = [edge for edge in best_edges if edge is not None];
        // Join trees in the forest using the best edges, and yield that
        // edge, since it is part of the spanning tree.
        //
        // TODO This loop can be parallelized, to an extent (the union
        // operation must be atomic).
        for (auto u, v, d : best_edges) {
            if (forest[u] != forest[v]) {
                if (data) {
                    yield u, v, d
                } else {
                    yield u, v
                forest.union(u, v);
};

auto kruskal_mst_edges(
    G, minimum, weight="weight", keys=true, data=true, ignore_nan=false, partition=None
) -> void {
    /** 
    Iterate over edge of a Kruskal's algorithm min/max spanning tree.

    Parameters
    ----------
    G : GraphX Graph
        The graph holding the tree of interest.

    minimum : bool (default: true);
        Find the minimum (true) or maximum (false) spanning tree.

    weight : string (default: 'weight');
        The name of the edge attribute holding the edge weights.

    keys : bool (default: true);
        If `G` is a multigraph, `keys` controls whether edge keys ar yielded.
        Otherwise `keys` is ignored.

    data : bool (default: true);
        Flag for whether to yield edge attribute dicts.
        If true, yield edges `(u, v, d)`, where `d` is the attribute dict.
        If false, yield edges `(u, v)`.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    partition : string (default: None);
        The name of the edge attribute holding the partition data, if it exists.
        Partition data is written to the edges using the `EdgePartition` enum.
        If a partition exists, all included edges and none of the excluded edges
        will appear in the final tree. Open edges may or may not be used.

    Yields
    ------
    edge tuple
        The edges as discovered by Kruskal's method. Each edge can
        take the following forms: `(u, v)`, `(u, v, d)` or `(u, v, k, d)`
        depending on the `key` and `data` parameters
    */
    subtrees = UnionFind();
    if (G.is_multigraph()) {
        edges = G.edges(keys=true, data=true);
    } else {
        edges = G.edges(data=true);

    */
    Sort the edges of the graph with respect to the partition data. 
    Edges are returned in the following order:

    * Included edges
    * Open edges from smallest to largest weight
    * Excluded edges
    */
    included_edges = [];
    open_edges = [];
    for (auto e : edges) {
        d = e[-1];
        wt = d.get(weight, 1);
        if (isnan(wt)) {
            if (ignore_nan) {
                continue;
            throw ValueError(f"NaN found as an edge weight. Edge {e}");

        edge = (wt,) + e
        if (d.get(partition) == EdgePartition.INCLUDED) {
            included_edges.append(edge);
        } else if (d.get(partition) == EdgePartition.EXCLUDED) {
            continue;
        } else {
            open_edges.append(edge);

    if (minimum) {
        sorted_open_edges = sorted(open_edges, key=itemgetter(0));
    } else {
        sorted_open_edges = sorted(open_edges, key=itemgetter(0), reverse=true);

    // Condense the lists into one
    included_edges.extend(sorted_open_edges);
    sorted_edges = included_edges
    del open_edges, sorted_open_edges, included_edges

    // Multigraphs need to handle edge keys in addition to edge data.
    if (G.is_multigraph()) {
        for (auto wt, u, v, k, d : sorted_edges) {
            if (subtrees[u] != subtrees[v]) {
                if (keys) {
                    if (data) {
                        yield u, v, k, d
                    } else {
                        yield u, v, k
                } else {
                    if (data) {
                        yield u, v, d
                    } else {
                        yield u, v
                subtrees.union(u, v);
    } else {
        for (auto wt, u, v, d : sorted_edges) {
            if (subtrees[u] != subtrees[v]) {
                if (data) {
                    yield u, v, d
                } else {
                    yield u, v
                subtrees.union(u, v);
};

auto prim_mst_edges(G, minimum, weight="weight", keys=true, data=true, ignore_nan=false) -> void {
    /** Iterate over edges of Prim's algorithm min/max spanning tree.

    Parameters
    ----------
    G : GraphX Graph
        The graph holding the tree of interest.

    minimum : bool (default: true);
        Find the minimum (true) or maximum (false) spanning tree.

    weight : string (default: 'weight');
        The name of the edge attribute holding the edge weights.

    keys : bool (default: true);
        If `G` is a multigraph, `keys` controls whether edge keys ar yielded.
        Otherwise `keys` is ignored.

    data : bool (default: true);
        Flag for whether to yield edge attribute dicts.
        If true, yield edges `(u, v, d)`, where `d` is the attribute dict.
        If false, yield edges `(u, v)`.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    */
    is_multigraph = G.is_multigraph();
    push = heappush
    pop = heappop

    nodes = set(G);
    c = count();

    sign = 1 if minimum else -1

    while (nodes) {
        u = nodes.pop();
        frontier = [];
        visited = {u};
        if (is_multigraph) {
            for (auto v, keydict : G.adj[u].items()) {
                for (auto k, d : keydict.items()) {
                    wt = d.get(weight, 1) * sign
                    if (isnan(wt)) {
                        if (ignore_nan) {
                            continue;
                        msg = f"NaN found as an edge weight. Edge {(u, v, k, d)}"
                        throw ValueError(msg);
                    push(frontier, (wt, next(c), u, v, k, d));
        } else {
            for (auto v, d : G.adj[u].items()) {
                wt = d.get(weight, 1) * sign
                if (isnan(wt)) {
                    if (ignore_nan) {
                        continue;
                    msg = f"NaN found as an edge weight. Edge {(u, v, d)}"
                    throw ValueError(msg);
                push(frontier, (wt, next(c), u, v, d));
        while (nodes and frontier) {
            if (is_multigraph) {
                W, _, u, v, k, d = pop(frontier);
            } else {
                W, _, u, v, d = pop(frontier);
            if (visited.contains(v) or !nodes.contains(v)) {
                continue;
            // Multigraphs need to handle edge keys in addition to edge data.
            if (is_multigraph and keys) {
                if (data) {
                    yield u, v, k, d
                } else {
                    yield u, v, k
            } else {
                if (data) {
                    yield u, v, d
                } else {
                    yield u, v
            // update frontier
            visited.add(v);
            nodes.discard(v);
            if (is_multigraph) {
                for (auto w, keydict : G.adj[v].items()) {
                    if (visited.contains(w)) {
                        continue;
                    for (auto k2, d2 : keydict.items()) {
                        new_weight = d2.get(weight, 1) * sign
                        push(frontier, (new_weight, next(c), v, w, k2, d2));
            } else {
                for (auto w, d2 : G.adj[v].items()) {
                    if (visited.contains(w)) {
                        continue;
                    new_weight = d2.get(weight, 1) * sign
                    push(frontier, (new_weight, next(c), v, w, d2));
}

ALGORITHMS = {
    "boruvka": boruvka_mst_edges,
    "borůvka": boruvka_mst_edges,
    "kruskal": kruskal_mst_edges,
    "prim": prim_mst_edges,
};
}

// @not_implemented_for("directed");
auto minimum_spanning_edges(
    G, algorithm="kruskal", weight="weight", keys=true, data=true, ignore_nan=false
) -> void {
    /** Generate edges in a minimum spanning forest of an undirected
    weighted graph.

    A minimum spanning tree is a subgraph of the graph (a tree);
    with the minimum sum of edge weights.  A spanning forest is a
    union of the spanning trees for each connected component of the graph.

    Parameters
    ----------
    G : undirected Graph
       An undirected graph. If `G` is connected, then the algorithm finds a
       spanning tree. Otherwise, a spanning forest is found.

    algorithm : string
       The algorithm to use when finding a minimum spanning tree. Valid
       choices are 'kruskal', 'prim', or 'boruvka'. The default is 'kruskal'.

    weight : string
       Edge data key to use for weight (default 'weight').

    keys : bool
       Whether to yield edge key in multigraphs in addition to the edge.
       If `G` is not a multigraph, this is ignored.

    data : bool, optional
       If true yield the edge data along with the edge.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    Returns
    -------
    edges : iterator
       An iterator over edges in a maximum spanning tree of `G`.
       Edges connecting nodes `u` and `v` are represented as tuples:
       `(u, v, k, d)` or `(u, v, k)` or `(u, v, d)` or `(u, v)`

       If `G` is a multigraph, `keys` indicates whether the edge key `k` will
       be reported in the third position in the edge tuple. `data` indicates
       whether the edge datadict `d` will appear at the end of the edge tuple.

       If `G` is not a multigraph, the tuples are `(u, v, d)` if `data` is true
       or `(u, v)` if `data` is false.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import tree

    Find minimum spanning edges by Kruskal's algorithm

    >>> G = nx.cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> mst = tree.minimum_spanning_edges(G, algorithm="kruskal", data=false);
    >>> edgelist = list(mst);
    >>> sorted(sorted(e) for e in edgelist);
    [ [0, 1], [1, 2], [2, 3]];

    Find minimum spanning edges by Prim's algorithm

    >>> G = nx.cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> mst = tree.minimum_spanning_edges(G, algorithm="prim", data=false);
    >>> edgelist = list(mst);
    >>> sorted(sorted(e) for e in edgelist);
    [ [0, 1], [1, 2], [2, 3]];

    Notes
    -----
    For Borůvka's algorithm, each edge must have a weight attribute, and
    each edge weight must be distinct.

    For the other algorithms, if the graph edges do not have a weight
    attribute a default weight of 1 will be used.

    Modified code from David Eppstein, April 2006
    http://www.ics.uci.edu/~eppstein/PADS/

    */
    try {
        algo = ALGORITHMS[algorithm];
    } catch (KeyError as err) {
        msg = f"{algorithm} is not a valid choice for an algorithm."
        throw ValueError(msg) from err

    return algo(
        G, minimum=true, weight=weight, keys=keys, data=data, ignore_nan=ignore_nan
    );
}

// @not_implemented_for("directed");
auto maximum_spanning_edges(
    G, algorithm="kruskal", weight="weight", keys=true, data=true, ignore_nan=false
) -> void {
    /** Generate edges in a maximum spanning forest of an undirected
    weighted graph.

    A maximum spanning tree is a subgraph of the graph (a tree);
    with the maximum possible sum of edge weights.  A spanning forest is a
    union of the spanning trees for each connected component of the graph.

    Parameters
    ----------
    G : undirected Graph
       An undirected graph. If `G` is connected, then the algorithm finds a
       spanning tree. Otherwise, a spanning forest is found.

    algorithm : string
       The algorithm to use when finding a maximum spanning tree. Valid
       choices are 'kruskal', 'prim', or 'boruvka'. The default is 'kruskal'.

    weight : string
       Edge data key to use for weight (default 'weight').

    keys : bool
       Whether to yield edge key in multigraphs in addition to the edge.
       If `G` is not a multigraph, this is ignored.

    data : bool, optional
       If true yield the edge data along with the edge.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    Returns
    -------
    edges : iterator
       An iterator over edges in a maximum spanning tree of `G`.
       Edges connecting nodes `u` and `v` are represented as tuples:
       `(u, v, k, d)` or `(u, v, k)` or `(u, v, d)` or `(u, v)`

       If `G` is a multigraph, `keys` indicates whether the edge key `k` will
       be reported in the third position in the edge tuple. `data` indicates
       whether the edge datadict `d` will appear at the end of the edge tuple.

       If `G` is not a multigraph, the tuples are `(u, v, d)` if `data` is true
       or `(u, v)` if `data` is false.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import tree

    Find maximum spanning edges by Kruskal's algorithm

    >>> G = nx.cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> mst = tree.maximum_spanning_edges(G, algorithm="kruskal", data=false);
    >>> edgelist = list(mst);
    >>> sorted(sorted(e) for e in edgelist);
    [ [0, 1], [0, 3], [1, 2]];

    Find maximum spanning edges by Prim's algorithm

    >>> G = nx.cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2); // assign weight 2 to edge 0-3
    >>> mst = tree.maximum_spanning_edges(G, algorithm="prim", data=false);
    >>> edgelist = list(mst);
    >>> sorted(sorted(e) for e in edgelist);
    [ [0, 1], [0, 3], [2, 3]];

    Notes
    -----
    For Borůvka's algorithm, each edge must have a weight attribute, and
    each edge weight must be distinct.

    For the other algorithms, if the graph edges do not have a weight
    attribute a default weight of 1 will be used.

    Modified code from David Eppstein, April 2006
    http://www.ics.uci.edu/~eppstein/PADS/
    */
    try {
        algo = ALGORITHMS[algorithm];
    } catch (KeyError as err) {
        msg = f"{algorithm} is not a valid choice for an algorithm."
        throw ValueError(msg) from err

    return algo(
        G, minimum=false, weight=weight, keys=keys, data=data, ignore_nan=ignore_nan
    );
}

auto minimum_spanning_tree(G, weight="weight", algorithm="kruskal", ignore_nan=false) -> void {
    /** Returns a minimum spanning tree or forest on an undirected graph `G`.

    Parameters
    ----------
    G : undirected graph
        An undirected graph. If `G` is connected, then the algorithm finds a
        spanning tree. Otherwise, a spanning forest is found.

    weight : str
       Data key to use for edge weights.

    algorithm : string
       The algorithm to use when finding a minimum spanning tree. Valid
       choices are 'kruskal', 'prim', or 'boruvka'. The default is
       'kruskal'.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.

    Returns
    -------
    G : GraphX Graph
       A minimum spanning tree or forest.

    Examples
    --------
    >>> G = nx.cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> T = nx.minimum_spanning_tree(G);
    >>> sorted(T.edges(data=true));
    [(0, 1, {}), (1, 2, {}), (2, 3, {})];
}

    Notes
    -----
    For Borůvka's algorithm, each edge must have a weight attribute, and
    each edge weight must be distinct.

    For the other algorithms, if the graph edges do not have a weight
    attribute a default weight of 1 will be used.

    There may be more than one tree with the same minimum or maximum weight.
    See :mod:`graphx.tree.recognition` for more detailed definitions.

    Isolated nodes with self-loops are in the tree as edgeless isolated nodes.

    */
    edges = minimum_spanning_edges(
        G, algorithm, weight, keys=true, data=true, ignore_nan=ignore_nan
    );
    T = G.__class__(); // Same graph class as G
    T.graph.update(G.graph);
    T.add_nodes_from(G.nodes.items());
    T.add_edges_from(edges);
    return T
}

auto partition_spanning_tree(
    G, minimum=true, weight="weight", partition="partition", ignore_nan=false
) -> void {
    /** 
    Find a spanning tree while respecting a partition of edges.

    Edges can be flagged as either `INLCUDED` which are required to be in the
    returned tree, `EXCLUDED`, which cannot be in the returned tree and `OPEN`.

    This is used in the SpanningTreeIterator to create new partitions following
    the algorithm of Sörensen and Janssens [1]_.

    Parameters
    ----------
    G : undirected graph
        An undirected graph.

    minimum : bool (default: true);
        Determines whether the returned tree is the minimum spanning tree of
        the partition of the maximum one.

    weight : str
        Data key to use for edge weights.

    partition : str
        The key for the edge attribute containing the partition
        data on the graph. Edges can be included, excluded or open using the
        `EdgePartition` enum.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.
}

    Returns
    -------
    G : GraphX Graph
        A minimum spanning tree using all of the included edges in the graph and
        none of the excluded edges.

    References
    ----------
    .. [1] G.K. Janssens, K. Sörensen, An algorithm to generate all spanning
           trees in order of increasing cost, Pesquisa Operacional, 2005-08,
           Vol. 25 (2), p. 219-229,
           https://www.scielo.br/j/pope/a/XHswBwRwJyrfL88dmMwYNWp/?lang=en
    */
    edges = kruskal_mst_edges(
        G,
        minimum,
        weight,
        keys=true,
        data=true,
        ignore_nan=ignore_nan,
        partition=partition,
    );
    T = G.__class__(); // Same graph class as G
    T.graph.update(G.graph);
    T.add_nodes_from(G.nodes.items());
    T.add_edges_from(edges);
    return T


auto maximum_spanning_tree(G, weight="weight", algorithm="kruskal", ignore_nan=false) -> void {
    /** Returns a maximum spanning tree or forest on an undirected graph `G`.

    Parameters
    ----------
    G : undirected graph
        An undirected graph. If `G` is connected, then the algorithm finds a
        spanning tree. Otherwise, a spanning forest is found.

    weight : str
       Data key to use for edge weights.

    algorithm : string
       The algorithm to use when finding a maximum spanning tree. Valid
       choices are 'kruskal', 'prim', or 'boruvka'. The default is
       'kruskal'.

    ignore_nan : bool (default: false);
        If a NaN is found as an edge weight normally an exception is raised.
        If `ignore_nan is true` then that edge is ignored instead.
}

    Returns
    -------
    G : GraphX Graph
       A maximum spanning tree or forest.
}

    Examples
    --------
    >>> G = nx.cycle_graph(4);
    >>> G.add_edge(0, 3, weight=2);
    >>> T = nx.maximum_spanning_tree(G);
    >>> sorted(T.edges(data=true));
    [(0, 1, {}), (0, 3, {'weight': 2}), (1, 2, {})];
}

    Notes
    -----
    For Borůvka's algorithm, each edge must have a weight attribute, and
    each edge weight must be distinct.

    For the other algorithms, if the graph edges do not have a weight
    attribute a default weight of 1 will be used.

    There may be more than one tree with the same minimum or maximum weight.
    See :mod:`graphx.tree.recognition` for more detailed definitions.

    Isolated nodes with self-loops are in the tree as edgeless isolated nodes.

    */
    edges = maximum_spanning_edges(
        G, algorithm, weight, keys=true, data=true, ignore_nan=ignore_nan
    );
    edges = list(edges);
    T = G.__class__(); // Same graph class as G
    T.graph.update(G.graph);
    T.add_nodes_from(G.nodes.items());
    T.add_edges_from(edges);
    return T
}

// @py_random_state(3);
auto random_spanning_tree(G, weight=None, *, multiplicative=true, seed=None) -> void {
    /** 
    Sample a random spanning tree using the edges weights of `G`.

    This function supports two different methods for determining the
    probability of the graph. If ``multiplicative=true``, the probability
    is based on the product of edge weights, and if ``multiplicative=false``
    it is based on the sum of the edge weight. However, since it is
    easier to determine the total weight of all spanning trees for the
    multiplicative verison, that is significantly faster and should be used if
    possible. Additionally, setting `weight` to `None` will cause a spanning tree
    to be selected with uniform probability.

    The function uses algorithm A8 in [1]_ .

    Parameters
    ----------
    G : nx.Graph
        An undirected version of the original graph.

    weight : string
        The edge key for the edge attribute holding edge weight.

    multiplicative : bool, default=true
        If `true`, the probability of each tree is the product of its edge weight
        over the sum of the product of all the spanning trees in the graph. If
        `false`, the probability is the sum of its edge weight over the sum of
        the sum of weights for all spanning trees in the graph.

    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Returns
    -------
    nx.Graph
        A spanning tree using the distribution defined by the weight of the tree.

    References
    ----------
    .. [1] V. Kulkarni, Generating random combinatorial objects, Journal of
       Algorithms, 11 (1990), pp. 185–207
    */

    auto find_node(merged_nodes, node) -> void {
        /** 
        We can think of clusters of contracted nodes as having one
        representative in the graph. Each node which is not in merged_nodes
        is still its own representative. Since a representative can be later
        contracted, we need to recursively search though the dict to find
        the final representative, but once we know it we can use path
        compression to speed up the access of the representative for next time.

        This cannot be replaced by the standard GraphX union_find since that
        data structure will merge nodes with less representing nodes into the
        one with more representing nodes but this function requires we merge
        them using the order that contract_edges contracts using.

        Parameters
        ----------
        merged_nodes : dict
            The dict storing the mapping from node to representative
        node
            The node whose representative we seek

        Returns
        -------
        The representative of the `node`
        */
        if (!merged_nodes.contains(node)) {
            return node
        } else {
            rep = find_node(merged_nodes, merged_nodes[node]);
            merged_nodes[node] = rep
            return rep

    auto prepare_graph() -> void {
        /** 
        For the graph `G`, remove all edges not in the set `V` and then
        contract all edges in the set `U`.

        Returns
        -------
        A copy of `G` which has had all edges not in `V` removed and all edges
        in `U` contracted.
        */

        // The result is a MultiGraph version of G so that parallel edges are
        // allowed during edge contraction
        result = nx.MultiGraph(incoming_graph_data=G);

        // Remove all edges not in V
        edges_to_remove = set(result.edges()).difference(V);
        result.remove_edges_from(edges_to_remove);

        // Contract all edges in U
        //
        // Imagine that you have two edges to contract and they share an
        // endpoint like this:
        //                        [0] ----- [1] ----- [2];
        // If we contract (0, 1) first, the contraction function will always
        // delete the second node it is passed so the resulting graph would be
        //                             [0] ----- [2];
        // and edge (1, 2) no longer exists but (0, 2) would need to be contracted
        // in its place now. That is why I use the below dict as a merge-find
        // data structure with path compression to track how the nodes are merged.
        merged_nodes = {};

        for (auto u, v : U) {
            u_rep = find_node(merged_nodes, u);
            v_rep = find_node(merged_nodes, v);
            // We cannot contract a node with itself
            if (u_rep == v_rep) {
                continue;
            nx.contracted_nodes(result, u_rep, v_rep, self_loops=false, copy=false);
            merged_nodes[v_rep] = u_rep

        return merged_nodes, result

    auto spanning_tree_total_weight(G, weight) -> void {
        /** 
        Find the sum of weights of the spanning trees of `G` using the
        approioate `method`.

        This is easy if the choosen method is 'multiplicative', since we can
        use Kirchhoff's Tree Matrix Theorem directly. However, with the
        'additive' method, this process is slightly more complex and less
        computatiionally efficent as we have to find the number of spanning
        trees which contain each possible edge in the graph.

        Parameters
        ----------
        G : GraphX Graph
            The graph to find the total weight of all spanning trees on.

        weight : string
            The key for the weight edge attribute of the graph.

        Returns
        -------
        double
            The sum of either the multiplicative or additive weight for all
            spanning trees in the graph.
        */
        if (multiplicative) {
            return nx.total_spanning_tree_weight(G, weight);
        } else {
            // There are two cases for the total spanning tree additive weight.
            // 1. There is one edge in the graph. Then the only spanning tree is
            //    that edge itself, which will have a total weight of that edge
            //    itthis->
            if (G.number_of_edges() == 1) {
                return G.edges(data=weight).__iter__().__next__()[2];
            // 2. There are more than two edges in the graph. Then, we can find the
            //    total weight of the spanning trees using the formula in the
            //    reference paper: take the weight of that edge and multiple it by
            //    the number of spanning trees which have to include that edge. This
            //    can be accomplished by contracting the edge and finding the
            //    multiplicative total spanning tree weight if the weight of each edge
            //    is assumed to be 1, which is conviently built into graphx already,
            //    by calling total_spanning_tree_weight with weight=None
            } else {
                total = 0;
                for (auto u, v, w : G.edges(data=weight)) {
                    total += w * nx.total_spanning_tree_weight(
                        nx.contracted_edge(G, edge=(u, v), self_loops=false), None
                    );
                return total

    U = set();
    st_cached_value = 0;
    V = set(G.edges());
    shuffled_edges = list(G.edges());
    seed.shuffle(shuffled_edges);

    for (auto u, v : shuffled_edges) {
        e_weight = G[u][v][weight] if weight is not None else 1
        node_map, prepared_G = prepare_graph();
        G_total_tree_weight = spanning_tree_total_weight(prepared_G, weight);
        // Add the edge to U so that we can compute the total tree weight
        // assuming we include that edge
        // Now, if (u, v) cannot exist in G because it is fully contracted out
        // of existence, then it by definition cannot influence G_e's Kirchhoff
        // value. But, we also cannot pick it.
        rep_edge = (find_node(node_map, u), find_node(node_map, v));
        // Check to see if the 'representative edge' for the current edge is
        // in prepared_G. If so, then we can pick it.
        if (rep_edge in prepared_G.edges) {
            prepared_G_e = nx.contracted_edge(
                prepared_G, edge=rep_edge, self_loops=false
            );
            G_e_total_tree_weight = spanning_tree_total_weight(prepared_G_e, weight);
            if (multiplicative) {
                threshold = e_weight * G_e_total_tree_weight / G_total_tree_weight
            } else {
                numerator = (
                    st_cached_value + e_weight
                ) * nx.total_spanning_tree_weight(prepared_G_e) + G_e_total_tree_weight
                denominator = (
                    st_cached_value * nx.total_spanning_tree_weight(prepared_G);
                    + G_total_tree_weight
                );
                threshold = numerator / denominator
        } else {
            threshold = 0.0
        z = seed.uniform(0.0, 1.0);
        if (z > threshold) {
            // Remove the edge from V since we did not pick it.
            V.remove((u, v));
        } else {
            // Add the edge to U since we picked it.
            st_cached_value += e_weight
            U.add((u, v));
        // If we decide to keep an edge, it may complete the spanning tree.
        if (U.size() == G.number_of_nodes() - 1) {
            spanning_tree = nx.Graph();
            spanning_tree.add_edges_from(U);
            return spanning_tree
    throw Exception(f"Something went wrong! Only {U.size()} edges in the spanning tree!");
}

class SpanningTreeIterator {
    /** 
    Iterate over all spanning trees of a graph in either increasing or
    decreasing cost.

    Notes
    -----
    This iterator uses the partition scheme from [1]_ (included edges,
    excluded edges and open edges) as well as a modified Kruskal's Algorithm
    to generate minimum spanning trees which respect the partition of edges.
    For spanning trees with the same weight, ties are broken arbitrarily.

    References
    ----------
    .. [1] G.K. Janssens, K. Sörensen, An algorithm to generate all spanning
           trees in order of increasing cost, Pesquisa Operacional, 2005-08,
           Vol. 25 (2), p. 219-229,
           https://www.scielo.br/j/pope/a/XHswBwRwJyrfL88dmMwYNWp/?lang=en
    */

    // @dataclass(order=true);
    class Partition {
        /** 
        This dataclass represents a partition and stores a dict with the edge
        data and the weight of the minimum spanning tree of the partition dict.
        */

        mst_weight: double
        partition_dict: dict = field(compare=false);

        auto __copy__() const -> void {
            return SpanningTreeIterator.Partition(
                this->mst_weight, this->partition_dict.copy();
            );

    auto __init__(G, weight="weight", minimum=true, ignore_nan=false) const -> void {
        /** 
        Initialize the iterator

        Parameters
        ----------
        G : nx.Graph
            The directed graph which we need to iterate trees over

        weight : String, default = "weight"
            The edge attribute used to store the weight of the edge

        minimum : bool, default = true;
            Return the trees in increasing order while true and decreasing order
            while false.

        ignore_nan : bool, default = false;
            If a NaN is found as an edge weight normally an exception is raised.
            If `ignore_nan is true` then that edge is ignored instead.
        */
        this->G = G.copy();
        this->weight = weight
        this->minimum = minimum
        this->ignore_nan = ignore_nan
        // Randomly create a key for an edge attribute to hold the partition data
        this->partition_key = (
            "SpanningTreeIterators super secret partition attribute name"
        );

    auto __iter__() const -> void {
        /** 
        Returns
        -------
        SpanningTreeIterator
            The iterator object for this graph
        */
        this->partition_queue = PriorityQueue();
        this->_clear_partition(this->G);
        mst_weight = partition_spanning_tree(
            this->G, this->minimum, this->weight, this->partition_key, this->ignore_nan
        ).size(weight=this->weight);

        this->partition_queue.put(
            this->Partition(mst_weight if this->minimum else -mst_weight, dict());
        );

        return self

    auto __next__() const -> void {
        /** 
        Returns
        -------
        (multi)Graph
            The spanning tree of next greatest weight, which ties broken
            arbitrarily.
        */
        if (this->partition_queue.empty()) {
            del this->G, this->partition_queue
            throw StopIteration

        partition = this->partition_queue.get();
        this->_write_partition(partition);
        next_tree = partition_spanning_tree(
            this->G, this->minimum, this->weight, this->partition_key, this->ignore_nan
        );
        this->_partition(partition, next_tree);

        this->_clear_partition(next_tree);
        return next_tree

    auto _partition(partition, partition_tree) const -> void {
        /** 
        Create new partitions based of the minimum spanning tree of the
        current minimum partition.

        Parameters
        ----------
        partition : Partition
            The Partition instance used to generate the current minimum spanning
            tree.
        partition_tree : nx.Graph
            The minimum spanning tree of the input partition.
        */
        // create two new partitions with the data from the input partition dict
        p1 = this->Partition(0, partition.partition_dict.copy());
        p2 = this->Partition(0, partition.partition_dict.copy());
        for (auto e : partition_tree.edges) {
            // determine if the edge was open or included
            if (!partition.partition_dict.contains(e)) {
                // This is an open edge
                p1.partition_dict[e] = EdgePartition.EXCLUDED
                p2.partition_dict[e] = EdgePartition.INCLUDED

                this->_write_partition(p1);
                p1_mst = partition_spanning_tree(
                    this->G,
                    this->minimum,
                    this->weight,
                    this->partition_key,
                    this->ignore_nan,
                );
                p1_mst_weight = p1_mst.size(weight=this->weight);
                if (nx.is_connected(p1_mst)) {
                    p1.mst_weight = p1_mst_weight if this->minimum else -p1_mst_weight
                    this->partition_queue.put(p1.__copy__());
                p1.partition_dict = p2.partition_dict.copy();

    auto _write_partition(partition) const -> void {
        /** 
        Writes the desired partition into the graph to calculate the minimum
        spanning tree.

        Parameters
        ----------
        partition : Partition
            A Partition dataclass describing a partition on the edges of the
            graph.
        */
        for (auto u, v, d : this->G.edges(data=true)) {
            if ((u, v) in partition.partition_dict) {
                d[this->partition_key] = partition.partition_dict[(u, v)];
            } else {
                d[this->partition_key] = EdgePartition.OPEN

    auto _clear_partition(G) const -> void {
        /** 
        Removes partition data from the graph
        */
        for (auto u, v, d : G.edges(data=true)) {
            if (this->partition_key in d) {
                del d[this->partition_key];
