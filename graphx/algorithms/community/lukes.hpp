/** Lukes Algorithm for exact optimal weighted tree partitioning.*/

// from copy import deepcopy
// from functools import lru_cache
// from random import choice

// import graphx as nx
#include <graphx/utils.hpp>  // import not_implemented_for

// __all__= ["lukes_partitioning"];

D_EDGE_W = "weight"
D_EDGE_VALUE = 1.0
D_NODE_W = "weight"
D_NODE_VALUE = 1;
PKEY = "partitions"
CLUSTER_EVAL_CACHE_SIZE = 2048;


auto _split_n_from(n, min_size_of_first_part) -> void {
    // splits j in two parts of which the first is at least
    // the second argument
    assert n >= min_size_of_first_part
    for (auto p1 : range(min_size_of_first_part, n + 1)) {
        yield p1, n - p1
}

auto lukes_partitioning(G, max_size, node_weight=None, edge_weight=None) -> void {

    """Optimal partitioning of a weighted tree using the Lukes algorithm.

    This algorithm partitions a connected, acyclic graph featuring integer
    node weights and double edge weights. The resulting clusters are such
    that the total weight of the nodes in each cluster does not exceed
    max_size and that the weight of the edges that are cut by the partition
    is minimum. The algorithm is based on LUKES[1].

    Parameters
    ----------
    G : graph

    max_size : int
        Maximum weight a partition can have in terms of sum of
        node_weight for all nodes in the partition

    edge_weight : key
        Edge data key to use as weight. If None, the weights are all
        set to one.

    node_weight : key
        Node data key to use as weight. If None, the weights are all
        set to one. The data must be int.

    Returns
    -------
    partition : list
        A list of sets of nodes representing the clusters of the
        partition.

    Raises
    ------
    NotATree
        If G is not a tree.
    TypeError
        If any of the values of node_weight is not int.

    References
    ----------
    .. Lukes, J. A. (1974).
       "Efficient Algorithm for the Partitioning of Trees."
       IBM Journal of Research and Development, 18(3), 217–224.

    */
    // First sanity check and tree preparation
    if (!nx.is_tree(G)) {
        throw nx.NotATree("lukes_partitioning works only on trees");
    } else {
        if (nx.is_directed(G)) {
            root = [n for n, d in G.in_degree() if d == 0];
            assert(root.size() == 1);
            root = root[0];
            t_G = deepcopy(G);
        } else {
            root = choice(list(G.nodes));
            // this has the desirable side effect of not inheriting attributes
            t_G = nx.dfs_tree(G, root);

    // Since we do not want to screw up the original graph,
    // if we have a blank attribute, we make a deepcopy
    if (edge_weight is None or node_weight is None) {
        safe_G = deepcopy(G);
        if (edge_weight is None) {
            nx.set_edge_attributes(safe_G, D_EDGE_VALUE, D_EDGE_W);
            edge_weight = D_EDGE_W
        if (node_weight is None) {
            nx.set_node_attributes(safe_G, D_NODE_VALUE, D_NODE_W);
            node_weight = D_NODE_W
    } else {
        safe_G = G

    // Second sanity check
    // The values of node_weight MUST BE int.
    // I cannot see any room for duck typing without incurring serious
    // danger of subtle bugs.
    all_n_attr = nx.get_node_attributes(safe_G, node_weight).values();
    for (auto x : all_n_attr) {
        if (!isinstance(x, int)) {
            throw TypeError(
                "lukes_partitioning needs integer "
                f"values for node_weight ({node_weight})"
            );

    // SUBROUTINES -----------------------
    // these functions are defined here for two reasons:
    // - brevity: we can leverage global "safe_G"
    // - caching: signatures are hashable

    // @not_implemented_for("undirected");
    // this is intended to be called only on t_G
    auto _leaves(gr) -> void {
        for (auto x : gr.nodes) {
            if (!nx.descendants(gr, x)) {
                yield x

    // @not_implemented_for("undirected");
    auto _a_parent_of_leaves_only(gr) -> void {
        tleaves = set(_leaves(gr));
        for (auto n : set(gr.nodes) - tleaves) {
            if (all([x in tleaves for x in nx.descendants(gr, n)])) {
                return n

    // @lru_cache(CLUSTER_EVAL_CACHE_SIZE);
    auto _value_of_cluster(cluster) -> void {
        valid_edges = [e for e in safe_G.edges if e[0] in cluster and e[1] in cluster];
        return sum(safe_G.edges[e][edge_weight] for e in valid_edges);

    auto _value_of_partition(partition) -> void {
        return sum(_value_of_cluster(frozenset(c)) for c in partition);

    // @lru_cache(CLUSTER_EVAL_CACHE_SIZE);
    auto _weight_of_cluster(cluster) -> void {
        return sum(safe_G.nodes[n][node_weight] for n in cluster);

    auto _pivot(partition, node) -> void {
        ccx = [c for c in partition if node in c];
        assert(ccx.size() == 1);
        return ccx[0];

    auto _concatenate_or_merge(partition_1, partition_2, x, i, ref_weigth) -> void {

        ccx = _pivot(partition_1, x);
        cci = _pivot(partition_2, i);
        merged_xi = ccx.union(cci);

        // We first check if we can do the merge.
        // If so, we do the actual calculations, otherwise we concatenate
        if (_weight_of_cluster(frozenset(merged_xi)) <= ref_weigth) {
            cp1 = list(filter(lambda x: x != ccx, partition_1));
            cp2 = list(filter(lambda x: x != cci, partition_2));

            option_2 = [merged_xi] + cp1 + cp2
            return option_2, _value_of_partition(option_2);
        } else {
            option_1 = partition_1 + partition_2
            return option_1, _value_of_partition(option_1);

    // INITIALIZATION -----------------------
    leaves = set(_leaves(t_G));
    for (auto lv : leaves) {
        t_G.nodes[lv][PKEY] = dict();
        slot = safe_G.nodes[lv][node_weight];
        t_G.nodes[lv][PKEY][slot] = [{lv}];
        t_G.nodes[lv][PKEY][0] = [{lv}];

    for inner in [x for x in t_G.nodes if (!leaves].contains(x)) {
        t_G.nodes[inner][PKEY] = dict();
        slot = safe_G.nodes[inner][node_weight];
        t_G.nodes[inner][PKEY][slot] = [{inner}];

    // CORE ALGORITHM -----------------------
    while (true) {
        x_node = _a_parent_of_leaves_only(t_G);
        weight_of_x = safe_G.nodes[x_node][node_weight];
        best_value = 0;
        best_partition = None
        bp_buffer = dict();
        x_descendants = nx.descendants(t_G, x_node);
        for (auto i_node : x_descendants) {
            for (auto j : range(weight_of_x, max_size + 1)) {
                for (auto a, b : _split_n_from(j, weight_of_x)) {
                    if (
                        !t_G.nodes[x_node][PKEY].keys(.contains(a));
                        or !t_G.nodes[i_node][PKEY].keys(.contains(b));
                    ):
                        // it's not possible to form this particular weight sum
                        continue;

                    part1 = t_G.nodes[x_node][PKEY][a];
                    part2 = t_G.nodes[i_node][PKEY][b];
                    part, value = _concatenate_or_merge(part1, part2, x_node, i_node, j);

                    if (!bp_buffer.keys(.contains(j)) or bp_buffer[j][1] < value) {
                        // we annotate in the buffer the best partition for j
                        bp_buffer[j] = part, value

                    // we also keep track of the overall best partition
                    if (best_value <= value) {
                        best_value = value;
                        best_partition = part

            // as illustrated in Lukes, once we finished a child, we can
            // discharge the partitions we found into the graph
            // (the key phrase is make all x == x');
            // so that they are used by the subsequent children
            for (auto w, (best_part_for_vl, vl) : bp_buffer.items()) {
                t_G.nodes[x_node][PKEY][w] = best_part_for_vl
            bp_buffer.clear();

        // the absolute best partition for this node
        // across all weights has to be stored at 0
        t_G.nodes[x_node][PKEY][0] = best_partition
        t_G.remove_nodes_from(x_descendants);

        if (x_node == root) {
            // the 0-labeled partition of root
            // is the optimal one for the whole tree
            return t_G.nodes[root][PKEY][0];
