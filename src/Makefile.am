bin_PROGRAMS = crack-attack

crack_attack_SOURCES = Attack.cxx Block.cxx BlockManager.cxx CelebrationManager.cxx Clock.cxx ComboManager.cxx ComboTabulator.cxx Communicator.cxx Controller.cxx CountDownManager.cxx Creep.cxx Displayer.cxx Game.cxx Garbage.cxx GarbageFlavorImage.cxx GarbageGenerator.cxx GarbageManager.cxx Grid.cxx LevelLights.cxx LightManager.cxx LoseBar.cxx MessageManager.cxx MetaState.cxx Random.cxx Sine.cxx Score.cxx ScoreRecordManager.cxx Swapper.cxx SparkleManager.cxx Spring.cxx SignManager.cxx String.cxx TextureLoader.cxx WinRecord.cxx X.cxx DrawBlocks.cxx DrawGarbage.cxx DrawCandy.cxx DrawScoreRecord.cxx DrawSwapper.cxx DrawExternalCandy.cxx DrawLevelLights.cxx DrawCountDown.cxx DrawMessages.cxx DrawWinRecord.cxx obj_block.cxx obj_garbage.cxx obj_garbage_thin_middle.cxx obj_garbage_thin_cap.cxx obj_garbage_thick_corner.cxx obj_garbage_thick_edge.cxx obj_garbage_thick_middle.cxx obj_garbage_small.cxx obj_sparkle.cxx obj_sign.cxx obj_swapper.cxx obj_external_candy.cxx obj_level_lights.cxx obj_messages.cxx obj_clock.cxx obj_name.cxx obj_score_record.cxx Attack.h Block.h BlockManager.h CelebrationManager.h Clock.h ComboManager.h ComboTabulator.h Communicator.h Controller.h CountDownManager.h Creep.h Displayer.h Game.h Garbage.h GarbageFlavorImage.h GarbageGenerator.h GarbageManager.h Grid.h LevelLights.h LightManager.h LoseBar.h MessageManager.h MetaState.h Random.h Sine.h Score.h ScoreRecordManager.h Swapper.h SparkleManager.h Spring.h SignManager.h String.h TextureLoader.h WinRecord.h X.h

CXXFLAGS = -O6 -s
INCLUDES = -DNDEBUG -DDATA_DIRECTORY='"${pkgdatadir}/"'

LDFLAGS = ${all_libraries}
LDADD = ${X_PRE_LIBS} ${X_LIBS} ${X_EXTRA_LIBS} -lGL -lGLU -lglut -lXi -lX11 -lXmu
