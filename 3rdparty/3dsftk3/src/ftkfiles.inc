#
# This file is used by all compiler platforms to determine dependencies
#


LIBRARY_HEADERS1 = 3dstype.h 3dsprim.h 3dserr.h chunkinf.h chunk3ds.h 3dsfile.h dbase3ds.h 
LIBRARY_HEADERS2 = 3dsrange.h 3dsftkst.h 3dsstrlf.h 3dsvprt.h 3dsmset.h 3dsaset.h 3dsbgnd.h 
LIBRARY_HEADERS3 = 3dsmatr.h 3dsiobj.h 3dsmobj.h 3dsrobj.h 3dslites.h 3dscamr.h 3dskey.h 
LIBRARY_HEADERS4 = 3dscamm.h 3dsambm.h 3dsobjm.h 3dsomnm.h 3dssptm.h 3dsmatx.h
LIBRARY_HEADERS5 = mdata3ds.h kdata3ds.h dumpsrc.h strcmpi.h  kfutils.h 3dshier.h 3dsvers.h 
LIBRARY_HEADERS6 = 3dsbbox.h   ftkhead.h ftktail.h
LIBRARY_HEADERS7 = seekchil.h swapbyte.h smartall.h 3dsutil.h dumpstr.h dumpchnk.h xdata.h 
LIBRARY_HEADERS8 = 
LIBRARY_HEADERS9 = 


LIBRARY_HEADERS = $(LIBRARY_HEADERS1) $(LIBRARY_HEADERS2) $(LIBRARY_HEADERS3) $(LIBRARY_HEADERS4) $(LIBRARY_HEADERS5) $(LIBRARY_HEADERS6) $(LIBRARY_HEADERS7) $(LIBRARY_HEADERS8) $(LIBRARY_HEADERS9)


LIBRARY_SOURCE = 3dsfile.c dbase3ds.c chunk3ds.c 3dserr.c 3dsstrlf.c seekchil.c 3dsprim.c makeswap.c smartall.c strcmpi.c 3dsutil.c 3dsiobj.c 3dsrobj.c 3dsmatr.c dumpstr.c 3dsmobj.c 3dsobjm.c assert.c dumpchnk.c xdata.c mdata3ds.c kdata3ds.c 3dscamr.c 3dscamm.c strdup.c dumpsrc.c 3dsaset.c 3dsmset.c 3dsbgnd.c kfutils.c 3dslites.c 3dshier.c scanhdr.c 3dssptm.c 3dsomnm.c 3dsambm.c 3dsvers.c 3dskey.c 3dsmatx.c 3dsbbox.c 3dsvprt.c

LIBRARY_SRC_DIR = source

LIBRARY_BATCH = buildat.bat targat.bat makelibs.bat makelib.bat maketarg.bat copykit.bat 
TOOLKIT_BATCH = makesamp.bat makesmp2.bat

MAKEFILES_UNX = makefile.spc
MAKEFILES_DOS = makefile.w95 makefile.mw3 makefile.m30 makefile.vc1 ftkvc22.mak ftkwc10.mak 

MAKEFILE_INC = ftkfiles.inc

UTILITY_DEPS = $(INC)/dumpchnk.h $(INC)/3dstype.h $(INC)/dbase3ds.h $(INC)/3dsmobj.h $(INC)/mdata3ds.h $(INC)/3dscamr.h $(INC)/3dscamm.h $(INC)/3dsaset.h $(INC)/3dsmset.h $(INC)/3dsbgnd.h $(INC)/3dslites.h

DUMP3DS_SOURCE = dump3ds.c
DUMP3DS_DIR = dump3ds

XDATA3DS_SOURCE = xdata3ds.c
XDATA3DS_DIR = xdata3ds

DUPTEST_SOURCE = duptest.c
DUPTEST_DIR = duptest

EXAMPLE_DIR = examples

POLYGON_SOURCE = polygon.c
POLYGON_DIR = $(EXAMPLE_DIR)/polygon

TUNNEL_SOURCE = tunnel.c tunnel2.c tunnel.h
TUNNEL_DIR = $(EXAMPLE_DIR)/tunnel

TOOLKIT_HEADER = 3dsftk.h
TOOLKIT_HEADER_DIR = inc

TOOLKIT_LIBS = 3dftkm30.lib 3dftkm32.lib 3dftkvc1.lib 3dftkw95.lib
TOOLKIT_LIB_DIR = lib

TOOLKIT_DIRS = $(DUMP3DS_DIR) $(DUPTEST_DIR) $(EXAMPLE_DIR) $(POLYGON_DIR) $(TUNNEL_DIR) $(TOOLKIT_HEADER_DIR) $(TOOLKIT_LIB_DIR) 

3DSOBJS = 3dsfile.$(OXT) dbase3ds.$(OXT) chunk3ds.$(OXT) 3dserr.$(OXT) 3dsstrlf.$(OXT) seekchil.$(OXT) 3dsprim.$(OXT) smartall.$(OXT) strcmpi.$(OXT) 3dsutil.$(OXT) 3dsiobj.$(OXT) 3dsrobj.$(OXT) 3dsmatr.$(OXT) dumpstr.$(OXT) 3dsmobj.$(OXT) 3dsobjm.$(OXT) assert.$(OXT) dumpchnk.$(OXT) xdata.$(OXT) mdata3ds.$(OXT) kdata3ds.$(OXT) 3dscamr.$(OXT) 3dscamm.$(OXT) strdup.$(OXT) dumpsrc.$(OXT) 3dsaset.$(OXT) 3dsmset.$(OXT) 3dsbgnd.$(OXT) kfutils.$(OXT) 3dslites.$(OXT) 3dshier.$(OXT) 3dssptm.$(OXT) 3dsomnm.$(OXT) 3dsambm.$(OXT) 3dsvers.$(OXT) 3dskey.$(OXT) 3dsmatx.$(OXT) 3dsbbox.$(OXT) 3dsvprt.$(OXT) swapbyte.$(OXT)

3DSFILE_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dsstrlf.h 3dsrange.h 3dsftkst.h 3dsfile.h 3dserr.h


DBASE3DS_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dsfile.h 3dsrange.h 3dserr.h 3dsftkst.h chunk3ds.h 3dsutil.h seekchil.h dbase3ds.h 3dshier.h 3dsvers.h


CHUNK3DS_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dsfile.h 3dsrange.h 3dserr.h chunk3ds.h 3dsftkst.h 3dsutil.h


3DSERR_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h


3DSSTRLF_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsstrlf.h


SEEKCHIL_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h 3dsfile.h 3dsrange.h 3dsftkst.h seekchil.h chunk3ds.h


3DSPRIM_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsfile.h 3dsrange.h swapbyte.h chunkinf.h


MAKESWAP_DEP = 3dstype.h smartall.h


SMARTALL_DEP = 

STRCMPI_DEP = 3dstype.h smartall.h strcmpi.h


3DSUTIL_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h chunk3ds.h 3dserr.h 3dsutil.h 3dsftkst.h


3DSIOBJ_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsmobj.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h chunk3ds.h xdata.h 3dsrobj.h 3dsiobj.h


3DSROBJ_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsmobj.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h chunk3ds.h xdata.h 3dsrobj.h


3DSMATR_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h 3dsmatr.h dbase3ds.h 3dsfile.h 3dsrange.h chunk3ds.h xdata.h 3dsftkst.h


DUMPSTR_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsmobj.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h chunk3ds.h xdata.h 3dsmatr.h dumpstr.h


3DSMOBJ_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsmobj.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h chunk3ds.h xdata.h 3dsftkst.h 3dsiobj.h 3dsutil.h


3DSOBJM_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h xdata.h 3dsobjm.h dbase3ds.h 3dsfile.h 3dsrange.h kfutils.h


ASSERT_DEP = 

DUMPCHNK_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h 3dsftkst.h 3dsutil.h chunk3ds.h dumpchnk.h dbase3ds.h 3dsfile.h 3dsrange.h 3dsmatr.h xdata.h kfutils.h


XDATA_DEP = 3dstype.h smartall.h chunkinf.h chunk3ds.h 3dserr.h 3dsprim.h 3dsftkst.h xdata.h


MDATA3DS_DEP = 3dstype.h smartall.h 3dsfile.h 3dsrange.h 3dserr.h 3dsprim.h dbase3ds.h chunk3ds.h 3dsftkst.h mdata3ds.h xdata.h


KDATA3DS_DEP = 3dstype.h smartall.h 3dsfile.h 3dsrange.h 3dserr.h 3dsprim.h dbase3ds.h chunk3ds.h 3dsftkst.h kdata3ds.h xdata.h


3DSCAMR_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dscamr.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h chunk3ds.h xdata.h 3dsftkst.h 3dsutil.h


3DSCAMM_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h xdata.h 3dscamm.h dbase3ds.h 3dsfile.h 3dsrange.h kfutils.h


STRDUP_DEP = 3dstype.h smartall.h


DUMPSRC_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsmobj.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h chunk3ds.h xdata.h 3dsmatr.h dumpsrc.h


3DSASET_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsftkst.h chunk3ds.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h 3dsaset.h


3DSMSET_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsftkst.h chunk3ds.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h 3dsmset.h mdata3ds.h xdata.h


3DSBGND_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsbgnd.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h chunk3ds.h 3dsftkst.h


KFUTILS_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h xdata.h 3dscamm.h dbase3ds.h 3dsfile.h 3dsrange.h kfutils.h


3DSLITES_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h xdata.h 3dslites.h 3dsmset.h dbase3ds.h 3dsfile.h 3dsrange.h kfutils.h


3DSHIER_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h 3dshier.h dbase3ds.h kfutils.h 3dsfile.h 3dsrange.h xdata.h


SCANHDR_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h


3DSSPTM_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h xdata.h 3dssptm.h dbase3ds.h 3dsfile.h 3dsrange.h kfutils.h


3DSOMNM_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h xdata.h 3dssptm.h dbase3ds.h 3dsfile.h 3dsrange.h 3dsomnm.h kfutils.h


3DSAMBM_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h xdata.h 3dssptm.h dbase3ds.h 3dsfile.h 3dsrange.h 3dsambm.h kfutils.h


3DSVERS_DEP = 3dstype.h smartall.h dbase3ds.h 3dsfile.h 3dsrange.h 3dserr.h 3dsprim.h chunk3ds.h chunkinf.h 3dsftkst.h 3dsvers.h


3DSKEY_DEP = 3dstype.h smartall.h 3dsprim.h chunkinf.h 3dserr.h chunk3ds.h 3dsftkst.h xdata.h 3dscamm.h dbase3ds.h 3dsfile.h 3dsrange.h kfutils.h 3dskey.h


3DSMATX_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h chunk3ds.h chunkinf.h 3dsmobj.h dbase3ds.h 3dsfile.h 3dsrange.h xdata.h 3dsmatx.h


3DSBBOX_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h chunk3ds.h chunkinf.h 3dsmobj.h dbase3ds.h 3dsfile.h 3dsrange.h xdata.h 3dsobjm.h 3dsbbox.h

3DSVPRT_DEP = 3dstype.h smartall.h 3dserr.h 3dsprim.h 3dsftkst.h chunk3ds.h chunkinf.h dbase3ds.h 3dsfile.h 3dsrange.h 3dsvprt.h

SWAPBYTE_DEP = 3dstype.h smartall.h swapbyte.h

3dsfile.$(OXT):		3dsfile.c $(3DSFILE_DEP)
dbase3ds.$(OXT):	dbase3ds.c $(DBASE3DS_DEP)
chunk3ds.$(OXT):	chunk3ds.c $(CHUNK3DS_DEP)
3dserr.$(OXT):		3dserr.c $(3DSERR_DEP)
3dsstrlf.$(OXT):	3dsstrlf.c $(3DSSTRLF_DEP)
seekchil.$(OXT):	seekchil.c $(SEEKCHIL_DEP)
3dsprim.$(OXT):		3dsprim.c $(3DSPRIM_DEP)
makeswap.$(OXT):	makeswap.c $(MAKESWAP_DEP)
smartall.$(OXT):	smartall.c $(SMARTALL_DEP)
strcmpi.$(OXT):		strcmpi.c $(STRCMPI_DEP)
3dsutil.$(OXT):		3dsutil.c $(3DSUTIL_DEP)
3dsiobj.$(OXT):		3dsiobj.c $(3DSIOBJ_DEP)
3dsrobj.$(OXT):		3dsrobj.c $(3DSROBJ_DEP)
3dsmatr.$(OXT):		3dsmatr.c $(3DSMATR_DEP)
dumpstr.$(OXT):		dumpstr.c $(DUMPSTR_DEP)
3dsmobj.$(OXT):		3dsmobj.c $(3DSMOBJ_DEP)
3dsobjm.$(OXT):		3dsobjm.c $(3DSOBJM_DEP)
assert.$(OXT):		assert.c $(ASSERT_DEP)
dumpchnk.$(OXT):	dumpchnk.c $(DUMPCHNK_DEP)
xdata.$(OXT):		xdata.c $(XDATA_DEP)
mdata3ds.$(OXT):	mdata3ds.c $(MDATA3DS_DEP)
kdata3ds.$(OXT):	kdata3ds.c $(KDATA3DS_DEP)
3dscamr.$(OXT):		3dscamr.c $(3DSCAMR_DEP)
3dscamm.$(OXT):		3dscamm.c $(3DSCAMM_DEP)
strdup.$(OXT):		strdup.c $(STRDUP_DEP)
dumpsrc.$(OXT):		dumpsrc.c $(DUMPSRC_DEP)
3dsaset.$(OXT):		3dsaset.c $(3DSASET_DEP)
3dsmset.$(OXT):		3dsmset.c $(3DSMSET_DEP)
3dsbgnd.$(OXT):		3dsbgnd.c $(3DSBGND_DEP)
kfutils.$(OXT):		kfutils.c $(KFUTILS_DEP)
3dslites.$(OXT):	3dslites.c $(3DSLITES_DEP)
3dshier.$(OXT):		3dshier.c $(3DSHIER_DEP)
scanhdr.$(OXT):		scanhdr.c $(SCANHDR_DEP)
3dssptm.$(OXT):		3dssptm.c $(3DSSPTM_DEP)
3dsomnm.$(OXT):		3dsomnm.c $(3DSOMNM_DEP)
3dsambm.$(OXT):		3dsambm.c $(3DSAMBM_DEP)
3dsvers.$(OXT):		3dsvers.c $(3DSVERS_DEP)
3dskey.$(OXT):		3dskey.c $(3DSKEY_DEP)
3dsmatx.$(OXT):		3dsmatx.c $(3DSMATX_DEP)
3dsbbox.$(OXT):		3dsbbox.c $(3DSBBOX_DEP)
3dsvprt.$(OXT):		3dsvprt.c $(3DSVPRT_DEP)

swapbyte.$(OXT):	swapbyte.c $(SWAPBYTE_DEP)
