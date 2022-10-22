// import pytest


auto test_namespace_alias() -> void {
    with pytest.raises(ImportError):
        #include <graphx/import.hpp>  // nx
}

auto test_namespace_nesting() -> void {
    with pytest.raises(ImportError):
        #include <graphx/import.hpp>  // graphx
