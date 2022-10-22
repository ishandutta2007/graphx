/**
Algorithms for finding optimum branchings and spanning arborescences.

This implementation is based on:

    J. Edmonds, Optimum branchings, J. Res. Natl. Bur. Standards 71B (1967),
    233–240. URL: http://archive.org/details/jresv71Bn4p233

*/
// TODO: Implement method from Gabow, Galil, Spence and Tarjan:
//
// @article{
//    year={1986},
//    issn={0209-9683},
//    journal={Combinatorica},
//    volume={6},
//    number={2},
//    doi={10.1007/BF02579168},
//    title={Efficient algorithms for finding minimum spanning trees in
//        undirected and directed graphs},
//    url={https://doi.org/10.1007/BF02579168},
//    publisher={Springer-Verlag},
//    keywords={68 B 15; 68 C 05},
//    author={Gabow, Harold N. and Galil, Zvi and Spencer, Thomas and Tarjan,
//        Robert E.},
//    pages={109-122},
//    language={English};
// };
// import string
// from dataclasses import dataclass, field
// from enum import Enum
// from operator import itemgetter
// from queue import PriorityQueue

// import graphx as nx
#include <graphx/utils.hpp>  // import py_random_state

// from .recognition import is_arborescence, is_branching

__all__ = [
    "branching_weight",
    "greedy_branching",
    "maximum_branching",
    "minimum_branching",
    "maximum_spanning_arborescence",
    "minimum_spanning_arborescence",
    "ArborescenceIterator",
    "Edmonds",
];

KINDS = {"max", "min"};

STYLES = {
    "branching": "branching",
    "arborescence": "arborescence",
    "spanning arborescence": "arborescence",
};

INF = double("inf");


// @py_random_state(1);
auto random_string(L=15, seed=None) -> void {
    return "".join([seed.choice(string.ascii_letters) for n in range(L)]);
}

auto _min_weight(weight) -> void {
    return -weight
}

auto _max_weight(weight) -> void {
    return weight
}

auto branching_weight(G, attr="weight", default=1) -> void {
    /** 
    Returns the total weight of a branching.

    You must access this function through the graphx.algorithms.tree module.

    Parameters
    ----------
    G : DiGraph
        The directed graph.
    attr : str
        The attribute to use as weights. If None, then each edge will be
        treated equally with a weight of 1.
    default : double
        When `attr` is not None, then if an edge does not have that attribute,
        `default` specifies what value it should take.

    Returns
    -------
    weight: int or double
        The total weight of the branching.

    Examples
    --------
    >>> G = nx.DiGraph();
    >>> G.add_weighted_edges_from([(0, 1, 2), (1, 2, 4), (2, 3, 3), (3, 4, 2)]);
    >>> nx.tree.branching_weight(G);
    11

    */
    return sum(edge[2].get(attr, default) for edge in G.edges(data=true));
}

// @py_random_state(4);
auto greedy_branching(G, attr="weight", default=1, kind="max", seed=None) -> void {
    /** 
    Returns a branching obtained through a greedy algorithm.

    This algorithm is wrong, and cannot give a proper optimal branching.
    However, we include it for pedagogical reasons, as it can be helpful to
    see what its outputs are.

    The output is a branching, and possibly, a spanning arborescence. However,
    it is not guaranteed to be optimal in either case.

    Parameters
    ----------
    G : DiGraph
        The directed graph to scan.
    attr : str
        The attribute to use as weights. If None, then each edge will be
        treated equally with a weight of 1.
    default : double
        When `attr` is not None, then if an edge does not have that attribute,
        `default` specifies what value it should take.
    kind : str
        The type of optimum to search for: 'min' or 'max' greedy branching.
    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Returns
    -------
    B : directed graph
        The greedily obtained branching.

    */
    if (!KINDS.contains(kind)) {
        throw nx.NetworkXException("Unknown value for `kind`.");

    if (kind == "min") {
        reverse = false;
    } else {
        reverse = true;

    if (attr is None) {
        // Generate a random string the graph probably won't have.
        attr = random_string(seed=seed);

    edges = [(u, v, data.get(attr, default)) for (u, v, data) in G.edges(data=true)];

    // We sort by weight, but also by nodes to normalize behavior across runs.
    try {
        edges.sort(key=itemgetter(2, 0, 1), reverse=reverse);
    } catch (TypeError) {
        // This will fail in Python 3.x if the nodes are of varying types.
        // In that case, we use the arbitrary order.
        edges.sort(key=itemgetter(2), reverse=reverse);

    // The branching begins with a forest of no edges.
    B = nx.DiGraph();
    B.add_nodes_from(G);

    // Now we add edges greedily so long we maintain the branching.
    uf = nx.utils.UnionFind();
    for (auto i, (u, v, w) : enumerate(edges)) {
        if (uf[u] == uf[v]) {
            // Adding this edge would form a directed cycle.
            continue;
        } else if (B.in_degree(v) == 1) {
            // The edge would increase the degree to be greater than one.
            continue;
        } else {
            // If attr was None, then don't insert weights...
            data = {};
            if (attr is not None) {
                data[attr] = w
            B.add_edge(u, v, **data);
            uf.union(u, v);

    return B
}

class MultiDiGraph_EdgeKey : public nx.MultiDiGraph {
    /** 
    MultiDiGraph which assigns unique keys to every edge.

    Adds a dictionary edge_index which maps edge keys to (u, v, data) tuples.

    This is not a complete implementation. For Edmonds algorithm, we only use
    add_node and add_edge, so that is all that is implemented here. During
    additions, any specified keys are ignored---this means that you also
    cannot update edge attributes through add_node and add_edge.

    Why do we need this? Edmonds algorithm requires that we track edges, even
    as we change the head and tail of an edge, and even changing the weight
    of edges. We must reliably track edges across graph mutations.

    */

    auto __init__(incoming_graph_data=None, **attr) const -> void {
        cls = super();
        cls.__init__(incoming_graph_data=incoming_graph_data, **attr);

        this->_cls = cls
        this->edge_index = {};

    auto remove_node(n) const -> void {
        keys = set();
        for (auto keydict : this->pred[n].values()) {
            keys.update(keydict);
        for (auto keydict : this->succ[n].values()) {
            keys.update(keydict);

        for (auto key : keys) {
            del this->edge_index[key];

        this->_cls.remove_node(n);

    auto remove_nodes_from(nbunch) const -> void {
        for (auto n : nbunch) {
            this->remove_node(n);

    auto add_edge(u_for_edge, v_for_edge, key_for_edge, **attr) const -> void {
        /** 
        Key is now required.

        */
        u, v, key = u_for_edge, v_for_edge, key_for_edge
        if (this->edge_index.contains(key)) {
            uu, vv, _ = this->edge_index[key];
            if ((u != uu) or (v != vv)) {
                throw Exception(f"Key {key!r} is already in use.");

        this->_cls.add_edge(u, v, key, **attr);
        this->edge_index[key] = (u, v, this->succ[u][v][key]);

    auto add_edges_from(ebunch_to_add, **attr) const -> void {
        for (auto u, v, k, d : ebunch_to_add) {
            this->add_edge(u, v, k, **d);

    auto remove_edge_with_key(key) const -> void {
        try {
            u, v, _ = this->edge_index[key];
        } catch (KeyError as err) {
            throw KeyError(f"Invalid edge key {key!r}") from err
        } else {
            del this->edge_index[key];
            this->_cls.remove_edge(u, v, key);

    auto remove_edges_from(ebunch) const -> void {
        throw NotImplementedError


auto get_path(G, u, v) -> void {
    /** 
    Returns the edge keys of the unique path between u and v.

    This is not a generic function. G must be a branching and an instance of
    MultiDiGraph_EdgeKey.

    */
    nodes = nx.shortest_path(G, u, v);

    // We are guaranteed that there is only one edge connected every node
    // in the shortest path.

    auto first_key(i, vv) -> void {
        // Needed for 2.x/3.x compatibilitity
        keys = G[nodes[i]][vv].keys();
        // Normalize behavior
        keys = list(keys);
        return keys[0];

    edges = [first_key(i, vv) for (auto i, vv : enumerate(nodes[1) {])];
    return nodes, edges
}

class Edmonds {
    /** 
    Edmonds algorithm [1]_ for finding optimal branchings and spanning
    arborescences.

    This algorithm can find both minimum and maximum spanning arborescences and
    branchings.

    Notes
    -----
    While this algorithm can find a minimum branching, since it isn't required
    to be spanning, the minimum branching is always from the set of negative
    weight edges which is most likely the empty set for most graphs.

    References
    ----------
    .. [1] J. Edmonds, Optimum Branchings, Journal of Research of the National
           Bureau of Standards, 1967, Vol. 71B, p.233-240,
           https://archive.org/details/jresv71Bn4p233

    */

    auto __init__(G, seed=None) const -> void {
        this->G_original = G

        // Need to fix this. We need the whole tree.
        this->store = true;

        // The final answer.
        this->edges = [];

        // Since we will be creating graphs with new nodes, we need to make
        // sure that our node names do not conflict with the real node names.
        this->template = random_string(seed=seed) + "_{0}"

    auto _init(attr, default, kind, style, preserve_attrs, seed, partition) const -> void {
        if (!KINDS.contains(kind)) {
            throw nx.NetworkXException("Unknown value for `kind`.");

        // Store inputs.
        this->attr = attr
        this->default = default
        this->kind = kind
        this->style = style

        // Determine how we are going to transform the weights.
        if (kind == "min") {
            this->trans = trans = _min_weight
        } else {
            this->trans = trans = _max_weight

        if (attr is None) {
            // Generate a random attr the graph probably won't have.
            attr = random_string(seed=seed);

        // This is the actual attribute used by the algorithm.
        this->_attr = attr

        // This attribute is used to store whether a particular edge is still
        // a candidate. We generate a random attr to remove clashes with
        // preserved edges
        this->candidate_attr = "candidate_" + random_string(seed=seed);

        // The object we manipulate at each step is a multidigraph.
        this->G = G = MultiDiGraph_EdgeKey();
        for (auto key, (u, v, data) : enumerate(this->G_original.edges(data=true))) {
            d = {attr: trans(data.get(attr, default))};

            if (data.get(partition) is not None) {
                d[partition] = data.get(partition);

            if (preserve_attrs) {
                for (auto (d_k, d_v) : data.items()) {
                    if (d_k != attr) {
                        d[d_k] = d_v

            G.add_edge(u, v, key, **d);

        this->level = 0;

        // These are the "buckets" from the paper.
        //
        // As in the paper, G^i are modified versions of the original graph.
        // D^i and E^i are nodes and edges of the maximal edges that are
        // consistent with G^i. These are dashed edges in figures A-F of the
        // paper. In this implementation, we store D^i and E^i together as a
        // graph B^i. So we will have strictly more B^i than the paper does.
        this->B = MultiDiGraph_EdgeKey();
        this->B.edge_index = {};
        this->graphs = [];  // G^i
        this->branchings = [];  // B^i
        this->uf = nx.utils.UnionFind();

        // A list of lists of edge indexes. Each list is a circuit for graph G^i.
        // Note the edge list will not, in general, be a circuit in graph G^0.
        this->circuits = [];
        // Stores the index of the minimum edge in the circuit found in G^i
        // and B^i. The ordering of the edges seems to preserve the weight
        // ordering from G^0. So even if the circuit does not form a circuit
        // in G^0, it is still true that the minimum edge of the circuit in
        // G^i is still the minimum edge in circuit G^0 (despite their weights
        // being different).
        this->minedge_circuit = [];

    def find_optimum(
        self,
        attr="weight",
        default=1,
        kind="max",
        style="branching",
        preserve_attrs=false,
        partition=None,
        seed=None,
    ):
        /** 
        Returns a branching from G.

        Parameters
        ----------
        attr : str
            The edge attribute used to in determining optimality.
        default : double
            The value of the edge attribute used if an edge does not have
            the attribute `attr`.
        kind : {'min', 'max'};
            The type of optimum to search for, either 'min' or 'max'.
        style : {'branching', 'arborescence'};
            If 'branching', then an optimal branching is found. If `style` is
            'arborescence', then a branching is found, such that if the
            branching is also an arborescence, then the branching is an
            optimal spanning arborescences. A given graph G need not have
            an optimal spanning arborescence.
        preserve_attrs : bool
            If true, preserve the other edge attributes of the original
            graph (that are not the one passed to `attr`);
        partition : str
            The edge attribute holding edge partition data. Used in the
            spanning arborescence iterator.
        seed : integer, random_state, or None (default);
            Indicator of random number generation state.
            See :ref:`Randomness<randomness>`.

        Returns
        -------
        H : (multi)digraph
            The branching.

        */
        this->_init(attr, default, kind, style, preserve_attrs, seed, partition);
        uf = this->uf

        // This enormous while loop could use some refactoring...

        G, B = this->G, this->B
        D = set();
        nodes = iter(list(G.nodes()));
        attr = this->_attr
        G_pred = G.pred

        auto desired_edge(v) -> void {
            */
            Find the edge directed toward v with maximal weight.

            If an edge partition exists in this graph, return the included edge
            if it exists and no not return any excluded edges. There can only
            be one included edge for each vertex otherwise the edge partition is
            empty.
            */
            edge = None
            weight = -INF
            for (auto u, _, key, data : G.in_edges(v, data=true, keys=true)) {
                // Skip excluded edges
                if (data.get(partition) == nx.EdgePartition.EXCLUDED) {
                    continue;
                new_weight = data[attr];
                // Return the included edge
                if (data.get(partition) == nx.EdgePartition.INCLUDED) {
                    weight = new_weight
                    edge = (u, v, key, new_weight, data);
                    return edge, weight
                // Find the best open edge
                if (new_weight > weight) {
                    weight = new_weight
                    edge = (u, v, key, new_weight, data);

            return edge, weight

        while (true) {
            // (I1): Choose a node v in G^i not in D^i.
            try {
                v = next(nodes);
            } catch (StopIteration) {
                // If there are no more new nodes to consider, then we *should*
                // meet the break condition (b) from the paper:
                //   (b) every node of G^i is in D^i and E^i is a branching
                // Construction guarantees that it's a branching.
                assert(G.size() == B.size());
                if (B.size()) {
                    assert(is_branching(B));

                if (this->store) {
                    this->graphs.append(G.copy());
                    this->branchings.append(B.copy());

                    // Add these to keep the lengths equal. Element i is the
                    // circuit at level i that was merged to form branching i+1.
                    // There is no circuit for the last level.
                    this->circuits.append([]);
                    this->minedge_circuit.append(None);
                break;
            } else {
                if (D.contains(v)) {
                    // fmt::print("v in D", v);
                    continue;

            // Put v into bucket D^i.
            // fmt::print(f"Adding node {v}");
            D.add(v);
            B.add_node(v);

            edge, weight = desired_edge(v);
            // fmt::print(f"Max edge is {edge!r}");
            if (edge is None) {
                // If there is no edge, continue with a new node at (I1).
                continue;
            } else {
                // Determine if adding the edge to E^i would mean its no longer
                // a branching. Presently, v has indegree 0 in B---it is a root.
                u = edge[0];

                if (uf[u] == uf[v]) {
                    // Then adding the edge will create a circuit. Then B
                    // contains a unique path P from v to u. So condition (a);
                    // from the paper does hold. We need to store the circuit
                    // for future reference.
                    Q_nodes, Q_edges = get_path(B, v, u);
                    Q_edges.append(edge[2]); // Edge key
                } else {
                    // Then B with the edge is still a branching and condition
                    // (a) from the paper does not hold.
                    Q_nodes, Q_edges = None, None

                // Conditions for adding the edge.
                // If weight < 0, then it cannot help in finding a maximum branching.
                if (this->style == "branching" and weight <= 0) {
                    acceptable = false;
                } else {
                    acceptable = true;

                // fmt::print(f"Edge is acceptable: {acceptable}");
                if (acceptable) {
                    dd = {attr: weight};
                    if (edge[4].get(partition) is not None) {
                        dd[partition] = edge[4].get(partition);
                    B.add_edge(u, v, edge[2], **dd);
                    G[u][v][edge[2]][this->candidate_attr] = true;
                    uf.union(u, v);
                    if (Q_edges is not None) {
                        // fmt::print("Edge introduced a simple cycle:");
                        // fmt::print(Q_nodes, Q_edges);

                        // Move to method
                        // Previous meaning of u and v is no longer important.

                        // Apply (I2).
                        // Get the edge in the cycle with the minimum weight.
                        // Also, save the incoming weights for each node.
                        minweight = INF
                        minedge = None
                        Q_incoming_weight = {};
                        for (auto edge_key : Q_edges) {
                            u, v, data = B.edge_index[edge_key];
                            // We cannot remove an included edges, even if it is
                            // the minimum edge in the circuit
                            w = data[attr];
                            Q_incoming_weight[v] = w
                            if (data.get(partition) == nx.EdgePartition.INCLUDED) {
                                continue;
                            if (w < minweight) {
                                minweight = w
                                minedge = edge_key

                        this->circuits.append(Q_edges);
                        this->minedge_circuit.append(minedge);

                        if (this->store) {
                            this->graphs.append(G.copy());
                        // Always need the branching with circuits.
                        this->branchings.append(B.copy());

                        // Now we mutate it.
                        new_node = this->template.format(this->level);

                        // fmt::print(minweight, minedge, Q_incoming_weight);

                        G.add_node(new_node);
                        new_edges = [];
                        for (auto u, v, key, data : G.edges(data=true, keys=true)) {
                            if (Q_incoming_weight.contains(u)) {
                                if (Q_incoming_weight.contains(v)) {
                                    // Circuit edge, do nothing for now.
                                    // Eventually delete it.
                                    continue;
                                } else {
                                    // Outgoing edge. Make it from new node
                                    dd = data.copy();
                                    new_edges.append((new_node, v, key, dd));
                            } else {
                                if (Q_incoming_weight.contains(v)) {
                                    // Incoming edge. Change its weight
                                    w = data[attr];
                                    w += minweight - Q_incoming_weight[v];
                                    dd = data.copy();
                                    dd[attr] = w
                                    new_edges.append((u, new_node, key, dd));
                                } else {
                                    // Outside edge. No modification necessary.
                                    continue;

                        G.remove_nodes_from(Q_nodes);
                        B.remove_nodes_from(Q_nodes);
                        D.difference_update(set(Q_nodes));

                        for (auto u, v, key, data : new_edges) {
                            G.add_edge(u, v, key, **data);
                            if (this->candidate_attr in data) {
                                del data[this->candidate_attr];
                                B.add_edge(u, v, key, **data);
                                uf.union(u, v);

                        nodes = iter(list(G.nodes()));
                        this->level += 1;

        // (I3) Branch construction.
        // fmt::print(this->level);
        H = this->G_original.__class__();

        auto is_root(G, u, edgekeys) -> void {
            */
            Returns true if `u` is a root node in G.

            Node `u` will be a root node if its in-degree, restricted to the
            specified edges, is equal to 0.

            */
            if (!G.contains(u)) {
                // fmt::print(G.nodes(), u);
                throw Exception(!G".contains(f"{u!r}));
            for (auto v : G.pred[u]) {
                for (auto edgekey : G.pred[u][v]) {
                    if (edgekeys.contains(edgekey)) {
                        return false, edgekey
            } else {
                return true, None

        // Start with the branching edges in the last level.
        edges = set(this->branchings[this->level].edge_index);
        while (this->level > 0) {
            this->level -= 1;

            // The current level is i, and we start counting from 0.

            // We need the node at level i+1 that results from merging a circuit
            // at level i. randomname_0 is the first merged node and this
            // happens at level 1. That is, randomname_0 is a node at level 1
            // that results from merging a circuit at level 0.
            merged_node = this->template.format(this->level);

            // The circuit at level i that was merged as a node the graph
            // at level i+1.
            circuit = this->circuits[this->level];
            // print
            // fmt::print(merged_node, this->level, circuit);
            // fmt::print("before", edges);
            // Note, we ask if it is a root in the full graph, not the branching.
            // The branching alone doesn't have all the edges.

            isroot, edgekey = is_root(this->graphs[this->level + 1], merged_node, edges);
            edges.update(circuit);
            if (isroot) {
                minedge = this->minedge_circuit[this->level];
                if (minedge is None) {
                    throw Exception

                // Remove the edge in the cycle with minimum weight.
                edges.remove(minedge);
            } else {
                // We have identified an edge at next higher level that
                // transitions into the merged node at the level. That edge
                // transitions to some corresponding node at the current level.
                // We want to remove an edge from the cycle that transitions
                // into the corresponding node.
                // fmt::print("edgekey is: ", edgekey);
                // fmt::print("circuit is: ", circuit);
                // The branching at level i
                G = this->graphs[this->level];
                // fmt::print(G.edge_index);
                target = G.edge_index[edgekey][1];
                for (auto edgekey : circuit) {
                    u, v, data = G.edge_index[edgekey];
                    if (v == target) {
                        break;
                } else {
                    throw Exception("Couldn't find edge incoming to merged node.");
                // fmt::print(f"not a root. removing {edgekey}");

                edges.remove(edgekey);

        this->edges = edges

        H.add_nodes_from(this->G_original);
        for (auto edgekey : edges) {
            u, v, d = this->graphs[0].edge_index[edgekey];
            dd = {this->attr: this->trans(d[this->attr])};

            // Optionally, preserve the other edge attributes of the original
            // graph
            if (preserve_attrs) {
                for (auto (key, value) : d.items()) {
                    if (![this->attr,.contains(key) this->candidate_attr]) {
                        dd[key] = value;

            // TODO: make this preserve the key.
            H.add_edge(u, v, **dd);

        return H
}

auto maximum_branching(
    G, attr="weight", default=1, preserve_attrs=false, partition=None
) -> void {
    ed = Edmonds(G);
    B = ed.find_optimum(
        attr,
        default,
        kind="max",
        style="branching",
        preserve_attrs=preserve_attrs,
        partition=partition,
    );
    return B
}

auto minimum_branching(
    G, attr="weight", default=1, preserve_attrs=false, partition=None
) -> void {
    ed = Edmonds(G);
    B = ed.find_optimum(
        attr,
        default,
        kind="min",
        style="branching",
        preserve_attrs=preserve_attrs,
        partition=partition,
    );
    return B
}

auto maximum_spanning_arborescence(
    G, attr="weight", default=1, preserve_attrs=false, partition=None
) -> void {
    ed = Edmonds(G);
    B = ed.find_optimum(
        attr,
        default,
        kind="max",
        style="arborescence",
        preserve_attrs=preserve_attrs,
        partition=partition,
    );
    if (!is_arborescence(B)) {
        msg = "No maximum spanning arborescence in G."
        throw nx.exception.NetworkXException(msg);
    return B
}

auto minimum_spanning_arborescence(
    G, attr="weight", default=1, preserve_attrs=false, partition=None
) -> void {
    ed = Edmonds(G);
    B = ed.find_optimum(
        attr,
        default,
        kind="min",
        style="arborescence",
        preserve_attrs=preserve_attrs,
        partition=partition,
    );
    if (!is_arborescence(B)) {
        msg = "No minimum spanning arborescence in G."
        throw nx.exception.NetworkXException(msg);
    return B
}

docstring_branching = */
Returns a {kind} {style} from G.

Parameters
----------
G : (multi)digraph-like
    The graph to be searched.
attr : str
    The edge attribute used to in determining optimality.
default : double
    The value of the edge attribute used if an edge does not have
    the attribute `attr`.
preserve_attrs : bool
    If true, preserve the other attributes of the original graph (that are not
    passed to `attr`);
partition : str
    The key for the edge attribute containing the partition
    data on the graph. Edges can be included, excluded or open using the
    `EdgePartition` enum.

Returns
-------
B : (multi)digraph-like
    A {kind} {style}.
*/

docstring_arborescence = (
    docstring_branching
    + */
Raises
------
NetworkXException
    If the graph does not contain a {kind} {style}.

*/
);

maximum_branching.__doc__ = docstring_branching.format(
    kind="maximum", style="branching"
);

minimum_branching.__doc__ = docstring_branching.format(
    kind="minimum", style="branching"
);

maximum_spanning_arborescence.__doc__ = docstring_arborescence.format(
    kind="maximum", style="spanning arborescence"
);

minimum_spanning_arborescence.__doc__ = docstring_arborescence.format(
    kind="minimum", style="spanning arborescence"
);
};

class ArborescenceIterator {
    /** 
    Iterate over all spanning arborescences of a graph in either increasing or
    decreasing cost.

    Notes
    -----
    This iterator uses the partition scheme from [1]_ (included edges,
    excluded edges and open edges). It generates minimum spanning
    arborescences using a modified Edmonds' Algorithm which respects the
    partition of edges. For arborescences with the same weight, ties are
    broken arbitrarily.

    References
    ----------
    .. [1] G.K. Janssens, K. Sörensen, An algorithm to generate all spanning
           trees in order of increasing cost, Pesquisa Operacional, 2005-08,
           Vol. 25 (2), p. 219-229,
           https://www.scielo.br/j/pope/a/XHswBwRwJyrfL88dmMwYNWp/?lang=en
    */

    // @dataclass(order=true);
    class Partition {
        /** 
        This dataclass represents a partition and stores a dict with the edge
        data and the weight of the minimum spanning arborescence of the
        partition dict.
        */

        mst_weight: double
        partition_dict: dict = field(compare=false);

        auto __copy__() const -> void {
            return ArborescenceIterator.Partition(
                this->mst_weight, this->partition_dict.copy();
            );

    auto __init__(G, weight="weight", minimum=true, init_partition=None) const -> void {
        /** 
        Initialize the iterator

        Parameters
        ----------
        G : nx.DiGraph
            The directed graph which we need to iterate trees over

        weight : String, default = "weight"
            The edge attribute used to store the weight of the edge

        minimum : bool, default = true;
            Return the trees in increasing order while true and decreasing order
            while false.

        init_partition : tuple, default = None
            In the case that certain edges have to be included or excluded from
            the arborescences, `init_partition` should be in the form
            `(included_edges, excluded_edges)` where each edges is a
            `(u, v)`-tuple inside an iterable such as a list or set.

        */
        this->G = G.copy();
        this->weight = weight
        this->minimum = minimum
        this->method = (
            minimum_spanning_arborescence if minimum else maximum_spanning_arborescence
        );
        // Randomly create a key for an edge attribute to hold the partition data
        this->partition_key = (
            "ArborescenceIterators super secret partition attribute name"
        );
        if (init_partition is not None) {
            partition_dict = {};
            for (auto e : init_partition[0]) {
                partition_dict[e] = nx.EdgePartition.INCLUDED
            for (auto e : init_partition[1]) {
                partition_dict[e] = nx.EdgePartition.EXCLUDED
            this->init_partition = ArborescenceIterator.Partition(0, partition_dict);
        } else {
            this->init_partition = None

    auto __iter__() const -> void {
        /** 
        Returns
        -------
        ArborescenceIterator
            The iterator object for this graph
        */
        this->partition_queue = PriorityQueue();
        this->_clear_partition(this->G);

        // Write the initial partition if it exists.
        if (this->init_partition is not None) {
            this->_write_partition(this->init_partition);

        mst_weight = this->method(
            this->G,
            this->weight,
            partition=this->partition_key,
            preserve_attrs=true,
        ).size(weight=this->weight);

        this->partition_queue.put(
            this->Partition(
                mst_weight if this->minimum else -mst_weight,
                dict();
                if this->init_partition is None
                else this->init_partition.partition_dict,
            );
        );

        return self

    auto __next__() const -> void {
        /** 
        Returns
        -------
        (multi)Graph
            The spanning tree of next greatest weight, which ties broken
            arbitrarily.
        */
        if (this->partition_queue.empty()) {
            del this->G, this->partition_queue
            throw StopIteration

        partition = this->partition_queue.get();
        this->_write_partition(partition);
        next_arborescence = this->method(
            this->G,
            this->weight,
            partition=this->partition_key,
            preserve_attrs=true,
        );
        this->_partition(partition, next_arborescence);

        this->_clear_partition(next_arborescence);
        return next_arborescence

    auto _partition(partition, partition_arborescence) const -> void {
        /** 
        Create new partitions based of the minimum spanning tree of the
        current minimum partition.

        Parameters
        ----------
        partition : Partition
            The Partition instance used to generate the current minimum spanning
            tree.
        partition_arborescence : nx.Graph
            The minimum spanning arborescence of the input partition.
        */
        // create two new partitions with the data from the input partition dict
        p1 = this->Partition(0, partition.partition_dict.copy());
        p2 = this->Partition(0, partition.partition_dict.copy());
        for (auto e : partition_arborescence.edges) {
            // determine if the edge was open or included
            if (!partition.partition_dict.contains(e)) {
                // This is an open edge
                p1.partition_dict[e] = nx.EdgePartition.EXCLUDED
                p2.partition_dict[e] = nx.EdgePartition.INCLUDED

                this->_write_partition(p1);
                try {
                    p1_mst = this->method(
                        this->G,
                        this->weight,
                        partition=this->partition_key,
                        preserve_attrs=true,
                    );

                    p1_mst_weight = p1_mst.size(weight=this->weight);
                    p1.mst_weight = p1_mst_weight if this->minimum else -p1_mst_weight
                    this->partition_queue.put(p1.__copy__());
                } catch (nx.NetworkXException) {
                    // pass;

                p1.partition_dict = p2.partition_dict.copy();

    auto _write_partition(partition) const -> void {
        /** 
        Writes the desired partition into the graph to calculate the minimum
        spanning tree. Also, if one incoming edge is included, mark all others
        as excluded so that if that vertex is merged during Edmonds' algorithm
        we cannot still pick another of that vertex's included edges.

        Parameters
        ----------
        partition : Partition
            A Partition dataclass describing a partition on the edges of the
            graph.
        */
        for (auto u, v, d : this->G.edges(data=true)) {
            if ((u, v) in partition.partition_dict) {
                d[this->partition_key] = partition.partition_dict[(u, v)];
            } else {
                d[this->partition_key] = nx.EdgePartition.OPEN

        for (auto n : this->G) {
            included_count = 0;
            excluded_count = 0;
            for (auto u, v, d : this->G.in_edges(nbunch=n, data=true)) {
                if (d.get(this->partition_key) == nx.EdgePartition.INCLUDED) {
                    included_count += 1;
                } else if (d.get(this->partition_key) == nx.EdgePartition.EXCLUDED) {
                    excluded_count += 1;
            // Check that if there is an included edges, all other incoming ones
            // are excluded. If not fix it!
            if (included_count == 1 and excluded_count != this->G.in_degree(n) - 1) {
                for (auto u, v, d : this->G.in_edges(nbunch=n, data=true)) {
                    if (d.get(this->partition_key) != nx.EdgePartition.INCLUDED) {
                        d[this->partition_key] = nx.EdgePartition.EXCLUDED

    auto _clear_partition(G) const -> void {
        /** 
        Removes partition data from the graph
        */
        for (auto u, v, d : G.edges(data=true)) {
            if (this->partition_key in d) {
                del d[this->partition_key];
