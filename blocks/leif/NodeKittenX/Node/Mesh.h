//
//  Mesh.h
//  nike3dField
//
//  Created by Chroma Developer on 3/31/14.
//
//

#ifndef __Mesh__
#define __Mesh__


#include "Node.h"
#include "Texture.h"

class Mesh : public Node
{
    M16t *boneOffsets {nullptr};
    V6t _aabb;
    V6t getAabb();
    
    U1t _numTris {0};
    
    // Static
    
    static Mesh *_fboSurface;
    
protected:
    shared_ptr<VertexBuffer> _vertexBuffer;
    vector<shared_ptr<Texture>> _textures;
    U1t _numTextures {0};
    GLenum _drawMode {GL_TRIANGLES};
    NKPrimitive _primitiveType {NKPrimitiveNone};
    
    // SHARED MESHES
    
    static Mesh& fboSurface();
public:
    
    // CLASS Methods
    
    static string stringForPrimitive(NKPrimitive primitive);
    
    // STATIC DRAWING
    
    static void fillRect(Scene* context, V4 rect, Color color = Color(1.0,1.0), shared_ptr<Texture> texture = nullptr);
    static void glInit();
    
    // LOCAL ATTRIBUTES
    
    bool shouldDrawBoundingBox = false;
    float pointSize {1.0};
    float lineWidth {1.0};
    bool usesLOD {false};
    int lodForDistance();
    
    V6t aabb(){
        return getAabb();
    };
    
    VertexBuffer* vertexBuffer(){return _vertexBuffer.get();};
    
    // OBJECT FACTORIES
    static shared_ptr<Mesh> nodeWithObj(string objName, V3t size = V3(1.0), bool normalize = true, bool anchor = false);
    static shared_ptr<Mesh> nodeWithPrimitive(NKPrimitive primitive, shared_ptr<Texture> texture, Color color, V3t size);
    static shared_ptr<Mesh> nodeWithVbo(shared_ptr<VertexBuffer> buffer, GLenum drawMode, shared_ptr<Texture> texture, Color color, V3t size);

    // INIT
    Mesh(shared_ptr<Texture> texture = nullptr, Color color = Color(1.0), V3t size = V3(1.0)); // COMMON CONSTRUCTOR
    Mesh(string objName, V3t size = V3(1.0), bool normalize = true, bool anchor = false);
    Mesh(NKPrimitive primitive, shared_ptr<Texture> texture, Color color, V3t size);
    Mesh(shared_ptr<VertexBuffer> buffer, GLenum drawMode, shared_ptr<Texture> texture, Color color, V3t size);
    
    ~Mesh();
    
    virtual void setTexture(shared_ptr<Texture> texture);
    virtual void chooseShader() override;
    void bindTextures();
    
    void setDrawMode(GLenum drawMode){_drawMode = drawMode;};
    
    // Node Overrides
    
    virtual void setColor(Color color) override;
    void customDraw() override;
    void setupViewMatrix() override;
    void customDrawWithHitShader() override;
    
    // Functions
    
    void drawBoundingBox();
    
    V6t boundingBox(){return _vertexBuffer->boundingBox();};
};

#endif
