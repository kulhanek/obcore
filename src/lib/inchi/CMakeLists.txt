# ==============================================================================
# OBCore CMake File
# ==============================================================================

FILE(GLOB INCHI_SRC "*.c")

add_library(inchi_shared SHARED ${INCHI_SRC})

SET_TARGET_PROPERTIES(inchi_shared PROPERTIES
        OUTPUT_NAME inchi
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${OBCORE_VERSION}
        )

INSTALL(TARGETS
            inchi_shared
        DESTINATION
            lib
        )
