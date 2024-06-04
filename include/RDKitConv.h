#include <openbabel/mol.h>
#include <openbabel/obiter.h>
#include <GraphMol/RWMol.h>
#include <GraphMol/Atom.h>

///Convert OpenBabel OBMol to and from RGKit molecules
RDKit::RWMol OBMolToRWMol(OpenBabel::OBMol* pOBMol);

//! \file RDKitConv.h
//! \brief Allow conversion from OBMol to RDKit RWMol
