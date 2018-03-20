#!/bin/sh

// Assume that the input is CIF exchange file format

../bin/mmcif2XML -dictName mmcif_pdbx.dic -df ../odb/mmcif_pdbx_v40.odb -prefix pdbx-v40 -ns PDBx -funct mmcif2xmlall -f /ftp-weekly-release/pending/current/mmcif/3f9l.cif 2>&1 | tee 3f91.log

cp ../xml_v50/pdbx-v40-v4.999.xsd pdbx-v40.xsd

// sed this
vi pdbx*.xsd
// sed this
vi 3f9l.cif.xml

export XERCESCROOT=/apps/xerces
export LD_LIBRARY_PATH=/apps/xerces/lib:/apps/xerces/lib:/opt/python/lib
/lcl/bin/SAX2Diags-2.8.0 -f -p -v=always 3f9l.cif.xml 2>&1 | tee 3f9l.cif.xml.log
