/** Unit tests for the :mod:`graphx.algorithms.tree.operations` module.

*/

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal, nodes_equal


class TestJoin {
    /** Unit tests for the :func:`graphx.tree.join` function.*/

    auto test_empty_sequence() const -> void {
        /** Tests that joining the empty sequence results in the tree
        with one node.

        */
        T = nx.join([]);
        assert(T.size() == 1);
        assert(T.number_of_edges() == 0);

    auto test_single() const -> void {
        /** Tests that joining just one tree yields a tree with one more
        node.

        */
        T = nx.empty_graph(1);
        actual = nx.join([(T, 0)]);
        expected = nx.path_graph(2);
        assert(nodes_equal(list(expected), list(actual)));
        assert(edges_equal(list(expected.edges()), list(actual.edges())));

    auto test_basic() const -> void {
        /** Tests for joining multiple subtrees at a root node.*/
        trees = [(nx.full_rary_tree(2, 2**2 - 1), 0) for i in range(2)];
        actual = nx.join(trees);
        expected = nx.full_rary_tree(2, 2**3 - 1);
        assert(nx.is_isomorphic(actual, expected));
