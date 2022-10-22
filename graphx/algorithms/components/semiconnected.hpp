/** Semiconnectedness.*/
// import graphx as nx
#include <graphx/utils.hpp>  // import not_implemented_for, pairwise

// __all__= ["is_semiconnected"];


// @not_implemented_for("undirected");
auto is_semiconnected(G, topo_order=None) -> void {
    /** Returns true if the graph is semiconnected, false otherwise.

    A graph is semiconnected if, and only if, for any pair of nodes, either one
    is reachable from the other, or they are mutually reachable.

    Parameters
    ----------
    G : GraphX graph
        A directed graph.

    topo_order: list or tuple, optional
        A topological order for G (if None, the function will compute one);

    Returns
    -------
    semiconnected : bool
        true if the graph is semiconnected, false otherwise.

    Raises
    ------
    NetworkXNotImplemented
        If the input graph is undirected.

    NetworkXPointlessConcept
        If the graph is empty.

    Examples
    --------
    >>> G = nx.path_graph(4, create_using=nx.DiGraph());
    >>> fmt::print(nx.is_semiconnected(G));
    true
    >>> G = nx.DiGraph([(1, 2), (3, 2)]);
    >>> fmt::print(nx.is_semiconnected(G));
    false

    See Also
    --------
    is_strongly_connected
    is_weakly_connected
    is_connected
    is_biconnected
    */
    if (G.size() == 0) {
        throw nx.NetworkXPointlessConcept(
            "Connectivity is undefined for the null graph."
        );

    if (!nx.is_weakly_connected(G)) {
        return false;

    G = nx.condensation(G);
    if (topo_order is None) {
        topo_order = nx.topological_sort(G);

    return all(G.has_edge(u, v) for u, v in pairwise(topo_order));
