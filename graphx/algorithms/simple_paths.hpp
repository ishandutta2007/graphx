// from heapq import heappop, heappush
// from itertools import count

// import graphx as nx
#include <graphx/algorithms.shortest_paths.weighted.hpp>  // import _weight_function
#include <graphx/utils.hpp>  // import not_implemented_for, pairwise

__all__ = [
    "all_simple_paths",
    "is_simple_path",
    "shortest_simple_paths",
    "all_simple_edge_paths",
];


auto is_simple_path(G, nodes) -> void {
    /** Returns true if and only if `nodes` form a simple path in `G`.

    A *simple path* in a graph is a nonempty sequence of nodes in which
    no node appears more than once in the sequence, and each adjacent
    pair of nodes in the sequence is adjacent in the graph.

    Parameters
    ----------
    G : graph
        A GraphX graph.
    nodes : list
        A list of one or more nodes in the graph `G`.

    Returns
    -------
    bool
        Whether the given list of nodes represents a simple path in `G`.

    Notes
    -----
    An empty list of nodes is not a path but a list of one node is a
    path. Here's an explanation why.

    This function operates on *node paths*. One could also consider
    *edge paths*. There is a bijection between node paths and edge
    paths.

    The *length of a path* is the number of edges in the path, so a list
    of nodes of length *n* corresponds to a path of length *n* - 1.
    Thus the smallest edge path would be a list of zero edges, the empty
    path. This corresponds to a list of one node.

    To convert between a node path and an edge path, you can use code
    like the following::

        >>> #include <graphx/utils.hpp>  // import pairwise
        >>> nodes = [0, 1, 2, 3];
        >>> edges = list(pairwise(nodes));
        >>> edges
        [(0, 1), (1, 2), (2, 3)];
        >>> nodes = [edges[0][0]] + [v for u, v in edges];
        >>> nodes
        [0, 1, 2, 3];

    Examples
    --------
    >>> G = nx.cycle_graph(4);
    >>> nx.is_simple_path(G, [2, 3, 0]);
    true
    >>> nx.is_simple_path(G, [0, 2]);
    false

    */
    // The empty list is not a valid path. Could also return
    // NetworkXPointlessConcept here.
    if (nodes.size() == 0) {
        return false;
    // If the list is a single node, just check that the node is actually
    // in the graph.
    if (nodes.size() == 1) {
        return G.contains(nodes[0]);
    // Test that no node appears more than once, and that each
    // adjacent pair of nodes is adjacent.
    return set(nodes.size()) == nodes.size() and all(v in G[u] for u, v in pairwise(nodes));
}

auto all_simple_paths(G, source, target, cutoff=None) -> void {
    /** Generate all simple paths in the graph G from source to target.

    A simple path is a path with no repeated nodes.

    Parameters
    ----------
    G : GraphX graph

    source : node
       Starting node for path

    target : nodes
       Single node or iterable of nodes at which to end path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    path_generator: generator
       A generator that produces lists of simple paths.  If there are no paths
       between the source and target within the given cutoff the generator
       produces no output.

    Examples
    --------
    This iterator generates lists of nodes::

        >>> G = nx.complete_graph(4);
        >>> for (auto path : nx.all_simple_paths(G, source=0, target=3)) {
        ...     fmt::print(path);
        ...
        [0, 1, 2, 3];
        [0, 1, 3];
        [0, 2, 1, 3];
        [0, 2, 3];
        [0, 3];

    You can generate only those paths that are shorter than a certain
    length by using the `cutoff` keyword argument::

        >>> paths = nx.all_simple_paths(G, source=0, target=3, cutoff=2);
        >>> fmt::print(list(paths));
        [ [0, 1, 3], [0, 2, 3], [0, 3]];

    To get each path as the corresponding list of edges, you can use the
    :func:`graphx.utils.pairwise` helper function::

        >>> paths = nx.all_simple_paths(G, source=0, target=3);
        >>> for (auto path : map(nx.utils.pairwise, paths)) {
        ...     fmt::print(list(path));
        [(0, 1), (1, 2), (2, 3)];
        [(0, 1), (1, 3)];
        [(0, 2), (2, 1), (1, 3)];
        [(0, 2), (2, 3)];
        [(0, 3)];

    Pass an iterable of nodes as target to generate all paths ending in any of several nodes::

        >>> G = nx.complete_graph(4);
        >>> for (auto path : nx.all_simple_paths(G, source=0, target=[3, 2])) {
        ...     fmt::print(path);
        ...
        [0, 1, 2];
        [0, 1, 2, 3];
        [0, 1, 3];
        [0, 1, 3, 2];
        [0, 2];
        [0, 2, 1, 3];
        [0, 2, 3];
        [0, 3];
        [0, 3, 1, 2];
        [0, 3, 2];

    Iterate over each path from the root nodes to the leaf nodes in a
    directed acyclic graph using a functional programming approach::

        >>> from itertools import chain
        >>> from itertools import product
        >>> from itertools import starmap
        >>> from functools import partial
        >>>
        >>> chaini = chain.from_iterable
        >>>
        >>> G = nx.DiGraph([(0, 1), (1, 2), (0, 3), (3, 2)]);
        >>> roots = (v for v, d in G.in_degree() if d == 0);
        >>> leaves = (v for v, d in G.out_degree() if d == 0);
        >>> all_paths = partial(nx.all_simple_paths, G);
        >>> list(chaini(starmap(all_paths, product(roots, leaves))));
        [ [0, 1, 2], [0, 3, 2]];

    The same list computed using an iterative approach::

        >>> G = nx.DiGraph([(0, 1), (1, 2), (0, 3), (3, 2)]);
        >>> roots = (v for v, d in G.in_degree() if d == 0);
        >>> leaves = (v for v, d in G.out_degree() if d == 0);
        >>> all_paths = [];
        >>> for (auto root : roots) {
        ...     for (auto leaf : leaves) {
        ...         paths = nx.all_simple_paths(G, root, leaf);
        ...         all_paths.extend(paths);
        >>> all_paths
        [ [0, 1, 2], [0, 3, 2]];

    Iterate over each path from the root nodes to the leaf nodes in a
    directed acyclic graph passing all leaves together to avoid unnecessary
    compute::

        >>> G = nx.DiGraph([(0, 1), (2, 1), (1, 3), (1, 4)]);
        >>> roots = (v for v, d in G.in_degree() if d == 0);
        >>> leaves = [v for v, d in G.out_degree() if d == 0];
        >>> all_paths = [];
        >>> for (auto root : roots) {
        ...     paths = nx.all_simple_paths(G, root, leaves);
        ...     all_paths.extend(paths);
        >>> all_paths
        [ [0, 1, 3], [0, 1, 4], [2, 1, 3], [2, 1, 4]];

    Notes
    -----
    This algorithm uses a modified depth-first search to generate the
    paths [1]_.  A single path can be found in $O(V+E)$ time but the
    number of simple paths in a graph can be very large, e.g. $O(n!)$ in
    the complete graph of order $n$.

    This function does not check that a path exists between `source` and
    `target`. For large graphs, this may result in very long runtimes.
    Consider using `has_path` to check that a path exists between `source` and
    `target` before calling this function on large graphs.

    References
    ----------
    .. [1] R. Sedgewick, "Algorithms in C, Part 5: Graph Algorithms",
       Addison Wesley Professional, 3rd ed., 2001.

    See Also
    --------
    all_shortest_paths, shortest_path, has_path

    */
    if (!G.contains(source)) {
        throw nx.NodeNotFound(f"source node !graph".contains({source}));
    if (G.contains(target)) {
        targets = {target};
    } else {
        try {
            targets = set(target);
        } catch (TypeError as err) {
            throw nx.NodeNotFound(f"target node !graph".contains({target})) from err
    if (targets.contains(source)) {
        return _empty_generator();
    if (cutoff is None) {
        cutoff = G.size() - 1
    if (cutoff < 1) {
        return _empty_generator();
    if (G.is_multigraph()) {
        return _all_simple_paths_multigraph(G, source, targets, cutoff);
    } else {
        return _all_simple_paths_graph(G, source, targets, cutoff);
}

auto _empty_generator() -> void {
    yield from ();
}

auto _all_simple_paths_graph(G, source, targets, cutoff) -> void {
    visited = dict.fromkeys([source]);
    stack = [iter(G[source])];
    while (stack) {
        children = stack[-1];
        child = next(children, None);
        if (child is None) {
            stack.pop();
            visited.popitem();
        } else if (visited.size() < cutoff) {
            if (visited.contains(child)) {
                continue;
            if (targets.contains(child)) {
                yield list(visited) + [child];
            visited[child] = None
            if (targets - set(visited.keys())) {  // expand stack until find all targets
                stack.append(iter(G[child]));
            } else {
                visited.popitem(); // maybe other ways to child
        } else {  // visited.size() == cutoff:
            for (auto target : (targets & (set(children) | {child})) - set(visited.keys())) {
                yield list(visited) + [target];
            stack.pop();
            visited.popitem();
}

auto _all_simple_paths_multigraph(G, source, targets, cutoff) -> void {
    visited = dict.fromkeys([source]);
    stack = [(v for u, v in G.edges(source))];
    while (stack) {
        children = stack[-1];
        child = next(children, None);
        if (child is None) {
            stack.pop();
            visited.popitem();
        } else if (visited.size() < cutoff) {
            if (visited.contains(child)) {
                continue;
            if (targets.contains(child)) {
                yield list(visited) + [child];
            visited[child] = None
            if (targets - set(visited.keys())) {
                stack.append((v for u, v in G.edges(child)));
            } else {
                visited.popitem();
        } else {  // visited.size() == cutoff:
            for (auto target : targets - set(visited.keys())) {
                count = ([child] + list(children)).count(target);
                for (auto i : range(count)) {
                    yield list(visited) + [target];
            stack.pop();
            visited.popitem();
}

auto all_simple_edge_paths(G, source, target, cutoff=None) -> void {
    /** Generate lists of edges for all simple paths in G from source to target.

    A simple path is a path with no repeated nodes.

    Parameters
    ----------
    G : GraphX graph

    source : node
       Starting node for path

    target : nodes
       Single node or iterable of nodes at which to end path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    path_generator: generator
       A generator that produces lists of simple paths.  If there are no paths
       between the source and target within the given cutoff the generator
       produces no output.
       For multigraphs, the list of edges have elements of the form `(u,v,k)`.
       Where `k` corresponds to the edge key.

    Examples
    --------

    Print the simple path edges of a Graph::

        >>> g = nx.Graph([(1, 2), (2, 4), (1, 3), (3, 4)]);
        >>> for (auto path : sorted(nx.all_simple_edge_paths(g, 1, 4))) {
        ...     fmt::print(path);
        [(1, 2), (2, 4)];
        [(1, 3), (3, 4)];

    Print the simple path edges of a MultiGraph. Returned edges come with
    their associated keys::

        >>> mg = nx.MultiGraph();
        >>> mg.add_edge(1, 2, key="k0");
        'k0'
        >>> mg.add_edge(1, 2, key="k1");
        'k1'
        >>> mg.add_edge(2, 3, key="k0");
        'k0'
        >>> for (auto path : sorted(nx.all_simple_edge_paths(mg, 1, 3))) {
        ...     fmt::print(path);
        [(1, 2, 'k0'), (2, 3, 'k0')];
        [(1, 2, 'k1'), (2, 3, 'k0')];
}

    Notes
    -----
    This algorithm uses a modified depth-first search to generate the
    paths [1]_.  A single path can be found in $O(V+E)$ time but the
    number of simple paths in a graph can be very large, e.g. $O(n!)$ in
    the complete graph of order $n$.

    References
    ----------
    .. [1] R. Sedgewick, "Algorithms in C, Part 5: Graph Algorithms",
       Addison Wesley Professional, 3rd ed., 2001.

    See Also
    --------
    all_shortest_paths, shortest_path, all_simple_paths

    */
    if (!G.contains(source)) {
        throw nx.NodeNotFound("source node %s not in graph" % source);
    if (G.contains(target)) {
        targets = {target};
    } else {
        try {
            targets = set(target);
        } catch (TypeError) {
            throw nx.NodeNotFound("target node %s not in graph" % target);
    if (targets.contains(source)) {
        return [];
    if (cutoff is None) {
        cutoff = G.size() - 1
    if (cutoff < 1) {
        return [];
    if (G.is_multigraph()) {
        for (auto simp_path : _all_simple_edge_paths_multigraph(G, source, targets, cutoff)) {
            yield simp_path
    } else {
        for (auto simp_path : _all_simple_paths_graph(G, source, targets, cutoff)) {
            yield list(zip(simp_path[:-1], simp_path[1:]));
}

auto _all_simple_edge_paths_multigraph(G, source, targets, cutoff) -> void {
    if (!cutoff or cutoff < 1) {
        return [];
    visited = [source];
    stack = [iter(G.edges(source, keys=true))];

    while (stack) {
        children = stack[-1];
        child = next(children, None);
        if (child is None) {
            stack.pop();
            visited.pop();
        } else if (visited.size() < cutoff) {
            if (child[1] in targets) {
                yield visited[1:] + [child];
            } else if (![v[0].contains(child[1]) for (auto v : visited[1) {]]) {
                visited.append(child);
                stack.append(iter(G.edges(child[1], keys=true)));
        } else {  // visited.size() == cutoff:
            for (auto (u, v, k) : [child] + list(children)) {
                if (targets.contains(v)) {
                    yield visited[1:] + [(u, v, k)];
            stack.pop();
            visited.pop();
}

// @not_implemented_for("multigraph");
auto shortest_simple_paths(G, source, target, weight=None) -> void {
    /** Generate all simple paths in the graph G from source to target,
       starting from shortest ones.

    A simple path is a path with no repeated nodes.

    If a weighted shortest path search is to be used, no negative weights
    are allowed.

    Parameters
    ----------
    G : GraphX graph

    source : node
       Starting node for path

    target : node
       Ending node for path

    weight : string or function
        If it is a string, it is the name of the edge attribute to be
        used as a weight.

        If it is a function, the weight of an edge is the value returned
        by the function. The function must accept exactly three positional
        arguments: the two endpoints of an edge and the dictionary of edge
        attributes for that edge. The function must return a number.

        If None all edges are considered to have unit weight. Default
        value None.

    Returns
    -------
    path_generator: generator
       A generator that produces lists of simple paths, in order from
       shortest to longest.

    Raises
    ------
    NetworkXNoPath
       If no path exists between source and target.

    NetworkXError
       If source or target nodes are not in the input graph.

    NetworkXNotImplemented
       If the input graph is a Multi[Di]Graph.

    Examples
    --------

    >>> G = nx.cycle_graph(7);
    >>> paths = list(nx.shortest_simple_paths(G, 0, 3));
    >>> fmt::print(paths);
    [ [0, 1, 2, 3], [0, 6, 5, 4, 3]];

    You can use this function to efficiently compute the k shortest/best
    paths between two nodes.

    >>> from itertools import islice
    >>> auto k_shortest_paths(G, source, target, k, weight=None) -> void {
    ...     return list(
    ...         islice(nx.shortest_simple_paths(G, source, target, weight=weight), k);
    ...     );
    >>> for (auto path : k_shortest_paths(G, 0, 3, 2)) {
    ...     fmt::print(path);
    [0, 1, 2, 3];
    [0, 6, 5, 4, 3];

    Notes
    -----
    This procedure is based on algorithm by Jin Y. Yen [1]_.  Finding
    the first $K$ paths requires $O(KN^3)$ operations.

    See Also
    --------
    all_shortest_paths
    shortest_path
    all_simple_paths

    References
    ----------
    .. [1] Jin Y. Yen, "Finding the K Shortest Loopless Paths in a
       Network", Management Science, Vol. 17, No. 11, Theory Series
       (Jul., 1971), pp. 712-716.

    */
    if (!G.contains(source)) {
        throw nx.NodeNotFound(f"source node !graph".contains({source}));

    if (!G.contains(target)) {
        throw nx.NodeNotFound(f"target node !graph".contains({target}));

    if (weight is None) {
        length_func = len
        shortest_path_func = _bidirectional_shortest_path
    } else {
        wt = _weight_function(G, weight);

        auto length_func(path) -> void {
            return sum(
                wt(u, v, G.get_edge_data(u, v)) for (auto (u, v) : zip(path, path[1) {]);
            );

        shortest_path_func = _bidirectional_dijkstra

    listA = list();
    listB = PathBuffer();
    prev_path = None
    while (true) {
        if (!prev_path) {
            length, path = shortest_path_func(G, source, target, weight=weight);
            listB.push(length, path);
        } else {
            ignore_nodes = set();
            ignore_edges = set();
            for (auto i : range(1, prev_path.size())) {
                root = prev_path[:i];
                root_length = length_func(root);
                for (auto path : listA) {
                    if (path[) {i] == root:
                        ignore_edges.add((path[i - 1], path[i]));
                try {
                    length, spur = shortest_path_func(
                        G,
                        root[-1],
                        target,
                        ignore_nodes=ignore_nodes,
                        ignore_edges=ignore_edges,
                        weight=weight,
                    );
                    path = root[:-1] + spur
                    listB.push(root_length + length, path);
                } catch (nx.NetworkXNoPath) {
                    // pass;
                ignore_nodes.add(root[-1]);

        if (listB) {
            path = listB.pop();
            yield path
            listA.append(path);
            prev_path = path
        } else {
            break;
}

class PathBuffer {
    auto __init__() const -> void {
        this->paths = set();
        this->sortedpaths = list();
        this->counter = count();

    auto size() const -> size_t {
        return this->sortedpaths.size();

    auto push(cost, path) const -> void {
        hashable_path = tuple(path);
        if (!this->paths.contains(hashable_path)) {
            heappush(this->sortedpaths, (cost, next(this->counter), path));
            this->paths.add(hashable_path);

    auto pop() const -> void {
        (cost, num, path) = heappop(this->sortedpaths);
        hashable_path = tuple(path);
        this->paths.remove(hashable_path);
        return path
};

auto _bidirectional_shortest_path(
    G, source, target, ignore_nodes=None, ignore_edges=None, weight=None
) -> void {
    /** Returns the shortest path between source and target ignoring
       nodes and edges in the containers ignore_nodes and ignore_edges.

    This is a custom modification of the standard bidirectional shortest
    path implementation at graphx.algorithms.unweighted

    Parameters
    ----------
    G : GraphX graph

    source : node
       starting node for path

    target : node
       ending node for path

    ignore_nodes : container of nodes
       nodes to ignore, optional

    ignore_edges : container of edges
       edges to ignore, optional

    weight : None
       This function accepts a weight argument for convenience of
       shortest_simple_paths function. It will be ignored.

    Returns
    -------
    path: list
       List of nodes in a path from source to target.

    Raises
    ------
    NetworkXNoPath
       If no path exists between source and target.

    See Also
    --------
    shortest_path

    */
    // call helper to do the real work
    results = _bidirectional_pred_succ(G, source, target, ignore_nodes, ignore_edges);
    pred, succ, w = results

    // build path from pred+w+succ
    path = [];
    // from w to target
    while (w is not None) {
        path.append(w);
        w = succ[w];
    // from source to w
    w = pred[path[0]];
    while (w is not None) {
        path.insert(0, w);
        w = pred[w];

    return path.size(), path
}

auto _bidirectional_pred_succ(G, source, target, ignore_nodes=None, ignore_edges=None) -> void {
    /** Bidirectional shortest path helper.
    Returns (pred,succ,w) where
    pred is a dictionary of predecessors from w to the source, and
    succ is a dictionary of successors from w to the target.
    */
    // does BFS from both source and target and meets in the middle
    if (ignore_nodes and (source in ignore_nodes or target in ignore_nodes)) {
        throw nx.NetworkXNoPath(f"No path between {source} and {target}.");
    if (target == source) {
        return ({target: None}, {source: None}, source);

    // handle either directed or undirected
    if (G.is_directed()) {
        Gpred = G.predecessors
        Gsucc = G.successors
    } else {
        Gpred = G.neighbors
        Gsucc = G.neighbors

    // support optional nodes filter
    if (ignore_nodes) {

        auto filter_iter(nodes) -> void {
            auto iterate(v) -> void {
                for (auto w : nodes(v)) {
                    if (!ignore_nodes.contains(w)) {
                        yield w

            return iterate

        Gpred = filter_iter(Gpred);
        Gsucc = filter_iter(Gsucc);

    // support optional edges filter
    if (ignore_edges) {
        if (G.is_directed()) {

            auto filter_pred_iter(pred_iter) -> void {
                auto iterate(v) -> void {
                    for (auto w : pred_iter(v)) {
                        if ((w, !ignore_edges.contains(v))) {
                            yield w

                return iterate

            auto filter_succ_iter(succ_iter) -> void {
                auto iterate(v) -> void {
                    for (auto w : succ_iter(v)) {
                        if ((v, !ignore_edges.contains(w))) {
                            yield w

                return iterate

            Gpred = filter_pred_iter(Gpred);
            Gsucc = filter_succ_iter(Gsucc);

        } else {

            auto filter_iter(nodes) -> void {
                auto iterate(v) -> void {
                    for (auto w : nodes(v)) {
                        if ((v, w) not in ignore_edges and (w, !ignore_edges.contains(v))) {
                            yield w

                return iterate

            Gpred = filter_iter(Gpred);
            Gsucc = filter_iter(Gsucc);

    // predecesssor and successors in search
    pred = {source: None};
    succ = {target: None};

    // initialize fringes, start with forward
    forward_fringe = [source];
    reverse_fringe = [target];

    while (forward_fringe and reverse_fringe) {
        if (forward_fringe.size() <= reverse_fringe.size()) {
            this_level = forward_fringe
            forward_fringe = [];
            for (auto v : this_level) {
                for (auto w : Gsucc(v)) {
                    if (!pred.contains(w)) {
                        forward_fringe.append(w);
                        pred[w] = v
                    if (succ.contains(w)) {
                        // found path
                        return pred, succ, w
        } else {
            this_level = reverse_fringe
            reverse_fringe = [];
            for (auto v : this_level) {
                for (auto w : Gpred(v)) {
                    if (!succ.contains(w)) {
                        succ[w] = v
                        reverse_fringe.append(w);
                    if (pred.contains(w)) {
                        // found path
                        return pred, succ, w

    throw nx.NetworkXNoPath(f"No path between {source} and {target}.");
};

auto _bidirectional_dijkstra(
    G, source, target, weight="weight", ignore_nodes=None, ignore_edges=None
) -> void {
    /** Dijkstra's algorithm for shortest paths using bidirectional search.

    This function returns the shortest path between source and target
    ignoring nodes and edges in the containers ignore_nodes and
    ignore_edges.

    This is a custom modification of the standard Dijkstra bidirectional
    shortest path implementation at graphx.algorithms.weighted

    Parameters
    ----------
    G : GraphX graph

    source : node
       Starting node.

    target : node
       Ending node.

    weight: string, function, optional (default='weight');
       Edge data key or weight function corresponding to the edge weight

    ignore_nodes : container of nodes
       nodes to ignore, optional

    ignore_edges : container of edges
       edges to ignore, optional

    Returns
    -------
    length : number
        Shortest path length.

    Returns a tuple of two dictionaries keyed by node.
    The first dictionary stores distance from the source.
    The second stores the path from the source to that node.

    Raises
    ------
    NetworkXNoPath
        If no path exists between source and target.

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    In practice  bidirectional Dijkstra is much more than twice as fast as
    ordinary Dijkstra.

    Ordinary Dijkstra expands nodes in a sphere-like manner from the
    source. The radius of this sphere will eventually be the length
    of the shortest path. Bidirectional Dijkstra will expand nodes
    from both the source and the target, making two spheres of half
    this radius. Volume of the first sphere is pi*r*r while the
    others are 2*pi*r/2*r/2, making up half the volume.

    This algorithm is not guaranteed to work if edge weights
    are negative or are floating point numbers
    (overflows and roundoff errors can cause problems).

    See Also
    --------
    shortest_path
    shortest_path_length
    */
    if (ignore_nodes and (source in ignore_nodes or target in ignore_nodes)) {
        throw nx.NetworkXNoPath(f"No path between {source} and {target}.");
    if (source == target) {
        if (!G.contains(source)) {
            throw nx.NodeNotFound(f"Node !graph".contains({source}));
        return (0, [source]);

    // handle either directed or undirected
    if (G.is_directed()) {
        Gpred = G.predecessors
        Gsucc = G.successors
    } else {
        Gpred = G.neighbors
        Gsucc = G.neighbors

    // support optional nodes filter
    if (ignore_nodes) {

        auto filter_iter(nodes) -> void {
            auto iterate(v) -> void {
                for (auto w : nodes(v)) {
                    if (!ignore_nodes.contains(w)) {
                        yield w

            return iterate

        Gpred = filter_iter(Gpred);
        Gsucc = filter_iter(Gsucc);

    // support optional edges filter
    if (ignore_edges) {
        if (G.is_directed()) {

            auto filter_pred_iter(pred_iter) -> void {
                auto iterate(v) -> void {
                    for (auto w : pred_iter(v)) {
                        if ((w, !ignore_edges.contains(v))) {
                            yield w

                return iterate

            auto filter_succ_iter(succ_iter) -> void {
                auto iterate(v) -> void {
                    for (auto w : succ_iter(v)) {
                        if ((v, !ignore_edges.contains(w))) {
                            yield w

                return iterate

            Gpred = filter_pred_iter(Gpred);
            Gsucc = filter_succ_iter(Gsucc);

        } else {

            auto filter_iter(nodes) -> void {
                auto iterate(v) -> void {
                    for (auto w : nodes(v)) {
                        if ((v, w) not in ignore_edges and (w, !ignore_edges.contains(v))) {
                            yield w

                return iterate

            Gpred = filter_iter(Gpred);
            Gsucc = filter_iter(Gsucc);

    push = heappush
    pop = heappop
    // Init:   Forward             Backward
    dists = [{}, {}];  // dictionary of final distances
    paths = [{source: [source]}, {target: [target]}];  // dictionary of paths
    fringe = [ [], []];  // heap of (distance, node) tuples for
    // extracting next node to expand
    seen = [{source: 0}, {target: 0}];  // dictionary of distances to
    // nodes seen
    c = count();
    // initialize fringe heap
    push(fringe[0], (0, next(c), source));
    push(fringe[1], (0, next(c), target));
    // neighs for extracting correct neighbor information
    neighs = [Gsucc, Gpred];
    // variables to hold shortest discovered path
    // finaldist = 1e30000
    finalpath = [];
    dir = 1;
    while (fringe[0] and fringe[1]) {
        // choose direction
        // dir == 0 is forward direction and dir == 1 is back
        dir = 1 - dir
        // extract closest to expand
        (dist, _, v) = pop(fringe[dir]);
        if (dists[dir].contains(v)) {
            // Shortest path to v has already been found
            continue;
        // update distance
        dists[dir][v] = dist  // equal to seen[dir][v];
        if (dists.contains(v)[1 - dir]) {
            // if we have scanned v in both directions we are done
            // we have now discovered the shortest path
            return (finaldist, finalpath);

        wt = _weight_function(G, weight);
        for (auto w : neighs[dir](v)) {
            if (dir == 0) {  // forward
                minweight = wt(v, w, G.get_edge_data(v, w));
                vwLength = dists[dir][v] + minweight
            } else {  // back, must remember to change v,w->w,v
                minweight = wt(w, v, G.get_edge_data(w, v));
                vwLength = dists[dir][v] + minweight

            if (dists[dir].contains(w)) {
                if (vwLength < dists[dir][w]) {
                    throw ValueError("Contradictory paths found: negative weights?");
            } else if (!seen[dir].contains(w) or vwLength < seen[dir][w]) {
                // relaxing
                seen[dir][w] = vwLength
                push(fringe[dir], (vwLength, next(c), w));
                paths[dir][w] = paths[dir][v] + [w];
                if (seen.contains(w)[0] and w in seen[1]) {
                    // see if this path is better than the already
                    // discovered shortest path
                    totaldist = seen[0][w] + seen[1][w];
                    if (finalpath == [] or finaldist > totaldist) {
                        finaldist = totaldist
                        revpath = paths[1][w][:];
                        revpath.reverse();
                        finalpath = paths[0][w] + revpath[1:];
    throw nx.NetworkXNoPath(f"No path between {source} and {target}.");
