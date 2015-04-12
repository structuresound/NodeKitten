//*
//*  NODE KITTEN
//*

#ifndef __NKVertexBuffer_h_
#define __NKVertexBuffer_h_

#include "NKTypes.h"

typedef struct NKPrimitiveArray{
    V3t vertex;
    V3t normal;
    V2t texCoord;
    C4t color;
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

class NKVertexBuffer;

typedef std::function<SegmentSubdivision(F1t t)> SegmentSubdivider;
typedef std::function<void(NKVertexBuffer*)> VertexUpdateBlock;
typedef std::function<void(void)> GeometrySetupBlock;

class NKIndexBuffer
{
    
    vector<U1t> _data;
    GLuint _elementBuffer {0};
    
    public :
    
    NKIndexBuffer(){
#warning not using object for this right now
        //glGenBuffers(1, &_elementBuffer);
    }
    
    NKIndexBuffer(vector<U1t> data) : _data(data){
//        glGenBuffers(1, &_elementBuffer);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                     _data.size(),
//                     &_data.front(),
//                     GL_STATIC_DRAW);
    };
    
    NKIndexBuffer(U1t* data, U1t length) {
//        for (int i = 0; i < length; i++) {
//            _data.push_back(data[i]);
//        }
//        glGenBuffers(1, &_elementBuffer);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                     _data.size(),
//                     &_data.front(),
//                     GL_STATIC_DRAW);
    };
    
    ~NKIndexBuffer(){
//        glDeleteBuffers(1, &_elementBuffer);
    }
    
    
    ULt size() {return _data.size();};
    U1t* data(){return &_data.front();}
    
    void bind(){
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
    };
    void unbind(){
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void update() {
        
    }
    
    void updateBuffer(){
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
//        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
//                        0,
//                        _data.size(),
//                        &_data.front());
    }
};

class NKVertexBuffer

{
    
private:
    vector<V3t> _vertices;
    vector<V3t> _normals;
    vector<V3t> _texCoords;
    vector<V4t> _colors;
    
    V3Stack _tangents;
    V3Stack _biNormals;
    F1Stack _boneWeights;
    M16Stack _boneTransforms;
    
    vector<U1t> _indices;
    
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
    
    F1Stack _lineWidths;
    
public:
    
    NKVertexBuffer(NKPrimitive primitive, GLenum bufferMode = 0);

    NKVertexBuffer(){
        _updateBlock = [](NKVertexBuffer* buffer){
            buffer->update();
        };
    }
    
    ~NKVertexBuffer();
    
    // STATICS

    static map<string, shared_ptr<NKVertexBuffer>> vboCache;
    static map<NKPrimitive, shared_ptr<NKVertexBuffer>> vboPrimitiveCache;
    static shared_ptr<NKVertexBuffer> cachedVbo(string vbo);
    
    static shared_ptr<NKVertexBuffer> axes();
    static shared_ptr<NKVertexBuffer> defaultRect();
    static shared_ptr<NKVertexBuffer> defaultCube();
    static shared_ptr<NKVertexBuffer> cube8v();
    static shared_ptr<NKVertexBuffer> edges8v();
    static shared_ptr<NKVertexBuffer> sphereWithStacks(GLint stacks, GLint slices, GLfloat squash);
    static shared_ptr<NKVertexBuffer> planeWithSubdivisions(GLint horizontal, GLint vertical);
    
    NKVertexBuffer(GLint stacks, GLint slices, GLfloat squash);
    NKVertexBuffer(GLint horizontal, GLint vertical);
    NKVertexBuffer(F1t width, F1t height, F1t depth, int resX, int resY, int resZ );
    
    static GeometrySetupBlock primitiveSetupBlock();
    static GeometrySetupBlock riggedMeshSetupBlock();

    // IVARS
    
    bool _dirty {true};
    NKIndexBuffer indexBuffer;
    V3t _center;

    ULt size(){return _vertices.size();}
    
    const V3t* vertices() const {return &_vertices.front();}//_vertices.data();}
//    void setVertices(V3Stack vertices){
//        _vertices = vertices;
//    }
//    
    const V4t* colors() const {return &_colors.front();}
//    void setColors(V4Stack colors){
//        _colors = colors;
//    }
    
    void interlaceAndBuffer(GLenum bufferMode = 0);
    void bufferData(GLsizeiptr size,
                    const GLvoid * data,
                    GLenum mode);
    
    
    
    void update();
    void updateBuffer();
    void bind();
    void unbind();
    
    void setLineWidth(F1t lineWidth) {
        _lineWidths.append(lineWidth);
    }
    
    // FOR LOD
    
    int* elementOffset {0};
    int* elementSize {0};
    
    V6t boundingBox(){
        return boundingSizeForVertexSet(vector<NKVertexBuffer*>{this});
    }
    V3t centerOfBoundingSize(V6t box);
    V3t sizeOfBoundingSize(V6t box);
    
private: // FUNCTIONS

    V6t boundingSizeForVertexSet(vector<NKVertexBuffer*> set);
    void normalizeAndCenter();
    V3t normalizeForGroupWithSize(F1t unitSize, V6t groupBoundingBox, bool center);
    U1t numberOfSegmentsForBezier(B12t* bezier);
    SegmentSubdivider subdividerForBezier(B12t* bez);
    void updateBufferWithBezier(V3Stack& bezier , F1Stack& lineWidths);
};

#endif

