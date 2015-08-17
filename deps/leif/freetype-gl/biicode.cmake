SET(freetype-gl_LIBS_SUPPLIED TRUE)
message ("Building with BIICODE, freetype dep @: ${CMAKE_SOURCE_DIR}/deps/miguel/freetype")
SET(FREETYPE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/deps/miguel/freetype/include")
SET(FREETYPE_INCLUDE_DIR_ft2build "${FREETYPE_INCLUDE_DIRS}")
SET(FREETYPE_INCLUDE_DIR_freetype2 "${FREETYPE_INCLUDE_DIRS}")
SET(FREETYPE_LIBRARY "${CMAKE_SOURCE_DIR}/lib/libmiguel_freetype.a")
message ("Libs @ ${FREETYPE_LIBRARY}")
SET(CMAKE_CURRENT_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")
message ("Binary out @ ${CMAKE_CURRENT_BINARY_DIR}")

SET( FREETYPE_GL_SRC freetype-gl.h
                     vec234.h
                     opengl.h
                     markup.h
                     mat4.c             mat4.h
                     texture-atlas.c    texture-atlas.h
                     texture-font.c     texture-font.h
                     vertex-buffer.c    vertex-buffer.h
                     vertex-attribute.c vertex-attribute.h
                     font-manager.c     font-manager.h
                     text-buffer.c      text-buffer.h
                     shader.c           shader.h
                     vector.c           vector.h
                     platform.c         platform.h
                     edtaa3func.h       edtaa3func.c)


SET(BII_LIB_SRC ${FREETYPE_GL_SRC})
SET(BII_LIB_TYPE STATIC)

ADD_BII_TARGETS()

# Copy font and shaders into build directory for in-place testing
FILE(GLOB Fonts "fonts/*.ttf")
FILE(GLOB Data   "data/256colors.txt")
FILE(GLOB Shaders "shaders/*.vert" "shaders/*.frag")


if (APPLE)
set(ASSET_DIR ${CMAKE_CURRENT_BINARY_DIR}/Contents/Resources)
else (APPLE)
set(ASSET_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif(APPLE)

file(COPY ${Fonts} DESTINATION ${ASSET_DIR})
file(COPY ${Data} DESTINATION ${ASSET_DIR})
file(COPY ${Shaders} DESTINATION ${ASSET_DIR})