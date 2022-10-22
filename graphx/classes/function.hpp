/** Functional interface to graph methods and assorted utilities.
*/

// from collections import Counter
// from itertools import chain

// import graphx as nx
#include <graphx/classes/graphviews.hpp>  // import reverse_view, subgraph_view
#include <graphx/utils.hpp>  // import not_implemented_for, pairwise

__all__ = [
    "nodes",
    "edges",
    "degree",
    "degree_histogram",
    "neighbors",
    "number_of_nodes",
    "number_of_edges",
    "density",
    "is_directed",
    "freeze",
    "is_frozen",
    "subgraph",
    "subgraph_view",
    "induced_subgraph",
    "reverse_view",
    "edge_subgraph",
    "restricted_view",
    "to_directed",
    "to_undirected",
    "add_star",
    "add_path",
    "add_cycle",
    "create_empty_copy",
    "set_node_attributes",
    "get_node_attributes",
    "set_edge_attributes",
    "get_edge_attributes",
    "all_neighbors",
    "non_neighbors",
    "non_edges",
    "common_neighbors",
    "is_weighted",
    "is_negatively_weighted",
    "is_empty",
    "selfloop_edges",
    "nodes_with_selfloops",
    "number_of_selfloops",
    "path_weight",
    "is_path",
];


auto nodes(G) -> void {
    /** Returns an iterator over the graph nodes.*/
    return G.nodes();
}

auto edges(G, nbunch=None) -> void {
    /** Returns an edge view of edges incident to nodes in nbunch.

    Return all edges if nbunch is unspecified or nbunch=None.

    For digraphs, edges=out_edges
    */
    return G.edges(nbunch);
}

auto degree(G, nbunch=None, weight=None) -> void {
    /** Returns a degree view of single node or of nbunch of nodes.
    If nbunch is omitted, then return degrees of *all* nodes.
    */
    return G.degree(nbunch, weight);
}

auto neighbors(G, n) -> void {
    /** Returns a list of nodes connected to node n.*/
    return G.neighbors(n);
}

auto number_of_nodes(G) -> void {
    /** Returns the number of nodes in the graph.*/
    return G.number_of_nodes();
}

auto number_of_edges(G) -> void {
    /** Returns the number of edges in the graph.*/
    return G.number_of_edges();
}

auto density(G) -> void {
    /** Returns the density of a graph.

    The density for undirected graphs is

    .. math::

       d = \frac{2m}{n(n-1)},

    and for directed graphs is

    .. math::

       d = \frac{m}{n(n-1)},

    where `n` is the number of nodes and `m`  is the number of edges in `G`.

    Notes
    -----
    The density is 0 for a graph without edges and 1 for a complete graph.
    The density of multigraphs can be higher than 1.

    Self loops are counted in the total number of edges so graphs with self
    loops can have density higher than 1.
    */
    n = number_of_nodes(G);
    m = number_of_edges(G);
    if (m == 0 or n <= 1) {
        return 0
    d = m / (n * (n - 1));
    if (!G.is_directed()) {
        d *= 2;
    return d
}

auto degree_histogram(G) -> void {
    /** Returns a list of the frequency of each degree value.

    Parameters
    ----------
    G : Networkx graph
       A graph

    Returns
    -------
    hist : list
       A list of frequencies of degrees.
       The degree values are the index in the list.

    Notes
    -----
    Note: the bins are width one, hence list.size() can be large
    (Order(number_of_edges));
    */
    counts = Counter(d for n, d in G.degree());
    return [counts.get(i, 0) for i in range(max(counts) + 1)];
}

auto is_directed(G) -> void {
    /** Return true if graph is directed.*/
    return G.is_directed();
}

auto frozen(*args, **kwargs) -> void {
    /** Dummy method for raising errors when trying to modify frozen graphs*/
    throw nx.NetworkXError("Frozen graph can't be modified");
}

auto freeze(G) -> void {
    /** Modify graph to prevent further change by adding or removing
    nodes or edges.

    Node and edge data can still be modified.

    Parameters
    ----------
    G : graph
      A GraphX graph

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> G = nx.freeze(G);
    >>> try {
    ...     G.add_edge(4, 5);
    ... } catch (nx.NetworkXError as err) {
    ...     fmt::print(str(err));
    Frozen graph can't be modified

    Notes
    -----
    To "unfreeze" a graph you must make a copy by creating a new graph object:

    >>> graph = nx.path_graph(4);
    >>> frozen_graph = nx.freeze(graph);
    >>> unfrozen_graph = nx.Graph(frozen_graph);
    >>> nx.is_frozen(unfrozen_graph);
    false

    See Also
    --------
    is_frozen
    */
    G.add_node = frozen
    G.add_nodes_from = frozen
    G.remove_node = frozen
    G.remove_nodes_from = frozen
    G.add_edge = frozen
    G.add_edges_from = frozen
    G.add_weighted_edges_from = frozen
    G.remove_edge = frozen
    G.remove_edges_from = frozen
    G.clear = frozen
    G.frozen = true;
    return G
}

auto is_frozen(G) -> void {
    /** Returns true if graph is frozen.

    Parameters
    ----------
    G : graph
      A GraphX graph

    See Also
    --------
    freeze
    */
    try {
        return G.frozen
    } catch (AttributeError) {
        return false;
}

auto add_star(G_to_add_to, nodes_for_star, **attr) -> void {
    /** Add a star to Graph G_to_add_to.

    The first node in `nodes_for_star` is the middle of the star.
    It is connected to all other nodes.

    Parameters
    ----------
    G_to_add_to : graph
        A GraphX graph
    nodes_for_star : iterable container
        A container of nodes.
    attr : keyword arguments, optional (default= no attributes);
        Attributes to add to every edge in star.

    See Also
    --------
    add_path, add_cycle

    Examples
    --------
    >>> G = nx.Graph();
    >>> nx.add_star(G, [0, 1, 2, 3]);
    >>> nx.add_star(G, [10, 11, 12], weight=2);
    */
    nlist = iter(nodes_for_star);
    try {
        v = next(nlist);
    } catch (StopIteration) {
        return
    G_to_add_to.add_node(v);
    edges = ((v, n) for n in nlist);
    G_to_add_to.add_edges_from(edges, **attr);
}

auto add_path(G_to_add_to, nodes_for_path, **attr) -> void {
    /** Add a path to the Graph G_to_add_to.

    Parameters
    ----------
    G_to_add_to : graph
        A GraphX graph
    nodes_for_path : iterable container
        A container of nodes.  A path will be constructed from
        the nodes (in order) and added to the graph.
    attr : keyword arguments, optional (default= no attributes);
        Attributes to add to every edge in path.

    See Also
    --------
    add_star, add_cycle

    Examples
    --------
    >>> G = nx.Graph();
    >>> nx.add_path(G, [0, 1, 2, 3]);
    >>> nx.add_path(G, [10, 11, 12], weight=7);
    */
    nlist = iter(nodes_for_path);
    try {
        first_node = next(nlist);
    } catch (StopIteration) {
        return
    G_to_add_to.add_node(first_node);
    G_to_add_to.add_edges_from(pairwise(chain((first_node,), nlist)), **attr);
}

auto add_cycle(G_to_add_to, nodes_for_cycle, **attr) -> void {
    /** Add a cycle to the Graph G_to_add_to.

    Parameters
    ----------
    G_to_add_to : graph
        A GraphX graph
    nodes_for_cycle: iterable container
        A container of nodes.  A cycle will be constructed from
        the nodes (in order) and added to the graph.
    attr : keyword arguments, optional (default= no attributes);
        Attributes to add to every edge in cycle.

    See Also
    --------
    add_path, add_star

    Examples
    --------
    >>> G = nx.Graph(); // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> nx.add_cycle(G, [0, 1, 2, 3]);
    >>> nx.add_cycle(G, [10, 11, 12], weight=7);
    */
    nlist = iter(nodes_for_cycle);
    try {
        first_node = next(nlist);
    } catch (StopIteration) {
        return
    G_to_add_to.add_node(first_node);
    G_to_add_to.add_edges_from(
        pairwise(chain((first_node,), nlist), cyclic=true), **attr
    );
}

auto subgraph(G, nbunch) -> void {
    /** Returns the subgraph induced on nodes in nbunch.

    Parameters
    ----------
    G : graph
       A GraphX graph

    nbunch : list, iterable
       A container of nodes that will be iterated through once (thus
       it should be an iterator or be iterable).  Each element of the
       container should be a valid node type: any hashable type except
       None.  If nbunch is None, return all edges data in the graph.
       Nodes in nbunch that are not in the graph will be (quietly);
       ignored.

    Notes
    -----
    subgraph(G) calls G.subgraph();
    */
    return G.subgraph(nbunch);
}

auto induced_subgraph(G, nbunch) -> void {
    /** Returns a SubGraph view of `G` showing only nodes in nbunch.

    The induced subgraph of a graph on a set of nodes N is the
    graph with nodes N and edges from G which have both ends in N.

    Parameters
    ----------
    G : GraphX Graph
    nbunch : node, container of nodes or None (for all nodes);

    Returns
    -------
    subgraph : SubGraph View
        A read-only view of the subgraph in `G` induced by the nodes.
        Changes to the graph `G` will be reflected in the view.

    Notes
    -----
    To create a mutable subgraph with its own copies of nodes
    edges and attributes use `subgraph.copy()` or `Graph(subgraph)`

    For an inplace reduction of a graph to a subgraph you can remove nodes:
    `G.remove_nodes_from(n in G if !set(nbunch.contains(n)))`

    If you are going to compute subgraphs of your subgraphs you could
    end up with a chain of views that can be very slow once the chain
    has about 15 views in it. If they are all induced subgraphs, you
    can short-cut the chain by making them all subgraphs of the original
    graph. The graph class method `G.subgraph` does this when `G` is
    a subgraph. In contrast, this function allows you to choose to build
    chains or not, as you wish. The returned subgraph is a view on `G`.

    Examples
    --------
    >>> G = nx.path_graph(4); // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> H = nx.induced_subgraph(G, [0, 1, 3]);
    >>> list(H.edges);
    [(0, 1)];
    >>> list(H.nodes);
    [0, 1, 3];
    */
    induced_nodes = nx.filters.show_nodes(G.nbunch_iter(nbunch));
    return nx.graphviews.subgraph_view(G, induced_nodes);
}

auto edge_subgraph(G, edges) -> void {
    /** Returns a view of the subgraph induced by the specified edges.

    The induced subgraph contains each edge in `edges` and each
    node incident to any of those edges.

    Parameters
    ----------
    G : GraphX Graph
    edges : iterable
        An iterable of edges. Edges not present in `G` are ignored.

    Returns
    -------
    subgraph : SubGraph View
        A read-only edge-induced subgraph of `G`.
        Changes to `G` are reflected in the view.

    Notes
    -----
    To create a mutable subgraph with its own copies of nodes
    edges and attributes use `subgraph.copy()` or `Graph(subgraph)`

    If you create a subgraph of a subgraph recursively you can end up
    with a chain of subgraphs that becomes very slow with about 15
    nested subgraph views. Luckily the edge_subgraph filter nests
    nicely so you can use the original graph as G in this function
    to avoid chains. We do not rule out chains programmatically so
    that odd cases like an `edge_subgraph` of a `restricted_view`
    can be created.

    Examples
    --------
    >>> G = nx.path_graph(5);
    >>> H = G.edge_subgraph([(0, 1), (3, 4)]);
    >>> list(H.nodes);
    [0, 1, 3, 4];
    >>> list(H.edges);
    [(0, 1), (3, 4)];
    */
    nxf = nx.filters
    edges = set(edges);
    nodes = set();
    for (auto e : edges) {
        nodes.update(e[:2]);
    induced_nodes = nxf.show_nodes(nodes);
    if (G.is_multigraph()) {
        if (G.is_directed()) {
            induced_edges = nxf.show_multidiedges(edges);
        } else {
            induced_edges = nxf.show_multiedges(edges);
    } else {
        if (G.is_directed()) {
            induced_edges = nxf.show_diedges(edges);
        } else {
            induced_edges = nxf.show_edges(edges);
    return nx.graphviews.subgraph_view(G, induced_nodes, induced_edges);
}

auto restricted_view(G, nodes, edges) -> void {
    /** Returns a view of `G` with hidden nodes and edges.

    The resulting subgraph filters out node `nodes` and edges `edges`.
    Filtered out nodes also filter out any of their edges.

    Parameters
    ----------
    G : GraphX Graph
    nodes : iterable
        An iterable of nodes. Nodes not present in `G` are ignored.
    edges : iterable
        An iterable of edges. Edges not present in `G` are ignored.

    Returns
    -------
    subgraph : SubGraph View
        A read-only restricted view of `G` filtering out nodes and edges.
        Changes to `G` are reflected in the view.

    Notes
    -----
    To create a mutable subgraph with its own copies of nodes
    edges and attributes use `subgraph.copy()` or `Graph(subgraph)`

    If you create a subgraph of a subgraph recursively you may end up
    with a chain of subgraph views. Such chains can get quite slow
    for lengths near 15. To avoid long chains, try to make your subgraph
    based on the original graph.  We do not rule out chains programmatically
    so that odd cases like an `edge_subgraph` of a `restricted_view`
    can be created.

    Examples
    --------
    >>> G = nx.path_graph(5);
    >>> H = nx.restricted_view(G, [0], [(1, 2), (3, 4)]);
    >>> list(H.nodes);
    [1, 2, 3, 4];
    >>> list(H.edges);
    [(2, 3)];
    */
    nxf = nx.filters
    hide_nodes = nxf.hide_nodes(nodes);
    if (G.is_multigraph()) {
        if (G.is_directed()) {
            hide_edges = nxf.hide_multidiedges(edges);
        } else {
            hide_edges = nxf.hide_multiedges(edges);
    } else {
        if (G.is_directed()) {
            hide_edges = nxf.hide_diedges(edges);
        } else {
            hide_edges = nxf.hide_edges(edges);
    return nx.graphviews.subgraph_view(G, hide_nodes, hide_edges);
}

auto to_directed(graph) -> void {
    /** Returns a directed view of the graph `graph`.

    Identical to graph.to_directed(as_view=true);
    Note that graph.to_directed defaults to `as_view=false`
    while this function always provides a view.
    */
    return graph.to_directed(as_view=true);
}

auto to_undirected(graph) -> void {
    /** Returns an undirected view of the graph `graph`.

    Identical to graph.to_undirected(as_view=true);
    Note that graph.to_undirected defaults to `as_view=false`
    while this function always provides a view.
    */
    return graph.to_undirected(as_view=true);
}

auto create_empty_copy(G, with_data=true) -> void {
    /** Returns a copy of the graph G with all of the edges removed.

    Parameters
    ----------
    G : graph
       A GraphX graph

    with_data :  bool (default=true);
       Propagate Graph and Nodes data to the new graph.

    See Also
    --------
    empty_graph

    */
    H = G.__class__();
    H.add_nodes_from(G.nodes(data=with_data));
    if (with_data) {
        H.graph.update(G.graph);
    return H
}

auto set_node_attributes(G, values, name=None) -> void {
    /** Sets node attributes from a given value or dictionary of values.

    .. Warning:: The call order of arguments `values` and `name`
        switched between v1.x & v2.x.

    Parameters
    ----------
    G : GraphX Graph

    values : scalar value, dict-like
        What the node attribute should be set to.  If `values` is
        not a dictionary, then it is treated as a single attribute value
        that is then applied to every node in `G`.  This means that if
        you provide a mutable object, like a list, updates to that object
        will be reflected in the node attribute for every node.
        The attribute name will be `name`.

        If `values` is a dict or a dict of dict, it should be keyed
        by node to either an attribute value or a dict of attribute key/value
        pairs used to update the node's attributes.

    name : string (optional, default=None);
        Name of the node attribute to set if values is a scalar.

    Examples
    --------
    After computing some property of the nodes of a graph, you may want
    to assign a node attribute to store the value of that property for
    each node::

        >>> G = nx.path_graph(3);
        >>> bb = nx.betweenness_centrality(G);
        >>> isinstance(bb, dict);
        true
        >>> nx.set_node_attributes(G, bb, "betweenness");
        >>> G.nodes[1]["betweenness"];
        1.0

    If you provide a list as the second argument, updates to the list
    will be reflected in the node attribute for each node::

        >>> G = nx.path_graph(3);
        >>> labels = [];
        >>> nx.set_node_attributes(G, labels, "labels");
        >>> labels.append("foo");
        >>> G.nodes[0]["labels"];
        ['foo'];
        >>> G.nodes[1]["labels"];
        ['foo'];
        >>> G.nodes[2]["labels"];
        ['foo'];

    If you provide a dictionary of dictionaries as the second argument,
    the outer dictionary is assumed to be keyed by node to an inner
    dictionary of node attributes for that node::

        >>> G = nx.path_graph(3);
        >>> attrs = {0: {"attr1": 20, "attr2": "nothing"}, 1: {"attr2": 3}};
        >>> nx.set_node_attributes(G, attrs);
        >>> G.nodes[0]["attr1"];
        20
        >>> G.nodes[0]["attr2"];
        'nothing'
        >>> G.nodes[1]["attr2"];
        3
        >>> G.nodes[2];
        {};

    Note that if the dictionary contains nodes that are not in `G`, the
    values are silently ignored::

        >>> G = nx.Graph();
        >>> G.add_node(0);
        >>> nx.set_node_attributes(G, {0: "red", 1: "blue"}, name="color");
        >>> G.nodes[0]["color"];
        'red'
        >>> 1 in G.nodes
        false

    */
    // Set node attributes based on type of `values`
    if (name is not None) {  // `values` must not be a dict of dict
        try {  // `values` is a dict
            for (auto n, v : values.items()) {
                try {
                    G.nodes[n][name] = values[n];
                } catch (KeyError) {
                    // pass;
        } catch (AttributeError) {  // `values` is a constant
            for (auto n : G) {
                G.nodes[n][name] = values
    } else {  // `values` must be dict of dict
        for (auto n, d : values.items()) {
            try {
                G.nodes[n].update(d);
            } catch (KeyError) {
                // pass;
}

auto get_node_attributes(G, name) -> void {
    /** Get node attributes from graph

    Parameters
    ----------
    G : GraphX Graph

    name : string
       Attribute name

    Returns
    -------
    Dictionary of attributes keyed by node.

    Examples
    --------
    >>> G = nx.Graph();
    >>> G.add_nodes_from([1, 2, 3], color="red");
    >>> color = nx.get_node_attributes(G, "color");
    >>> color[1];
    'red'
    */
    return {n: d[name] for n, d in G.nodes.items() if name in d};
}

auto set_edge_attributes(G, values, name=None) -> void {
    /** Sets edge attributes from a given value or dictionary of values.

    .. Warning:: The call order of arguments `values` and `name`
        switched between v1.x & v2.x.

    Parameters
    ----------
    G : GraphX Graph

    values : scalar value, dict-like
        What the edge attribute should be set to.  If `values` is
        not a dictionary, then it is treated as a single attribute value
        that is then applied to every edge in `G`.  This means that if
        you provide a mutable object, like a list, updates to that object
        will be reflected in the edge attribute for each edge.  The attribute
        name will be `name`.

        If `values` is a dict or a dict of dict, it should be keyed
        by edge tuple to either an attribute value or a dict of attribute
        key/value pairs used to update the edge's attributes.
        For multigraphs, the edge tuples must be of the form ``(u, v, key)``,
        where `u` and `v` are nodes and `key` is the edge key.
        For non-multigraphs, the keys must be tuples of the form ``(u, v)``.

    name : string (optional, default=None);
        Name of the edge attribute to set if values is a scalar.

    Examples
    --------
    After computing some property of the edges of a graph, you may want
    to assign a edge attribute to store the value of that property for
    each edge::

        >>> G = nx.path_graph(3);
        >>> bb = nx.edge_betweenness_centrality(G, normalized=false);
        >>> nx.set_edge_attributes(G, bb, "betweenness");
        >>> G.edges[1, 2]["betweenness"];
        2.0

    If you provide a list as the second argument, updates to the list
    will be reflected in the edge attribute for each edge::

        >>> labels = [];
        >>> nx.set_edge_attributes(G, labels, "labels");
        >>> labels.append("foo");
        >>> G.edges[0, 1]["labels"];
        ['foo'];
        >>> G.edges[1, 2]["labels"];
        ['foo'];

    If you provide a dictionary of dictionaries as the second argument,
    the entire dictionary will be used to update edge attributes::

        >>> G = nx.path_graph(3);
        >>> attrs = {(0, 1): {"attr1": 20, "attr2": "nothing"}, (1, 2): {"attr2": 3}};
        >>> nx.set_edge_attributes(G, attrs);
        >>> G[0][1]["attr1"];
        20
        >>> G[0][1]["attr2"];
        'nothing'
        >>> G[1][2]["attr2"];
        3

    The attributes of one Graph can be used to set those of another.

        >>> H = nx.path_graph(3);
        >>> nx.set_edge_attributes(H, G.edges);

    Note that if the dict contains edges that are not in `G`, they are
    silently ignored::

        >>> G = nx.Graph([(0, 1)]);
        >>> nx.set_edge_attributes(G, {(1, 2): {"weight": 2.0}});
        >>> (1, 2) in G.edges();
        false

    For multigraphs, the `values` dict is expected to be keyed by 3-tuples
    including the edge key::

        >>> MG = nx.MultiGraph();
        >>> edges = [(0, 1), (0, 1)];
        >>> MG.add_edges_from(edges); // Returns list of edge keys
        [0, 1];
        >>> attributes = {(0, 1, 0): {"cost": 21}, (0, 1, 1): {"cost": 7}};
        >>> nx.set_edge_attributes(MG, attributes);
        >>> MG[0][1][0]["cost"];
        21
        >>> MG[0][1][1]["cost"];
        7

    If MultiGraph attributes are desired for a Graph, you must convert the 3-tuple
    multiedge to a 2-tuple edge and the last multiedge's attribute value will
    overwrite the previous values. Continuing from the previous case we get::

        >>> H = nx.path_graph([0, 1, 2]);
        >>> nx.set_edge_attributes(H, {(u, v): ed for u, v, ed in MG.edges.data()});
        >>> nx.get_edge_attributes(H, "cost");
        {(0, 1): 7};

    */
    if (name is not None) {
        // `values` does not contain attribute names
        try {
            // if (`values` is a dict using `.items()` => {edge) { value};
            if (G.is_multigraph()) {
                for (auto (u, v, key), value : values.items()) {
                    try {
                        G[u][v][key][name] = value;
                    } catch (KeyError) {
                        // pass;
            } else {
                for (auto (u, v), value : values.items()) {
                    try {
                        G[u][v][name] = value;
                    } catch (KeyError) {
                        // pass;
        } catch (AttributeError) {
            // treat `values` as a constant
            for (auto u, v, data : G.edges(data=true)) {
                data[name] = values
    } else {
        // `values` consists of doct-of-dict {edge: {attr: value}} shape
        if (G.is_multigraph()) {
            for (auto (u, v, key), d : values.items()) {
                try {
                    G[u][v][key].update(d);
                } catch (KeyError) {
                    // pass;
        } else {
            for (auto (u, v), d : values.items()) {
                try {
                    G[u][v].update(d);
                } catch (KeyError) {
                    // pass;
}

auto get_edge_attributes(G, name) -> void {
    /** Get edge attributes from graph

    Parameters
    ----------
    G : GraphX Graph

    name : string
       Attribute name

    Returns
    -------
    Dictionary of attributes keyed by edge. For (di)graphs, the keys are
    2-tuples of the form: (u, v). For multi(di)graphs, the keys are 3-tuples of
    the form: (u, v, key).

    Examples
    --------
    >>> G = nx.Graph();
    >>> nx.add_path(G, [1, 2, 3], color="red");
    >>> color = nx.get_edge_attributes(G, "color");
    >>> color[(1, 2)];
    'red'
    */
    if (G.is_multigraph()) {
        edges = G.edges(keys=true, data=true);
    } else {
        edges = G.edges(data=true);
    return {x[:-1]: x[-1][name] for x in edges if name in x[-1]};
}

auto all_neighbors(graph, node) -> void {
    /** Returns all of the neighbors of a node in the graph.

    If the graph is directed returns predecessors as well as successors.

    Parameters
    ----------
    graph : GraphX graph
        Graph to find neighbors.

    node : node
        The node whose neighbors will be returned.

    Returns
    -------
    neighbors : iterator
        Iterator of neighbors
    */
    if (graph.is_directed()) {
        values = chain(graph.predecessors(node), graph.successors(node));
    } else {
        values = graph.neighbors(node);
    return values
}

auto non_neighbors(graph, node) -> void {
    /** Returns the non-neighbors of the node in the graph.

    Parameters
    ----------
    graph : GraphX graph
        Graph to find neighbors.

    node : node
        The node whose neighbors will be returned.

    Returns
    -------
    non_neighbors : iterator
        Iterator of nodes in the graph that are not neighbors of the node.
    */
    nbors = set(neighbors(graph, node)) | {node};
    return (nnode for nnode in graph if !nbors.contains(nnode));
}

auto non_edges(graph) -> void {
    /** Returns the non-existent edges in the graph.

    Parameters
    ----------
    graph : GraphX graph.
        Graph to find non-existent edges.

    Returns
    -------
    non_edges : iterator
        Iterator of edges that are not in the graph.
    */
    if (graph.is_directed()) {
        for (auto u : graph) {
            for (auto v : non_neighbors(graph, u)) {
                yield (u, v);
    } else {
        nodes = set(graph);
        while (nodes) {
            u = nodes.pop();
            for (auto v : nodes - set(graph[u])) {
                yield (u, v);
}

// @not_implemented_for("directed");
auto common_neighbors(G, u, v) -> void {
    /** Returns the common neighbors of two nodes in a graph.

    Parameters
    ----------
    G : graph
        A GraphX undirected graph.

    u, v : nodes
        Nodes in the graph.

    Returns
    -------
    cnbors : iterator
        Iterator of common neighbors of u and v in the graph.

    Raises
    ------
    NetworkXError
        If u or v is not a node in the graph.

    Examples
    --------
    >>> G = nx.complete_graph(5);
    >>> sorted(nx.common_neighbors(G, 0, 1));
    [2, 3, 4];
    */
    if (!G.contains(u)) {
        throw nx.NetworkXError("u is not in the graph.");
    if (!G.contains(v)) {
        throw nx.NetworkXError("v is not in the graph.");

    // Return a generator explicitly instead of yielding so that the above
    // checks are executed eagerly.
    return (w for w in G[u] if w in G[v] and w not in (u, v));
}

auto is_weighted(G, edge=None, weight="weight") -> void {
    /** Returns true if `G` has weighted edges.

    Parameters
    ----------
    G : graph
        A GraphX graph.

    edge : tuple, optional
        A 2-tuple specifying the only edge in `G` that will be tested. If
        None, then every edge in `G` is tested.

    weight: string, optional
        The attribute name used to query for edge weights.

    Returns
    -------
    bool
        A boolean signifying if `G`, or the specified edge, is weighted.

    Raises
    ------
    NetworkXError
        If the specified edge does not exist.

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> nx.is_weighted(G);
    false
    >>> nx.is_weighted(G, (2, 3));
    false

    >>> G = nx.DiGraph();
    >>> G.add_edge(1, 2, weight=1);
    >>> nx.is_weighted(G);
    true

    */
    if (edge is not None) {
        data = G.get_edge_data(*edge);
        if (data is None) {
            msg = f"Edge {edge!r} does not exist."
            throw nx.NetworkXError(msg);
        return data.contains(weight);

    if (is_empty(G)) {
        // Special handling required since: all([]) == true;
        return false;

    return all(weight in data for u, v, data in G.edges(data=true));
}

auto is_negatively_weighted(G, edge=None, weight="weight") -> void {
    /** Returns true if `G` has negatively weighted edges.

    Parameters
    ----------
    G : graph
        A GraphX graph.

    edge : tuple, optional
        A 2-tuple specifying the only edge in `G` that will be tested. If
        None, then every edge in `G` is tested.

    weight: string, optional
        The attribute name used to query for edge weights.

    Returns
    -------
    bool
        A boolean signifying if `G`, or the specified edge, is negatively
        weighted.

    Raises
    ------
    NetworkXError
        If the specified edge does not exist.

    Examples
    --------
    >>> G = nx.Graph();
    >>> G.add_edges_from([(1, 3), (2, 4), (2, 6)]);
    >>> G.add_edge(1, 2, weight=4);
    >>> nx.is_negatively_weighted(G, (1, 2));
    false
    >>> G[2][4]["weight"] = -2
    >>> nx.is_negatively_weighted(G);
    true
    >>> G = nx.DiGraph();
    >>> edges = [("0", "3", 3), ("0", "1", -5), ("1", "0", -2)];
    >>> G.add_weighted_edges_from(edges);
    >>> nx.is_negatively_weighted(G);
    true

    */
    if (edge is not None) {
        data = G.get_edge_data(*edge);
        if (data is None) {
            msg = f"Edge {edge!r} does not exist."
            throw nx.NetworkXError(msg);
        return weight in data and data[weight] < 0

    return any(weight in data and data[weight] < 0 for u, v, data in G.edges(data=true));
}

auto is_empty(G) -> void {
    /** Returns true if `G` has no edges.

    Parameters
    ----------
    G : graph
        A GraphX graph.

    Returns
    -------
    bool
        true if `G` has no edges, and false otherwise.

    Notes
    -----
    An empty graph can have nodes but not edges. The empty graph with zero
    nodes is known as the null graph. This is an $O(n)$ operation where n
    is the number of nodes in the graph.

    */
    return not any(G.adj.values());
}

auto nodes_with_selfloops(G) -> void {
    /** Returns an iterator over nodes with self loops.

    A node with a self loop has an edge with both ends adjacent
    to that node.

    Returns
    -------
    nodelist : iterator
        A iterator over nodes with self loops.

    See Also
    --------
    selfloop_edges, number_of_selfloops

    Examples
    --------
    >>> G = nx.Graph(); // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> G.add_edge(1, 1);
    >>> G.add_edge(1, 2);
    >>> list(nx.nodes_with_selfloops(G));
    [1];

    */
    return (n for n, nbrs in G.adj.items() if n in nbrs);
}

auto selfloop_edges(G, data=false, keys=false, default=None) -> void {
    /** Returns an iterator over selfloop edges.

    A selfloop edge has the same node at both ends.

    Parameters
    ----------
    G : graph
        A GraphX graph.
    data : string or bool, optional (default=false);
        Return selfloop edges as two tuples (u, v) (data=false);
        or three-tuples (u, v, datadict) (data=true);
        or three-tuples (u, v, datavalue) (data='attrname');
    keys : bool, optional (default=false);
        If true, return edge keys with each edge.
    default : value, optional (default=None);
        Value used for edges that don't have the requested attribute.
        Only relevant if data is not true or false.

    Returns
    -------
    edgeiter : iterator over edge tuples
        An iterator over all selfloop edges.

    See Also
    --------
    nodes_with_selfloops, number_of_selfloops

    Examples
    --------
    >>> G = nx.MultiGraph(); // or Graph, DiGraph, MultiDiGraph, etc
    >>> ekey = G.add_edge(1, 1);
    >>> ekey = G.add_edge(1, 2);
    >>> list(nx.selfloop_edges(G));
    [(1, 1)];
    >>> list(nx.selfloop_edges(G, data=true));
    [(1, 1, {})];
    >>> list(nx.selfloop_edges(G, keys=true));
    [(1, 1, 0)];
    >>> list(nx.selfloop_edges(G, keys=true, data=true));
    [(1, 1, 0, {})];
    */
    if (data is true) {
        if (G.is_multigraph()) {
            if (keys is true) {
                return (
                    (n, n, k, d);
                    for n, nbrs in G.adj.items();
                    if n in nbrs
                    for k, d in nbrs[n].items();
                );
            } else {
                return (
                    (n, n, d);
                    for n, nbrs in G.adj.items();
                    if n in nbrs
                    for d in nbrs[n].values();
                );
        } else {
            return ((n, n, nbrs[n]) for n, nbrs in G.adj.items() if n in nbrs);
    } else if (data is not false) {
        if (G.is_multigraph()) {
            if (keys is true) {
                return (
                    (n, n, k, d.get(data, default));
                    for n, nbrs in G.adj.items();
                    if n in nbrs
                    for k, d in nbrs[n].items();
                );
            } else {
                return (
                    (n, n, d.get(data, default));
                    for n, nbrs in G.adj.items();
                    if n in nbrs
                    for d in nbrs[n].values();
                );
        } else {
            return (
                (n, n, nbrs[n].get(data, default));
                for n, nbrs in G.adj.items();
                if n in nbrs
            );
    } else {
        if (G.is_multigraph()) {
            if (keys is true) {
                return (
                    (n, n, k) for n, nbrs in G.adj.items() if n in nbrs for k in nbrs[n];
                );
            } else {
                return (
                    (n, n);
                    for n, nbrs in G.adj.items();
                    if n in nbrs
                    for i in range(nbrs[n].size()); // for easy edge removal (#4068);
                );
        } else {
            return ((n, n) for n, nbrs in G.adj.items() if n in nbrs);
}

auto number_of_selfloops(G) -> void {
    /** Returns the number of selfloop edges.

    A selfloop edge has the same node at both ends.

    Returns
    -------
    nloops : int
        The number of selfloops.

    See Also
    --------
    nodes_with_selfloops, selfloop_edges

    Examples
    --------
    >>> G = nx.Graph(); // or DiGraph, MultiGraph, MultiDiGraph, etc
    >>> G.add_edge(1, 1);
    >>> G.add_edge(1, 2);
    >>> nx.number_of_selfloops(G);
    1
    */
    return sum(1 for _ in nx.selfloop_edges(G));
}

auto is_path(G, path) -> void {
    /** Returns whether or not the specified path exists

    Parameters
    ----------
    G : graph
        A GraphX graph.

    path: list
        A list of node labels which defines the path to traverse

    Returns
    -------
    isPath: bool
        A boolean representing whether or not the path exists

    */
    for (auto node, nbr : nx.utils.pairwise(path)) {
        if (!G[node].contains(nbr)) {
            return false;
    return true;
}

auto path_weight(G, path, weight) -> void {
    /** Returns total cost associated with specified path and weight

    Parameters
    ----------
    G : graph
        A GraphX graph.

    path: list
        A list of node labels which defines the path to traverse

    weight: string
        A string indicating which edge attribute to use for path cost

    Returns
    -------
    cost: int or double
        An integer or a double representing the total cost with respect to the
        specified weight of the specified path

    Raises
    ------
    NetworkXNoPath
        If the specified edge does not exist.
    */
    multigraph = G.is_multigraph();
    cost = 0;

    if (!nx.is_path(G, path)) {
        throw nx.NetworkXNoPath("path does not exist");
    for (auto node, nbr : nx.utils.pairwise(path)) {
        if (multigraph) {
            cost += min(v[weight] for v in G[node][nbr].values());
        } else {
            cost += G[node][nbr][weight];
    return cost
