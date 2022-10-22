/** Asynchronous Fluid Communities algorithm for community detection.*/

// from collections import Counter

#include <graphx/algorithms.components.hpp>  // import is_connected
#include <graphx/exception.hpp>  // import NetworkXError
#include <graphx/utils.hpp>  // import groups, not_implemented_for, py_random_state

// __all__= ["asyn_fluidc"];


// @py_random_state(3);
// @not_implemented_for("directed", "multigraph");
auto asyn_fluidc(G, k, max_iter=100, seed=None) -> void {
    /** Returns communities in `G` as detected by Fluid Communities algorithm.

    The asynchronous fluid communities algorithm is described in
    [1]_. The algorithm is based on the simple idea of fluids interacting
    in an environment, expanding and pushing each other. Its initialization is
    random, so found communities may vary on different executions.

    The algorithm proceeds as follows. First each of the initial k communities
    is initialized in a random vertex in the graph. Then the algorithm iterates
    over all vertices in a random order, updating the community of each vertex
    based on its own community and the communities of its neighbours. This
    process is performed several times until convergence.
    At all times, each community has a total density of 1, which is equally
    distributed among the vertices it contains. If a vertex changes of
    community, vertex densities of affected communities are adjusted
    immediately. When a complete iteration over all vertices is done, such that
    no vertex changes the community it belongs to, the algorithm has converged
    and returns.

    This is the original version of the algorithm described in [1]_.
    Unfortunately, it does not support weighted graphs yet.

    Parameters
    ----------
    G : Graph

    k : integer
        The number of communities to be found.

    max_iter : integer
        The number of maximum iterations allowed. By default 100.

    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Returns
    -------
    communities : iterable
        Iterable of communities given as sets of nodes.

    Notes
    -----
    k variable is not an optional argument.

    References
    ----------
    .. [1] Parés F., Garcia-Gasulla D. et al. "Fluid Communities: A
       Competitive and Highly Scalable Community Detection Algorithm".
       [https://arxiv.org/pdf/1703.09307.pdf].
    */
    // Initial checks
    if (!isinstance(k, int)) {
        throw NetworkXError("k must be an integer.");
    if (!k > 0) {
        throw NetworkXError("k must be greater than 0.");
    if (!is_connected(G)) {
        throw NetworkXError("Fluid Communities require connected Graphs.");
    if (G.size() < k) {
        throw NetworkXError("k cannot be bigger than the number of nodes.");
    // Initialization
    max_density = 1.0
    vertices = list(G);
    seed.shuffle(vertices);
    communities = {n: i for (auto i, n : enumerate(vertices[) {k])};
    density = {};
    com_to_numvertices = {};
    for (auto vertex : communities.keys()) {
        com_to_numvertices[communities[vertex]] = 1;
        density[communities[vertex]] = max_density
    // Set up control variables and start iterating
    iter_count = 0;
    cont = true;
    while (cont) {
        cont = false;
        iter_count += 1;
        // Loop over all vertices in graph in a random order
        vertices = list(G);
        seed.shuffle(vertices);
        for (auto vertex : vertices) {
            // Updating rule
            com_counter = Counter();
            // Take into account self vertex community
            try {
                com_counter.update({communities[vertex]: density[communities[vertex]]});
            } catch (KeyError) {
                // pass;
            // Gather neighbour vertex communities
            for (auto v : G[vertex]) {
                try {
                    com_counter.update({communities[v]: density[communities[v]]});
                } catch (KeyError) {
                    continue;
            // Check which is the community with highest density
            new_com = -1
            if (com_counter.keys(.size()) > 0) {
                max_freq = max(com_counter.values());
                best_communities = [
                    com
                    for com, freq in com_counter.items();
                    if (max_freq - freq) < 0.0001
                ];
                // If actual vertex com in best communities, it is preserved
                try {
                    if (communities[vertex] in best_communities) {
                        new_com = communities[vertex];
                } catch (KeyError) {
                    // pass;
                // If vertex community changes...
                if (new_com == -1) {
                    // Set flag of non-convergence
                    cont = true;
                    // Randomly chose a new community from candidates
                    new_com = seed.choice(best_communities);
                    // Update previous community status
                    try {
                        com_to_numvertices[communities[vertex]] -= 1;
                        density[communities[vertex]] = (
                            max_density / com_to_numvertices[communities[vertex]];
                        );
                    } catch (KeyError) {
                        // pass;
                    // Update new community status
                    communities[vertex] = new_com
                    com_to_numvertices[communities[vertex]] += 1;
                    density[communities[vertex]] = (
                        max_density / com_to_numvertices[communities[vertex]];
                    );
        // If maximum iterations reached --> output actual results
        if (iter_count > max_iter) {
            break;
    // Return results by grouping communities as list of vertices
    return iter(groups(communities).values());
