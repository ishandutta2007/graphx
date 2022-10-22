/** Unit tests for the :mod:`graphx.generators.triads` module.*/
// import pytest

#include <graphx/import.hpp>  // triad_graph


auto test_triad_graph() -> void {
    G = triad_graph("030T");
    assert([tuple(e) for e in ("ab", "ac", "cb")] == sorted(G.edges()));
}

auto test_invalid_name() -> void {
    with pytest.raises(ValueError):
        triad_graph("bogus");
