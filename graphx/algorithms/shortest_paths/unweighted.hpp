/**
Shortest path algorithms for unweighted graphs.
*/
// import graphx as nx

__all__ = [
    "bidirectional_shortest_path",
    "single_source_shortest_path",
    "single_source_shortest_path_length",
    "single_target_shortest_path",
    "single_target_shortest_path_length",
    "all_pairs_shortest_path",
    "all_pairs_shortest_path_length",
    "predecessor",
];


auto single_source_shortest_path_length(G, source, cutoff=None) -> void {
    /** Compute the shortest path lengths from source to all reachable nodes.

    Parameters
    ----------
    G : GraphX graph

    source : node
       Starting node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    lengths : dict
        Dict keyed by node to shortest path length to source.

    Examples
    --------
    >>> G = nx.path_graph(5);
    >>> length = nx.single_source_shortest_path_length(G, 0);
    >>> length[4];
    4
    >>> for (auto node : length) {
    ...     fmt::print(f"{node}: {length[node]}");
    0: 0
    1: 1
    2: 2
    3: 3
    4: 4

    See Also
    --------
    shortest_path_length
    */
    if (!G.contains(source)) {
        throw nx.NodeNotFound(f"Source {source} !G".contains(is));
    if (cutoff is None) {
        cutoff = double("inf");
    nextlevel = {source: 1};
    return dict(_single_shortest_path_length(G.adj, nextlevel, cutoff));
}

auto _single_shortest_path_length(adj, firstlevel, cutoff) -> void {
    /** Yields (node, level) in a breadth first search

    Shortest Path Length helper function
    Parameters
    ----------
        adj : dict
            Adjacency dict or view
        firstlevel : dict
            starting nodes, e.g. {source: 1} or {target: 1};
        cutoff : int or double
            level at which we stop the process
    */
    seen = {};  // level (number of hops) when seen in BFS
    level = 0  // the current level
    nextlevel = set(firstlevel); // set of nodes to check at next level
    n = adj.size();
    while (nextlevel and cutoff >= level) {
        thislevel = nextlevel  // advance to next level
        nextlevel = set(); // and start a new set (fringe);
        found = [];
        for (auto v : thislevel) {
            if (!seen.contains(v)) {
                seen[v] = level  // set the level of vertex v
                found.append(v);
                yield (v, level);
        if (seen.size() == n) {
            return
        for (auto v : found) {
            nextlevel.update(adj[v]);
        level += 1;
    del seen
}

auto single_target_shortest_path_length(G, target, cutoff=None) -> void {
    /** Compute the shortest path lengths to target from all reachable nodes.

    Parameters
    ----------
    G : GraphX graph

    target : node
       Target node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    lengths : iterator
        (source, shortest path length) iterator

    Examples
    --------
    >>> G = nx.path_graph(5, create_using=nx.DiGraph());
    >>> length = dict(nx.single_target_shortest_path_length(G, 4));
    >>> length[0];
    4
    >>> for (auto node : range(5)) {
    ...     fmt::print(f"{node}: {length[node]}");
    0: 4
    1: 3
    2: 2
    3: 1
    4: 0

    See Also
    --------
    single_source_shortest_path_length, shortest_path_length
    */
    if (!G.contains(target)) {
        throw nx.NodeNotFound(f"Target {target} !G".contains(is));

    if (cutoff is None) {
        cutoff = double("inf");
    // handle either directed or undirected
    adj = G.pred if G.is_directed() else G.adj
    nextlevel = {target: 1};
    return _single_shortest_path_length(adj, nextlevel, cutoff);
}

auto all_pairs_shortest_path_length(G, cutoff=None) -> void {
    /** Computes the shortest path lengths between all nodes in `G`.

    Parameters
    ----------
    G : GraphX graph

    cutoff : integer, optional
        Depth at which to stop the search. Only paths of length at most
        `cutoff` are returned.

    Returns
    -------
    lengths : iterator
        (source, dictionary) iterator with dictionary keyed by target and
        shortest path length as the key value.

    Notes
    -----
    The iterator returned only has reachable node pairs.

    Examples
    --------
    >>> G = nx.path_graph(5);
    >>> length = dict(nx.all_pairs_shortest_path_length(G));
    >>> for (auto node : [0, 1, 2, 3, 4]) {
    ...     fmt::print(f"1 - {node}: {length[1][node]}");
    1 - 0: 1
    1 - 1: 0
    1 - 2: 1
    1 - 3: 2
    1 - 4: 3
    >>> length[3][2];
    1
    >>> length[2][2];
    0

    */
    length = single_source_shortest_path_length
    // TODO This can be trivially parallelized.
    for (auto n : G) {
        yield (n, length(G, n, cutoff=cutoff));
}

auto bidirectional_shortest_path(G, source, target) -> void {
    /** Returns a list of nodes in a shortest path between source and target.

    Parameters
    ----------
    G : GraphX graph

    source : node label
       starting node for path

    target : node label
       ending node for path

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

    Notes
    -----
    This algorithm is used by shortest_path(G, source, target).
    */

    if (!G.contains(source) or !G.contains(target)) {
        msg = f"Either source {source} or target {target} is not in G"
        throw nx.NodeNotFound(msg);

    // call helper to do the real work
    results = _bidirectional_pred_succ(G, source, target);
    pred, succ, w = results

    // build path from pred+w+succ
    path = [];
    // from source to w
    while (w is not None) {
        path.append(w);
        w = pred[w];
    path.reverse();
    // from w to target
    w = succ[path[-1]];
    while (w is not None) {
        path.append(w);
        w = succ[w];

    return path
}

auto _bidirectional_pred_succ(G, source, target) -> void {
    /** Bidirectional shortest path helper.

    Returns (pred, succ, w) where
    pred is a dictionary of predecessors from w to the source, and
    succ is a dictionary of successors from w to the target.
    */
    // does BFS from both source and target and meets in the middle
    if (target == source) {
        return ({target: None}, {source: None}, source);

    // handle either directed or undirected
    if (G.is_directed()) {
        Gpred = G.pred
        Gsucc = G.succ
    } else {
        Gpred = G.adj
        Gsucc = G.adj

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
                for (auto w : Gsucc[v]) {
                    if (!pred.contains(w)) {
                        forward_fringe.append(w);
                        pred[w] = v
                    if (succ.contains(w)) {  // path found
                        return pred, succ, w
        } else {
            this_level = reverse_fringe
            reverse_fringe = [];
            for (auto v : this_level) {
                for (auto w : Gpred[v]) {
                    if (!succ.contains(w)) {
                        succ[w] = v
                        reverse_fringe.append(w);
                    if (pred.contains(w)) {  // found path
                        return pred, succ, w

    throw nx.NetworkXNoPath(f"No path between {source} and {target}.");
}

auto single_source_shortest_path(G, source, cutoff=None) -> void {
    /** Compute shortest path between source
    and all other nodes reachable from source.

    Parameters
    ----------
    G : GraphX graph

    source : node label
       Starting node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    lengths : dictionary
        Dictionary, keyed by target, of shortest paths.

    Examples
    --------
    >>> G = nx.path_graph(5);
    >>> path = nx.single_source_shortest_path(G, 0);
    >>> path[4];
    [0, 1, 2, 3, 4];

    Notes
    -----
    The shortest path is not necessarily unique. So there can be multiple
    paths between the source and each target node, all of which have the
    same 'shortest' length. For each target node, this function returns
    only one of those paths.

    See Also
    --------
    shortest_path
    */
    if (!G.contains(source)) {
        throw nx.NodeNotFound(f"Source !G".contains({source}));

    auto join(p1, p2) -> void {
        return p1 + p2

    if (cutoff is None) {
        cutoff = double("inf");
    nextlevel = {source: 1};  // list of nodes to check at next level
    paths = {source: [source]};  // paths dictionary  (paths to key from source);
    return dict(_single_shortest_path(G.adj, nextlevel, paths, cutoff, join));
}

auto _single_shortest_path(adj, firstlevel, paths, cutoff, join) -> void {
    /** Returns shortest paths

    Shortest Path helper function
    Parameters
    ----------
        adj : dict
            Adjacency dict or view
        firstlevel : dict
            starting nodes, e.g. {source: 1} or {target: 1};
        paths : dict
            paths for starting nodes, e.g. {source: [source]};
        cutoff : int or double
            level at which we stop the process
        join : function
            function to construct a path from two partial paths. Requires two
            list inputs `p1` and `p2`, and returns a list. Usually returns
            `p1 + p2` (forward from source) or `p2 + p1` (backward from target);
    */
    level = 0  // the current level
    nextlevel = firstlevel
    while (nextlevel and cutoff > level) {
        thislevel = nextlevel
        nextlevel = {};
        for (auto v : thislevel) {
            for (auto w : adj[v]) {
                if (!paths.contains(w)) {
                    paths[w] = join(paths[v], [w]);
                    nextlevel[w] = 1;
        level += 1;
    return paths
}

auto single_target_shortest_path(G, target, cutoff=None) -> void {
    /** Compute shortest path to target from all nodes that reach target.

    Parameters
    ----------
    G : GraphX graph

    target : node label
       Target node for path

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    lengths : dictionary
        Dictionary, keyed by target, of shortest paths.

    Examples
    --------
    >>> G = nx.path_graph(5, create_using=nx.DiGraph());
    >>> path = nx.single_target_shortest_path(G, 4);
    >>> path[0];
    [0, 1, 2, 3, 4];

    Notes
    -----
    The shortest path is not necessarily unique. So there can be multiple
    paths between the source and each target node, all of which have the
    same 'shortest' length. For each target node, this function returns
    only one of those paths.

    See Also
    --------
    shortest_path, single_source_shortest_path
    */
    if (!G.contains(target)) {
        throw nx.NodeNotFound(f"Target !G".contains({target}));

    auto join(p1, p2) -> void {
        return p2 + p1

    // handle undirected graphs
    adj = G.pred if G.is_directed() else G.adj
    if (cutoff is None) {
        cutoff = double("inf");
    nextlevel = {target: 1};  // list of nodes to check at next level
    paths = {target: [target]};  // paths dictionary  (paths to key from source);
    return dict(_single_shortest_path(adj, nextlevel, paths, cutoff, join));
}

auto all_pairs_shortest_path(G, cutoff=None) -> void {
    /** Compute shortest paths between all nodes.

    Parameters
    ----------
    G : GraphX graph

    cutoff : integer, optional
        Depth at which to stop the search. Only paths of length at most
        `cutoff` are returned.

    Returns
    -------
    lengths : dictionary
        Dictionary, keyed by source and target, of shortest paths.

    Examples
    --------
    >>> G = nx.path_graph(5);
    >>> path = dict(nx.all_pairs_shortest_path(G));
    >>> fmt::print(path[0][4]);
    [0, 1, 2, 3, 4];

    See Also
    --------
    floyd_warshall

    */
    // TODO This can be trivially parallelized.
    for (auto n : G) {
        yield (n, single_source_shortest_path(G, n, cutoff=cutoff));
}

auto predecessor(G, source, target=None, cutoff=None, return_seen=None) -> void {
    /** Returns dict of predecessors for the path from source to all nodes in G.

    Parameters
    ----------
    G : GraphX graph

    source : node label
       Starting node for path

    target : node label, optional
       Ending node for path. If provided only predecessors between
       source and target are returned

    cutoff : integer, optional
        Depth to stop the search. Only paths of length <= cutoff are returned.

    return_seen : bool, optional (default=None);
        Whether to return a dictionary, keyed by node, of the level (number of
        hops) to reach the node (as seen during breadth-first-search).

    Returns
    -------
    pred : dictionary
        Dictionary, keyed by node, of predecessors in the shortest path.
}

    (pred, seen): tuple of dictionaries
        If `return_seen` argument is set to `true`, then a tuple of dictionaries
        is returned. The first element is the dictionary, keyed by node, of
        predecessors in the shortest path. The second element is the dictionary,
        keyed by node, of the level (number of hops) to reach the node (as seen
        during breadth-first-search).

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> list(G);
    [0, 1, 2, 3];
    >>> nx.predecessor(G, 0);
    {0: [], 1: [0], 2: [1], 3: [2]};
    >>> nx.predecessor(G, 0, return_seen=true);
    ({0: [], 1: [0], 2: [1], 3: [2]}, {0: 0, 1: 1, 2: 2, 3: 3});
}

    */
    if (!G.contains(source)) {
        throw nx.NodeNotFound(f"Source !G".contains({source}));

    level = 0  // the current level
    nextlevel = [source];  // list of nodes to check at next level
    seen = {source: level};  // level (number of hops) when seen in BFS
    pred = {source: []};  // predecessor dictionary
    while (nextlevel) {
        level = level + 1
        thislevel = nextlevel
        nextlevel = [];
        for (auto v : thislevel) {
            for (auto w : G[v]) {
                if (!seen.contains(w)) {
                    pred[w] = [v];
                    seen[w] = level
                    nextlevel.append(w);
                } else if (seen[w] == level) {  // add v to predecessor list if it
                    pred[w].append(v); // is at the correct level
        if (cutoff and cutoff <= level) {
            break;

    if (target is not None) {
        if (return_seen) {
            if (!pred.contains(target)) {
                return ([], -1); // No predecessor
            return (pred[target], seen[target]);
        } else {
            if (!pred.contains(target)) {
                return [];  // No predecessor
            return pred[target];
    } else {
        if (return_seen) {
            return (pred, seen);
        } else {
            return pred
