# Yes, INIT_BIICODE_BLOCKS() is no longer necessary

# ************* PLATFORM *************

cmake_minimum_required (VERSION 2.8)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
        message("GCC version must be at least 4.8!")
        message(FATAL_ERROR "
        To Install gcc-4.8 g++-4.8 on Raspberry Pi / Debian, enter the following commands:\n
        \n
        $ sudo apt-get install gcc-4.8 g++-4.8\n
        $ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 100 --slave /usr/bin/g++ g++ /usr/bin/g++-4.8\n
        \n
        Then run the the command and select gcc-4.8.\n
        $ sudo update-alternatives --config gcc \n
        ")
    else()
        message("FOUND GCC >= 4.8 \n")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 3.2
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
        message(FATAL_ERROR "Clang version must be at least 3.2!")
    else()
        message("FOUND CLANG >= 3.2 \n")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()

#force
if(EXISTS "/opt/vc/include/bcm_host.h")
message("*** CONFIGURE FOR RASPBERRY PI *** \n")
add_definitions( -DTARGET_RASPBERRY_PI=1)
add_definitions( -DTARGET_LINUX_ARM=1)
set(TARGET_RPI 1)
endif()

# ************* LOCATIONS *************

set(NK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ************* HEADERS *************

file(GLOB NODEKITTEN RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/*.h)
file(GLOB_RECURSE NK_TYPES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Types/*.h)
file(GLOB_RECURSE NK_UTILS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Utils/*.h)
file(GLOB_RECURSE NK_EVENT RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Event/*.h)
file(GLOB_RECURSE NK_TEXTURE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Texture/*.h)
file(GLOB_RECURSE NK_SHADER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Shader/*.h)
file(GLOB_RECURSE NK_NODE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Node/*.h)
file(GLOB_RECURSE NK_VIEW RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/View/*.h)


set(NK_HEADERS ${NODEKITTEN} ${NK_TYPES} ${NK_UTILS} ${NK_EVENT} ${NK_TEXTURE} ${NK_SHADER} ${NK_NODE} ${NK_VIEW})

if(APPLE)
file(GLOB_RECURSE NK_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/Cocoa/*.h)
elseif(EMSCRIPTEN)
file(GLOB_RECURSE NK_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/Emscripten/*.h)
else(SDL)
file(GLOB_RECURSE NK_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/SDL/*.h)
endif(PLATFORM)

list (APPEND NK_HEADERS ${NK_PLATFORM})

message("*** NK HEADERS *** \n" ${NK_HEADERS})

set (INCLUDE_DIRS "")

foreach (_headerFile ${NK_HEADERS})
get_filename_component(_dir ${_headerFile} PATH)
list (APPEND INCLUDE_DIRS ${_dir})
endforeach()
foreach (_headerFile ${LIB_HEADERS})
get_filename_component(_dir ${_headerFile} PATH)
list (APPEND INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)

if (TARGET_RPI)
SET(COMPILE_DEFINITIONS -Werror)

include_directories(
/opt/vc/include/GLES
/opt/vc/include
/opt/vc/include/interface/vcos
/opt/vc/include/interface/vcos/pthreads
/opt/vc/include/interface/vmcs_host/linux
usr/include
)

link_directories(/opt/vc/lib)

endif (TARGET_RPI)

list (APPEND INCLUDE_DIRS ${SYS_INCLUDE})

message("*** INCLUDE DIRS *** \n" ${INCLUDE_DIRS} ${DEP_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS} ${DEP_INCLUDE_DIRS})

# ************* SOURCES *************

file(GLOB_RECURSE NK_TYPES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Types/*.cpp)
file(GLOB_RECURSE NK_UTILS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Utils/*.cpp)
file(GLOB_RECURSE NK_EVENT RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Event/*.cpp)
file(GLOB_RECURSE NK_TEXTURE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Texture/*.cpp)
file(GLOB_RECURSE NK_SHADER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Shader/*.cpp)
file(GLOB_RECURSE NK_NODE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Node/*.cpp)
file(GLOB_RECURSE NK_VIEW RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/View/*.cpp)


set(NK_SOURCES ${NK_TYPES} ${NK_UTILS} ${NK_EVENT} ${NK_TEXTURE} ${NK_SHADER} ${NK_NODE} ${NK_VIEW} ${NK_EXAMPLE})


if(APPLE)
file(GLOB_RECURSE NK_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/Cocoa/*.mm)
set_source_files_properties(${NK_PLATFORM} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++ -fobjc-arc")
elseif(EMSCRIPTEN)
file(GLOB_RECURSE NK_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/Emscripten/*.cpp)
else(SDL)
file(GLOB_RECURSE NK_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/SDL/*.cpp)
endif(PLATFORM)

list (APPEND NK_SOURCES ${NK_PLATFORM})

message("*** NK SOURCES *** \n" ${NK_SOURCES})

if(BIICODE)

SET(BII_LIB_SRC ${LIB_HEADERS} ${NK_HEADERS} ${LIB_SOURCES} ${NK_SOURCES})

ADD_BII_TARGETS()

message(" BII TARGETS: \n ${BII_LIB_TARGET} \n block \n ${BII_BLOCK_TARGETS} \n name \n ${BII_exe_name_TARGET} \n exe \n ${BII_main_TARGET}")

SET(BII_exe_name_TARGET nodeKittenExample)

# > EXAMPLE: how to activate C++11
#
#if(APPLE)
#         TARGET_COMPILE_OPTIONS(${BII_BLOCK_TARGET} INTERFACE "-std=c++11")
#elseif (WIN32 OR UNIX)
#         TARGET_COMPILE_OPTIONS(${BII_BLOCK_TARGET} INTERFACE "-std=c++11")
#endif(APPLE)

#-stdlib=libc++

else(BIICODE)

macro(ADD_FRAMEWORK fwname appname)
find_library(FRAMEWORK_${fwname}
NAMES ${fwname}
PATHS ${CMAKE_OSX_SYSROOT}/System/Library
PATH_SUFFIXES Frameworks
NO_DEFAULT_PATH)
if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
MESSAGE(ERROR ": Framework ${fwname} not found")
else()
TARGET_LINK_LIBRARIES(${appname} ${FRAMEWORK_${fwname}})
MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
endif()
endmacro(ADD_FRAMEWORK)

#set (PROJECTNAME NodeKittenXExample)
cmake_minimum_required (VERSION 2.8)

project(NodeKittenXExample)

message("*** START NK BUILD *** \n")

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

if (EMSCRIPTEN)
message("*** TARGET EMSCRIPTEN *** \n")
add_definitions(-DEMSCRIPTEN)
endif (EMSCRIPTEN)

#set (EXPORTED_FUNCTIONS [‘_main’])

# ************* DEPENDENCIES *************

SET(EXTRA_LIBS "")

if(APPLE)

message("*** TARGET APPLE *** \n")

list (APPEND EXTRA_LIBS ${COREFOUNDATION_FRAMEWORK} ${FOUNDATION_FRAMEWORK} ${IOKIT_FRAMEWORK} ${PLUGIN_INTERNAL_DEPS})

message("APPLE LOCATIONS : " ${EXTRA_LIBS})

endif (APPLE)

# This is the shortcut to finding GLU, GLUT and OpenGL if they are properly installed on your system
# This should be the case.

set(DEP_INCLUDE_DIRS "")

FIND_PACKAGE(OpenGL)

list (APPEND DEP_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
list (APPEND EXTRA_LIBS ${OPENGL_LIBRARIES})

message("LIBRARY LOCATIONS : " ${DEP_INCLUDE_DIRS})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

set(HEADERS ${LIBS} ${NK_HEADERS})
set(SOURCES ${LIBS} ${NK_SOURCES})

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# ************* LINK *************
set (ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/Assets)

#file(GLOB_RECURSE ASSETS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Assets/*.png)

set (EXPORTS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/exports.txt)

if (EMSCRIPTEN)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --bind -O2 --llvm-opts 2 -s FULL_ES2=1 -s DEMANGLE_SUPPORT=1 -s GL_ASSERTIONS=1 -s GL_DEBUG=1 -s TOTAL_MEMORY=268435456 -s EXPORTED_FUNCTIONS=@${EXPORTS_FILE} --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/Assets@/Assets ")

# OTHER OPTS
# --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/Assets@/Assets
# -o NodeKittenX.html
#${ASSETS}@/outdoors.png --preload-file ${ASSETS}@/moon.png")

# --closure 0 -s WARN_ON_UNDEFINED_SYMBOLS=1"
message("*** LINKER FLAGS *** \n" ${CMAKE_EXE_LINKER_FLAGS})
endif (EMSCRIPTEN)

if (APPLE)
add_framework(Foundation ${PROJECT_NAME})
add_framework(AppKit ${PROJECT_NAME})
add_framework(QuartzCore ${PROJECT_NAME})
endif(APPLE)

message("*** LINK LIBS ***: " ${EXTRA_LIBS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${EXTRA_LIBS})

endif(BIICODE)

# ************* COMPILER OPTIONS *************

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


###############################################################################
# ************* RESOURCES *************                                       #
###############################################################################

# COPY THIS IN YOUR PROJECT IF IT NEEDS TO ADD ASSETS

message(STATUS "Copying data files to build and bin dirs ")

file(GLOB PNG RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Assets/*.png *.jpg)
file(GLOB JPEG RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Assets/*.jpg)

set(Assets ${PNG} ${JPEG})

if (APPLE)
foreach(file ${Assets})
    message(STATUS "Copying file ${pngFile}")
    file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Contents/Resources)
    file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Contents/Resources)
endforeach(file)
else(APPLE)
foreach(file ${Assets})
    message(STATUS "Copying file ${pngFile}")
    file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
    file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
endforeach(file)
endif(APPLE)

# You can safely delete lines from here...

###############################################################################
#      REFERENCE                                                              #
###############################################################################
#
# This CMakeLists.txt file helps defining your block building and compiling
# To learn more about the CMake use with biicode, visit http://docs.biicode.com/c++.html
#
# ----------------------------------------------------
# NEW FEATURE! Include cmake files from remote blocks:
# -----------------------------------------------------
# Now you can handle cmake dependencies alike you do with c/c++:
#
#     INCLUDE(user/block/myrecipe) # include myrecipe.cmake from remote user/block
#
# > EXAMPLE: Include our recipes and activate C++11 in your block (http://www.biicode.com/biicode/cmake)
#
#     INCLUDE(biicode/cmake/tools) # Include tools.cmake file from cmake block from the "biicode" user
#     ACTIVATE_CPP11(INTERFACE ${BII_BLOCK_TARGET})
#
#     Remember to run "bii find" to download out cmake tools file
#
# ---------------------
# INIT_BIICODE_BLOCK()
# ---------------------
# This function creates several helper variables as ${BII_BLOCK_NAME} and ${BII_BLOCK_USER}
# Also it loads variables from the cmake/bii_user_block_vars.cmake
#     ${BII_LIB_SRC}          File list to create the library
#     ${BII_LIB_TYPE}         Empty (default, STATIC most casess) STATIC or SHARED
#     ${BII_LIB_DEPS}         Dependencies to other libraries (user2_block2, user3_blockX)
#     ${BII_LIB_SYSTEM_HEADERS}  System linking requirements as windows.h, pthread.h, etc
#
# You can use or modify them here, for example, to add or remove files from targets based on OS
# Or use typical cmake configurations done BEFORE defining targets. Examples:
#     ADD_DEFINITIONS(-DFOO)
#     FIND_PACKAGE(OpenGL QUIET)
#     You can add INCLUDE_DIRECTORIES here too
#
# ---------------------
# ADD_BIICODE_TARGETS()
# ---------------------
#
# This function creates the following variables:
#     ${BII_BLOCK_TARGET}  Interface (no files) target for convenient configuration of all
#                          targets in this block, as the rest of targets always depend on it
#                          has name in the form "user_block_interface"
#     ${BII_LIB_TARGET}  Target library name, usually in the form "user_block". May not exist
#                        if BII_LIB_SRC is empty
#     ${BII_BLOCK_TARGETS} List of all targets defined in this block
#     ${BII_BLOCK_EXES} List of executables targets defined in this block
#     ${BII_exe_name_TARGET}: Executable target (e.g. ${BII_main_TARGET}. You can also use
#                            directly the name of the executable target (e.g. user_block_main)
#
# > EXAMPLE: Add include directories to all targets of this block
#
#    TARGET_INCLUDE_DIRECTORIES(${BII_BLOCK_TARGET} INTERFACE myincludedir)
#
#    You can add private include directories to the Lib (if existing)
#
# > EXAMPLE: Link with pthread:
#
#    TARGET_LINK_LIBRARIES(${BII_BLOCK_TARGET} INTERFACE pthread)
#        or link against library:
#    TARGET_LINK_LIBRARIES(${BII_LIB_TARGET} PUBLIC pthread)
#        or directly use the library target name:
#    TARGET_LINK_LIBRARIES(user_block PUBLIC pthread)
#
#    NOTE:  This can be also done adding pthread to ${BII_LIB_DEPS}
#            BEFORE calling ADD_BIICODE_TARGETS()
#
# > EXAMPLE: how to activate C++11
#
#    IF(APPLE)
#         TARGET_COMPILE_OPTIONS(${BII_BLOCK_TARGET} INTERFACE "-std=c++11 -stdlib=libc++")
#    ELSEIF (WIN32 OR UNIX)
#         TARGET_COMPILE_OPTIONS(${BII_BLOCK_TARGET} INTERFACE "-std=c++11")
#    ENDIF(APPLE)
#
# > EXAMPLE: Set properties to target
#
#    SET_TARGET_PROPERTIES(${BII_BLOCK_TARGET} PROPERTIES COMPILE_DEFINITIONS "IOV_MAX=255")
#


