//*
//*  NODE KITTEN
//*

#ifndef __VertexBuffer_h_
#define __VertexBuffer_h_

#include "CommonTypes.h"

typedef struct NKPrimitiveArray{
    V3t vertex;
    V3t normal;
    V2t texCoord;
    V4t color;
} NKPrimitiveArray;

typedef struct NKRiggedStruct {
    V3t vertex;
    V3t normal;
    V3t texCoord;
    V3t tangent;
    V3t biNormal;
    F1t boneWeight;
} NKRiggedStuct;

typedef struct NKMaterial{
    V3t diffuse;
    V3t ambient;
    V3t specular;
    V3t emissive;
    F1t shininess;
    U1t texCount;
} NKMaterial;

typedef enum NKPrimitive {
    NKPrimitiveNone,
    NKPrimitiveAxes,
    NKPrimitiveRect,
    NKPrimitivePlane,
    NKPrimitiveCube,
    NKPrimitive8VCube,
    NKPrimitiveBoundingBox,
    NKPrimitiveSphere,
    NKPrimitiveLODSphere,
    NKNumPrimitives
} NKPrimitive;


typedef V6t SegmentSubdivision;

class VertexBuffer;

typedef std::function<SegmentSubdivision(F1t t)> SegmentSubdivider;
typedef std::function<void(VertexBuffer*)> VertexUpdateBlock;
typedef std::function<void(void)> GeometrySetupBlock;

class VertexBuffer

{
    
private:


    
    V3Stack _tangents;
    V3Stack _biNormals;
    F1Stack _boneWeights;
    M16Stack _boneTransforms;

    int _verticesOffset {0};
    int _normalsOffset {0};
    int _texCoordsOffset {0};
    int _colorsOffset {0};
    int _tangentsOffset {0};
    int _biNormalsOffset {0};
    int _boneWeightsOffset {0};
    
    int _stride {0};
    
    V3t _boundingBoxSize;
    
    GeometrySetupBlock _geometrySetupBlock {nullptr};
    VertexUpdateBlock _updateBlock {nullptr};
    
    GLuint _vertexArray {0};
    GLuint _vertexBuffer {0};
    GLuint _elementArray {0};
    
    F1Stack _lineWidths {0};
    
    GLenum _bufferMode {GL_STATIC_DRAW};
    
public:
    
    vector<V3t> _vertices;
    vector<V3t> _normals;
    vector<V3t> _texCoords;
    vector<V4t> _colors;
    vector<U1t> _indices;

    
    VertexBuffer(NKPrimitive primitive, GLenum bufferMode = GL_STATIC_DRAW);

    VertexBuffer(){
        _updateBlock = [](VertexBuffer* buffer){
            buffer->update();
        };
    }
    
    ~VertexBuffer();
    
    // STATICS

    static GLuint activeVAO;
    static VertexBuffer *activeVBO;
    
    static map<string, shared_ptr<VertexBuffer>> vboCache;
    static map<NKPrimitive, shared_ptr<VertexBuffer>> vboPrimitiveCache;
    static shared_ptr<VertexBuffer> cachedVbo(string vbo);
    
    static shared_ptr<VertexBuffer> axes();
    static shared_ptr<VertexBuffer> defaultRect();
    static shared_ptr<VertexBuffer> defaultCube();
    static shared_ptr<VertexBuffer> cube8v();
    static shared_ptr<VertexBuffer> edges8v();
    static shared_ptr<VertexBuffer> sphereWithStacks(GLint stacks, GLint slices, GLfloat squash);
    static shared_ptr<VertexBuffer> planeWithSubdivisions(GLint horizontal, GLint vertical);
    
    VertexBuffer(GLint stacks, GLint slices, GLfloat squash);
    VertexBuffer(GLint horizontal, GLint vertical);
    VertexBuffer(F1t width, F1t height, F1t depth, int resX, int resY, int resZ );
    
    static GeometrySetupBlock primitiveSetupBlock();
    static GeometrySetupBlock riggedMeshSetupBlock();

    // IVARS
    
    bool _dirty {true};
    V3t _center;

    ULt size(){return _vertices.size();}
    
    const V3t* vertices() const {return &_vertices.front();}//_vertices.data();}
    const V4t* colors() const {return &_colors.front();}
    
    void gen();
    
    void bind();
    void bindVBO();
    void bindVAO();
    void bindEAO();
    
    void unbind();
    void unbindVBO();
    void unbindVAO();
    void unbindEAO();
    
    void clear();
    
    void setLineWidth(F1t lineWidth) {
        _lineWidths.append(lineWidth);
    }
    
    // FOR LOD
    
    int* elementOffset {0};
    int* elementSize {0};
    
    V6t boundingBox(){
        return boundingSizeForVertexSet(vector<VertexBuffer*>{this});
    }
    V3t centerOfBoundingSize(V6t box);
    V3t sizeOfBoundingSize(V6t box);
    
    void update();
    
    void setupVAOInterlaceAndBufferVBO();
    void interlaceAndBufferVBO();
    void bufferVBO(GLsizeiptr size,
                   const GLvoid * data);
    void bufferEAO();
    
    
    
private: // FUNCTIONS

    V6t boundingSizeForVertexSet(vector<VertexBuffer*> set);
    void normalizeAndCenter();
    V3t normalizeForGroupWithSize(F1t unitSize, V6t groupBoundingBox, bool center);
    U1t numberOfSegmentsForBezier(B12t* bezier);
    SegmentSubdivider subdividerForBezier(B12t* bez);
    void updateBufferWithBezier(V3Stack& bezier , F1Stack& lineWidths);
    
    // INDEX BUFFER

};

#endif

