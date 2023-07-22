project(game CXX)

set(GAME_SOURCES
        # aciparser
        aciparser/aci_parser.cpp
        # FX
        FX/Hermit.h
        FX/XTList.h
        FX/fxlabApplication.h
        FX/fxlabBodyInterface.cpp
        FX/fxlabBodyInterface.h
        FX/fxlabClientEffect.cpp
        FX/fxlabClientEffect.h
        FX/fxlabClientGeneral.cpp
        FX/fxlabClientGeneral.h
        FX/fxlabClientLauncher.cpp
        FX/fxlabClientLauncher.h
        FX/fxlabClientLightning.cpp
        FX/fxlabClientLightning.h
        FX/fxlabClientModel.cpp
        FX/fxlabClientModel.h
        FX/fxlabClientSound.cpp
        FX/fxlabClientSound.h
        FX/fxlabClientStuff.cpp
        FX/fxlabClientStuff.h
        FX/fxlabGeneral.cpp
        FX/fxlabGeneral.h
        FX/fxlabID.h
        FX/fxlabInterface.cpp
        FX/fxlabInterface.h
        FX/fxlabKey.cpp
        FX/fxlabKey.h
        FX/fxlabResource.cpp
        FX/fxlabResource.h
        FX/fxlabServerArcane.cpp
        FX/fxlabServerArcane.h
        FX/fxlabServerGeneral.cpp
        FX/fxlabServerGeneral.h
        FX/fxlabServerStuff.cpp
        FX/fxlabServerStuff.h
        FX/fxlabSprite.cpp
        FX/fxlabSprite.h
        # RUNTIME
        RUNTIME/ArcaneDefine.h
        RUNTIME/Chtree.cpp
        RUNTIME/Chtree.h
        RUNTIME/KEYS.CPP
        RUNTIME/KEYS.H
        RUNTIME/LocalVersion.cpp
        RUNTIME/LocalVersion.h
        RUNTIME/Scripts
        RUNTIME/SpriteDispatcher.cpp
        RUNTIME/SpriteDispatcher.h
        RUNTIME/TGA.H
        RUNTIME/TrackDispatcher.cpp
        RUNTIME/TrackDispatcher.h
        RUNTIME/TrackRecorder.cpp
        RUNTIME/TrackRecorder.h
        RUNTIME/ai_arcane.h
        RUNTIME/ai_arcane_prm.h
        RUNTIME/arcane.cpp
        RUNTIME/arcane.h
        RUNTIME/arcane_menu.cpp
        RUNTIME/arcane_menu.h
        RUNTIME/arcane_menu_d3d.cpp
        RUNTIME/arcane_menu_d3d.h
        RUNTIME/cdcheck.h
        RUNTIME/ctl_point.cpp
        RUNTIME/ctl_point.h
        RUNTIME/demo_dispatcher.cpp
        RUNTIME/demo_dispatcher.h
        RUNTIME/fcontainer.cpp
        RUNTIME/fcontainer.h
        RUNTIME/intro.cpp
        RUNTIME/intro.h
        RUNTIME/intro_movie.cpp
        RUNTIME/intro_movie.h
        RUNTIME/intro_movie_id.h
        RUNTIME/mch_common.h
        RUNTIME/mch_list.h
        RUNTIME/mch_rto.H
        RUNTIME/mechosoma.cpp
        RUNTIME/mechosoma.h
        RUNTIME/og_buffer.cpp
        RUNTIME/online_game.cpp
        RUNTIME/online_game.h
        RUNTIME/parts_pool.cpp
        RUNTIME/parts_pool.h
        RUNTIME/psi_model.h
        RUNTIME/race.cpp
        RUNTIME/race.h
        RUNTIME/resource.cpp
        RUNTIME/setup.cpp
        RUNTIME/tga.cpp
        RUNTIME/timer.cpp
        RUNTIME/track.cpp
        RUNTIME/track.h
        RUNTIME/wininet_api_port.cpp
        RUNTIME/wininet_api.h
        RUNTIME/xINI.cpp
        RUNTIME/xINI_File.cpp
        RUNTIME/xINI_File.h
        # IVisGeneric
        IVisGeneric/IVisGeneric.h
        IVisGeneric/M3d_id.h
        IVisGeneric/UMATH.H
        IVisGeneric/Unknown.h
        IVisGeneric/VisGenericDefine.h
        IVisGeneric/World_id.h
        # Sound
        Sound/audio_file.cpp
        Sound/audio_file.h
        Sound/music_player.cpp
        Sound/music_player.h
        Sound/sound.cpp
        Sound/sound.h
        Sound/sound_api.cpp
        Sound/sound_api.h
        Sound/sound_manager.cpp
        Sound/sound_manager.h
        # Terra
        Terra/JPEG_FM.cpp
        Terra/LAND.CPP
        Terra/RENDER.H
        Terra/VMAP.CPP
        Terra/VMAP.H
        Terra/ch_points.cpp
        Terra/ch_points.h
        Terra/land.h
        Terra/non_rotate_scan_min_full.cpp
        Terra/sur_scr.cpp
        Terra/sur_scr.h
        # VisGeneric
        VisGeneric/AnimChannelNode.cpp
        VisGeneric/AnimChannelNode.h
        VisGeneric/Base.h
        VisGeneric/BaseClass.h
        VisGeneric/BaseDefine.h
        VisGeneric/BaseLight.h
        VisGeneric/BaseObject.cpp
        VisGeneric/BaseObject.h
        VisGeneric/BasePolyGrid.cpp
        VisGeneric/BasePolyGrid.h
        VisGeneric/BaseReflection.cpp
        VisGeneric/BaseReflection.h
        VisGeneric/BaseTrail.h
        VisGeneric/BoundingBox.h
        VisGeneric/Camera.cpp
        VisGeneric/Camera.h
        VisGeneric/DefineWSC.h
        VisGeneric/Dispatcher.cpp
        VisGeneric/Dispatcher.h
        VisGeneric/DrawPolygonShade.h
        VisGeneric/Dummy.cpp
        VisGeneric/Dummy.h
        VisGeneric/File3ds.cpp
        VisGeneric/File3ds.h
        VisGeneric/Frame.cpp
        VisGeneric/Frame.h
        VisGeneric/GameClient.cpp
        VisGeneric/GameClient.h
        VisGeneric/HashStringGroup.h
        VisGeneric/IncTerra.h
        VisGeneric/Levin.cpp
        VisGeneric/Levin.h
        VisGeneric/M3d_effects.h
        VisGeneric/Material.cpp
        VisGeneric/Material.h
        VisGeneric/Math3d.cpp
        VisGeneric/Math3d.h
        VisGeneric/Maths.cpp
        VisGeneric/Maths.h
        VisGeneric/Mesh3ds.cpp
        VisGeneric/Mesh3ds.h
        VisGeneric/Object.cpp
        VisGeneric/Object.h
        VisGeneric/Object3d.cpp
        VisGeneric/Object3d.h
        VisGeneric/PlayWSC.cpp
        VisGeneric/PlayWSC.h
        VisGeneric/PolyGrid.cpp
        VisGeneric/PolyGrid.h
        VisGeneric/PolyMgr.cpp
        VisGeneric/PolyMgr.h
        VisGeneric/PolyMgrD3D.cpp
        VisGeneric/PolyMgrDraw.cpp
        VisGeneric/PolyMgrTex.cpp
        VisGeneric/RenderDevice.cpp
        VisGeneric/RenderDevice.h
        VisGeneric/Scene.cpp
        VisGeneric/Scene.h
        VisGeneric/SurmapClient.cpp
        VisGeneric/SurmapClient.h
        VisGeneric/TERRA.H
        VisGeneric/TexMgr.cpp
        VisGeneric/TexMgr.h
        VisGeneric/TgaServ.cpp
        VisGeneric/TgaServ.h
        VisGeneric/TileMap.cpp
        VisGeneric/TileMap.h
        VisGeneric/TileWater.cpp
        VisGeneric/TileWater.h
        VisGeneric/UnkLibrary.h
        VisGeneric/Unknown.cpp
        VisGeneric/VisGeneric.cpp
        VisGeneric/VisGeneric.dsp
        VisGeneric/VisGeneric.h
        VisGeneric/VisGeneric.vcproj
        VisGeneric/WinVideo_port.cpp
        VisGeneric/WinVideo.h
        VisGeneric/XGR.h
        VisGeneric/drawPolygonShade.cpp
        VisGeneric/vgBaseObject.cpp
        VisGeneric/vgCamera.cpp
        VisGeneric/vgGraph.cpp
        VisGeneric/vgObject.cpp
        VisGeneric/vgOmni.cpp
        VisGeneric/vgUnknown.cpp
        VisGeneric/vgWorld.cpp
        VisGeneric/xgr.cpp
        # Graph3d
        VisGeneric/Graph3d/Graph3d_Direct3d.cpp
        VisGeneric/Graph3d/Graph3d_Direct3d.h
        VisGeneric/Graph3d/IGraph3d.cpp
        VisGeneric/Graph3d/IGraph3d.h
        # Md3d
        VisGeneric/md3d/d3dSprites.cpp
        # PluginMAX
        VisGeneric/PluginMAX/src/AnimationMaterial.cpp
        VisGeneric/PluginMAX/src/AnimationMesh.cpp
        VisGeneric/PluginMAX/src/ChannelAnimation.cpp
        VisGeneric/PluginMAX/src/ChannelMapping.cpp
        VisGeneric/PluginMAX/src/DefMeshFile.cpp
        VisGeneric/PluginMAX/src/HelperObject.cpp
        VisGeneric/PluginMAX/src/LodObject.cpp
        VisGeneric/PluginMAX/src/MaterialObject.cpp
        VisGeneric/PluginMAX/src/MeshFile.cpp
        VisGeneric/PluginMAX/src/NodeObject.cpp
        VisGeneric/PluginMAX/src/ObjectMesh.cpp
        VisGeneric/PluginMAX/src/SceneMesh.cpp
        VisGeneric/PluginMAX/src/StreamBuffer.cpp
        VisGeneric/PluginMAX/src/StreamBuffer.h
        # XMath
        XMath/XMath.cpp
        XMath/xmath.h
        # XTool
        XTool/filesystem.cpp
        XTool/filesystem.h
        XTool/port.cpp
        XTool/port.h
        XTool/xtool.h
        XTool/inzip.cpp
        XTool/outzip.cpp
        XTool/treezip.cpp
        XTool/zip_headers.h
        XTool/zip_resource.cpp
        XTool/xstream.cpp
        XTool/xbcnvin.cpp
        XTool/xbcnvout.cpp
        XTool/xbcore.cpp
        XTool/xbsearch.cpp
        XTool/xconsole.cpp
        XTool/xkey.cpp
        XTool/xgraph.cpp
        XTool/xgraph.h
        XTool/xerrhand.cpp
        XTool/xerrhand.h
        XTool/xrecorder.cpp
        XTool/xutil.cpp
        XTool/xsound.cpp
        XTool/Statistics.cpp
        XTool/xnames.cpp
        # Xreal
        Xreal/AnimalArcansPrm.h
        Xreal/AnimalsPrm.h
        Xreal/AnimatedBody.h
        Xreal/ArcaneStatistics.h
        Xreal/ArcansOfMechos.cpp
        Xreal/ArcansPrm.h
        Xreal/Archimed.cpp
        Xreal/Ball.cpp
        Xreal/Ball.h
        Xreal/BaseObject.h
        Xreal/Body.cpp
        Xreal/Body.h
        Xreal/BodyDispatcher.cpp
        Xreal/BodyDispatcher.h
        Xreal/BodyFactory.cpp
        Xreal/BodyFactory.h
        Xreal/BodyFeatureHT.h
        Xreal/BonusMan.cpp
        Xreal/BonusMan.h
        Xreal/Brevno.cpp
        Xreal/Brevno.h
        Xreal/Bubble.cpp
        Xreal/Bubble.h
        Xreal/Butterfly.cpp
        Xreal/Butterfly.h
        Xreal/CameraClipping.cpp
        Xreal/CameraDispatcher.cpp
        Xreal/CameraDispatcher.h
        Xreal/CameraPrm.h
        Xreal/Cannon.cpp
        Xreal/Cannon.h
        Xreal/Clown.cpp
        Xreal/Clown.h
        Xreal/CollisionHandler.cpp
        Xreal/CollisionHandler.hpp
        Xreal/Contact.cpp
        Xreal/Contact.h
        Xreal/ControlledObject.cpp
        Xreal/ControlledObject.h
        Xreal/CrawlingObject.cpp
        Xreal/CrawlingObject.h
        Xreal/Crystal.cpp
        Xreal/Crystal.h
        Xreal/CustomFieldSources.cpp
        Xreal/CustomFieldSources.h
        Xreal/CycledMath.h
        Xreal/Debris.cpp
        Xreal/Debris.h
        Xreal/DebugPrm.h
        Xreal/Dog.h
        Xreal/Dragon.cpp
        Xreal/Dragon.h
        Xreal/Dynamics.cpp
        Xreal/EvilEye.cpp
        Xreal/EvilEye.h
        Xreal/FieldGrid.cpp
        Xreal/FieldGrid.h
        Xreal/FieldSource.cpp
        Xreal/FieldSource.h
        Xreal/FirePoint.cpp
        Xreal/FirePoint.h
        Xreal/FixedPathBody.cpp
        Xreal/FixedPathBody.h
        Xreal/FlyingObject.cpp
        Xreal/FlyingObject.h
        Xreal/Forest_world.cpp
        Xreal/Frog.cpp
        Xreal/Frog.h
        Xreal/Grid2D.h
        Xreal/GrowingBody.cpp
        Xreal/GrowingBody.h
        Xreal/Handle.h
        Xreal/HydrophobicObject.cpp
        Xreal/HydrophobicObject.h
        Xreal/Joystick
        Xreal/LCP_ContactSolver.cpp
        Xreal/LCP_ContactSolver.h
        Xreal/LCP_Solver.cpp
        Xreal/LCP_Solver.h
        Xreal/LCP_frictional.cpp
        Xreal/Line2D.h
        Xreal/LittleSpider.cpp
        Xreal/LittleSpider.h
        Xreal/Mechos.cpp
        Xreal/Mechos.h
        Xreal/MechosEffects.cpp
        Xreal/MechosForSale.cpp
        Xreal/MechosForSale.h
        Xreal/MechosPrm.h
        Xreal/MemoryHeap.cpp
        Xreal/MemoryHeap.h
        Xreal/Meteorite.cpp
        Xreal/Meteorite.h
        Xreal/Moutain_world.cpp
        Xreal/MultiBodyCollision.cpp
        Xreal/MultibodyContact.h
        Xreal/OwnerProtection.h
        Xreal/Params.cpp
        Xreal/Params.h
        Xreal/Part.cpp
        Xreal/Part.h
        Xreal/Penguin.h
        Xreal/PersonageDifferetiationPrm.h
        Xreal/Pig.cpp
        Xreal/Pig.h
        Xreal/PointControlledObject.cpp
        Xreal/PointControlledObject.h
        Xreal/Polyhedron.cpp
        Xreal/Polyhedron.h
        Xreal/PolyhedronClip.cpp
        Xreal/PolyhedronLibrary.cpp
        Xreal/PolyhedronLibrary.h
        Xreal/QHull
        Xreal/QuatMath.h
        Xreal/ResourceDispatcher.cpp
        Xreal/ResourceDispatcher.h
        Xreal/RungeKutta4.h
        Xreal/SST_Reader.h
        Xreal/Shark.h
        Xreal/Ship.cpp
        Xreal/Ship.h
        Xreal/SimpleClip.cpp
        Xreal/SimpleClip.h
        Xreal/SimpleTrack.h
        Xreal/SmartMissile.cpp
        Xreal/SmartMissile.h
        Xreal/SmartStone.cpp
        Xreal/SmartStone.h
        Xreal/Spider.cpp
        Xreal/Spider.h
        Xreal/StaticBonus.cpp
        Xreal/StaticBonus.h
        Xreal/StatisticalAnalizator.h
        Xreal/StdAfx.cpp
        Xreal/StdAfx.h
        Xreal/Stone.cpp
        Xreal/Stone.h
        Xreal/Suicide.cpp
        Xreal/Suicide.h
        Xreal/SynchroTimer.h
        Xreal/Timers.h
        Xreal/TitaniumBall.cpp
        Xreal/TitaniumBall.h
        Xreal/TnT
        Xreal/Tomato.cpp
        Xreal/Tomato.h
        Xreal/TrackIterator.cpp
        Xreal/TrackIterator.h
        Xreal/TrackSpline.cpp
        Xreal/TrackSpline.h
        Xreal/VectorOperations.inl
        Xreal/VectorOperationsAligned.inl
        Xreal/Vitamina.cpp
        Xreal/Vitamina.hpp
        Xreal/Water_world.cpp
        Xreal/Whale.cpp
        Xreal/Whale.h
        Xreal/WorldDispatcher.cpp
        Xreal/Worm.cpp
        Xreal/Worm.h
        Xreal/Xreal.cpp
        Xreal/Xreal.h
        Xreal/XrealWorlds.h
        Xreal/Xreal_utl.cpp
        Xreal/Xreal_utl.h
        Xreal/fcm.inl
        Xreal/gen_animalarcans.cpp
        Xreal/gen_animals.cpp
        Xreal/gen_arcans.cpp
        Xreal/gen_camera.cpp
        Xreal/gen_debug.cpp
        Xreal/gen_game.cpp
        Xreal/gen_game.h
        Xreal/gen_mechos.cpp
        Xreal/gen_personagedifferntiation.cpp
        Xreal/gen_xreal.cpp
        Xreal/gen_xreal.h
        Xreal/pt.cpp
        # Xreal/Joystick
        Xreal/Joystick/XJoystick_port.cpp
        # iScreen
        iScreen/SCRIPTS/SCR_DEFS.H
        iScreen/ACS_CORE.CPP
        iScreen/ACS_EXT.CPP
        iScreen/HFONT.CPP
        iScreen/HFONT.H
        iScreen/I-World.cpp
        iScreen/acs_quant.cpp
        iScreen/hbm.cpp
        iScreen/hbm.h
        iScreen/iScript.cpp
        iScreen/iText.cpp
        iScreen/savegame.cpp)
set(LIBRARY_NAME game)

include_directories(aciparser)
include_directories(FX)
include_directories(IVisGeneric)
include_directories(Sound)
include_directories(Terra)
include_directories(RUNTIME)
include_directories(RUNTIME/Scripts)
include_directories(VisGeneric)
include_directories(VisGeneric/md3d)
include_directories(VisGeneric/Graph3d)
include_directories(VisGeneric/Scripts)
include_directories(VisGeneric/UTIL)
include_directories(VisGeneric/PluginMAX/inc)
include_directories(VisGeneric/PluginMAX/src)
include_directories(XMath)
include_directories(XTool)
include_directories(Xreal)
include_directories(Xreal/Joystick)
include_directories(Xreal/TnT)
include_directories(iScreen)
include_directories(iScreen/SCRIPTS)

if (APPLE)
    include_directories(../3rdparty/libjpeg-turbo/include)
    link_directories(../3rdparty/libjpeg-turbo/lib)
endif()

add_library(${LIBRARY_NAME} ${GAME_SOURCES})
target_link_libraries(${LIBRARY_NAME} PUBLIC 3dsftk3 iniparser jpeg)

set_target_properties(${LIBRARY_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

include_directories(../3rdparty/sokol)

include_directories(../3rdparty/SDL-2.26.5/include)
link_directories(../3rdparty/SDL-2.26.5/lib)

include_directories(../3rdparty/FFmpeg-n4.3.6/include)
link_directories(../3rdparty/FFmpeg-n4.3.6/lib)

include_directories(../3rdparty/miniaudio)

find_package(ZLIB REQUIRED)
find_package(Iconv REQUIRED)

if (WIN32)
    set(BCRYPT_LIBRARY bcrypt)
    set(WINDOWS_COMMON_LIBRARIES SetupAPI Imm32 Version Winmm Cfgmgr32)
endif(WIN32)

set(COMMON_APPLICATION_LIBRARIES avformat avcodec avutil ZLIB::ZLIB Iconv::Iconv ${BCRYPT_LIBRARY} SDL2 ${WINDOWS_COMMON_LIBRARIES})

set(APPLICATION_SOURCES
    app/back_buffer.cpp
    app/back_buffer.h
    app/main.cpp
    app/md3d.cpp
    app/offscreen_buffer.cpp
    app/offscreen_buffer.h
    app/render_state.h
    app/renderer.cpp
    app/renderer.h
    app/sokol-shader.h
    app/texture_manager.cpp
    app/texture_manager.h
    app/vertex_type.h
)

if (MINGW)
    add_executable(${APPLICATION_NAME} ${APPLICATION_SOURCES})
    target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME} ${COMMON_APPLICATION_LIBRARIES})
    target_link_options(${APPLICATION_NAME} PRIVATE -static-libgcc -static-libstdc++)
    add_custom_command(TARGET ${APPLICATION_NAME} POST_BUILD
            COMMAND cp -v /mingw64/bin/libjpeg-8.dll "${CMAKE_CURRENT_BINARY_DIR}"
            COMMAND cp -v /mingw64/bin/libwinpthread-1.dll "${CMAKE_CURRENT_BINARY_DIR}")
elseif (LINUX)
    add_executable(${APPLICATION_NAME} ${APPLICATION_SOURCES})
    target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME} ${COMMON_APPLICATION_LIBRARIES}
            X11 Xi Xcursor GL dl pthread)
elseif (APPLE)
    add_executable(${APPLICATION_NAME} MACOSX_BUNDLE ${APPLICATION_SOURCES})
    target_compile_options(${APPLICATION_NAME} PRIVATE -x objective-c++)
    target_link_libraries(${APPLICATION_NAME} PRIVATE stdc++
            "-framework CoreFoundation" "-framework Cocoa" "-framework QuartzCore" "-framework OpenGL" 
            "-framework CoreAudio" "-framework AudioToolbox" "-framework ForceFeedback" "-framework CoreHaptics"
            "-framework GameController" "-framework IOKit" "-framework Carbon" "-framework Metal"
            objc ${LIBRARY_NAME} ${COMMON_APPLICATION_LIBRARIES})
endif(MINGW)
