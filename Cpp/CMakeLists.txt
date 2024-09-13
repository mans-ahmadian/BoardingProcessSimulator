cmake_minimum_required(VERSION 3.24 FATAL_ERROR )

if(WIN32)
	File(TO_CMAKE_PATH $ENV{VCPKG_ROOT} VCPKG_ROOT)
	message(STATUS ${VCPKG_ROOT})
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	set(VCPKG_TARGET_TRIPLET "x64-windows-static-ffmpeg" CACHE STRING "")
else(WIN32)
	if(NOT DEFINED ENV{VCPKG_ROOT})
		message(WARNING "Please set VCPKG_ROOT to the root of the vcpkg installation.")
	endif()
	file(TO_CMAKE_PATH $ENV{VCPKG_ROOT} VCPKG_ROOT)
	message(STATUS ${VCPKG_ROOT})
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
	set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")
endif(WIN32)

project(BoardingProcess)

# setup msvc
if (MSVC)
    add_definitions (-D_USE_MATH_DEFINES)
	add_definitions(/MP)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ot /openmp" )
else (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif (MSVC)

# add opencv
set(BUILD_SHARED_LIBS off) # used static library in windows
find_package(OpenCV REQUIRED)
message(STATUS ${OpenCV_LIBS})

#find JimaTechLibrary
File(TO_CMAKE_PATH $ENV{JIMATECH_LIB} JIMATECH_LIB)
file(GLOB_RECURSE JimaTechLibrary_sources "${JIMATECH_LIB}/*.h" "${JIMATECH_LIB}/*.hpp")

# Add local library
set(LOCAL_LIBS "${CMAKE_SOURCE_DIR}/src/Library")
file(GLOB_RECURSE Lib_sources "${LOCAL_LIBS}/*.h" "${LOCAL_LIBS}/*.hpp")

## add  BoardingProcess project
add_executable(BoardingProcess src/main.cpp ${Lib_sources} ${JimaTechLibrary_sources})
set_target_properties(BoardingProcess PROPERTIES CXX_STANDARD 17
					MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
					)
target_include_directories(BoardingProcess PUBLIC ${OpenCV_INCLUDE_DIRS} )
target_link_libraries(BoardingProcess PRIVATE ${OpenCV_LIBS} )
target_include_directories(BoardingProcess PUBLIC ${JIMATECH_LIB} ${LOCAL_LIBS})
			   