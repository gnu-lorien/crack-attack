noinst_LIBRARIES = libgtkgui.a

libgtkgui_a_SOURCES = \
  interface.cxx callbacks.cxx support.cxx gui_main.cxx \
  interface.h   callbacks.h   support.h   gui_main.h

AM_CXXFLAGS = -O2
INCLUDES = -DDATA_DIRECTORY='"${pkgdatadir}/"'	\
    -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
  @PACKAGE_CFLAGS@ @DEBUG_INCLUDES@

AM_LDFLAGS = ${all_libraries}
LDADD = ${X_PRE_LIBS} ${X_LIBS} ${X_EXTRA_LIBS} -lGL -lGLU -lglut -lXi -lX11 -lXmu @PACKAGE_LIBS@
