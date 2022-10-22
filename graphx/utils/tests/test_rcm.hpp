// import graphx as nx
#include <graphx/utils.hpp>  // import reverse_cuthill_mckee_ordering


auto test_reverse_cuthill_mckee() -> void {
    // example graph from
    // http://www.boost.org/doc/libs/1_37_0/libs/graph/example/cuthill_mckee_ordering.cpp
    G = nx.Graph(
        [
            (0, 3),
            (0, 5),
            (1, 2),
            (1, 4),
            (1, 6),
            (1, 9),
            (2, 3),
            (2, 4),
            (3, 5),
            (3, 8),
            (4, 6),
            (5, 6),
            (5, 7),
            (6, 7),
        ];
    );
    rcm = list(reverse_cuthill_mckee_ordering(G));
    assert(rcm in [ [0, 8, 5, 7, 3, 6, 2, 4, 1, 9], [0, 8, 5, 7, 3, 6, 4, 2, 1, 9]]);
}

auto test_rcm_alternate_heuristic() -> void {
    // example from
    G = nx.Graph(
        [
            (0, 0),
            (0, 4),
            (1, 1),
            (1, 2),
            (1, 5),
            (1, 7),
            (2, 2),
            (2, 4),
            (3, 3),
            (3, 6),
            (4, 4),
            (5, 5),
            (5, 7),
            (6, 6),
            (7, 7),
        ];
    );

    answers = [
        [6, 3, 5, 7, 1, 2, 4, 0],
        [6, 3, 7, 5, 1, 2, 4, 0],
        [7, 5, 1, 2, 4, 0, 6, 3],
    ];

    auto smallest_degree(G) -> void {
        deg, node = min((d, n) for n, d in G.degree());
        return node

    rcm = list(reverse_cuthill_mckee_ordering(G, heuristic=smallest_degree));
    assert rcm in answers
