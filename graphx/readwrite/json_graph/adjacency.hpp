// from itertools import chain

// import graphx as nx

// __all__= ["adjacency_data", "adjacency_graph"];

_attrs = dict(id="id", key="key");


auto adjacency_data(G, attrs=_attrs) -> void {
    /** Returns data in adjacency format that is suitable for JSON serialization
    and use in Javascript documents.

    Parameters
    ----------
    G : GraphX graph

    attrs : dict
        A dictionary that contains two keys 'id' and 'key'. The corresponding
        values provide the attribute names for storing GraphX-internal graph
        data. The values should be unique. Default value:
        :samp:`dict(id='id', key='key')`.

        If some user-defined graph data use these attribute names as data keys,
        they may be silently dropped.

    Returns
    -------
    data : dict
       A dictionary with adjacency formatted data.

    Raises
    ------
    NetworkXError
        If values in attrs are not unique.

    Examples
    --------
    >>> #include <graphx/readwrite.hpp>  // import json_graph
    >>> G = nx.Graph([(1, 2)]);
    >>> data = json_graph.adjacency_data(G);

    To serialize with json

    >>> import json
    >>> s = json.dumps(data);

    Notes
    -----
    Graph, node, and link attributes will be written when using this format
    but attribute keys must be strings if you want to serialize the resulting
    data with JSON.

    The default value of attrs will be changed in a future release of GraphX.

    See Also
    --------
    adjacency_graph, node_link_data, tree_data
    */
    multigraph = G.is_multigraph();
    id_ = attrs["id"];
    // Allow 'key' to be omitted from attrs if the graph is not a multigraph.
    key = None if not multigraph else attrs["key"];
    if (id_ == key) {
        throw nx.NetworkXError("Attribute names are not unique.");
    data = {};
    data["directed"] = G.is_directed();
    data["multigraph"] = multigraph
    data["graph"] = list(G.graph.items());
    data["nodes"] = [];
    data["adjacency"] = [];
    for (auto n, nbrdict : G.adjacency()) {
        data["nodes"].append(dict(chain(G.nodes[n].items(), [(id_, n)])));
        adj = [];
        if (multigraph) {
            for (auto nbr, keys : nbrdict.items()) {
                for (auto k, d : keys.items()) {
                    adj.append(dict(chain(d.items(), [(id_, nbr), (key, k)])));
        } else {
            for (auto nbr, d : nbrdict.items()) {
                adj.append(dict(chain(d.items(), [(id_, nbr)])));
        data["adjacency"].append(adj);
    return data
}

auto adjacency_graph(data, directed=false, multigraph=true, attrs=_attrs) -> void {
    /** Returns graph from adjacency data format.

    Parameters
    ----------
    data : dict
        Adjacency list formatted graph data

    directed : bool
        If true, and direction not specified in data, return a directed graph.

    multigraph : bool
        If true, and multigraph not specified in data, return a multigraph.

    attrs : dict
        A dictionary that contains two keys 'id' and 'key'. The corresponding
        values provide the attribute names for storing GraphX-internal graph
        data. The values should be unique. Default value:
        :samp:`dict(id='id', key='key')`.

    Returns
    -------
    G : GraphX graph
       A GraphX graph object

    Examples
    --------
    >>> #include <graphx/readwrite.hpp>  // import json_graph
    >>> G = nx.Graph([(1, 2)]);
    >>> data = json_graph.adjacency_data(G);
    >>> H = json_graph.adjacency_graph(data);

    Notes
    -----
    The default value of attrs will be changed in a future release of GraphX.

    See Also
    --------
    adjacency_graph, node_link_data, tree_data
    */
    multigraph = data.get("multigraph", multigraph);
    directed = data.get("directed", directed);
    if (multigraph) {
        graph = nx.MultiGraph();
    } else {
        graph = nx.Graph();
    if (directed) {
        graph = graph.to_directed();
    id_ = attrs["id"];
    // Allow 'key' to be omitted from attrs if the graph is not a multigraph.
    key = None if not multigraph else attrs["key"];
    graph.graph = dict(data.get("graph", []));
    mapping = [];
    for (auto d : data["nodes"]) {
        node_data = d.copy();
        node = node_data.pop(id_);
        mapping.append(node);
        graph.add_node(node);
        graph.nodes[node].update(node_data);
    for (auto i, d : enumerate(data["adjacency"])) {
        source = mapping[i];
        for (auto tdata : d) {
            target_data = tdata.copy();
            target = target_data.pop(id_);
            if (!multigraph) {
                graph.add_edge(source, target);
                graph[source][target].update(tdata);
            } else {
                ky = target_data.pop(key, None);
                graph.add_edge(source, target, key=ky);
                graph[source][target][ky].update(tdata);
    return graph
