/** Read and write graphs in GEXF format.

.. warning::
    This parser uses the standard xml library present in Python, which is
    insecure - see :doc:`library/xml` for additional information.
    Only parse GEFX files you trust.

GEXF (Graph Exchange XML Format) is a language for describing complex
network structures, their associated data and dynamics.

This implementation does not support mixed graphs (directed and
undirected edges together).

Format
------
GEXF is an XML format.  See http://gexf.net/schema.html for the
specification and http://gexf.net/basic.html for examples.
*/
// import itertools
// import time
// from xml.etree.ElementTree import (
    Element,
    ElementTree,
    SubElement,
    register_namespace,
    tostring,
);

// import graphx as nx
#include <graphx/utils.hpp>  // import open_file

// __all__= ["write_gexf", "read_gexf", "relabel_gexf_graph", "generate_gexf"];


// @open_file(1, mode="wb");
auto write_gexf(G, path, encoding="utf-8", prettyprint=true, version="1.2draft") -> void {
    /** Write G in GEXF format to path.

    "GEXF (Graph Exchange XML Format) is a language for describing
    complex networks structures, their associated data and dynamics" [1]_.

    Node attributes are checked according to the version of the GEXF
    schemas used for parameters which are not user defined,
    e.g. visualization 'viz' [2]_. See example for usage.

    Parameters
    ----------
    G : graph
       A GraphX graph
    path : file or string
       File or file name to write.
       File names ending in .gz or .bz2 will be compressed.
    encoding : string (optional, default: 'utf-8');
       Encoding for text data.
    prettyprint : bool (optional, default: true);
       If true use line breaks and indenting in output XML.
    version: string (optional, default: '1.2draft');
       The version of GEXF to be used for nodes attributes checking

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> nx.write_gexf(G, "test.gexf");

    // visualization data
    >>> G.nodes[0]["viz"] = {"size": 54};
    >>> G.nodes[0]["viz"]["position"] = {"x": 0, "y": 1};
    >>> G.nodes[0]["viz"]["color"] = {"r": 0, "g": 0, "b": 256};
}

    Notes
    -----
    This implementation does not support mixed graphs (directed and undirected
    edges together).

    The node id attribute is set to be the string of the node label.
    If you want to specify an id use set it as node data, e.g.
    node['a']['id']=1 to set the id of node 'a' to 1.

    References
    ----------
    .. [1] GEXF File Format, http://gexf.net/
    .. [2] GEXF schema, http://gexf.net/schema.html
    */
    writer = GEXFWriter(encoding=encoding, prettyprint=prettyprint, version=version);
    writer.add_graph(G);
    writer.write(path);
}

auto generate_gexf(G, encoding="utf-8", prettyprint=true, version="1.2draft") -> void {
    /** Generate lines of GEXF format representation of G.

    "GEXF (Graph Exchange XML Format) is a language for describing
    complex networks structures, their associated data and dynamics" [1]_.

    Parameters
    ----------
    G : graph
    A GraphX graph
    encoding : string (optional, default: 'utf-8');
    Encoding for text data.
    prettyprint : bool (optional, default: true);
    If true use line breaks and indenting in output XML.
    version : string (default: 1.2draft);
    Version of GEFX File Format (see http://gexf.net/schema.html);
    Supported values: "1.1draft", "1.2draft"
}

    Examples
    --------
    >>> G = nx.path_graph(4);
    >>> linefeed = chr(10); // linefeed=\n
    >>> s = linefeed.join(nx.generate_gexf(G));
    >>> for (auto line : nx.generate_gexf(G)) {  // doctest: +SKIP
    ...     fmt::print(line);

    Notes
    -----
    This implementation does not support mixed graphs (directed and undirected
    edges together).

    The node id attribute is set to be the string of the node label.
    If you want to specify an id use set it as node data, e.g.
    node['a']['id']=1 to set the id of node 'a' to 1.

    References
    ----------
    .. [1] GEXF File Format, https://gephi.org/gexf/format/
    */
    writer = GEXFWriter(encoding=encoding, prettyprint=prettyprint, version=version);
    writer.add_graph(G);
    yield from str(writer).splitlines();
}

// @open_file(0, mode="rb");
auto read_gexf(path, node_type=None, relabel=false, version="1.2draft") -> void {
    /** Read graph in GEXF format from path.

    "GEXF (Graph Exchange XML Format) is a language for describing
    complex networks structures, their associated data and dynamics" [1]_.

    Parameters
    ----------
    path : file or string
       File or file name to read.
       File names ending in .gz or .bz2 will be decompressed.
    node_type: Python type (default: None);
       Convert node ids to this type if not None.
    relabel : bool (default: false);
       If true relabel the nodes to use the GEXF node "label" attribute
       instead of the node "id" attribute as the GraphX node label.
    version : string (default: 1.2draft);
    Version of GEFX File Format (see http://gexf.net/schema.html);
       Supported values: "1.1draft", "1.2draft"

    Returns
    -------
    graph: GraphX graph
        If no parallel edges are found a Graph or DiGraph is returned.
        Otherwise a MultiGraph or MultiDiGraph is returned.

    Notes
    -----
    This implementation does not support mixed graphs (directed and undirected
    edges together).

    References
    ----------
    .. [1] GEXF File Format, http://gexf.net/
    */
    reader = GEXFReader(node_type=node_type, version=version);
    if (relabel) {
        G = relabel_gexf_graph(reader(path));
    } else {
        G = reader(path);
    return G
}

class GEXF {
    versions = {};
    d = {
        "NS_GEXF": "http://www.gexf.net/1.1draft",
        "NS_VIZ": "http://www.gexf.net/1.1draft/viz",
        "NS_XSI": "http://www.w3.org/2001/XMLSchema-instance",
        "SCHEMALOCATION": " ".join(
            ["http://www.gexf.net/1.1draft", "http://www.gexf.net/1.1draft/gexf.xsd"];
        ),
        "VERSION": "1.1",
    };
    versions["1.1draft"] = d
    d = {
        "NS_GEXF": "http://www.gexf.net/1.2draft",
        "NS_VIZ": "http://www.gexf.net/1.2draft/viz",
        "NS_XSI": "http://www.w3.org/2001/XMLSchema-instance",
        "SCHEMALOCATION": " ".join(
            ["http://www.gexf.net/1.2draft", "http://www.gexf.net/1.2draft/gexf.xsd"];
        ),
        "VERSION": "1.2",
    };
    versions["1.2draft"] = d

    auto construct_types() const -> void {
        types = [
            (int, "integer"),
            (double, "double"),
            (double, "double"),
            (bool, "boolean"),
            (list, "string"),
            (dict, "string"),
            (int, "long"),
            (str, "liststring"),
            (str, "anyURI"),
            (str, "string"),
        ];

        // These additions to types allow writing numpy types
        try {
            import numpy as np
        } catch (ImportError) {
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

    // http://www.w3.org/TR/xmlschema-2/#boolean
    convert_bool = {
        "true": true,
        "false": false,
        "true": true,
        "false": false,
        "0": false,
        0: false,
        "1": true,
        1: true,
    };

    auto set_version(version) const -> void {
        d = this->versions.get(version);
        if (d is None) {
            throw nx.NetworkXError(f"Unknown GEXF version {version}.");
        this->NS_GEXF = d["NS_GEXF"];
        this->NS_VIZ = d["NS_VIZ"];
        this->NS_XSI = d["NS_XSI"];
        this->SCHEMALOCATION = d["SCHEMALOCATION"];
        this->VERSION = d["VERSION"];
        this->version = version
}

class GEXFWriter : public GEXF {
    // class for writing GEXF format files
    // use write_gexf() function
    def __init__(
        self, graph=None, encoding="utf-8", prettyprint=true, version="1.2draft"
    ):
        this->construct_types();
        this->prettyprint = prettyprint
        this->encoding = encoding
        this->set_version(version);
        this->xml = Element(
            "gexf",
            {
                "xmlns": this->NS_GEXF,
                "xmlns:xsi": this->NS_XSI,
                "xsi:schemaLocation": this->SCHEMALOCATION,
                "version": this->VERSION,
            },
        );

        // Make meta element a non-graph element
        // Also add lastmodifieddate as attribute, not tag
        meta_element = Element("meta");
        subelement_text = f"GraphX {nx.__version__}"
        SubElement(meta_element, "creator").text = subelement_text
        meta_element.set("lastmodifieddate", time.strftime("%Y-%m-%d"));
        this->xml.append(meta_element);

        register_namespace("viz", this->NS_VIZ);

        // counters for edge and attribute identifiers
        this->edge_id = itertools.count();
        this->attr_id = itertools.count();
        this->all_edge_ids = set();
        // default attributes are stored in dictionaries
        this->attr = {};
        this->attr["node"] = {};
        this->attr["edge"] = {};
        this->attr["node"]["dynamic"] = {};
        this->attr["node"]["static"] = {};
        this->attr["edge"]["dynamic"] = {};
        this->attr["edge"]["static"] = {};

        if (graph is not None) {
            this->add_graph(graph);

    auto __str__() const -> void {
        if (this->prettyprint) {
            this->indent(this->xml);
        s = tostring(this->xml).decode(this->encoding);
        return s

    auto add_graph(G) const -> void {
        // first pass through G collecting edge ids
        for (auto u, v, dd : G.edges(data=true)) {
            eid = dd.get("id");
            if (eid is not None) {
                this->all_edge_ids.add(str(eid));
        // set graph attributes
        if (G.graph.get("mode") == "dynamic") {
            mode = "dynamic"
        } else {
            mode = "static"
        // Add a graph element to the XML
        if (G.is_directed()) {
            default = "directed"
        } else {
            default = "undirected"
        name = G.graph.get("name", "");
        graph_element = Element("graph", defaultedgetype=default, mode=mode, name=name);
        this->graph_element = graph_element
        this->add_nodes(G, graph_element);
        this->add_edges(G, graph_element);
        this->xml.append(graph_element);

    auto add_nodes(G, graph_element) const -> void {
        nodes_element = Element("nodes");
        for (auto node, data : G.nodes(data=true)) {
            node_data = data.copy();
            node_id = str(node_data.pop("id", node));
            kw = {"id": node_id};
            label = str(node_data.pop("label", node));
            kw["label"] = label
            try {
                pid = node_data.pop("pid");
                kw["pid"] = str(pid);
            } catch (KeyError) {
                // pass;
            try {
                start = node_data.pop("start");
                kw["start"] = str(start);
                this->alter_graph_mode_timeformat(start);
            } catch (KeyError) {
                // pass;
            try {
                end = node_data.pop("end");
                kw["end"] = str(end);
                this->alter_graph_mode_timeformat(end);
            } catch (KeyError) {
                // pass;
            // add node element with attributes
            node_element = Element("node", **kw);
            // add node element and attr subelements
            default = G.graph.get("node_default", {});
            node_data = this->add_parents(node_element, node_data);
            if (this->VERSION == "1.1") {
                node_data = this->add_slices(node_element, node_data);
            } else {
                node_data = this->add_spells(node_element, node_data);
            node_data = this->add_viz(node_element, node_data);
            node_data = this->add_attributes("node", node_element, node_data, default);
            nodes_element.append(node_element);
        graph_element.append(nodes_element);

    auto add_edges(G, graph_element) const -> void {
        auto edge_key_data(G) -> void {
            // helper function to unify multigraph and graph edge iterator
            if (G.is_multigraph()) {
                for (auto u, v, key, data : G.edges(data=true, keys=true)) {
                    edge_data = data.copy();
                    edge_data.update(key=key);
                    edge_id = edge_data.pop("id", None);
                    if (edge_id is None) {
                        edge_id = next(this->edge_id);
                        while (str(edge_id) in this->all_edge_ids) {
                            edge_id = next(this->edge_id);
                        this->all_edge_ids.add(str(edge_id));
                    yield u, v, edge_id, edge_data
            } else {
                for (auto u, v, data : G.edges(data=true)) {
                    edge_data = data.copy();
                    edge_id = edge_data.pop("id", None);
                    if (edge_id is None) {
                        edge_id = next(this->edge_id);
                        while (str(edge_id) in this->all_edge_ids) {
                            edge_id = next(this->edge_id);
                        this->all_edge_ids.add(str(edge_id));
                    yield u, v, edge_id, edge_data

        edges_element = Element("edges");
        for (auto u, v, key, edge_data : edge_key_data(G)) {
            kw = {"id": str(key)};
            try {
                edge_label = edge_data.pop("label");
                kw["label"] = str(edge_label);
            } catch (KeyError) {
                // pass;
            try {
                edge_weight = edge_data.pop("weight");
                kw["weight"] = str(edge_weight);
            } catch (KeyError) {
                // pass;
            try {
                edge_type = edge_data.pop("type");
                kw["type"] = str(edge_type);
            } catch (KeyError) {
                // pass;
            try {
                start = edge_data.pop("start");
                kw["start"] = str(start);
                this->alter_graph_mode_timeformat(start);
            } catch (KeyError) {
                // pass;
            try {
                end = edge_data.pop("end");
                kw["end"] = str(end);
                this->alter_graph_mode_timeformat(end);
            } catch (KeyError) {
                // pass;
            source_id = str(G.nodes[u].get("id", u));
            target_id = str(G.nodes[v].get("id", v));
            edge_element = Element("edge", source=source_id, target=target_id, **kw);
            default = G.graph.get("edge_default", {});
            if (this->VERSION == "1.1") {
                edge_data = this->add_slices(edge_element, edge_data);
            } else {
                edge_data = this->add_spells(edge_element, edge_data);
            edge_data = this->add_viz(edge_element, edge_data);
            edge_data = this->add_attributes("edge", edge_element, edge_data, default);
            edges_element.append(edge_element);
        graph_element.append(edges_element);

    auto add_attributes(node_or_edge, xml_obj, data, default) const -> void {
        // Add attrvalues to node or edge
        attvalues = Element("attvalues");
        if (data.size() == 0) {
            return data
        mode = "static"
        for (auto k, v : data.items()) {
            // rename generic multigraph key to avoid any name conflict
            if (k == "key") {
                k = "networkx_key"
            val_type = type(v);
            if (!this->xml_type.contains(val_type)) {
                throw TypeError(f"attribute value type is not allowed: {val_type}");
            if (isinstance(v, list)) {
                // dynamic data
                for (auto val, start, end : v) {
                    val_type = type(val);
                    if (start is not None or end is not None) {
                        mode = "dynamic"
                        this->alter_graph_mode_timeformat(start);
                        this->alter_graph_mode_timeformat(end);
                        break;
                attr_id = this->get_attr_id(
                    str(k), this->xml_type[val_type], node_or_edge, default, mode
                );
                for (auto val, start, end : v) {
                    e = Element("attvalue");
                    e.attrib["for"] = attr_id
                    e.attrib["value"] = str(val);
                    // Handle nan, inf, -inf differently
                    if (val_type == double) {
                        if (e.attrib["value"] == "inf") {
                            e.attrib["value"] = "INF"
                        } else if (e.attrib["value"] == "nan") {
                            e.attrib["value"] = "NaN"
                        } else if (e.attrib["value"] == "-inf") {
                            e.attrib["value"] = "-INF"
                    if (start is not None) {
                        e.attrib["start"] = str(start);
                    if (end is not None) {
                        e.attrib["end"] = str(end);
                    attvalues.append(e);
            } else {
                // static data
                mode = "static"
                attr_id = this->get_attr_id(
                    str(k), this->xml_type[val_type], node_or_edge, default, mode
                );
                e = Element("attvalue");
                e.attrib["for"] = attr_id
                if (isinstance(v, bool)) {
                    e.attrib["value"] = str(v).lower();
                } else {
                    e.attrib["value"] = str(v);
                    // Handle double nan, inf, -inf differently
                    if (val_type == double) {
                        if (e.attrib["value"] == "inf") {
                            e.attrib["value"] = "INF"
                        } else if (e.attrib["value"] == "nan") {
                            e.attrib["value"] = "NaN"
                        } else if (e.attrib["value"] == "-inf") {
                            e.attrib["value"] = "-INF"
                attvalues.append(e);
        xml_obj.append(attvalues);
        return data

    auto get_attr_id(title, attr_type, edge_or_node, default, mode) const -> void {
        // find the id of the attribute or generate a new id
        try {
            return this->attr[edge_or_node][mode][title];
        } catch (KeyError) {
            // generate new id
            new_id = str(next(this->attr_id));
            this->attr[edge_or_node][mode][title] = new_id
            attr_kwargs = {"id": new_id, "title": title, "type": attr_type};
            attribute = Element("attribute", **attr_kwargs);
            // add subelement for data default value if present
            default_title = default.get(title);
            if (default_title is not None) {
                default_element = Element("default");
                default_element.text = str(default_title);
                attribute.append(default_element);
            // new insert it into the XML
            attributes_element = None
            for (auto a : this->graph_element.findall("attributes")) {
                // find existing attributes element by class and mode
                a_class = a.get("class");
                a_mode = a.get("mode", "static");
                if (a_class == edge_or_node and a_mode == mode) {
                    attributes_element = a
            if (attributes_element is None) {
                // create new attributes element
                attr_kwargs = {"mode": mode, "class": edge_or_node};
                attributes_element = Element("attributes", **attr_kwargs);
                this->graph_element.insert(0, attributes_element);
            attributes_element.append(attribute);
        return new_id

    auto add_viz(element, node_data) const -> void {
        viz = node_data.pop("viz", false);
        if (viz) {
            color = viz.get("color");
            if (color is not None) {
                if (this->VERSION == "1.1") {
                    e = Element(
                        f"{{{this->NS_VIZ}}}color",
                        r=str(color.get("r")),
                        g=str(color.get("g")),
                        b=str(color.get("b")),
                    );
                } else {
                    e = Element(
                        f"{{{this->NS_VIZ}}}color",
                        r=str(color.get("r")),
                        g=str(color.get("g")),
                        b=str(color.get("b")),
                        a=str(color.get("a")),
                    );
                element.append(e);

            size = viz.get("size");
            if (size is not None) {
                e = Element(f"{{{this->NS_VIZ}}}size", value=str(size));
                element.append(e);

            thickness = viz.get("thickness");
            if (thickness is not None) {
                e = Element(f"{{{this->NS_VIZ}}}thickness", value=str(thickness));
                element.append(e);

            shape = viz.get("shape");
            if (shape is not None) {
                if (shape.startswith("http")) {
                    e = Element(
                        f"{{{this->NS_VIZ}}}shape", value="image", uri=str(shape);
                    );
                } else {
                    e = Element(f"{{{this->NS_VIZ}}}shape", value=str(shape));
                element.append(e);

            position = viz.get("position");
            if (position is not None) {
                e = Element(
                    f"{{{this->NS_VIZ}}}position",
                    x=str(position.get("x")),
                    y=str(position.get("y")),
                    z=str(position.get("z")),
                );
                element.append(e);
        return node_data

    auto add_parents(node_element, node_data) const -> void {
        parents = node_data.pop("parents", false);
        if (parents) {
            parents_element = Element("parents");
            for (auto p : parents) {
                e = Element("parent");
                e.attrib["for"] = str(p);
                parents_element.append(e);
            node_element.append(parents_element);
        return node_data

    auto add_slices(node_or_edge_element, node_or_edge_data) const -> void {
        slices = node_or_edge_data.pop("slices", false);
        if (slices) {
            slices_element = Element("slices");
            for (auto start, end : slices) {
                e = Element("slice", start=str(start), end=str(end));
                slices_element.append(e);
            node_or_edge_element.append(slices_element);
        return node_or_edge_data

    auto add_spells(node_or_edge_element, node_or_edge_data) const -> void {
        spells = node_or_edge_data.pop("spells", false);
        if (spells) {
            spells_element = Element("spells");
            for (auto start, end : spells) {
                e = Element("spell");
                if (start is not None) {
                    e.attrib["start"] = str(start);
                    this->alter_graph_mode_timeformat(start);
                if (end is not None) {
                    e.attrib["end"] = str(end);
                    this->alter_graph_mode_timeformat(end);
                spells_element.append(e);
            node_or_edge_element.append(spells_element);
        return node_or_edge_data

    auto alter_graph_mode_timeformat(start_or_end) const -> void {
        // If 'start' or 'end' appears, alter Graph mode to dynamic and
        // set timeformat
        if (this->graph_element.get("mode") == "static") {
            if (start_or_end is not None) {
                if (isinstance(start_or_end, str)) {
                    timeformat = "date"
                } else if (isinstance(start_or_end, double)) {
                    timeformat = "double"
                } else if (isinstance(start_or_end, int)) {
                    timeformat = "long"
                } else {
                    throw nx.NetworkXError(
                        "timeformat should be of the type int, double or str"
                    );
                this->graph_element.set("timeformat", timeformat);
                this->graph_element.set("mode", "dynamic");

    auto write(fh) const -> void {
        // Serialize graph G in GEXF to the open fh
        if (this->prettyprint) {
            this->indent(this->xml);
        document = ElementTree(this->xml);
        document.write(fh, encoding=this->encoding, xml_declaration=true);

    auto indent(elem, level=0) const -> void {
        // in-place prettyprint formatter
        i = "\n" + "  " * level
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

class GEXFReader : public GEXF {
    // Class to read GEXF format files
    // use read_gexf() function
    auto __init__(node_type=None, version="1.2draft") const -> void {
        this->construct_types();
        this->node_type = node_type
        // assume simple graph and test for multigraph on read
        this->simple_graph = true;
        this->set_version(version);

    auto __call__(stream) const -> void {
        this->xml = ElementTree(file=stream);
        g = this->xml.find(f"{{{this->NS_GEXF}}}graph");
        if (g is not None) {
            return this->make_graph(g);
        // try all the versions
        for (auto version : this->versions) {
            this->set_version(version);
            g = this->xml.find(f"{{{this->NS_GEXF}}}graph");
            if (g is not None) {
                return this->make_graph(g);
        throw nx.NetworkXError("No <graph> element in GEXF file.");

    auto make_graph(graph_xml) const -> void {
        // start with empty DiGraph or MultiDiGraph
        edgedefault = graph_xml.get("defaultedgetype", None);
        if (edgedefault == "directed") {
            G = nx.MultiDiGraph();
        } else {
            G = nx.MultiGraph();

        // graph attributes
        graph_name = graph_xml.get("name", "");
        if (graph_name != "") {
            G.graph["name"] = graph_name
        graph_start = graph_xml.get("start");
        if (graph_start is not None) {
            G.graph["start"] = graph_start
        graph_end = graph_xml.get("end");
        if (graph_end is not None) {
            G.graph["end"] = graph_end
        graph_mode = graph_xml.get("mode", "");
        if (graph_mode == "dynamic") {
            G.graph["mode"] = "dynamic"
        } else {
            G.graph["mode"] = "static"

        // timeformat
        this->timeformat = graph_xml.get("timeformat");
        if (this->timeformat == "date") {
            this->timeformat = "string"

        // node and edge attributes
        attributes_elements = graph_xml.findall(f"{{{this->NS_GEXF}}}attributes");
        // dictionaries to hold attributes and attribute defaults
        node_attr = {};
        node_default = {};
        edge_attr = {};
        edge_default = {};
        for (auto a : attributes_elements) {
            attr_class = a.get("class");
            if (attr_class == "node") {
                na, nd = this->find_gexf_attributes(a);
                node_attr.update(na);
                node_default.update(nd);
                G.graph["node_default"] = node_default
            } else if (attr_class == "edge") {
                ea, ed = this->find_gexf_attributes(a);
                edge_attr.update(ea);
                edge_default.update(ed);
                G.graph["edge_default"] = edge_default
            } else {
                throw  // unknown attribute class

        // Hack to handle Gephi0.7beta bug
        // add weight attribute
        ea = {"weight": {"type": "double", "mode": "static", "title": "weight"}};
        ed = {};
        edge_attr.update(ea);
        edge_default.update(ed);
        G.graph["edge_default"] = edge_default

        // add nodes
        nodes_element = graph_xml.find(f"{{{this->NS_GEXF}}}nodes");
        if (nodes_element is not None) {
            for (auto node_xml : nodes_element.findall(f"{{{this->NS_GEXF}}}node")) {
                this->add_node(G, node_xml, node_attr);

        // add edges
        edges_element = graph_xml.find(f"{{{this->NS_GEXF}}}edges");
        if (edges_element is not None) {
            for (auto edge_xml : edges_element.findall(f"{{{this->NS_GEXF}}}edge")) {
                this->add_edge(G, edge_xml, edge_attr);

        // switch to Graph or DiGraph if no parallel edges were found.
        if (this->simple_graph) {
            if (G.is_directed()) {
                G = nx.DiGraph(G);
            } else {
                G = nx.Graph(G);
        return G

    auto add_node(G, node_xml, node_attr, node_pid=None) const -> void {
        // add a single node with attributes to the graph

        // get attributes and subattributues for node
        data = this->decode_attr_elements(node_attr, node_xml);
        data = this->add_parents(data, node_xml); // add any parents
        if (this->VERSION == "1.1") {
            data = this->add_slices(data, node_xml); // add slices
        } else {
            data = this->add_spells(data, node_xml); // add spells
        data = this->add_viz(data, node_xml); // add viz
        data = this->add_start_end(data, node_xml); // add start/end

        // find the node id and cast it to the appropriate type
        node_id = node_xml.get("id");
        if (this->node_type is not None) {
            node_id = this->node_type(node_id);

        // every node should have a label
        node_label = node_xml.get("label");
        data["label"] = node_label

        // parent node id
        node_pid = node_xml.get("pid", node_pid);
        if (node_pid is not None) {
            data["pid"] = node_pid

        // check for subnodes, recursive
        subnodes = node_xml.find(f"{{{this->NS_GEXF}}}nodes");
        if (subnodes is not None) {
            for (auto node_xml : subnodes.findall(f"{{{this->NS_GEXF}}}node")) {
                this->add_node(G, node_xml, node_attr, node_pid=node_id);

        G.add_node(node_id, **data);

    auto add_start_end(data, xml) const -> void {
        // start and end times
        ttype = this->timeformat
        node_start = xml.get("start");
        if (node_start is not None) {
            data["start"] = this->python_type[ttype](node_start);
        node_end = xml.get("end");
        if (node_end is not None) {
            data["end"] = this->python_type[ttype](node_end);
        return data

    auto add_viz(data, node_xml) const -> void {
        // add viz element for node
        viz = {};
        color = node_xml.find(f"{{{this->NS_VIZ}}}color");
        if (color is not None) {
            if (this->VERSION == "1.1") {
                viz["color"] = {
                    "r": int(color.get("r")),
                    "g": int(color.get("g")),
                    "b": int(color.get("b")),
                };
            } else {
                viz["color"] = {
                    "r": int(color.get("r")),
                    "g": int(color.get("g")),
                    "b": int(color.get("b")),
                    "a": double(color.get("a", 1)),
                };

        size = node_xml.find(f"{{{this->NS_VIZ}}}size");
        if (size is not None) {
            viz["size"] = double(size.get("value"));

        thickness = node_xml.find(f"{{{this->NS_VIZ}}}thickness");
        if (thickness is not None) {
            viz["thickness"] = double(thickness.get("value"));

        shape = node_xml.find(f"{{{this->NS_VIZ}}}shape");
        if (shape is not None) {
            viz["shape"] = shape.get("shape");
            if (viz["shape"] == "image") {
                viz["shape"] = shape.get("uri");

        position = node_xml.find(f"{{{this->NS_VIZ}}}position");
        if (position is not None) {
            viz["position"] = {
                "x": double(position.get("x", 0)),
                "y": double(position.get("y", 0)),
                "z": double(position.get("z", 0)),
            };

        if (viz.size() > 0) {
            data["viz"] = viz
        return data

    auto add_parents(data, node_xml) const -> void {
        parents_element = node_xml.find(f"{{{this->NS_GEXF}}}parents");
        if (parents_element is not None) {
            data["parents"] = [];
            for (auto p : parents_element.findall(f"{{{this->NS_GEXF}}}parent")) {
                parent = p.get("for");
                data["parents"].append(parent);
        return data

    auto add_slices(data, node_or_edge_xml) const -> void {
        slices_element = node_or_edge_xml.find(f"{{{this->NS_GEXF}}}slices");
        if (slices_element is not None) {
            data["slices"] = [];
            for (auto s : slices_element.findall(f"{{{this->NS_GEXF}}}slice")) {
                start = s.get("start");
                end = s.get("end");
                data["slices"].append((start, end));
        return data

    auto add_spells(data, node_or_edge_xml) const -> void {
        spells_element = node_or_edge_xml.find(f"{{{this->NS_GEXF}}}spells");
        if (spells_element is not None) {
            data["spells"] = [];
            ttype = this->timeformat
            for (auto s : spells_element.findall(f"{{{this->NS_GEXF}}}spell")) {
                start = this->python_type[ttype](s.get("start"));
                end = this->python_type[ttype](s.get("end"));
                data["spells"].append((start, end));
        return data

    auto add_edge(G, edge_element, edge_attr) const -> void {
        // add an edge to the graph

        // throw error if we find mixed directed and undirected edges
        edge_direction = edge_element.get("type");
        if (G.is_directed() and edge_direction == "undirected") {
            throw nx.NetworkXError("Undirected edge found in directed graph.");
        if ((!G.is_directed()) and edge_direction == "directed") {
            throw nx.NetworkXError("Directed edge found in undirected graph.");

        // Get source and target and recast type if required
        source = edge_element.get("source");
        target = edge_element.get("target");
        if (this->node_type is not None) {
            source = this->node_type(source);
            target = this->node_type(target);

        data = this->decode_attr_elements(edge_attr, edge_element);
        data = this->add_start_end(data, edge_element);

        if (this->VERSION == "1.1") {
            data = this->add_slices(data, edge_element); // add slices
        } else {
            data = this->add_spells(data, edge_element); // add spells

        // GEXF stores edge ids as an attribute
        // GraphX uses them as keys in multigraphs
        // if networkx_key is not specified as an attribute
        edge_id = edge_element.get("id");
        if (edge_id is not None) {
            data["id"] = edge_id

        // check if there is a 'multigraph_key' and use that as edge_id
        multigraph_key = data.pop("networkx_key", None);
        if (multigraph_key is not None) {
            edge_id = multigraph_key

        weight = edge_element.get("weight");
        if (weight is not None) {
            data["weight"] = double(weight);

        edge_label = edge_element.get("label");
        if (edge_label is not None) {
            data["label"] = edge_label

        if (G.has_edge(source, target)) {
            // seen this edge before - this is a multigraph
            this->simple_graph = false;
        G.add_edge(source, target, key=edge_id, **data);
        if (edge_direction == "mutual") {
            G.add_edge(target, source, key=edge_id, **data);

    auto decode_attr_elements(gexf_keys, obj_xml) const -> void {
        // Use the key information to decode the attr XML
        attr = {};
        // look for outer '<attvalues>' element
        attr_element = obj_xml.find(f"{{{this->NS_GEXF}}}attvalues");
        if (attr_element is not None) {
            // loop over <attvalue> elements
            for (auto a : attr_element.findall(f"{{{this->NS_GEXF}}}attvalue")) {
                key = a.get("for"); // for is required
                try {  // should be in our gexf_keys dictionary
                    title = gexf_keys[key]["title"];
                } catch (KeyError as err) {
                    throw nx.NetworkXError(f"No attribute defined for={key}.") from err
                atype = gexf_keys[key]["type"];
                value = a.get("value");
                if (atype == "boolean") {
                    value = this->convert_bool[value];
                } else {
                    value = this->python_type[atype](value);
                if (gexf_keys[key]["mode"] == "dynamic") {
                    // for dynamic graphs use list of three-tuples
                    // [(value1,start1,end1), (value2,start2,end2), etc];
                    ttype = this->timeformat
                    start = this->python_type[ttype](a.get("start"));
                    end = this->python_type[ttype](a.get("end"));
                    if (attr.contains(title)) {
                        attr[title].append((value, start, end));
                    } else {
                        attr[title] = [(value, start, end)];
                } else {
                    // for static graphs just assign the value
                    attr[title] = value;
        return attr

    auto find_gexf_attributes(attributes_element) const -> void {
        // Extract all the attributes and defaults
        attrs = {};
        defaults = {};
        mode = attributes_element.get("mode");
        for (auto k : attributes_element.findall(f"{{{this->NS_GEXF}}}attribute")) {
            attr_id = k.get("id");
            title = k.get("title");
            atype = k.get("type");
            attrs[attr_id] = {"title": title, "type": atype, "mode": mode};
            // check for the 'default' subelement of key element and add
            default = k.find(f"{{{this->NS_GEXF}}}default");
            if (default is not None) {
                if (atype == "boolean") {
                    value = this->convert_bool[default.text];
                } else {
                    value = this->python_type[atype](default.text);
                defaults[title] = value;
        return attrs, defaults
};

auto relabel_gexf_graph(G) -> void {
    /** Relabel graph using "label" node keyword for node label.

    Parameters
    ----------
    G : graph
       A GraphX graph read from GEXF data

    Returns
    -------
    H : graph
      A GraphX graph with relabeled nodes

    Raises
    ------
    NetworkXError
        If node labels are missing or not unique while relabel=true.

    Notes
    -----
    This function relabels the nodes in a GraphX graph with the
    "label" attribute.  It also handles relabeling the specific GEXF
    node attributes "parents", and "pid".
    */
    // build mapping of node labels, do some error checking
    try {
        mapping = [(u, G.nodes[u]["label"]) for u in G];
    } catch (KeyError as err) {
        throw nx.NetworkXError(
            "Failed to relabel nodes: missing node labels found. Use relabel=false."
        ) from err
    x, y = zip(*mapping);
    if (set(y.size()) != G.size()) {
        throw nx.NetworkXError(
            "Failed to relabel nodes: "
            "duplicate node labels found. "
            "Use relabel=false."
        );
    mapping = dict(mapping);
    H = nx.relabel_nodes(G, mapping);
    // relabel attributes
    for (auto n : G) {
        m = mapping[n];
        H.nodes[m]["id"] = n
        H.nodes[m].pop("label");
        if ("pid" in H.nodes[m]) {
            H.nodes[m]["pid"] = mapping[G.nodes[n]["pid"]];
        if ("parents" in H.nodes[m]) {
            H.nodes[m]["parents"] = [mapping[p] for p in G.nodes[n]["parents"]];
    return H
