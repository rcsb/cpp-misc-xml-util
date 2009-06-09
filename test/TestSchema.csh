#!/bin/sh

../bin/Dict2XMLSchema -dictName mmcif_pdbx.dic -df ../../odb/mmcif_pdbx.odb \
  -ns "PDBx" -prefix pdbx-v32 2>&1 | tee pdbx.log

