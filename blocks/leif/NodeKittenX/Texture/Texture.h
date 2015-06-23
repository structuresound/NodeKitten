//
//  Texture.h
//  NodeKittenExample
//
//  Created by Chroma Developer on 3/5/14.
//  Copyright (c) 2014 Chroma. All rights reserved.
//

#ifndef __Texture_h_
#define __Texture_h_

#include "FrameBuffer.h"
#include "FileUtil.h"

class Node;

enum {
    // Enable image clamping on the UV, 0 to 1 only no texture repeat.
            NK_TEXTURE_CLAMP = ( 1 << 0 ),
    //! Enable automatic mipmap generation.
            NK_TEXTURE_MIPMAP = ( 1 << 1 ),
    //! Enable automatic 16 bits conversion.
            NK_TEXTURE_16_BITS = ( 1 << 2 ),
    //! Force the conversion of 32 bits textures to use use 5551 instead of 4444.
            NK_TEXTURE_16_BITS_5551 = ( 1 << 3 )
};
enum {
    //! Image filtering nearest.
            NK_TEXTURE_FILTER_0X = 0,
    //! Image filtering linear.
            NK_TEXTURE_FILTER_1X = 1,
    //! Bilinear image filtering.
            NK_TEXTURE_FILTER_2X = 2,
    //! Trilinear image filtering.
            NK_TEXTURE_FILTER_3X = 3
};



class Texture
{
    string _name;
    
public:
    
    static shared_ptr<Texture> blankTexture();
    
    static shared_ptr<Texture> textureWithImageFile(string filePath,bool relative_path = true, U1t flags = NK_TEXTURE_CLAMP, U1t filter = 0, F1t anistropicFilter= 0);
    
    GLuint glName {0};
    
    U1t width {0};
    U1t height {0};
    
    //TextureMapStyle textureMapStyle {TextureMapStyleNone};
    //! The texture bits.
    unsigned char	bytes {0};
    //! The total size of the texture in bytes.
    unsigned int	size {0};
    //! The texture target
    GLenum target {GL_TEXTURE_2D};
    //! The internal format of the texture.
    unsigned int	internal_format {0};
    //! The format of the texture.
    unsigned int	format {0};
    //! The type of texel to use for this texture.
    unsigned int	texelType {GL_UNSIGNED_BYTE};
    //! The raw texel array.
    unsigned char	*texelArray {0};
    //! The number of mipmaps (PVRTC only).
    unsigned int	mipmap {0};
    //! The compression type.
    unsigned int	compression {0};
    
    // INIT WITH DATA
    Texture(string filename, bool relative_path = true, U1t flags = 0, U1t filter = 0, F1t anistropicFilter= 0, bool cube = 0);
    
    // INIT FOR FBO
    Texture(I1t width_, I1t height_){
        width = width_;
        height = height_;

        genGLTexture(width, height);
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
        glTexImage2D( target, 0, GL_RGBA, width, height,
                     0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, NULL );
#else
        glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#endif
        
        GetGLError();
    }

    Texture() : Texture(1,1){
    }
    
    ~Texture(){};
    
    void loadPng(File& file);
    void loadJpg(File& file);
    void loadPvr(File& file);
    //void loadTex();
    void convert16Bits( bool use_5551 );
    void allocate(unsigned int flags, unsigned char filter, float anisotropic_filter, bool cube);
    void clear();

    string name(){return _name;};
    
    bool shouldResizeToTexture;
    bool keepInMemory;
    void genGLTexture(int width, int height);


    //+(instancetype) textureWithImageNamed:(NSString*)name fromBundleNamed:(NSString*)bundleName;
    //+(instancetype) textureWithImageNamed:(NSString*)name;
    //+(instancetype) textureWithImage:(NKImage*)image;
    //+(instancetype) textureWithFrameBuffer:(FrameBuffer*)frameBuffer;
    //
    //+(instancetype) textureWithString:(NSString *)string ForLabelNode:(Label*)node;
    //+(instancetype) textureWithString:(NSString *)string ForLabelNode:(Label *)node inBackGroundWithCompletionBlock:(void (^)())block;
    //
    //-(void)updateWithTimeSinceLast:(F1t) dt;
    //

    virtual void bind();
    void bindToUniform(GLuint uLocation);
    void enableAndBind(int tLocation);
    void enableAndBindToUniform(GLuint uSamplerLocation, GLuint tLocation = 0);
    
    void unbind();
    
    
    //
    //+(Texture*)blankTexture;
    //
    //-(GLuint)glName;
    //-(GLuint)glTarget;
    //
    //-(void)setGlName:(GLuint)loc;
    //
    //-(void)unload;
    
    // *** CACHE *** //
    
    static map <string, shared_ptr<Texture>> fontCache;
    static map <string, shared_ptr<Texture>> nameCache;
    static map <shared_ptr<Texture>, vector<shared_ptr<Node>>> sharedTextures;
    
    static bool registerNode(shared_ptr<Node> node);
    static bool unresgisterNode(shared_ptr<Node> node);

};

//! PVRTC file header data.
typedef struct
{
    //! The size of the PVRTC header.
    unsigned int headersize;
    
    //! The height of the original texture size.
    unsigned int height;
    
    //! The width of the original texture size.
    unsigned int width;
    
    //! The number of number of mipmap contained in the PVR image stream.
    unsigned int n_mipmap;
    
    //! The flags of the PVR image.
    unsigned int flags;
    
    //! Data block size.
    unsigned int datasize;
    
    //! The number of bits per pixel.
    unsigned int bpp;
    
    //! The size of the red bit.
    unsigned int bitred;
    
    //! The size of the green bit.
    unsigned int bitgreen;
    
    //! The size of the blue bit.
    unsigned int bitblue;
    
    //! The size of the alpha bit.
    unsigned int bitalpha;
    
    //! Unknown.
    unsigned int tag;
    
    //! The number of surface held in the PVR file.
    unsigned int n_surface;
    
} PVRHEADER;

#endif
