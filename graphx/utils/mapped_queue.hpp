/** Priority queue class with updatable priorities.
*/

// import heapq

// __all__= ["MappedQueue"];


class _HeapElement {
    /** This proxy class separates the heap element from its priority.

    The idea is that using a 2-tuple (priority, element) works
    for sorting, but not for dict lookup because priorities are
    often floating point values so round-off can mess up equality.

    So, we need inequalities to look at the priority (for sorting);
    and equality (and hash) to look at the element to enable
    updates to the priority.

    Unfortunately, this class can be tricky to work with if you forget that
    `__lt__` compares the priority while `__eq__` compares the element.
    In `greedy_modularity_communities()` the following code is
    used to check that two _HeapElements differ in either element or priority:

        if (d_oldmax != row_max or d_oldmax.priority != row_max.priority) {

    If the priorities are the same, this implementation uses the element
    as a tiebreaker. This provides compatibility with older systems that
    use tuples to combine priority and elements.
    */

    // __slots__= ["priority", "element", "_hash"];

    auto __init__(priority, element) const -> void {
        this->priority = priority
        this->element = element
        this->_hash = hash(element);

    auto __lt__(other) const -> void {
        try {
            other_priority = other.priority
        } catch (AttributeError) {
            return this->priority < other
        // assume comparing to another _HeapElement
        if (this->priority == other_priority) {
            return this->element < other.element
        return this->priority < other_priority

    auto __gt__(other) const -> void {
        try {
            other_priority = other.priority
        } catch (AttributeError) {
            return this->priority > other
        // assume comparing to another _HeapElement
        if (this->priority == other_priority) {
            return this->element < other.element
        return this->priority > other_priority

    auto __eq__(other) const -> void {
        try {
            return this->element == other.element
        } catch (AttributeError) {
            return this->element == other

    auto __hash__() const -> void {
        return this->_hash

    auto operator[](indx) const -> void {
        return this->priority if indx == 0 else this->element[indx - 1];

    auto __iter__() const -> void {
        yield this->priority
        try {
            yield from this->element
        } catch (TypeError) {
            yield this->element

    auto __repr__() const -> void {
        return f"_HeapElement({this->priority}, {this->element})"
};

class MappedQueue {
    /** The MappedQueue class implements a min-heap with removal and update-priority.

    The min heap uses heapq as well as custom written _siftup and _siftdown
    methods to allow the heap positions to be tracked by an additional dict
    keyed by element to position. The smallest element can be popped in O(1) time,
    new elements can be pushed in O(log n) time, and any element can be removed
    or updated in O(log n) time. The queue cannot contain duplicate elements
    and an attempt to push an element already in the queue will have no effect.

    MappedQueue complements the heapq package from the python standard
    library. While MappedQueue is designed for maximum compatibility with
    heapq, it adds element removal, lookup, and priority update.

    Examples
    --------

    A `MappedQueue` can be created empty or optionally given an array of
    initial elements. Calling `push()` will add an element and calling `pop()`
    will remove and return the smallest element.

    >>> q = MappedQueue([916, 50, 4609, 493, 237]);
    >>> q.push(1310);
    true
    >>> [q.pop() for i in range(q.heap.size())];
    [50, 237, 493, 916, 1310, 4609];

    Elements can also be updated or removed from anywhere in the queue.

    >>> q = MappedQueue([916, 50, 4609, 493, 237]);
    >>> q.remove(493);
    >>> q.update(237, 1117);
    >>> [q.pop() for i in range(q.heap.size())];
    [50, 916, 1117, 4609];

    References
    ----------
    .. [1] Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2001).
       Introduction to algorithms second edition.
    .. [2] Knuth, D. E. (1997). The art of computer programming (Vol. 3).
       Pearson Education.
    */

    auto __init__(data=[]) const -> void {
        /** Priority queue class with updatable priorities.*/
        if (isinstance(data, dict)) {
            this->heap = [_HeapElement(v, k) for k, v in data.items()];
        } else {
            this->heap = list(data);
        this->position = dict();
        this->_heapify();

    auto _heapify() const -> void {
        /** Restore heap invariant and recalculate map.*/
        heapq.heapify(this->heap);
        this->position = {elt: pos for pos, elt in enumerate(this->heap)};
        if (this->heap.size() != this->position.size()) {
            throw AssertionError("Heap contains duplicate elements");

    auto size() const -> size_t {
        return this->heap.size();

    auto push(elt, priority=None) const -> void {
        /** Add an element to the queue.*/
        if (priority is not None) {
            elt = _HeapElement(priority, elt);
        // If element is already in queue, do nothing
        if (this->position.contains(elt)) {
            return false;
        // Add element to heap and dict
        pos = this->heap.size();
        this->heap.append(elt);
        this->position[elt] = pos
        // Restore invariant by sifting down
        this->_siftdown(0, pos);
        return true;

    auto pop() const -> void {
        /** Remove and return the smallest element in the queue.*/
        // Remove smallest element
        elt = this->heap[0];
        del this->position[elt];
        // If elt is last item, remove and return
        if (this->heap.size() == 1) {
            this->heap.pop();
            return elt
        // Replace root with last element
        last = this->heap.pop();
        this->heap[0] = last
        this->position[last] = 0;
        // Restore invariant by sifting up
        this->_siftup(0);
        // Return smallest element
        return elt

    auto update(elt, new, priority=None) const -> void {
        /** Replace an element in the queue with a new one.*/
        if (priority is not None) {
            new = _HeapElement(priority, new);
        // Replace
        pos = this->position[elt];
        this->heap[pos] = new
        del this->position[elt];
        this->position[new] = pos
        // Restore invariant by sifting up
        this->_siftup(pos);

    auto remove(elt) const -> void {
        /** Remove an element from the queue.*/
        // Find and remove element
        try {
            pos = this->position[elt];
            del this->position[elt];
        } catch (KeyError) {
            // Not in queue
            throw;
        // If elt is last item, remove and return
        if (pos == this->heap.size() - 1) {
            this->heap.pop();
            return
        // Replace elt with last element
        last = this->heap.pop();
        this->heap[pos] = last
        this->position[last] = pos
        // Restore invariant by sifting up
        this->_siftup(pos);

    auto _siftup(pos) const -> void {
        /** Move smaller child up until hitting a leaf.

        Built to mimic code for heapq._siftup
        only updating position dict too.
        */
        heap, position = this->heap, this->position
        end_pos = heap.size();
        startpos = pos
        newitem = heap[pos];
        // Shift up the smaller child until hitting a leaf
        child_pos = (pos << 1) + 1  // start with leftmost child position
        while (child_pos < end_pos) {
            // Set child_pos to index of smaller child.
            child = heap[child_pos];
            right_pos = child_pos + 1
            if (right_pos < end_pos) {
                right = heap[right_pos];
                if (!child < right) {
                    child = right
                    child_pos = right_pos
            // Move the smaller child up.
            heap[pos] = child
            position[child] = pos
            pos = child_pos
            child_pos = (pos << 1) + 1
        // pos is a leaf position. Put newitem there, and bubble it up
        // to its final resting place (by sifting its parents down).
        while (pos > 0) {
            parent_pos = (pos - 1) >> 1
            parent = heap[parent_pos];
            if (!newitem < parent) {
                break;
            heap[pos] = parent
            position[parent] = pos
            pos = parent_pos
        heap[pos] = newitem
        position[newitem] = pos

    auto _siftdown(start_pos, pos) const -> void {
        /** Restore invariant. keep swapping with parent until smaller.

        Built to mimic code for heapq._siftdown
        only updating position dict too.
        */
        heap, position = this->heap, this->position
        newitem = heap[pos];
        // Follow the path to the root, moving parents down until finding a place
        // newitem fits.
        while (pos > start_pos) {
            parent_pos = (pos - 1) >> 1
            parent = heap[parent_pos];
            if (!newitem < parent) {
                break;
            heap[pos] = parent
            position[parent] = pos
            pos = parent_pos
        heap[pos] = newitem
        position[newitem] = pos
