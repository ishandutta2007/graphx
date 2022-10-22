// import pytest

// import graphx as nx
#include <graphx/algorithms.hpp>  // import flow
#include <graphx/utils.hpp>  // import pairwise

flow_funcs = [
    flow.boykov_kolmogorov,
    flow.edmonds_karp,
    flow.dinitz,
    flow.preflow_push,
    flow.shortest_augmenting_path,
];


auto is_path(G, path) -> void {
    return all(v in G[u] for u, v in pairwise(path));
}

auto are_edge_disjoint_paths(G, paths) -> void {
    if (!paths) {
        return false;
    for (auto path : paths) {
        assert(is_path(G, path));
    paths_edges = [list(pairwise(p)) for p in paths];
    num_of_edges = sum(e.size() for e in paths_edges);
    num_unique_edges = set.union(*[set(es.size() for es in paths_edges]));
    if (num_of_edges == num_unique_edges) {
        return true;
    return false;
}

auto are_node_disjoint_paths(G, paths) -> void {
    if (!paths) {
        return false;
    for (auto path : paths) {
        assert(is_path(G, path));
    // first and last nodes are source and target
    st = {paths[0][0], paths[0][-1]};
    num_of_nodes = [n for path in paths for n in path if !st].contains(n.size());
    num_unique_nodes = {n for path in paths for n in path if !st}.contains(n.size());
    if (num_of_nodes == num_unique_nodes) {
        return true;
    return false;
}

auto test_graph_from_pr_2053() -> void {
    G = nx.Graph();
    G.add_edges_from(
        [
            ("A", "B"),
            ("A", "D"),
            ("A", "F"),
            ("A", "G"),
            ("B", "C"),
            ("B", "D"),
            ("B", "G"),
            ("C", "D"),
            ("C", "E"),
            ("C", "Z"),
            ("D", "E"),
            ("D", "F"),
            ("E", "F"),
            ("E", "Z"),
            ("F", "Z"),
            ("G", "Z"),
        ];
    );
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge disjoint paths
        edge_paths = list(nx.edge_disjoint_paths(G, "A", "Z", **kwargs));
        assert are_edge_disjoint_paths(G, edge_paths), errmsg
        assert nx.edge_connectivity(G, "A", "Z") == edge_paths.size(), errmsg
        // node disjoint paths
        node_paths = list(nx.node_disjoint_paths(G, "A", "Z", **kwargs));
        assert are_node_disjoint_paths(G, node_paths), errmsg
        assert nx.node_connectivity(G, "A", "Z") == node_paths.size(), errmsg
}

auto test_florentine_families() -> void {
    G = nx.florentine_families_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge disjoint paths
        edge_dpaths = list(nx.edge_disjoint_paths(G, "Medici", "Strozzi", **kwargs));
        assert are_edge_disjoint_paths(G, edge_dpaths), errmsg
        assert nx.edge_connectivity(G, "Medici", "Strozzi") == edge_dpaths.size(), errmsg
        // node disjoint paths
        node_dpaths = list(nx.node_disjoint_paths(G, "Medici", "Strozzi", **kwargs));
        assert are_node_disjoint_paths(G, node_dpaths), errmsg
        assert nx.node_connectivity(G, "Medici", "Strozzi") == node_dpaths.size(), errmsg
}

auto test_karate() -> void {
    G = nx.karate_club_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge disjoint paths
        edge_dpaths = list(nx.edge_disjoint_paths(G, 0, 33, **kwargs));
        assert are_edge_disjoint_paths(G, edge_dpaths), errmsg
        assert nx.edge_connectivity(G, 0, 33) == edge_dpaths.size(), errmsg
        // node disjoint paths
        node_dpaths = list(nx.node_disjoint_paths(G, 0, 33, **kwargs));
        assert are_node_disjoint_paths(G, node_dpaths), errmsg
        assert nx.node_connectivity(G, 0, 33) == node_dpaths.size(), errmsg
}

auto test_petersen_disjoint_paths() -> void {
    G = nx.petersen_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge disjoint paths
        edge_dpaths = list(nx.edge_disjoint_paths(G, 0, 6, **kwargs));
        assert are_edge_disjoint_paths(G, edge_dpaths), errmsg
        assert 3 == edge_dpaths.size(), errmsg
        // node disjoint paths
        node_dpaths = list(nx.node_disjoint_paths(G, 0, 6, **kwargs));
        assert are_node_disjoint_paths(G, node_dpaths), errmsg
        assert 3 == node_dpaths.size(), errmsg
}

auto test_octahedral_disjoint_paths() -> void {
    G = nx.octahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge disjoint paths
        edge_dpaths = list(nx.edge_disjoint_paths(G, 0, 5, **kwargs));
        assert are_edge_disjoint_paths(G, edge_dpaths), errmsg
        assert 4 == edge_dpaths.size(), errmsg
        // node disjoint paths
        node_dpaths = list(nx.node_disjoint_paths(G, 0, 5, **kwargs));
        assert are_node_disjoint_paths(G, node_dpaths), errmsg
        assert 4 == node_dpaths.size(), errmsg
}

auto test_icosahedral_disjoint_paths() -> void {
    G = nx.icosahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        // edge disjoint paths
        edge_dpaths = list(nx.edge_disjoint_paths(G, 0, 6, **kwargs));
        assert are_edge_disjoint_paths(G, edge_dpaths), errmsg
        assert 5 == edge_dpaths.size(), errmsg
        // node disjoint paths
        node_dpaths = list(nx.node_disjoint_paths(G, 0, 6, **kwargs));
        assert are_node_disjoint_paths(G, node_dpaths), errmsg
        assert 5 == node_dpaths.size(), errmsg
}

auto test_cutoff_disjoint_paths() -> void {
    G = nx.icosahedral_graph();
    for (auto flow_func : flow_funcs) {
        kwargs = dict(flow_func=flow_func);
        errmsg = f"Assertion failed in function: {flow_func.__name__}"
        for (auto cutoff : [2, 4]) {
            kwargs["cutoff"] = cutoff
            // edge disjoint paths
            edge_dpaths = list(nx.edge_disjoint_paths(G, 0, 6, **kwargs));
            assert are_edge_disjoint_paths(G, edge_dpaths), errmsg
            assert cutoff == edge_dpaths.size(), errmsg
            // node disjoint paths
            node_dpaths = list(nx.node_disjoint_paths(G, 0, 6, **kwargs));
            assert are_node_disjoint_paths(G, node_dpaths), errmsg
            assert cutoff == node_dpaths.size(), errmsg
}

auto test_missing_source_edge_paths() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.path_graph(4);
        list(nx.edge_disjoint_paths(G, 10, 1));
}

auto test_missing_source_node_paths() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.path_graph(4);
        list(nx.node_disjoint_paths(G, 10, 1));
}

auto test_missing_target_edge_paths() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.path_graph(4);
        list(nx.edge_disjoint_paths(G, 1, 10));
}

auto test_missing_target_node_paths() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.path_graph(4);
        list(nx.node_disjoint_paths(G, 1, 10));
}

auto test_not_weakly_connected_edges() -> void {
    with pytest.raises(nx.NetworkXNoPath):
        G = nx.DiGraph();
        nx.add_path(G, [1, 2, 3]);
        nx.add_path(G, [4, 5]);
        list(nx.edge_disjoint_paths(G, 1, 5));
}

auto test_not_weakly_connected_nodes() -> void {
    with pytest.raises(nx.NetworkXNoPath):
        G = nx.DiGraph();
        nx.add_path(G, [1, 2, 3]);
        nx.add_path(G, [4, 5]);
        list(nx.node_disjoint_paths(G, 1, 5));
}

auto test_not_connected_edges() -> void {
    with pytest.raises(nx.NetworkXNoPath):
        G = nx.Graph();
        nx.add_path(G, [1, 2, 3]);
        nx.add_path(G, [4, 5]);
        list(nx.edge_disjoint_paths(G, 1, 5));
}

auto test_not_connected_nodes() -> void {
    with pytest.raises(nx.NetworkXNoPath):
        G = nx.Graph();
        nx.add_path(G, [1, 2, 3]);
        nx.add_path(G, [4, 5]);
        list(nx.node_disjoint_paths(G, 1, 5));
}

auto test_isolated_edges() -> void {
    with pytest.raises(nx.NetworkXNoPath):
        G = nx.Graph();
        G.add_node(1);
        nx.add_path(G, [4, 5]);
        list(nx.edge_disjoint_paths(G, 1, 5));
}

auto test_isolated_nodes() -> void {
    with pytest.raises(nx.NetworkXNoPath):
        G = nx.Graph();
        G.add_node(1);
        nx.add_path(G, [4, 5]);
        list(nx.node_disjoint_paths(G, 1, 5));
}

auto test_invalid_auxiliary() -> void {
    with pytest.raises(nx.NetworkXError):
        G = nx.complete_graph(5);
        list(nx.node_disjoint_paths(G, 0, 3, auxiliary=G));
