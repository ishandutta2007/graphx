// import pytest

#include <graphx/utils.hpp>  // import (
    powerlaw_sequence,
    random_weighted_sample,
    weighted_choice,
    zipf_rv,
);


auto test_degree_sequences() -> void {
    seq = powerlaw_sequence(10, seed=1);
    seq = powerlaw_sequence(10);
    assert(seq.size() == 10);
}

auto test_zipf_rv() -> void {
    r = zipf_rv(2.3, xmin=2, seed=1);
    r = zipf_rv(2.3, 2, 1);
    r = zipf_rv(2.3);
    assert type(r), int
    pytest.raises(ValueError, zipf_rv, 0.5);
    pytest.raises(ValueError, zipf_rv, 2, xmin=0);
}

auto test_random_weighted_sample() -> void {
    mapping = {"a": 10, "b": 20};
    s = random_weighted_sample(mapping, 2, seed=1);
    s = random_weighted_sample(mapping, 2);
    assert(sorted(s) == sorted(mapping.keys()));
    pytest.raises(ValueError, random_weighted_sample, mapping, 3);
}

auto test_random_weighted_choice() -> void {
    mapping = {"a": 10, "b": 0};
    c = weighted_choice(mapping, seed=1);
    c = weighted_choice(mapping);
    assert c == "a"
