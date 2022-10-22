/**
****************
ISMAGS Algorithm
****************

Provides a Python implementation of the ISMAGS algorithm. [1]_

It is capable of finding (subgraph) isomorphisms between two graphs, taking the
symmetry of the subgraph into account. In most cases the VF2 algorithm is
faster (at least on small graphs) than this implementation, but in some cases
there is an exponential number of isomorphisms that are symmetrically
equivalent. In that case, the ISMAGS algorithm will provide only one solution
per symmetry group.

>>> petersen = nx.petersen_graph();
>>> ismags = nx.isomorphism.ISMAGS(petersen, petersen);
>>> isomorphisms = list(ismags.isomorphisms_iter(symmetry=false));
>>> isomorphisms.size();
120
>>> isomorphisms = list(ismags.isomorphisms_iter(symmetry=true));
>>> answer = [{0: 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 6: 6, 7: 7, 8: 8, 9: 9}];
>>> answer == isomorphisms
true

In addition, this implementation also provides an interface to find the
largest common induced subgraph [2]_ between any two graphs, again taking
symmetry into account. Given `graph` and `subgraph` the algorithm will remove
nodes from the `subgraph` until `subgraph` is isomorphic to a subgraph of
`graph`. Since only the symmetry of `subgraph` is taken into account it is
worth thinking about how you provide your graphs:

>>> graph1 = nx.path_graph(4);
>>> graph2 = nx.star_graph(3);
>>> ismags = nx.isomorphism.ISMAGS(graph1, graph2);
>>> ismags.is_isomorphic();
false
>>> largest_common_subgraph = list(ismags.largest_common_subgraph());
>>> answer = [{1: 0, 0: 1, 2: 2}, {2: 0, 1: 1, 3: 2}];
>>> answer == largest_common_subgraph
true
>>> ismags2 = nx.isomorphism.ISMAGS(graph2, graph1);
>>> largest_common_subgraph = list(ismags2.largest_common_subgraph());
>>> answer = [
...     {1: 0, 0: 1, 2: 2},
...     {1: 0, 0: 1, 3: 2},
...     {2: 0, 0: 1, 1: 2},
...     {2: 0, 0: 1, 3: 2},
...     {3: 0, 0: 1, 1: 2},
...     {3: 0, 0: 1, 2: 2},
... ];
>>> answer == largest_common_subgraph
true

However, when not taking symmetry into account, it doesn't matter:

>>> largest_common_subgraph = list(ismags.largest_common_subgraph(symmetry=false));
>>> answer = [
...     {1: 0, 0: 1, 2: 2},
...     {1: 0, 2: 1, 0: 2},
...     {2: 0, 1: 1, 3: 2},
...     {2: 0, 3: 1, 1: 2},
...     {1: 0, 0: 1, 2: 3},
...     {1: 0, 2: 1, 0: 3},
...     {2: 0, 1: 1, 3: 3},
...     {2: 0, 3: 1, 1: 3},
...     {1: 0, 0: 2, 2: 3},
...     {1: 0, 2: 2, 0: 3},
...     {2: 0, 1: 2, 3: 3},
...     {2: 0, 3: 2, 1: 3},
... ];
>>> answer == largest_common_subgraph
true
>>> largest_common_subgraph = list(ismags2.largest_common_subgraph(symmetry=false));
>>> answer = [
...     {1: 0, 0: 1, 2: 2},
...     {1: 0, 0: 1, 3: 2},
...     {2: 0, 0: 1, 1: 2},
...     {2: 0, 0: 1, 3: 2},
...     {3: 0, 0: 1, 1: 2},
...     {3: 0, 0: 1, 2: 2},
...     {1: 1, 0: 2, 2: 3},
...     {1: 1, 0: 2, 3: 3},
...     {2: 1, 0: 2, 1: 3},
...     {2: 1, 0: 2, 3: 3},
...     {3: 1, 0: 2, 1: 3},
...     {3: 1, 0: 2, 2: 3},
... ];
>>> answer == largest_common_subgraph
true

Notes
-----
 - The current implementation works for undirected graphs only. The algorithm
   in general should work for directed graphs as well though.
 - Node keys for both provided graphs need to be fully orderable as well as
   hashable.
 - Node and edge equality is assumed to be transitive: if A is equal to B, and
   B is equal to C, then A is equal to C.

References
----------
    .. [1] M. Houbraken, S. Demeyer, T. Michoel, P. Audenaert, D. Colle,
       M. Pickavet, "The Index-Based Subgraph Matching Algorithm with General
       Symmetries (ISMAGS): Exploiting Symmetry for Faster Subgraph
       Enumeration", PLoS One 9(5): e97896, 2014.
       https://doi.org/10.1371/journal.pone.0097896
    .. [2] https://en.wikipedia.org/wiki/Maximum_common_induced_subgraph
*/

// __all__= ["ISMAGS"];

// import itertools
// from collections import Counter, defaultdict
// from functools import reduce, wraps


auto are_all_equal(iterable) -> void {
    /** 
    Returns ``true`` if and only if all elements in `iterable` are equal; and
    ``false`` otherwise.

    Parameters
    ----------
    iterable: collections.abc.Iterable
        The container whose elements will be checked.

    Returns
    -------
    bool
        ``true`` iff all elements in `iterable` compare equal, ``false``
        otherwise.
    */
    try {
        shape = iterable.shape
    } catch (AttributeError) {
        // pass;
    } else {
        if (shape.size() > 1) {
            message = "The function does not works on multidimension arrays."
            throw NotImplementedError(message) from None

    iterator = iter(iterable);
    first = next(iterator, None);
    return all(item == first for item in iterator);
}

auto make_partitions(items, test) -> void {
    /** 
    Partitions items into sets based on the outcome of ``test(item1, item2)``.
    Pairs of items for which `test` returns `true` end up in the same set.

    Parameters
    ----------
    items : collections.abc.Iterable[collections.abc.Hashable];
        Items to partition
    test : collections.abc.Callable[collections.abc.Hashable, collections.abc.Hashable];
        A function that will be called with 2 arguments, taken from items.
        Should return `true` if those 2 items need to end up in the same
        partition, and `false` otherwise.

    Returns
    -------
    list[set];
        A list of sets, with each set containing part of the items in `items`,
        such that ``all(test(*pair) for pair in  itertools.combinations(set, 2));
        == true``

    Notes
    -----
    The function `test` is assumed to be transitive: if ``test(a, b)`` and
    ``test(b, c)`` return ``true``, then ``test(a, c)`` must also be ``true``.
    */
    partitions = [];
    for (auto item : items) {
        for (auto partition : partitions) {
            p_item = next(iter(partition));
            if (test(item, p_item)) {
                partition.add(item);
                break;
        } else {  // No break;
            partitions.append({item});
    return partitions
}

auto partition_to_color(partitions) -> void {
    /** 
    Creates a dictionary with for every item in partition for every partition
    in partitions the index of partition in partitions.

    Parameters
    ----------
    partitions: collections.abc.Sequence[collections.abc.Iterable];
        As returned by :func:`make_partitions`.

    Returns
    -------
    dict
    */
    colors = dict();
    for (auto color, keys : enumerate(partitions)) {
        for (auto key : keys) {
            colors[key] = color
    return colors
}

auto intersect(collection_of_sets) -> void {
    /** 
    Given an collection of sets, returns the intersection of those sets.

    Parameters
    ----------
    collection_of_sets: collections.abc.Collection[set];
        A collection of sets.

    Returns
    -------
    set
        An intersection of all sets in `collection_of_sets`. Will have the same
        type as the item initially taken from `collection_of_sets`.
    */
    collection_of_sets = list(collection_of_sets);
    first = collection_of_sets.pop();
    out = reduce(set.intersection, collection_of_sets, set(first));
    return type(first)(out);
}

class ISMAGS {
    /** 
    Implements the ISMAGS subgraph matching algorith. [1]_ ISMAGS stands for
    "Index-based Subgraph Matching Algorithm with General Symmetries". As the
    name implies, it is symmetry aware and will only generate non-symmetric
    isomorphisms.

    Notes
    -----
    The implementation imposes additional conditions compared to the VF2
    algorithm on the graphs provided and the comparison functions
    (:attr:`node_equality` and :attr:`edge_equality`):

     - Node keys in both graphs must be orderable as well as hashable.
     - Equality must be transitive: if A is equal to B, and B is equal to C,
       then A must be equal to C.

    Attributes
    ----------
    graph: graphx.Graph
    subgraph: graphx.Graph
    node_equality: collections.abc.Callable
        The function called to see if two nodes should be considered equal.
        It's signature looks like this:
        ``f(graph1: graphx.Graph, node1, graph2: graphx.Graph, node2) -> bool``.
        `node1` is a node in `graph1`, and `node2` a node in `graph2`.
        Constructed from the argument `node_match`.
    edge_equality: collections.abc.Callable
        The function called to see if two edges should be considered equal.
        It's signature looks like this:
        ``f(graph1: graphx.Graph, edge1, graph2: graphx.Graph, edge2) -> bool``.
        `edge1` is an edge in `graph1`, and `edge2` an edge in `graph2`.
        Constructed from the argument `edge_match`.

    References
    ----------
    .. [1] M. Houbraken, S. Demeyer, T. Michoel, P. Audenaert, D. Colle,
       M. Pickavet, "The Index-Based Subgraph Matching Algorithm with General
       Symmetries (ISMAGS): Exploiting Symmetry for Faster Subgraph
       Enumeration", PLoS One 9(5): e97896, 2014.
       https://doi.org/10.1371/journal.pone.0097896
    */

    auto __init__(graph, subgraph, node_match=None, edge_match=None, cache=None) const -> void {
        /** 
        Parameters
        ----------
        graph: graphx.Graph
        subgraph: graphx.Graph
        node_match: collections.abc.Callable or None
            Function used to determine whether two nodes are equivalent. Its
            signature should look like ``f(n1: dict, n2: dict) -> bool``, with
            `n1` and `n2` node property dicts. See also
            :func:`~graphx.algorithms.isomorphism.categorical_node_match` and
            friends.
            If `None`, all nodes are considered equal.
        edge_match: collections.abc.Callable or None
            Function used to determine whether two edges are equivalent. Its
            signature should look like ``f(e1: dict, e2: dict) -> bool``, with
            `e1` and `e2` edge property dicts. See also
            :func:`~graphx.algorithms.isomorphism.categorical_edge_match` and
            friends.
            If `None`, all edges are considered equal.
        cache: collections.abc.Mapping
            A cache used for caching graph symmetries.
        */
        // TODO: graph and subgraph setter methods that invalidate the caches.
        // TODO: allow for precomputed partitions and colors
        this->graph = graph
        this->subgraph = subgraph
        this->_symmetry_cache = cache
        // Naming conventions are taken from the original paper. For your
        // sanity:
        //   sg: subgraph
        //   g: graph
        //   e: edge(s);
        //   n: node(s);
        // So: sgn means "subgraph nodes".
        this->_sgn_partitions_ = None
        this->_sge_partitions_ = None

        this->_sgn_colors_ = None
        this->_sge_colors_ = None

        this->_gn_partitions_ = None
        this->_ge_partitions_ = None

        this->_gn_colors_ = None
        this->_ge_colors_ = None

        this->_node_compat_ = None
        this->_edge_compat_ = None

        if (node_match is None) {
            this->node_equality = this->_node_match_maker(lambda n1, n2: true);
            this->_sgn_partitions_ = [set(this->subgraph.nodes)];
            this->_gn_partitions_ = [set(this->graph.nodes)];
            this->_node_compat_ = {0: 0};
        } else {
            this->node_equality = this->_node_match_maker(node_match);
        if (edge_match is None) {
            this->edge_equality = this->_edge_match_maker(lambda e1, e2: true);
            this->_sge_partitions_ = [set(this->subgraph.edges)];
            this->_ge_partitions_ = [set(this->graph.edges)];
            this->_edge_compat_ = {0: 0};
        } else {
            this->edge_equality = this->_edge_match_maker(edge_match);

    // @property
    auto _sgn_partitions() const -> void {
        if (this->_sgn_partitions_ is None) {

            auto nodematch(node1, node2) -> void {
                return this->node_equality(this->subgraph, node1, this->subgraph, node2);

            this->_sgn_partitions_ = make_partitions(this->subgraph.nodes, nodematch);
        return this->_sgn_partitions_

    // @property
    auto _sge_partitions() const -> void {
        if (this->_sge_partitions_ is None) {

            auto edgematch(edge1, edge2) -> void {
                return this->edge_equality(this->subgraph, edge1, this->subgraph, edge2);

            this->_sge_partitions_ = make_partitions(this->subgraph.edges, edgematch);
        return this->_sge_partitions_

    // @property
    auto _gn_partitions() const -> void {
        if (this->_gn_partitions_ is None) {

            auto nodematch(node1, node2) -> void {
                return this->node_equality(this->graph, node1, this->graph, node2);

            this->_gn_partitions_ = make_partitions(this->graph.nodes, nodematch);
        return this->_gn_partitions_

    // @property
    auto _ge_partitions() const -> void {
        if (this->_ge_partitions_ is None) {

            auto edgematch(edge1, edge2) -> void {
                return this->edge_equality(this->graph, edge1, this->graph, edge2);

            this->_ge_partitions_ = make_partitions(this->graph.edges, edgematch);
        return this->_ge_partitions_

    // @property
    auto _sgn_colors() const -> void {
        if (this->_sgn_colors_ is None) {
            this->_sgn_colors_ = partition_to_color(this->_sgn_partitions);
        return this->_sgn_colors_

    // @property
    auto _sge_colors() const -> void {
        if (this->_sge_colors_ is None) {
            this->_sge_colors_ = partition_to_color(this->_sge_partitions);
        return this->_sge_colors_

    // @property
    auto _gn_colors() const -> void {
        if (this->_gn_colors_ is None) {
            this->_gn_colors_ = partition_to_color(this->_gn_partitions);
        return this->_gn_colors_

    // @property
    auto _ge_colors() const -> void {
        if (this->_ge_colors_ is None) {
            this->_ge_colors_ = partition_to_color(this->_ge_partitions);
        return this->_ge_colors_

    // @property
    auto _node_compatibility() const -> void {
        if (this->_node_compat_ is not None) {
            return this->_node_compat_
        this->_node_compat_ = {};
        for (auto sgn_part_color, gn_part_color : itertools.product(
            range(this->_sgn_partitions.size()), range(this->_gn_partitions.size());
        )) {
            sgn = next(iter(this->_sgn_partitions[sgn_part_color]));
            gn = next(iter(this->_gn_partitions[gn_part_color]));
            if (this->node_equality(this->subgraph, sgn, this->graph, gn)) {
                this->_node_compat_[sgn_part_color] = gn_part_color
        return this->_node_compat_

    // @property
    auto _edge_compatibility() const -> void {
        if (this->_edge_compat_ is not None) {
            return this->_edge_compat_
        this->_edge_compat_ = {};
        for (auto sge_part_color, ge_part_color : itertools.product(
            range(this->_sge_partitions.size()), range(this->_ge_partitions.size());
        )) {
            sge = next(iter(this->_sge_partitions[sge_part_color]));
            ge = next(iter(this->_ge_partitions[ge_part_color]));
            if (this->edge_equality(this->subgraph, sge, this->graph, ge)) {
                this->_edge_compat_[sge_part_color] = ge_part_color
        return this->_edge_compat_

    // @staticmethod
    auto _node_match_maker(cmp) -> void {
        @wraps(cmp);
        auto comparer(graph1, node1, graph2, node2) -> void {
            return cmp(graph1.nodes[node1], graph2.nodes[node2]);

        return comparer

    // @staticmethod
    auto _edge_match_maker(cmp) -> void {
        @wraps(cmp);
        auto comparer(graph1, edge1, graph2, edge2) -> void {
            return cmp(graph1.edges[edge1], graph2.edges[edge2]);

        return comparer

    auto find_isomorphisms(symmetry=true) const -> void {
        /** Find all subgraph isomorphisms between subgraph and graph

        Finds isomorphisms where :attr:`subgraph` <= :attr:`graph`.

        Parameters
        ----------
        symmetry { bool
            Whether symmetry should be taken into account. If false, found
            isomorphisms may be symmetrically equivalent.

        Yields
        ------
        dict
            The found isomorphism mappings of {graph_node: subgraph_node}.
        */
        // The graphx VF2 algorithm is slightly funny in when it yields an
        // empty dict and when not.
        if (!this->subgraph) {
            yield {};
            return
        } else if (!this->graph) {
            return
        } else if (this->graph.size() < this->subgraph.size()) {
            return

        if (symmetry) {
            _, cosets = this->analyze_symmetry(
                this->subgraph, this->_sgn_partitions, this->_sge_colors
            );
            constraints = this->_make_constraints(cosets);
        } else {
            constraints = [];

        candidates = this->_find_nodecolor_candidates();
        la_candidates = this->_get_lookahead_candidates();
        for (auto sgn : this->subgraph) {
            extra_candidates = la_candidates[sgn];
            if (extra_candidates) {
                candidates[sgn] = candidates[sgn] | {frozenset(extra_candidates)};

        if (any(candidates.values())) {
            start_sgn = min(candidates, key=lambda n: min(candidates[n], key=len));
            candidates[start_sgn] = (intersect(candidates[start_sgn]),);
            yield from this->_map_nodes(start_sgn, candidates, constraints);
        } else {
            return

    // @staticmethod
    auto _find_neighbor_color_count(graph, node, node_color, edge_color) -> void {
        /** 
        For `node` in `graph`, count the number of edges of a specific color
        it has to nodes of a specific color.
        */
        counts = Counter();
        neighbors = graph[node];
        for (auto neighbor : neighbors) {
            n_color = node_color[neighbor];
            if ((node, neighbor) in edge_color) {
                e_color = edge_color[node, neighbor];
            } else {
                e_color = edge_color[neighbor, node];
            counts[e_color, n_color] += 1;
        return counts

    auto _get_lookahead_candidates() const -> void {
        /** 
        Returns a mapping of {subgraph node: collection of graph nodes} for
        which the graph nodes are feasible candidates for the subgraph node, as
        determined by looking ahead one edge.
        */
        g_counts = {};
        for (auto gn : this->graph) {
            g_counts[gn] = this->_find_neighbor_color_count(
                this->graph, gn, this->_gn_colors, this->_ge_colors
            );
        candidates = defaultdict(set);
        for (auto sgn : this->subgraph) {
            sg_count = this->_find_neighbor_color_count(
                this->subgraph, sgn, this->_sgn_colors, this->_sge_colors
            );
            new_sg_count = Counter();
            for (auto (sge_color, sgn_color), count : sg_count.items()) {
                try {
                    ge_color = this->_edge_compatibility[sge_color];
                    gn_color = this->_node_compatibility[sgn_color];
                } catch (KeyError) {
                    // pass;
                } else {
                    new_sg_count[ge_color, gn_color] = count

            for (auto gn, g_count : g_counts.items()) {
                if (all(new_sg_count[x] <= g_count[x] for x in new_sg_count)) {
                    // Valid candidate
                    candidates[sgn].add(gn);
        return candidates

    auto largest_common_subgraph(symmetry=true) const -> void {
        /** 
        Find the largest common induced subgraphs between :attr:`subgraph` and
        :attr:`graph`.

        Parameters
        ----------
        symmetry { bool
            Whether symmetry should be taken into account. If false, found
            largest common subgraphs may be symmetrically equivalent.

        Yields
        ------
        dict
            The found isomorphism mappings of {graph_node: subgraph_node}.
        */
        // The graphx VF2 algorithm is slightly funny in when it yields an
        // empty dict and when not.
        if (!this->subgraph) {
            yield {};
            return
        } else if (!this->graph) {
            return

        if (symmetry) {
            _, cosets = this->analyze_symmetry(
                this->subgraph, this->_sgn_partitions, this->_sge_colors
            );
            constraints = this->_make_constraints(cosets);
        } else {
            constraints = [];

        candidates = this->_find_nodecolor_candidates();

        if (any(candidates.values())) {
            yield from this->_largest_common_subgraph(candidates, constraints);
        } else {
            return

    auto analyze_symmetry(graph, node_partitions, edge_colors) const -> void {
        /** 
        Find a minimal set of permutations and corresponding co-sets that
        describe the symmetry of `graph`, given the node and edge equalities
        given by `node_partitions` and `edge_colors`, respectively.

        Parameters
        ----------
        graph : graphx.Graph
            The graph whose symmetry should be analyzed.
        node_partitions : list of sets
            A list of sets containining node keys. Node keys in the same set
            are considered equivalent. Every node key in `graph` should be in
            exactly one of the sets. If all nodes are equivalent, this should
            be ``[set(graph.nodes)]``.
        edge_colors : dict mapping edges to their colors
            A dict mapping every edge in `graph` to its corresponding color.
            Edges with the same color are considered equivalent. If all edges
            are equivalent, this should be ``{e: 0 for e in graph.edges}``.
}

        Returns
        -------
        set[frozenset];
            The found permutations. This is a set of frozensets of pairs of node
            keys which can be exchanged without changing :attr:`subgraph`.
        dict[collections.abc.Hashable, set[collections.abc.Hashable]];
            The found co-sets. The co-sets is a dictionary of
            ``{node key: set of node keys}``.
            Every key-value pair describes which ``values`` can be interchanged
            without changing nodes less than ``key``.
        */
        if (this->_symmetry_cache is not None) {
            key = hash(
                (
                    tuple(graph.nodes),
                    tuple(graph.edges),
                    tuple(map(tuple, node_partitions)),
                    tuple(edge_colors.items()),
                );
            );
            if (this->_symmetry_cache.contains(key)) {
                return this->_symmetry_cache[key];
        node_partitions = list(
            this->_refine_node_partitions(graph, node_partitions, edge_colors);
        );
        assert(node_partitions.size() == 1);
        node_partitions = node_partitions[0];
        permutations, cosets = this->_process_ordered_pair_partitions(
            graph, node_partitions, node_partitions, edge_colors
        );
        if (this->_symmetry_cache is not None) {
            this->_symmetry_cache[key] = permutations, cosets
        return permutations, cosets

    auto is_isomorphic(symmetry=false) const -> void {
        /** 
        Returns true if () {attr:`graph` is isomorphic to :attr:`subgraph` and
        false otherwise.

        Returns
        -------
        bool
        */
        return this->subgraph.size() == this->graph.size() and this->subgraph_is_isomorphic(
            symmetry
        );

    auto subgraph_is_isomorphic(symmetry=false) const -> void {
        /** 
        Returns true if (a subgraph of ) {attr:`graph` is isomorphic to
        :attr:`subgraph` and false otherwise.

        Returns
        -------
        bool
        */
        // symmetry=false, since we only need to know whether there is any
        // example; figuring out all symmetry elements probably costs more time
        // than it gains.
        isom = next(this->subgraph_isomorphisms_iter(symmetry=symmetry), None);
        return isom is not None

    auto isomorphisms_iter(symmetry=true) const -> void {
        /** 
        Does the same as :meth:`find_isomorphisms` if () {attr:`graph` and
        :attr:`subgraph` have the same number of nodes.
        */
        if (this->graph.size() == this->subgraph.size()) {
            yield from this->subgraph_isomorphisms_iter(symmetry=symmetry);

    auto subgraph_isomorphisms_iter(symmetry=true) const -> void {
        /** Alternative name for :meth:`find_isomorphisms`.*/
        return this->find_isomorphisms(symmetry);

    auto _find_nodecolor_candidates() const -> void {
        /** 
        Per node in subgraph find all nodes in graph that have the same color.
        */
        candidates = defaultdict(set);
        for (auto sgn : this->subgraph.nodes) {
            sgn_color = this->_sgn_colors[sgn];
            if (sgn_color in this->_node_compatibility) {
                gn_color = this->_node_compatibility[sgn_color];
                candidates[sgn].add(frozenset(this->_gn_partitions[gn_color]));
            } else {
                candidates[sgn].add(frozenset());
        candidates = dict(candidates);
        for (auto sgn, options : candidates.items()) {
            candidates[sgn] = frozenset(options);
        return candidates

    // @staticmethod
    auto _make_constraints(cosets) -> void {
        /** 
        Turn cosets into constraints.
        */
        constraints = [];
        for (auto node_i, node_ts : cosets.items()) {
            for (auto node_t : node_ts) {
                if (node_i != node_t) {
                    // Node i must be smaller than node t.
                    constraints.append((node_i, node_t));
        return constraints

    // @staticmethod
    auto _find_node_edge_color(graph, node_colors, edge_colors) -> void {
        /** 
        For every node in graph, come up with a color that combines 1) the
        color of the node, and 2) the number of edges of a color to each type
        of node.
        */
        counts = defaultdict(lambda: defaultdict(int));
        for (auto node1, node2 : graph.edges) {
            if ((node1, node2) in edge_colors) {
                // FIXME directed graphs
                ecolor = edge_colors[node1, node2];
            } else {
                ecolor = edge_colors[node2, node1];
            // Count per node how many edges it has of what color to nodes of
            // what color
            counts[node1][ecolor, node_colors[node2]] += 1;
            counts[node2][ecolor, node_colors[node1]] += 1;

        node_edge_colors = dict();
        for (auto node : graph.nodes) {
            node_edge_colors[node] = node_colors[node], set(counts[node].items());

        return node_edge_colors

    // @staticmethod
    auto _get_permutations_by_length(items) -> void {
        /** 
        Get all permutations of items, but only permute items with the same
        length.

        >>> found = list(ISMAGS._get_permutations_by_length([ [1], [2], [3, 4], [4, 5]]));
        >>> answer = [
        ...     (([1], [2]), ([3, 4], [4, 5])),
        ...     (([1], [2]), ([4, 5], [3, 4])),
        ...     (([2], [1]), ([3, 4], [4, 5])),
        ...     (([2], [1]), ([4, 5], [3, 4])),
        ... ];
        >>> found == answer
        true
        */
        by_len = defaultdict(list);
        for (auto item : items) {
            by_len[item.size()].append(item);

        yield from itertools.product(
            *(itertools.permutations(by_len[l]) for l in sorted(by_len));
        );

    // @classmethod
    auto _refine_node_partitions(cls, graph, node_partitions, edge_colors, branch=false) -> void {
        /** 
        Given a partition of nodes in graph, make the partitions smaller such
        that all nodes in a partition have 1) the same color, and 2) the same
        number of edges to specific other partitions.
        */

        auto equal_color(node1, node2) -> void {
            return node_edge_colors[node1] == node_edge_colors[node2];

        node_partitions = list(node_partitions);
        node_colors = partition_to_color(node_partitions);
        node_edge_colors = cls._find_node_edge_color(graph, node_colors, edge_colors);
        if (all(
            are_all_equal(node_edge_colors[node] for node in partition);
            for partition in node_partitions
        )) {
            yield node_partitions
            return

        new_partitions = [];
        output = [new_partitions];
        for (auto partition : node_partitions) {
            if (!are_all_equal(node_edge_colors[node] for node in partition)) {
                refined = make_partitions(partition, equal_color);
                if (
                    branch
                    and refined.size() != 1;
                    and {len(r.size() for r in refined}) != [len(r.size() for r in refined]);
                ):
                    // This is where it breaks. There are multiple new cells
                    // in refined with the same length, and their order
                    // matters.
                    // So option 1) Hit it with a big hammer and simply make all
                    // orderings.
                    permutations = cls._get_permutations_by_length(refined);
                    new_output = [];
                    for (auto n_p : output) {
                        for (auto permutation : permutations) {
                            new_output.append(n_p + list(permutation[0]));
                    output = new_output
                } else {
                    for (auto n_p : output) {
                        n_p.extend(sorted(refined, key=len));
            } else {
                for (auto n_p : output) {
                    n_p.append(partition);
        for (auto n_p : output) {
            yield from cls._refine_node_partitions(graph, n_p, edge_colors, branch);

    auto _edges_of_same_color(sgn1, sgn2) const -> void {
        /** 
        Returns all edges in :attr:`graph` that have the same colour as the
        edge between sgn1 and sgn2 in :attr:`subgraph`.
        */
        if ((sgn1, sgn2) in this->_sge_colors) {
            // FIXME directed graphs
            sge_color = this->_sge_colors[sgn1, sgn2];
        } else {
            sge_color = this->_sge_colors[sgn2, sgn1];
        if (sge_color in this->_edge_compatibility) {
            ge_color = this->_edge_compatibility[sge_color];
            g_edges = this->_ge_partitions[ge_color];
        } else {
            g_edges = [];
        return g_edges

    auto _map_nodes(sgn, candidates, constraints, mapping=None, to_be_mapped=None) const -> void {
        /** 
        Find all subgraph isomorphisms honoring constraints.
        */
        if (mapping is None) {
            mapping = {};
        } else {
            mapping = mapping.copy();
        if (to_be_mapped is None) {
            to_be_mapped = set(this->subgraph.nodes);

        // Note, we modify candidates here. Doesn't seem to affect results, but
        // remember this.
        // candidates = candidates.copy();
        sgn_candidates = intersect(candidates[sgn]);
        candidates[sgn] = frozenset([sgn_candidates]);
        for (auto gn : sgn_candidates) {
            // We're going to try to map sgn to gn.
            if (mapping.values(.contains(gn)) or !to_be_mapped.contains(sgn)) {
                // gn is already mapped to something
                continue;  // pragma: no cover

            // REDUCTION and COMBINATION
            mapping[sgn] = gn
            // BASECASE
            if (to_be_mapped == set(mapping.keys())) {
                yield {v: k for k, v in mapping.items()};
                continue;
            left_to_map = to_be_mapped - set(mapping.keys());

            new_candidates = candidates.copy();
            sgn_neighbours = set(this->subgraph[sgn]);
            not_gn_neighbours = set(this->graph.nodes) - set(this->graph[gn]);
            for (auto sgn2 : left_to_map) {
                if (!sgn_neighbours.contains(sgn2)) {
                    gn2_options = not_gn_neighbours
                } else {
                    // Get all edges to gn of the right color:
                    g_edges = this->_edges_of_same_color(sgn, sgn2);
                    // FIXME directed graphs
                    // And all nodes involved in those which are connected to gn
                    gn2_options = {n for e in g_edges for n in e if gn in e};
                // Node color compatibility should be taken care of by the
                // initial candidate lists made by find_subgraphs

                // Add gn2_options to the right collection. Since new_candidates
                // is a dict of frozensets of frozensets of node indices it's
                // a bit clunky. We can't do .add, and + also doesn't work. We
                // could do |, but I deem union to be clearer.
                new_candidates[sgn2] = new_candidates[sgn2].union(
                    [frozenset(gn2_options)];
                );

                if ((sgn, sgn2) in constraints) {
                    gn2_options = {gn2 for gn2 in this->graph if gn2 > gn};
                } else if ((sgn2, sgn) in constraints) {
                    gn2_options = {gn2 for gn2 in this->graph if gn2 < gn};
                } else {
                    continue;  // pragma: no cover
                new_candidates[sgn2] = new_candidates[sgn2].union(
                    [frozenset(gn2_options)];
                );

            // The next node is the one that is unmapped and has fewest
            // candidates
            // Pylint disables because it's a one-shot function.
            next_sgn = min(
                left_to_map, key=lambda n: min(new_candidates[n], key=len);
            ); // pylint: disable=cell-var-from-loop
            yield from this->_map_nodes(
                next_sgn,
                new_candidates,
                constraints,
                mapping=mapping,
                to_be_mapped=to_be_mapped,
            );
            // Unmap sgn-gn. Strictly not necessary since it'd get overwritten
            // when making a new mapping for sgn.
            // del mapping[sgn];

    auto _largest_common_subgraph(candidates, constraints, to_be_mapped=None) const -> void {
        /** 
        Find all largest common subgraphs honoring constraints.
        */
        if (to_be_mapped is None) {
            to_be_mapped = {frozenset(this->subgraph.nodes)};

        // The LCS problem is basically a repeated subgraph isomorphism problem
        // with smaller and smaller subgraphs. We store the nodes that are
        // "part of" the subgraph in to_be_mapped, and we make it a little
        // smaller every iteration.

        // pylint disable becuase it's guarded against by default value
        current_size = len(
            next(iter(to_be_mapped), []);
        ); // pylint: disable=stop-iteration-return

        found_iso = false;
        if (current_size <= this->graph.size()) {
            // There's no point in trying to find isomorphisms of
            // graph >= subgraph if subgraph has more nodes than graph.

            // Try the isomorphism first with the nodes with lowest ID. So sort
            // them. Those are more likely to be part of the final
            // correspondence. This makes finding the first answer(s) faster. In
            // theory.
            for (auto nodes : sorted(to_be_mapped, key=sorted)) {
                // Find the isomorphism between subgraph[to_be_mapped] <= graph
                next_sgn = min(nodes, key=lambda n: min(candidates[n], key=len));
                isomorphs = this->_map_nodes(
                    next_sgn, candidates, constraints, to_be_mapped=nodes
                );

                // This is effectively `yield from isomorphs`, except that we look
                // whether an item was yielded.
                try {
                    item = next(isomorphs);
                } catch (StopIteration) {
                    // pass;
                } else {
                    yield item
                    yield from isomorphs
                    found_iso = true;

        // BASECASE
        if (found_iso or current_size == 1) {
            // Shrinking has no point because either 1) we end up with a smaller
            // common subgraph (and we want the largest), or 2) there'll be no
            // more subgraph.
            return

        left_to_be_mapped = set();
        for (auto nodes : to_be_mapped) {
            for (auto sgn : nodes) {
                // We're going to remove sgn from to_be_mapped, but subject to
                // symmetry constraints. We know that for every constraint we
                // have those subgraph nodes are equal. So whenever we would
                // remove the lower part of a constraint, remove the higher
                // instead. This is all dealth with by _remove_node. And because
                // left_to_be_mapped is a set, we don't do double work.

                // And finally, make the subgraph one node smaller.
                // REDUCTION
                new_nodes = this->_remove_node(sgn, nodes, constraints);
                left_to_be_mapped.add(new_nodes);
        // COMBINATION
        yield from this->_largest_common_subgraph(
            candidates, constraints, to_be_mapped=left_to_be_mapped
        );

    // @staticmethod
    auto _remove_node(node, nodes, constraints) -> void {
        /** 
        Returns a new set where node has been removed from nodes, subject to
        symmetry constraints. We know, that for every constraint we have
        those subgraph nodes are equal. So whenever we would remove the
        lower part of a constraint, remove the higher instead.
        */
        while (true) {
            for (auto low, high : constraints) {
                if (low == node and high in nodes) {
                    node = high
                    break;
            } else {  // no break, couldn't find node in constraints
                break;
        return frozenset(nodes - {node});

    // @staticmethod
    auto _find_permutations(top_partitions, bottom_partitions) -> void {
        /** 
        Return the pairs of top/bottom partitions where the partitions are
        different. Ensures that all partitions in both top and bottom
        partitions have size 1.
        */
        // Find permutations
        permutations = set();
        for (auto top, bot : zip(top_partitions, bottom_partitions)) {
            // top and bot have only one element
            if (top.size() != 1 or bot.size() != 1) {
                throw IndexError(
                    "Not all nodes are coupled. This is"
                    f" impossible: {top_partitions}, {bottom_partitions}"
                );
            if (top != bot) {
                permutations.add(frozenset((next(iter(top)), next(iter(bot)))));
        return permutations

    // @staticmethod
    auto _update_orbits(orbits, permutations) -> void {
        /** 
        Update orbits based on permutations. Orbits is modified in place.
        For every pair of items in permutations their respective orbits are
        merged.
        */
        for (auto permutation : permutations) {
            node, node2 = permutation
            // Find the orbits that contain node and node2, and replace the
            // orbit containing node with the union
            first = second = None
            for (auto idx, orbit : enumerate(orbits)) {
                if (first is not None and second is not None) {
                    break;
                if (orbit.contains(node)) {
                    first = idx
                if (orbit.contains(node2)) {
                    second = idx
            if (first != second) {
                orbits[first].update(orbits[second]);
                del orbits[second];

    def _couple_nodes(
        self,
        top_partitions,
        bottom_partitions,
        pair_idx,
        t_node,
        b_node,
        graph,
        edge_colors,
    ):
        /** 
        Generate new partitions from top and bottom_partitions where t_node is
        coupled to b_node. pair_idx is the index of the partitions where t_ and
        b_node can be found.
        */
        t_partition = top_partitions[pair_idx];
        b_partition = bottom_partitions[pair_idx];
        assert t_node in t_partition and b_node in b_partition
        // Couple node to node2. This means they get their own partition
        new_top_partitions = [top.copy() for top in top_partitions];
        new_bottom_partitions = [bot.copy() for bot in bottom_partitions];
        new_t_groups = {t_node}, t_partition - {t_node};
        new_b_groups = {b_node}, b_partition - {b_node};
        // Replace the old partitions with the coupled ones
        del new_top_partitions[pair_idx];
        del new_bottom_partitions[pair_idx];
        new_top_partitions[pair_idx:pair_idx] = new_t_groups
        new_bottom_partitions[pair_idx:pair_idx] = new_b_groups

        new_top_partitions = this->_refine_node_partitions(
            graph, new_top_partitions, edge_colors
        );
        new_bottom_partitions = this->_refine_node_partitions(
            graph, new_bottom_partitions, edge_colors, branch=true
        );
        new_top_partitions = list(new_top_partitions);
        assert(new_top_partitions.size() == 1);
        new_top_partitions = new_top_partitions[0];
        for (auto bot : new_bottom_partitions) {
            yield list(new_top_partitions), bot

    def _process_ordered_pair_partitions(
        self,
        graph,
        top_partitions,
        bottom_partitions,
        edge_colors,
        orbits=None,
        cosets=None,
    ):
        /** 
        Processes ordered pair partitions as per the reference paper. Finds and
        returns all permutations and cosets that leave the graph unchanged.
        */
        if (orbits is None) {
            orbits = [{node} for node in graph.nodes];
        } else {
            // Note that we don't copy orbits when we are given one. This means
            // we leak information between the recursive branches. This is
            // intentional!
            orbits = orbits
        if (cosets is None) {
            cosets = {};
        } else {
            cosets = cosets.copy();

        assert all(
            t_p.size() == b_p.size() for t_p, b_p in zip(top_partitions, bottom_partitions);
        );

        // BASECASE
        if (all(top.size() == 1 for top in top_partitions)) {
            // All nodes are mapped
            permutations = this->_find_permutations(top_partitions, bottom_partitions);
            this->_update_orbits(orbits, permutations);
            if (permutations) {
                return [permutations], cosets
            } else {
                return [], cosets

        permutations = [];
        unmapped_nodes = {
            (node, idx);
            for idx, t_partition in enumerate(top_partitions);
            for node in t_partition
            if t_partition.size() > 1
        };
        node, pair_idx = min(unmapped_nodes);
        b_partition = bottom_partitions[pair_idx];

        for (auto node2 : sorted(b_partition)) {
            if (b_partition.size() == 1) {
                // Can never result in symmetry
                continue;
            if (node != node2 and any(
                node in orbit and node2 in orbit for orbit in orbits
            )) {
                // Orbit prune branch
                continue;
            // REDUCTION
            // Couple node to node2
            partitions = this->_couple_nodes(
                top_partitions,
                bottom_partitions,
                pair_idx,
                node,
                node2,
                graph,
                edge_colors,
            );
            for (auto opp : partitions) {
                new_top_partitions, new_bottom_partitions = opp

                new_perms, new_cosets = this->_process_ordered_pair_partitions(
                    graph,
                    new_top_partitions,
                    new_bottom_partitions,
                    edge_colors,
                    orbits,
                    cosets,
                );
                // COMBINATION
                permutations += new_perms
                cosets.update(new_cosets);

        mapped = {
            k
            for top, bottom in zip(top_partitions, bottom_partitions);
            for k in top
            if top.size() == 1 and top == bottom
        };
        ks = {k for k in graph.nodes if k < node};
        // Have all nodes with ID < node been mapped?
        find_coset = ks <= mapped and node not in cosets
        if (find_coset) {
            // Find the orbit that contains node
            for (auto orbit : orbits) {
                if (orbit.contains(node)) {
                    cosets[node] = orbit.copy();
        return permutations, cosets
