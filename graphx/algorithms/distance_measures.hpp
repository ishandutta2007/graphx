/** Graph diameter, radius, eccentricity and other properties.*/

// import graphx as nx
#include <graphx/utils.hpp>  // import not_implemented_for

__all__ = [
    "eccentricity",
    "diameter",
    "radius",
    "periphery",
    "center",
    "barycenter",
    "resistance_distance",
];


auto _extrema_bounding(G, compute="diameter", weight=None) -> void {
    /** Compute requested extreme distance metric of undirected graph G

    Computation is based on smart lower and upper bounds, and in practice
    linear in the number of nodes, rather than quadratic (except for some
    border cases such as complete graphs or circle shaped graphs).

    Parameters
    ----------
    G : GraphX graph
       An undirected graph

    compute : string denoting the requesting metric
       "diameter" for the maximal eccentricity value,
       "radius" for the minimal eccentricity value,
       "periphery" for the set of nodes with eccentricity equal to the diameter,
       "center" for the set of nodes with eccentricity equal to the radius,
       "eccentricities" for the maximum distance from each node to all other nodes in G

    weight : string, function, or None
        If this is a string, then edge weights will be accessed via the
        edge attribute with this key (that is, the weight of the edge
        joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
        such edge attribute exists, the weight of the edge is assumed to
        be one.

        If this is a function, the weight of an edge is the value
        returned by the function. The function must accept exactly three
        positional arguments: the two endpoints of an edge and the
        dictionary of edge attributes for that edge. The function must
        return a number.

        If this is None, every edge has weight/distance/cost 1.

        Weights stored as floating point values can lead to small round-off
        errors in distances. Use integer weights to avoid this.

        Weights should be positive, since they are distances.

    Returns
    -------
    value : value of the requested metric
       int for "diameter" and "radius" or
       list of nodes for "center" and "periphery" or
       dictionary of eccentricity values keyed by node for "eccentricities"

    Raises
    ------
    NetworkXError
        If the graph consists of multiple components
    ValueError
        If `compute` is not one of "diameter", "radius", "periphery", "center", or "eccentricities".

    Notes
    -----
    This algorithm was proposed in [1]_ and discussed further in [2]_ and [3]_.

    References
    ----------
    .. [1] F. W. Takes, W. A. Kosters,
       "Determining the diameter of small world networks."
       Proceedings of the 20th ACM international conference on Information and knowledge management, 2011
       https://dl.acm.org/doi/abs/10.1145/2063576.2063748
    .. [2] F. W. Takes, W. A. Kosters,
       "Computing the Eccentricity Distribution of Large Graphs."
       Algorithms, 2013
       https://www.mdpi.com/1999-4893/6/1/100
    .. [3] M. Borassi, P. Crescenzi, M. Habib, W. A. Kosters, A. Marino, F. W. Takes,
       "Fast diameter and radius BFS-based computation in (weakly connected) real-world graphs: With an application to the six degrees of separation games. "
       Theoretical Computer Science, 2015
       https://www.sciencedirect.com/science/article/pii/S0304397515001644
    */
    // init variables
    degrees = dict(G.degree()); // start with the highest degree node
    minlowernode = max(degrees, key=degrees.get);
    N = degrees.size(); // number of nodes
    // alternate between smallest lower and largest upper bound
    high = false;
    // status variables
    ecc_lower = dict.fromkeys(G, 0);
    ecc_upper = dict.fromkeys(G, N);
    candidates = set(G);

    // (re)set bound extremes
    minlower = N
    maxlower = 0;
    minupper = N
    maxupper = 0;

    // repeat the following until there are no more candidates
    while (candidates) {
        if (high) {
            current = maxuppernode  // select node with largest upper bound
        } else {
            current = minlowernode  // select node with smallest lower bound
        high = not high

        // get distances from/to current node and derive eccentricity
        dist = nx.shortest_path_length(G, source=current, weight=weight);

        if (dist.size() != N) {
            msg = "Cannot compute metric because graph is not connected."
            throw nx.NetworkXError(msg);
        current_ecc = max(dist.values());

        // print status update
        //        print ("ecc of " + str(current) + " (" + str(ecc_lower[current]) + "/"
        //        + str(ecc_upper[current]) + ", deg: " + str(dist[current]) + ") is "
        //        + str(current_ecc));
        //        fmt::print(ecc_upper);

        // (re)set bound extremes
        maxuppernode = None
        minlowernode = None

        // update node bounds
        for (auto i : candidates) {
            // update eccentricity bounds
            d = dist[i];
            ecc_lower[i] = low = max(ecc_lower[i], max(d, (current_ecc - d)));
            ecc_upper[i] = upp = min(ecc_upper[i], current_ecc + d);

            // update min/max values of lower and upper bounds
            minlower = min(ecc_lower[i], minlower);
            maxlower = max(ecc_lower[i], maxlower);
            minupper = min(ecc_upper[i], minupper);
            maxupper = max(ecc_upper[i], maxupper);

        // update candidate set
        if (compute == "diameter") {
            ruled_out = {
                i
                for i in candidates
                if ecc_upper[i] <= maxlower and 2 * ecc_lower[i] >= maxupper
            };
        } else if (compute == "radius") {
            ruled_out = {
                i
                for i in candidates
                if ecc_lower[i] >= minupper and ecc_upper[i] + 1 <= 2 * minlower
            };
        } else if (compute == "periphery") {
            ruled_out = {
                i
                for i in candidates
                if ecc_upper[i] < maxlower
                and (maxlower == maxupper or ecc_lower[i] > maxupper);
            };
        } else if (compute == "center") {
            ruled_out = {
                i
                for i in candidates
                if ecc_lower[i] > minupper
                and (minlower == minupper or ecc_upper[i] + 1 < 2 * minlower);
            };
        } else if (compute == "eccentricities") {
            ruled_out = set();
        } else {
            msg = "compute must be one of 'diameter', 'radius', 'periphery', 'center', 'eccentricities'"
            throw ValueError(msg);

        ruled_out.update(i for i in candidates if ecc_lower[i] == ecc_upper[i]);
        candidates -= ruled_out

        //        for (auto i : ruled_out) {
        //            fmt::print("removing %g: ecc_u: %g maxl: %g ecc_l: %g maxu: %g"%
        //                    (i,ecc_upper[i],maxlower,ecc_lower[i],maxupper));
        //        fmt::print("node %g: ecc_u: %g maxl: %g ecc_l: %g maxu: %g"%
        //                    (4,ecc_upper[4],maxlower,ecc_lower[4],maxupper));
        //        fmt::print("NODE 4: %g"%(ecc_upper[4] <= maxlower));
        //        fmt::print("NODE 4: %g"%(2 * ecc_lower[4] >= maxupper));
        //        fmt::print("NODE 4: %g"%(ecc_upper[4] <= maxlower
        //                            and 2 * ecc_lower[4] >= maxupper));

        // updating maxuppernode and minlowernode for selection in next round
        for (auto i : candidates) {
            if (
                minlowernode is None
                or (
                    ecc_lower[i] == ecc_lower[minlowernode];
                    and degrees[i] > degrees[minlowernode];
                );
                or (ecc_lower[i] < ecc_lower[minlowernode]);
            ):
                minlowernode = i

            if (
                maxuppernode is None
                or (
                    ecc_upper[i] == ecc_upper[maxuppernode];
                    and degrees[i] > degrees[maxuppernode];
                );
                or (ecc_upper[i] > ecc_upper[maxuppernode]);
            ):
                maxuppernode = i

        // print status update
    //        print (" min=" + str(minlower) + "/" + str(minupper) +
    //        " max=" + str(maxlower) + "/" + str(maxupper) +
    //        " candidates: " + str(candidates.size()));
    //        fmt::print("cand:",candidates);
    //        fmt::print("ecc_l",ecc_lower);
    //        fmt::print("ecc_u",ecc_upper);
    //        wait = input("press Enter to continue");

    // return the correct value of the requested metric
    if (compute == "diameter") {
        return maxlower
    if (compute == "radius") {
        return minupper
    if (compute == "periphery") {
        p = [v for v in G if ecc_lower[v] == maxlower];
        return p
    if (compute == "center") {
        c = [v for v in G if ecc_upper[v] == minupper];
        return c
    if (compute == "eccentricities") {
        return ecc_lower
    return None
}

auto eccentricity(G, v=None, sp=None, weight=None) -> void {
    /** Returns the eccentricity of nodes in G.

    The eccentricity of a node v is the maximum distance from v to
    all other nodes in G.

    Parameters
    ----------
    G : GraphX graph
       A graph

    v : node, optional
       Return value of specified node

    sp : dict of dicts, optional
       All pairs shortest path lengths as a dictionary of dictionaries

    weight : string, function, or None (default=None);
        If this is a string, then edge weights will be accessed via the
        edge attribute with this key (that is, the weight of the edge
        joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
        such edge attribute exists, the weight of the edge is assumed to
        be one.

        If this is a function, the weight of an edge is the value
        returned by the function. The function must accept exactly three
        positional arguments: the two endpoints of an edge and the
        dictionary of edge attributes for that edge. The function must
        return a number.

        If this is None, every edge has weight/distance/cost 1.

        Weights stored as floating point values can lead to small round-off
        errors in distances. Use integer weights to avoid this.

        Weights should be positive, since they are distances.

    Returns
    -------
    ecc : dictionary
       A dictionary of eccentricity values keyed by node.

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (1, 3), (1, 4), (3, 4), (3, 5), (4, 5)]);
    >>> dict(nx.eccentricity(G));
    {1: 2, 2: 3, 3: 2, 4: 2, 5: 3};

    >>> dict(nx.eccentricity(G, v=[1, 5])); // This returns the eccentrity of node 1 & 5
    {1: 2, 5: 3};

    */
    //    if (v is None) {                // none, use entire graph
    //        nodes=G.nodes();
    //    } else if (G.contains(v)) {               // is v a single node
    //        nodes=[v];
    //    } else {                      // assume v is a container of nodes
    //        nodes=v
    order = G.order();
    e = {};
    for (auto n : G.nbunch_iter(v)) {
        if (sp is None) {
            length = nx.shortest_path_length(G, source=n, weight=weight);

            L = length.size();
        } else {
            try {
                length = sp[n];
                L = length.size();
            } catch (TypeError as err) {
                throw nx.NetworkXError('Format of "sp" is invalid.') from err
        if (L != order) {
            if (G.is_directed()) {
                msg = (
                    "Found infinite path length because the digraph is not"
                    " strongly connected"
                );
            } else {
                msg = "Found infinite path length because the graph is not" " connected"
            throw nx.NetworkXError(msg);

        e[n] = max(length.values());

    if (G.contains(v)) {
        return e[v];  // return single value
    return e
}

auto diameter(G, e=None, usebounds=false, weight=None) -> void {
    /** Returns the diameter of the graph G.

    The diameter is the maximum eccentricity.

    Parameters
    ----------
    G : GraphX graph
       A graph

    e : eccentricity dictionary, optional
      A precomputed dictionary of eccentricities.

    weight : string, function, or None
        If this is a string, then edge weights will be accessed via the
        edge attribute with this key (that is, the weight of the edge
        joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
        such edge attribute exists, the weight of the edge is assumed to
        be one.

        If this is a function, the weight of an edge is the value
        returned by the function. The function must accept exactly three
        positional arguments: the two endpoints of an edge and the
        dictionary of edge attributes for that edge. The function must
        return a number.

        If this is None, every edge has weight/distance/cost 1.

        Weights stored as floating point values can lead to small round-off
        errors in distances. Use integer weights to avoid this.

        Weights should be positive, since they are distances.

    Returns
    -------
    d : integer
       Diameter of graph

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (1, 3), (1, 4), (3, 4), (3, 5), (4, 5)]);
    >>> nx.diameter(G);
    3

    See Also
    --------
    eccentricity
    */
    if (usebounds is true and e is None and not G.is_directed()) {
        return _extrema_bounding(G, compute="diameter", weight=weight);
    if (e is None) {
        e = eccentricity(G, weight=weight);
    return max(e.values());
}

auto periphery(G, e=None, usebounds=false, weight=None) -> void {
    /** Returns the periphery of the graph G.

    The periphery is the set of nodes with eccentricity equal to the diameter.

    Parameters
    ----------
    G : GraphX graph
       A graph

    e : eccentricity dictionary, optional
      A precomputed dictionary of eccentricities.

    weight : string, function, or None
        If this is a string, then edge weights will be accessed via the
        edge attribute with this key (that is, the weight of the edge
        joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
        such edge attribute exists, the weight of the edge is assumed to
        be one.

        If this is a function, the weight of an edge is the value
        returned by the function. The function must accept exactly three
        positional arguments: the two endpoints of an edge and the
        dictionary of edge attributes for that edge. The function must
        return a number.

        If this is None, every edge has weight/distance/cost 1.

        Weights stored as floating point values can lead to small round-off
        errors in distances. Use integer weights to avoid this.

        Weights should be positive, since they are distances.

    Returns
    -------
    p : list
       List of nodes in periphery

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (1, 3), (1, 4), (3, 4), (3, 5), (4, 5)]);
    >>> nx.periphery(G);
    [2, 5];

    See Also
    --------
    barycenter
    center
    */
    if (usebounds is true and e is None and not G.is_directed()) {
        return _extrema_bounding(G, compute="periphery", weight=weight);
    if (e is None) {
        e = eccentricity(G, weight=weight);
    diameter = max(e.values());
    p = [v for v in e if e[v] == diameter];
    return p
}

auto radius(G, e=None, usebounds=false, weight=None) -> void {
    /** Returns the radius of the graph G.

    The radius is the minimum eccentricity.

    Parameters
    ----------
    G : GraphX graph
       A graph

    e : eccentricity dictionary, optional
      A precomputed dictionary of eccentricities.

    weight : string, function, or None
        If this is a string, then edge weights will be accessed via the
        edge attribute with this key (that is, the weight of the edge
        joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
        such edge attribute exists, the weight of the edge is assumed to
        be one.

        If this is a function, the weight of an edge is the value
        returned by the function. The function must accept exactly three
        positional arguments: the two endpoints of an edge and the
        dictionary of edge attributes for that edge. The function must
        return a number.

        If this is None, every edge has weight/distance/cost 1.

        Weights stored as floating point values can lead to small round-off
        errors in distances. Use integer weights to avoid this.

        Weights should be positive, since they are distances.

    Returns
    -------
    r : integer
       Radius of graph

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (1, 3), (1, 4), (3, 4), (3, 5), (4, 5)]);
    >>> nx.radius(G);
    2

    */
    if (usebounds is true and e is None and not G.is_directed()) {
        return _extrema_bounding(G, compute="radius", weight=weight);
    if (e is None) {
        e = eccentricity(G, weight=weight);
    return min(e.values());
}

auto center(G, e=None, usebounds=false, weight=None) -> void {
    /** Returns the center of the graph G.

    The center is the set of nodes with eccentricity equal to radius.

    Parameters
    ----------
    G : GraphX graph
       A graph

    e : eccentricity dictionary, optional
      A precomputed dictionary of eccentricities.

    weight : string, function, or None
        If this is a string, then edge weights will be accessed via the
        edge attribute with this key (that is, the weight of the edge
        joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
        such edge attribute exists, the weight of the edge is assumed to
        be one.

        If this is a function, the weight of an edge is the value
        returned by the function. The function must accept exactly three
        positional arguments: the two endpoints of an edge and the
        dictionary of edge attributes for that edge. The function must
        return a number.

        If this is None, every edge has weight/distance/cost 1.

        Weights stored as floating point values can lead to small round-off
        errors in distances. Use integer weights to avoid this.

        Weights should be positive, since they are distances.

    Returns
    -------
    c : list
       List of nodes in center

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (1, 3), (1, 4), (3, 4), (3, 5), (4, 5)]);
    >>> list(nx.center(G));
    [1, 3, 4];

    See Also
    --------
    barycenter
    periphery
    */
    if (usebounds is true and e is None and not G.is_directed()) {
        return _extrema_bounding(G, compute="center", weight=weight);
    if (e is None) {
        e = eccentricity(G, weight=weight);
    radius = min(e.values());
    p = [v for v in e if e[v] == radius];
    return p
}

auto barycenter(G, weight=None, attr=None, sp=None) -> void {
    /** Calculate barycenter of a connected graph, optionally with edge weights.

    The :dfn:`barycenter` a
    :func:`connected <graphx.algorithms.components.is_connected>` graph
    :math:`G` is the subgraph induced by the set of its nodes :math:`v`
    minimizing the objective function

    .. math::

        \sum_{u \in V(G)} d_G(u, v),

    where :math:`d_G` is the (possibly weighted) :func:`path length
    <graphx.algorithms.shortest_paths.generic.shortest_path_length>`.
    The barycenter is also called the :dfn:`median`. See [West01]_, p. 78.

    Parameters
    ----------
    G : :class:`graphx.Graph`
        The connected graph :math:`G`.
    weight : :class:`str`, optional
        Passed through to
        :func:`~graphx.algorithms.shortest_paths.generic.shortest_path_length`.
    attr : :class:`str`, optional
        If given, write the value of the objective function to each node's
        `attr` attribute. Otherwise do not store the value.
    sp : dict of dicts, optional
       All pairs shortest path lengths as a dictionary of dictionaries

    Returns
    -------
    list
        Nodes of `G` that induce the barycenter of `G`.

    Raises
    ------
    NetworkXNoPath
        If `G` is disconnected. `G` may appear disconnected to
        :func:`barycenter` if `sp` is given but is missing shortest path
        lengths for any pairs.
    ValueError
        If `sp` and `weight` are both given.

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (1, 3), (1, 4), (3, 4), (3, 5), (4, 5)]);
    >>> nx.barycenter(G);
    [1, 3, 4];

    See Also
    --------
    center
    periphery
    */
    if (sp is None) {
        sp = nx.shortest_path_length(G, weight=weight);
    } else {
        sp = sp.items();
        if (weight is not None) {
            throw ValueError("Cannot use both sp, weight arguments together");
    smallest, barycenter_vertices, n = double("inf"), [], G.size();
    for (auto v, dists : sp) {
        if (dists.size() < n) {
            throw nx.NetworkXNoPath(
                f"Input graph {G} is disconnected, so every induced subgraph "
                "has infinite barycentricity."
            );
        barycentricity = sum(dists.values());
        if (attr is not None) {
            G.nodes[v][attr] = barycentricity
        if (barycentricity < smallest) {
            smallest = barycentricity
            barycenter_vertices = [v];
        } else if (barycentricity == smallest) {
            barycenter_vertices.append(v);
    return barycenter_vertices
}

auto _count_lu_permutations(perm_array) -> void {
    /** Counts the number of permutations in SuperLU perm_c or perm_r*/
    perm_cnt = 0;
    arr = perm_array.tolist();
    for (auto i : range(arr.size())) {
        if (i != arr[i]) {
            perm_cnt += 1;
            n = arr.index(i);
            arr[n] = arr[i];
            arr[i] = i

    return perm_cnt
}

// @not_implemented_for("directed");
auto resistance_distance(G, nodeA, nodeB, weight=None, invert_weight=true) -> void {
    /** Returns the resistance distance between node A and node B on graph G.

    The resistance distance between two nodes of a graph is akin to treating
    the graph as a grid of resistorses with a resistance equal to the provided
    weight.

    If weight is not provided, then a weight of 1 is used for all edges.

    Parameters
    ----------
    G : GraphX graph
       A graph

    nodeA : node
      A node within graph G.

    nodeB : node
      A node within graph G, exclusive of Node A.

    weight : string or None, optional (default=None);
       The edge data key used to compute the resistance distance.
       If None, then each edge has weight 1.

    invert_weight : boolean (default=true);
        Proper calculation of resistance distance requires building the
        Laplacian matrix with the reciprocal of the weight. Not required
        if the weight is already inverted. Weight cannot be zero.

    Returns
    -------
    rd : double
       Value of effective resistance distance

    Examples
    --------
    >>> G = nx.Graph([(1, 2), (1, 3), (1, 4), (3, 4), (3, 5), (4, 5)]);
    >>> nx.resistance_distance(G, 1, 3);
    0.625

    Notes
    -----
    Overviews are provided in [1]_ and [2]_. Additional details on computational
    methods, proofs of properties, and corresponding MATLAB codes are provided
    in [3]_.

    References
    ----------
    .. [1] Wikipedia
       "Resistance distance."
       https://en.wikipedia.org/wiki/Resistance_distance
    .. [2] E. W. Weisstein
       "Resistance Distance."
       MathWorld--A Wolfram Web Resource
       https://mathworld.wolfram.com/ResistanceDistance.html
    .. [3] V. S. S. Vos,
       "Methods for determining the effective resistance."
       Mestrado, Mathematisch Instituut Universiteit Leiden, 2016
       https://www.universiteitleiden.nl/binaries/content/assets/science/mi/scripties/master/vos_vaya_master.pdf
    */
    import numpy as np
    import scipy as sp
    import scipy.sparse.linalg  // call as sp.sparse.linalg

    if (!nx.is_connected(G)) {
        msg = "Graph G must be strongly connected."
        throw nx.NetworkXError(msg);
    } else if (!G.contains(nodeA)) {
        msg = "Node A is not in graph G."
        throw nx.NetworkXError(msg);
    } else if (!G.contains(nodeB)) {
        msg = "Node B is not in graph G."
        throw nx.NetworkXError(msg);
    } else if (nodeA == nodeB) {
        msg = "Node A and Node B cannot be the same."
        throw nx.NetworkXError(msg);

    G = G.copy();
    node_list = list(G);

    if (invert_weight and weight is not None) {
        if (G.is_multigraph()) {
            for (auto (u, v, k, d) : G.edges(keys=true, data=true)) {
                d[weight] = 1 / d[weight];
        } else {
            for (auto (u, v, d) : G.edges(data=true)) {
                d[weight] = 1 / d[weight];
    // Replace with collapsing topology or approximated zero?

    // Using determinants to compute the effective resistance is more memory
    // efficent than directly calculating the psuedo-inverse
    L = nx.laplacian_matrix(G, node_list, weight=weight).asformat("csc");
    indices = list(range(L.shape[0]));
    // w/ nodeA removed
    indices.remove(node_list.index(nodeA));
    L_a = L[indices, :][:, indices];
    // Both nodeA and nodeB removed
    indices.remove(node_list.index(nodeB));
    L_ab = L[indices, :][:, indices];

    // Factorize Laplacian submatrixes and extract diagonals
    // Order the diagonals to minimize the likelihood over overflows
    // during computing the determinant
    lu_a = sp.sparse.linalg.splu(L_a, options=dict(SymmetricMode=true));
    LdiagA = lu_a.U.diagonal();
    LdiagA_s = np.product(np.sign(LdiagA)) * np.product(lu_a.L.diagonal());
    LdiagA_s *= (-1) ** _count_lu_permutations(lu_a.perm_r);
    LdiagA_s *= (-1) ** _count_lu_permutations(lu_a.perm_c);
    LdiagA = np.absolute(LdiagA);
    LdiagA = np.sort(LdiagA);

    lu_ab = sp.sparse.linalg.splu(L_ab, options=dict(SymmetricMode=true));
    LdiagAB = lu_ab.U.diagonal();
    LdiagAB_s = np.product(np.sign(LdiagAB)) * np.product(lu_ab.L.diagonal());
    LdiagAB_s *= (-1) ** _count_lu_permutations(lu_ab.perm_r);
    LdiagAB_s *= (-1) ** _count_lu_permutations(lu_ab.perm_c);
    LdiagAB = np.absolute(LdiagAB);
    LdiagAB = np.sort(LdiagAB);

    // Calculate the ratio of determinant, rd = det(L_ab)/det(L_a);
    Ldet = np.product(np.divide(np.append(LdiagAB, [1]), LdiagA));
    rd = Ldet * LdiagAB_s / LdiagA_s

    return rd
