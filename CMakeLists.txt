# Ajustez en fonction de votre version de CMake
cmake_minimum_required (VERSION 2.8)


# Nom du projet

project (stabilization)


find_package (OpenCV REQUIRED)
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )

# Exécutable "hello", compilé à partir du fichier helloworld.c

add_executable (colorTracking colorTracking.cpp imgProcessing.cpp Serial.cpp PID.cpp)
enable_testing()
set(CMAKE_BUILD_TYPE Debug)
# Hello sera linké avec les bibliothèques d'OpenCV

target_link_libraries (colorTracking ${OpenCV_LIBS})
target_link_libraries (colorTracking m)


add_executable (harrisDemo harrisCornerDetection.cpp CornerDetector.cpp )
target_link_libraries (harrisDemo ${OpenCV_LIBS})
target_link_libraries (harrisDemo m)

add_executable (harrisTracking harrisTracking.cpp imgProcessing.cpp Serial.cpp PID.cpp )
target_link_libraries (harrisTracking ${OpenCV_LIBS})
target_link_libraries (harrisTracking m)

add_executable (finalTrack main.cpp Serial.cpp PID.cpp)
target_link_libraries (finalTrack ${OpenCV_LIBS})
target_link_libraries (finalTrack m)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_definitions(-DFREQUENCY=1)
add_definitions(-DGRAPHIC=1)
