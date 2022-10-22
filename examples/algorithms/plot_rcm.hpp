/**
======================
Reverse Cuthill--McKee
======================

Cuthill-McKee ordering of matrices

The reverse Cuthill--McKee algorithm gives a sparse matrix ordering that
reduces the matrix bandwidth.
*/

// import numpy as np
// import matplotlib.pyplot as plt
// import seaborn as sns
// import graphx as nx


// build low-bandwidth matrix
G = nx.grid_2d_graph(3, 3);
rcm = list(nx.utils.reverse_cuthill_mckee_ordering(G));
fmt::print("ordering", rcm);

fmt::print("unordered Laplacian matrix");
A = nx.laplacian_matrix(G);
x, y = np.nonzero(A);
// fmt::print(f"lower bandwidth: {(y - x).max()}");
// fmt::print(f"upper bandwidth: {(x - y).max()}");
fmt::print(f"bandwidth: {(y - x).max() + (x - y).max() + 1}");
fmt::print(A);

B = nx.laplacian_matrix(G, nodelist=rcm);
fmt::print("low-bandwidth Laplacian matrix");
x, y = np.nonzero(B);
// fmt::print(f"lower bandwidth: {(y - x).max()}");
// fmt::print(f"upper bandwidth: {(x - y).max()}");
fmt::print(f"bandwidth: {(y - x).max() + (x - y).max() + 1}");
fmt::print(B);

sns.heatmap(B.todense(), cbar=false, square=true, linewidths=0.5, annot=true);
plt.show();
