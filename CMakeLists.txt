cmake_minimum_required (VERSION 2.8)
project (vpercnode)
set(_ECLIPSE_VERSION "4.2")

include_directories(src)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(vpercnode src/vpercnode.cpp src/MoveTo.cpp src/JointInterpolateAngle.cpp src/Avatar.cpp)
#target_compile_features(vpercnode PRIVATE cxx_range_for)
if(GCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
endif()

set(Boost_USE_STATIC_LIBS FALSE) 
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME FALSE) 
add_definitions( -DBOOST_ALL_NO_LIB ) # avoid LNK1104 libboost_thread on Windows: http://stackoverflow.com/a/28902261/122441
find_package(Boost 1.54.0 COMPONENTS system log filesystem thread REQUIRED) #COMPONENTS system log filesystem thread 
message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Library Dirs: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIRS}) 
link_directories(${Boost_LIBRARY_DIRS})
#add_executable(vpercnode vpercnode.cpp)
target_link_libraries(vpercnode ${Boost_LIBRARIES}) # not required on Windows??? dunno in Linux?

include_directories(librabbitmq SimpleAmqpClient)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib_linux64/)
if(MSVC)
	# assume Win64
	target_link_libraries(vpercnode ${CMAKE_CURRENT_SOURCE_DIR}/lib_win64/rabbitmq.4.lib ${CMAKE_CURRENT_SOURCE_DIR}/lib_win64/SimpleAmqpClient.2.lib)
	file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} CMAKE_CURRENT_SOURCE_DIR_WIN)
	add_custom_command(TARGET vpercnode POST_BUILD COMMAND xcopy ARGS /y ${CMAKE_CURRENT_SOURCE_DIR_WIN}\\lib_win64\\*.dll "$(OutDir)")
	add_custom_command(TARGET vpercnode POST_BUILD COMMAND xcopy ARGS /y ${BOOST_LIBRARYDIR}\\boost_*.dll "$(OutDir)")
else()
	# assume Linux64
	target_link_libraries(vpercnode ${CMAKE_CURRENT_SOURCE_DIR}/lib_linux64/librabbitmq.so ${CMAKE_CURRENT_SOURCE_DIR}/lib_linux64/libSimpleAmqpClient.so)
endif()

# http://stackoverflow.com/q/10688549/122441
set(MULTITHREADED_BUILD 8)
if(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
	message(STATUS ${CMAKE_BUILD_TOOL})
	#set(CMAKE_MAKE_COMMAND "${CMAKE_MAKE_PROGRAM} -j${MULTITHREADED_BUILD}")
	#message(STATUS "Added arguments to CMAKE_BUILD_TOOL: ${CMAKE_MAKE_PROGRAM}")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()
