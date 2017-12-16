# ==============================================================================
# OBCore CMake File
# ==============================================================================

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/formats)

SET(FORMATS
        acrformat
        adfformat
        alchemyformat
        amberformat
        balstformat
        bgfformat
        boxformat
        cacaoformat
        cacheformat
        carformat
        cccformat
        chem3dformat
        chemdrawct
        chemkinformat
        chemtoolformat
        cifformat
        copyformat
        crkformat
        cssrformat
        dmolformat
        fastaformat
        fastsearchformat
        fchkformat
        featformat
        fhformat
        fingerprintformat
        freefracformat
        gamessformat
        gausscubeformat
        gaussformat
        gausszmatformat
        ghemicalformat
        gromos96format
        hinformat
        jaguarformat
        mdlformat
        mmcifformat
        mmodformat
        mol2format
        moldenformat
        molproformat
        molreport
        mopacformat
        mpdformat
        mpqcformat
        msiformat
        msmsformat
        nwchemformat
        opendxformat
        outformat
        pcmodelformat
        pdbformat
        pdbqtformat
        povrayformat
        pqrformat
        qchemformat
        reportformat
        rsmiformat
        rxnformat
        shelxformat
        smilesformat
        svgformat
        thermoformat
        tinkerformat
        titleformat
        turbomoleformat
        unichemformat
        vaspformat
        viewmolformat
        xedformat
        xyzformat
        yasaraformat
        zindoformat
        )

# system dependent stuff -------------------------

IF(UNIX)
    SET(FORMATS
#        pngformat
#        xtcformat
#        gamessukformat
        ${FORMATS}
        )
ENDIF(UNIX)

# ------------------------------------------------------------------------------
FOREACH(FORMAT ${FORMATS})
  ADD_LIBRARY(${FORMAT} SHARED ${FORMAT}.cpp)

  TARGET_LINK_LIBRARIES(${FORMAT} ${OBCORE_LIB_NAME})

  SET_TARGET_PROPERTIES(${FORMAT} PROPERTIES
                        OUTPUT_NAME ${FORMAT}
                        PREFIX "")

  ADD_DEPENDENCIES(${FORMAT} obcore_shared)

  INSTALL(TARGETS
            ${FORMAT}
        DESTINATION
            lib/formats
        )
ENDFOREACH(FORMAT ${FORMATS})

# ------------------------------------------------------------------

SET(INCHI_FORMAT inchiformat)
SET(INCHI_FORMAT_SRC inchiformat.cpp getinchi.cpp)
ADD_LIBRARY(${INCHI_FORMAT} SHARED ${INCHI_FORMAT_SRC})

TARGET_LINK_LIBRARIES(${INCHI_FORMAT} ${OBCORE_LIB_NAME} ${INCHI_LIB_NAME})

SET_TARGET_PROPERTIES(${INCHI_FORMAT} PROPERTIES
                    OUTPUT_NAME ${INCHI_FORMAT}
                    PREFIX "")

ADD_DEPENDENCIES(${INCHI_FORMAT} obcore_shared)

INSTALL(TARGETS
        ${INCHI_FORMAT}
    DESTINATION
        lib/formats
    )
