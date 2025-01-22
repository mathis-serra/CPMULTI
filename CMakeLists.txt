cmake_minimum_required(VERSION 3.30)
project(CPMULTI)

set(CMAKE_CXX_STANDARD 17)

#set(OpenCV_DIR "C:/opencv/opencv/build/lib")


# Rechercher OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Copier les fichiers .dll
set(OpenCV_DLL_DIR "C:/tools/opencv/build/bin")
file(GLOB OpenCV_DLLS "${OpenCV_DLL_DIR}/*.dll")
file(COPY ${OpenCV_DLLS} DESTINATION ${CMAKE_BINARY_DIR})

# Définir l'exécutable
add_executable(CPMULTI main.cpp)

# Lier les bibliothèques d'OpenCV
target_link_libraries(CPMULTI ${OpenCV_LIBS})



