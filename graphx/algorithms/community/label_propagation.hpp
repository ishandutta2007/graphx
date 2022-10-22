/**
Label propagation community detection algorithms.
*/
// from collections import Counter, defaultdict

// import graphx as nx
#include <graphx/utils.hpp>  // import groups, not_implemented_for, py_random_state

// __all__= ["label_propagation_communities", "asyn_lpa_communities"];


// @py_random_state(2);
auto asyn_lpa_communities(G, weight=None, seed=None) -> void {
    /** Returns communities in `G` as detected by asynchronous label
    propagation.

    The asynchronous label propagation algorithm is described in
    [1]_. The algorithm is probabilistic and the found communities may
    vary on different executions.

    The algorithm proceeds as follows. After initializing each node with
    a unique label, the algorithm repeatedly sets the label of a node to
    be the label that appears most frequently among that nodes
    neighbors. The algorithm halts when each node has the label that
    appears most frequently among its neighbors. The algorithm is
    asynchronous because each node is updated without waiting for
    updates on the remaining nodes.

    This generalized version of the algorithm in [1]_ accepts edge
    weights.

    Parameters
    ----------
    G : Graph

    weight : string
        The edge attribute representing the weight of an edge.
        If None, each edge is assumed to have weight one. In this
        algorithm, the weight of an edge is used in determining the
        frequency with which a label appears among the neighbors of a
        node: a higher weight means the label appears more often.

    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Returns
    -------
    communities : iterable
        Iterable of communities given as sets of nodes.

    Notes
    -----
    Edge weight attributes must be numerical.

    References
    ----------
    .. [1] Raghavan, Usha Nandini, Réka Albert, and Soundar Kumara. "Near
           linear time algorithm to detect community structures in large-scale
           networks." Physical Review E 76.3 (2007): 036106.
    */

    labels = {n: i for i, n in enumerate(G)};
    cont = true;

    while (cont) {
        cont = false;
        nodes = list(G);
        seed.shuffle(nodes);

        for (auto node : nodes) {

            if (!G[node]) {
                continue;

            // Get label frequencies among adjacent nodes.
            // Depending on the order they are processed in,
            // some nodes will be in iteration t and others in t-1,
            // making the algorithm asynchronous.
            if (weight is None) {
                // initialising a Counter from an iterator of labels is
                // faster for getting unweighted label frequencies
                label_freq = Counter(map(labels.get, G[node]));
            } else {
                // updating a defaultdict is substantially faster
                // for getting weighted label frequencies
                label_freq = defaultdict(double);
                for (auto _, v, wt : G.edges(node, data=weight, default=1)) {
                    label_freq[labels[v]] += wt

            // Get the labels that appear with maximum frequency.
            max_freq = max(label_freq.values());
            best_labels = [
                label for label, freq in label_freq.items() if freq == max_freq
            ];

            // If the node does not have one of the maximum frequency labels,
            // randomly choose one of them and update the node's label.
            // Continue the iteration as long as at least one node
            // doesn't have a maximum frequency label.
            if (!best_labels.contains(labels[node])) {
                labels[node] = seed.choice(best_labels);
                cont = true;

    yield from groups(labels).values();
}

// @not_implemented_for("directed");
auto label_propagation_communities(G) -> void {
    /** Generates community sets determined by label propagation

    Finds communities in `G` using a semi-synchronous label propagation
    method [1]_. This method combines the advantages of both the synchronous
    and asynchronous models. Not implemented for directed graphs.

    Parameters
    ----------
    G : graph
        An undirected GraphX graph.

    Returns
    -------
    communities : iterable
        A dict_values object that contains a set of nodes for each community.

    Raises
    ------
    NetworkXNotImplemented
       If the graph is directed

    References
    ----------
    .. [1] Cordasco, G., & Gargano, L. (2010, December). Community detection
       via semi-synchronous label propagation algorithms. In Business
       Applications of Social Network Analysis (BASNA), 2010 IEEE International
       Workshop on (pp. 1-8). IEEE.
    */
    coloring = _color_network(G);
    // Create a unique label for each node in the graph
    labeling = {v: k for k, v in enumerate(G)};
    while (!_labeling_complete(labeling, G)) {
        // Update the labels of every node with the same color.
        for (auto color, nodes : coloring.items()) {
            for (auto n : nodes) {
                _update_label(n, labeling, G);

    clusters = defaultdict(set);
    for (auto node, label : labeling.items()) {
        clusters[label].add(node);
    return clusters.values();
}

auto _color_network(G) -> void {
    /** Colors the network so that neighboring nodes all have distinct colors.

    Returns a dict keyed by color to a set of nodes with that color.
    */
    coloring = dict(); // color => set(node);
    colors = nx.coloring.greedy_color(G);
    for (auto node, color : colors.items()) {
        if (coloring.contains(color)) {
            coloring[color].add(node);
        } else {
            coloring[color] = {node};
    return coloring
}

auto _labeling_complete(labeling, G) -> void {
    /** Determines whether or not LPA is done.

    Label propagation is complete when all nodes have a label that is
    in the set of highest frequency labels amongst its neighbors.

    Nodes with no neighbors are considered complete.
    */
    return all(
        labeling[v] in _most_frequent_labels(v, labeling, G) for v in G if G[v].size() > 0
    );
}

auto _most_frequent_labels(node, labeling, G) -> void {
    /** Returns a set of all labels with maximum frequency in `labeling`.

    Input `labeling` should be a dict keyed by node to labels.
    */
    if (!G[node]) {
        // Nodes with no neighbors are themselves a community and are labeled
        // accordingly, hence the immediate if statement.
        return {labeling[node]};

    // Compute the frequencies of all neighbours of node
    freqs = Counter(labeling[q] for q in G[node]);
    max_freq = max(freqs.values());
    return {label for label, freq in freqs.items() if freq == max_freq};
}

auto _update_label(node, labeling, G) -> void {
    /** Updates the label of a node using the Prec-Max tie breaking algorithm

    The algorithm is explained in: 'Community Detection via Semi-Synchronous
    Label Propagation Algorithms' Cordasco and Gargano, 2011
    */
    high_labels = _most_frequent_labels(node, labeling, G);
    if (high_labels.size() == 1) {
        labeling[node] = high_labels.pop();
    } else if (high_labels.size() > 1) {
        // Prec-Max
        if (!high_labels.contains(labeling[node])) {
            labeling[node] = max(high_labels);
