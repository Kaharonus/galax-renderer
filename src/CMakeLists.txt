set(BINARY ${CMAKE_PROJECT_NAME})

# -------------------------------------------------------------------------------
# Source files

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.h *.cpp *.ui)
set(SOURCES ${SOURCES} generators/SolarSystemLoader.cpp generators/SolarSystemLoader.h)
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

# --------------------------------------------------------------------------------
# QT

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
qt_standard_project_setup()

# --------------------------------------------------------------------------------
# CONAN

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# --------------------------------------------------------------------------------
# CPM

set(CPM_DOWNLOAD_VERSION 0.35.5)

if(CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD
            https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
            ${CPM_DOWNLOAD_LOCATION}
            )
endif()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
        NAME QCodeEditor
        GITHUB_REPOSITORY Kaharonus/QCodeEditor
        GIT_TAG b6840de235de3b2fcce2182f5aed46aca0c1fc54
)

set(CPM_LIBS ${CPM_LIBS} QCodeEditor)


# --------------------------------------------------------------------------------
# ASSETS

set(ASSET_TARGET assets)
add_custom_target(assets
        COMMAND ${CMAKE_COMMAND} -P "assets/CMakeLists.txt" ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# --------------------------------------------------------------------------------
# BUILD

add_executable(${BINARY} ${SOURCES})
add_dependencies(${BINARY} ${ASSET_TARGET})
target_link_libraries(${BINARY}  Qt6::Widgets Qt6::OpenGLWidgets ${CONAN_LIBS} ${CPM_LIBS})
