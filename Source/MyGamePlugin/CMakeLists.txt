cmake_minimum_required(VERSION 3.14)
project (MyGamePlugin)

# C++17 is required for this project.
set (CMAKE_CXX_STANDARD 17)

# Setup output directories.
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file (GLOB SOURCE_FILES *.cpp *.h)

# Windows only: URHO3D_WIN32_CONSOLE controls whether to keep console or not
if (NOT URHO3D_WIN32_CONSOLE)
    set (PLATFORM WIN32)
endif ()


add_library (MyGamePlugin ${SOURCE_FILES})
target_link_libraries (MyGamePlugin PUBLIC Urho3D)
define_static_plugin (MyGamePlugin "MyGamePlugin")

install(TARGETS MyGamePlugin
    RUNTIME DESTINATION ${DEST_BIN_DIR_CONFIG}
    LIBRARY DESTINATION ${DEST_LIBRARY_DIR_CONFIG}
    ARCHIVE DESTINATION ${DEST_ARCHIVE_DIR_CONFIG}
    BUNDLE  DESTINATION ${DEST_BIN_DIR_CONFIG}
)
