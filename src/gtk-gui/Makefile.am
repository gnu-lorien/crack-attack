noinst_LIBRARIES = libgtkgui.a

libgtkgui_a_SOURCES = \
  interface.cxx callbacks.cxx support.cxx gui_main.cxx persist.cxx modeparser.cxx \
  interface.h   callbacks.h   support.h   gui_main.h   persist.h   modeparser.h

AM_CXXFLAGS = -O2 -pedantic -Wall

if WANT_BINRELOC
  PATH_INCLUDES = 
else
  PATH_INCLUDES = -DDATA_DIRECTORY='"${pkgdatadir}/"' \
      -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
    -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
		-DBINARY_DIRECTORY='"${bindir}"'
endif

INCLUDES = @PACKAGE_CFLAGS@ @DEBUG_INCLUDES@ ${PATH_INCLUDES}

AM_LDFLAGS = ${all_libraries}
LDADD = ${X_PRE_LIBS} ${X_LIBS} ${X_EXTRA_LIBS} -lGL -lGLU -lglut -lXi -lX11 -lXmu @PACKAGE_LIBS@
