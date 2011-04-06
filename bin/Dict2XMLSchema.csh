#!/bin/csh -f
#
# File:  Dict2XMLSchema.csh
# Date:  15-April-2004 J. Westbrook
#
if ("$1" == "" ) then 
   echo "Usage:  $0 <Dictionary name (e.g. mmcif_pdbx)>"
   exit 1
endif

set dictToSchemaExe = "../bin/Dict2XMLSchema"
set xmlDir = "xml_v40"

if ("$2" == "" ) then
   set dictToSchemaExe = "/lcl/bin/Dict2XMLSchema"
   set xmlDir = "xml_v32"
endif

set d = "Creating XML schema from $1"

set d = "$1"
set dName = $d

switch ($d)
    case mmcif_pdbx:
    set prefix = "pdbx-v32"
    set ns     = "PDBx"
    breaksw

    case mmcif_pdbx_v33:
    set prefix = "pdbx-v33"
    set ns     = "PDBx"
    set dName = "mmcif_pdbx"
    breaksw

    case mmcif_pdbx_v31:
    set prefix = "pdbx"
    set ns     = "PDBx"
    set dName = "mmcif_pdbx"
    breaksw

    case mmcif_pdbx_internal:
    set prefix = "pdbx"
    set ns     = "PDBx"
    set dName = "mmcif_pdbx"
    exit 0
    breaksw

    case mmcif_pdbx_v4_internal:
    set prefix = "pdbx-v40"
    set ns     = "PDBx"
    set dName = "mmcif_pdbx"
    breaksw
    
    case mmcif_std:
    set prefix = "mmcif_std"
    set ns     = "mmCIF"
    breaksw

    case mmcif_biosync:
    set prefix = "mmcif_biosync"
    set ns     = "biosync"
    breaksw

    case mmcif_nmr-star:
    set prefix = "mmcif_nmr-star"
    set ns     = "nmrstar"
    breaksw

    case mmcif_bsoft:
    set prefix = "mmcif_bsoft"
    set ns     = "bsoft"
    breaksw

    case mmcif_ccp4:
    set prefix = "mmcif_ccp4"
    set ns     = "ccp4"
    breaksw

    case mmcif_ddl:
    set prefix = "mmcif_ddl"
    set ns     = "mmcif_ddl"
    breaksw

    case mmcif_emx:
    set prefix = "mmcif_emx"
    set ns     = "emx"
    breaksw

    case mmcif_em:
    set prefix = "mmcif_em"
    set ns     = "mmcif_em"
    breaksw

    case mmcif_iims:
    set prefix = "mmcif_iims"
    set ns     = "mmcif_iims"
    breaksw

    case mmcif_img:
    set prefix = "mmcif_img"
    set ns     = "mmcif_img"
    breaksw

    case mmcif_mdb:
    set prefix = "mmcif_mdb"
    set ns     = "mmcif_mdb"
    breaksw

    case mmcif_rcsb_internal:
    set prefix = "mmcif_rcsb_internal"
    set ns     = "RCSB"
    breaksw

    case mmcif_rcsb_nmr:
    set prefix = "mmcif_rcsb_nmr"
    set ns     = "RCSB"
    breaksw

    case mmcif_rcsb_xray:
    set prefix = "mmcif_rcsb_xray"
    set ns     = "RCSB"
    breaksw

    case mmcif_sym:
    set prefix = "mmcif_sym"
    set ns     = "mmcif_sym"
    breaksw

    default:
    set prefix = "mmcif"
    set ns     = "mmCIF"
    breaksw

    endsw
#
cd $xmlDir; $dictToSchemaExe -dictName $dName.dic -df ../odb/$d.odb \
  -ns $ns -prefix $prefix >& $d.log
  
#
