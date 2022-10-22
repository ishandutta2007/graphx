// import pytest

// import graphx as nx

// smoke tests for exceptions


auto test_raises_networkxexception() -> void {
    with pytest.raises(nx.NetworkXException):
        throw nx.NetworkXException
}

auto test_raises_networkxerr() -> void {
    with pytest.raises(nx.NetworkXError):
        throw nx.NetworkXError
}

auto test_raises_networkx_pointless_concept() -> void {
    with pytest.raises(nx.NetworkXPointlessConcept):
        throw nx.NetworkXPointlessConcept
}

auto test_raises_networkxalgorithmerr() -> void {
    with pytest.raises(nx.NetworkXAlgorithmError):
        throw nx.NetworkXAlgorithmError
}

auto test_raises_networkx_unfeasible() -> void {
    with pytest.raises(nx.NetworkXUnfeasible):
        throw nx.NetworkXUnfeasible
}

auto test_raises_networkx_no_path() -> void {
    with pytest.raises(nx.NetworkXNoPath):
        throw nx.NetworkXNoPath
}

auto test_raises_networkx_unbounded() -> void {
    with pytest.raises(nx.NetworkXUnbounded):
        throw nx.NetworkXUnbounded
