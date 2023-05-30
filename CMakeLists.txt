project(AshEdit)
cmake_minimum_required(VERSION 2.6)

option(SUPPORT_GROUPS "Support for groups (from MO3)" off)
option(USE_STATIC_LIBRARIES "Build with static libraries" on)
option(DEBUG "Build for debugging" off)
option(DUMP "Build as a dumping tool (for developers)" off)

set(USER_INCLUDE_PATH "" CACHE FILEPATH "Any extra include directories you want")
set(USER_LIBRARY_PATH "" CACHE FILEPATH "Any extra library directories you want")

include_directories(${USER_INCLUDE_PATH})
link_directories(${USER_LIBRARY_PATH})

include_directories("include")

set(ASHEDIT_SRC src/main.cpp src/general.cpp)

if(MINGW)
	set(FLAGS "${FLAGS} -DTGUI2_STATIC -DALLEGRO_STATICLINK -DALLEGRO_UNSTABLE -std=c++11")
	set(LFLAGS "${LFLAGS} -static -static-libgcc -static-libstdc++")
	set(LFLAGS "${LFLAGS} -Wl,--subsystem,windows")
	if(DUMP)
		set(FLAGS "${FLAGS} -DDUMP")
	endif()

	if(SUPPORT_GROUPS)
		set(FLAGS "${FLAGS} -DSUPPORT_GROUPS")
	endif()

	list(APPEND ASHEDIT_SRC "misc/icons/icon.rc" "misc/versioninfo.rc")

	add_executable(AshEdit WIN32 ${ASHEDIT_SRC})

	set(LIBRARIES tgui2-static allegro_monolith-static freetype zlibstatic)

	set_target_properties(AshEdit PROPERTIES COMPILE_FLAGS "${FLAGS}")
	set_target_properties(AshEdit PROPERTIES LINK_FLAGS "${LFLAGS}")

	target_link_libraries(AshEdit ${LIBRARIES} opengl32 winmm psapi shlwapi gdiplus)
elseif(WIN32)
	set(FLAGS "${FLAGS} /MP")
	SET(FLAGS "${FLAGS} /DNTDDI_VERSION=0x06010000 /D_WIN32_WINNT=0x601")
	if(DUMP)
		set(FLAGS "${FLAGS} /DDUMP")
	endif()

	if(SUPPORT_GROUPS)
		set(FLAGS "${FLAGS} /DSUPPORT_GROUPS")
	endif()

	list(APPEND ASHEDIT_SRC "misc/icons/icon.rc" "misc/versioninfo.rc")

	add_executable(AshEdit WIN32 ${ASHEDIT_SRC})

	if(USE_STATIC_LIBRARIES)
		set(LIBRARIES tgui2-static allegro_monolith-static freetype zlibstatic)
		set(FLAGS "${FLAGS} /DTGUI2_STATIC")
	else()
		set(LIBRARIES tgui2 allegro_monolith freetype zlib)
	endif()

	set_target_properties(AshEdit PROPERTIES COMPILE_FLAGS "${FLAGS} /MT")

	target_link_libraries(AshEdit ${LIBRARIES} opengl32 winmm psapi shlwapi gdiplus)
else()
	set(FLAGS "${FLAGS} -Wno-narrowing")
	if(SUPPORT_GROUPS)
		set(FLAGS "${FLAGS} -DSUPPORT_GROUPS")
	endif()
	if(DEBUG)
		set(FLAGS "${FLAGS} -g -fsanitize=address -fsanitize=leak") 
	endif()
	if(NOT APPLE)
		set(FLAGS "${FLAGS} -std=c++0x -fpermissive -DALLEGRO_UNSTABLE")
	endif()

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS}")

	add_executable(AshEdit ${ASHEDIT_SRC})

	set(LIBRARIES tgui2-static allegro_monolith-static)

	if(APPLE)
		set_target_properties(AshEdit PROPERTIES LINK_FLAGS "-framework Foundation -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lz")
		set_target_properties(AshEdit PROPERTIES COMPILE_FLAGS "")
		list(APPEND LIBRARIES "freetype")
	else()
		include(FindPkgConfig)
		pkg_check_modules(GTK gtk+-2.0)
		pkg_check_modules(GT gthread-2.0)
		if(GTK_FOUND AND GT_FOUND)
			include_directories(${GTK_INCLUDE_DIRS} ${GT_INCLUDE_DIRS})
			list(APPEND LIBRARIES ${GTK_LIBRARIES} ${GT_LIBRARIES})
		else()
			message(FATAL_ERROR "ERROR: GTK development libraries not found!")
		endif()
		list(APPEND LIBRARIES png GL Xcursor Xrandr Xi Xinerama Xpm X11 pthread)
	endif()

	target_link_libraries(AshEdit ${LIBRARIES})
endif()
