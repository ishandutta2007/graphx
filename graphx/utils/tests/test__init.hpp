// import pytest


auto test_utils_namespace() -> void {
    /** Ensure objects are not unintentionally exposed in utils namespace.*/
    with pytest.raises(ImportError):
        #include <graphx/utils.hpp>  // import nx
    with pytest.raises(ImportError):
        #include <graphx/utils.hpp>  // import sys
    with pytest.raises(ImportError):
        #include <graphx/utils.hpp>  // import defaultdict, deque
