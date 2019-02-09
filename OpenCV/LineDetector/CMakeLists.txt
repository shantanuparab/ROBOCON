# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Using C++11 because we need a few components present in it
# Please Update Compiler On Target System to Latest VERSION
# Update Other Applications as well
set (CMAKE_CXX_STANDARD 14)

project ("LineDetector")

# Use this to Find the RasPiCAM Package
# Note that we only need this package when using RasPiCam
find_package(raspicam)
find_package(OpenCV REQUIRED)	

# Add source to this project's executable.
add_executable (LineDetector "main.cxx"
							 "include/SwitchCamera.hxx"
							 "include/Window.hxx"
							 "include/LineDetector.hxx"
							 "include/I2C.hxx"
							 "src/LineDetector.cxx")


# Check compiler options for Clang-Cl
# Clang-Cl is a Special MSVC Driver for Clang
# Also specify same values for MSVC
if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") AND ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC"))
		target_compile_options(LineDetector PRIVATE /Zc:__cplusplus /permissive- /Zc:twoPhase -Xclang -Wpedantic)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
		target_compile_options(LineDetector PRIVATE /Zc:__cplusplus /permissive- /Zc:twoPhase)
elseif((${CMAKE_CXX_COMPILER_ID} MATCHES "Clang|GNU"))
	target_compile_options(LineDetector PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries( LineDetector PRIVATE ${OpenCV_LIBS} )
