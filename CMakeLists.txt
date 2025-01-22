cmake_minimum_required(VERSION 3.30)
project(CPMULTI)

set(CMAKE_CXX_STANDARD 20)

# Chemin vers OpenCV
set(OpenCV_DIR "C:/opencv/build")

# Trouver les paquets OpenCV
find_package(OpenCV REQUIRED)

# Ajouter les répertoires d'inclusion
include_directories(${OpenCV_INCLUDE_DIRS})

# Copier les fichiers .dll
set(OpenCV_DLL_DIR "C:/tools/opencv/build/bin")
file(GLOB OpenCV_DLLS "${OpenCV_DLL_DIR}/*.dll")
file(COPY ${OpenCV_DLLS} DESTINATION ${CMAKE_BINARY_DIR})

# Lier les bibliothèques OpenCV à votre projet
add_executable(CPMULTI main.cpp)
target_link_libraries(CPMULTI ${OpenCV_LIBS})
