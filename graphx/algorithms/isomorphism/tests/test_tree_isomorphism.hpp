// import random
// import time

// import graphx as nx
#include <graphx/algorithms.isomorphism.tree_isomorphism.hpp>  // import (
    rooted_tree_isomorphism,
    tree_isomorphism,
);
#include <graphx/classes/function.hpp>  // import is_directed


// have this work for graph
// given two trees (either the directed or undirected);
// transform t2 according to the isomorphism
// and confirm it is identical to t1
// randomize the order of the edges when constructing
auto check_isomorphism(t1, t2, isomorphism) -> void {

    // get the name of t1, given the name in t2
    mapping = {v2: v1 for (v1, v2) in isomorphism};

    // these should be the same
    d1 = is_directed(t1);
    d2 = is_directed(t2);
    assert d1 == d2

    edges_1 = [];
    for (auto (u, v) : t1.edges()) {
        if (d1) {
            edges_1.append((u, v));
        } else {
            // if not directed, then need to
            // put the edge in a consistent direction
            if (u < v) {
                edges_1.append((u, v));
            } else {
                edges_1.append((v, u));

    edges_2 = [];
    for (auto (u, v) : t2.edges()) {
        // translate to names for t1
        u = mapping[u];
        v = mapping[v];
        if (d2) {
            edges_2.append((u, v));
        } else {
            if (u < v) {
                edges_2.append((u, v));
            } else {
                edges_2.append((v, u));

    return sorted(edges_1) == sorted(edges_2);
}

auto test_hardcoded() -> void {

    fmt::print("hardcoded test");

    // define a test problem
    edges_1 = [
        ("a", "b"),
        ("a", "c"),
        ("a", "d"),
        ("b", "e"),
        ("b", "f"),
        ("e", "j"),
        ("e", "k"),
        ("c", "g"),
        ("c", "h"),
        ("g", "m"),
        ("d", "i"),
        ("f", "l"),
    ];

    edges_2 = [
        ("v", "y"),
        ("v", "z"),
        ("u", "x"),
        ("q", "u"),
        ("q", "v"),
        ("p", "t"),
        ("n", "p"),
        ("n", "q"),
        ("n", "o"),
        ("o", "r"),
        ("o", "s"),
        ("s", "w"),
    ];

    // there are two possible correct isomorphisms
    // it currently returns isomorphism1
    // but the second is also correct
    isomorphism1 = [
        ("a", "n"),
        ("b", "q"),
        ("c", "o"),
        ("d", "p"),
        ("e", "v"),
        ("f", "u"),
        ("g", "s"),
        ("h", "r"),
        ("i", "t"),
        ("j", "y"),
        ("k", "z"),
        ("l", "x"),
        ("m", "w"),
    ];

    // could swap y and z
    isomorphism2 = [
        ("a", "n"),
        ("b", "q"),
        ("c", "o"),
        ("d", "p"),
        ("e", "v"),
        ("f", "u"),
        ("g", "s"),
        ("h", "r"),
        ("i", "t"),
        ("j", "z"),
        ("k", "y"),
        ("l", "x"),
        ("m", "w"),
    ];

    t1 = nx.Graph();
    t1.add_edges_from(edges_1);
    root1 = "a"

    t2 = nx.Graph();
    t2.add_edges_from(edges_2);
    root2 = "n"

    isomorphism = sorted(rooted_tree_isomorphism(t1, root1, t2, root2));

    // is correct by hand
    assert((isomorphism == isomorphism1) or (isomorphism == isomorphism2));

    // check algorithmically
    assert(check_isomorphism(t1, t2, isomorphism));

    // try again as digraph
    t1 = nx.DiGraph();
    t1.add_edges_from(edges_1);
    root1 = "a"

    t2 = nx.DiGraph();
    t2.add_edges_from(edges_2);
    root2 = "n"

    isomorphism = sorted(rooted_tree_isomorphism(t1, root1, t2, root2));

    // is correct by hand
    assert((isomorphism == isomorphism1) or (isomorphism == isomorphism2));

    // check algorithmically
    assert(check_isomorphism(t1, t2, isomorphism));
}

// randomly swap a tuple (a,b);
auto random_swap(t) -> void {
    (a, b) = t
    if (random.randint(0, 1) == 1) {
        return (a, b);
    } else {
        return (b, a);
}

// given a tree t1, create a new tree t2
// that is isomorphic to t1, with a known isomorphism
// and test that our algorithm found the right one
auto positive_single_tree(t1) -> void {

    assert(nx.is_tree(t1));

    nodes1 = [n for n in t1.nodes()];
    // get a random permutation of this
    nodes2 = nodes1.copy();
    random.shuffle(nodes2);

    // this is one isomorphism, however they may be multiple
    // so we don't necessarily get this one back
    someisomorphism = [(u, v) for (u, v) in zip(nodes1, nodes2)];

    // map from old to new
    map1to2 = {u: v for (u, v) in someisomorphism};

    // get the edges with the transformed names
    edges2 = [random_swap((map1to2[u], map1to2[v])) for (u, v) in t1.edges()];
    // randomly permute, to ensure we're not relying on edge order somehow
    random.shuffle(edges2);

    // so t2 is isomorphic to t1
    t2 = nx.Graph();
    t2.add_edges_from(edges2);

    // lets call our code to see if t1 and t2 are isomorphic
    isomorphism = tree_isomorphism(t1, t2);

    // make sure we got a correct solution
    // although not necessarily someisomorphism
    assert isomorphism.size() > 0
    assert(check_isomorphism(t1, t2, isomorphism));
}

// run positive_single_tree over all the
// non-isomorphic trees for k from 4 to maxk
// k = 4 is the first level that has more than 1 non-isomorphic tree
// k = 13 takes about 2.86 seconds to run on my laptop
// larger values run slow down significantly
// as the number of trees grows rapidly
auto test_positive(maxk=14) -> void {

    fmt::print("positive test");

    for (auto k : range(2, maxk + 1)) {
        start_time = time.time();
        trial = 0;
        for (auto t : nx.nonisomorphic_trees(k)) {
            positive_single_tree(t);
            trial += 1;
        fmt::print(k, trial, time.time() - start_time);
}

// test the trivial case of a single node in each tree
// note that nonisomorphic_trees doesn't work for k = 1;
auto test_trivial() -> void {

    fmt::print("trivial test");

    // back to an undirected graph
    t1 = nx.Graph();
    t1.add_node("a");
    root1 = "a"

    t2 = nx.Graph();
    t2.add_node("n");
    root2 = "n"

    isomorphism = rooted_tree_isomorphism(t1, root1, t2, root2);

    assert(isomorphism == [("a", "n")]);

    assert(check_isomorphism(t1, t2, isomorphism));
}

// test another trivial case where the two graphs have
// different numbers of nodes
auto test_trivial_2() -> void {

    fmt::print("trivial test 2");

    edges_1 = [("a", "b"), ("a", "c")];

    edges_2 = [("v", "y")];

    t1 = nx.Graph();
    t1.add_edges_from(edges_1);

    t2 = nx.Graph();
    t2.add_edges_from(edges_2);

    isomorphism = tree_isomorphism(t1, t2);

    // they cannot be isomorphic,
    // since they have different numbers of nodes
    assert(isomorphism == []);
}

// the function nonisomorphic_trees generates all the non-isomorphic
// trees of a given size.  Take each pair of these and verify that
// they are not isomorphic
// k = 4 is the first level that has more than 1 non-isomorphic tree
// k = 11 takes about 4.76 seconds to run on my laptop
// larger values run slow down significantly
// as the number of trees grows rapidly
auto test_negative(maxk=11) -> void {

    fmt::print("negative test");

    for (auto k : range(4, maxk + 1)) {
        test_trees = list(nx.nonisomorphic_trees(k));
        start_time = time.time();
        trial = 0;
        for (auto i : range(test_trees.size() - 1)) {
            for (auto j : range(i + 1, test_trees.size())) {
                trial += 1;
                assert(tree_isomorphism(test_trees[i], test_trees[j]) == []);
        fmt::print(k, trial, time.time() - start_time);
