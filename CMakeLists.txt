cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Trouver OpenCV
find_package(OpenCV REQUIRED)

# Inclure les fichiers headers
include_directories(headers ${OpenCV_INCLUDE_DIRS})

# Ajouter les fichiers sources du projet
add_executable(MyProject 
    main.cpp 
    src/Application.cpp 
    src/ImageProcessor.cpp 
    src/Utils.cpp
)

target_link_libraries(MyProject ${OpenCV_LIBS})
