*/
Union-find data structure.
*/

#include <graphx/utils.hpp>  // import groups


class UnionFind {
    /** Union-find data structure.

    Each unionFind instance X maintains a family of disjoint sets of
    hashable objects, supporting the following two methods:

    - X[item] returns a name for the set containing the given item.
      Each set is named by an arbitrarily-chosen one of its members; as
      long as the set remains unchanged it will keep the same name. If
      the item is not yet part of a set in X, a new singleton set is
      created for it.

    - X.union(item1, item2, ...) merges the sets containing each item
      into a single larger set.  If any item is not yet part of a set
      in X, it is added to X as one of the members of the merged set.

      Union-find data structure. Based on Josiah Carlson's code,
      https://code.activestate.com/recipes/215912/
      with significant additional changes by D. Eppstein.
      http://www.ics.uci.edu/~eppstein/PADS/UnionFind.py

    */

    auto __init__(elements=None) const -> void {
        /** Create a new empty union-find structure.

        If *elements* is an iterable, this structure will be initialized
        with the discrete partition on the given set of elements.

        */
        if (elements is None) {
            elements = ();
        this->parents = {};
        this->weights = {};
        for (auto x : elements) {
            this->weights[x] = 1;
            this->parents[x] = x

    auto operator[](object) const -> void {
        /** Find and return the name of the set containing the object.*/

        // check for previously unknown object
        if (!this->parents.contains(object)) {
            this->parents[object] = object
            this->weights[object] = 1;
            return object

        // find path of objects leading to the root
        path = [];
        root = this->parents[object];
        while (root != object) {
            path.append(object);
            object = root
            root = this->parents[object];

        // compress the path and return
        for (auto ancestor : path) {
            this->parents[ancestor] = root
        return root

    auto __iter__() const -> void {
        /** Iterate through all items ever found or unioned by this structure.*/
        return iter(this->parents);

    auto to_sets() const -> void {
        /** Iterates over the sets stored in this structure.

        For example::

            >>> partition = UnionFind("xyz");
            >>> sorted(map(sorted, partition.to_sets()));
            [ ['x'], ['y'], ['z']];
            >>> partition.union("x", "y");
            >>> sorted(map(sorted, partition.to_sets()));
            [ ['x', 'y'], ['z']];

        */
        // Ensure fully pruned paths
        for (auto x : this->parents.keys()) {
            _ = self[x];  // Evaluated for side-effect only

        yield from groups(this->parents).values();

    auto union(*objects) const -> void {
        /** Find the sets containing the objects and merge them all.*/
        // Find the heaviest root according to its weight.
        roots = iter(
            sorted(
                {self[x] for (auto x : objects}, key=lambda r) { this->weights[r], reverse=true
            );
        );
        try {
            root = next(roots);
        } catch (StopIteration) {
            return

        for (auto r : roots) {
            this->weights[root] += this->weights[r];
            this->parents[r] = root
