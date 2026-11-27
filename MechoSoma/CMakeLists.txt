project(game CXX)

if (CMAKE_BUILD_TYPE MATCHES Debug AND NOT MSVC)
    add_definitions(-D_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

set(GAME_SOURCES
    # ParserC
    ParserC/math_exp.cpp
    ParserC/prepare.cpp
    ParserC/script_err.cpp
    ParserC/script.cpp
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
    Xreal/Joystick/XJoystick.cpp
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
    iScreen/savegame.cpp
    # application
    app/main.cpp
    app/render_backend.h
    app/render_state.h
    app/renderer.cpp
    app/renderer.h
    app/texture_manager.cpp
    app/window.cpp
    app/window.h
    # widescreen fix
    RUNTIME/aspect_ratio.cpp
    RUNTIME/aspect_ratio.h)

# The SDL GPU renderer's shaders, compiled with the toolchain the top-level
# CMakeLists.txt picked, into one generated header (cmake/compile_shaders.cmake).
# <name>:<stage> for app/sdl_gpu/shaders/<name>.hlsl.
set(GPU_SHADERS
    scene_vs:vertex
    mesh_vs:vertex
    tile_vs:vertex
    scene_fs:fragment
    shadow_vs:vertex
    shadow_fs:fragment
    quad_vs:vertex
    back_buffer_fs:fragment
    flush_vs:vertex
    flush_fs:fragment)
set(GPU_SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/app/sdl_gpu/shaders")
set(GPU_SHADER_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(GPU_SHADER_HEADER "${GPU_SHADER_GENERATED_DIR}/sdl_gpu_shaders.h")
set(GPU_SHADER_SOURCES "${GPU_SHADER_DIR}/common.hlsl")
foreach (_shader IN LISTS GPU_SHADERS)
    string(REGEX REPLACE ":.*" "" _name "${_shader}")
    list(APPEND GPU_SHADER_SOURCES "${GPU_SHADER_DIR}/${_name}.hlsl")
endforeach ()
if (GPU_SHADER_HEADER_PREBUILT)
    # Compiled by another build (checked in the top CMakeLists): copied in, and copied
    # again if it changes.
    configure_file("${GPU_SHADER_HEADER_PREBUILT}" "${GPU_SHADER_HEADER}" COPYONLY)
else ()
    # | rather than ;, which would split the argument (see the script).
    string(REPLACE ";" "|" _shader_list "${GPU_SHADERS}")
    if (SHADER_COMPILER MATCHES "TARGET_FILE")
        set(_shader_compiler_target shadercross)
    endif ()
    add_custom_command(
        OUTPUT "${GPU_SHADER_HEADER}"
        COMMAND "${CMAKE_COMMAND}"
            "-DOUTPUT=${GPU_SHADER_HEADER}"
            "-DMODE=${SHADER_MODE}"
            "-DCOMPILER=${SHADER_COMPILER}"
            "-DFORMAT=${SHADER_FORMAT}"
            "-DSOURCE_DIR=${GPU_SHADER_DIR}"
            "-DWORK_DIR=${GPU_SHADER_GENERATED_DIR}/shaders"
            "-DSHADERS=${_shader_list}"
            -P "${MOONSHINE_RUNNERS_SOURCE_DIR}/cmake/compile_shaders.cmake"
        DEPENDS ${GPU_SHADER_SOURCES} ${_shader_compiler_target}
            "${MOONSHINE_RUNNERS_SOURCE_DIR}/cmake/compile_shaders.cmake"
        COMMENT "Compiling the SDL GPU shaders to ${SHADER_FORMAT}"
        VERBATIM)
    # The header alone (cmake --build <dir> --target gpu_shaders), for a build that takes
    # it prebuilt. The game depends on this target rather than listing the header among
    # its sources: an output in two targets could be made twice at once.
    add_custom_target(gpu_shaders DEPENDS "${GPU_SHADER_HEADER}")
endif ()

list(APPEND GAME_SOURCES
    app/sdl_gpu/backend.cpp
    app/sdl_gpu/backend.h
    app/sdl_gpu/shaders.cpp
    app/sdl_gpu/shaders.h)
include_directories("${GPU_SHADER_GENERATED_DIR}")

include_directories(app)
include_directories(FX)
include_directories(IVisGeneric)
include_directories(Sound)
include_directories(Terra)
include_directories(RUNTIME)
include_directories(RUNTIME/Scripts)
include_directories(ParserC)
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

# SDL 3 is fetched and built by the top-level CMakeLists.txt. SDL3::SDL3-static brings
# its include directories and the system libraries it needs.

include_directories(../3rdparty/miniaudio)
include_directories(../3rdparty/stb)

# On Windows the top-level CMakeLists.txt builds zlib.
if (NOT TARGET ZLIB::ZLIB)
    find_package(ZLIB REQUIRED)
endif ()

if (WIN32)
    set(BCRYPT_LIBRARY bcrypt)
    set(WINDOWS_COMMON_LIBRARIES SetupAPI Imm32 Version Winmm Cfgmgr32 dbghelp user32)
endif(WIN32)

if (STEAM_VERSION)
    add_definitions(-DSTEAM_VERSION)
    include_directories(../3rdparty/steamworks_sdk_158/include)

    if (WIN32)
        link_directories(../3rdparty/steamworks_sdk_158/lib/win64)
        set(STEAM_LIBRARY steam_api64)
    elseif (LINUX)
        link_directories(../3rdparty/steamworks_sdk_158/lib/linux64)
        set(STEAM_LIBRARY steam_api)
    elseif (APPLE)
        link_directories(../3rdparty/steamworks_sdk_158/lib/osx)
        set(STEAM_LIBRARY steam_api)
    else()
        message(FATAL_ERROR "Steam version supports only Windows, Linux and macOS platforms")
    endif()
endif()

set(COMMON_APPLICATION_LIBRARIES 3dsftk3 iniparser mpcdec ZLIB::ZLIB ${BCRYPT_LIBRARY} SDL3::SDL3-static ${WINDOWS_COMMON_LIBRARIES} ${STEAM_LIBRARY})

if (WIN32)
    if (NOT MSVC)
        message(FATAL_ERROR "The Windows build needs Visual C++ (MSVC)")
    endif ()
    # A GUI program, so no console window. Without SDL_main.h there is no WinMain, so it
    # starts at the CRT's main() entry point.
    add_executable(${APPLICATION_NAME} WIN32 ${GAME_SOURCES})
    target_link_libraries(${APPLICATION_NAME} ${COMMON_APPLICATION_LIBRARIES})
    # The main thread gets 8 MiB of stack, as on Linux and macOS (MSVC's default is 1 MiB,
    # MinGW's was 2 MiB). Release builds drop unused code and don't link incrementally.
    set(OPTIMIZED_CONFIG $<CONFIG:RelWithDebInfo,Release>)
    target_link_options(${APPLICATION_NAME} PRIVATE /ENTRY:mainCRTStartup /STACK:8388608
            $<${OPTIMIZED_CONFIG}:/INCREMENTAL:NO> $<${OPTIMIZED_CONFIG}:/OPT:REF> $<${OPTIMIZED_CONFIG}:/OPT:ICF>)
elseif (LINUX)
    add_executable(${APPLICATION_NAME} ${GAME_SOURCES})
    target_link_libraries(${APPLICATION_NAME} ${COMMON_APPLICATION_LIBRARIES}
            X11 Xi Xcursor dl pthread)
    # Export symbols so backtrace_symbols() can name functions in crash reports.
    target_link_options(${APPLICATION_NAME} PRIVATE -rdynamic)
    if (STEAM_VERSION)
        # The Steam depot puts libsteam_api.so beside the program, so that is where the
        # program looks for it: an RPATH of $ORIGIN alone, in the build tree too, as the
        # build tree's program is the one shipped. The default named the SDK folder on the
        # machine that built it. The library is copied beside it here for a local run.
        set_target_properties(${APPLICATION_NAME} PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "$ORIGIN")
        add_custom_command(TARGET ${APPLICATION_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/steamworks_sdk_158/lib/linux64/libsteam_api.so"
                "$<TARGET_FILE_DIR:${APPLICATION_NAME}>"
            VERBATIM)
    endif ()
elseif (APPLE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    add_executable(${APPLICATION_NAME} MACOSX_BUNDLE ${GAME_SOURCES})
    set_property(TARGET ${APPLICATION_NAME} PROPERTY MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Info.plist.in")
    target_link_libraries(${APPLICATION_NAME} PRIVATE
        "-framework AudioToolbox"
        "-framework Carbon"
        "-framework Cocoa"
        "-framework CoreAudio"
        "-framework CoreFoundation"
        "-framework CoreHaptics"
        "-framework ForceFeedback"
        "-framework GameController"
        "-framework IOKit"
        "-framework Metal"
        "-framework QuartzCore"
        ${COMMON_APPLICATION_LIBRARIES}
    )

    if (STEAM_VERSION)
        set(APP_BUNDLE_CONTENTS_DIR "${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_NAME}.app/Contents")
        set(APP_BUNDLE_FRAMEWORKS_DIR "${APP_BUNDLE_CONTENTS_DIR}/Frameworks")
        set(STEAM_SDK_DLIB "../3rdparty/steamworks_sdk_158/lib/osx/libsteam_api.dylib")
        file(COPY ${STEAM_SDK_DLIB} DESTINATION ${APP_BUNDLE_FRAMEWORKS_DIR})

        set(LOADER_PATH "@loader_path/libsteam_api.dylib")
        set(EXECUTABLE_PATH "@executable_path/../Frameworks/libsteam_api.dylib")
        add_custom_command(TARGET ${APPLICATION_NAME}
            POST_BUILD COMMAND
            install_name_tool -change "${LOADER_PATH}" "${EXECUTABLE_PATH}" "${APP_BUNDLE_CONTENTS_DIR}/MacOS/${APPLICATION_NAME}"
        )
    endif()
endif(WIN32)

if (TARGET ${APPLICATION_NAME})
    # The game links SDL, so crash reports can use SDL_ShowSimpleMessageBox on
    # every desktop platform. ScriptTool has no SDL and keeps the log/stdout path.
    target_compile_definitions(${APPLICATION_NAME} PRIVATE XERRH_USE_SDL)
    if (TARGET gpu_shaders)
        add_dependencies(${APPLICATION_NAME} gpu_shaders)
    endif()
endif()
