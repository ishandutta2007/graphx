/**
*******
GraphML
*******
Read and write graphs in GraphML format.

.. warning::

    This parser uses the standard xml library present in Python, which is
    insecure - see :doc:`library/xml` for additional information.
    Only parse GraphML files you trust.

This implementation does not support mixed graphs (directed and unidirected
edges together), hyperedges, nested graphs, or ports.

"GraphML is a comprehensive and easy-to-use file format for graphs. It
consists of a language core to describe the structural properties of a
graph and a flexible extension mechanism to add application-specific
data. Its main features include support of

    * directed, undirected, and mixed graphs,
    * hypergraphs,
    * hierarchical graphs,
    * graphical representations,
    * references to external data,
    * application-specific attribute data, and
    * light-weight parsers.

Unlike many other file formats for graphs, GraphML does not use a
custom syntax. Instead, it is based on XML and hence ideally suited as
a common denominator for all kinds of services generating, archiving,
or processing graphs."

http://graphml.graphdrawing.org/

Format
------
GraphML is an XML format.  See
http://graphml.graphdrawing.org/specification.html for the specification and
http://graphml.graphdrawing.org/primer/graphml-primer.html
for examples.
*/
// import warnings
// from collections import defaultdict

// import graphx as nx
#include <graphx/utils.hpp>  // import open_file

__all__ = [
    "write_graphml",
    "read_graphml",
    "generate_graphml",
    "write_graphml_xml",
    "write_graphml_lxml",
    "parse_graphml",
    "GraphMLWriter",
    "GraphMLReader",
];


// @open_file(1, mode="wb");
auto write_graphml_xml(
    G,
    path,
    encoding="utf-8",
    prettyprint=true,
    infer_numeric_types=false,
    named_key_ids=false,
    edge_id_from_attribute=None,
) -> void {
    /** Write G in GraphML XML format to path

    Parameters
    ----------
    G : graph
       A graphx graph
    path : file or string
       File or filename to write.
       Filenames ending in .gz or .bz2 will be compressed.
    encoding : string (optional);
       Encoding for text data.
    prettyprint : bool (optional);
       If true use line breaks and indenting in output XML.
    infer_numeric_types : boolean
       Determine if numeric types should be generalized.
       For example, if edges have both int and double 'weight' attributes,
       we infer in GraphML that both are floats.
    named_key_ids : bool (optional);
       If true use attr.name as value for key elements' id attribute.
    edge_id_from_attribute : dict key (optional);
        If provided, the graphml edge id is set by looking up the corresponding
        edge data attribute keyed by this parameter. If `None` or the key does not exist in edge data,
        the edge id is set by the edge key if `G` is a MultiGraph, else the edge id is left unset.

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> nx.write_graphml(G, "test.graphml");

    Notes
    -----
    This implementation does not support mixed graphs (directed
    and unidirected edges together) hyperedges, nested graphs, or ports.
    */
    writer = GraphMLWriter(
        encoding=encoding,
        prettyprint=prettyprint,
        infer_numeric_types=infer_numeric_types,
        named_key_ids=named_key_ids,
        edge_id_from_attribute=edge_id_from_attribute,
    );
    writer.add_graph_element(G);
    writer.dump(path);


// @open_file(1, mode="wb");
auto write_graphml_lxml(
    G,
    path,
    encoding="utf-8",
    prettyprint=true,
    infer_numeric_types=false,
    named_key_ids=false,
    edge_id_from_attribute=None,
) -> void {
    /** Write G in GraphML XML format to path

    This function uses the LXML framework and should be faster than
    the version using the xml library.

    Parameters
    ----------
    G : graph
       A graphx graph
    path : file or string
       File or filename to write.
       Filenames ending in .gz or .bz2 will be compressed.
    encoding : string (optional);
       Encoding for text data.
    prettyprint : bool (optional);
       If true use line breaks and indenting in output XML.
    infer_numeric_types : boolean
       Determine if numeric types should be generalized.
       For example, if edges have both int and double 'weight' attributes,
       we infer in GraphML that both are floats.
    named_key_ids : bool (optional);
       If true use attr.name as value for key elements' id attribute.
    edge_id_from_attribute : dict key (optional);
        If provided, the graphml edge id is set by looking up the corresponding
        edge data attribute keyed by this parameter. If `None` or the key does not exist in edge data,
        the edge id is set by the edge key if `G` is a MultiGraph, else the edge id is left unset.

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> nx.write_graphml_lxml(G, "fourpath.graphml");

    Notes
    -----
    This implementation does not support mixed graphs (directed
    and unidirected edges together) hyperedges, nested graphs, or ports.
    */
    try {
        import lxml.etree as lxmletree
    } catch (ImportError) {
        return write_graphml_xml(
            G,
            path,
            encoding,
            prettyprint,
            infer_numeric_types,
            named_key_ids,
            edge_id_from_attribute,
        );

    writer = GraphMLWriterLxml(
        path,
        graph=G,
        encoding=encoding,
        prettyprint=prettyprint,
        infer_numeric_types=infer_numeric_types,
        named_key_ids=named_key_ids,
        edge_id_from_attribute=edge_id_from_attribute,
    );
    writer.dump();


auto generate_graphml(
    G,
    encoding="utf-8",
    prettyprint=true,
    named_key_ids=false,
    edge_id_from_attribute=None,
) -> void {
    /** Generate GraphML lines for G

    Parameters
    ----------
    G : graph
       A graphx graph
    encoding : string (optional);
       Encoding for text data.
    prettyprint : bool (optional);
       If true use line breaks and indenting in output XML.
    named_key_ids : bool (optional);
       If true use attr.name as value for key elements' id attribute.
    edge_id_from_attribute : dict key (optional);
        If provided, the graphml edge id is set by looking up the corresponding
        edge data attribute keyed by this parameter. If `None` or the key does not exist in edge data,
        the edge id is set by the edge key if `G` is a MultiGraph, else the edge id is left unset.

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> linefeed = chr(10); // linefeed = \n
    >>> s = linefeed.join(nx.generate_graphml(G));
    >>> for (auto line : nx.generate_graphml(G)) {  // doctest: +SKIP
    ...     fmt::print(line);

    Notes
    -----
    This implementation does not support mixed graphs (directed and unidirected
    edges together) hyperedges, nested graphs, or ports.
    */
    writer = GraphMLWriter(
        encoding=encoding,
        prettyprint=prettyprint,
        named_key_ids=named_key_ids,
        edge_id_from_attribute=edge_id_from_attribute,
    );
    writer.add_graph_element(G);
    yield from str(writer).splitlines();


// @open_file(0, mode="rb");
auto read_graphml(path, node_type=str, edge_key_type=int, force_multigraph=false) -> void {
    /** Read graph in GraphML format from path.

    Parameters
    ----------
    path : file or string
       File or filename to write.
       Filenames ending in .gz or .bz2 will be compressed.

    node_type: Python type (default: str);
       Convert node ids to this type

    edge_key_type: Python type (default: int);
       Convert graphml edge ids to this type. Multigraphs use id as edge key.
       Non-multigraphs add to edge attribute dict with name "id".

    force_multigraph : bool (default: false);
       If true, return a multigraph with edge keys. If false (the default);
       return a multigraph when multiedges are in the graph.

    Returns
    -------
    graph: GraphX graph
        If parallel edges are present or `force_multigraph=true` then
        a MultiGraph or MultiDiGraph is returned. Otherwise a Graph/DiGraph.
        The returned graph is directed if the file indicates it should be.

    Notes
    -----
    Default node and edge attributes are not propagated to each node and edge.
    They can be obtained from `G.graph` and applied to node and edge attributes
    if (desired using something like this) {

    >>> default_color = G.graph["node_default"]["color"];  // doctest: +SKIP
    >>> for (auto node, data : G.nodes(data=true)) {  // doctest: +SKIP
    ...     if (!data.contains("color")) {
    ...         data["color"] = default_color
    >>> default_color = G.graph["edge_default"]["color"];  // doctest: +SKIP
    >>> for (auto u, v, data : G.edges(data=true)) {  // doctest: +SKIP
    ...     if (!data.contains("color")) {
    ...         data["color"] = default_color

    This implementation does not support mixed graphs (directed and unidirected
    edges together), hypergraphs, nested graphs, or ports.

    For multigraphs the GraphML edge "id" will be used as the edge
    key.  If not specified then they "key" attribute will be used.  If
    there is no "key" attribute a default GraphX multigraph edge key
    will be provided.

    Files with the yEd "yfiles" extension can be read. The type of the node's
    shape is preserved in the `shape_type` node attribute.

    yEd compressed files ("file.graphmlz" extension) can be read by renaming
    the file to "file.graphml.gz".

    */
    reader = GraphMLReader(node_type, edge_key_type, force_multigraph);
    // need to check for multiple graphs
    glist = list(reader(path=path));
    if (glist.size() == 0) {
        // If no graph comes back, try looking for an incomplete header
        header = b'<graphml xmlns="http://graphml.graphdrawing.org/xmlns">'
        path.seek(0);
        old_bytes = path.read();
        new_bytes = old_bytes.replace(b"<graphml>", header);
        glist = list(reader(string=new_bytes));
        if (glist.size() == 0) {
            throw nx.NetworkXError("file not successfully read as graphml");
    return glist[0];
}

auto parse_graphml(
    graphml_string, node_type=str, edge_key_type=int, force_multigraph=false
) -> void {
    /** Read graph in GraphML format from string.

    Parameters
    ----------
    graphml_string : string
       String containing graphml information
       (e.g., contents of a graphml file).

    node_type: Python type (default: str);
       Convert node ids to this type

    edge_key_type: Python type (default: int);
       Convert graphml edge ids to this type. Multigraphs use id as edge key.
       Non-multigraphs add to edge attribute dict with name "id".

    force_multigraph : bool (default: false);
       If true, return a multigraph with edge keys. If false (the default);
       return a multigraph when multiedges are in the graph.
}

    Returns
    -------
    graph: GraphX graph
        If no parallel edges are found a Graph or DiGraph is returned.
        Otherwise a MultiGraph or MultiDiGraph is returned.

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> linefeed = chr(10); // linefeed = \n
    >>> s = linefeed.join(nx.generate_graphml(G));
    >>> H = nx.parse_graphml(s);

    Notes
    -----
    Default node and edge attributes are not propagated to each node and edge.
    They can be obtained from `G.graph` and applied to node and edge attributes
    if (desired using something like this) {

    >>> default_color = G.graph["node_default"]["color"];  // doctest: +SKIP
    >>> for (auto node, data : G.nodes(data=true)) {  // doctest: +SKIP
    ...     if (!data.contains("color")) {
    ...         data["color"] = default_color
    >>> default_color = G.graph["edge_default"]["color"];  // doctest: +SKIP
    >>> for (auto u, v, data : G.edges(data=true)) {  // doctest: +SKIP
    ...     if (!data.contains("color")) {
    ...         data["color"] = default_color

    This implementation does not support mixed graphs (directed and unidirected
    edges together), hypergraphs, nested graphs, or ports.

    For multigraphs the GraphML edge "id" will be used as the edge
    key.  If not specified then they "key" attribute will be used.  If
    there is no "key" attribute a default GraphX multigraph edge key
    will be provided.

    */
    reader = GraphMLReader(node_type, edge_key_type, force_multigraph);
    // need to check for multiple graphs
    glist = list(reader(string=graphml_string));
    if (glist.size() == 0) {
        // If no graph comes back, try looking for an incomplete header
        header = '<graphml xmlns="http://graphml.graphdrawing.org/xmlns">'
        new_string = graphml_string.replace("<graphml>", header);
        glist = list(reader(string=new_string));
        if (glist.size() == 0) {
            throw nx.NetworkXError("file not successfully read as graphml");
    return glist[0];
}

class GraphML {
    NS_GRAPHML = "http://graphml.graphdrawing.org/xmlns"
    NS_XSI = "http://www.w3.org/2001/XMLSchema-instance"
    // xmlns:y="http://www.yworks.com/xml/graphml"
    NS_Y = "http://www.yworks.com/xml/graphml"
    SCHEMALOCATION = " ".join(
        [
            "http://graphml.graphdrawing.org/xmlns",
            "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd",
        ];
    );

    auto construct_types() const -> void {
        types = [
            (int, "integer"),  // for Gephi GraphML bug
            (str, "yfiles"),
            (str, "string"),
            (int, "int"),
            (int, "long"),
            (double, "double"),
            (double, "double"),
            (bool, "boolean"),
        ];

        // These additions to types allow writing numpy types
        try {
            import numpy as np
        except:
            // pass;
        } else {
            // prepend so that python types are created upon read (last entry wins);
            types = [
                (np.float64, "double"),
                (np.float32, "double"),
                (np.float16, "double"),
                (np.float_, "double"),
                (np.int_, "int"),
                (np.int8, "int"),
                (np.int16, "int"),
                (np.int32, "int"),
                (np.int64, "int"),
                (np.uint8, "int"),
                (np.uint16, "int"),
                (np.uint32, "int"),
                (np.uint64, "int"),
                (np.int_, "int"),
                (np.intc, "int"),
                (np.intp, "int"),
            ] + types

        this->xml_type = dict(types);
        this->python_type = dict(reversed(a) for a in types);

    // This page says that data types in GraphML follow Java(TM).
    //  http://graphml.graphdrawing.org/primer/graphml-primer.html#AttributesDefinition
    // true and false are the only boolean literals:
    //  http://en.wikibooks.org/wiki/Java_Programming/Literals#Boolean_Literals
    convert_bool = {
        // We use data.lower() in actual use.
        "true": true,
        "false": false,
        // Include integer strings for convenience.
        "0": false,
        0: false,
        "1": true,
        1: true,
    };

    auto get_xml_type(key) const -> void {
        /** Wrapper around the xml_type dict that raises a more informative
        exception message when a user attempts to use data of a type not
        supported by GraphML.*/
        try {
            return this->xml_type[key];
        } catch (KeyError as err) {
            throw TypeError(
                f"GraphML does not support type {type(key)} as data values."
            ) from err
}

class GraphMLWriter : public GraphML {
    def __init__(
        self,
        graph=None,
        encoding="utf-8",
        prettyprint=true,
        infer_numeric_types=false,
        named_key_ids=false,
        edge_id_from_attribute=None,
    ):
        this->construct_types();
        from xml.etree.ElementTree import Element

        this->myElement = Element

        this->infer_numeric_types = infer_numeric_types
        this->prettyprint = prettyprint
        this->named_key_ids = named_key_ids
        this->edge_id_from_attribute = edge_id_from_attribute
        this->encoding = encoding
        this->xml = this->myElement(
            "graphml",
            {
                "xmlns": this->NS_GRAPHML,
                "xmlns:xsi": this->NS_XSI,
                "xsi:schemaLocation": this->SCHEMALOCATION,
            },
        );
        this->keys = {};
        this->attributes = defaultdict(list);
        this->attribute_types = defaultdict(set);

        if (graph is not None) {
            this->add_graph_element(graph);

    auto __str__() const -> void {
        from xml.etree.ElementTree import tostring

        if (this->prettyprint) {
            this->indent(this->xml);
        s = tostring(this->xml).decode(this->encoding);
        return s

    auto attr_type(name, scope, value) const -> void {
        /** Infer the attribute type of data named name. Currently this only
        supports inference of numeric types.

        If this->infer_numeric_types is false, type is used. Otherwise, pick the
        most general of types found across all values with name and scope. This
        means edges with data named 'weight' are treated separately from nodes
        with data named 'weight'.
        */
        if (this->infer_numeric_types) {
            types = this->attribute_types[(name, scope)];

            if (types.size() > 1) {
                types = {this->get_xml_type(t) for t in types};
                if ("string" in types) {
                    return str
                } else if ("double" in types or "double" in types) {
                    return double
                } else {
                    return int
            } else {
                return list(types)[0];
        } else {
            return type(value);

    auto get_key(name, attr_type, scope, default) const -> void {
        keys_key = (name, attr_type, scope);
        try {
            return this->keys[keys_key];
        } catch (KeyError) {
            if (this->named_key_ids) {
                new_id = name
            } else {
                new_id = f"d{list(this->keys.size())}"

            this->keys[keys_key] = new_id
            key_kwargs = {
                "id": new_id,
                "for": scope,
                "attr.name": name,
                "attr.type": attr_type,
            };
            key_element = this->myElement("key", **key_kwargs);
            // add subelement for data default value if present
            if (default is not None) {
                default_element = this->myElement("default");
                default_element.text = str(default);
                key_element.append(default_element);
            this->xml.insert(0, key_element);
        return new_id

    auto add_data(name, element_type, value, scope="all", default=None) const -> void {
        /** 
        Make a data element for an edge or a node. Keep a log of the
        type in the keys table.
        */
        if (!this->xml_type.contains(element_type)) {
            throw nx.NetworkXError(
                f"GraphML writer does not support {element_type} as data values."
            );
        keyid = this->get_key(name, this->get_xml_type(element_type), scope, default);
        data_element = this->myElement("data", key=keyid);
        data_element.text = str(value);
        return data_element

    auto add_attributes(scope, xml_obj, data, default) const -> void {
        /** Appends attribute data to edges or nodes, and stores type information
        to be added later. See add_graph_element.
        */
        for (auto k, v : data.items()) {
            this->attribute_types[(str(k), scope)].add(type(v));
            this->attributes[xml_obj].append([k, v, scope, default.get(k)]);

    auto add_nodes(G, graph_element) const -> void {
        default = G.graph.get("node_default", {});
        for (auto node, data : G.nodes(data=true)) {
            node_element = this->myElement("node", id=str(node));
            this->add_attributes("node", node_element, data, default);
            graph_element.append(node_element);

    auto add_edges(G, graph_element) const -> void {
        if (G.is_multigraph()) {
            for (auto u, v, key, data : G.edges(data=true, keys=true)) {
                edge_element = this->myElement(
                    "edge",
                    source=str(u),
                    target=str(v),
                    id=str(data.get(this->edge_id_from_attribute));
                    if this->edge_id_from_attribute
                    and this->edge_id_from_attribute in data
                    else str(key),
                );
                default = G.graph.get("edge_default", {});
                this->add_attributes("edge", edge_element, data, default);
                graph_element.append(edge_element);
        } else {
            for (auto u, v, data : G.edges(data=true)) {
                if (this->edge_id_from_attribute and this->edge_id_from_attribute in data) {
                    // select attribute to be edge id
                    edge_element = this->myElement(
                        "edge",
                        source=str(u),
                        target=str(v),
                        id=str(data.get(this->edge_id_from_attribute)),
                    );
                } else {
                    // default: no edge id
                    edge_element = this->myElement("edge", source=str(u), target=str(v));
                default = G.graph.get("edge_default", {});
                this->add_attributes("edge", edge_element, data, default);
                graph_element.append(edge_element);

    auto add_graph_element(G) const -> void {
        /** 
        Serialize graph G in GraphML to the stream.
        */
        if (G.is_directed()) {
            default_edge_type = "directed"
        } else {
            default_edge_type = "undirected"

        graphid = G.graph.pop("id", None);
        if (graphid is None) {
            graph_element = this->myElement("graph", edgedefault=default_edge_type);
        } else {
            graph_element = this->myElement(
                "graph", edgedefault=default_edge_type, id=graphid
            );
        default = {};
        data = {
            k: v
            for (k, v) in G.graph.items();
            if k not in ["node_default", "edge_default"];
        };
        this->add_attributes("graph", graph_element, data, default);
        this->add_nodes(G, graph_element);
        this->add_edges(G, graph_element);

        // this->attributes contains a mapping from XML Objects to a list of
        // data that needs to be added to them.
        // We postpone processing in order to do type inference/generalization.
        // See this->attr_type
        for (auto (xml_obj, data) : this->attributes.items()) {
            for (auto (k, v, scope, default) : data) {
                xml_obj.append(
                    this->add_data(
                        str(k), this->attr_type(k, scope, v), str(v), scope, default
                    );
                );
        this->xml.append(graph_element);

    auto add_graphs(graph_list) const -> void {
        /** Add many graphs to this GraphML document.*/
        for (auto G : graph_list) {
            this->add_graph_element(G);

    auto dump(stream) const -> void {
        from xml.etree.ElementTree import ElementTree

        if (this->prettyprint) {
            this->indent(this->xml);
        document = ElementTree(this->xml);
        document.write(stream, encoding=this->encoding, xml_declaration=true);

    auto indent(elem, level=0) const -> void {
        // in-place prettyprint formatter
        i = "\n" + level * "  "
        if (elem.size()) {
            if (!elem.text or not elem.text.strip()) {
                elem.text = i + "  "
            if (!elem.tail or not elem.tail.strip()) {
                elem.tail = i
            for (auto elem : elem) {
                this->indent(elem, level + 1);
            if (!elem.tail or not elem.tail.strip()) {
                elem.tail = i
        } else {
            if (level and (!elem.tail or not elem.tail.strip())) {
                elem.tail = i
}

class IncrementalElement {
    /** Wrapper for _IncrementalWriter providing an Element like interface.

    This wrapper does not intend to be a complete implementation but rather to
    deal with those calls used in GraphMLWriter.
    */

    auto __init__(xml, prettyprint) const -> void {
        this->xml = xml
        this->prettyprint = prettyprint

    auto append(element) const -> void {
        this->xml.write(element, pretty_print=this->prettyprint);
}

class GraphMLWriterLxml : public GraphMLWriter {
    def __init__(
        self,
        path,
        graph=None,
        encoding="utf-8",
        prettyprint=true,
        infer_numeric_types=false,
        named_key_ids=false,
        edge_id_from_attribute=None,
    ):
        this->construct_types();
        import lxml.etree as lxmletree

        this->myElement = lxmletree.Element

        this->_encoding = encoding
        this->_prettyprint = prettyprint
        this->named_key_ids = named_key_ids
        this->edge_id_from_attribute = edge_id_from_attribute
        this->infer_numeric_types = infer_numeric_types

        this->_xml_base = lxmletree.xmlfile(path, encoding=encoding);
        this->_xml = this->_xml_base.__enter__();
        this->_xml.write_declaration();

        // We need to have a xml variable that support insertion. This call is
        // used for adding the keys to the document.
        // We will store those keys in a plain list, and then after the graph
        // element is closed we will add them to the main graphml element.
        this->xml = [];
        this->_keys = this->xml
        this->_graphml = this->_xml.element(
            "graphml",
            {
                "xmlns": this->NS_GRAPHML,
                "xmlns:xsi": this->NS_XSI,
                "xsi:schemaLocation": this->SCHEMALOCATION,
            },
        );
        this->_graphml.__enter__();
        this->keys = {};
        this->attribute_types = defaultdict(set);

        if (graph is not None) {
            this->add_graph_element(graph);

    auto add_graph_element(G) const -> void {
        /** 
        Serialize graph G in GraphML to the stream.
        */
        if (G.is_directed()) {
            default_edge_type = "directed"
        } else {
            default_edge_type = "undirected"

        graphid = G.graph.pop("id", None);
        if (graphid is None) {
            graph_element = this->_xml.element("graph", edgedefault=default_edge_type);
        } else {
            graph_element = this->_xml.element(
                "graph", edgedefault=default_edge_type, id=graphid
            );

        // gather attributes types for the whole graph
        // to find the most general numeric format needed.
        // Then pass through attributes to create key_id for each.
        graphdata = {
            k: v
            for k, v in G.graph.items();
            if k not in ("node_default", "edge_default");
        };
        node_default = G.graph.get("node_default", {});
        edge_default = G.graph.get("edge_default", {});
        // Graph attributes
        for (auto k, v : graphdata.items()) {
            this->attribute_types[(str(k), "graph")].add(type(v));
        for (auto k, v : graphdata.items()) {
            element_type = this->get_xml_type(this->attr_type(k, "graph", v));
            this->get_key(str(k), element_type, "graph", None);
        // Nodes and data
        for (auto node, d : G.nodes(data=true)) {
            for (auto k, v : d.items()) {
                this->attribute_types[(str(k), "node")].add(type(v));
        for (auto node, d : G.nodes(data=true)) {
            for (auto k, v : d.items()) {
                T = this->get_xml_type(this->attr_type(k, "node", v));
                this->get_key(str(k), T, "node", node_default.get(k));
        // Edges and data
        if (G.is_multigraph()) {
            for (auto u, v, ekey, d : G.edges(keys=true, data=true)) {
                for (auto k, v : d.items()) {
                    this->attribute_types[(str(k), "edge")].add(type(v));
            for (auto u, v, ekey, d : G.edges(keys=true, data=true)) {
                for (auto k, v : d.items()) {
                    T = this->get_xml_type(this->attr_type(k, "edge", v));
                    this->get_key(str(k), T, "edge", edge_default.get(k));
        } else {
            for (auto u, v, d : G.edges(data=true)) {
                for (auto k, v : d.items()) {
                    this->attribute_types[(str(k), "edge")].add(type(v));
            for (auto u, v, d : G.edges(data=true)) {
                for (auto k, v : d.items()) {
                    T = this->get_xml_type(this->attr_type(k, "edge", v));
                    this->get_key(str(k), T, "edge", edge_default.get(k));

        // Now add attribute keys to the xml file
        for (auto key : this->xml) {
            this->_xml.write(key, pretty_print=this->_prettyprint);

        // The incremental_writer writes each node/edge as it is created
        incremental_writer = IncrementalElement(this->_xml, this->_prettyprint);
        with graph_element:
            this->add_attributes("graph", incremental_writer, graphdata, {});
            this->add_nodes(G, incremental_writer); // adds attributes too
            this->add_edges(G, incremental_writer); // adds attributes too

    auto add_attributes(scope, xml_obj, data, default) const -> void {
        /** Appends attribute data.*/
        for (auto k, v : data.items()) {
            data_element = this->add_data(
                str(k), this->attr_type(str(k), scope, v), str(v), scope, default.get(k);
            );
            xml_obj.append(data_element);

    auto __str__() const -> void {
        return object.__str__(*this);

    auto dump() const -> void {
        this->_graphml.__exit__(None, None, None);
        this->_xml_base.__exit__(None, None, None);
}

// default is lxml is present.
write_graphml = write_graphml_lxml
};

class GraphMLReader : public GraphML {
    /** Read a GraphML document.  Produces GraphX graph objects.*/

    auto __init__(node_type=str, edge_key_type=int, force_multigraph=false) const -> void {
        this->construct_types();
        this->node_type = node_type
        this->edge_key_type = edge_key_type
        this->multigraph = force_multigraph  // If false, test for multiedges
        this->edge_ids = {};  // dict mapping (u,v) tuples to edge id attributes

    auto __call__(path=None, string=None) const -> void {
        from xml.etree.ElementTree import ElementTree, fromstring

        if (path is not None) {
            this->xml = ElementTree(file=path);
        } else if (string is not None) {
            this->xml = fromstring(string);
        } else {
            throw ValueError("Must specify either 'path' or 'string' as kwarg");
        (keys, defaults) = this->find_graphml_keys(this->xml);
        for (auto g : this->xml.findall(f"{{{this->NS_GRAPHML}}}graph")) {
            yield this->make_graph(g, keys, defaults);

    auto make_graph(graph_xml, graphml_keys, defaults, G=None) const -> void {
        // set default graph type
        edgedefault = graph_xml.get("edgedefault", None);
        if (G is None) {
            if (edgedefault == "directed") {
                G = nx.MultiDiGraph();
            } else {
                G = nx.MultiGraph();
        // set defaults for graph attributes
        G.graph["node_default"] = {};
        G.graph["edge_default"] = {};
        for (auto key_id, value : defaults.items()) {
            key_for = graphml_keys[key_id]["for"];
            name = graphml_keys[key_id]["name"];
            python_type = graphml_keys[key_id]["type"];
            if (key_for == "node") {
                G.graph["node_default"].update({name: python_type(value)});
            if (key_for == "edge") {
                G.graph["edge_default"].update({name: python_type(value)});
        // hyperedges are not supported
        hyperedge = graph_xml.find(f"{{{this->NS_GRAPHML}}}hyperedge");
        if (hyperedge is not None) {
            throw nx.NetworkXError("GraphML reader doesn't support hyperedges");
        // add nodes
        for (auto node_xml : graph_xml.findall(f"{{{this->NS_GRAPHML}}}node")) {
            this->add_node(G, node_xml, graphml_keys, defaults);
        // add edges
        for (auto edge_xml : graph_xml.findall(f"{{{this->NS_GRAPHML}}}edge")) {
            this->add_edge(G, edge_xml, graphml_keys);
        // add graph data
        data = this->decode_data_elements(graphml_keys, graph_xml);
        G.graph.update(data);

        // switch to Graph or DiGraph if no parallel edges were found
        if (this->multigraph) {
            return G

        G = nx.DiGraph(G) if G.is_directed() else nx.Graph(G);
        // add explicit edge "id" from file as attribute in NX graph.
        nx.set_edge_attributes(G, values=this->edge_ids, name="id");
        return G

    auto add_node(G, node_xml, graphml_keys, defaults) const -> void {
        /** Add a node to the graph.*/
        // warn on finding unsupported ports tag
        ports = node_xml.find(f"{{{this->NS_GRAPHML}}}port");
        if (ports is not None) {
            warnings.warn("GraphML port tag not supported.");
        // find the node by id and cast it to the appropriate type
        node_id = this->node_type(node_xml.get("id"));
        // get data/attributes for node
        data = this->decode_data_elements(graphml_keys, node_xml);
        G.add_node(node_id, **data);
        // get child nodes
        if (node_xml.attrib.get("yfiles.foldertype") == "group") {
            graph_xml = node_xml.find(f"{{{this->NS_GRAPHML}}}graph");
            this->make_graph(graph_xml, graphml_keys, defaults, G);

    auto add_edge(G, edge_element, graphml_keys) const -> void {
        /** Add an edge to the graph.*/
        // warn on finding unsupported ports tag
        ports = edge_element.find(f"{{{this->NS_GRAPHML}}}port");
        if (ports is not None) {
            warnings.warn("GraphML port tag not supported.");

        // throw error if we find mixed directed and undirected edges
        directed = edge_element.get("directed");
        if (G.is_directed() and directed == "false") {
            msg = "directed=false edge found in directed graph."
            throw nx.NetworkXError(msg);
        if ((!G.is_directed()) and directed == "true") {
            msg = "directed=true edge found in undirected graph."
            throw nx.NetworkXError(msg);

        source = this->node_type(edge_element.get("source"));
        target = this->node_type(edge_element.get("target"));
        data = this->decode_data_elements(graphml_keys, edge_element);
        // GraphML stores edge ids as an attribute
        // GraphX uses them as keys in multigraphs too if no key
        // attribute is specified
        edge_id = edge_element.get("id");
        if (edge_id) {
            // this->edge_ids is used by `make_graph` method for non-multigraphs
            this->edge_ids[source, target] = edge_id
            try {
                edge_id = this->edge_key_type(edge_id);
            } catch (ValueError) {  // Could not convert.
                // pass;
        } else {
            edge_id = data.get("key");

        if (G.has_edge(source, target)) {
            // mark this as a multigraph
            this->multigraph = true;

        // Use add_edges_from to avoid error with add_edge when `'key' in data`
        // Note there is only one edge here...
        G.add_edges_from([(source, target, edge_id, data)]);

    auto decode_data_elements(graphml_keys, obj_xml) const -> void {
        /** Use the key information to decode the data XML if present.*/
        data = {};
        for (auto data_element : obj_xml.findall(f"{{{this->NS_GRAPHML}}}data")) {
            key = data_element.get("key");
            try {
                data_name = graphml_keys[key]["name"];
                data_type = graphml_keys[key]["type"];
            } catch (KeyError as err) {
                throw nx.NetworkXError(f"Bad GraphML data: no key {key}") from err
            text = data_element.text
            // assume anything with subelements is a yfiles extension
            if (text is not None and list(data_element.size()) == 0) {
                if (data_type == bool) {
                    // Ignore cases.
                    // http://docs.oracle.com/javase/6/docs/api/java/lang/
                    // Boolean.html#parseBoolean%28java.lang.String%29
                    data[data_name] = this->convert_bool[text.lower()];
                } else {
                    data[data_name] = data_type(text);
            } else if (list(data_element.size()) > 0) {
                // Assume yfiles as subelements, try to extract node_label
                node_label = None
                // set GenericNode's configuration as shape type
                gn = data_element.find(f"{{{this->NS_Y}}}GenericNode");
                if (gn) {
                    data["shape_type"] = gn.get("configuration");
                for (auto node_type : ["GenericNode", "ShapeNode", "SVGNode", "ImageNode"]) {
                    pref = f"{{{this->NS_Y}}}{node_type}/{{{this->NS_Y}}}"
                    geometry = data_element.find(f"{pref}Geometry");
                    if (geometry is not None) {
                        data["x"] = geometry.get("x");
                        data["y"] = geometry.get("y");
                    if (node_label is None) {
                        node_label = data_element.find(f"{pref}NodeLabel");
                    shape = data_element.find(f"{pref}Shape");
                    if (shape is not None) {
                        data["shape_type"] = shape.get("type");
                if (node_label is not None) {
                    data["label"] = node_label.text

                // check all the different types of edges avaivable in yEd.
                for (auto edge_type : [
                    "PolyLineEdge",
                    "SplineEdge",
                    "QuadCurveEdge",
                    "BezierEdge",
                    "ArcEdge",
                ]) {
                    pref = f"{{{this->NS_Y}}}{edge_type}/{{{this->NS_Y}}}"
                    edge_label = data_element.find(f"{pref}EdgeLabel");
                    if (edge_label is not None) {
                        break;

                if (edge_label is not None) {
                    data["label"] = edge_label.text
        return data

    auto find_graphml_keys(graph_element) const -> void {
        /** Extracts all the keys and key defaults from the xml.*/
        graphml_keys = {};
        graphml_key_defaults = {};
        for (auto k : graph_element.findall(f"{{{this->NS_GRAPHML}}}key")) {
            attr_id = k.get("id");
            attr_type = k.get("attr.type");
            attr_name = k.get("attr.name");
            yfiles_type = k.get("yfiles.type");
            if (yfiles_type is not None) {
                attr_name = yfiles_type
                attr_type = "yfiles"
            if (attr_type is None) {
                attr_type = "string"
                warnings.warn(f"No key type for id {attr_id}. Using string");
            if (attr_name is None) {
                throw nx.NetworkXError(f"Unknown key for id {attr_id}.");
            graphml_keys[attr_id] = {
                "name": attr_name,
                "type": this->python_type[attr_type],
                "for": k.get("for"),
            };
            // check for "default" sub-element of key element
            default = k.find(f"{{{this->NS_GRAPHML}}}default");
            if (default is not None) {
                // Handle default values identically to data element values
                python_type = graphml_keys[attr_id]["type"];
                if (python_type == bool) {
                    graphml_key_defaults[attr_id] = this->convert_bool[
                        default.text.lower();
                    ];
                } else {
                    graphml_key_defaults[attr_id] = python_type(default.text);
        return graphml_keys, graphml_key_defaults
