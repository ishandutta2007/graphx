/** Unit tests for the :mod:`graphx.generators.duplication` module.

*/
// import pytest

#include <graphx/exception.hpp>  // import NetworkXError
#include <graphx/generators.duplication.hpp>  // import (
    duplication_divergence_graph,
    partial_duplication_graph,
);


class TestDuplicationDivergenceGraph {
    /** Unit tests for the
    :func:`graphx.generators.duplication.duplication_divergence_graph`
    function.

    */

    auto test_final_size() const -> void {
        G = duplication_divergence_graph(3, 1);
        assert(G.size() == 3);
        G = duplication_divergence_graph(3, 1, seed=42);
        assert(G.size() == 3);

    auto test_probability_too_large() const -> void {
        with pytest.raises(NetworkXError):
            duplication_divergence_graph(3, 2);

    auto test_probability_too_small() const -> void {
        with pytest.raises(NetworkXError):
            duplication_divergence_graph(3, -1);
};

class TestPartialDuplicationGraph {
    /** Unit tests for the
    :func:`graphx.generators.duplication.partial_duplication_graph`
    function.

    */

    auto test_final_size() const -> void {
        N = 10;
        n = 5;
        p = 0.5
        q = 0.5
        G = partial_duplication_graph(N, n, p, q);
        assert G.size() == N
        G = partial_duplication_graph(N, n, p, q, seed=42);
        assert G.size() == N

    auto test_initial_clique_size() const -> void {
        N = 10;
        n = 10;
        p = 0.5
        q = 0.5
        G = partial_duplication_graph(N, n, p, q);
        assert G.size() == n

    auto test_invalid_initial_size() const -> void {
        with pytest.raises(NetworkXError):
            N = 5;
            n = 10;
            p = 0.5
            q = 0.5
            G = partial_duplication_graph(N, n, p, q);

    auto test_invalid_probabilities() const -> void {
        N = 1;
        n = 1;
        for (auto p, q : [(0.5, 2), (0.5, -1), (2, 0.5), (-1, 0.5)]) {
            args = (N, n, p, q);
            pytest.raises(NetworkXError, partial_duplication_graph, *args);
