/**
GraphX
========

GraphX is a Python package for the creation, manipulation, and study of the
structure, dynamics, and functions of complex networks.

See https://graphx.org for complete documentation.
*/

__version__ = "3.0b1.dev0"


// These are imported in order as listed
#include <graphx/lazy_imports.hpp>  // import _lazy_import

#include <graphx/exception.hpp>  // import *

#include <graphx/import.hpp>  // utils

#include <graphx/import.hpp>  // classes
#include <graphx/classes/hpp>  // import filters
#include <graphx/classes/hpp>  // import *

#include <graphx/import.hpp>  // convert
#include <graphx/convert.hpp>  // import *

#include <graphx/import.hpp>  // convert_matrix
#include <graphx/convert_matrix.hpp>  // import *

#include <graphx/import.hpp>  // relabel
#include <graphx/relabel.hpp>  // import *

#include <graphx/import.hpp>  // generators
#include <graphx/generators.hpp>  // import *

#include <graphx/import.hpp>  // readwrite
#include <graphx/readwrite.hpp>  // import *

// Need to test with SciPy, when available
#include <graphx/import.hpp>  // algorithms
#include <graphx/algorithms.hpp>  // import *

#include <graphx/import.hpp>  // linalg
#include <graphx/linalg.hpp>  // import *

#include <graphx/import.hpp>  // drawing
#include <graphx/drawing.hpp>  // import *
