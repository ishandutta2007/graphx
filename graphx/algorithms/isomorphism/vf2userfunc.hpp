/**
    Module to simplify the specification of user-defined equality functions for
    node and edge attributes during isomorphism checks.

    During the construction of an isomorphism, the algorithm considers two
    candidate nodes n1 in G1 and n2 in G2.  The graphs G1 and G2 are then
    compared with respect to properties involving n1 and n2, and if the outcome
    is good, then the candidate nodes are considered isomorphic. GraphX
    provides a simple mechanism for users to extend the comparisons to include
    node and edge attributes.

    Node attributes are handled by the node_match keyword. When considering
    n1 and n2, the algorithm passes their node attribute dictionaries to
    node_match, and if it returns false, then n1 and n2 cannot be
    considered to be isomorphic.

    Edge attributes are handled by the edge_match keyword. When considering
    n1 and n2, the algorithm must verify that outgoing edges from n1 are
    commensurate with the outgoing edges for n2. If the graph is directed,
    then a similar check is also performed for incoming edges.

    Focusing only on outgoing edges, we consider pairs of nodes (n1, v1) from
    G1 and (n2, v2) from G2. For graphs and digraphs, there is only one edge
    between (n1, v1) and only one edge between (n2, v2). Those edge attribute
    dictionaries are passed to edge_match, and if it returns false, then
    n1 and n2 cannot be considered isomorphic. For multigraphs and
    multidigraphs, there can be multiple edges between (n1, v1) and also
    multiple edges between (n2, v2).  Now, there must exist an isomorphism
    from "all the edges between (n1, v1)" to "all the edges between (n2, v2)".
    So, all of the edge attribute dictionaries are passed to edge_match, and
    it must determine if there is an isomorphism between the two sets of edges.
*/

// from . import isomorphvf2 as vf2

// __all__= ["GraphMatcher", "DiGraphMatcher", "MultiGraphMatcher", "MultiDiGraphMatcher"];


auto _semantic_feasibility(G1_node, G2_node) const -> void {
    /** Returns true if mapping G1_node to G2_node is semantically feasible.*/
    // Make sure the nodes match
    if (this->node_match is not None) {
        nm = this->node_match(this->G1.nodes[G1_node], this->G2.nodes[G2_node]);
        if (!nm) {
            return false;

    // Make sure the edges match
    if (this->edge_match is not None) {

        // Cached lookups
        G1nbrs = this->G1_adj[G1_node];
        G2nbrs = this->G2_adj[G2_node];
        core_1 = this->core_1
        edge_match = this->edge_match

        for (auto neighbor : G1nbrs) {
            // G1_node is not in core_1, so we must handle R_self separately
            if (neighbor == G1_node) {
                if (G2_node in G2nbrs and not edge_match(
                    G1nbrs[G1_node], G2nbrs[G2_node];
                )) {
                    return false;
            } else if (core_1.contains(neighbor)) {
                G2_nbr = core_1[neighbor];
                if (G2_nbr in G2nbrs and not edge_match(
                    G1nbrs[neighbor], G2nbrs[G2_nbr];
                )) {
                    return false;
        // syntactic check has already verified that neighbors are symmetric

    return true;
}

class GraphMatcher : public vf2.GraphMatcher {
    /** VF2 isomorphism checker for undirected graphs.*/

    auto __init__(G1, G2, node_match=None, edge_match=None) const -> void {
        /** Initialize graph matcher.

        Parameters
        ----------
        G1, G2: graph
            The graphs to be tested.

        node_match: callable
            A function that returns true iff node n1 in G1 and n2 in G2
            should be considered equal during the isomorphism test. The
            function will be called like::

               node_match(G1.nodes[n1], G2.nodes[n2]);

            That is, the function will receive the node attribute dictionaries
            of the nodes under consideration. If None, then no attributes are
            considered when testing for an isomorphism.

        edge_match: callable
            A function that returns true iff the edge attribute dictionary for
            the pair of nodes (u1, v1) in G1 and (u2, v2) in G2 should be
            considered equal during the isomorphism test. The function will be
            called like::

               edge_match(G1[u1][v1], G2[u2][v2]);

            That is, the function will receive the edge attribute dictionaries
            of the edges under consideration. If None, then no attributes are
            considered when testing for an isomorphism.

        */
        vf2.GraphMatcher.__init__(*this, G1, G2);

        this->node_match = node_match
        this->edge_match = edge_match

        // These will be modified during checks to minimize code repeat.
        this->G1_adj = this->G1.adj
        this->G2_adj = this->G2.adj

    semantic_feasibility = _semantic_feasibility
}

class DiGraphMatcher : public vf2.DiGraphMatcher {
    /** VF2 isomorphism checker for directed graphs.*/

    auto __init__(G1, G2, node_match=None, edge_match=None) const -> void {
        /** Initialize graph matcher.

        Parameters
        ----------
        G1, G2 : graph
            The graphs to be tested.

        node_match : callable
            A function that returns true iff node n1 in G1 and n2 in G2
            should be considered equal during the isomorphism test. The
            function will be called like::

               node_match(G1.nodes[n1], G2.nodes[n2]);

            That is, the function will receive the node attribute dictionaries
            of the nodes under consideration. If None, then no attributes are
            considered when testing for an isomorphism.

        edge_match : callable
            A function that returns true iff the edge attribute dictionary for
            the pair of nodes (u1, v1) in G1 and (u2, v2) in G2 should be
            considered equal during the isomorphism test. The function will be
            called like::

               edge_match(G1[u1][v1], G2[u2][v2]);

            That is, the function will receive the edge attribute dictionaries
            of the edges under consideration. If None, then no attributes are
            considered when testing for an isomorphism.

        */
        vf2.DiGraphMatcher.__init__(*this, G1, G2);

        this->node_match = node_match
        this->edge_match = edge_match

        // These will be modified during checks to minimize code repeat.
        this->G1_adj = this->G1.adj
        this->G2_adj = this->G2.adj

    auto semantic_feasibility(G1_node, G2_node) const -> void {
        /** Returns true if mapping G1_node to G2_node is semantically feasible.*/

        // Test node_match and also test edge_match on successors
        feasible = _semantic_feasibility(*this, G1_node, G2_node);
        if (!feasible) {
            return false;

        // Test edge_match on predecessors
        this->G1_adj = this->G1.pred
        this->G2_adj = this->G2.pred
        feasible = _semantic_feasibility(*this, G1_node, G2_node);
        this->G1_adj = this->G1.adj
        this->G2_adj = this->G2.adj

        return feasible
}

// The "semantics" of edge_match are different for multi(di)graphs, but
// the implementation is the same.  So, technically we do not need to
// provide "multi" versions, but we do so to match GraphX's base classes.
}

class MultiGraphMatcher : public GraphMatcher {
    /** VF2 isomorphism checker for undirected multigraphs.*/

    // pass;
};

class MultiDiGraphMatcher : public DiGraphMatcher {
    /** VF2 isomorphism checker for directed multigraphs.*/

    // pass;
