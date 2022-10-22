// import graphx as nx
#include <graphx/algorithms.hpp>  // import isomorphism as iso


class TestIsomorph {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.G1 = nx.Graph();
        cls.G2 = nx.Graph();
        cls.G3 = nx.Graph();
        cls.G4 = nx.Graph();
        cls.G5 = nx.Graph();
        cls.G6 = nx.Graph();
        cls.G1.add_edges_from([ [1, 2], [1, 3], [1, 5], [2, 3]]);
        cls.G2.add_edges_from([ [10, 20], [20, 30], [10, 30], [10, 50]]);
        cls.G3.add_edges_from([ [1, 2], [1, 3], [1, 5], [2, 5]]);
        cls.G4.add_edges_from([ [1, 2], [1, 3], [1, 5], [2, 4]]);
        cls.G5.add_edges_from([ [1, 2], [1, 3]]);
        cls.G6.add_edges_from([ [10, 20], [20, 30], [10, 30], [10, 50], [20, 50]]);

    auto test_could_be_isomorphic() const -> void {
        assert(iso.could_be_isomorphic(this->G1, this->G2));
        assert(iso.could_be_isomorphic(this->G1, this->G3));
        assert(!iso.could_be_isomorphic(this->G1, this->G4));
        assert(iso.could_be_isomorphic(this->G3, this->G2));
        assert(!iso.could_be_isomorphic(this->G1, this->G6));

    auto test_fast_could_be_isomorphic() const -> void {
        assert(iso.fast_could_be_isomorphic(this->G3, this->G2));
        assert(!iso.fast_could_be_isomorphic(this->G3, this->G5));
        assert(!iso.fast_could_be_isomorphic(this->G1, this->G6));

    auto test_faster_could_be_isomorphic() const -> void {
        assert(iso.faster_could_be_isomorphic(this->G3, this->G2));
        assert(!iso.faster_could_be_isomorphic(this->G3, this->G5));
        assert(!iso.faster_could_be_isomorphic(this->G1, this->G6));

    auto test_is_isomorphic() const -> void {
        assert(iso.is_isomorphic(this->G1, this->G2));
        assert(!iso.is_isomorphic(this->G1, this->G4));
