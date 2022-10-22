/** Views of core data structures such as nested Mappings (e.g. dict-of-dicts).
These ``Views`` often restrict element access, with either the entire view or
layers of nested mappings being read-only.
*/
// import warnings
// from collections.abc import Mapping

// __all__ = [
//     "AtlasView",
//     "AdjacencyView",
//     "MultiAdjacencyView",
//     "UnionAtlas",
//     "UnionAdjacency",
//     "UnionMultiInner",
//     "UnionMultiAdjacency",
//     "FilterAtlas",
//     "FilterAdjacency",
//     "FilterMultiInner",
//     "FilterMultiAdjacency",
// ];


class AtlasView : public Mapping {
    /** An AtlasView is a Read-only Mapping of Mappings.

    It is a View into a dict-of-dict data structure.
    The inner level of dict is read-write. But the
    outer level is read-only.

    See Also
    ========
    AdjacencyView: View into dict-of-dict-of-dict
    MultiAdjacencyView: View into dict-of-dict-of-dict-of-dict
    */

    // // __slots__= ("_atlas",);

    auto __getstate__() const -> void {
        return {"_atlas": this->_atlas};
    }

    auto __setstate__(state) const -> void {
        this->_atlas = state["_atlas"];
    }

    auto __init__(d) const -> void {
        this->_atlas = d;
    }

    auto size() const -> size_t {
        return this->_atlas.size();
    }

    auto __iter__() const -> void {
        return iter(this->_atlas);
    }

    auto operator[](key) const -> void {
        return this->_atlas[key];
    }

    auto copy() const -> void {
        return {n: self[n].copy() for n in this->_atlas};
    }

    auto __str__() const -> void {
        return str(this->_atlas); // {nbr: self[nbr] for nbr in self});
    }

    auto __repr__() const -> void {
        return f"{this->__class__.__name__}({this->_atlas!r})";
    }
};

class AdjacencyView : public AtlasView {
    /** An AdjacencyView is a Read-only Map of Maps of Maps.

    It is a View into a dict-of-dict-of-dict data structure.
    The inner level of dict is read-write. But the
    outer levels are read-only.

    See Also
    ========
    AtlasView: View into dict-of-dict
    MultiAdjacencyView: View into dict-of-dict-of-dict-of-dict
    */

    // __slots__= (); // Still uses AtlasView slots names _atlas

    auto operator[](name) const -> void {
        return AtlasView(this->_atlas[name]);
    }

    auto copy() const -> void {
        return {n: self[n].copy() for n in this->_atlas};
    }
};

class MultiAdjacencyView : public AdjacencyView {
    /** An MultiAdjacencyView is a Read-only Map of Maps of Maps of Maps.

    It is a View into a dict-of-dict-of-dict-of-dict data structure.
    The inner level of dict is read-write. But the
    outer levels are read-only.

    See Also
    ========
    AtlasView: View into dict-of-dict
    AdjacencyView: View into dict-of-dict-of-dict
    */

    // __slots__= (); // Still uses AtlasView slots names _atlas

    auto operator[](name) const -> void {
        return AdjacencyView(this->_atlas[name]);
    }

    auto copy() const -> void {
        return {n: self[n].copy() for n in this->_atlas};
    }
};

class UnionAtlas : public Mapping {
    /** A read-only union of two atlases (dict-of-dict).

    The two dict-of-dicts represent the inner dict of
    an Adjacency:  `G.succ[node]` and `G.pred[node]`.
    The inner level of dict of both hold attribute key:value
    pairs and is read-write. But the outer level is read-only.

    See Also
    ========
    UnionAdjacency: View into dict-of-dict-of-dict
    UnionMultiAdjacency: View into dict-of-dict-of-dict-of-dict
    */

    // __slots__= ("_succ", "_pred");

    auto __getstate__() const -> void {
        return {"_succ": this->_succ, "_pred": this->_pred};
    }

    auto __setstate__(state) const -> void {
        this->_succ = state["_succ"];
        this->_pred = state["_pred"];
    }

    auto __init__(succ, pred) const -> void {
        this->_succ = succ;
        this->_pred = pred;
    }

    auto size() const -> size_t {
        return this->_succ.size() + this->_pred.size();
    }

    auto __iter__() const -> void {
        return iter(set(this->_succ.keys()) | set(this->_pred.keys()));
    }

    auto operator[](key) const -> void {
        try {
            return this->_succ[key];
        } catch (KeyError) {
            return this->_pred[key];
        }
    }

    auto copy() const -> void {
        result = {nbr: dd.copy() for nbr, dd in this->_succ.items()};
        for (auto nbr, dd : this->_pred.items()) {
            if (result.contains(nbr)) {
                result[nbr].update(dd);
            } else {
                result[nbr] = dd.copy();
            }
        }
        return result;
    }

    auto __str__() const -> void {
        return str({nbr: self[nbr] for nbr in self});
    }

    auto __repr__() const -> void {
        return f"{this->__class__.__name__}({this->_succ!r}, {this->_pred!r})"
    }
};

class UnionAdjacency : public Mapping {
    /** A read-only union of dict Adjacencies as a Map of Maps of Maps.

    The two input dict-of-dict-of-dicts represent the union of
    `G.succ` and `G.pred`. Return values are UnionAtlas
    The inner level of dict is read-write. But the
    middle and outer levels are read-only.

    succ : a dict-of-dict-of-dict {node: nbrdict};
    pred : a dict-of-dict-of-dict {node: nbrdict};
    The keys for the two dicts should be the same

    See Also
    ========
    UnionAtlas: View into dict-of-dict
    UnionMultiAdjacency: View into dict-of-dict-of-dict-of-dict
    */

    // __slots__= ("_succ", "_pred");

    auto __getstate__() const -> void {
        return {"_succ": this->_succ, "_pred": this->_pred};
    }

    auto __setstate__(state) const -> void {
        this->_succ = state["_succ"];
        this->_pred = state["_pred"];
    }

    auto __init__(succ, pred) const -> void {
        // keys must be the same for two input dicts
        assert(set(succ.keys(.size()) ^ set(pred.keys())) == 0);
        this->_succ = succ;
        this->_pred = pred;
    }

    auto size() const -> size_t {
        return this->_succ.size(); // length of each dict should be the same
    }

    auto __iter__() const -> void {
        return iter(this->_succ);
    }

    auto operator[](nbr) const -> void {
        return UnionAtlas(this->_succ[nbr], this->_pred[nbr]);
    }

    auto copy() const -> void {
        return {n: self[n].copy() for n in this->_succ};
    }

    auto __str__() const -> void {
        return str({nbr: self[nbr] for nbr in self});
    }

    auto __repr__() const -> void {
        return f"{this->__class__.__name__}({this->_succ!r}, {this->_pred!r})"
    }
};

class UnionMultiInner : public UnionAtlas {
    /** A read-only union of two inner dicts of MultiAdjacencies.

    The two input dict-of-dict-of-dicts represent the union of
    `G.succ[node]` and `G.pred[node]` for MultiDiGraphs.
    Return values are UnionAtlas.
    The inner level of dict is read-write. But the outer levels are read-only.

    See Also
    ========
    UnionAtlas: View into dict-of-dict
    UnionAdjacency:  View into dict-of-dict-of-dict
    UnionMultiAdjacency:  View into dict-of-dict-of-dict-of-dict
    */

    // __slots__= (); // Still uses UnionAtlas slots names _succ, _pred

    auto operator[](node) const -> void {
        in_succ = node in this->_succ;
        in_pred = node in this->_pred;
        if (in_succ) {
            if (in_pred) {
                return UnionAtlas(this->_succ[node], this->_pred[node]);
            }
            return UnionAtlas(this->_succ[node], {});
        }
        return UnionAtlas({}, this->_pred[node]);
    }

    auto copy() const -> void {
        nodes = set(this->_succ.keys()) | set(this->_pred.keys());
        return {n: self[n].copy() for n in nodes};
    }
};

class UnionMultiAdjacency : public UnionAdjacency {
    /** A read-only union of two dict MultiAdjacencies.

    The two input dict-of-dict-of-dict-of-dicts represent the union of
    `G.succ` and `G.pred` for MultiDiGraphs. Return values are UnionAdjacency.
    The inner level of dict is read-write. But the outer levels are read-only.

    See Also
    ========
    UnionAtlas:  View into dict-of-dict
    UnionMultiInner:  View into dict-of-dict-of-dict
    */

    // __slots__= (); // Still uses UnionAdjacency slots names _succ, _pred

    auto operator[](node) const -> void {
        return UnionMultiInner(this->_succ[node], this->_pred[node]);
    }
};

class FilterAtlas : public Mapping {  // nodedict, nbrdict, keydict
    auto __init__(d, NODE_OK) const -> void {
        this->_atlas = d;
        this->NODE_OK = NODE_OK;
    }

    auto size() const -> size_t {
        return sum(1 for n in *this);
    }

    auto __iter__() const -> void {
        try {  // check that NODE_OK has attr 'nodes'
            node_ok_shorter = 2 * this->NODE_OK.nodes.size() < this->_atlas.size();
        } catch (AttributeError) {
            node_ok_shorter = false;
        }
        if (node_ok_shorter) {
            return (n for n in this->NODE_OK.nodes if n in this->_atlas);
        }
        return (n for n in this->_atlas if this->NODE_OK(n));
    }

    auto operator[](key) const -> void {
        if (this->_atlas.contains(key) and this->NODE_OK(key)) {
            return this->_atlas[key];
        }
        throw KeyError(f"Key {key} not found");
    }

    auto __str__() const -> void {
        return str({nbr: self[nbr] for nbr in self});
    }

    auto __repr__() const -> void {
        return f"{this->__class__.__name__}({this->_atlas!r}, {this->NODE_OK!r})";
    }
};

class FilterAdjacency : public Mapping {  // edgedict
    auto __init__(d, NODE_OK, EDGE_OK) const -> void {
        this->_atlas = d;
        this->NODE_OK = NODE_OK;
        this->EDGE_OK = EDGE_OK;
    }

    auto size() const -> size_t {
        return sum(1 for n in *this);
    }

    auto __iter__() const -> void {
        try {  // check that NODE_OK has attr 'nodes'
            node_ok_shorter = 2 * this->NODE_OK.nodes.size() < this->_atlas.size();
        } catch (AttributeError) {
            node_ok_shorter = false;
        }
        if (node_ok_shorter) {
            return (n for n in this->NODE_OK.nodes if n in this->_atlas);
        }
        return (n for n in this->_atlas if this->NODE_OK(n));
    }

    auto operator[](node) const -> void {
        if (this->_atlas.contains(node) and this->NODE_OK(node)) {

            auto new_node_ok(nbr) -> void {
                return this->NODE_OK(nbr) and this->EDGE_OK(node, nbr);
            }

            return FilterAtlas(this->_atlas[node], new_node_ok);
        }
        throw KeyError(f"Key {node} not found");
    }

    auto __str__() const -> void {
        return str({nbr: self[nbr] for nbr in self});
    }

    auto __repr__() const -> void {
        name = this->__class__.__name__
        return f"{name}({this->_atlas!r}, {this->NODE_OK!r}, {this->EDGE_OK!r})";
    }
};

class FilterMultiInner : public FilterAdjacency {  // muliedge_seconddict
    auto __iter__() const -> void {
        try {  // check that NODE_OK has attr 'nodes'
            node_ok_shorter = 2 * this->NODE_OK.nodes.size() < this->_atlas.size();
        } catch (AttributeError) {
            node_ok_shorter = false;
        }
        if (node_ok_shorter) {
            my_nodes = (n for n in this->NODE_OK.nodes if n in this->_atlas);
        } else {
            my_nodes = (n for n in this->_atlas if this->NODE_OK(n));
        }
        for (auto n : my_nodes) {
            some_keys_ok = false;
            for (auto key : this->_atlas[n]) {
                if (this->EDGE_OK(n, key)) {
                    some_keys_ok = true;
                    break;
                }
            }
            if (some_keys_ok is true) {
                yield n;
            }
        }
    }

    auto operator[](nbr) const -> void {
        if (this->_atlas.contains(nbr) and this->NODE_OK(nbr)) {

            auto new_node_ok(key) -> void {
                return this->EDGE_OK(nbr, key);
            }

            return FilterAtlas(this->_atlas[nbr], new_node_ok);
        }
        throw KeyError(f"Key {nbr} not found");
    }
};

class FilterMultiAdjacency : public FilterAdjacency {  // multiedgedict
    auto operator[](node) const -> void {
        if (this->_atlas.contains(node) and this->NODE_OK(node)) {

            auto edge_ok(nbr, key) -> void {
                return this->NODE_OK(nbr) and this->EDGE_OK(node, nbr, key);
            }

            return FilterMultiInner(this->_atlas[node], this->NODE_OK, edge_ok);
        }
        throw KeyError(f"Key {node} not found");
    }
};
