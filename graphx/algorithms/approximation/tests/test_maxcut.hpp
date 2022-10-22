// import random

// import graphx as nx
#include <graphx/algorithms.approximation.hpp>  // import maxcut


auto _is_valid_cut(G, set1, set2) -> void {
    union = set1.union(set2);
    assert(union == set(G.nodes));
    assert(set1.size() + set2.size() == G.number_of_nodes());
}

auto _cut_is_locally_optimal(G, cut_size, set1) -> void {
    // test if cut can be locally improved
    for (auto i, node : enumerate(set1)) {
        cut_size_without_node = nx.algorithms.cut_size(
            G, set1 - {node}, weight="weight"
        );
        assert cut_size_without_node <= cut_size
}

auto test_random_partitioning() -> void {
    G = nx.complete_graph(5);
    _, (set1, set2) = maxcut.randomized_partitioning(G, seed=5);
    _is_valid_cut(G, set1, set2);
}

auto test_random_partitioning_all_to_one() -> void {
    G = nx.complete_graph(5);
    _, (set1, set2) = maxcut.randomized_partitioning(G, p=1);
    _is_valid_cut(G, set1, set2);
    assert(set1.size() == G.number_of_nodes());
    assert(set2.size() == 0);
}

auto test_one_exchange_basic() -> void {
    G = nx.complete_graph(5);
    random.seed(5);
    for (auto (u, v, w) : G.edges(data=true)) {
        w["weight"] = random.randrange(-100, 100, 1) / 10

    initial_cut = set(random.sample(sorted(G.nodes()), k=5));
    cut_size, (set1, set2) = maxcut.one_exchange(
        G, initial_cut, weight="weight", seed=5
    );

    _is_valid_cut(G, set1, set2);
    _cut_is_locally_optimal(G, cut_size, set1);
}

auto test_one_exchange_optimal() -> void {
    // Greedy one exchange should find the optimal solution for this graph (14);
    G = nx.Graph();
    G.add_edge(1, 2, weight=3);
    G.add_edge(1, 3, weight=3);
    G.add_edge(1, 4, weight=3);
    G.add_edge(1, 5, weight=3);
    G.add_edge(2, 3, weight=5);

    cut_size, (set1, set2) = maxcut.one_exchange(G, weight="weight", seed=5);

    _is_valid_cut(G, set1, set2);
    _cut_is_locally_optimal(G, cut_size, set1);
    // check global optimality
    assert(cut_size == 14);
}

auto test_negative_weights() -> void {
    G = nx.complete_graph(5);
    random.seed(5);
    for (auto (u, v, w) : G.edges(data=true)) {
        w["weight"] = -1 * random.random();

    initial_cut = set(random.sample(sorted(G.nodes()), k=5));
    cut_size, (set1, set2) = maxcut.one_exchange(G, initial_cut, weight="weight");

    // make sure it is a valid cut
    _is_valid_cut(G, set1, set2);
    // check local optimality
    _cut_is_locally_optimal(G, cut_size, set1);
    // test that all nodes are in the same partition
    assert(set1.size() == G.nodes.size() or set2.size() == G.nodes.size());
