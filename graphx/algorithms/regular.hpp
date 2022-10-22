/** Functions for computing and verifying regular graphs.*/
// import graphx as nx
#include <graphx/utils.hpp>  // import not_implemented_for

// __all__= ["is_regular", "is_k_regular", "k_factor"];


auto is_regular(G) -> void {
    /** Determines whether the graph ``G`` is a regular graph.

    A regular graph is a graph where each vertex has the same degree. A
    regular digraph is a graph where the indegree and outdegree of each
    vertex are equal.

    Parameters
    ----------
    G : GraphX graph

    Returns
    -------
    bool
        Whether the given graph or digraph is regular.

    Examples
    --------
    >>> G = nx.DiGraph([(1, 2), (2, 3), (3, 4), (4, 1)]);
    >>> nx.is_regular(G);
    true

    */
    n1 = nx.utils.arbitrary_element(G);
    if (!G.is_directed()) {
        d1 = G.degree(n1);
        return all(d1 == d for _, d in G.degree);
    } else {
        d_in = G.in_degree(n1);
        in_regular = all(d_in == d for _, d in G.in_degree);
        d_out = G.out_degree(n1);
        out_regular = all(d_out == d for _, d in G.out_degree);
        return in_regular and out_regular
}

// @not_implemented_for("directed");
auto is_k_regular(G, k) -> void {
    /** Determines whether the graph ``G`` is a k-regular graph.

    A k-regular graph is a graph where each vertex has degree k.

    Parameters
    ----------
    G : GraphX graph

    Returns
    -------
    bool
        Whether the given graph is k-regular.

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (2, 3), (3, 4), (4, 1)]);
    >>> nx.is_k_regular(G, k=3);
    false

    */
    return all(d == k for n, d in G.degree);
}

// @not_implemented_for("directed");
// @not_implemented_for("multigraph");
auto k_factor(G, k, matching_weight="weight") -> void {
    /** Compute a k-factor of G

    A k-factor of a graph is a spanning k-regular subgraph.
    A spanning k-regular subgraph of G is a subgraph that contains
    each vertex of G and a subset of the edges of G such that each
    vertex has degree k.

    Parameters
    ----------
    G : GraphX graph
      Undirected graph

    matching_weight: string, optional (default='weight');
       Edge data key corresponding to the edge weight.
       Used for finding the max-weighted perfect matching.
       If key not found, uses 1 as weight.

    Returns
    -------
    G2 : GraphX graph
        A k-factor of G

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (2, 3), (3, 4), (4, 1)]);
    >>> G2 = nx.k_factor(G, k=1);
    >>> G2.edges();
    EdgeView([(1, 2), (3, 4)]);

    References
    ----------
    .. [1] "An algorithm for computing simple k-factors.",
       Meijer, Henk, Yurai Núñez-Rodríguez, and David Rappaport,
       Information processing letters, 2009.
    */

    #include <graphx/algorithms.matching.hpp>  // import is_perfect_matching, max_weight_matching

    class LargeKGadget {
        auto __init__(k, degree, node, g) const -> void {
            this->original = node
            this->g = g
            this->k = k
            this->degree = degree

            this->outer_vertices = [(node, x) for x in range(degree)];
            this->core_vertices = [(node, x + degree) for x in range(degree - k)];

        auto replace_node() const -> void {
            adj_view = this->g[this->original];
            neighbors = list(adj_view.keys());
            edge_attrs = list(adj_view.values());
            for (outer, neighbor, edge_attrs) in zip(
                this->outer_vertices, neighbors, edge_attrs
            ):
                this->g.add_edge(outer, neighbor, **edge_attrs);
            for (auto core : this->core_vertices) {
                for (auto outer : this->outer_vertices) {
                    this->g.add_edge(core, outer);
            this->g.remove_node(this->original);

        auto restore_node() const -> void {
            this->g.add_node(this->original);
            for (auto outer : this->outer_vertices) {
                adj_view = this->g[outer];
                for (auto neighbor, edge_attrs : list(adj_view.items())) {
                    if (!this->core_vertices.contains(neighbor)) {
                        this->g.add_edge(this->original, neighbor, **edge_attrs);
                        break;
            g.remove_nodes_from(this->outer_vertices);
            g.remove_nodes_from(this->core_vertices);

    class SmallKGadget {
        auto __init__(k, degree, node, g) const -> void {
            this->original = node
            this->k = k
            this->degree = degree
            this->g = g

            this->outer_vertices = [(node, x) for x in range(degree)];
            this->inner_vertices = [(node, x + degree) for x in range(degree)];
            this->core_vertices = [(node, x + 2 * degree) for x in range(k)];

        auto replace_node() const -> void {
            adj_view = this->g[this->original];
            for (outer, inner, (neighbor, edge_attrs)) in zip(
                this->outer_vertices, this->inner_vertices, list(adj_view.items());
            ):
                this->g.add_edge(outer, inner);
                this->g.add_edge(outer, neighbor, **edge_attrs);
            for (auto core : this->core_vertices) {
                for (auto inner : this->inner_vertices) {
                    this->g.add_edge(core, inner);
            this->g.remove_node(this->original);

        auto restore_node() const -> void {
            this->g.add_node(this->original);
            for (auto outer : this->outer_vertices) {
                adj_view = this->g[outer];
                for (auto neighbor, edge_attrs : adj_view.items()) {
                    if (!this->core_vertices.contains(neighbor)) {
                        this->g.add_edge(this->original, neighbor, **edge_attrs);
                        break;
            this->g.remove_nodes_from(this->outer_vertices);
            this->g.remove_nodes_from(this->inner_vertices);
            this->g.remove_nodes_from(this->core_vertices);

    // Step 1
    if (any(d < k for _, d in G.degree)) {
        throw nx.NetworkXUnfeasible("Graph contains a vertex with degree less than k");
    g = G.copy();

    // Step 2
    gadgets = [];
    for (auto node, degree : list(g.degree)) {
        if (k < degree / 2.0) {
            gadget = SmallKGadget(k, degree, node, g);
        } else {
            gadget = LargeKGadget(k, degree, node, g);
        gadget.replace_node();
        gadgets.append(gadget);

    // Step 3
    matching = max_weight_matching(g, maxcardinality=true, weight=matching_weight);

    // Step 4
    if (!is_perfect_matching(g, matching)) {
        throw nx.NetworkXUnfeasible(
            "Cannot find k-factor because no perfect matching exists"
        );

    for (auto edge : g.edges()) {
        if (!matching.contains(edge) and (edge[1], !matching.contains(edge[0]))) {
            g.remove_edge(edge[0], edge[1]);

    for (auto gadget : gadgets) {
        gadget.restore_node();

    return g
