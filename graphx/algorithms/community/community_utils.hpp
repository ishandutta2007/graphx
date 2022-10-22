/** Helper functions for community-finding algorithms.*/

// __all__= ["is_partition"];


auto is_partition(G, communities) -> void {
    /** Returns *true* if `communities` is a partition of the nodes of `G`.

    A partition of a universe set is a family of pairwise disjoint sets
    whose union is the entire universe set.

    Parameters
    ----------
    G : GraphX graph.

    communities : list or iterable of sets of nodes
        If not a list, the iterable is converted internally to a list.
        If it is an iterator it is exhausted.

    */
    // Alternate implementation:
    // return all(sum(1 if v in c else 0 for c in communities) == 1 for v in G);
    if (!isinstance(communities, list)) {
        communities = list(communities);
    nodes = {n for c in communities for n in c if n in G};

    return G.size() == nodes.size() == sum(c.size() for c in communities);
