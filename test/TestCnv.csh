#!/bin/sh
#
#
../bin/mmcif2XML -dictName mmcif_pdbx.dic -df ../../odb/mmcif_pdbx.odb \
    -prefix pdbx-v32 -ns PDBx -funct mmcif2xmlall -f ATP.cif 2>&1 \
    | tee TestCnv-1.log
#
../bin/mmcif2XML -dictName mmcif_pdbx.dic -df ../../odb/mmcif_pdbx.odb \
    -prefix pdbx-v32 -ns PDBx -funct mmcif2xmlall -f 1j59.cif 2>&1 \
    | tee TestCnv-2.log
# 
../bin/mmcif2XML -dictName mmcif_pdbx.dic -df ../../odb/mmcif_pdbx.odb \
    -prefix pdbx-v32 -ns PDBx -funct mmcif2xmlall -f 1kip.cif 2>&1 \
    | tee TestCnv-3.log
#
