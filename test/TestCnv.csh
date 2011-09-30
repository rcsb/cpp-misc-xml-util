#!/bin/sh
#
#
../bin/mmcif2XML -dictName mmcif_pdbx.dic -df ../../odb/mmcif_pdbx_v40.odb \
    -prefix pdbx-v40 -ns PDBx -funct mmcif2xmlall -f ATP.cif 2>&1 \
    | tee TestCnv-1.log
#
../bin/mmcif2XML -dictName mmcif_pdbx.dic -df ../../odb/mmcif_pdbx_v40.odb \
    -prefix pdbx-v40 -ns PDBx -funct mmcif2xmlall -f 1j59.cif 2>&1 \
    | tee TestCnv-2.log
# 
../bin/mmcif2XML -dictName mmcif_pdbx.dic -df ../../odb/mmcif_pdbx_v40.odb \
    -prefix pdbx-v40 -ns PDBx -funct mmcif2xmlall -f 1kip.cif 2>&1 \
    | tee TestCnv-3.log
#
