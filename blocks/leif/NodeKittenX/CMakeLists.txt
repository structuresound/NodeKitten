# Yes, INIT_BIICODE_BLOCKS() is no longer necessary

cmake_minimum_required (VERSION 2.8)

# ************* PLATFORM *************

if (EMSCRIPTEN)
message("*** TARGET EMSCRIPTEN *** \n")
add_definitions(-DEMSCRIPTEN)
endif (EMSCRIPTEN)

#force
if(EXISTS "/opt/vc/include/bcm_host.h")
message("*** CONFIGURE FOR RASPBERRY PI *** \n")
add_definitions( -DTARGET_RASPBERRY_PI=1)
add_definitions( -DTARGET_LINUX_ARM=1)
set(TARGET_RPI 1)
endif()

if(NOT BIICODE)
project(NodeKittenXExample)
message("*** START NK CMAKE - BUILD *** \n")
endif (NOT BIICODE)

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
    if (EMSCRIPTEN)
    else (CHECK)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
        include(compiler_check.cmake)
        info_compiler()
        message(FATAL_ERROR "Clang version must be at least 3.2!" -> "${CMAKE_CXX_COMPILER_ID}" ${CMAKE_CXX_COMPILER_VERSION})
    else()
        message("FOUND CLANG >= 3.2 \n")
    endif()
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
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
file(GLOB_RECURSE NK_EXAMPLES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Examples/*.h)

set(NK_HEADERS ${NODEKITTEN} ${NK_TYPES} ${NK_UTILS} ${NK_EVENT} ${NK_TEXTURE} ${NK_SHADER} ${NK_NODE} ${NK_VIEW} ${NK_EXAMPLES})

if(EMSCRIPTEN)
    file(GLOB_RECURSE NK_SDL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/Emscripten/*.h)
else (SDL)
    if(APPLE)
        file(GLOB_RECURSE NK_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/Cocoa/*.h)
    endif(APPLE)
    file(GLOB_RECURSE NK_SDL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/SDL/*.h)
endif(PLATFORM)

list (APPEND NK_HEADERS ${NK_SDL})
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
file(GLOB_RECURSE NK_EXAMPLES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Examples/*.cpp)

set(NK_SOURCES ${NK_TYPES} ${NK_UTILS} ${NK_EVENT} ${NK_TEXTURE} ${NK_SHADER} ${NK_NODE} ${NK_VIEW} ${NK_EXAMPLES})


if(EMSCRIPTEN)
    file(GLOB_RECURSE NK_SDL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/Emscripten/*.cpp)
else (SDL)
    if(APPLE)
        file(GLOB_RECURSE NK_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/Cocoa/*.mm)
        set_source_files_properties(${NK_PLATFORM} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++ -fobjc-arc")
    endif(APPLE)
    file(GLOB_RECURSE NK_SDL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${NK_ROOT}/Platform/SDL/*.cpp)
endif(PLATFORM)

list (APPEND NK_SOURCES ${NK_SDL})
list (APPEND NK_SOURCES ${NK_PLATFORM})

message("*** NK SOURCES *** \n" ${NK_SOURCES})

# ************* ADD TARGETS *************

if(BIICODE)

SET(BII_LIB_SRC ${LIB_HEADERS} ${NK_HEADERS} ${LIB_SOURCES} ${NK_SOURCES})

ADD_BII_TARGETS()

message(" BII TARGETS: \n ${BII_LIB_TARGET} \n block \n ${BII_BLOCK_TARGETS} \n name \n ${BII_exe_name_TARGET} \n exe \n ${BII_main_TARGET}")

SET(BII_exe_name_TARGET nodeKittenExample)

else(CMAKE_ONLY)
# SETUP BUILD NOT USING BII ADD TARGETS

set(HEADERS ${LIBS} ${NK_HEADERS})
set(SOURCES ${LIBS} ${NK_SOURCES})

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

endif(CMAKE_ONLY)

# ************* DEPENDENCIES / FRAMEWORKS *************

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

# ************* LINK *************

set (EXPORTS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/exports.txt)

if(NOT BIICODE)
message("*** LINK LIBS ***: " ${EXTRA_LIBS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${EXTRA_LIBS})

if (APPLE)

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

add_framework(Foundation ${PROJECT_NAME})
add_framework(AppKit ${PROJECT_NAME})
add_framework(QuartzCore ${PROJECT_NAME})
endif(APPLE)

endif(NOT BIICODE)


if (EMSCRIPTEN)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --bind -O2 --llvm-opts 2 -s FULL_ES2=1 -s DEMANGLE_SUPPORT=1 -s GL_ASSERTIONS=1 -s GL_DEBUG=1 -s TOTAL_MEMORY=268435456 -s EXPORTED_FUNCTIONS=@${EXPORTS_FILE} --preload-file ${CMAKE_CURRENT_BINARY_DIR}/assets@/Assets ")
message("*** LINKER FLAGS *** \n" ${CMAKE_EXE_LINKER_FLAGS})
endif (EMSCRIPTEN)



# ************* COMPILER OPTIONS *************

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


###############################################################################
# ************* RESOURCES *************                                       #
###############################################################################

# COPY THIS IN YOUR PROJECT IF IT NEEDS TO ADD ASSETS

message(STATUS "Copying data files to build and bin dirs ")

file(GLOB PNG RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Examples/Assets/*.png *.jpg)
file(GLOB JPEG RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Examples/Assets/*.jpg)

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
    if (NOT EMSCRIPTEN)
    file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
    endif()
endforeach(file)
endif(APPLE)