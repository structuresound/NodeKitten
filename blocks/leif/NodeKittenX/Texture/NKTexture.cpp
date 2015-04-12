//
//  NKTexture.m
//  NodeKittenExample
//
//  Created by Chroma Developer on 3/5/14.
//  Copyright (c) 2014 Chroma. All rights reserved.
//

#include "NKTexture.h"
#include "png.h"
#include "picojpeg_util.h"
#include "NKStringUtil.h"
#include "NKFrameBuffer.h"

#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define 	GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG   0x8C00
#define 	GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG   0x8C01
#define 	GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG   0x8C02
#define 	GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG   0x8C03
#endif

using namespace String;

shared_ptr<NKTexture> NKTexture::textureWithImageFile(string filePath, bool relative_path, U1t flags, U1t filter, F1t anistropicFilter) {
    
    return make_shared<NKTexture>(filePath, relative_path, flags, filter, anistropicFilter);
    
}

void NKTexture::genGLTexture(int w, int h){
    
    glActiveTexture(GL_TEXTURE0);
    
    glGenTextures(1, (GLuint *)&glName);
    glBindTexture(target, glName);
    
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    nkGetGLError();
    
}


void NKTexture::bind() { // GL 1
    glActiveTexture(GL_TEXTURE0);
    nkGetGLError();
    glBindTexture(target, glName);
    nkGetGLError();
}
//
void NKTexture::unbind() { // GL 1
    glBindTexture(target, 0);
}

void NKTexture::enableAndBind(int tLocation){
    glActiveTexture(GL_TEXTURE0+tLocation);
    glBindTexture(target, glName);
}

void NKTexture::bindToUniform(GLuint uLocation){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(target, glName);
    glUniform1i(uLocation, 0);
}

void NKTexture::enableAndBindToUniform(GLuint uSamplerLocation, GLuint tLocation){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(target, glName);
    glUniform1i(uSamplerLocation, tLocation);
}

/*!
	Helper function to automatically create, load and generate a new TEXTURE.
 
	\param[in] name The internal name to use for the new TEXTURE.
	\param[in] filename The file to load.
	\param[in] relative_path Determine if the filename is a relative (1) or absolute (0) path.
	\param[in] flags The TEXTURE flags to use.
	\param[in] filter The mipmap filtering to use.
	\param[in] anisotropic_filter The anisotropic filtering to use for the texture.
	
	\return Return a new TEXTURE structure pointer, or return NULL if the function was unable
	to load the file.
 */

NKTexture::NKTexture(string filename, bool relative_path, U1t flags, U1t filter, F1t anistropicFilter, bool cube){
    
    NKFile file {filename, relative_path};
    
    //nkLog("loading texture: fullpath: %s", file.filename());
    
    string name = lastPathExtension(file.filename());
    
    string ext = fileType(name, 1);
    
    if( file.size() )
    {
        nkLog("loading texture: fullpath: %s \n name: %s \n ext: %s \n", file.filename(), name, ext);
        
        if( ext == "PNG" ) loadPng(file);
        
        else if( ext == "PVR") loadPvr(file);
        
        else if( ext == "JPG" || ext == "JPEG") loadJpg(file);
        
        target = GL_TEXTURE_2D;
        
        allocate(flags, filter, anistropicFilter, cube);
        
        if( texelArray )
        {
            free( texelArray );
            texelArray = nullptr;
        }
        
    }
    
    else {
        nkLog("error loading file : %s", file.filename());
    }
    
}


/*!
	Callback function used internally to decompress a PNG file in memory.
	
	\param[in] structp The PNG structure that point to the attached MEMORY stream.
	\param[in] bytep The byte pointer to of the stream to read.
	\param[in] size The size in bytes to read from the stream.
 */
void png_memory_read( png_structp structp, png_bytep bytep, png_size_t size )
{
    NKFile *m = (NKFile*) png_get_io_ptr( structp );
    
    m->read(bytep, size);
}


/*!
	Helper function to decompress a PNG texture in-memory.
	
	\param[in,out] texture A valid TEXTURE pointer.
	\param[in] memory A valid MEMORY pointer that contains a PNG buffer.
 */
void NKTexture::loadPng(NKFile &file)
{
    png_structp structp;
    
    png_infop infop;
    
    png_bytep *bytep = NULL;
    
    unsigned int i = 0;
    
    int n,
    png_bit_depth,
    png_color_type;
    
    structp = png_create_read_struct( PNG_LIBPNG_VER_STRING,
                                     NULL,
                                     NULL,
                                     NULL );
    
    infop = png_create_info_struct( structp );
    
    png_set_read_fn( structp, ( png_voidp * )&file, png_memory_read );
    
    png_read_info( structp, infop );
    
    png_bit_depth = png_get_bit_depth( structp, infop );
    
    png_color_type = png_get_color_type( structp, infop );
    
    if( png_color_type == PNG_COLOR_TYPE_PALETTE )
    { png_set_expand( structp ); }
    
    if( png_color_type == PNG_COLOR_TYPE_GRAY && png_bit_depth < 8 )
    { png_set_expand( structp ); }
    
    if( png_get_valid( structp, infop, PNG_INFO_tRNS ) )
    { png_set_expand( structp ); }
    
    if( png_bit_depth == 16 )
    { png_set_strip_16( structp ); }
    
    if( png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
    { png_set_gray_to_rgb( structp ); }
    
    png_read_update_info( structp, infop );
    
    //nkLog("get png info :");
    
    png_uint_32 w, h;
    
    png_get_IHDR( structp,
                 infop,
                 ( &w  ),
                 ( &h ),
                 &png_bit_depth,
                 &png_color_type,
                 NULL, NULL, NULL );
    
    width = (U1t)w;
    height = (U1t)h;
    
    //nkLog("size %d x %d", width, height);
    
    switch( png_color_type )
    {
        case PNG_COLOR_TYPE_GRAY:
        {
            bytes			 = 1;
            internal_format =
            format		     = GL_LUMINANCE;
            
            break;
        }
            
        case PNG_COLOR_TYPE_GRAY_ALPHA:
        {
            bytes			 = 2;
            internal_format =
            format		     = GL_LUMINANCE_ALPHA;
            
            break;
        }
            
        case PNG_COLOR_TYPE_RGB:
        {
            bytes			 = 3;
            internal_format =
            format		     = GL_RGB;
            
            break;
        }
            
        case PNG_COLOR_TYPE_RGB_ALPHA:
        {
            bytes			 = 4;
            internal_format =
            format		     = GL_RGBA;
            
            break;
        }
    }
    
    texelType = GL_UNSIGNED_BYTE;
    
    size = width * height * bytes;
    
    texelArray = ( unsigned char * ) malloc( size );
    
    bytep = ( png_bytep * ) malloc( height * sizeof( png_bytep ) );
    
    
    while( i != height )
    {
        n = height - ( i + 1 );
        
        bytep[ n ] = texelArray + ( n * width * bytes );
        
        ++i;
    }
    
    
    png_read_image( structp, bytep );
    
    png_read_end( structp, NULL );
    
    png_destroy_read_struct( &structp,
                            &infop,
                            NULL );
    
    free( bytep );
}

void NKTexture::loadJpg(NKFile &file)
{
 
    int comps;
    pjpeg_scan_type_t scanType;
    int reduce = 0;
    
    texelArray = pjpeg_load_from_file(file.filename().c_str(), &width, &height, &comps, &scanType, reduce);
    
    nkLog("load jpeg size %d x %d", width, height);
    
//    bytes			 = 3;
//    internal_format =
//    format		     = GL_RGB;
    
    bytes			 = 3;
    internal_format =
    format		     = GL_RGBA;
    
    texelType = GL_UNSIGNED_BYTE;
    
    size = width * height * bytes;
}

void NKTexture::loadPvr(NKFile &file)
{
    const unsigned char pvrtc_identifier[ 4 ] = { 'P', 'V', 'R', '!' };
    
    PVRHEADER *pvrheader = ( PVRHEADER * )file.data();
    
    if( ( ( pvrheader->tag >> 0  ) & 0xFF ) != pvrtc_identifier[ 0 ] ||
       ( ( pvrheader->tag >> 8  ) & 0xFF ) != pvrtc_identifier[ 1 ] ||
       ( ( pvrheader->tag >> 16 ) & 0xFF ) != pvrtc_identifier[ 2 ] ||
       ( ( pvrheader->tag >> 24 ) & 0xFF ) != pvrtc_identifier[ 3 ] )
    { return; }
    
    
    if( ( pvrheader->flags & 0xFF ) == 24 || // PVRTC2
       ( pvrheader->flags & 0xFF ) == 25 )  // PVRTC4
    {
        width    = pvrheader->width;
        height   = pvrheader->height;
        bytes     = pvrheader->bpp;
        mipmap = pvrheader->n_mipmap + 1;
        
        if( pvrheader->bitalpha )
        {
            compression = pvrheader->bpp == 4 ?
        GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
            GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
        }
        else
        {
            compression = pvrheader->bpp == 4 ?
        GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
            GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
        }
        
        texelArray = ( unsigned char * ) malloc( pvrheader->datasize );
        
        memcpy( texelArray,
               &file.data()[ sizeof( PVRHEADER ) ],
               pvrheader->datasize );
    }
}

/*!
	Helper function to automatically generate an OpenGLES texture.
	
	\param[in,out] texture A valid TEXTURE structure pointer.
	\param[in] flags The bit flags to use to generate the GLES texture.
	\param[in] filter The type of mipmap filtering to use.
	\param[in] anisotropic_filter The amount of anisotropic filtering to apply on the texture.
 */

//glActiveTexture(GL_TEXTURE0);
//glEnable(GL_TEXTURE_CUBE_MAP);
//glGenTextures(1, &texture);
//glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

void NKTexture::allocate(unsigned int flags, unsigned char filter, float anisotropic_filter, bool cube){
    
    clear();
    
    glGenTextures( 1, &glName);
    
    glBindTexture(target, glName);
    
    nkGetGLError();
    
    if( !compression )
    {
        switch( bytes )
        {
            case 1: glPixelStorei( GL_PACK_ALIGNMENT, 1 ); break;
            case 2: glPixelStorei( GL_PACK_ALIGNMENT, 2 ); break;
            case 3:
            case 4: glPixelStorei( GL_PACK_ALIGNMENT, 4 ); break;
        }
        
        if( flags & TEXTURE_16_BITS ) convert16Bits( flags & TEXTURE_16_BITS_5551 );
    }
    
    
    if( flags & TEXTURE_CLAMP )
    {
        glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
#if(!NK_USE_GLES)
        if (cube) {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
#endif
        
    }
    else
    {
        glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
#if(!NK_USE_GLES)
        if (cube) {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
        }
#endif
    }
    
    
    if( anisotropic_filter )
    {
        static float texture_max_anisotropy = 0.0f;
        
        if( !texture_max_anisotropy ) glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,
                                                  &texture_max_anisotropy );
        
        anisotropic_filter = CLAMP( anisotropic_filter,
                                   0.0f,
                                   texture_max_anisotropy );
        
        glTexParameterf( target,
                        GL_TEXTURE_MAX_ANISOTROPY_EXT,
                        anisotropic_filter );
    }
    
    
    if( flags & TEXTURE_MIPMAP )
    {
        switch( filter )
        {
            case TEXTURE_FILTER_1X:
            {
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                
                break;
            }
                
            case TEXTURE_FILTER_2X:
            {
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                
                break;
            }
                
            case TEXTURE_FILTER_3X:
            {
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                
                break;
            }
                
            case TEXTURE_FILTER_0X:
            default:
            {
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                
                break;
            }
        }
    }
    else
    {
        switch( filter )
        {
            case TEXTURE_FILTER_0X:
            {
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
                
                break;
            }
                
            default:
            {
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                
                break;
            }
        }
    }
    
    
    if( compression )
    {
       // nkLog("load compressed image");
        
        unsigned int i		= 0,
        size	= 0,
        offset = 0,
        bsize  = bytes == 4 ? 16 : 32,
        bwidth,
        bheight;
        
        while( i != mipmap )
        {
            if( width  < 1 ){ width  = 1; }
            if( height < 1 ){ height = 1; }
            
            bwidth = bytes == 4 ?
            width >> 2:
            width >> 3;
            
            bheight = height >> 2;
            
            size = bwidth * bheight * ( ( bsize * bytes ) >> 3 );
            
            if( size < 32 ){ size = 32; }
            
            glCompressedTexImage2D( target,
                                   i,
                                   compression,
                                   width,
                                   height,
                                   0,
                                   size,
                                   &texelArray[ offset ] );
            width  >>= 1;
            height >>= 1;
            
            offset += size;
            
            ++i;
        }
    }
    else
    {
        glTexImage2D(target, 0, internal_format,width,height,0,format,texelType,texelArray );
        nkGetGLError();
    }
    
#if !NK_USE_WEBGL
    if( flags & TEXTURE_MIPMAP && !mipmap ) glGenerateMipmap( target );
#endif
    
    nkGetGLError();
}

void NKTexture::convert16Bits(bool use_5551){
    
    unsigned int i = 0,
    s = width * height,
				*t = NULL;
    
    unsigned short *texel_array = NULL;
    
    switch( bytes )
    {
        case 3:
        {
            unsigned int j = 0;
            
            unsigned char *tmp_array = NULL;
            
            bytes		= 4;
            size		= s * bytes;
            texelType = GL_UNSIGNED_SHORT_5_6_5;
            
            tmp_array = ( unsigned char * ) malloc( size );
            
            while( i != size )
            {
                tmp_array[ i     ] = texelArray[ j     ];
                tmp_array[ i + 1 ] = texelArray[ j + 1 ];
                tmp_array[ i + 2 ] = texelArray[ j + 2 ];
                tmp_array[ i + 3 ] = 255;
                
                i += bytes;
                j += 3;
            }
            
            free( texelArray );
            texelArray = tmp_array;
            
            texel_array = ( unsigned short * )texelArray;
            
            t = ( unsigned int * )texelArray;
            
            i = 0;
            while( i != s )
            {
                *texel_array++ = ( ( (   *t         & 0xff ) >> 3 ) << 11 ) |
                ( ( ( ( *t >>  8 ) & 0xff ) >> 2 ) <<  5 ) |
                ( ( *t >> 16 ) & 0xff ) >> 3;
                ++t;
                ++i;
            }
            
            break;
        }
            
        case 4:
        {
            texel_array = ( unsigned short * )texelArray;
            
            t = ( unsigned int * )texelArray;
            
            if( use_5551 )
            {
                texelType = GL_UNSIGNED_SHORT_5_5_5_1;
                
                while( i != s )
                {
                    *texel_array++ = ( ( (   *t         & 0xff ) >> 3 ) << 11 ) |
                    ( ( ( ( *t >>  8 ) & 0xff ) >> 3 ) <<  6 ) |
                    ( ( ( ( *t >> 16 ) & 0xff ) >> 3 ) <<  1 ) |
                    ( ( *t >> 24 ) & 0xff ) >> 7;
                    ++t;
                    ++i;
                }
            }
            else
            {
                texelType = GL_UNSIGNED_SHORT_4_4_4_4;
                
                while( i != s )
                {
                    *texel_array++ = ( ( (   *t         & 0xff ) >> 4 ) << 12 ) |
                    ( ( ( ( *t >>  8 ) & 0xff ) >> 4 ) <<  8 ) |
                    ( ( ( ( *t >> 16 ) & 0xff ) >> 4 ) <<  4 ) |
                    ( ( *t >> 24 ) & 0xff ) >> 4;
                    ++t;
                    ++i;
                }
            }
            
            break;
        }
    }
}



/*!
	Delete the GLES texture id attached to the TEXTURE structure.
	
	\param[in,out] texture A valid TEXTURE structure pointer.
 */
void NKTexture::clear()
{
    if(glName)
    {
        glDeleteTextures( 1, &glName);
        glName = 0;
    }
}

//- (void)bindToUniform:(GLuint)uniform {
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(target, glName);
//    glUniform1i(uniform, 0);
//}
//
//- (void)enableAndBindToUniform:(GLuint)uniformSamplerLocation
//{
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(target, glName);
//    glUniform1i(uniformSamplerLocation, 0);
//}
//
//- (void)enableAndBindToUniform:(GLuint)uniformSamplerLocation atPosition:(int)textureNum
//{
//    assert(GL_TEXTURE1 == GL_TEXTURE0 + 1);
//    glActiveTexture(GL_TEXTURE0 + textureNum);
//    glBindTexture(target, glName);
//    glUniform1i(uniformSamplerLocation, textureNum);
//}
//
//-(void)dealloc {
//    glDeleteTextures(1, &glName);
//}
//
//@end

