
find_path(LIBLO_INCLUDE_DIR lo/lo.h)

set(LIBLO_INCLUDE_DIRS ${LIBLO_INCLUDE_DIR})

find_library(LIBLO_LIBRARY lo)
if (LIBLO_LIBRARY)
	set(LIBLO_LIBRARIES ${LIBLO_LIBRARY})
endif (LIBLO_LIBRARY)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LIBLO DEFAULT_MSG LIBLO_INCLUDE_DIRS LIBLO_LIBRARIES)

