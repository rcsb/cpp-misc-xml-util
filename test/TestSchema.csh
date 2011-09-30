#!/bin/sh

../bin/Dict2XMLSchema -dictName mmcif_pdbx.dic \
  -df ../../odb/mmcif_pdbx_v40.odb \
  -ns "PDBx" -prefix pdbx-v40 2>&1 | tee pdbx.log

