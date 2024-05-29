# ==============================================================================
# OBCore CMake File
# ==============================================================================

ADD_DEFINITIONS(-DUSING_DYNAMIC_LIBS)

SET(OBCORE_LIB_SRC
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
        stereo/perception.cpp
        stereo/squareplanar.cpp
        stereo/stereo.cpp
        stereo/tetrahedral.cpp
        stereo/tetranonplanar.cpp
        stereo/tetraplanar.cpp
        stereo/gen3dstereohelper.cpp

        ops/addinindex.cpp
        ops/addpolarh.cpp
        ops/canonical.cpp
        ops/fillUC.cpp
        ops/forcefield.cpp
        ops/gen3d.cpp
        ops/loader.cpp
        ops/opisomorph.cpp
        ops/optransform.cpp
        ops/partialcharges.cpp
        ops/readconformers.cpp
        ops/sort.cpp
        ops/xout.cpp
        #ops/conformer.cpp
        ops/gen2D.cpp
        #ops/op2d.cpp
        #ops/unique.cpp

        charges/gasteiger.cpp
        charges/mmff94.cpp
        charges/qeq.cpp
        charges/qtpie.cpp

        descriptors/cansmidescriptor.cpp
        descriptors/cmpdfilter.cpp
        descriptors/filters.cpp
        descriptors/groupcontrib.cpp
        descriptors/inchidescriptor.cpp
        descriptors/smartsdescriptors.cpp

        fingerprints/finger2.cpp
        fingerprints/finger3.cpp

        depict/depict.cpp
        depict/svgpainter.cpp

        graphsym.cpp
        alias.cpp
        atom.cpp
        base.cpp
        bitvec.cpp
        bond.cpp
        bondtyper.cpp
        builder.cpp
        canon.cpp
        chains.cpp
        # chiral.cpp
        chargemodel.cpp
        data.cpp
        descriptor.cpp
        elements.cpp
        fingerprint.cpp
        forcefield.cpp
        format.cpp
        generic.cpp
        grid.cpp
        griddata.cpp
        isomorphism.cpp
        kekulize.cpp
        locale.cpp
        matrix.cpp
        molchrg.cpp
        mol.cpp
        mcdlutil.cpp
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
        residue.cpp
        ring.cpp
        rotamer.cpp
        rotor.cpp
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
