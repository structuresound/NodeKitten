//
//  NKTexture.h
//  NodeKittenExample
//
//  Created by Chroma Developer on 3/5/14.
//  Copyright (c) 2014 Chroma. All rights reserved.
//

#ifndef __NKTexture_h_
#define __NKTexture_h_

#include "NKFrameBuffer.h"
#include "NKFileSystem.h"

class NKNode;

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



class NKTexture
{
    string _name;
    
public:
    
    static shared_ptr<NKTexture> blankTexture();
    
    static shared_ptr<NKTexture> textureWithImageFile(string filePath,bool relative_path = true, U1t flags = NK_TEXTURE_CLAMP, U1t filter = 0, F1t anistropicFilter= 0);
    
    GLuint glName {0};
    
    U1t width {0};
    U1t height {0};
    
    //NKTextureMapStyle textureMapStyle {NKTextureMapStyleNone};
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
    NKTexture(string filename, bool relative_path = true, U1t flags = 0, U1t filter = 0, F1t anistropicFilter= 0, bool cube = 0);
    
    // INIT FOR FBO
    NKTexture(I1t width_, I1t height_){
        width = width_;
        height = height_;

        genGLTexture(width, height);
        glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        GetGLError();
    }

    NKTexture(){
    }
    
    ~NKTexture(){};
    
    void loadPng(NKFile& file);
    void loadJpg(NKFile& file);
    void loadPvr(NKFile& file);
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
    //+(instancetype) textureWithFrameBuffer:(NKFrameBuffer*)frameBuffer;
    //
    //+(instancetype) textureWithString:(NSString *)string ForLabelNode:(NKLabelNode*)node;
    //+(instancetype) textureWithString:(NSString *)string ForLabelNode:(NKLabelNode *)node inBackGroundWithCompletionBlock:(void (^)())block;
    //
    //-(void)updateWithTimeSinceLast:(F1t) dt;
    //

    virtual void bind();
    void bindToUniform(GLuint uLocation);
    void enableAndBind(int tLocation);
    void enableAndBindToUniform(GLuint uSamplerLocation, GLuint tLocation = 0);
    
    void unbind();
    
    
    //
    //+(NKTexture*)blankTexture;
    //
    //-(GLuint)glName;
    //-(GLuint)glTarget;
    //
    //-(void)setGlName:(GLuint)loc;
    //
    //-(void)unload;
    
    // *** CACHE *** //
    
    static map <shared_ptr<NKTexture>, string> cachedNamed;
    static map <shared_ptr<NKTexture>, vector<shared_ptr<NKNode>>> sharedTextures;
    
    static bool registerNode(shared_ptr<NKNode> node);
    static bool unresgisterNode(shared_ptr<NKNode> node);

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
