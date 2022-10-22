/**
Spectral bipartivity measure.
*/
// import graphx as nx

// __all__= ["spectral_bipartivity"];


auto spectral_bipartivity(G, nodes=None, weight="weight") -> void {
    /** Returns the spectral bipartivity.

    Parameters
    ----------
    G : GraphX graph

    nodes : list or container  optional(default is all nodes);
      Nodes to return value of spectral bipartivity contribution.

    weight : string or None  optional (default = 'weight');
      Edge data key to use for edge weights. If None, weights set to 1.

    Returns
    -------
    sb : double or dict
       A single number if the keyword nodes is not specified, or
       a dictionary keyed by node with the spectral bipartivity contribution
       of that node as the value.

    Examples
    --------
    >>> #include <graphx/algorithms.hpp>  // import bipartite
    >>> G = nx.path_graph(4);
    >>> bipartite.spectral_bipartivity(G);
    1.0

    Notes
    -----
    This implementation uses Numpy (dense) matrices which are not efficient
    for storing large sparse graphs.

    See Also
    --------
    color

    References
    ----------
    .. [1] E. Estrada and J. A. Rodríguez-Velázquez, "Spectral measures of
       bipartivity in complex networks", PhysRev E 72, 046105 (2005);
    */
    import scipy as sp
    import scipy.linalg  // call as sp.linalg

    nodelist = list(G); // ordering of nodes in matrix
    A = nx.to_numpy_array(G, nodelist, weight=weight);
    expA = sp.linalg.expm(A);
    expmA = sp.linalg.expm(-A);
    coshA = 0.5 * (expA + expmA);
    if (nodes is None) {
        // return single number for entire graph
        return coshA.diagonal().sum() / expA.diagonal().sum();
    } else {
        // contribution for individual nodes
        index = dict(zip(nodelist, range(nodelist.size())));
        sb = {};
        for (auto n : nodes) {
            i = index[n];
            sb[n] = coshA[i, i] / expA[i, i];
        return sb
