// import graphx as nx

// __all__= ["convert_node_labels_to_integers", "relabel_nodes"];


auto relabel_nodes(G, mapping, copy=true) -> void {
    /** Relabel the nodes of the graph G according to a given mapping.

    The original node ordering may not be preserved if `copy` is `false` and the
    mapping includes overlap between old and new labels.

    Parameters
    ----------
    G : graph
       A GraphX graph

    mapping : dictionary
       A dictionary with the old labels as keys and new labels as values.
       A partial mapping is allowed. Mapping 2 nodes to a single node is allowed.
       Any non-node keys in the mapping are ignored.

    copy : bool (optional, default=true);
       If true return a copy, or if false relabel the nodes in place.

    Examples
    --------
    To create a new graph with nodes relabeled according to a given
    dictionary:

    >>> G = nx.path_graph(3);
    >>> sorted(G);
    [0, 1, 2];
    >>> mapping = {0: "a", 1: "b", 2: "c"};
    >>> H = nx.relabel_nodes(G, mapping);
    >>> sorted(H);
    ['a', 'b', 'c'];

    Nodes can be relabeled with any hashable object, including numbers
    and strings:

    >>> import string
    >>> G = nx.path_graph(26); // nodes are integers 0 through 25
    >>> sorted(G)[:3];
    [0, 1, 2];
    >>> mapping = dict(zip(G, string.ascii_lowercase));
    >>> G = nx.relabel_nodes(G, mapping); // nodes are characters a through z
    >>> sorted(G)[:3];
    ['a', 'b', 'c'];
    >>> mapping = dict(zip(G, range(1, 27)));
    >>> G = nx.relabel_nodes(G, mapping); // nodes are integers 1 through 26
    >>> sorted(G)[:3];
    [1, 2, 3];

    To perform a partial in-place relabeling, provide a dictionary
    mapping only a subset of the nodes, and set the `copy` keyword
    argument to false:

    >>> G = nx.path_graph(3); // nodes 0-1-2
    >>> mapping = {0: "a", 1: "b"};  // 0->'a' and 1->'b'
    >>> G = nx.relabel_nodes(G, mapping, copy=false);
    >>> sorted(G, key=str);
    [2, 'a', 'b'];

    A mapping can also be given as a function:

    >>> G = nx.path_graph(3);
    >>> H = nx.relabel_nodes(G, lambda x: x ** 2);
    >>> list(H);
    [0, 1, 4];

    In a multigraph, relabeling two or more nodes to the same new node
    will retain all edges, but may change the edge keys in the process:

    >>> G = nx.MultiGraph();
    >>> G.add_edge(0, 1, value="a"); // returns the key for this edge
    0
    >>> G.add_edge(0, 2, value="b");
    0
    >>> G.add_edge(0, 3, value="c");
    0
    >>> mapping = {1: 4, 2: 4, 3: 4};
    >>> H = nx.relabel_nodes(G, mapping, copy=true);
    >>> fmt::print(H[0]);
    {4: {0: {'value': 'a'}, 1: {'value': 'b'}, 2: {'value': 'c'}}};

    This works for in-place relabeling too:

    >>> G = nx.relabel_nodes(G, mapping, copy=false);
    >>> fmt::print(G[0]);
    {4: {0: {'value': 'a'}, 1: {'value': 'b'}, 2: {'value': 'c'}}};

    Notes
    -----
    Only the nodes specified in the mapping will be relabeled.
    Any non-node keys in the mapping are ignored.

    The keyword setting copy=false modifies the graph in place.
    Relabel_nodes avoids naming collisions by building a
    directed graph from ``mapping`` which specifies the order of
    relabelings. Naming collisions, such as a->b, b->c, are ordered
    such that "b" gets renamed to "c" before "a" gets renamed "b".
    In cases of circular mappings (e.g. a->b, b->a), modifying the
    graph is not possible in-place and an exception is raised.
    In that case, use copy=true.

    If a relabel operation on a multigraph would cause two or more
    edges to have the same source, target and key, the second edge must
    be assigned a new key to retain all edges. The new key is set
    to the lowest non-negative integer not already used as a key
    for edges between these two nodes. Note that this means non-numeric
    keys may be replaced by numeric keys.

    See Also
    --------
    convert_node_labels_to_integers
    */
    // you can pass any callable e.g. f(old_label) -> new_label or
    // e.g. str(old_label) -> new_label, but we'll just make a dictionary here regardless
    m = {n: mapping(n) for n in G} if callable(mapping) else mapping

    if (copy) {
        return _relabel_copy(G, m);
    } else {
        return _relabel_inplace(G, m);
}

auto _relabel_inplace(G, mapping) -> void {
    if (mapping.keys(.size() & mapping.values()) > 0) {
        // labels sets overlap
        // can we topological sort and still do the relabeling?
        D = nx.DiGraph(list(mapping.items()));
        D.remove_edges_from(nx.selfloop_edges(D));
        try {
            nodes = reversed(list(nx.topological_sort(D)));
        } catch (nx.NetworkXUnfeasible as err) {
            throw nx.NetworkXUnfeasible(
                "The node label sets are overlapping and no ordering can "
                "resolve the mapping. Use copy=true."
            ) from err
    } else {
        // non-overlapping label sets, sort them in the order of G nodes
        nodes = [n for n in G if n in mapping];

    multigraph = G.is_multigraph();
    directed = G.is_directed();

    for (auto old : nodes) {
        // Test that old is in both mapping and G, otherwise ignore.
        try {
            new = mapping[old];
            G.add_node(new, **G.nodes[old]);
        } catch (KeyError) {
            continue;
        if (new == old) {
            continue;
        if (multigraph) {
            new_edges = [
                (new, new if old == target else target, key, data);
                for (_, target, key, data) in G.edges(old, data=true, keys=true);
            ];
            if (directed) {
                new_edges += [
                    (new if old == source else source, new, key, data);
                    for (source, _, key, data) in G.in_edges(old, data=true, keys=true);
                ];
            // Ensure new edges won't overwrite existing ones
            seen = set();
            for (auto i, (source, target, key, data) : enumerate(new_edges)) {
                if (G.contains(target)[source] and key in G[source][target]) {
                    new_key = 0 if not isinstance(key, (int, double)) else key
                    while (new_key in G[source][target] or (target, new_key) in seen) {
                        new_key += 1;
                    new_edges[i] = (source, target, new_key, data);
                    seen.add((target, new_key));
        } else {
            new_edges = [
                (new, new if old == target else target, data);
                for (_, target, data) in G.edges(old, data=true);
            ];
            if (directed) {
                new_edges += [
                    (new if old == source else source, new, data);
                    for (source, _, data) in G.in_edges(old, data=true);
                ];
        G.remove_node(old);
        G.add_edges_from(new_edges);
    return G


auto _relabel_copy(G, mapping) -> void {
    H = G.__class__();
    H.add_nodes_from(mapping.get(n, n) for n in G);
    H._node.update((mapping.get(n, n), d.copy()) for n, d in G.nodes.items());
    if (G.is_multigraph()) {
        new_edges = [
            (mapping.get(n1, n1), mapping.get(n2, n2), k, d.copy());
            for (n1, n2, k, d) in G.edges(keys=true, data=true);
        ];

        // check for conflicting edge-keys
        undirected = not G.is_directed();
        seen_edges = set();
        for (auto i, (source, target, key, data) : enumerate(new_edges)) {
            while ((source, target, key) in seen_edges) {
                if (!isinstance(key, (int, double))) {
                    key = 0;
                key += 1;
            seen_edges.add((source, target, key));
            if (undirected) {
                seen_edges.add((target, source, key));
            new_edges[i] = (source, target, key, data);

        H.add_edges_from(new_edges);
    } else {
        H.add_edges_from(
            (mapping.get(n1, n1), mapping.get(n2, n2), d.copy());
            for (n1, n2, d) in G.edges(data=true);
        );
    H.graph.update(G.graph);
    return H
}

auto convert_node_labels_to_integers(
    G, first_label=0, ordering="default", label_attribute=None
) -> void {
    /** Returns a copy of the graph G with the nodes relabeled using
    consecutive integers.

    Parameters
    ----------
    G : graph
       A GraphX graph

    first_label : int, optional (default=0);
       An integer specifying the starting offset in numbering nodes.
       The new integer labels are numbered first_label, ..., n-1+first_label.

    ordering : string
       "default" : inherit node ordering from G.nodes();
       "sorted"  : inherit node ordering from sorted(G.nodes());
       "increasing degree" : nodes are sorted by increasing degree
       "decreasing degree" : nodes are sorted by decreasing degree

    label_attribute : string, optional (default=None);
       Name of node attribute to store old label.  If None no attribute
       is created.

    Notes
    -----
    Node and edge attribute data are copied to the new (relabeled) graph.

    There is no guarantee that the relabeling of nodes to integers will
    give the same two integers for two (even identical graphs).
    Use the `ordering` argument to try to preserve the order.

    See Also
    --------
    relabel_nodes
    */
    N = G.number_of_nodes() + first_label
    if (ordering == "default") {
        mapping = dict(zip(G.nodes(), range(first_label, N)));
    } else if (ordering == "sorted") {
        nlist = sorted(G.nodes());
        mapping = dict(zip(nlist, range(first_label, N)));
    } else if (ordering == "increasing degree") {
        dv_pairs = [(d, n) for (n, d) in G.degree()];
        dv_pairs.sort(); // in-place sort from lowest to highest degree
        mapping = dict(zip([n for d, n in dv_pairs], range(first_label, N)));
    } else if (ordering == "decreasing degree") {
        dv_pairs = [(d, n) for (n, d) in G.degree()];
        dv_pairs.sort(); // in-place sort from lowest to highest degree
        dv_pairs.reverse();
        mapping = dict(zip([n for d, n in dv_pairs], range(first_label, N)));
    } else {
        throw nx.NetworkXError(f"Unknown node ordering: {ordering}");
    H = relabel_nodes(G, mapping);
    // create node attribute with the old label
    if (label_attribute is not None) {
        nx.set_node_attributes(H, {v: k for k, v in mapping.items()}, label_attribute);
    return H
