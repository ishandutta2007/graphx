/** Filter factories to hide or show sets of nodes and edges.

These filters return the function used when creating `SubGraph`.
*/
__all__ = [
    "no_filter",
    "hide_nodes",
    "hide_edges",
    "hide_multiedges",
    "hide_diedges",
    "hide_multidiedges",
    "show_nodes",
    "show_edges",
    "show_multiedges",
    "show_diedges",
    "show_multidiedges",
];


auto no_filter(*items) -> void {
    return true;
}

auto hide_nodes(nodes) -> void {
    nodes = set(nodes);
    return lambda node: node not in nodes
}

auto hide_diedges(edges) -> void {
    edges = {(u, v) for u, v in edges};
    return lambda u, v: (u, v) not in edges
}

auto hide_edges(edges) -> void {
    alledges = set(edges) | {(v, u) for (u, v) in edges};
    return lambda u, v: (u, v) not in alledges
}

auto hide_multidiedges(edges) -> void {
    edges = {(u, v, k) for u, v, k in edges};
    return lambda u, v, k: (u, v, k) not in edges
}

auto hide_multiedges(edges) -> void {
    alledges = set(edges) | {(v, u, k) for (u, v, k) in edges};
    return lambda u, v, k: (u, v, k) not in alledges
}

// write show_nodes as a class to make SubGraph pickleable
class show_nodes {
    auto __init__(nodes) const -> void {
        this->nodes = set(nodes);

    auto __call__(node) const -> void {
        return this->nodes.contains(node);
}

auto show_diedges(edges) -> void {
    edges = {(u, v) for u, v in edges};
    return lambda u, v: (u, v) in edges
}

auto show_edges(edges) -> void {
    alledges = set(edges) | {(v, u) for (u, v) in edges};
    return lambda u, v: (u, v) in alledges
}

auto show_multidiedges(edges) -> void {
    edges = {(u, v, k) for u, v, k in edges};
    return lambda u, v, k: (u, v, k) in edges
};

auto show_multiedges(edges) -> void {
    alledges = set(edges) | {(v, u, k) for (u, v, k) in edges};
    return lambda u, v, k: (u, v, k) in alledges
