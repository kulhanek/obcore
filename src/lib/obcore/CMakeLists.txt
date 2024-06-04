# ==============================================================================
# OBCore CMake File
# ==============================================================================

ADD_DEFINITIONS(-DUSING_DYNAMIC_LIBS -DHAVE_EIGEN -DHAVE_EIGEN3)

SET(OBCORE_LIB_SRC
        math/align.cpp
        math/matrix3x3.cpp
        math/spacegroup.cpp
        math/transform3d.cpp
        math/vector3.cpp

        forcefields/forcefieldgaff.cpp
#        forcefields/forcefieldmm2.cpp
        forcefields/forcefieldghemical.cpp
        forcefields/forcefieldmmff94.cpp
        forcefields/forcefielduff.cpp

        stereo/cistrans.cpp
        stereo/facade.cpp
        stereo/gen3dstereohelper.cpp
        stereo/perception.cpp
        stereo/squareplanar.cpp
        stereo/stereo.cpp
        stereo/tetrahedral.cpp
        stereo/tetranonplanar.cpp
        stereo/tetraplanar.cpp

        charges/eem.cpp
        charges/eqeq.cpp
        charges/fromfile.cpp
        charges/gasteiger.cpp
        charges/mmff94.cpp
        charges/none.cpp
        charges/qeq.cpp
        charges/qtpie.cpp

        descriptors/cansmidescriptor.cpp
        descriptors/cmpdfilter.cpp
        descriptors/filters.cpp
        descriptors/groupcontrib.cpp
        descriptors/inchidescriptor.cpp
        descriptors/smartsdescriptors.cpp

        fingerprints/fingerecfp.cpp
        fingerprints/finger2.cpp
        fingerprints/finger3.cpp

        depict/asciipainter.cpp
        # depict/cairopainter.cpp
        depict/commandpainter.cpp
        depict/depict.cpp
        depict/svgpainter.cpp

        alias.cpp
        atom.cpp
        base.cpp
        bitvec.cpp
        bond.cpp
        bondtyper.cpp
        builder.cpp
        canon.cpp
        chains.cpp
        chargemodel.cpp
        conformersearch.cpp
        confsearch.cpp
        data.cpp
        data_utilities.cpp
        descriptor.cpp
        distgeom.cpp
        dlhandler_unix.cpp
       # dlhandler_win32.cpp
        doxygen_pages.cpp
        elements.cpp
        fingerprint.cpp
        forcefield.cpp
        format.cpp
        generic.cpp
        graphsym.cpp
        grid.cpp
        griddata.cpp
        isomorphism.cpp
        kekulize.cpp
        locale.cpp
        matrix.cpp
        mcdlutil.cpp
        molchrg.cpp
        mol.cpp
        obconversion.cpp
        oberror.cpp
        obfunctions.cpp
        obiter.cpp
        obmolecformat.cpp
        obutil.cpp
        op.cpp
        parsmart.cpp
        patty.cpp
        phmodel.cpp
        plugin.cpp
        pointgroup.cpp
        query.cpp
        rand.cpp
#        RDKitConv.cpp
        reactionfacade.cpp
        residue.cpp
        ring.cpp
        rotamer.cpp
        rotor.cpp
        spectrophore.cpp
        tautomer.cpp
        tokenst.cpp
        transform.cpp
        typer.cpp
        )

# system dependent stuff -------------------------
IF(UNIX)
    SET(OBCORE_LIB_SRC
        dlhandler_unix.cpp
        prefix_unix.c
        ${OBCORE_LIB_SRC}
        )
ENDIF(UNIX)

IF(WIN32)
    SET(OBCORE_LIB_SRC
        dlhandler_win32.cpp
        prefix_win32.cpp
        ${OBCORE_LIB_SRC}
        )
ENDIF(WIN32)

# create shared library --------------------------------------------------------

ADD_LIBRARY(obcore_shared SHARED ${OBCORE_LIB_SRC})

IF(UNIX)
    TARGET_LINK_LIBRARIES(obcore_shared
                dl
                )
ENDIF(UNIX)

SET_TARGET_PROPERTIES(obcore_shared PROPERTIES
                        OUTPUT_NAME obcore
                        CLEAN_DIRECT_OUTPUT 1
                        VERSION ${OBCORE_VERSION})
INSTALL(TARGETS
            obcore_shared
        DESTINATION
            lib
        )

#-------------------------------------------------------------------------------
