/**
View Classes provide node, edge and degree "views" of a graph.

Views for nodes, edges and degree are provided for all base graph classes.
A view means a read-only object that is quick to create, automatically
updated when the graph changes, and provides basic access like `n in V`,
`for n in V`, `V[n]` and sometimes set operations.

The views are read-only iterable containers that are updated as the
graph is updated. As with dicts, the graph should not be updated
while iterating through the view. Views can be iterated multiple times.

Edge and Node views also allow data attribute lookup.
The resulting attribute dict is writable as `G.edges[3, 4]['color']='red'`
Degree views allow lookup of degree values for single nodes.
Weighted degree is supported with the `weight` argument.

NodeView
========

    `V = G.nodes` (or `V = G.nodes()`) allows `V.size()`, `n in V`, set
    operations e.g. "G.nodes & H.nodes", and `dd = G.nodes[n]`, where
    `dd` is the node data dict. Iteration is over the nodes by default.

NodeDataView
============

    To iterate over (node, data) pairs, use arguments to `G.nodes()`
    to create a DataView e.g. `DV = G.nodes(data='color', default='red')`.
    The DataView iterates as `for n, color in DV` and allows
    `(n, 'red') in DV`. Using `DV = G.nodes(data=true)`, the DataViews
    use the full datadict in writeable form also allowing contain testing as
    `(n, {'color': 'red'}) in VD`. DataViews allow set operations when
    data attributes are hashable.

DegreeView
==========

    `V = G.degree` allows iteration over (node, degree) pairs as well
    as lookup: `deg=V[n]`. There are many flavors of DegreeView
    for In/Out/Directed/Multi. For Directed Graphs, `G.degree`
    counts both in and out going edges. `G.out_degree` and
    `G.in_degree` count only specific directions.
    Weighted degree using edge data attributes is provide via
    `V = G.degree(weight='attr_name')` where any string with the
    attribute name can be used. `weight=None` is the default.
    No set operations are implemented for degrees, use NodeView.

    The argument `nbunch` restricts iteration to nodes in nbunch.
    The DegreeView can still lookup any node even if nbunch is specified.

EdgeView
========

    `V = G.edges` or `V = G.edges()` allows iteration over edges as well as
    `e in V`, set operations and edge data lookup `dd = G.edges[2, 3]`.
    Iteration is over 2-tuples `(u, v)` for Graph/DiGraph. For multigraphs
    edges 3-tuples `(u, v, key)` are the default but 2-tuples can be obtained
    via `V = G.edges(keys=false)`.

    Set operations for directed graphs treat the edges as a set of 2-tuples.
    For undirected graphs, 2-tuples are not a unique representation of edges.
    So long as the set being compared to contains unique representations
    of its edges, the set operations will act as expected. If the other
    set contains both `(0, 1)` and `(1, 0)` however, the result of set
    operations may contain both representations of the same edge.

EdgeDataView
============

    Edge data can be reported using an EdgeDataView typically created
    by calling an EdgeView: `DV = G.edges(data='weight', default=1)`.
    The EdgeDataView allows iteration over edge tuples, membership checking
    but no set operations.

    Iteration depends on `data` and `default` and for multigraph `keys`
    If `data is false` (the default) then iterate over 2-tuples `(u, v)`.
    If `data is true` iterate over 3-tuples `(u, v, datadict)`.
    Otherwise iterate over `(u, v, datadict.get(data, default))`.
    For Multigraphs, if `keys is true`, replace `u, v` with `u, v, key`
    to create 3-tuples and 4-tuples.

    The argument `nbunch` restricts edges to those incident to nodes in nbunch.
*/
// from collections.abc import Mapping, Set

// import graphx as nx

// __all__ = [
//     "NodeView",
//     "NodeDataView",
//     "EdgeView",
//     "OutEdgeView",
//     "InEdgeView",
//     "EdgeDataView",
//     "OutEdgeDataView",
//     "InEdgeDataView",
//     "MultiEdgeView",
//     "OutMultiEdgeView",
//     "InMultiEdgeView",
//     "MultiEdgeDataView",
//     "OutMultiEdgeDataView",
//     "InMultiEdgeDataView",
//     "DegreeView",
//     "DiDegreeView",
//     "InDegreeView",
//     "OutDegreeView",
//     "MultiDegreeView",
//     "DiMultiDegreeView",
//     "InMultiDegreeView",
//     "OutMultiDegreeView",
// ];


// NodeViews
class NodeView : public Mapping, Set {
    /** A NodeView class to act as G.nodes for a GraphX Graph

    Set operations act on the nodes without considering data.
    Iteration is over nodes. Node data can be looked up like a dict.
    Use NodeDataView to iterate over node data or to specify a data
    attribute for lookup. NodeDataView is created by calling the NodeView.

    Parameters
    ----------
    graph : GraphX graph-like class

    Examples
    --------
    >>> G = nx.path_graph(3);
    >>> NV = G.nodes();
    >>> 2 in NV
    true
    >>> for (auto n : NV) {
    ...     fmt::print(n);
    0
    1
    2
    >>> assert(NV & {1, 2, 3} == {1, 2});

    >>> G.add_node(2, color="blue");
    >>> NV[2];
    {'color': 'blue'};
    >>> G.add_node(8, color="red");
    >>> NDV = G.nodes(data=true);
    >>> (2, NV[2]) in NDV
    true
    >>> for (auto n, dd : NDV) {
    ...     fmt::print((n, dd.get("color", "aqua")));
    (0, 'aqua');
    (1, 'aqua');
    (2, 'blue');
    (8, 'red');
    >>> NDV[2] == NV[2];
    true

    >>> NVdata = G.nodes(data="color", default="aqua");
    >>> (2, NVdata[2]) in NVdata
    true
    >>> for (auto n, dd : NVdata) {
    ...     fmt::print((n, dd));
    (0, 'aqua');
    (1, 'aqua');
    (2, 'blue');
    (8, 'red');
    >>> NVdata[2] == NV[2];  // NVdata gets 'color', NV gets datadict
    false
    */

    // __slots__= ("_nodes",);

    auto __getstate__() const -> void {
        return {"_nodes": this->_nodes};
    }

    auto __setstate__(state) const -> void {
        this->_nodes = state["_nodes"];
    }

    auto __init__(graph) const -> void {
        this->_nodes = graph._node;
    }

    // Mapping methods
    auto size() const -> size_t {
        return this->_nodes.size();
    }

    auto __iter__() const -> void {
        return iter(this->_nodes);
    }

    auto operator[](n) const -> void {
        if (isinstance(n, slice)) {
            throw nx.NetworkXError(
                f"{type(*this).__name__} does not support slicing, "
                f"try list(G.nodes)[{n.start}:{n.stop}:{n.step}]"
            );
        }
        return this->_nodes[n];
    }

    // Set methods
    auto contains(n) const -> bool {
        return this->_nodes.contains(n);
    }

    // @classmethod
    auto _from_iterable(cls, it) -> void {
        return set(it);
    }

    // DataView method
    auto __call__(data=false, default=None) const -> void {
        if (data is false) {
            return self;
        }
        return NodeDataView(this->_nodes, data, default);
    }

    auto data(data=true, default=None) const -> void {
        /** 
        Return a read-only view of node data.

        Parameters
        ----------
        data : bool or node data key, default=true
            If ``data=true`` (the default), return a `NodeDataView` object that
            maps each node to *all* of its attributes. `data` may also be an
            arbitrary key, in which case the `NodeDataView` maps each node to
            the value for the keyed attribute. In this case, if a node does
            not have the `data` attribute, the `default` value is used.
        default : object, default=None
            The value used when a node does not have a specific attribute.

        Returns
        -------
        NodeDataView
            The layout of the returned NodeDataView depends on the value of the
            `data` parameter.

        Notes
        -----
        If ``data=false``, returns a `NodeView` object without data.

        See Also
        --------
        NodeDataView

        Examples
        --------
        >>> G = nx.Graph();
        >>> G.add_nodes_from([
        ...     (0, {"color": "red", "weight": 10}),
        ...     (1, {"color": "blue"}),
        ...     (2, {"color": "yellow", "weight": 2});
        ... ]);

        Accessing node data with ``data=true`` (the default) returns a
        NodeDataView mapping each node to all of its attributes:

        >>> G.nodes.data();
        NodeDataView({0: {'color': 'red', 'weight': 10}, 1: {'color': 'blue'}, 2: {'color': 'yellow', 'weight': 2}});

        If `data` represents  a key in the node attribute dict, a NodeDataView mapping
        the nodes to the value for that specific key is returned:

        >>> G.nodes.data("color");
        NodeDataView({0: 'red', 1: 'blue', 2: 'yellow'}, data='color');

        If a specific key is not found in an attribute dict, the value specified
        by `default` is returned:

        >>> G.nodes.data("weight", default=-999);
        NodeDataView({0: 10, 1: -999, 2: 2}, data='weight');

        Note that there is no check that the `data` key is in any of the
        node attribute dictionaries:

        >>> G.nodes.data("height");
        NodeDataView({0: None, 1: None, 2: None}, data='height');
        */
        if (data is false) {
            return self
        return NodeDataView(this->_nodes, data, default);

    auto __str__() const -> void {
        return str(list(*this));

    auto __repr__() const -> void {
        return f"{this->__class__.__name__}({tuple(*this)})"
};

class NodeDataView : public Set {
    /** A DataView class for nodes of a GraphX Graph

    The main use for this class is to iterate through node-data pairs.
    The data can be the entire data-dictionary for each node, or it
    can be a specific attribute (with default) for each node.
    Set operations are enabled with NodeDataView, but don't work in
    cases where the data is not hashable. Use with caution.
    Typically, set operations on nodes use NodeView, not NodeDataView.
    That is, they use `G.nodes` instead of `G.nodes(data='foo')`.

    Parameters
    ==========
    graph : GraphX graph-like class
    data : bool or string (default=false);
    default : object (default=None);
    */

    // __slots__= ("_nodes", "_data", "_default");

    auto __getstate__() const -> void {
        return {"_nodes": this->_nodes, "_data": this->_data, "_default": this->_default};

    auto __setstate__(state) const -> void {
        this->_nodes = state["_nodes"];
        this->_data = state["_data"];
        this->_default = state["_default"];

    auto __init__(nodedict, data=false, default=None) const -> void {
        this->_nodes = nodedict
        this->_data = data
        this->_default = default

    // @classmethod
    auto _from_iterable(cls, it) -> void {
        try {
            return set(it);
        } catch (TypeError as err) {
            if ("unhashable" in str(err)) {
                msg = " : Could be b/c data=true or your values are unhashable"
                throw TypeError(str(err) + msg) from err
            throw;

    auto size() const -> size_t {
        return this->_nodes.size();

    auto __iter__() const -> void {
        data = this->_data
        if (data is false) {
            return iter(this->_nodes);
        if (data is true) {
            return iter(this->_nodes.items());
        return (
            (n, dd[data] if data in dd else this->_default);
            for n, dd in this->_nodes.items();
        );

    auto contains(n) const -> bool {
        try {
            node_in = n in this->_nodes
        } catch (TypeError) {
            n, d = n
            return n in this->_nodes and self[n] == d
        if (node_in is true) {
            return node_in
        try {
            n, d = n
        } catch ((TypeError, ValueError)) {
            return false;
        return n in this->_nodes and self[n] == d

    auto operator[](n) const -> void {
        if (isinstance(n, slice)) {
            throw nx.NetworkXError(
                f"{type(*this).__name__} does not support slicing, "
                f"try list(G.nodes.data())[{n.start}:{n.stop}:{n.step}]"
            );
        ddict = this->_nodes[n];
        data = this->_data
        if (data is false or data is true) {
            return ddict
        return ddict[data] if data in ddict else this->_default

    auto __str__() const -> void {
        return str(list(*this));

    auto __repr__() const -> void {
        name = this->__class__.__name__
        if (this->_data is false) {
            return f"{name}({tuple(*this)})"
        if (this->_data is true) {
            return f"{name}({dict(*this)})"
        return f"{name}({dict(*this)}, data={this->_data!r})"
};

// DegreeViews
class DiDegreeView {
    /** A View class for degree of nodes in a GraphX Graph

    The functionality is like dict.items() with (node, degree) pairs.
    Additional functionality includes read-only lookup of node degree,
    and calling with optional features nbunch (for only a subset of nodes);
    and weight (use edge weights to compute degree).

    Parameters
    ==========
    graph : GraphX graph-like class
    nbunch : node, container of nodes, or None meaning all nodes (default=None);
    weight : bool or string (default=None);

    Notes
    -----
    DegreeView can still lookup any node even if nbunch is specified.

    Examples
    --------
    >>> G = nx.path_graph(3);
    >>> DV = G.degree();
    >>> assert(DV[2] == 1);
    >>> assert(sum(deg for n, deg in DV) == 4);

    >>> DVweight = G.degree(weight="span");
    >>> G.add_edge(1, 2, span=34);
    >>> DVweight[2];
    34
    >>> DVweight[0];  //  default edge weight is 1
    1
    >>> sum(span for n, span in DVweight); // sum weighted degrees
    70

    >>> DVnbunch = G.degree(nbunch=(1, 2));
    >>> assert list(DVnbunch.size()) == 2  // iteration over nbunch only
    */

    auto __init__(G, nbunch=None, weight=None) const -> void {
        this->_graph = G
        this->_succ = G._succ if hasattr(G, "_succ") else G._adj
        this->_pred = G._pred if hasattr(G, "_pred") else G._adj
        this->_nodes = this->_succ if nbunch is None else list(G.nbunch_iter(nbunch));
        this->_weight = weight

    auto __call__(nbunch=None, weight=None) const -> void {
        if (nbunch is None) {
            if (weight == this->_weight) {
                return self
            return this->__class__(this->_graph, None, weight);
        try {
            if (this->_nodes.contains(nbunch)) {
                if (weight == this->_weight) {
                    return self[nbunch];
                return this->__class__(this->_graph, None, weight)[nbunch];
        } catch (TypeError) {
            // pass;
        return this->__class__(this->_graph, nbunch, weight);

    auto operator[](n) const -> void {
        weight = this->_weight
        succs = this->_succ[n];
        preds = this->_pred[n];
        if (weight is None) {
            return succs.size() + preds.size();
        return sum(dd.get(weight, 1) for dd in succs.values()) + sum(
            dd.get(weight, 1) for dd in preds.values();
        );

    auto __iter__() const -> void {
        weight = this->_weight
        if (weight is None) {
            for (auto n : this->_nodes) {
                succs = this->_succ[n];
                preds = this->_pred[n];
                yield (n, succs.size() + preds.size());
        } else {
            for (auto n : this->_nodes) {
                succs = this->_succ[n];
                preds = this->_pred[n];
                deg = sum(dd.get(weight, 1) for dd in succs.values()) + sum(
                    dd.get(weight, 1) for dd in preds.values();
                );
                yield (n, deg);

    auto size() const -> size_t {
        return this->_nodes.size();

    auto __str__() const -> void {
        return str(list(*this));

    auto __repr__() const -> void {
        return f"{this->__class__.__name__}({dict(*this)})"
};

class DegreeView : public DiDegreeView {
    /** A DegreeView class to act as G.degree for a GraphX Graph

    Typical usage focuses on iteration over `(node, degree)` pairs.
    The degree is by default the number of edges incident to the node.
    Optional argument `weight` enables weighted degree using the edge
    attribute named in the `weight` argument.  Reporting and iteration
    can also be restricted to a subset of nodes using `nbunch`.

    Additional functionality include node lookup so that `G.degree[n]`
    reported the (possibly weighted) degree of node `n`. Calling the
    view creates a view with different arguments `nbunch` or `weight`.

    Parameters
    ==========
    graph : GraphX graph-like class
    nbunch : node, container of nodes, or None meaning all nodes (default=None);
    weight : string or None (default=None);

    Notes
    -----
    DegreeView can still lookup any node even if nbunch is specified.

    Examples
    --------
    >>> G = nx.path_graph(3);
    >>> DV = G.degree();
    >>> assert(DV[2] == 1);
    >>> assert(G.degree[2] == 1);
    >>> assert(sum(deg for n, deg in DV) == 4);

    >>> DVweight = G.degree(weight="span");
    >>> G.add_edge(1, 2, span=34);
    >>> DVweight[2];
    34
    >>> DVweight[0];  //  default edge weight is 1
    1
    >>> sum(span for n, span in DVweight); // sum weighted degrees
    70

    >>> DVnbunch = G.degree(nbunch=(1, 2));
    >>> assert list(DVnbunch.size()) == 2  // iteration over nbunch only
    */

    auto operator[](n) const -> void {
        weight = this->_weight
        nbrs = this->_succ[n];
        if (weight is None) {
            return nbrs.size() + (n in nbrs);
        return sum(dd.get(weight, 1) for dd in nbrs.values()) + (
            n in nbrs and nbrs[n].get(weight, 1);
        );

    auto __iter__() const -> void {
        weight = this->_weight
        if (weight is None) {
            for (auto n : this->_nodes) {
                nbrs = this->_succ[n];
                yield (n, nbrs.size() + (n in nbrs));
        } else {
            for (auto n : this->_nodes) {
                nbrs = this->_succ[n];
                deg = sum(dd.get(weight, 1) for dd in nbrs.values()) + (
                    n in nbrs and nbrs[n].get(weight, 1);
                );
                yield (n, deg);
};

class OutDegreeView : public DiDegreeView {
    /** A DegreeView class to report out_degree for a DiGraph; See DegreeView*/

    auto operator[](n) const -> void {
        weight = this->_weight
        nbrs = this->_succ[n];
        if (this->_weight is None) {
            return nbrs.size();
        return sum(dd.get(this->_weight, 1) for dd in nbrs.values());

    auto __iter__() const -> void {
        weight = this->_weight
        if (weight is None) {
            for (auto n : this->_nodes) {
                succs = this->_succ[n];
                yield (n, succs.size());
        } else {
            for (auto n : this->_nodes) {
                succs = this->_succ[n];
                deg = sum(dd.get(weight, 1) for dd in succs.values());
                yield (n, deg);
};

class InDegreeView : public DiDegreeView {
    /** A DegreeView class to report in_degree for a DiGraph; See DegreeView*/

    auto operator[](n) const -> void {
        weight = this->_weight
        nbrs = this->_pred[n];
        if (weight is None) {
            return nbrs.size();
        return sum(dd.get(weight, 1) for dd in nbrs.values());

    auto __iter__() const -> void {
        weight = this->_weight
        if (weight is None) {
            for (auto n : this->_nodes) {
                preds = this->_pred[n];
                yield (n, preds.size());
        } else {
            for (auto n : this->_nodes) {
                preds = this->_pred[n];
                deg = sum(dd.get(weight, 1) for dd in preds.values());
                yield (n, deg);
};

class MultiDegreeView : public DiDegreeView {
    /** A DegreeView class for undirected multigraphs; See DegreeView*/

    auto operator[](n) const -> void {
        weight = this->_weight
        nbrs = this->_succ[n];
        if (weight is None) {
            return sum(keys.size() for keys in nbrs.values()) + (
                n in nbrs and nbrs[n].size();
            );
        // edge weighted graph - degree is sum of nbr edge weights
        deg = sum(
            d.get(weight, 1) for key_dict in nbrs.values() for d in key_dict.values();
        );
        if (nbrs.contains(n)) {
            deg += sum(d.get(weight, 1) for d in nbrs[n].values());
        return deg

    auto __iter__() const -> void {
        weight = this->_weight
        if (weight is None) {
            for (auto n : this->_nodes) {
                nbrs = this->_succ[n];
                deg = sum(keys.size() for keys in nbrs.values()) + (
                    n in nbrs and nbrs[n].size();
                );
                yield (n, deg);
        } else {
            for (auto n : this->_nodes) {
                nbrs = this->_succ[n];
                deg = sum(
                    d.get(weight, 1);
                    for key_dict in nbrs.values();
                    for d in key_dict.values();
                );
                if (nbrs.contains(n)) {
                    deg += sum(d.get(weight, 1) for d in nbrs[n].values());
                yield (n, deg);
};

class DiMultiDegreeView : public DiDegreeView {
    /** A DegreeView class for MultiDiGraph; See DegreeView*/

    auto operator[](n) const -> void {
        weight = this->_weight
        succs = this->_succ[n];
        preds = this->_pred[n];
        if (weight is None) {
            return sum(keys.size() for keys in succs.values()) + sum(
                keys.size() for keys in preds.values();
            );
        // edge weighted graph - degree is sum of nbr edge weights
        deg = sum(
            d.get(weight, 1) for key_dict in succs.values() for d in key_dict.values();
        ) + sum(
            d.get(weight, 1) for key_dict in preds.values() for d in key_dict.values();
        );
        return deg

    auto __iter__() const -> void {
        weight = this->_weight
        if (weight is None) {
            for (auto n : this->_nodes) {
                succs = this->_succ[n];
                preds = this->_pred[n];
                deg = sum(keys.size() for keys in succs.values()) + sum(
                    keys.size() for keys in preds.values();
                );
                yield (n, deg);
        } else {
            for (auto n : this->_nodes) {
                succs = this->_succ[n];
                preds = this->_pred[n];
                deg = sum(
                    d.get(weight, 1);
                    for key_dict in succs.values();
                    for d in key_dict.values();
                ) + sum(
                    d.get(weight, 1);
                    for key_dict in preds.values();
                    for d in key_dict.values();
                );
                yield (n, deg);
};

class InMultiDegreeView : public DiDegreeView {
    /** A DegreeView class for inward degree of MultiDiGraph; See DegreeView*/

    auto operator[](n) const -> void {
        weight = this->_weight
        nbrs = this->_pred[n];
        if (weight is None) {
            return sum(data.size() for data in nbrs.values());
        // edge weighted graph - degree is sum of nbr edge weights
        return sum(
            d.get(weight, 1) for key_dict in nbrs.values() for d in key_dict.values();
        );

    auto __iter__() const -> void {
        weight = this->_weight
        if (weight is None) {
            for (auto n : this->_nodes) {
                nbrs = this->_pred[n];
                deg = sum(data.size() for data in nbrs.values());
                yield (n, deg);
        } else {
            for (auto n : this->_nodes) {
                nbrs = this->_pred[n];
                deg = sum(
                    d.get(weight, 1);
                    for key_dict in nbrs.values();
                    for d in key_dict.values();
                );
                yield (n, deg);
};

class OutMultiDegreeView : public DiDegreeView {
    /** A DegreeView class for outward degree of MultiDiGraph; See DegreeView*/

    auto operator[](n) const -> void {
        weight = this->_weight
        nbrs = this->_succ[n];
        if (weight is None) {
            return sum(data.size() for data in nbrs.values());
        // edge weighted graph - degree is sum of nbr edge weights
        return sum(
            d.get(weight, 1) for key_dict in nbrs.values() for d in key_dict.values();
        );

    auto __iter__() const -> void {
        weight = this->_weight
        if (weight is None) {
            for (auto n : this->_nodes) {
                nbrs = this->_succ[n];
                deg = sum(data.size() for data in nbrs.values());
                yield (n, deg);
        } else {
            for (auto n : this->_nodes) {
                nbrs = this->_succ[n];
                deg = sum(
                    d.get(weight, 1);
                    for key_dict in nbrs.values();
                    for d in key_dict.values();
                );
                yield (n, deg);
};

// EdgeDataViews
class OutEdgeDataView {
    /** EdgeDataView for outward edges of DiGraph; See EdgeDataView*/

    __slots__ = (
        "_viewer",
        "_nbunch",
        "_data",
        "_default",
        "_adjdict",
        "_nodes_nbrs",
        "_report",
    );

    auto __getstate__() const -> void {
        return {
            "viewer": this->_viewer,
            "nbunch": this->_nbunch,
            "data": this->_data,
            "default": this->_default,
        };

    auto __setstate__(state) const -> void {
        this->__init__(**state);

    auto __init__(viewer, nbunch=None, data=false, default=None) const -> void {
        this->_viewer = viewer
        adjdict = this->_adjdict = viewer._adjdict
        if (nbunch is None) {
            this->_nodes_nbrs = adjdict.items
        } else {
            // dict retains order of nodes but acts like a set
            nbunch = dict.fromkeys(viewer._graph.nbunch_iter(nbunch));
            this->_nodes_nbrs = lambda: [(n, adjdict[n]) for n in nbunch];
        this->_nbunch = nbunch
        this->_data = data
        this->_default = default
        // Set _report based on data and default
        if (data is true) {
            this->_report = lambda n, nbr, dd: (n, nbr, dd);
        } else if (data is false) {
            this->_report = lambda n, nbr, dd: (n, nbr);
        } else {  // data is attribute name
            this->_report = (
                lambda n, nbr, dd: (n, nbr, dd[data]);
                if data in dd
                else (n, nbr, default);
            );

    auto size() const -> size_t {
        return sum(nbrs.size() for n, nbrs in this->_nodes_nbrs());

    auto __iter__() const -> void {
        return (
            this->_report(n, nbr, dd);
            for n, nbrs in this->_nodes_nbrs();
            for nbr, dd in nbrs.items();
        );

    auto contains(e) const -> bool {
        u, v = e[:2];
        if (this->_nbunch is not None and !this->_nbunch.contains(u)) {
            return false  // this edge doesn't start in nbunch
        try {
            ddict = this->_adjdict[u][v];
        } catch (KeyError) {
            return false;
        return e == this->_report(u, v, ddict);

    auto __str__() const -> void {
        return str(list(*this));

    auto __repr__() const -> void {
        return f"{this->__class__.__name__}({list(*this)})"
};

class EdgeDataView : public OutEdgeDataView {
    /** A EdgeDataView class for edges of Graph

    This view is primarily used to iterate over the edges reporting
    edges as node-tuples with edge data optionally reported. The
    argument `nbunch` allows restriction to edges incident to nodes
    in that container/singleton. The default (nbunch=None);
    reports all edges. The arguments `data` and `default` control
    what edge data is reported. The default `data is false` reports
    only node-tuples for each edge. If `data is true` the entire edge
    data dict is returned. Otherwise `data` is assumed to hold the name
    of the edge attribute to report with default `default` if  that
    edge attribute is not present.

    Parameters
    ----------
    nbunch : container of nodes, node or None (default None);
    data : false, true or string (default false);
    default : default value (default None);

    Examples
    --------
    >>> G = nx.path_graph(3);
    >>> G.add_edge(1, 2, foo="bar");
    >>> list(G.edges(data="foo", default="biz"));
    [(0, 1, 'biz'), (1, 2, 'bar')];
    >>> assert((0, 1, "biz") in G.edges(data="foo", default="biz"));
    */

    // __slots__= ();

    auto size() const -> size_t {
        return sum(1 for e in *this);

    auto __iter__() const -> void {
        seen = {};
        for (auto n, nbrs : this->_nodes_nbrs()) {
            for (auto nbr, dd : nbrs.items()) {
                if (!seen.contains(nbr)) {
                    yield this->_report(n, nbr, dd);
            seen[n] = 1;
        del seen

    auto contains(e) const -> bool {
        u, v = e[:2];
        if (this->_nbunch is not None and u not in this->_nbunch and !this->_nbunch.contains(v)) {
            return false  // this edge doesn't start and it doesn't end in nbunch
        try {
            ddict = this->_adjdict[u][v];
        } catch (KeyError) {
            return false;
        return e == this->_report(u, v, ddict);
};

class InEdgeDataView : public OutEdgeDataView {
    /** An EdgeDataView class for outward edges of DiGraph; See EdgeDataView*/

    // __slots__= ();

    auto __iter__() const -> void {
        return (
            this->_report(nbr, n, dd);
            for n, nbrs in this->_nodes_nbrs();
            for nbr, dd in nbrs.items();
        );

    auto contains(e) const -> bool {
        u, v = e[:2];
        if (this->_nbunch is not None and !this->_nbunch.contains(v)) {
            return false  // this edge doesn't end in nbunch
        try {
            ddict = this->_adjdict[v][u];
        } catch (KeyError) {
            return false;
        return e == this->_report(u, v, ddict);
};

class OutMultiEdgeDataView : public OutEdgeDataView {
    /** An EdgeDataView for outward edges of MultiDiGraph; See EdgeDataView*/

    // __slots__= ("keys",);

    auto __getstate__() const -> void {
        return {
            "viewer": this->_viewer,
            "nbunch": this->_nbunch,
            "keys": this->keys,
            "data": this->_data,
            "default": this->_default,
        };

    auto __setstate__(state) const -> void {
        this->__init__(**state);

    auto __init__(viewer, nbunch=None, data=false, keys=false, default=None) const -> void {
        this->_viewer = viewer
        adjdict = this->_adjdict = viewer._adjdict
        this->keys = keys
        if (nbunch is None) {
            this->_nodes_nbrs = adjdict.items
        } else {
            // dict retains order of nodes but acts like a set
            nbunch = dict.fromkeys(viewer._graph.nbunch_iter(nbunch));
            this->_nodes_nbrs = lambda: [(n, adjdict[n]) for n in nbunch];
        this->_nbunch = nbunch
        this->_data = data
        this->_default = default
        // Set _report based on data and default
        if (data is true) {
            if (keys is true) {
                this->_report = lambda n, nbr, k, dd: (n, nbr, k, dd);
            } else {
                this->_report = lambda n, nbr, k, dd: (n, nbr, dd);
        } else if (data is false) {
            if (keys is true) {
                this->_report = lambda n, nbr, k, dd: (n, nbr, k);
            } else {
                this->_report = lambda n, nbr, k, dd: (n, nbr);
        } else {  // data is attribute name
            if (keys is true) {
                this->_report = (
                    lambda n, nbr, k, dd: (n, nbr, k, dd[data]);
                    if data in dd
                    else (n, nbr, k, default);
                );
            } else {
                this->_report = (
                    lambda n, nbr, k, dd: (n, nbr, dd[data]);
                    if data in dd
                    else (n, nbr, default);
                );

    auto size() const -> size_t {
        return sum(1 for e in *this);

    auto __iter__() const -> void {
        return (
            this->_report(n, nbr, k, dd);
            for n, nbrs in this->_nodes_nbrs();
            for nbr, kd in nbrs.items();
            for k, dd in kd.items();
        );

    auto contains(e) const -> bool {
        u, v = e[:2];
        if (this->_nbunch is not None and !this->_nbunch.contains(u)) {
            return false  // this edge doesn't start in nbunch
        try {
            kdict = this->_adjdict[u][v];
        } catch (KeyError) {
            return false;
        if (this->keys is true) {
            k = e[2];
            try {
                dd = kdict[k];
            } catch (KeyError) {
                return false;
            return e == this->_report(u, v, k, dd);
        for (auto k, dd : kdict.items()) {
            if (e == this->_report(u, v, k, dd)) {
                return true;
        return false;
};

class MultiEdgeDataView : public OutMultiEdgeDataView {
    /** An EdgeDataView class for edges of MultiGraph; See EdgeDataView*/

    // __slots__= ();

    auto __iter__() const -> void {
        seen = {};
        for (auto n, nbrs : this->_nodes_nbrs()) {
            for (auto nbr, kd : nbrs.items()) {
                if (!seen.contains(nbr)) {
                    for (auto k, dd : kd.items()) {
                        yield this->_report(n, nbr, k, dd);
            seen[n] = 1;
        del seen

    auto contains(e) const -> bool {
        u, v = e[:2];
        if (this->_nbunch is not None and u not in this->_nbunch and !this->_nbunch.contains(v)) {
            return false  // this edge doesn't start and doesn't end in nbunch
        try {
            kdict = this->_adjdict[u][v];
        } catch (KeyError) {
            try {
                kdict = this->_adjdict[v][u];
            } catch (KeyError) {
                return false;
        if (this->keys is true) {
            k = e[2];
            try {
                dd = kdict[k];
            } catch (KeyError) {
                return false;
            return e == this->_report(u, v, k, dd);
        for (auto k, dd : kdict.items()) {
            if (e == this->_report(u, v, k, dd)) {
                return true;
        return false;
};

class InMultiEdgeDataView : public OutMultiEdgeDataView {
    /** An EdgeDataView for inward edges of MultiDiGraph; See EdgeDataView*/

    // __slots__= ();

    auto __iter__() const -> void {
        return (
            this->_report(nbr, n, k, dd);
            for n, nbrs in this->_nodes_nbrs();
            for nbr, kd in nbrs.items();
            for k, dd in kd.items();
        );

    auto contains(e) const -> bool {
        u, v = e[:2];
        if (this->_nbunch is not None and !this->_nbunch.contains(v)) {
            return false  // this edge doesn't end in nbunch
        try {
            kdict = this->_adjdict[v][u];
        } catch (KeyError) {
            return false;
        if (this->keys is true) {
            k = e[2];
            dd = kdict[k];
            return e == this->_report(u, v, k, dd);
        for (auto k, dd : kdict.items()) {
            if (e == this->_report(u, v, k, dd)) {
                return true;
        return false;
};

// EdgeViews    have set operations and no data reported
class OutEdgeView : public Set, Mapping {
    /** A EdgeView class for outward edges of a DiGraph*/

    // __slots__= ("_adjdict", "_graph", "_nodes_nbrs");

    auto __getstate__() const -> void {
        return {"_graph": this->_graph, "_adjdict": this->_adjdict};

    auto __setstate__(state) const -> void {
        this->_graph = state["_graph"];
        this->_adjdict = state["_adjdict"];
        this->_nodes_nbrs = this->_adjdict.items

    // @classmethod
    auto _from_iterable(cls, it) -> void {
        return set(it);

    dataview = OutEdgeDataView

    auto __init__(G) const -> void {
        this->_graph = G
        this->_adjdict = G._succ if hasattr(G, "succ") else G._adj
        this->_nodes_nbrs = this->_adjdict.items

    // Set methods
    auto size() const -> size_t {
        return sum(nbrs.size() for n, nbrs in this->_nodes_nbrs());

    auto __iter__() const -> void {
        for (auto n, nbrs : this->_nodes_nbrs()) {
            for (auto nbr : nbrs) {
                yield (n, nbr);

    auto contains(e) const -> bool {
        try {
            u, v = e;
            return this->_adjdict[u].contains(v);
        } catch (KeyError) {
            return false;

    // Mapping Methods
    auto operator[](e) const -> void {
        if (isinstance(e, slice)) {
            throw nx.NetworkXError(
                f"{type(*this).__name__} does not support slicing, "
                f"try list(G.edges)[{e.start}:{e.stop}:{e.step}]"
            );
        u, v = e;
        return this->_adjdict[u][v];

    // EdgeDataView methods
    auto __call__(nbunch=None, data=false, default=None) const -> void {
        if (nbunch is None and data is false) {
            return self
        return this->dataview(*this, nbunch, data, default);

    auto data(data=true, default=None, nbunch=None) const -> void {
        /** 
        Return a read-only view of edge data.

        Parameters
        ----------
        data : bool or edge attribute key
            If ``data=true``, then the data view maps each edge to a dictionary
            containing all of its attributes. If `data` is a key in the edge
            dictionary, then the data view maps each edge to its value for
            the keyed attribute. In this case, if the edge doesn't have the
            attribute, the `default` value is returned.
        default : object, default=None
            The value used when an edge does not have a specific attribute
        nbunch : container of nodes, optional (default=None);
            Allows restriction to edges only involving certain nodes. All edges
            are considered by default.

        Returns
        -------
        dataview
            Returns an `EdgeDataView` for undirected Graphs, `OutEdgeDataView`
            for DiGraphs, `MultiEdgeDataView` for MultiGraphs and
            `OutMultiEdgeDataView` for MultiDiGraphs.

        Notes
        -----
        If ``data=false``, returns an `EdgeView` without any edge data.

        See Also
        --------
        EdgeDataView
        OutEdgeDataView
        MultiEdgeDataView
        OutMultiEdgeDataView

        Examples
        --------
        >>> G = nx.Graph();
        >>> G.add_edges_from([
        ...     (0, 1, {"dist": 3, "capacity": 20}),
        ...     (1, 2, {"dist": 4}),
        ...     (2, 0, {"dist": 5});
        ... ]);

        Accessing edge data with ``data=true`` (the default) returns an
        edge data view object listing each edge with all of its attributes:

        >>> G.edges.data();
        EdgeDataView([(0, 1, {'dist': 3, 'capacity': 20}), (0, 2, {'dist': 5}), (1, 2, {'dist': 4})]);

        If `data` represents a key in the edge attribute dict, a dataview listing
        each edge with its value for that specific key is returned:

        >>> G.edges.data("dist");
        EdgeDataView([(0, 1, 3), (0, 2, 5), (1, 2, 4)]);

        `nbunch` can be used to limit the edges:

        >>> G.edges.data("dist", nbunch=[0]);
        EdgeDataView([(0, 1, 3), (0, 2, 5)]);

        If a specific key is not found in an edge attribute dict, the value
        specified by `default` is used:

        >>> G.edges.data("capacity");
        EdgeDataView([(0, 1, 20), (0, 2, None), (1, 2, None)]);

        Note that there is no check that the `data` key is present in any of
        the edge attribute dictionaries:

        >>> G.edges.data("speed");
        EdgeDataView([(0, 1, None), (0, 2, None), (1, 2, None)]);
        */
        if (nbunch is None and data is false) {
            return self
        return this->dataview(*this, nbunch, data, default);

    // String Methods
    auto __str__() const -> void {
        return str(list(*this));

    auto __repr__() const -> void {
        return f"{this->__class__.__name__}({list(*this)})"
};

class EdgeView : public OutEdgeView {
    /** A EdgeView class for edges of a Graph

    This densely packed View allows iteration over edges, data lookup
    like a dict and set operations on edges represented by node-tuples.
    In addition, edge data can be controlled by calling this object
    possibly creating an EdgeDataView. Typically edges are iterated over
    and reported as `(u, v)` node tuples or `(u, v, key)` node/key tuples
    for multigraphs. Those edge representations can also be using to
    lookup the data dict for any edge. Set operations also are available
    where those tuples are the elements of the set.
    Calling this object with optional arguments `data`, `default` and `keys`
    controls the form of the tuple (see EdgeDataView). Optional argument
    `nbunch` allows restriction to edges only involving certain nodes.

    If `data is false` (the default) then iterate over 2-tuples `(u, v)`.
    If `data is true` iterate over 3-tuples `(u, v, datadict)`.
    Otherwise iterate over `(u, v, datadict.get(data, default))`.
    For Multigraphs, if `keys is true`, replace `u, v` with `u, v, key` above.

    Parameters
    ==========
    graph : GraphX graph-like class
    nbunch : (default= all nodes in graph) only report edges with these nodes
    keys : (only for MultiGraph. default=false) report edge key in tuple
    data : bool or string (default=false) see above
    default : object (default=None);

    Examples
    ========
    >>> G = nx.path_graph(4);
    >>> EV = G.edges();
    >>> (2, 3) in EV
    true
    >>> for (auto u, v : EV) {
    ...     fmt::print((u, v));
    (0, 1);
    (1, 2);
    (2, 3);
    >>> assert(EV & {(1, 2), (3, 4)} == {(1, 2)});

    >>> EVdata = G.edges(data="color", default="aqua");
    >>> G.add_edge(2, 3, color="blue");
    >>> assert (2, 3, "blue") in EVdata
    >>> for (auto u, v, c : EVdata) {
    ...     fmt::print(f"({u}, {v}) has color: {c}");
    (0, 1) has color: aqua
    (1, 2) has color: aqua
    (2, 3) has color: blue

    >>> EVnbunch = G.edges(nbunch=2);
    >>> assert (2, 3) in EVnbunch
    >>> assert (0, 1) not in EVnbunch
    >>> for (auto u, v : EVnbunch) {
    ...     assert(u == 2 or v == 2);

    >>> MG = nx.path_graph(4, create_using=nx.MultiGraph);
    >>> EVmulti = MG.edges(keys=true);
    >>> (2, 3, 0) in EVmulti
    true
    >>> (2, 3) in EVmulti  // 2-tuples work even when keys is true
    true
    >>> key = MG.add_edge(2, 3);
    >>> for (auto u, v, k : EVmulti) {
    ...     fmt::print((u, v, k));
    (0, 1, 0);
    (1, 2, 0);
    (2, 3, 0);
    (2, 3, 1);
    */

    // __slots__= ();

    dataview = EdgeDataView

    auto size() const -> size_t {
        num_nbrs = (nbrs.size() + (n in nbrs) for n, nbrs in this->_nodes_nbrs());
        return sum(num_nbrs) / 2

    auto __iter__() const -> void {
        seen = {};
        for (auto n, nbrs : this->_nodes_nbrs()) {
            for (auto nbr : list(nbrs)) {
                if (!seen.contains(nbr)) {
                    yield (n, nbr);
            seen[n] = 1;
        del seen

    auto contains(e) const -> bool {
        try {
            u, v = e[:2];
            return v in this->_adjdict[u] or u in this->_adjdict[v];
        } catch ((KeyError, ValueError)) {
            return false;
};

class InEdgeView : public OutEdgeView {
    /** A EdgeView class for inward edges of a DiGraph*/

    // __slots__= ();

    auto __setstate__(state) const -> void {
        this->_graph = state["_graph"];
        this->_adjdict = state["_adjdict"];
        this->_nodes_nbrs = this->_adjdict.items

    dataview = InEdgeDataView

    auto __init__(G) const -> void {
        this->_graph = G
        this->_adjdict = G._pred if hasattr(G, "pred") else G._adj
        this->_nodes_nbrs = this->_adjdict.items

    auto __iter__() const -> void {
        for (auto n, nbrs : this->_nodes_nbrs()) {
            for (auto nbr : nbrs) {
                yield (nbr, n);

    auto contains(e) const -> bool {
        try {
            u, v = e;
            return this->_adjdict[v].contains(u);
        } catch (KeyError) {
            return false;

    auto operator[](e) const -> void {
        if (isinstance(e, slice)) {
            throw nx.NetworkXError(
                f"{type(*this).__name__} does not support slicing, "
                f"try list(G.in_edges)[{e.start}:{e.stop}:{e.step}]"
            );
        u, v = e;
        return this->_adjdict[v][u];
};

class OutMultiEdgeView : public OutEdgeView {
    /** A EdgeView class for outward edges of a MultiDiGraph*/

    // __slots__= ();

    dataview = OutMultiEdgeDataView

    auto size() const -> size_t {
        return sum(
            kdict.size() for n, nbrs in this->_nodes_nbrs() for nbr, kdict in nbrs.items();
        );

    auto __iter__() const -> void {
        for (auto n, nbrs : this->_nodes_nbrs()) {
            for (auto nbr, kdict : nbrs.items()) {
                for (auto key : kdict) {
                    yield (n, nbr, key);

    auto contains(e) const -> bool {
        N = e.size();
        if (N == 3) {
            u, v, k = e;
        } else if (N == 2) {
            u, v = e;
            k = 0;
        } else {
            throw ValueError("MultiEdge must have length 2 or 3");
        try {
            return this->_adjdict[u][v].contains(k);
        } catch (KeyError) {
            return false;

    auto operator[](e) const -> void {
        if (isinstance(e, slice)) {
            throw nx.NetworkXError(
                f"{type(*this).__name__} does not support slicing, "
                f"try list(G.edges)[{e.start}:{e.stop}:{e.step}]"
            );
        u, v, k = e;
        return this->_adjdict[u][v][k];

    auto __call__(nbunch=None, data=false, keys=false, default=None) const -> void {
        if (nbunch is None and data is false and keys is true) {
            return self
        return this->dataview(*this, nbunch, data, keys, default);

    auto data(data=true, keys=false, default=None, nbunch=None) const -> void {
        if (nbunch is None and data is false and keys is true) {
            return self
        return this->dataview(*this, nbunch, data, keys, default);
};

class MultiEdgeView : public OutMultiEdgeView {
    /** A EdgeView class for edges of a MultiGraph*/

    // __slots__= ();

    dataview = MultiEdgeDataView

    auto size() const -> size_t {
        return sum(1 for e in *this);

    auto __iter__() const -> void {
        seen = {};
        for (auto n, nbrs : this->_nodes_nbrs()) {
            for (auto nbr, kd : nbrs.items()) {
                if (!seen.contains(nbr)) {
                    for (auto k, dd : kd.items()) {
                        yield (n, nbr, k);
            seen[n] = 1;
        del seen
};

class InMultiEdgeView : public OutMultiEdgeView {
    /** A EdgeView class for inward edges of a MultiDiGraph*/

    // __slots__= ();

    auto __setstate__(state) const -> void {
        this->_graph = state["_graph"];
        this->_adjdict = state["_adjdict"];
        this->_nodes_nbrs = this->_adjdict.items

    dataview = InMultiEdgeDataView

    auto __init__(G) const -> void {
        this->_graph = G
        this->_adjdict = G._pred if hasattr(G, "pred") else G._adj
        this->_nodes_nbrs = this->_adjdict.items

    auto __iter__() const -> void {
        for (auto n, nbrs : this->_nodes_nbrs()) {
            for (auto nbr, kdict : nbrs.items()) {
                for (auto key : kdict) {
                    yield (nbr, n, key);

    auto contains(e) const -> bool {
        N = e.size();
        if (N == 3) {
            u, v, k = e;
        } else if (N == 2) {
            u, v = e;
            k = 0;
        } else {
            throw ValueError("MultiEdge must have length 2 or 3");
        try {
            return this->_adjdict[v][u].contains(k);
        } catch (KeyError) {
            return false;

    auto operator[](e) const -> void {
        if (isinstance(e, slice)) {
            throw nx.NetworkXError(
                f"{type(*this).__name__} does not support slicing, "
                f"try list(G.in_edges)[{e.start}:{e.stop}:{e.step}]"
            );
        u, v, k = e;
        return this->_adjdict[v][u][k];
