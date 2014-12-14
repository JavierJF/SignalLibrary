FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Doxygen)
SET(Qt5Widgets_DIR "/opt/Qt5/5.3/gcc_64/lib/cmake/Qt5Widgets/")

# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)
# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(Qt5Widgets REQUIRED)
