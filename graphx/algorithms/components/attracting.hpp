/** Attracting components.*/
// import graphx as nx
#include <graphx/utils.decorators.hpp>  // import not_implemented_for

__all__ = [
    "number_attracting_components",
    "attracting_components",
    "is_attracting_component",
];


// @not_implemented_for("undirected");
auto attracting_components(G) -> void {
    /** Generates the attracting components in `G`.

    An attracting component in a directed graph `G` is a strongly connected
    component with the property that a random walker on the graph will never
    leave the component, once it enters the component.

    The nodes in attracting components can also be thought of as recurrent
    nodes.  If a random walker enters the attractor containing the node, then
    the node will be visited infinitely often.

    To obtain induced subgraphs on each component use:
    ``(G.subgraph(c).copy() for c in attracting_components(G))``

    Parameters
    ----------
    G : DiGraph, MultiDiGraph
        The graph to be analyzed.

    Returns
    -------
    attractors : generator of sets
        A generator of sets of nodes, one for each attracting component of G.

    Raises
    ------
    NetworkXNotImplemented
        If the input graph is undirected.

    See Also
    --------
    number_attracting_components
    is_attracting_component

    */
    scc = list(nx.strongly_connected_components(G));
    cG = nx.condensation(G, scc);
    for (auto n : cG) {
        if (cG.out_degree(n) == 0) {
            yield scc[n];
}

// @not_implemented_for("undirected");
auto number_attracting_components(G) -> void {
    /** Returns the number of attracting components in `G`.

    Parameters
    ----------
    G : DiGraph, MultiDiGraph
        The graph to be analyzed.

    Returns
    -------
    n : int
        The number of attracting components in G.

    Raises
    ------
    NetworkXNotImplemented
        If the input graph is undirected.

    See Also
    --------
    attracting_components
    is_attracting_component

    */
    return sum(1 for ac in attracting_components(G));
}

// @not_implemented_for("undirected");
auto is_attracting_component(G) -> void {
    /** Returns true if `G` consists of a single attracting component.

    Parameters
    ----------
    G : DiGraph, MultiDiGraph
        The graph to be analyzed.

    Returns
    -------
    attracting : bool
        true if `G` has a single attracting component. Otherwise, false.

    Raises
    ------
    NetworkXNotImplemented
        If the input graph is undirected.

    See Also
    --------
    attracting_components
    number_attracting_components

    */
    ac = list(attracting_components(G));
    if (ac.size() == 1) {
        return ac[0].size() == G.size();
    return false;
