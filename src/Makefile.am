bin_PROGRAMS = crack-attack

if AUDIO_ENABLED
audio_sources = Music.cxx Sound.cxx
else
audio_sources = 
endif
crack_attack_SOURCES = ActionRecorder.cxx Attack.cxx Block.cxx BlockManager.cxx CelebrationManager.cxx Clock.cxx ComboManager.cxx ComboTabulator.cxx Communicator.cxx ComputerPlayer.cxx ComputerPlayerAI.cxx Controller.cxx CountDownManager.cxx Creep.cxx Displayer.cxx Game.cxx Garbage.cxx GarbageFlavorImage.cxx GarbageGenerator.cxx GarbageManager.cxx GarbageQueue.cxx Grid.cxx LevelLights.cxx LightManager.cxx LoseBar.cxx MessageManager.cxx MetaState.cxx OBJModel.cxx Primitives.cxx Random.cxx Sine.cxx Score.cxx ScoreRecordManager.cxx Swapper.cxx SparkleManager.cxx Spring.cxx SignManager.cxx String.cxx TextureLoader.cxx WinRecord.cxx X.cxx DrawBlocks.cxx DrawGarbage.cxx DrawCandy.cxx DrawScoreRecord.cxx DrawSwapper.cxx DrawExternalCandy.cxx DrawLevelLights.cxx DrawCountDown.cxx DrawMessages.cxx DrawWinRecord.cxx obj_block.cxx obj_garbage.cxx obj_garbage_thin_middle.cxx obj_garbage_thin_cap.cxx obj_garbage_thick_corner.cxx obj_garbage_thick_edge.cxx obj_garbage_thick_middle.cxx obj_garbage_small.cxx obj_sparkle.cxx obj_sign.cxx obj_swapper.cxx obj_external_candy.cxx obj_level_lights.cxx obj_messages.cxx obj_clock.cxx obj_name.cxx obj_score_record.cxx ActionRecorder.h Attack.h Block.h BlockManager.h CelebrationManager.h Clock.h ComboManager.h ComboTabulator.h Communicator.h ComputerPlayer.h ComputerPlayerAI.h Controller.h CountDownManager.h Creep.h Displayer.h Game.h Garbage.h GarbageFlavorImage.h GarbageGenerator.h GarbageManager.h GarbageQueue.h Grid.h LevelLights.h LightManager.h LoseBar.h MessageManager.h MetaState.h OBJModel.h Primitives.h Random.h Sine.h Score.h ScoreRecordManager.h Swapper.h SparkleManager.h Spring.h SignManager.h String.h TextureLoader.h WinRecord.h X.h Mode.h prefix.h prefix.c $(audio_sources)

# prefix sources compiled whether we use them or not

if WANT_GTK
  GUI_DIR = gtk-gui
  GUI_LIBS = gtk-gui/libgtkgui.a
endif

  NET_INCLUDES = -I../enet/include
  NET_LIBS = ../enet/libenet.a

SUBDIRS = $(GUI_DIR)

AM_CXXFLAGS = -DBR_PTHREAD=0 @DEBUG_CFLAGS@ @AUDIO_CFLAGS@ $(NET_INCLUDES) $(CFLAGS)

if WANT_BINRELOC
  PATH_INCLUDES = 
else
  PATH_INCLUDES = -DDATA_DIRECTORY='"${pkgdatadir}/"' \
      -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
    -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
		-DBINARY_DIRECTORY='"${bindir}"'
endif

INCLUDES = @PACKAGE_CFLAGS@ @DEBUG_INCLUDES@ ${PATH_INCLUDES} 

crack_attack_DEPENDENCIES = $(GUI_LIBS) $(NET_LIBS)
AM_LDFLAGS = ${all_libraries} $(NET_INCLUDES)
crack_attack_LDADD = ${X_PRE_LIBS} ${X_LIBS} ${X_EXTRA_LIBS} -lXi -lX11 $(GUI_LIBS) @PACKAGE_LIBS@ @AUDIO_LIBS@ $(NET_LIBS) -lglut
