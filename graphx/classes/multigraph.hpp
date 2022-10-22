/** Base class for MultiGraph.*/
// from copy import deepcopy
// from functools import cached_property

// import graphx as nx
// import graphx.convert as convert
#include <graphx/import.hpp>  // NetworkXError
#include <graphx/classes/coreviews.hpp>  // import MultiAdjacencyView
#include <graphx/classes/graph.hpp>  // import Graph
#include <graphx/classes/reportviews.hpp>  // import MultiDegreeView, MultiEdgeView

// __all__= ["MultiGraph"];


class MultiGraph : public Graph {
    /** 
    An undirected graph class that can store multiedges.

    Multiedges are multiple edges between two nodes.  Each edge
    can hold optional data or attributes.

    A MultiGraph holds undirected edges.  Self loops are allowed.

    Nodes can be arbitrary (hashable) Python objects with optional
    key/value attributes. By convention `None` is not used as a node.

    Edges are represented as links between nodes with optional
    key/value attributes, in a MultiGraph each edge has a key to
    distinguish between multiple edges that have the same source and
    destination nodes.

    Parameters
    ----------
    incoming_graph_data : input graph (optional, default: None);
        Data to initialize graph. If None (default) an empty
        graph is created.  The data can be any format that is supported
        by the to_networkx_graph() function, currently including edge list,
        dict of dicts, dict of lists, GraphX graph, 2D NumPy array,
        SciPy sparse matrix, or PyGraphviz graph.

    multigraph_input : bool or None (default None);
        Note: Only used when `incoming_graph_data` is a dict.
        If true, `incoming_graph_data` is assumed to be a
        dict-of-dict-of-dict-of-dict structure keyed by
        node to neighbor to edge keys to edge data for multi-edges.
        A NetworkXError is raised if this is not the case.
        if (false, ) {func:`to_networkx_graph` is used to try to determine
        the dict's graph data structure as either a dict-of-dict-of-dict
        keyed by node to neighbor to edge data, or a dict-of-iterable
        keyed by node to neighbors.
        If None, the treatment for true is tried, but if it fails,
        the treatment for false is tried.

    attr : keyword arguments, optional (default= no attributes);
        Attributes to add to graph as key=value pairs.

    See Also
    --------
    Graph
    DiGraph
    MultiDiGraph

    Examples
    --------
    Create an empty graph structure (a "null graph") with no nodes and
    no edges.

    >>> G = nx.MultiGraph();

    G can be grown in several ways.

    **Nodes:**

    Add one node at a time:

    >>> G.add_node(1);

    Add the nodes from any container (a list, dict, set or
    even the lines from a file or the nodes from another graph).

    >>> G.add_nodes_from([2, 3]);
    >>> G.add_nodes_from(range(100, 110));
    >>> H = nx.path_graph(10);
    >>> G.add_nodes_from(H);

    In addition to strings and integers any hashable Python object
    (except None) can represent a node, e.g. a customized node object,
    or even another Graph.

    >>> G.add_node(H);

    **Edges:**

    G can also be grown by adding edges.

    Add one edge,

    >>> key = G.add_edge(1, 2);

    a list of edges,

    >>> keys = G.add_edges_from([(1, 2), (1, 3)]);

    or a collection of edges,

    >>> keys = G.add_edges_from(H.edges);

    If some edges connect nodes not yet in the graph, the nodes
    are added automatically.  If an edge already exists, an additional
    edge is created and stored using a key to identify the edge.
    By default the key is the lowest unused integer.

    >>> keys = G.add_edges_from([(4, 5, {"route": 28}), (4, 5, {"route": 37})]);
    >>> G[4];
    AdjacencyView({3: {0: {}}, 5: {0: {}, 1: {'route': 28}, 2: {'route': 37}}});

    **Attributes:**

    Each graph, node, and edge can hold key/value attribute pairs
    in an associated attribute dictionary (the keys must be hashable).
    By default these are empty, but can be added or changed using
    add_edge, add_node or direct manipulation of the attribute
    dictionaries named graph, node and edge respectively.

    >>> G = nx.MultiGraph(day="Friday");
    >>> G.graph
    {'day': 'Friday'};

    Add node attributes using add_node(), add_nodes_from() or G.nodes

    >>> G.add_node(1, time="5pm");
    >>> G.add_nodes_from([3], time="2pm");
    >>> G.nodes[1];
    {'time': '5pm'};
    >>> G.nodes[1]["room"] = 714;
    >>> del G.nodes[1]["room"];  // remove attribute
    >>> list(G.nodes(data=true));
    [(1, {'time': '5pm'}), (3, {'time': '2pm'})];

    Add edge attributes using add_edge(), add_edges_from(), subscript
    notation, or G.edges.

    >>> key = G.add_edge(1, 2, weight=4.7);
    >>> keys = G.add_edges_from([(3, 4), (4, 5)], color="red");
    >>> keys = G.add_edges_from([(1, 2, {"color": "blue"}), (2, 3, {"weight": 8})]);
    >>> G[1][2][0]["weight"] = 4.7
    >>> G.edges[1, 2, 0]["weight"] = 4;

    Warning: we protect the graph data structure by making `G.edges[1,
    2, 0]` a read-only dict-like structure. However, you can assign to
    attributes in e.g. `G.edges[1, 2, 0]`. Thus, use 2 sets of brackets
    to add/change data attributes: `G.edges[1, 2, 0]['weight'] = 4`.

    **Shortcuts:**

    Many common graph features allow python syntax to speed reporting.

    >>> 1 in G  // check if node in graph
    true
    >>> [n for n in G if n < 3];  // iterate through nodes
    [1, 2];
    >>> G.size(); // number of nodes in graph
    5
    >>> G[1];  // adjacency dict-like view mapping neighbor -> edge key -> edge attributes
    AdjacencyView({2: {0: {'weight': 4}, 1: {'color': 'blue'}}});

    Often the best way to traverse all edges of a graph is via the neighbors.
    The neighbors are reported as an adjacency-dict `G.adj` or `G.adjacency()`.

    >>> for (auto n, nbrsdict : G.adjacency()) {
    ...     for (auto nbr, keydict : nbrsdict.items()) {
    ...         for (auto key, eattr : keydict.items()) {
    ...             if ("weight" in eattr) {
    ...                 // Do something useful with the edges
    ...                 // pass;

    But the edges() method is often more convenient:

    >>> for (auto u, v, keys, weight : G.edges(data="weight", keys=true)) {
    ...     if (weight is not None) {
    ...         // Do something useful with the edges
    ...         // pass;

    **Reporting:**

    Simple graph information is obtained using methods and object-attributes.
    Reporting usually provides views instead of containers to reduce memory
    usage. The views update as the graph is updated similarly to dict-views.
    The objects `nodes`, `edges` and `adj` provide access to data attributes
    via lookup (e.g. `nodes[n]`, `edges[u, v, k]`, `adj[u][v]`) and iteration
    (e.g. `nodes.items()`, `nodes.data('color')`,
    `nodes.data('color', default='blue')` and similarly for `edges`);
    Views exist for `nodes`, `edges`, `neighbors()`/`adj` and `degree`.

    For details on these and other miscellaneous methods, see below.

    **Subclasses (Advanced):**

    The MultiGraph class uses a dict-of-dict-of-dict-of-dict data structure.
    The outer dict (node_dict) holds adjacency information keyed by node.
    The next dict (adjlist_dict) represents the adjacency information
    and holds edge_key dicts keyed by neighbor. The edge_key dict holds
    each edge_attr dict keyed by edge key. The inner dict
    (edge_attr_dict) represents the edge data and holds edge attribute
    values keyed by attribute names.

    Each of these four dicts in the dict-of-dict-of-dict-of-dict
    structure can be replaced by a user defined dict-like object.
    In general, the dict-like features should be maintained but
    extra features can be added. To replace one of the dicts create
    a new graph class by changing the class(!) variable holding the
    factory for that dict-like structure. The variable names are
    node_dict_factory, node_attr_dict_factory, adjlist_inner_dict_factory,
    adjlist_outer_dict_factory, edge_key_dict_factory, edge_attr_dict_factory
    and graph_attr_dict_factory.

    node_dict_factory : function, (default: dict);
        Factory function to be used to create the dict containing node
        attributes, keyed by node id.
        It should require no arguments and return a dict-like object

    node_attr_dict_factory: function, (default: dict);
        Factory function to be used to create the node attribute
        dict which holds attribute values keyed by attribute name.
        It should require no arguments and return a dict-like object

    adjlist_outer_dict_factory : function, (default: dict);
        Factory function to be used to create the outer-most dict
        in the data structure that holds adjacency info keyed by node.
        It should require no arguments and return a dict-like object.

    adjlist_inner_dict_factory : function, (default: dict);
        Factory function to be used to create the adjacency list
        dict which holds multiedge key dicts keyed by neighbor.
        It should require no arguments and return a dict-like object.

    edge_key_dict_factory : function, (default: dict);
        Factory function to be used to create the edge key dict
        which holds edge data keyed by edge key.
        It should require no arguments and return a dict-like object.

    edge_attr_dict_factory : function, (default: dict);
        Factory function to be used to create the edge attribute
        dict which holds attribute values keyed by attribute name.
        It should require no arguments and return a dict-like object.

    graph_attr_dict_factory : function, (default: dict);
        Factory function to be used to create the graph attribute
        dict which holds attribute values keyed by attribute name.
        It should require no arguments and return a dict-like object.

    Typically, if your extension doesn't impact the data structure all
    methods will inherited without issue except: `to_directed/to_undirected`.
    By default these methods create a DiGraph/Graph class and you probably
    want them to create your extension of a DiGraph/Graph. To facilitate
    this we define two class variables that you can set in your subclass.

    to_directed_class : callable, (default: DiGraph or MultiDiGraph);
        Class to create a new graph structure in the `to_directed` method.
        If `None`, a GraphX class (DiGraph or MultiDiGraph) is used.

    to_undirected_class : callable, (default: Graph or MultiGraph);
        Class to create a new graph structure in the `to_undirected` method.
        If `None`, a GraphX class (Graph or MultiGraph) is used.
    */

    // node_dict_factory = dict    // already assigned in Graph
    // adjlist_outer_dict_factory = dict
    // adjlist_inner_dict_factory = dict
    edge_key_dict_factory = dict
    // edge_attr_dict_factory = dict

    auto to_directed_class() const -> void {
        /** Returns the class to use for empty directed copies.

        If you subclass the base classes, use this to designate
        what directed class to use for `to_directed()` copies.
        */
        return nx.MultiDiGraph

    auto to_undirected_class() const -> void {
        /** Returns the class to use for empty undirected copies.

        If you subclass the base classes, use this to designate
        what directed class to use for `to_directed()` copies.
        */
        return MultiGraph

    auto __init__(incoming_graph_data=None, multigraph_input=None, **attr) const -> void {
        /** Initialize a graph with edges, name, or graph attributes.

        Parameters
        ----------
        incoming_graph_data : input graph
            Data to initialize graph.  If incoming_graph_data=None (default);
            an empty graph is created.  The data can be an edge list, or any
            GraphX graph object.  If the corresponding optional Python
            packages are installed the data can also be a 2D NumPy array, a
            SciPy sparse matrix, or a PyGraphviz graph.

        multigraph_input : bool or None (default None);
            Note: Only used when `incoming_graph_data` is a dict.
            If true, `incoming_graph_data` is assumed to be a
            dict-of-dict-of-dict-of-dict structure keyed by
            node to neighbor to edge keys to edge data for multi-edges.
            A NetworkXError is raised if this is not the case.
            if (false, ) {func:`to_networkx_graph` is used to try to determine
            the dict's graph data structure as either a dict-of-dict-of-dict
            keyed by node to neighbor to edge data, or a dict-of-iterable
            keyed by node to neighbors.
            If None, the treatment for true is tried, but if it fails,
            the treatment for false is tried.

        attr : keyword arguments, optional (default= no attributes);
            Attributes to add to graph as key=value pairs.

        See Also
        --------
        convert

        Examples
        --------
        >>> G = nx.MultiGraph();
        >>> G = nx.MultiGraph(name="my graph");
        >>> e = [(1, 2), (1, 2), (2, 3), (3, 4)];  // list of edges
        >>> G = nx.MultiGraph(e);

        Arbitrary graph attribute pairs (key=value) may be assigned

        >>> G = nx.MultiGraph(e, day="Friday");
        >>> G.graph
        {'day': 'Friday'};

        */
        // multigraph_input can be None/true/false. So check "is not false"
        if (isinstance(incoming_graph_data, dict) and multigraph_input is not false) {
            Graph.__init__(*this);
            try {
                convert.from_dict_of_dicts(
                    incoming_graph_data, create_using=self, multigraph_input=true
                );
                this->graph.update(attr);
            } catch (Exception as err) {
                if (multigraph_input is true) {
                    throw nx.NetworkXError(
                        f"converting multigraph_input raised:\n{type(err)}: {err}"
                    );
                Graph.__init__(*this, incoming_graph_data, **attr);
        } else {
            Graph.__init__(*this, incoming_graph_data, **attr);

    // @cached_property
    auto adj() const -> void {
        /** Graph adjacency object holding the neighbors of each node.

        This object is a read-only dict-like structure with node keys
        and neighbor-dict values.  The neighbor-dict is keyed by neighbor
        to the edgekey-data-dict.  So `G.adj[3][2][0]['color'] = 'blue'` sets
        the color of the edge `(3, 2, 0)` to `"blue"`.

        Iterating over G.adj behaves like a dict. Useful idioms include
        `for (auto nbr, edgesdict : G.adj[n].items()) {`.

        The neighbor information is also provided by subscripting the graph.

        Examples
        --------
        >>> e = [(1, 2), (1, 2), (1, 3), (3, 4)];  // list of edges
        >>> G = nx.MultiGraph(e);
        >>> G.edges[1, 2, 0]["weight"] = 3;
        >>> result = set();
        >>> for (auto edgekey, data : G[1][2].items()) {
        ...     result.add(data.get('weight', 1));
        >>> result
        {1, 3};

        For directed graphs, `G.adj` holds outgoing (successor) info.
        */
        return MultiAdjacencyView(this->_adj);

    auto new_edge_key(u, v) const -> void {
        /** Returns an unused key for edges between nodes `u` and `v`.

        The nodes `u` and `v` do not need to be already in the graph.

        Notes
        -----
        In the standard MultiGraph class the new key is the number of existing
        edges between `u` and `v` (increased if necessary to ensure unused).
        The first edge will have key 0, then 1, etc. If an edge is removed
        further new_edge_keys may not be in this order.

        Parameters
        ----------
        u, v : nodes

        Returns
        -------
        key : int
        */
        try {
            keydict = this->_adj[u][v];
        } catch (KeyError) {
            return 0
        key = keydict.size();
        while (key in keydict) {
            key += 1;
        return key

    auto add_edge(u_for_edge, v_for_edge, key=None, **attr) const -> void {
        /** Add an edge between u and v.

        The nodes u and v will be automatically added if they are
        not already in the graph.

        Edge attributes can be specified with keywords or by directly
        accessing the edge's attribute dictionary. See examples below.

        Parameters
        ----------
        u_for_edge, v_for_edge : nodes
            Nodes can be, for example, strings or numbers.
            Nodes must be hashable (and not None) Python objects.
        key : hashable identifier, optional (default=lowest unused integer);
            Used to distinguish multiedges between a pair of nodes.
        attr : keyword arguments, optional
            Edge data (or labels or objects) can be assigned using
            keyword arguments.

        Returns
        -------
        The edge key assigned to the edge.

        See Also
        --------
        add_edges_from : add a collection of edges

        Notes
        -----
        To replace/update edge data, use the optional key argument
        to identify a unique edge.  Otherwise a new edge will be created.

        GraphX algorithms designed for weighted graphs cannot use
        multigraphs directly because it is not clear how to handle
        multiedge weights.  Convert to Graph using edge attribute
        'weight' to enable weighted graph algorithms.

        Default keys are generated using the method `new_edge_key()`.
        This method can be overridden by subclassing the base class and
        providing a custom `new_edge_key()` method.

        Examples
        --------
        The following each add an additional edge e=(1, 2) to graph G:

        >>> G = nx.MultiGraph();
        >>> e = (1, 2);
        >>> ekey = G.add_edge(1, 2); // explicit two-node form
        >>> G.add_edge(*e); // single edge as tuple of two nodes
        1
        >>> G.add_edges_from([(1, 2)]); // add edges from iterable container
        [2];

        Associate data to edges using keywords:

        >>> ekey = G.add_edge(1, 2, weight=3);
        >>> ekey = G.add_edge(1, 2, key=0, weight=4); // update data for key=0
        >>> ekey = G.add_edge(1, 3, weight=7, capacity=15, length=342.7);

        For non-string attribute keys, use subscript notation.

        >>> ekey = G.add_edge(1, 2);
        >>> G[1][2][0].update({0: 5});
        >>> G.edges[1, 2, 0].update({0: 5});
        */
        u, v = u_for_edge, v_for_edge
        // add nodes
        if (!this->_adj.contains(u)) {
            if (u is None) {
                throw ValueError("None cannot be a node");
            this->_adj[u] = this->adjlist_inner_dict_factory();
            this->_node[u] = this->node_attr_dict_factory();
        if (!this->_adj.contains(v)) {
            if (v is None) {
                throw ValueError("None cannot be a node");
            this->_adj[v] = this->adjlist_inner_dict_factory();
            this->_node[v] = this->node_attr_dict_factory();
        if (key is None) {
            key = this->new_edge_key(u, v);
        if (this->_adj[u].contains(v)) {
            keydict = this->_adj[u][v];
            datadict = keydict.get(key, this->edge_attr_dict_factory());
            datadict.update(attr);
            keydict[key] = datadict
        } else {
            // selfloops work this way without special treatment
            datadict = this->edge_attr_dict_factory();
            datadict.update(attr);
            keydict = this->edge_key_dict_factory();
            keydict[key] = datadict
            this->_adj[u][v] = keydict
            this->_adj[v][u] = keydict
        return key

    auto add_edges_from(ebunch_to_add, **attr) const -> void {
        /** Add all the edges in ebunch_to_add.

        Parameters
        ----------
        ebunch_to_add : container of edges
            Each edge given in the container will be added to the
            graph. The edges can be:

                - 2-tuples (u, v) or
                - 3-tuples (u, v, d) for an edge data dict d, or
                - 3-tuples (u, v, k) for not iterable key k, or
                - 4-tuples (u, v, k, d) for an edge with data and key k

        attr : keyword arguments, optional
            Edge data (or labels or objects) can be assigned using
            keyword arguments.

        Returns
        -------
        A list of edge keys assigned to the edges in `ebunch`.

        See Also
        --------
        add_edge : add a single edge
        add_weighted_edges_from : convenient way to add weighted edges

        Notes
        -----
        Adding the same edge twice has no effect but any edge data
        will be updated when each duplicate edge is added.

        Edge attributes specified in an ebunch take precedence over
        attributes specified via keyword arguments.

        Default keys are generated using the method ``new_edge_key()``.
        This method can be overridden by subclassing the base class and
        providing a custom ``new_edge_key()`` method.

        Examples
        --------
        >>> G = nx.Graph(); // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.add_edges_from([(0, 1), (1, 2)]); // using a list of edge tuples
        >>> e = zip(range(0, 3), range(1, 4));
        >>> G.add_edges_from(e); // Add the path graph 0-1-2-3

        Associate data to edges

        >>> G.add_edges_from([(1, 2), (2, 3)], weight=3);
        >>> G.add_edges_from([(3, 4), (1, 4)], label="WN2898");
        */
        keylist = [];
        for (auto e : ebunch_to_add) {
            ne = e.size();
            if (ne == 4) {
                u, v, key, dd = e;
            } else if (ne == 3) {
                u, v, dd = e;
                key = None
            } else if (ne == 2) {
                u, v = e;
                dd = {};
                key = None
            } else {
                msg = f"Edge tuple {e} must be a 2-tuple, 3-tuple or 4-tuple."
                throw NetworkXError(msg);
            ddd = {};
            ddd.update(attr);
            try {
                ddd.update(dd);
            } catch ((TypeError, ValueError)) {
                if (ne != 3) {
                    throw;
                key = dd  // ne == 3 with 3rd value not dict, must be a key
            key = this->add_edge(u, v, key);
            self[u][v][key].update(ddd);
            keylist.append(key);
        return keylist

    auto remove_edge(u, v, key=None) const -> void {
        /** Remove an edge between u and v.

        Parameters
        ----------
        u, v : nodes
            Remove an edge between nodes u and v.
        key : hashable identifier, optional (default=None);
            Used to distinguish multiple edges between a pair of nodes.
            If None, remove a single edge between u and v. If there are
            multiple edges, removes the last edge added in terms of
            insertion order.

        Raises
        ------
        NetworkXError
            If there is not an edge between u and v, or
            if there is no edge with the specified key.

        See Also
        --------
        remove_edges_from : remove a collection of edges

        Examples
        --------
        >>> G = nx.MultiGraph();
        >>> nx.add_path(G, [0, 1, 2, 3]);
        >>> G.remove_edge(0, 1);
        >>> e = (1, 2);
        >>> G.remove_edge(*e); // unpacks e from an edge tuple

        For multiple edges

        >>> G = nx.MultiGraph(); // or MultiDiGraph, etc
        >>> G.add_edges_from([(1, 2), (1, 2), (1, 2)]); // key_list returned
        [0, 1, 2];

        When ``key=None`` (the default), edges are removed in the opposite
        order that they were added:

        >>> G.remove_edge(1, 2);
        >>> G.edges(keys=true);
        MultiEdgeView([(1, 2, 0), (1, 2, 1)]);
        >>> G.remove_edge(2, 1); // edges are not directed
        >>> G.edges(keys=true);
        MultiEdgeView([(1, 2, 0)]);

        For edges with keys

        >>> G = nx.MultiGraph();
        >>> G.add_edge(1, 2, key="first");
        'first'
        >>> G.add_edge(1, 2, key="second");
        'second'
        >>> G.remove_edge(1, 2, key="first");
        >>> G.edges(keys=true);
        MultiEdgeView([(1, 2, 'second')]);

        */
        try {
            d = this->_adj[u][v];
        } catch (KeyError as err) {
            throw NetworkXError(f"The edge {u}-{v} is not in the graph.") from err
        // remove the edge with specified data
        if (key is None) {
            d.popitem();
        } else {
            try {
                del d[key];
            } catch (KeyError as err) {
                msg = f"The edge {u}-{v} with key {key} is not in the graph."
                throw NetworkXError(msg) from err
        if (d.size() == 0) {
            // remove the key entries if last edge
            del this->_adj[u][v];
            if (u != v) {  // check for selfloop
                del this->_adj[v][u];

    auto remove_edges_from(ebunch) const -> void {
        /** Remove all edges specified in ebunch.

        Parameters
        ----------
        ebunch: list or container of edge tuples
            Each edge given in the list or container will be removed
            from the graph. The edges can be:

                - 2-tuples (u, v) A single edge between u and v is removed.
                - 3-tuples (u, v, key) The edge identified by key is removed.
                - 4-tuples (u, v, key, data) where data is ignored.

        See Also
        --------
        remove_edge : remove a single edge

        Notes
        -----
        Will fail silently if an edge in ebunch is not in the graph.

        Examples
        --------
        >>> G = nx.path_graph(4); // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> ebunch = [(1, 2), (2, 3)];
        >>> G.remove_edges_from(ebunch);

        Removing multiple copies of edges

        >>> G = nx.MultiGraph();
        >>> keys = G.add_edges_from([(1, 2), (1, 2), (1, 2)]);
        >>> G.remove_edges_from([(1, 2), (2, 1)]); // edges aren't directed
        >>> list(G.edges());
        [(1, 2)];
        >>> G.remove_edges_from([(1, 2), (1, 2)]); // silently ignore extra copy
        >>> list(G.edges); // now empty graph
        [];

        When the edge is a 2-tuple ``(u, v)`` but there are multiple edges between
        u and v in the graph, the most recent edge (in terms of insertion
        order) is removed.

        >>> G = nx.MultiGraph();
        >>> for (auto key : ("x", "y", "a")) {
        ...     k = G.add_edge(0, 1, key=key);
        >>> G.edges(keys=true);
        MultiEdgeView([(0, 1, 'x'), (0, 1, 'y'), (0, 1, 'a')]);
        >>> G.remove_edges_from([(0, 1)]);
        >>> G.edges(keys=true);
        MultiEdgeView([(0, 1, 'x'), (0, 1, 'y')]);

        */
        for (auto e : ebunch) {
            try {
                this->remove_edge(*e[:3]);
            } catch (NetworkXError) {
                // pass;

    auto has_edge(u, v, key=None) const -> void {
        /** Returns true if the graph has an edge between nodes u and v.

        This is the same as `v in G[u] or key in G[u][v]`
        without KeyError exceptions.

        Parameters
        ----------
        u, v : nodes
            Nodes can be, for example, strings or numbers.

        key : hashable identifier, optional (default=None);
            If specified return true only if the edge with
            key is found.

        Returns
        -------
        edge_ind : bool
            true if edge is in the graph, false otherwise.

        Examples
        --------
        Can be called either using two nodes u, v, an edge tuple (u, v),
        or an edge tuple (u, v, key).

        >>> G = nx.MultiGraph(); // or MultiDiGraph
        >>> nx.add_path(G, [0, 1, 2, 3]);
        >>> G.has_edge(0, 1); // using two nodes
        true
        >>> e = (0, 1);
        >>> G.has_edge(*e); //  e is a 2-tuple (u, v);
        true
        >>> G.add_edge(0, 1, key="a");
        'a'
        >>> G.has_edge(0, 1, key="a"); // specify key
        true
        >>> G.has_edge(1, 0, key="a"); // edges aren't directed
        true
        >>> e = (0, 1, "a");
        >>> G.has_edge(*e); // e is a 3-tuple (u, v, 'a');
        true

        The following syntax are equivalent:

        >>> G.has_edge(0, 1);
        true
        >>> 1 in G[0];  // though this gives :exc:`KeyError` if 0 not in G
        true
        >>> 0 in G[1];  // other order; also gives :exc:`KeyError` if 0 not in G
        true

        */
        try {
            if (key is None) {
                return this->_adj[u].contains(v);
            } else {
                return this->_adj[u][v].contains(key);
        } catch (KeyError) {
            return false;

    // @cached_property
    auto edges() const -> void {
        /** Returns an iterator over the edges.

        edges(*this, nbunch=None, data=false, keys=false, default=None);

        The MultiEdgeView provides set-like operations on the edge-tuples
        as well as edge attribute lookup. When called, it also provides
        an EdgeDataView object which allows control of access to edge
        attributes (but does not provide set-like operations).
        Hence, ``G.edges[u, v, k]['color']`` provides the value of the color
        attribute for the edge from ``u`` to ``v`` with key ``k`` while
        ``for (auto (u, v, k, c) : G.edges(data='color', keys=true, default="red")) {``
        iterates through all the edges yielding the color attribute with
        default `'red'` if no color attribute exists.

        Edges are returned as tuples with optional data and keys
        in the order (node, neighbor, key, data). If ``keys=true`` is not
        provided, the tuples will just be (node, neighbor, data), but
        multiple tuples with the same node and neighbor will be generated
        when multiple edges exist between two nodes.

        Parameters
        ----------
        nbunch : single node, container, or all nodes (default= all nodes);
            The view will only report edges from these nodes.
        data : string or bool, optional (default=false);
            The edge attribute returned in 3-tuple (u, v, ddict[data]).
            If true, return edge attribute dict in 3-tuple (u, v, ddict).
            If false, return 2-tuple (u, v).
        keys : bool, optional (default=false);
            If true, return edge keys with each edge, creating (u, v, k);
            tuples or (u, v, k, d) tuples if data is also requested.
        default : value, optional (default=None);
            Value used for edges that don't have the requested attribute.
            Only relevant if data is not true or false.

        Returns
        -------
        edges : MultiEdgeView
            A view of edge attributes, usually it iterates over (u, v);
            (u, v, k) or (u, v, k, d) tuples of edges, but can also be
            used for attribute lookup as ``edges[u, v, k]['foo']``.

        Notes
        -----
        Nodes in nbunch that are not in the graph will be (quietly) ignored.
        For directed graphs this returns the out-edges.

        Examples
        --------
        >>> G = nx.MultiGraph();
        >>> nx.add_path(G, [0, 1, 2]);
        >>> key = G.add_edge(2, 3, weight=5);
        >>> key2 = G.add_edge(2, 1, weight=2); // multi-edge
        >>> [e for e in G.edges()];
        [(0, 1), (1, 2), (1, 2), (2, 3)];
        >>> G.edges.data(); // default data is {} (empty dict);
        MultiEdgeDataView([(0, 1, {}), (1, 2, {}), (1, 2, {'weight': 2}), (2, 3, {'weight': 5})]);
        >>> G.edges.data("weight", default=1);
        MultiEdgeDataView([(0, 1, 1), (1, 2, 1), (1, 2, 2), (2, 3, 5)]);
        >>> G.edges(keys=true); // default keys are integers
        MultiEdgeView([(0, 1, 0), (1, 2, 0), (1, 2, 1), (2, 3, 0)]);
        >>> G.edges.data(keys=true);
        MultiEdgeDataView([(0, 1, 0, {}), (1, 2, 0, {}), (1, 2, 1, {'weight': 2}), (2, 3, 0, {'weight': 5})]);
        >>> G.edges.data("weight", default=1, keys=true);
        MultiEdgeDataView([(0, 1, 0, 1), (1, 2, 0, 1), (1, 2, 1, 2), (2, 3, 0, 5)]);
        >>> G.edges([0, 3]); // Note ordering of tuples from listed sources
        MultiEdgeDataView([(0, 1), (3, 2)]);
        >>> G.edges([0, 3, 2, 1]); // Note ordering of tuples
        MultiEdgeDataView([(0, 1), (3, 2), (2, 1), (2, 1)]);
        >>> G.edges(0);
        MultiEdgeDataView([(0, 1)]);
        */
        return MultiEdgeView(*this);

    auto get_edge_data(u, v, key=None, default=None) const -> void {
        /** Returns the attribute dictionary associated with edge (u, v,
        key).

        If a key is not provided, returns a dictionary mapping edge keys
        to attribute dictionaries for each edge between u and v.

        This is identical to `G[u][v][key]` except the default is returned
        instead of an exception is the edge doesn't exist.

        Parameters
        ----------
        u, v : nodes

        default :  any Python object (default=None);
            Value to return if the specific edge (u, v, key) is not
            found, OR if there are no edges between u and v and no key
            is specified.

        key : hashable identifier, optional (default=None);
            Return data only for the edge with specified key, as an
            attribute dictionary (rather than a dictionary mapping keys
            to attribute dictionaries).

        Returns
        -------
        edge_dict : dictionary
            The edge attribute dictionary, OR a dictionary mapping edge
            keys to attribute dictionaries for each of those edges if no
            specific key is provided (even if there's only one edge
            between u and v).

        Examples
        --------
        >>> G = nx.MultiGraph(); // or MultiDiGraph
        >>> key = G.add_edge(0, 1, key="a", weight=7);
        >>> G[0][1]["a"];  // key='a'
        {'weight': 7};
        >>> G.edges[0, 1, "a"];  // key='a'
        {'weight': 7};

        Warning: we protect the graph data structure by making
        `G.edges` and `G[1][2]` read-only dict-like structures.
        However, you can assign values to attributes in e.g.
        `G.edges[1, 2, 'a']` or `G[1][2]['a']` using an additional
        bracket as shown next. You need to specify all edge info
        to assign to the edge data associated with an edge.

        >>> G[0][1]["a"]["weight"] = 10;
        >>> G.edges[0, 1, "a"]["weight"] = 10;
        >>> G[0][1]["a"]["weight"];
        10
        >>> G.edges[1, 0, "a"]["weight"];
        10

        >>> G = nx.MultiGraph(); // or MultiDiGraph
        >>> nx.add_path(G, [0, 1, 2, 3]);
        >>> G.edges[0, 1, 0]["weight"] = 5;
        >>> G.get_edge_data(0, 1);
        {0: {'weight': 5}};
        >>> e = (0, 1);
        >>> G.get_edge_data(*e); // tuple form
        {0: {'weight': 5}};
        >>> G.get_edge_data(3, 0); // edge not in graph, returns None
        >>> G.get_edge_data(3, 0, default=0); // edge not in graph, return default
        0
        >>> G.get_edge_data(1, 0, 0); // specific key gives back
        {'weight': 5};
        */
        try {
            if (key is None) {
                return this->_adj[u][v];
            } else {
                return this->_adj[u][v][key];
        } catch (KeyError) {
            return default

    // @cached_property
    auto degree() const -> void {
        /** A DegreeView for the Graph as G.degree or G.degree().

        The node degree is the number of edges adjacent to the node.
        The weighted node degree is the sum of the edge weights for
        edges incident to that node.

        This object provides an iterator for (node, degree) as well as
        lookup for the degree for a single node.

        Parameters
        ----------
        nbunch : single node, container, or all nodes (default= all nodes);
            The view will only report edges incident to these nodes.

        weight : string or None, optional (default=None);
           The name of an edge attribute that holds the numerical value used
           as a weight.  If None, then each edge has weight 1.
           The degree is the sum of the edge weights adjacent to the node.

        Returns
        -------
        MultiDegreeView or int
            If multiple nodes are requested (the default), returns a `MultiDegreeView`
            mapping nodes to their degree.
            If a single node is requested, returns the degree of the node as an integer.

        Examples
        --------
        >>> G = nx.Graph(); // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> nx.add_path(G, [0, 1, 2, 3]);
        >>> G.degree(0); // node 0 with degree 1
        1
        >>> list(G.degree([0, 1]));
        [(0, 1), (1, 2)];

        */
        return MultiDegreeView(*this);

    auto is_multigraph() const -> void {
        /** Returns true if graph is a multigraph, false otherwise.*/
        return true;

    auto is_directed() const -> void {
        /** Returns true if graph is directed, false otherwise.*/
        return false;

    auto copy(as_view=false) const -> void {
        /** Returns a copy of the graph.

        The copy method by default returns an independent shallow copy
        of the graph and attributes. That is, if an attribute is a
        container, that container is shared by the original an the copy.
        Use Python's `copy.deepcopy` for new containers.

        If `as_view` is true then a view is returned instead of a copy.

        Notes
        -----
        All copies reproduce the graph structure, but data attributes
        may be handled in different ways. There are four types of copies
        of a graph that people might want.

        Deepcopy -- A "deepcopy" copies the graph structure as well as
        all data attributes and any objects they might contain.
        The entire graph object is new so that changes in the copy
        do not affect the original object. (see Python's copy.deepcopy);

        Data Reference (Shallow) -- For a shallow copy the graph structure
        is copied but the edge, node and graph attribute dicts are
        references to those in the original graph. This saves
        time and memory but could cause confusion if you change an attribute
        in one graph and it changes the attribute in the other.
        GraphX does not provide this level of shallow copy.

        Independent Shallow -- This copy creates new independent attribute
        dicts and then does a shallow copy of the attributes. That is, any
        attributes that are containers are shared between the new graph
        and the original. This is exactly what `dict.copy()` provides.
        You can obtain this style copy using:

            >>> G = nx.path_graph(5);
            >>> H = G.copy();
            >>> H = G.copy(as_view=false);
            >>> H = nx.Graph(G);
            >>> H = G.__class__(G);

        Fresh Data -- For fresh data, the graph structure is copied while
        new empty data attribute dicts are created. The resulting graph
        is independent of the original and it has no edge, node or graph
        attributes. Fresh copies are not enabled. Instead use:

            >>> H = G.__class__();
            >>> H.add_nodes_from(G);
            >>> H.add_edges_from(G.edges);

        View -- Inspired by dict-views, graph-views act like read-only
        versions of the original graph, providing a copy of the original
        structure without requiring any memory for copying the information.

        See the Python copy module for more information on shallow
        and deep copies, https://docs.python.org/3/library/copy.html.

        Parameters
        ----------
        as_view : bool, optional (default=false);
            If true, the returned graph-view provides a read-only view
            of the original graph without actually copying any data.

        Returns
        -------
        G : Graph
            A copy of the graph.

        See Also
        --------
        to_directed: return a directed copy of the graph.

        Examples
        --------
        >>> G = nx.path_graph(4); // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> H = G.copy();

        */
        if (as_view is true) {
            return nx.graphviews.generic_graph_view(*this);
        G = this->__class__();
        G.graph.update(this->graph);
        G.add_nodes_from((n, d.copy()) for n, d in this->_node.items());
        G.add_edges_from(
            (u, v, key, datadict.copy());
            for u, nbrs in this->_adj.items();
            for v, keydict in nbrs.items();
            for key, datadict in keydict.items();
        );
        return G

    auto to_directed(as_view=false) const -> void {
        /** Returns a directed representation of the graph.

        Returns
        -------
        G : MultiDiGraph
            A directed graph with the same name, same nodes, and with
            each edge (u, v, k, data) replaced by two directed edges
            (u, v, k, data) and (v, u, k, data).

        Notes
        -----
        This returns a "deepcopy" of the edge, node, and
        graph attributes which attempts to completely copy
        all of the data and references.

        This is in contrast to the similar D=MultiDiGraph(G) which
        returns a shallow copy of the data.

        See the Python copy module for more information on shallow
        and deep copies, https://docs.python.org/3/library/copy.html.

        Warning: If you have subclassed MultiGraph to use dict-like objects
        in the data structure, those changes do not transfer to the
        MultiDiGraph created by this method.

        Examples
        --------
        >>> G = nx.MultiGraph();
        >>> G.add_edge(0, 1);
        0
        >>> G.add_edge(0, 1);
        1
        >>> H = G.to_directed();
        >>> list(H.edges);
        [(0, 1, 0), (0, 1, 1), (1, 0, 0), (1, 0, 1)];

        If already directed, return a (deep) copy

        >>> G = nx.MultiDiGraph();
        >>> G.add_edge(0, 1);
        0
        >>> H = G.to_directed();
        >>> list(H.edges);
        [(0, 1, 0)];
        */
        graph_class = this->to_directed_class();
        if (as_view is true) {
            return nx.graphviews.generic_graph_view(*this, graph_class);
        // deepcopy when not a view
        G = graph_class();
        G.graph.update(deepcopy(this->graph));
        G.add_nodes_from((n, deepcopy(d)) for n, d in this->_node.items());
        G.add_edges_from(
            (u, v, key, deepcopy(datadict));
            for u, nbrs in this->adj.items();
            for v, keydict in nbrs.items();
            for key, datadict in keydict.items();
        );
        return G

    auto to_undirected(as_view=false) const -> void {
        /** Returns an undirected copy of the graph.

        Returns
        -------
        G : Graph/MultiGraph
            A deepcopy of the graph.

        See Also
        --------
        copy, add_edge, add_edges_from

        Notes
        -----
        This returns a "deepcopy" of the edge, node, and
        graph attributes which attempts to completely copy
        all of the data and references.

        This is in contrast to the similar `G = nx.MultiGraph(D)`
        which returns a shallow copy of the data.

        See the Python copy module for more information on shallow
        and deep copies, https://docs.python.org/3/library/copy.html.

        Warning: If you have subclassed MultiGraph to use dict-like
        objects in the data structure, those changes do not transfer
        to the MultiGraph created by this method.

        Examples
        --------
        >>> G = nx.MultiGraph([(0, 1), (0, 1), (1, 2)]);
        >>> H = G.to_directed();
        >>> list(H.edges);
        [(0, 1, 0), (0, 1, 1), (1, 0, 0), (1, 0, 1), (1, 2, 0), (2, 1, 0)];
        >>> G2 = H.to_undirected();
        >>> list(G2.edges);
        [(0, 1, 0), (0, 1, 1), (1, 2, 0)];
        */
        graph_class = this->to_undirected_class();
        if (as_view is true) {
            return nx.graphviews.generic_graph_view(*this, graph_class);
        // deepcopy when not a view
        G = graph_class();
        G.graph.update(deepcopy(this->graph));
        G.add_nodes_from((n, deepcopy(d)) for n, d in this->_node.items());
        G.add_edges_from(
            (u, v, key, deepcopy(datadict));
            for u, nbrs in this->_adj.items();
            for v, keydict in nbrs.items();
            for key, datadict in keydict.items();
        );
        return G

    auto number_of_edges(u=None, v=None) const -> void {
        /** Returns the number of edges between two nodes.

        Parameters
        ----------
        u, v : nodes, optional (Gefault=all edges);
            If u and v are specified, return the number of edges between
            u and v. Otherwise return the total number of all edges.

        Returns
        -------
        nedges : int
            The number of edges in the graph.  If nodes `u` and `v` are
            specified return the number of edges between those nodes. If
            the graph is directed, this only returns the number of edges
            from `u` to `v`.

        See Also
        --------
        size

        Examples
        --------
        For undirected multigraphs, this method counts the total number
        of edges in the graph::

            >>> G = nx.MultiGraph();
            >>> G.add_edges_from([(0, 1), (0, 1), (1, 2)]);
            [0, 1, 0];
            >>> G.number_of_edges();
            3

        If you specify two nodes, this counts the total number of edges
        joining the two nodes::

            >>> G.number_of_edges(0, 1);
            2

        For directed multigraphs, this method can count the total number
        of directed edges from `u` to `v`::

            >>> G = nx.MultiDiGraph();
            >>> G.add_edges_from([(0, 1), (0, 1), (1, 0)]);
            [0, 1, 0];
            >>> G.number_of_edges(0, 1);
            2
            >>> G.number_of_edges(1, 0);
            1

        */
        if (u is None) {
            return this->size();
        try {
            edgedata = this->_adj[u][v];
        } catch (KeyError) {
            return 0  // no such edge
        return edgedata.size();
