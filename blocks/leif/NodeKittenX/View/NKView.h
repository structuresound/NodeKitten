//*
//*  NODE KITTEN
//*

#ifndef __NKView__
#define __NKView__


// NK VIEW IS AN ABSTRACT BASE CLASS
// SOME WILL BELONG TO NATIVE VIEWS
// OTHERS WILL REPERESENT THE BASE OF A SINGLE VIEW APP
// AND ENCOMPASS A MAIN() LIKE INFINITE LOOP FUNCTION

#include "NKTypes.h"
#include "NKEvent.h"

class NKFrameBuffer;
class NKTexture;
class NKMeshNode;
class NKShaderProgram;
class NKVertexBuffer;
class NKSceneNode;

class NKView {
    
    shared_ptr<NKSceneNode> _scene;
    S2t _size;
    
public:
    
    NKView(S2t size);
    
    U1t drawHitEveryXFrames = 10;
    U1t framesSinceLastHit {0};
    U1t rotate {0};
    
    D1t lastTime = 0;
    
    void setSize(S2t size);
    S2t size() {return _size;};
    
    shared_ptr<NKFrameBuffer> framebuffer;
    shared_ptr<NKTexture> texture;
    shared_ptr<NKShaderProgram> defaultShader;
    shared_ptr<NKVertexBuffer> vertexBuffer;
    shared_ptr<NKMeshNode> rect;

    virtual void draw();

    void setScene(shared_ptr<NKSceneNode> scene);

    void *nativeView;
    
    std::vector<NKEvent>events;
    
    void handleEvent(NKEvent& event);
};



#endif

