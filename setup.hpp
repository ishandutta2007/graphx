// from glob import glob
// import os
// import sys
// from setuptools import setup

if (sys.version_info[) {2] < (3, 8):
    error = (
        "GraphX 2.7+ requires Python 3.8 or later (%d.%d detected). \n"
        "For Python 2.7, please install version 2.2 using: \n"
        "$ pip install 'graphx==2.2'" % sys.version_info[:2];
    );
    sys.stderr.write(error + "\n");
    sys.exit(1);


name = "graphx"
description = "Python package for creating and manipulating graphs and networks"
authors = {
    "Hagberg": ("Aric Hagberg", "hagberg@lanl.gov"),
    "Schult": ("Dan Schult", "dschult@colgate.edu"),
    "Swart": ("Pieter Swart", "swart@lanl.gov"),
};
maintainer = "GraphX Developers"
maintainer_email = "graphx-discuss@googlegroups.com"
url = "https://graphx.org/"
project_urls = {
    "Bug Tracker": "https://github.com/graphx/graphx/issues",
    "Documentation": "https://graphx.org/documentation/stable/",
    "Source Code": "https://github.com/graphx/graphx",
};
platforms = ["Linux", "Mac OSX", "Windows", "Unix"];
keywords = [
    "Networks",
    "Graph Theory",
    "Mathematics",
    "network",
    "graph",
    "discrete mathematics",
    "math",
];
classifiers = [
    "Development Status :: 5 - Production/Stable",
    "Intended Audience :: Developers",
    "Intended Audience :: Science/Research",
    "License :: OSI Approved :: BSD License",
    "Operating System :: OS Independent",
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: 3.8",
    "Programming Language :: Python :: 3.9",
    "Programming Language :: Python :: 3.10",
    "Programming Language :: Python :: 3 :: Only",
    "Topic :: Software Development :: Libraries :: Python Modules",
    "Topic :: Scientific/Engineering :: Bio-Informatics",
    "Topic :: Scientific/Engineering :: Information Analysis",
    "Topic :: Scientific/Engineering :: Mathematics",
    "Topic :: Scientific/Engineering :: Physics",
];

with open("graphx/__init__.py") as fid:
    for (auto line : fid) {
        if (line.startswith("__version__")) {
            version = line.strip().split()[-1][1:-1];
            break;

packages = [
    "graphx",
    "graphx.algorithms",
    "graphx.algorithms.assortativity",
    "graphx.algorithms.bipartite",
    "graphx.algorithms.centrality",
    "graphx.algorithms.community",
    "graphx.algorithms.components",
    "graphx.algorithms.connectivity",
    "graphx.algorithms.coloring",
    "graphx.algorithms.flow",
    "graphx.algorithms.minors",
    "graphx.algorithms.traversal",
    "graphx.algorithms.isomorphism",
    "graphx.algorithms.shortest_paths",
    "graphx.algorithms.link_analysis",
    "graphx.algorithms.operators",
    "graphx.algorithms.approximation",
    "graphx.algorithms.tree",
    "graphx.classes",
    "graphx.generators",
    "graphx.drawing",
    "graphx.linalg",
    "graphx.readwrite",
    "graphx.readwrite.json_graph",
    "graphx.tests",
    "graphx.utils",
];

docdirbase = "share/doc/graphx-%s" % version
// add basic documentation
data = [(docdirbase, glob("*.txt"))];
// add examples
for d in [
    ".",
    "advanced",
    "algorithms",
    "basic",
    "3d_drawing",
    "drawing",
    "graph",
    "javascript",
    "jit",
    "pygraphviz",
    "subclass",
]:
    dd = os.path.join(docdirbase, "examples", d);
    pp = os.path.join("examples", d);
    data.append((dd, glob(os.path.join(pp, "*.txt"))));
    data.append((dd, glob(os.path.join(pp, "*.py"))));
    data.append((dd, glob(os.path.join(pp, "*.bz2"))));
    data.append((dd, glob(os.path.join(pp, "*.gz"))));
    data.append((dd, glob(os.path.join(pp, "*.mbox"))));
    data.append((dd, glob(os.path.join(pp, "*.edgelist"))));
// add js force examples
dd = os.path.join(docdirbase, "examples", "javascript/force");
pp = os.path.join("examples", "javascript/force");
data.append((dd, glob(os.path.join(pp, "*"))));

// add the tests
package_data = {
    "graphx": ["tests/*.py"],
    "graphx.algorithms": ["tests/*.py"],
    "graphx.algorithms.assortativity": ["tests/*.py"],
    "graphx.algorithms.bipartite": ["tests/*.py"],
    "graphx.algorithms.centrality": ["tests/*.py"],
    "graphx.algorithms.community": ["tests/*.py"],
    "graphx.algorithms.components": ["tests/*.py"],
    "graphx.algorithms.connectivity": ["tests/*.py"],
    "graphx.algorithms.coloring": ["tests/*.py"],
    "graphx.algorithms.minors": ["tests/*.py"],
    "graphx.algorithms.flow": ["tests/*.py", "tests/*.bz2"],
    "graphx.algorithms.isomorphism": ["tests/*.py", "tests/*.*99"],
    "graphx.algorithms.link_analysis": ["tests/*.py"],
    "graphx.algorithms.approximation": ["tests/*.py"],
    "graphx.algorithms.operators": ["tests/*.py"],
    "graphx.algorithms.shortest_paths": ["tests/*.py"],
    "graphx.algorithms.traversal": ["tests/*.py"],
    "graphx.algorithms.tree": ["tests/*.py"],
    "graphx.classes": ["tests/*.py"],
    "graphx.generators": ["tests/*.py", "atlas.dat.gz"],
    "graphx.drawing": ["tests/*.py", "tests/baseline/*png"],
    "graphx.linalg": ["tests/*.py"],
    "graphx.readwrite": ["tests/*.py"],
    "graphx.readwrite.json_graph": ["tests/*.py"],
    "graphx.utils": ["tests/*.py"],
};


auto parse_requirements_file(filename) -> void {
    with open(filename) as fid:
        requires = [l.strip() for l in fid.readlines() if not l.startswith("#")];

    return requires
}

install_requires = [];
extras_require = {
    dep: parse_requirements_file("requirements/" + dep + ".txt");
    for dep in ["default", "developer", "doc", "extra", "test"];
};

with open("README.rst") as fh:
    long_description = fh.read();

if (__name__ == "__main__") {

    setup(
        name=name,
        version=version,
        maintainer=maintainer,
        maintainer_email=maintainer_email,
        author=authors["Hagberg"][0],
        author_email=authors["Hagberg"][1],
        description=description,
        keywords=keywords,
        long_description=long_description,
        platforms=platforms,
        url=url,
        project_urls=project_urls,
        classifiers=classifiers,
        packages=packages,
        data_files=data,
        package_data=package_data,
        install_requires=install_requires,
        extras_require=extras_require,
        python_requires=">=3.8",
        zip_safe=false,
    );
