//
//  Mesh.h
//  nike3dField
//
//  Created by Chroma Developer on 3/31/14.
//
//

#pragma once

#include "node.h"
#include "texture.h"
#include "vertexBuffer.h"

class Mesh : public Node
{
    M16t *boneOffsets {nullptr};
    V6t _aabb;
    V6t getAabb() const;

    U1t _numTris {0};

    // Static

    static Mesh *_fboSurface;

protected:
    std::shared_ptr<VertexBuffer> _vertexBuffer;
    std::vector<std::shared_ptr<Texture>> _textures;
    U1t _numTextures {0};
    GLenum _drawMode {GL_TRIANGLES};
    NKPrimitive _primitiveType {NKPrimitiveNone};

    // SHARED MESHES

    static Mesh& fboSurface();
public:

    // CLASS Methods

    static std::string stringForPrimitive(NKPrimitive primitive);

    // STATIC DRAWING

    static void fillRect(Scene* context, V4 rect, Color color = Color(1.0,1.0), std::shared_ptr<Texture> texture = nullptr);
    static void glInit();

    // LOCAL ATTRIBUTES

    bool shouldDrawBoundingBox = false;
    float pointSize {1.0};
    float lineWidth {1.0};
    bool usesLOD {false};
    int lodForDistance() const;

    V6t aabb() const{
        return getAabb();
    };

    VertexBuffer* vertexBuffer(){return _vertexBuffer.get();};

    // OBJECT FACTORIES
    static std::shared_ptr<Mesh> nodeWithObj(std::string objName, V3t size = V3(1.0), bool normalize = true, bool anchor = false);
    static std::shared_ptr<Mesh> nodeWithPrimitive(NKPrimitive primitive, std::shared_ptr<Texture> texture, Color color, V3t size);
    static std::shared_ptr<Mesh> nodeWithVbo(std::shared_ptr<VertexBuffer> buffer, GLenum drawMode, std::shared_ptr<Texture> texture, Color color, V3t size);

    // INIT
    Mesh(std::shared_ptr<Texture> texture = nullptr, Color color = Color(1.0), V3t size = V3(1.0)); // COMMON CONSTRUCTOR
    Mesh(std::string objName, V3t size = V3(1.0), bool normalize = true, bool anchor = false);
    Mesh(NKPrimitive primitive, std::shared_ptr<Texture> texture, Color color, V3t size);
    Mesh(std::shared_ptr<VertexBuffer> buffer, GLenum drawMode, std::shared_ptr<Texture> texture, Color color, V3t size);

    ~Mesh();

    virtual void setTexture(std::shared_ptr<Texture> texture);
    virtual void chooseShader() override;
    void bindTextures();

    void setDrawMode(GLenum drawMode){_drawMode = drawMode;};

    // Node Overrides

    void customDraw() override;
    void setupViewMatrix() const override;
    void customDrawWithHitShader() const override;

    // Functions

    void drawBoundingBox();

    V6t boundingBox(){return _vertexBuffer->boundingBox();};
};
