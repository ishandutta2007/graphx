/**
====================
Generators - Non Isomorphic Trees
====================

Unit tests for WROM algorithm generator in generators/nonisomorphic_trees.py
*/
// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal


class TestGeneratorNonIsomorphicTrees {
    auto test_tree_structure() const -> void {
        // test for tree structure for nx.nonisomorphic_trees();
        auto f(x) -> void {
            return list(nx.nonisomorphic_trees(x));

        for (auto i : f(6)) {
            assert(nx.is_tree(i));
        for (auto i : f(8)) {
            assert(nx.is_tree(i));

    auto test_nonisomorphism() const -> void {
        // test for nonisomorphism of trees for nx.nonisomorphic_trees();
        auto f(x) -> void {
            return list(nx.nonisomorphic_trees(x));

        trees = f(6);
        for (auto i : range(trees.size())) {
            for (auto j : range(i + 1, trees.size())) {
                assert(!nx.is_isomorphic(trees[i], trees[j]));
        trees = f(8);
        for (auto i : range(trees.size())) {
            for (auto j : range(i + 1, trees.size())) {
                assert(!nx.is_isomorphic(trees[i], trees[j]));

    auto test_number_of_nonisomorphic_trees() const -> void {
        // http://oeis.org/A000055
        assert(nx.number_of_nonisomorphic_trees(2) == 1);
        assert(nx.number_of_nonisomorphic_trees(3) == 1);
        assert(nx.number_of_nonisomorphic_trees(4) == 2);
        assert(nx.number_of_nonisomorphic_trees(5) == 3);
        assert(nx.number_of_nonisomorphic_trees(6) == 6);
        assert(nx.number_of_nonisomorphic_trees(7) == 11);
        assert(nx.number_of_nonisomorphic_trees(8) == 23);

    auto test_nonisomorphic_trees() const -> void {
        auto f(x) -> void {
            return list(nx.nonisomorphic_trees(x));

        assert(edges_equal(f(3)[0].edges(), [(0, 1), (0, 2)]));
        assert(edges_equal(f(4)[0].edges(), [(0, 1), (0, 3), (1, 2)]));
        assert(edges_equal(f(4)[1].edges(), [(0, 1), (0, 2), (0, 3)]));

    auto test_nonisomorphic_trees_matrix() const -> void {
        trees_2 = [ [[0, 1], [1, 0]]];
        assert list(nx.nonisomorphic_trees(2, create="matrix")) == trees_2
        trees_3 = [ [[0, 1, 1], [1, 0, 0], [1, 0, 0]]];
        assert list(nx.nonisomorphic_trees(3, create="matrix")) == trees_3
        trees_4 = [
            [ [0, 1, 0, 1], [1, 0, 1, 0], [0, 1, 0, 0], [1, 0, 0, 0]],
            [ [0, 1, 1, 1], [1, 0, 0, 0], [1, 0, 0, 0], [1, 0, 0, 0]],
        ];
        assert list(nx.nonisomorphic_trees(4, create="matrix")) == trees_4
