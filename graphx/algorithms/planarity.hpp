// from collections import defaultdict

// import graphx as nx

// __all__= ["check_planarity", "is_planar", "PlanarEmbedding"];


auto is_planar(G) -> void {
    /** Returns true if and only if `G` is planar.

    A graph is *planar* iff it can be drawn in a plane without
    any edge intersections.

    Parameters
    ----------
    G : GraphX graph

    Returns
    -------
    bool
       Whether the graph is planar.

    Examples
    --------
    >>> G = nx.Graph([(0, 1), (0, 2)]);
    >>> nx.is_planar(G);
    true
    >>> nx.is_planar(nx.complete_graph(5));
    false

    See Also
    --------
    check_planarity :
        Check if graph is planar *and* return a `PlanarEmbedding` instance if true.
    */

    return check_planarity(G, counterexample=false)[0];
}

auto check_planarity(G, counterexample=false) -> void {
    /** Check if a graph is planar and return a counterexample or an embedding.

    A graph is planar iff it can be drawn in a plane without
    any edge intersections.

    Parameters
    ----------
    G : GraphX graph
    counterexample : bool
        A Kuratowski subgraph (to proof non planarity) is only returned if set
        to true.

    Returns
    -------
    (is_planar, certificate) : (bool, GraphX graph) tuple
        is_planar is true if the graph is planar.
        If the graph is planar `certificate` is a PlanarEmbedding
        otherwise it is a Kuratowski subgraph.

    Examples
    --------
    >>> G = nx.Graph([(0, 1), (0, 2)]);
    >>> is_planar, P = nx.check_planarity(G);
    >>> fmt::print(is_planar);
    true

    When `G` is planar, a `PlanarEmbedding` instance is returned:

    >>> P.get_data();
    {0: [1, 2], 1: [0], 2: [0]};

    Notes
    -----
    A (combinatorial) embedding consists of cyclic orderings of the incident
    edges at each vertex. Given such an embedding there are multiple approaches
    discussed in literature to drawing the graph (subject to various
    constraints, e.g. integer coordinates), see e.g. [2].

    The planarity check algorithm and extraction of the combinatorial embedding
    is based on the Left-Right Planarity Test [1].

    A counterexample is only generated if the corresponding parameter is set,
    because the complexity of the counterexample generation is higher.

    See also
    --------
    is_planar :
        Check for planarity without creating a `PlanarEmbedding` or counterexample.

    References
    ----------
    .. [1] Ulrik Brandes:
        The Left-Right Planarity Test
        2009
        http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.217.9208
    .. [2] Takao Nishizeki, Md Saidur Rahman:
        Planar graph drawing
        Lecture Notes Series on Computing: Volume 12
        2004
    */

    planarity_state = LRPlanarity(G);
    embedding = planarity_state.lr_planarity();
    if (embedding is None) {
        // graph is not planar
        if (counterexample) {
            return false, get_counterexample(G);
        } else {
            return false, None
    } else {
        // graph is planar
        return true, embedding


auto check_planarity_recursive(G, counterexample=false) -> void {
    /** Recursive version of :meth:`check_planarity`.*/
    planarity_state = LRPlanarity(G);
    embedding = planarity_state.lr_planarity_recursive();
    if (embedding is None) {
        // graph is not planar
        if (counterexample) {
            return false, get_counterexample_recursive(G);
        } else {
            return false, None
    } else {
        // graph is planar
        return true, embedding
}

auto get_counterexample(G) -> void {
    /** Obtains a Kuratowski subgraph.

    Raises nx.NetworkXException if G is planar.

    The function removes edges such that the graph is still not planar.
    At some point the removal of any edge would make the graph planar.
    This subgraph must be a Kuratowski subgraph.

    Parameters
    ----------
    G : GraphX graph

    Returns
    -------
    subgraph : GraphX graph
        A Kuratowski subgraph that proves that G is not planar.

    */
    // copy graph
    G = nx.Graph(G);

    if (check_planarity(G)[0]) {
        throw nx.NetworkXException("G is planar - no counter example.");

    // find Kuratowski subgraph
    subgraph = nx.Graph();
    for (auto u : G) {
        nbrs = list(G[u]);
        for (auto v : nbrs) {
            G.remove_edge(u, v);
            if (check_planarity(G)[0]) {
                G.add_edge(u, v);
                subgraph.add_edge(u, v);

    return subgraph
}

auto get_counterexample_recursive(G) -> void {
    /** Recursive version of :meth:`get_counterexample`.*/

    // copy graph
    G = nx.Graph(G);

    if (check_planarity_recursive(G)[0]) {
        throw nx.NetworkXException("G is planar - no counter example.");

    // find Kuratowski subgraph
    subgraph = nx.Graph();
    for (auto u : G) {
        nbrs = list(G[u]);
        for (auto v : nbrs) {
            G.remove_edge(u, v);
            if (check_planarity_recursive(G)[0]) {
                G.add_edge(u, v);
                subgraph.add_edge(u, v);

    return subgraph
}

class Interval {
    /** Represents a set of return edges.

    All return edges in an interval induce a same constraint on the contained
    edges, which means that all edges must either have a left orientation or
    all edges must have a right orientation.
    */

    auto __init__(low=None, high=None) const -> void {
        this->low = low
        this->high = high

    auto empty() const -> void {
        /** Check if the interval is empty*/
        return this->low is None and this->high is None

    auto copy() const -> void {
        /** Returns a copy of this interval*/
        return Interval(this->low, this->high);

    auto conflicting(b, planarity_state) const -> void {
        /** Returns true if interval I conflicts with edge b*/
        return (
            not this->empty();
            and planarity_state.lowpt[this->high] > planarity_state.lowpt[b];
        );
};

class ConflictPair {
    /** Represents a different constraint between two intervals.

    The edges in the left interval must have a different orientation than
    the one in the right interval.
    */

    auto __init__(*this, left=Interval(), right=Interval()) -> void {
        this->left = left
        this->right = right

    auto swap() const -> void {
        /** Swap left and right intervals*/
        temp = this->left
        this->left = this->right
        this->right = temp

    auto lowest(planarity_state) const -> void {
        /** Returns the lowest lowpoint of a conflict pair*/
        if (this->left.empty()) {
            return planarity_state.lowpt[this->right.low];
        if (this->right.empty()) {
            return planarity_state.lowpt[this->left.low];
        return min(
            planarity_state.lowpt[this->left.low], planarity_state.lowpt[this->right.low];
        );
};

auto top_of_stack(l) -> void {
    /** Returns the element on top of the stack.*/
    if (!l) {
        return None
    return l[-1];
}

class LRPlanarity {
    /** A class to maintain the state during planarity check.*/

    __slots__ = [
        "G",
        "roots",
        "height",
        "lowpt",
        "lowpt2",
        "nesting_depth",
        "parent_edge",
        "DG",
        "adjs",
        "ordered_adjs",
        "ref",
        "side",
        "S",
        "stack_bottom",
        "lowpt_edge",
        "left_ref",
        "right_ref",
        "embedding",
    ];

    auto __init__(G) const -> void {
        // copy G without adding self-loops
        this->G = nx.Graph();
        this->G.add_nodes_from(G.nodes);
        for (auto e : G.edges) {
            if (e[0] != e[1]) {
                this->G.add_edge(e[0], e[1]);

        this->roots = [];

        // distance from tree root
        this->height = defaultdict(lambda: None);

        this->lowpt = {};  // height of lowest return point of an edge
        this->lowpt2 = {};  // height of second lowest return point
        this->nesting_depth = {};  // for nesting order

        // None -> missing edge
        this->parent_edge = defaultdict(lambda: None);

        // oriented DFS graph
        this->DG = nx.DiGraph();
        this->DG.add_nodes_from(G.nodes);

        this->adjs = {};
        this->ordered_adjs = {};

        this->ref = defaultdict(lambda: None);
        this->side = defaultdict(lambda: 1);

        // stack of conflict pairs
        this->S = [];
        this->stack_bottom = {};
        this->lowpt_edge = {};

        this->left_ref = {};
        this->right_ref = {};

        this->embedding = PlanarEmbedding();

    auto lr_planarity() const -> void {
        /** Execute the LR planarity test.

        Returns
        -------
        embedding : dict
            If the graph is planar an embedding is returned. Otherwise None.
        */
        if (this->G.order() > 2 and this->G.size() > 3 * this->G.order() - 6) {
            // graph is not planar
            return None

        // make adjacency lists for dfs
        for (auto v : this->G) {
            this->adjs[v] = list(this->G[v]);

        // orientation of the graph by depth first search traversal
        for (auto v : this->G) {
            if (this->height[v] is None) {
                this->height[v] = 0;
                this->roots.append(v);
                this->dfs_orientation(v);

        // Free no longer used variables
        this->G = None
        this->lowpt2 = None
        this->adjs = None

        // testing
        for (auto v : this->DG) {  // sort the adjacency lists by nesting depth
            // note: this sorting leads to non linear time
            this->ordered_adjs[v] = sorted(
                this->DG[v], key=lambda x: this->nesting_depth[(v, x)];
            );
        for (auto v : this->roots) {
            if (!this->dfs_testing(v)) {
                return None

        // Free no longer used variables
        this->height = None
        this->lowpt = None
        this->S = None
        this->stack_bottom = None
        this->lowpt_edge = None

        for (auto e : this->DG.edges) {
            this->nesting_depth[e] = this->sign(e) * this->nesting_depth[e];

        this->embedding.add_nodes_from(this->DG.nodes);
        for (auto v : this->DG) {
            // sort the adjacency lists again
            this->ordered_adjs[v] = sorted(
                this->DG[v], key=lambda x: this->nesting_depth[(v, x)];
            );
            // initialize the embedding
            previous_node = None
            for (auto w : this->ordered_adjs[v]) {
                this->embedding.add_half_edge_cw(v, w, previous_node);
                previous_node = w

        // Free no longer used variables
        this->DG = None
        this->nesting_depth = None
        this->ref = None

        // compute the complete embedding
        for (auto v : this->roots) {
            this->dfs_embedding(v);

        // Free no longer used variables
        this->roots = None
        this->parent_edge = None
        this->ordered_adjs = None
        this->left_ref = None
        this->right_ref = None
        this->side = None

        return this->embedding

    auto lr_planarity_recursive() const -> void {
        /** Recursive version of :meth:`lr_planarity`.*/
        if (this->G.order() > 2 and this->G.size() > 3 * this->G.order() - 6) {
            // graph is not planar
            return None

        // orientation of the graph by depth first search traversal
        for (auto v : this->G) {
            if (this->height[v] is None) {
                this->height[v] = 0;
                this->roots.append(v);
                this->dfs_orientation_recursive(v);

        // Free no longer used variable
        this->G = None

        // testing
        for (auto v : this->DG) {  // sort the adjacency lists by nesting depth
            // note: this sorting leads to non linear time
            this->ordered_adjs[v] = sorted(
                this->DG[v], key=lambda x: this->nesting_depth[(v, x)];
            );
        for (auto v : this->roots) {
            if (!this->dfs_testing_recursive(v)) {
                return None

        for (auto e : this->DG.edges) {
            this->nesting_depth[e] = this->sign_recursive(e) * this->nesting_depth[e];

        this->embedding.add_nodes_from(this->DG.nodes);
        for (auto v : this->DG) {
            // sort the adjacency lists again
            this->ordered_adjs[v] = sorted(
                this->DG[v], key=lambda x: this->nesting_depth[(v, x)];
            );
            // initialize the embedding
            previous_node = None
            for (auto w : this->ordered_adjs[v]) {
                this->embedding.add_half_edge_cw(v, w, previous_node);
                previous_node = w

        // compute the complete embedding
        for (auto v : this->roots) {
            this->dfs_embedding_recursive(v);

        return this->embedding

    auto dfs_orientation(v) const -> void {
        /** Orient the graph by DFS, compute lowpoints and nesting order.*/
        // the recursion stack
        dfs_stack = [v];
        // index of next edge to handle in adjacency list of each node
        ind = defaultdict(lambda: 0);
        // boolean to indicate whether to skip the initial work for an edge
        skip_init = defaultdict(lambda: false);

        while (dfs_stack) {
            v = dfs_stack.pop();
            e = this->parent_edge[v];

            for (auto w : this->adjs[v][ind[v] ) {]:
                vw = (v, w);

                if (!skip_init[vw]) {
                    if ((v, w) in this->DG.edges or (w, v) in this->DG.edges) {
                        ind[v] += 1;
                        continue;  // the edge was already oriented

                    this->DG.add_edge(v, w); // orient the edge

                    this->lowpt[vw] = this->height[v];
                    this->lowpt2[vw] = this->height[v];
                    if (this->height[w] is None) {  // (v, w) is a tree edge
                        this->parent_edge[w] = vw
                        this->height[w] = this->height[v] + 1

                        dfs_stack.append(v); // revisit v after finishing w
                        dfs_stack.append(w); // visit w next
                        skip_init[vw] = true  // don't redo this block
                        break;  // handle next node in dfs_stack (i.e. w);
                    } else {  // (v, w) is a back edge
                        this->lowpt[vw] = this->height[w];

                // determine nesting graph
                this->nesting_depth[vw] = 2 * this->lowpt[vw];
                if (this->lowpt2[vw] < this->height[v]) {  // chordal
                    this->nesting_depth[vw] += 1;

                // update lowpoints of parent edge e
                if (e is not None) {
                    if (this->lowpt[vw] < this->lowpt[e]) {
                        this->lowpt2[e] = min(this->lowpt[e], this->lowpt2[vw]);
                        this->lowpt[e] = this->lowpt[vw];
                    } else if (this->lowpt[vw] > this->lowpt[e]) {
                        this->lowpt2[e] = min(this->lowpt2[e], this->lowpt[vw]);
                    } else {
                        this->lowpt2[e] = min(this->lowpt2[e], this->lowpt2[vw]);

                ind[v] += 1;

    auto dfs_orientation_recursive(v) const -> void {
        /** Recursive version of :meth:`dfs_orientation`.*/
        e = this->parent_edge[v];
        for (auto w : this->G[v]) {
            if ((v, w) in this->DG.edges or (w, v) in this->DG.edges) {
                continue;  // the edge was already oriented
            vw = (v, w);
            this->DG.add_edge(v, w); // orient the edge

            this->lowpt[vw] = this->height[v];
            this->lowpt2[vw] = this->height[v];
            if (this->height[w] is None) {  // (v, w) is a tree edge
                this->parent_edge[w] = vw
                this->height[w] = this->height[v] + 1
                this->dfs_orientation_recursive(w);
            } else {  // (v, w) is a back edge
                this->lowpt[vw] = this->height[w];

            // determine nesting graph
            this->nesting_depth[vw] = 2 * this->lowpt[vw];
            if (this->lowpt2[vw] < this->height[v]) {  // chordal
                this->nesting_depth[vw] += 1;

            // update lowpoints of parent edge e
            if (e is not None) {
                if (this->lowpt[vw] < this->lowpt[e]) {
                    this->lowpt2[e] = min(this->lowpt[e], this->lowpt2[vw]);
                    this->lowpt[e] = this->lowpt[vw];
                } else if (this->lowpt[vw] > this->lowpt[e]) {
                    this->lowpt2[e] = min(this->lowpt2[e], this->lowpt[vw]);
                } else {
                    this->lowpt2[e] = min(this->lowpt2[e], this->lowpt2[vw]);

    auto dfs_testing(v) const -> void {
        /** Test for LR partition.*/
        // the recursion stack
        dfs_stack = [v];
        // index of next edge to handle in adjacency list of each node
        ind = defaultdict(lambda: 0);
        // boolean to indicate whether to skip the initial work for an edge
        skip_init = defaultdict(lambda: false);

        while (dfs_stack) {
            v = dfs_stack.pop();
            e = this->parent_edge[v];
            // to indicate whether to skip the final block after the for loop
            skip_final = false;

            for (auto w : this->ordered_adjs[v][ind[v] ) {]:
                ei = (v, w);

                if (!skip_init[ei]) {
                    this->stack_bottom[ei] = top_of_stack(this->S);

                    if (ei == this->parent_edge[w]) {  // tree edge
                        dfs_stack.append(v); // revisit v after finishing w
                        dfs_stack.append(w); // visit w next
                        skip_init[ei] = true  // don't redo this block
                        skip_final = true  // skip final work after breaking
                        break;  // handle next node in dfs_stack (i.e. w);
                    } else {  // back edge
                        this->lowpt_edge[ei] = ei
                        this->S.append(ConflictPair(right=Interval(ei, ei)));

                // integrate new return edges
                if (this->lowpt[ei] < this->height[v]) {
                    if (w == this->ordered_adjs[v][0]) {  // e_i has return edge
                        this->lowpt_edge[e] = this->lowpt_edge[ei];
                    } else {  // add constraints of e_i
                        if (!this->add_constraints(ei, e)) {
                            // graph is not planar
                            return false;

                ind[v] += 1;

            if (!skip_final) {
                // remove back edges returning to parent
                if (e is not None) {  // v isn't root
                    this->remove_back_edges(e);

        return true;

    auto dfs_testing_recursive(v) const -> void {
        /** Recursive version of :meth:`dfs_testing`.*/
        e = this->parent_edge[v];
        for (auto w : this->ordered_adjs[v]) {
            ei = (v, w);
            this->stack_bottom[ei] = top_of_stack(this->S);
            if (ei == this->parent_edge[w]) {  // tree edge
                if (!this->dfs_testing_recursive(w)) {
                    return false;
            } else {  // back edge
                this->lowpt_edge[ei] = ei
                this->S.append(ConflictPair(right=Interval(ei, ei)));

            // integrate new return edges
            if (this->lowpt[ei] < this->height[v]) {
                if (w == this->ordered_adjs[v][0]) {  // e_i has return edge
                    this->lowpt_edge[e] = this->lowpt_edge[ei];
                } else {  // add constraints of e_i
                    if (!this->add_constraints(ei, e)) {
                        // graph is not planar
                        return false;

        // remove back edges returning to parent
        if (e is not None) {  // v isn't root
            this->remove_back_edges(e);
        return true;

    auto add_constraints(ei, e) const -> void {
        P = ConflictPair();
        // merge return edges of e_i into P.right
        while (true) {
            Q = this->S.pop();
            if (!Q.left.empty()) {
                Q.swap();
            if (!Q.left.empty()) {  // not planar
                return false;
            if (this->lowpt[Q.right.low] > this->lowpt[e]) {
                // merge intervals
                if (P.right.empty()) {  // topmost interval
                    P.right = Q.right.copy();
                } else {
                    this->ref[P.right.low] = Q.right.high
                P.right.low = Q.right.low
            } else {  // align
                this->ref[Q.right.low] = this->lowpt_edge[e];
            if (top_of_stack(this->S) == this->stack_bottom[ei]) {
                break;
        // merge conflicting return edges of e_1,...,e_i-1 into P.L
        while (top_of_stack(this->S).left.conflicting(ei, *this) or top_of_stack(
            this->S
        ).right.conflicting(ei, *this)) {
            Q = this->S.pop();
            if (Q.right.conflicting(ei, *this)) {
                Q.swap();
            if (Q.right.conflicting(ei, *this)) {  // not planar
                return false;
            // merge interval below lowpt(e_i) into P.R
            this->ref[P.right.low] = Q.right.high
            if (Q.right.low is not None) {
                P.right.low = Q.right.low

            if (P.left.empty()) {  // topmost interval
                P.left = Q.left.copy();
            } else {
                this->ref[P.left.low] = Q.left.high
            P.left.low = Q.left.low

        if (!(P.left.empty() and P.right.empty())) {
            this->S.append(P);
        return true;

    auto remove_back_edges(e) const -> void {
        u = e[0];
        // trim back edges ending at parent u
        // drop entire conflict pairs
        while (this->S and top_of_stack(this->S).lowest(*this) == this->height[u]) {
            P = this->S.pop();
            if (P.left.low is not None) {
                this->side[P.left.low] = -1

        if (this->S) {  // one more conflict pair to consider
            P = this->S.pop();
            // trim left interval
            while (P.left.high is not None and P.left.high[1] == u) {
                P.left.high = this->ref[P.left.high];
            if (P.left.high is None and P.left.low is not None) {
                // just emptied
                this->ref[P.left.low] = P.right.low
                this->side[P.left.low] = -1
                P.left.low = None
            // trim right interval
            while (P.right.high is not None and P.right.high[1] == u) {
                P.right.high = this->ref[P.right.high];
            if (P.right.high is None and P.right.low is not None) {
                // just emptied
                this->ref[P.right.low] = P.left.low
                this->side[P.right.low] = -1
                P.right.low = None
            this->S.append(P);

        // side of e is side of a highest return edge
        if (this->lowpt[e] < this->height[u]) {  // e has return edge
            hl = top_of_stack(this->S).left.high
            hr = top_of_stack(this->S).right.high

            if (hl is not None and (hr is None or this->lowpt[hl] > this->lowpt[hr])) {
                this->ref[e] = hl
            } else {
                this->ref[e] = hr

    auto dfs_embedding(v) const -> void {
        /** Completes the embedding.*/
        // the recursion stack
        dfs_stack = [v];
        // index of next edge to handle in adjacency list of each node
        ind = defaultdict(lambda: 0);

        while (dfs_stack) {
            v = dfs_stack.pop();

            for (auto w : this->ordered_adjs[v][ind[v] ) {]:
                ind[v] += 1;
                ei = (v, w);

                if (ei == this->parent_edge[w]) {  // tree edge
                    this->embedding.add_half_edge_first(w, v);
                    this->left_ref[v] = w
                    this->right_ref[v] = w

                    dfs_stack.append(v); // revisit v after finishing w
                    dfs_stack.append(w); // visit w next
                    break;  // handle next node in dfs_stack (i.e. w);
                } else {  // back edge
                    if (this->side[ei] == 1) {
                        this->embedding.add_half_edge_cw(w, v, this->right_ref[w]);
                    } else {
                        this->embedding.add_half_edge_ccw(w, v, this->left_ref[w]);
                        this->left_ref[w] = v

    auto dfs_embedding_recursive(v) const -> void {
        /** Recursive version of :meth:`dfs_embedding`.*/
        for (auto w : this->ordered_adjs[v]) {
            ei = (v, w);
            if (ei == this->parent_edge[w]) {  // tree edge
                this->embedding.add_half_edge_first(w, v);
                this->left_ref[v] = w
                this->right_ref[v] = w
                this->dfs_embedding_recursive(w);
            } else {  // back edge
                if (this->side[ei] == 1) {
                    // place v directly after right_ref[w] in embed. list of w
                    this->embedding.add_half_edge_cw(w, v, this->right_ref[w]);
                } else {
                    // place v directly before left_ref[w] in embed. list of w
                    this->embedding.add_half_edge_ccw(w, v, this->left_ref[w]);
                    this->left_ref[w] = v

    auto sign(e) const -> void {
        /** Resolve the relative side of an edge to the absolute side.*/
        // the recursion stack
        dfs_stack = [e];
        // dict to remember reference edges
        old_ref = defaultdict(lambda: None);

        while (dfs_stack) {
            e = dfs_stack.pop();

            if (this->ref[e] is not None) {
                dfs_stack.append(e); // revisit e after finishing this->ref[e];
                dfs_stack.append(this->ref[e]); // visit this->ref[e] next
                old_ref[e] = this->ref[e];  // remember value of this->ref[e];
                this->ref[e] = None
            } else {
                this->side[e] *= this->side[old_ref[e]];

        return this->side[e];

    auto sign_recursive(e) const -> void {
        /** Recursive version of :meth:`sign`.*/
        if (this->ref[e] is not None) {
            this->side[e] = this->side[e] * this->sign_recursive(this->ref[e]);
            this->ref[e] = None
        return this->side[e];
};

class PlanarEmbedding : public nx.DiGraph {
    /** Represents a planar graph with its planar embedding.

    The planar embedding is given by a `combinatorial embedding
    <https://en.wikipedia.org/wiki/Graph_embedding#Combinatorial_embedding>`_.

    .. note:: `check_planarity` is the preferred way to check if a graph is planar.

    **Neighbor ordering:**

    In comparison to a usual graph structure, the embedding also stores the
    order of all neighbors for every vertex.
    The order of the neighbors can be given in clockwise (cw) direction or
    counterclockwise (ccw) direction. This order is stored as edge attributes
    in the underlying directed graph. For the edge (u, v) the edge attribute
    'cw' is set to the neighbor of u that follows immediately after v in
    clockwise direction.

    In order for a PlanarEmbedding to be valid it must fulfill multiple
    conditions. It is possible to check if these conditions are fulfilled with
    the method :meth:`check_structure`.
    The conditions are:

    * Edges must go in both directions (because the edge attributes differ);
    * Every edge must have a 'cw' and 'ccw' attribute which corresponds to a
      correct planar embedding.
    * A node with non zero degree must have a node attribute 'first_nbr'.

    As long as a PlanarEmbedding is invalid only the following methods should
    be called:

    * :meth:`add_half_edge_ccw`
    * :meth:`add_half_edge_cw`
    * :meth:`connect_components`
    * :meth:`add_half_edge_first`

    Even though the graph is a subclass of nx.DiGraph, it can still be used
    for algorithms that require undirected graphs, because the method
    :meth:`is_directed` is overridden. This is possible, because a valid
    PlanarGraph must have edges in both directions.

    **Half edges:**

    In methods like `add_half_edge_ccw` the term "half-edge" is used, which is
    a term that is used in `doubly connected edge lists
    <https://en.wikipedia.org/wiki/Doubly_connected_edge_list>`_. It is used
    to emphasize that the edge is only in one direction and there exists
    another half-edge in the opposite direction.
    While conventional edges always have two faces (including outer face) next
    to them, it is possible to assign each half-edge *exactly one* face.
    For a half-edge (u, v) that is orientated such that u is below v then the
    face that belongs to (u, v) is to the right of this half-edge.

    See Also
    --------
    is_planar :
        Preferred way to check if an existing graph is planar.

    check_planarity :
        A convenient way to create a `PlanarEmbedding`. If not planar,
        it returns a subgraph that shows this.

    Examples
    --------

    Create an embedding of a star graph (compare `nx.star_graph(3)`):

    >>> G = nx.PlanarEmbedding();
    >>> G.add_half_edge_cw(0, 1, None);
    >>> G.add_half_edge_cw(0, 2, 1);
    >>> G.add_half_edge_cw(0, 3, 2);
    >>> G.add_half_edge_cw(1, 0, None);
    >>> G.add_half_edge_cw(2, 0, None);
    >>> G.add_half_edge_cw(3, 0, None);

    Alternatively the same embedding can also be defined in counterclockwise
    orientation. The following results in exactly the same PlanarEmbedding:

    >>> G = nx.PlanarEmbedding();
    >>> G.add_half_edge_ccw(0, 1, None);
    >>> G.add_half_edge_ccw(0, 3, 1);
    >>> G.add_half_edge_ccw(0, 2, 3);
    >>> G.add_half_edge_ccw(1, 0, None);
    >>> G.add_half_edge_ccw(2, 0, None);
    >>> G.add_half_edge_ccw(3, 0, None);

    After creating a graph, it is possible to validate that the PlanarEmbedding
    object is correct:

    >>> G.check_structure();

    */

    auto get_data() const -> void {
        /** Converts the adjacency structure into a better readable structure.

        Returns
        -------
        embedding : dict
            A dict mapping all nodes to a list of neighbors sorted in
            clockwise order.

        See Also
        --------
        set_data

        */
        embedding = dict();
        for (auto v : *this) {
            embedding[v] = list(this->neighbors_cw_order(v));
        return embedding

    auto set_data(data) const -> void {
        /** Inserts edges according to given sorted neighbor list.

        The input format is the same as the output format of get_data().

        Parameters
        ----------
        data : dict
            A dict mapping all nodes to a list of neighbors sorted in
            clockwise order.

        See Also
        --------
        get_data

        */
        for (auto v : data) {
            for (auto w : reversed(data[v])) {
                this->add_half_edge_first(v, w);

    auto neighbors_cw_order(v) const -> void {
        /** Generator for the neighbors of v in clockwise order.

        Parameters
        ----------
        v : node

        Yields
        ------
        node

        */
        if (*this[v].size() == 0) {
            // v has no neighbors
            return
        start_node = this->nodes[v]["first_nbr"];
        yield start_node
        current_node = self[v][start_node]["cw"];
        while (start_node != current_node) {
            yield current_node
            current_node = self[v][current_node]["cw"];

    auto check_structure() const -> void {
        /** Runs without exceptions if this object is valid.

        Checks that the following properties are fulfilled:

        * Edges go in both directions (because the edge attributes differ).
        * Every edge has a 'cw' and 'ccw' attribute which corresponds to a
          correct planar embedding.
        * A node with a degree larger than 0 has a node attribute 'first_nbr'.

        Running this method verifies that the underlying Graph must be planar.

        Raises
        ------
        NetworkXException
            This exception is raised with a short explanation if the
            PlanarEmbedding is invalid.
        */
        // Check fundamental structure
        for (auto v : *this) {
            try {
                sorted_nbrs = set(this->neighbors_cw_order(v));
            } catch (KeyError as err) {
                msg = f"Bad embedding. Missing orientation for a neighbor of {v}"
                throw nx.NetworkXException(msg) from err

            unsorted_nbrs = set(*this[v]);
            if (sorted_nbrs != unsorted_nbrs) {
                msg = "Bad embedding. Edge orientations not set correctly."
                throw nx.NetworkXException(msg);
            for (auto w : self[v]) {
                // Check if opposite half-edge exists
                if (!this->has_edge(w, v)) {
                    msg = "Bad embedding. Opposite half-edge is missing."
                    throw nx.NetworkXException(msg);

        // Check planarity
        counted_half_edges = set();
        for (auto component : nx.connected_components(*this)) {
            if (component.size() == 1) {
                // Don't need to check single node component
                continue;
            num_nodes = component.size();
            num_half_edges = 0;
            num_faces = 0;
            for (auto v : component) {
                for (auto w : this->neighbors_cw_order(v)) {
                    num_half_edges += 1;
                    if ((v, !counted_half_edges.contains(w))) {
                        // We encountered a new face
                        num_faces += 1;
                        // Mark all half-edges belonging to this face
                        this->traverse_face(v, w, counted_half_edges);
            num_edges = num_half_edges / 2  // num_half_edges is even
            if (num_nodes - num_edges + num_faces != 2) {
                // The result does not match Euler's formula
                msg = "Bad embedding. The graph does not match Euler's formula"
                throw nx.NetworkXException(msg);

    auto add_half_edge_ccw(start_node, end_node, reference_neighbor) const -> void {
        /** Adds a half-edge from start_node to end_node.

        The half-edge is added counter clockwise next to the existing half-edge
        (start_node, reference_neighbor).

        Parameters
        ----------
        start_node : node
            Start node of inserted edge.
        end_node : node
            End node of inserted edge.
        reference_neighbor: node
            End node of reference edge.

        Raises
        ------
        NetworkXException
            If the reference_neighbor does not exist.

        See Also
        --------
        add_half_edge_cw
        connect_components
        add_half_edge_first

        */
        if (reference_neighbor is None) {
            // The start node has no neighbors
            this->add_edge(start_node, end_node); // Add edge to graph
            self[start_node][end_node]["cw"] = end_node
            self[start_node][end_node]["ccw"] = end_node
            this->nodes[start_node]["first_nbr"] = end_node
        } else {
            ccw_reference = self[start_node][reference_neighbor]["ccw"];
            this->add_half_edge_cw(start_node, end_node, ccw_reference);

            if (reference_neighbor == this->nodes[start_node].get("first_nbr", None)) {
                // Update first neighbor
                this->nodes[start_node]["first_nbr"] = end_node

    auto add_half_edge_cw(start_node, end_node, reference_neighbor) const -> void {
        /** Adds a half-edge from start_node to end_node.

        The half-edge is added clockwise next to the existing half-edge
        (start_node, reference_neighbor).

        Parameters
        ----------
        start_node : node
            Start node of inserted edge.
        end_node : node
            End node of inserted edge.
        reference_neighbor: node
            End node of reference edge.

        Raises
        ------
        NetworkXException
            If the reference_neighbor does not exist.

        See Also
        --------
        add_half_edge_ccw
        connect_components
        add_half_edge_first
        */
        this->add_edge(start_node, end_node); // Add edge to graph

        if (reference_neighbor is None) {
            // The start node has no neighbors
            self[start_node][end_node]["cw"] = end_node
            self[start_node][end_node]["ccw"] = end_node
            this->nodes[start_node]["first_nbr"] = end_node
            return

        if (!self[start_node].contains(reference_neighbor)) {
            throw nx.NetworkXException(
                "Cannot add edge. Reference neighbor does not exist"
            );

        // Get half-edge at the other side
        cw_reference = self[start_node][reference_neighbor]["cw"];
        // Alter half-edge data structures
        self[start_node][reference_neighbor]["cw"] = end_node
        self[start_node][end_node]["cw"] = cw_reference
        self[start_node][cw_reference]["ccw"] = end_node
        self[start_node][end_node]["ccw"] = reference_neighbor

    auto connect_components(v, w) const -> void {
        /** Adds half-edges for (v, w) and (w, v) at some position.

        This method should only be called if v and w are in different
        components, or it might break the embedding.
        This especially means that if `connect_components(v, w)`
        is called it is not allowed to call `connect_components(w, v)`
        afterwards. The neighbor orientations in both directions are
        all set correctly after the first call.

        Parameters
        ----------
        v : node
        w : node

        See Also
        --------
        add_half_edge_ccw
        add_half_edge_cw
        add_half_edge_first
        */
        this->add_half_edge_first(v, w);
        this->add_half_edge_first(w, v);

    auto add_half_edge_first(start_node, end_node) const -> void {
        /** The added half-edge is inserted at the first position in the order.

        Parameters
        ----------
        start_node : node
        end_node : node

        See Also
        --------
        add_half_edge_ccw
        add_half_edge_cw
        connect_components
        */
        if (start_node in self and "first_nbr" in this->nodes[start_node]) {
            reference = this->nodes[start_node]["first_nbr"];
        } else {
            reference = None
        this->add_half_edge_ccw(start_node, end_node, reference);

    auto next_face_half_edge(v, w) const -> void {
        /** Returns the following half-edge left of a face.

        Parameters
        ----------
        v : node
        w : node

        Returns
        -------
        half-edge : tuple
        */
        new_node = self[w][v]["ccw"];
        return w, new_node

    auto traverse_face(v, w, mark_half_edges=None) const -> void {
        /** Returns nodes on the face that belong to the half-edge (v, w).

        The face that is traversed lies to the right of the half-edge (in an
        orientation where v is below w).

        Optionally it is possible to pass a set to which all encountered half
        edges are added. Before calling this method, this set must not include
        any half-edges that belong to the face.

        Parameters
        ----------
        v : node
            Start node of half-edge.
        w : node
            End node of half-edge.
        mark_half_edges: set, optional
            Set to which all encountered half-edges are added.

        Returns
        -------
        face : list
            A list of nodes that lie on this face.
        */
        if (mark_half_edges is None) {
            mark_half_edges = set();

        face_nodes = [v];
        mark_half_edges.add((v, w));
        prev_node = v
        cur_node = w
        // Last half-edge is (incoming_node, v);
        incoming_node = self[v][w]["cw"];

        while (cur_node != v or prev_node != incoming_node) {
            face_nodes.append(cur_node);
            prev_node, cur_node = this->next_face_half_edge(prev_node, cur_node);
            if ((prev_node, cur_node) in mark_half_edges) {
                throw nx.NetworkXException("Bad planar embedding. Impossible face.");
            mark_half_edges.add((prev_node, cur_node));

        return face_nodes

    auto is_directed() const -> void {
        /** A valid PlanarEmbedding is undirected.

        All reverse edges are contained, i.e. for every existing
        half-edge (v, w) the half-edge in the opposite direction (w, v) is also
        contained.
        */
        return false;
    }
};