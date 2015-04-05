cmake_minimum_required (VERSION 2.6)
project (vpercnode)
set(_ECLIPSE_VERSION "4.2")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.54.0 COMPONENTS system)

add_executable(vpercnode vpercnode.cpp)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
	#add_executable(vpercnode vpercnode.cpp)
    target_link_libraries(vpercnode ${Boost_LIBRARIES})
endif()
