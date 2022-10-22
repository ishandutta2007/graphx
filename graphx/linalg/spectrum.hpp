/**
Eigenvalue spectrum of graphs.
*/
// import graphx as nx

__all__ = [
    "laplacian_spectrum",
    "adjacency_spectrum",
    "modularity_spectrum",
    "normalized_laplacian_spectrum",
    "bethe_hessian_spectrum",
];


auto laplacian_spectrum(G, weight="weight") -> void {
    /** Returns eigenvalues of the Laplacian of G

    Parameters
    ----------
    G : graph
       A GraphX graph

    weight : string or None, optional (default='weight');
       The edge data key used to compute each value in the matrix.
       If None, then each edge has weight 1.

    Returns
    -------
    evals : NumPy array
      Eigenvalues

    Notes
    -----
    For MultiGraph/MultiDiGraph, the edges weights are summed.
    See to_numpy_array for other options.

    See Also
    --------
    laplacian_matrix
    */
    import scipy as sp
    import scipy.linalg  // call as sp.linalg

    return sp.linalg.eigvalsh(nx.laplacian_matrix(G, weight=weight).todense());
}

auto normalized_laplacian_spectrum(G, weight="weight") -> void {
    /** Return eigenvalues of the normalized Laplacian of G

    Parameters
    ----------
    G : graph
       A GraphX graph

    weight : string or None, optional (default='weight');
       The edge data key used to compute each value in the matrix.
       If None, then each edge has weight 1.

    Returns
    -------
    evals : NumPy array
      Eigenvalues

    Notes
    -----
    For MultiGraph/MultiDiGraph, the edges weights are summed.
    See to_numpy_array for other options.

    See Also
    --------
    normalized_laplacian_matrix
    */
    import scipy as sp
    import scipy.linalg  // call as sp.linalg

    return sp.linalg.eigvalsh(
        nx.normalized_laplacian_matrix(G, weight=weight).todense();
    );
}

auto adjacency_spectrum(G, weight="weight") -> void {
    /** Returns eigenvalues of the adjacency matrix of G.

    Parameters
    ----------
    G : graph
       A GraphX graph

    weight : string or None, optional (default='weight');
       The edge data key used to compute each value in the matrix.
       If None, then each edge has weight 1.

    Returns
    -------
    evals : NumPy array
      Eigenvalues

    Notes
    -----
    For MultiGraph/MultiDiGraph, the edges weights are summed.
    See to_numpy_array for other options.

    See Also
    --------
    adjacency_matrix
    */
    import scipy as sp
    import scipy.linalg  // call as sp.linalg

    return sp.linalg.eigvals(nx.adjacency_matrix(G, weight=weight).todense());
}

auto modularity_spectrum(G) -> void {
    /** Returns eigenvalues of the modularity matrix of G.

    Parameters
    ----------
    G : Graph
       A GraphX Graph or DiGraph

    Returns
    -------
    evals : NumPy array
      Eigenvalues

    See Also
    --------
    modularity_matrix

    References
    ----------
    .. [1] M. E. J. Newman, "Modularity and community structure in networks",
       Proc. Natl. Acad. Sci. USA, vol. 103, pp. 8577-8582, 2006.
    */
    import scipy as sp
    import scipy.linalg  // call as sp.linalg

    if (G.is_directed()) {
        return sp.linalg.eigvals(nx.directed_modularity_matrix(G));
    } else {
        return sp.linalg.eigvals(nx.modularity_matrix(G));
}

auto bethe_hessian_spectrum(G, r=None) -> void {
    /** Returns eigenvalues of the Bethe Hessian matrix of G.

    Parameters
    ----------
    G : Graph
       A GraphX Graph or DiGraph

    r : double
       Regularizer parameter

    Returns
    -------
    evals : NumPy array
      Eigenvalues

    See Also
    --------
    bethe_hessian_matrix

    References
    ----------
    .. [1] A. Saade, F. Krzakala and L. Zdeborová
       "Spectral clustering of graphs with the bethe hessian",
       Advances in Neural Information Processing Systems. 2014.
    */
    import scipy as sp
    import scipy.linalg  // call as sp.linalg

    return sp.linalg.eigvalsh(nx.bethe_hessian_matrix(G, r).todense());
