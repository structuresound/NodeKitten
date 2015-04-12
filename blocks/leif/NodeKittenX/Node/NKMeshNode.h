//
//  NKMeshNode.h
//  nike3dField
//
//  Created by Chroma Developer on 3/31/14.
//
//

#ifndef __NKMeshNode__
#define __NKMeshNode__


#include "NKNode.h"
#include "NKTexture.h"

class NKMeshNode : public NKNode
{
    M16t *boneOffsets {nullptr};

   
    V6t _aabb;
    V6t getAabb();
    
    U1t _numTris = 0;
    
    // Static
    
    static NKMeshNode* _fboSurface;
    
protected:
    shared_ptr<NKVertexBuffer> _vertexBuffer;
    vector<shared_ptr<NKTexture>> _textures;
    U1t _numTextures {0};
    GLenum _drawMode {0};
     NKPrimitive _primitiveType {NKPrimitiveNone};
public:
    
    // CLASS Methods
    
    static string stringForPrimitive(NKPrimitive primitive);
    static NKMeshNode& fboSurface();
    
    // NSDictionary *animations;
    bool shouldDrawBoundingBox = false;
    
    float pointSize {1.0};
    float lineWidth {1.0};
    bool usesLOD {false};
    int lodForDistance();
    
    bool forceOrthographic {false};
    
    V6t aabb(){
        return getAabb();
//        if (_dirty){
//            _aabb = getAabb();
//        }
//        return _aabb;
    };
    
    NKVertexBuffer* vertexBuffer(){return _vertexBuffer.get();};
    
    static shared_ptr<NKMeshNode> meshNodeWithObj(string objName, V3t size = V3MakeF(1.0), bool normalize = true, bool anchor = false);
    static shared_ptr<NKMeshNode> meshNodeWithPrimitive(NKPrimitive primitive, shared_ptr<NKTexture> texture, NKColor color, V3t size);
    static shared_ptr<NKMeshNode> meshNodeWithVbo(shared_ptr<NKVertexBuffer> buffer, GLenum drawMode, shared_ptr<NKTexture> texture, NKColor color, V3t size);

    NKMeshNode();

    NKMeshNode(shared_ptr<NKTexture> texture, NKColor color, V3t size); // COMMON CONSTRUCTOR
    NKMeshNode(string objName, V3t size = V3MakeF(1.0), bool normalize = true, bool anchor = false);
    NKMeshNode(NKPrimitive primitive, shared_ptr<NKTexture> texture, NKColor color, V3t size);
    NKMeshNode(shared_ptr<NKVertexBuffer> buffer, GLenum drawMode, shared_ptr<NKTexture> texture, NKColor color, V3t size);
    
    ~NKMeshNode();
    
    virtual void setTexture(shared_ptr<NKTexture> texture);
    virtual void chooseShader();
    void bindTextures();
    
    void setDrawMode(GLenum drawMode){_drawMode = drawMode;};
    
    // NKNode Overrides
    
    virtual void setColor(NKColor color) override;
    virtual void setScene(NKSceneNode* scene) override;
    void setupViewMatrix() override;
    void customDraw() override;
    void drawBoundingBox();
    void customDrawWithHitShader() override;
};

#endif
