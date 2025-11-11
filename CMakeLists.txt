cmake_minimum_required(VERSION 3.29)
project(DataLibC C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared library" ON)

if (NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/build" CACHE PATH "Install path prefix" FORCE)
endif ()

find_library(WINSDK_XAUDIO2_LIBRARY NAMES XAudio2_8)

file(GLOB_RECURSE DATALIBC_SOURCES
        LIST_DIRECTORIES false
        CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/DataLibC/**/*.c
        ${PROJECT_SOURCE_DIR}/DataLibC/**/*.h
)

add_library(${PROJECT_NAME} ${DATALIBC_SOURCES}
        DataLibC/Physics/Backend/PhysicsCore.c
        DataLibC/Physics/Backend/PhysicsCore.h
        DataLibC/Physics/Rigidbody/RigidbodyType.h
        DataLibC/Physics/Physics.h)


target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME} PRIVATE ${WINSDK_XAUDIO2_LIBRARY} ws2_32)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/DataLibC/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h"
)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        ENABLE_EXPORT
        EXPORT_MODULE
)