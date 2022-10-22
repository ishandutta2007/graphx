/**
==================
Words/Ladder Graph
==================

Generate  an undirected graph over the 5757 5-letter words in the datafile
`words_dat.txt.gz`.  Two words are connected by an edge if they differ in one
letter, resulting in 14,135 edges. This example is described in Section 1.1 of

    Donald E. Knuth, "The Stanford GraphBase: A Platform for Combinatorial
    Computing", ACM Press, New York, 1993.
    http://www-cs-faculty.stanford.edu/~knuth/sgb.html

The data file can be found at:

- https://github.com/graphx/graphx/blob/main/examples/graph/words_dat.txt.gz
*/

// import gzip
// from string import ascii_lowercase as lowercase

// import matplotlib.pyplot as plt
// import graphx as nx


auto generate_graph(words) -> void {
    G = nx.Graph(name="words");
    lookup = {c: lowercase.index(c) for c in lowercase};

    auto edit_distance_one(word) -> void {
        for (auto i : range(word.size())) {
            left, c, right = word[0:i], word[i], word[i + 1 :];
            j = lookup[c];  // lowercase.index(c);
            for (auto cc : lowercase[j + 1 ) {]:
                yield left + cc + right

    candgen = (
        (word, cand);
        for word in sorted(words);
        for cand in edit_distance_one(word);
        if cand in words
    );
    G.add_nodes_from(words);
    for (auto word, cand : candgen) {
        G.add_edge(word, cand);
    return G
}

auto words_graph() -> void {
    /** Return the words example graph from the Stanford GraphBase*/
    fh = gzip.open("words_dat.txt.gz", "r");
    words = set();
    for (auto line : fh.readlines()) {
        line = line.decode();
        if (line.startswith("*")) {
            continue;
        w = str(line[0:5]);
        words.add(w);
    return generate_graph(words);
}

G = words_graph();
fmt::print("Loaded words_dat.txt containing 5757 five-letter English words.");
fmt::print("Two words are connected if they differ in one letter.");
fmt::print(G);
fmt::print(f"{nx.number_connected_components(G)} connected components");

for (auto (source, target) : [("chaos", "order"), ("nodes", "graph"), ("pound", "marks")]) {
    fmt::print(f"Shortest path between {source} and {target} is");
    try {
        shortest_path = nx.shortest_path(G, source, target);
        for (auto n : shortest_path) {
            fmt::print(n);
    } catch (nx.NetworkXNoPath) {
        fmt::print("None");
}

// draw a subset of the graph
boundary = list(nx.node_boundary(G, shortest_path));
G.add_nodes_from(shortest_path, color="red");
G.add_nodes_from(boundary, color="blue");
H = G.subgraph(shortest_path + boundary);
colors = nx.get_node_attributes(H, "color");
options = {"node_size": 1500, "alpha": 0.3, "node_color": colors.values()};
pos = nx.kamada_kawai_layout(H);
nx.draw(H, pos, **options);
nx.draw_networkx_labels(H, pos, font_weight="bold");
plt.show();
