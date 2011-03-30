##
# SConscript for mmcif-xml-util-v2.0
# Updated: Dec 18, 2008 - Jdw
#          Mar 30, 2011 - jdw clone environment
#
Import('env')
env=env.Clone()
#
#if (len(env.subst('$MYDEBUG')) > 0):
#	dict = env.Dictionary()
#	for k,v in dict.items():
#		print  k, " = ", str(v)
#
libName = 'xmlutil'

libSrcList = ['src/MmcifToXml.C']

libObjList = [s.replace('.C','.o') for s in libSrcList]
#
libIncList = ['include/MmcifToXml.h']

myLib=env.Library(libName,libSrcList)
#
#
env.Install(env.subst('$MY_INCLUDE_INSTALL_PATH'),libIncList)
env.Alias('install-include',env.subst('$MY_INCLUDE_INSTALL_PATH'))
#
env.Install(env.subst('$MY_LIB_INSTALL_PATH'),myLib)
env.Alias('install-lib',env.subst('$MY_LIB_INSTALL_PATH'))
#
env.Install(env.subst('$MY_OBJ_INSTALL_PATH'),libObjList)
env.Alias('install-obj',env.subst('$MY_OBJ_INSTALL_PATH'))
#
env.Default('install-include','install-obj','install-lib')
#
