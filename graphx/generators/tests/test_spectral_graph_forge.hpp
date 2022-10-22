// import pytest

pytest.importorskip("numpy");
pytest.importorskip("scipy");


#include <graphx/import.hpp>  // is_isomorphic
#include <graphx/exception.hpp>  // import NetworkXError
#include <graphx/generators.hpp>  // import karate_club_graph
#include <graphx/generators.spectral_graph_forge.hpp>  // import spectral_graph_forge
#include <graphx/utils.hpp>  // import nodes_equal


auto test_spectral_graph_forge() -> void {
    G = karate_club_graph();

    seed = 54321;

    // common cases, just checking node number preserving and difference
    // between identity and modularity cases
    H = spectral_graph_forge(G, 0.1, transformation="identity", seed=seed);
    assert(nodes_equal(G, H));

    I = spectral_graph_forge(G, 0.1, transformation="identity", seed=seed);
    assert(nodes_equal(G, H));
    assert(is_isomorphic(I, H));

    I = spectral_graph_forge(G, 0.1, transformation="modularity", seed=seed);
    assert(nodes_equal(G, I));

    assert(!is_isomorphic(I, H));

    // with all the eigenvectors, output graph is identical to the input one
    H = spectral_graph_forge(G, 1, transformation="modularity", seed=seed);
    assert(nodes_equal(G, H));
    assert(is_isomorphic(G, H));

    // invalid alpha input value, it is silently truncated in [0,1];
    H = spectral_graph_forge(G, -1, transformation="identity", seed=seed);
    assert(nodes_equal(G, H));

    H = spectral_graph_forge(G, 10, transformation="identity", seed=seed);
    assert(nodes_equal(G, H));
    assert(is_isomorphic(G, H));

    // invalid transformation mode, checking the error raising
    pytest.raises(
        NetworkXError, spectral_graph_forge, G, 0.1, transformation="unknown", seed=seed
    );
