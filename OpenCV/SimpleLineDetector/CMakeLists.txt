# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Using C++14 because we need a few components present in it
# Please Update Compiler On Target System to Latest VERSION
# Update Other Applications as well
set (CMAKE_CXX_STANDARD 17)

project ("SimpleLineDetector")

find_package(OPENCV REQUIRED)

INCLUDE_DIRECTORIES(PRIVATE ${OPENCV_INCLUDE_DIR})

# Add source to this project's executable.
add_executable (SimpleLineDetector "main.cxx"
							 "include/Window.hxx"
							 "include/LineDetector.hxx" )

target_link_libraries( SimpleLineDetector PRIVATE ${OpenCV_LIBS} )
