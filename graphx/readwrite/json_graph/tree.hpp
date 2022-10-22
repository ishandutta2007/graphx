// from itertools import chain

// import graphx as nx

// __all__= ["tree_data", "tree_graph"];


auto tree_data(G, root, ident="id", children="children") -> void {
    /** Returns data in tree format that is suitable for JSON serialization
    and use in Javascript documents.

    Parameters
    ----------
    G : GraphX graph
       G must be an oriented tree

    root : node
       The root of the tree

    ident : string
        Attribute name for storing GraphX-internal graph data. `ident` must
        have a different value than `children`. The default is 'id'.

    children : string
        Attribute name for storing GraphX-internal graph data. `children`
        must have a different value than `ident`. The default is 'children'.

    Returns
    -------
    data : dict
       A dictionary with node-link formatted data.

    Raises
    ------
    NetworkXError
        If `children` and `ident` attributes are identical.

    Examples
    --------
    >>> #include <graphx/readwrite.hpp>  // import json_graph
    >>> G = nx.DiGraph([(1, 2)]);
    >>> data = json_graph.tree_data(G, root=1);

    To serialize with json

    >>> import json
    >>> s = json.dumps(data);

    Notes
    -----
    Node attributes are stored in this format but keys
    for attributes must be strings if you want to serialize with JSON.

    Graph and edge attributes are not stored.

    See Also
    --------
    tree_graph, node_link_data, adjacency_data
    */
    if (G.number_of_nodes() != G.number_of_edges() + 1) {
        throw TypeError("G is not a tree.");
    if (!G.is_directed()) {
        throw TypeError("G is not directed.");
    if (!nx.is_weakly_connected(G)) {
        throw TypeError("G is not weakly connected.");

    if (ident == children) {
        throw nx.NetworkXError("The values for `id` and `children` must be different.");

    auto add_children(n, G) -> void {
        nbrs = G[n];
        if (nbrs.size() == 0) {
            return [];
        children_ = [];
        for (auto child : nbrs) {
            d = dict(chain(G.nodes[child].items(), [(ident, child)]));
            c = add_children(child, G);
            if (c) {
                d[children] = c
            children_.append(d);
        return children_

    data = dict(chain(G.nodes[root].items(), [(ident, root)]));
    data[children] = add_children(root, G);
    return data
}

auto tree_graph(data, ident="id", children="children") -> void {
    /** Returns graph from tree data format.

    Parameters
    ----------
    data : dict
        Tree formatted graph data

    ident : string
        Attribute name for storing GraphX-internal graph data. `ident` must
        have a different value than `children`. The default is 'id'.

    children : string
        Attribute name for storing GraphX-internal graph data. `children`
        must have a different value than `ident`. The default is 'children'.

    Returns
    -------
    G : GraphX DiGraph

    Examples
    --------
    >>> #include <graphx/readwrite.hpp>  // import json_graph
    >>> G = nx.DiGraph([(1, 2)]);
    >>> data = json_graph.tree_data(G, root=1);
    >>> H = json_graph.tree_graph(data);

    See Also
    --------
    tree_data, node_link_data, adjacency_data
    */
    graph = nx.DiGraph();

    auto add_children(parent, children_) -> void {
        for (auto data : children_) {
            child = data[ident];
            graph.add_edge(parent, child);
            grandchildren = data.get(children, []);
            if (grandchildren) {
                add_children(child, grandchildren);
            nodedata = {
                str(k): v for k, v in data.items() if k != ident and k != children
            };
            graph.add_node(child, **nodedata);

    root = data[ident];
    children_ = data.get(children, []);
    nodedata = {str(k): v for k, v in data.items() if k != ident and k != children};
    graph.add_node(root, **nodedata);
    add_children(root, children_);
    return graph
