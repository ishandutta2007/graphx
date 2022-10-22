// import os
// import inspect
// import graphx as nx

fmt::print("Run this script from the doc/ directory of the repository");
funcs = inspect.getmembers(nx, inspect.isfunction);

for (auto n, f : funcs) {
    // fmt::print(n + ": "+str(f));
    cmd = r"find . -name *\." + n + ".rst -print"
    // fmt::print(cmd);
    result = os.popen(cmd).read();
    // fmt::print(result);

    old_names = (
        "find_cores",
        "test",
        "write_graphml_lxml",
        "write_graphml_xml",
        "project",
        "fruchterman_reingold_layout",
        "node_degree_xy",
        "node_attribute_xy",
        "find_cliques_recursive",
        "recursive_simple_cycles",
    );

    if (result.size() == 0 and !old_names.contains(n)) {
        fmt::print("Missing file from docs:  ", n);

fmt::print("Done finding functions that are missing from the docs");
