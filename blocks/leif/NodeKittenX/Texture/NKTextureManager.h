//
//  NKTextureManager.h
//  nike3dField
//
//  Created by Chroma Developer on 3/27/14.
//
//

#include "NKTypes.h"

#if (TARGET_IOS || TARGET_OSX)
#import <CoreVideo/CoreVideo.h>
#endif

class NKTexture;

class NKTextureManager {
public:
    static NKTextureManager* sharedInstance();
    
#if TARGET_IOS
    CVOpenGLESTextureCacheRef videoTextureCache;
#elif TARGET_OSX
    CVOpenGLTextureCacheRef videoTextureCache;
#endif
    
    GLuint defaultTexture;
    
    int videosPlaying;
    
private:
    NKTextureManager();
    static NKTextureManager* pSingleton;		// singleton instance
    
    
//    std::map<NKTexture> textureCache;
//    std::map<NKTexture> textureNodeMap;
//    std::map<NKTexture> labelCache;
    
    
    
//    + (void)purgeVideoTextures;
//    + (NKTextureManager *)sharedInstance;
//    + (NSMutableDictionary*) textureCache;
//    + (dispatch_queue_t) textureThread;
//    
//    + (void)addNode:(NKNode*)node forTexture:(NKTexture*)texture;
//    + (void)removeNode:(NKNode*)node forTexture:(NKTexture*)texture;
//    
//    + (NSString*)pathForFileNamed:(NSString *)name ext:(NSString*)ext;
//    + (NSMutableDictionary*) labelCache;
//    + (GLuint) defaultTextureLocation;

};
