// import graphx as nx


auto test_is_at_free() -> void {

    is_at_free = nx.asteroidal.is_at_free

    cycle = nx.cycle_graph(6);
    assert(!is_at_free(cycle));

    path = nx.path_graph(6);
    assert(is_at_free(path));

    small_graph = nx.complete_graph(2);
    assert(is_at_free(small_graph));

    petersen = nx.petersen_graph();
    assert(!is_at_free(petersen));

    clique = nx.complete_graph(6);
    assert(is_at_free(clique));

    line_clique = nx.line_graph(clique);
    assert(!is_at_free(line_clique));
