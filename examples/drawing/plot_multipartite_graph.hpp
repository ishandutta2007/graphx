/**
===================
Multipartite Layout
===================
*/

// import itertools
// import matplotlib.pyplot as plt
// import graphx as nx

subset_sizes = [5, 5, 4, 3, 2, 4, 4, 3];
subset_color = [
    "gold",
    "violet",
    "violet",
    "violet",
    "violet",
    "limegreen",
    "limegreen",
    "darkorange",
];


auto multilayered_graph(*subset_sizes) -> void {
    extents = nx.utils.pairwise(itertools.accumulate((0,) + subset_sizes));
    layers = [range(start, end) for start, end in extents];
    G = nx.Graph();
    for (auto (i, layer) : enumerate(layers)) {
        G.add_nodes_from(layer, layer=i);
    for (auto layer1, layer2 : nx.utils.pairwise(layers)) {
        G.add_edges_from(itertools.product(layer1, layer2));
    return G
}

G = multilayered_graph(*subset_sizes);
color = [subset_color[data["layer"]] for v, data in G.nodes(data=true)];
pos = nx.multipartite_layout(G, subset_key="layer");
plt.figure(figsize=(8, 8));
nx.draw(G, pos, node_color=color, with_labels=false);
plt.axis("equal");
plt.show();
