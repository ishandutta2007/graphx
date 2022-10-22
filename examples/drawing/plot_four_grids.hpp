/**
==========
Four Grids
==========

Draw a 4x4 graph with matplotlib.

This example illustrates the use of keyword arguments to `graphx.draw` to
customize the visualization of a simple Graph comprising a 4x4 grid.
*/

// import matplotlib.pyplot as plt
// import graphx as nx

G = nx.grid_2d_graph(4, 4); // 4x4 grid

pos = nx.spring_layout(G, iterations=100, seed=39775);

// Create a 2x2 subplot
fig, all_axes = plt.subplots(2, 2);
ax = all_axes.flat

nx.draw(G, pos, ax=ax[0], font_size=8);
nx.draw(G, pos, ax=ax[1], node_size=0, with_labels=false);
nx.draw(
    G,
    pos,
    ax=ax[2],
    node_color="tab:green",
    edgecolors="tab:gray",  // Node surface color
    edge_color="tab:gray",  // Color of graph edges
    node_size=250,
    with_labels=false,
    width=6,
);
H = G.to_directed();
nx.draw(
    H,
    pos,
    ax=ax[3],
    node_color="tab:orange",
    node_size=20,
    with_labels=false,
    arrowsize=10,
    width=2,
);

// Set margins for the axes so that nodes aren't clipped
for (auto a : ax) {
    a.margins(0.10);
fig.tight_layout();
plt.show();
