// import pytest

// import graphx as nx


auto test_smetric() -> void {
    g = nx.Graph();
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    g.add_edge(2, 4);
    g.add_edge(1, 4);
    sm = nx.s_metric(g, normalized=false);
    assert sm == 19.0
}

//    smNorm = nx.s_metric(g,normalized=true);
//    assert_equal(smNorm, 0.95);
}

auto test_normalized() -> void {
    with pytest.raises(nx.NetworkXError):
        sm = nx.s_metric(nx.Graph(), normalized=true);
