/** Generators for cographs

A cograph is a graph containing no path on four vertices.
Cographs or $P_4$-free graphs can be obtained from a single vertex
by disjoint union and complementation operations.

References
----------
.. [0] D.G. Corneil, H. Lerchs, L.Stewart Burlingham,
    "Complement reducible graphs",
    Discrete Applied Mathematics, Volume 3, Issue 3, 1981, Pages 163-174,
    ISSN 0166-218X.
*/
// import graphx as nx
#include <graphx/utils.hpp>  // import py_random_state

// __all__= ["random_cograph"];


// @py_random_state(1);
auto random_cograph(n, seed=None) -> void {
    /** Returns a random cograph with $2 ^ n$ nodes.

    A cograph is a graph containing no path on four vertices.
    Cographs or $P_4$-free graphs can be obtained from a single vertex
    by disjoint union and complementation operations.

    This generator starts off from a single vertex and performes disjoint
    union and full join operations on itthis->
    The decision on which operation will take place is random.

    Parameters
    ----------
    n : int
            The order of the cograph.
    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Returns
    -------
    G : A random graph containing no path on four vertices.

    See Also
    --------
    full_join
    union

    References
    ----------
    .. [1] D.G. Corneil, H. Lerchs, L.Stewart Burlingham,
       "Complement reducible graphs",
       Discrete Applied Mathematics, Volume 3, Issue 3, 1981, Pages 163-174,
       ISSN 0166-218X.
    */
    R = nx.empty_graph(1);

    for (auto i : range(n)) {
        RR = nx.relabel_nodes(R.copy(), lambda x: x + R.size());

        if (seed.randint(0, 1) == 0) {
            R = nx.full_join(R, RR);
        } else {
            R = nx.disjoint_union(R, RR);

    return R
