//
//  NKAttributeBuffer.m
//  NKNikeField
//
//  Created by Leif Shackelford on 5/1/14.
//  Copyright (c) 2014 Chroma Developer. All rights reserved.
//

#include "NKVertexBuffer.h"
#include "NKShaderTools.h"
#include "NKFrameBuffer.h"

#pragma mark - Convenience Initializers

// statics

map<string, shared_ptr<NKVertexBuffer>> NKVertexBuffer::vboCache;
map<NKPrimitive, shared_ptr<NKVertexBuffer>> NKVertexBuffer::vboPrimitiveCache;

// Factories

shared_ptr<NKVertexBuffer> NKVertexBuffer::defaultRect() { // SPRITE
    
    if (NKVertexBuffer::vboPrimitiveCache[NKPrimitiveRect]) return vboPrimitiveCache[NKPrimitiveRect];
    
    vboPrimitiveCache[NKPrimitiveRect] = make_shared<NKVertexBuffer>(NKPrimitiveRect);
    
    return vboPrimitiveCache[NKPrimitiveRect];
}

shared_ptr<NKVertexBuffer> NKVertexBuffer::axes() { // XYZ
    
    if (NKVertexBuffer::vboPrimitiveCache[NKPrimitiveAxes]) return vboPrimitiveCache[NKPrimitiveAxes];
    
    vboPrimitiveCache[NKPrimitiveAxes] = make_shared<NKVertexBuffer>(NKPrimitiveAxes);
    
    return vboPrimitiveCache[NKPrimitiveAxes];
}

shared_ptr<NKVertexBuffer> NKVertexBuffer::defaultCube() {
    
    if (NKVertexBuffer::vboPrimitiveCache[NKPrimitiveCube]) return vboPrimitiveCache[NKPrimitiveCube];
    
    vboPrimitiveCache[NKPrimitiveCube] = make_shared<NKVertexBuffer>(NKPrimitiveCube);
    
    return  vboPrimitiveCache[NKPrimitiveCube];
}

shared_ptr<NKVertexBuffer> NKVertexBuffer::cube8v() {
    
    if (NKVertexBuffer::vboPrimitiveCache[NKPrimitive8VCube]) return vboPrimitiveCache[NKPrimitive8VCube];
    
    vboPrimitiveCache[NKPrimitive8VCube] = make_shared<NKVertexBuffer>(NKPrimitive8VCube);
    
    return  vboPrimitiveCache[NKPrimitive8VCube];
}

shared_ptr<NKVertexBuffer> NKVertexBuffer::edges8v() {
    
    if (NKVertexBuffer::vboPrimitiveCache[NKPrimitiveBoundingBox]) return vboPrimitiveCache[NKPrimitiveBoundingBox];
    
    vboPrimitiveCache[NKPrimitiveBoundingBox] = make_shared<NKVertexBuffer>(NKPrimitiveBoundingBox);
    
    return  vboPrimitiveCache[NKPrimitiveBoundingBox];
}

shared_ptr<NKVertexBuffer> NKVertexBuffer::sphereWithStacks(GLint stacks, GLint slices, GLfloat squash)
{
    if (NKVertexBuffer::vboPrimitiveCache[NKPrimitiveSphere]) return vboPrimitiveCache[NKPrimitiveSphere];
    
    vboPrimitiveCache[NKPrimitiveSphere] = make_shared<NKVertexBuffer>(stacks, slices, squash);;
    
    return vboPrimitiveCache[NKPrimitiveSphere];
}

shared_ptr<NKVertexBuffer> NKVertexBuffer::planeWithSubdivisions(GLint horizontal, GLint vertical){
    
    if (NKVertexBuffer::vboPrimitiveCache[NKPrimitivePlane]) return vboPrimitiveCache[NKPrimitivePlane];
    
    vboPrimitiveCache[NKPrimitivePlane] = make_shared<NKVertexBuffer>(horizontal, vertical);
    
    return vboPrimitiveCache[NKPrimitivePlane];
}


NKVertexBuffer::NKVertexBuffer(GLint rows, GLint columns){
    
    V3t vert;
    V3t normal {0, 0, 1}; // always facing forward //
    V3t texcoord {0, 0, 0};
    
    int mode = 1;
    
    auto w = 1.0f;
    auto h = 1.0f;
    // the origin of the plane is the center //
    float halfW = w/2.f;
    float halfH = h/2.f;
    // add the vertexes //
    for(int iy = 0; iy < rows; iy++) {
        for(int ix = 0; ix < columns; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)columns-1.f));
            texcoord.y = ((float)iy/((float)rows-1.f));
            
            vert.x = texcoord.x * w - halfW;
            vert.y = texcoord.y * h - halfH;
            
            _vertices.push_back(vert);
            _texCoords.push_back(texcoord);
            _colors.push_back({texcoord.x, texcoord.y, 1, 1});
            _normals.push_back(normal);
        }
    }
    if(mode == 1) { // TRIANGLE STRIP
        for(int y = 0; y < rows-1; y++) {
            // even rows //
            if((y&1)==0) {
                for(int x = 0; x < columns; x++) {
                    _indices.push_back( (y) * columns + x );
                    _indices.push_back( (y+1) * columns + x);
                }
            } else {
                for(int x = columns-1; x >0; x--) {
                    _indices.push_back( (y+1) * columns + x );
                    _indices.push_back( y * columns + x-1 );
                }
            }
        }
        
        if(rows%2!=0) _indices.push_back((U1t)_vertices.size()-columns);
    } else {
        // Triangles //
        for(int y = 0; y < rows-1; y++) {
            for(int x = 0; x < columns-1; x++) {
                // first triangle //
                _indices.push_back((y)*columns + x);
                _indices.push_back((y)*columns + x+1);
                _indices.push_back((y+1)*columns + x);
                
                // second triangle //
                _indices.push_back((y)*columns + x+1);
                _indices.push_back((y+1)*columns + x+1);
                _indices.push_back((y+1)*columns + x);
            }
        }
    }
    
    
    //    rows++;
    //    columns++;
    //    // Set up vertices
    //    for(int y = 0; y < rows; y++)
    //    {
    //        // int base = y * columns;
    //        for(int x = 0; x < columns; x++)
    //        {
    //            _vertices.push_back({(float) x / (F1t)columns - 0.5f, (float) y / (F1t)rows - 0.5f, 0});
    //            _texCoords.push_back({(float) x / (F1t)columns, (float) y / (F1t)rows, 0});
    //            _normals.push_back({ 1.0f, 0.0f, 0.0f});
    //            _colors.push_back({(float) x / (F1t)columns, (float) y / (F1t)rows, 1, 1});
    //        }
    //    }
    //
    //    nkLog("-------------------------");
    //
    //    // Set up indices
    //    rows--;
    //    for(int y = 0; y < rows; y++)
    //    {
    //        int base = y * columns;
    //
    //        //_indices.push_back(base);
    //        for(int x = 0; x < columns; x++)
    //        {
    //            _indices.push_back((base + x));
    //            _indices.push_back((base + columns + x));
    //        }
    //        // add a degenerate triangle (except in a last row)
    //        if(y < rows - 1)
    //        {
    //            _indices.push_back((y + 1) * columns + (columns - 1));
    //            _indices.push_back((y + 1) * columns);
    //        }
    //    }
    //
    //    for(int ind=0; ind < _indices.size(); ind++)
    //        nkLog("%d ", _indices[ind]);
    //
    //        // Set up vertices
    //        for (int r = 0; r < rows; ++r) {
    //            for (int c = 0; c < columns; ++c) {
    //                _vertices.push_back({(float) c / (F1t)columns - 0.5f, (float) r / (F1t)rows - 0.5f, 0});
    //                _texCoords.push_back({(float) c / (F1t)columns, (float) r / (F1t)rows, 0});
    //                _normals.push_back({ 1.0f, 0.0f, 0.0f});
    //                _colors.push_back({(float) c / (F1t)columns, (float) r / (F1t)rows, 1, 1});
    //            }
    //        }
    //
    //        // Set up indices
    //        //int i = 0;
    //        for (int r = 0; r < rows - 1; ++r) {
    //            _indices.push_back(r * columns);
    //            for (int c = 0; c < columns; ++c) {
    //                _indices.push_back(r * columns + c);
    //                _indices.push_back((r + 1) * columns + c);
    //            }
    //            _indices.push_back((r + 1) * columns + (columns - 1));
    //        }
    
    
    //        for (int y = 0; y <= rows; y++){
    //            for (int x = 0; x <= columns; x++) {
    //                _vertices.push_back(V3t{x / (F1t)columns - 0.5f, y / (F1t)rows - 0.5f, 0});
    //                _texCoords.push_back(V3t{x / (F1t)columns, y / (F1t)rows, 0});
    //                _colors.push_back(V4t{x / (F1t)columns, y / (F1t)rows, 1.0, 1});
    //                _normals.push_back(V3t{ 1.0f, 0.0f, 0.0f});
    //            }
    //        }
    //
    //
    //        for (int y = 0; y < rows; y++) {
    //            for (int x = 0; x < columns; x++) {
    //
    //                int base = y * columns + x;
    //                int next = base + columns;
    //
    //                _indices.push_back(base);
    //                _indices.push_back(next);
    //                _indices.push_back(next+1);
    //
    //
    //                _indices.push_back(next+1);
    //                _indices.push_back(base+1);
    //                _indices.push_back(base);
    //            }
    //        }
    //
    //     printf("indices \n");
    //
    //    for (int i = 0; i <_indices.size(); i+=3) {
    //        printf("%u %u %u \n", _indices[i],_indices[i+1],_indices[i+2]);
    //    }
    
    //      printf("indices \n");
    //
    //    printVector(_indices);
    
    
    interlaceAndBuffer(0);
    
}

NKVertexBuffer::NKVertexBuffer(NKPrimitive primitive, GLenum bufferMode){
    
    printf("float size: %lu \n", sizeof(F1t));
    
    if (primitive == NKPrimitiveRect) {
        _vertices =  {
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {0.5f, 0.5f, 0.0f}
        };
        
        printf("init V3 stack \n");
        //_vertices.print();
        
        _normals = {
            { 1.0f, 0.0f, 0.0f},
            { 1.0f, 0.0f, 0.0f},
            { 1.0f, 0.0f, 0.0f},
            { 1.0f, 0.0f, 0.0f}
        };
        
        _texCoords = {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
        };
        
    }
    
    else if (primitive == NKPrimitiveAxes) {
        _vertices = {
            {-1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},
            { 0.0f, 0.0f, 1.0f}
        };
        
        _colors = {
            {.5f, 0.0f, 0.0f, 1.0f},
            {1.0f, 0.5f, 0.5f, 1.0f},
            {0.0f, .5f, 0.0f, 1.0f},
            {0.5f, 1.0f, 0.5f, 1.0f},
            {0.0f, 0.0f, .5f, 1.0f},
            {0.5f, 0.5f, 1.0f, 1.0f}
        };
    }
    
    else if (primitive == NKPrimitive8VCube) {
        // Vertex Data
        
        _vertices =  {
            {-0.5f, -.5f, .5f},
            {.5f, -.5f, .5f},
            {.5f, .5f, .5f},
            {-.5f, .5f, .5f},
            {-.5f, -.5f, -.5f},
            {.5f, -.5f, -.5f},
            {.5f, .5f, -.5f},
            {-.5f, .5f, -.5f}
        };
        
        //_vertices = vertices;
        
        // Normal Data for the Cube Verticies
        _normals = {
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f}
        };
        
        _texCoords = {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f}
        };
        
        // Color Data for the Cube Verticies
        _colors =  {
            NKColor("cyan").data,
            NKColor("cyan").data,
            NKColor("blue").data,
            NKColor("blue").data,
            NKColor("green").data,
            NKColor("green").data,
            NKColor("red").data,
            NKColor("red").data
        };
        
        // Element Indicies for the Cube
        _indices = {
            0, 1, 2, 2, 3, 0,
            3, 2, 6, 6, 7, 3,
            7, 6, 5, 5, 4, 7,
            4, 0, 3, 3, 7, 4,
            0, 1, 5, 5, 4, 0,
            1, 5, 6, 6, 2, 1
        };
        
        indexBuffer = NKIndexBuffer(_indices);
        
    }
    
    else if (primitive == NKPrimitiveBoundingBox) {
        // Vertex Data
        
        _vertices =  {
            {-0.5f, -.5f, .5f},
            {.5f, -.5f, .5f},
            {.5f, .5f, .5f},
            {-.5f, .5f, .5f},
            {-.5f, -.5f, -.5f},
            {.5f, -.5f, -.5f},
            {.5f, .5f, -.5f},
            {-.5f, .5f, -.5f}
        };
        
        //_vertices = vertices;
        
        // Normal Data for the Cube Verticies
        _normals = {
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f}
        };
        
        _texCoords = {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f}
        };
        
        // Color Data for the Cube Verticies
        _colors =  {
            NKColor("cyan").data,
            NKColor("cyan").data,
            NKColor("blue").data,
            NKColor("blue").data,
            NKColor("green").data,
            NKColor("green").data,
            NKColor("red").data,
            NKColor("red").data
        };
        
        // Element Indicies for the Cube
        _indices = {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7
        };
        
        indexBuffer = NKIndexBuffer(_indices);
        
    }
    
    else if (primitive == NKPrimitiveCube) {
        /// normals, tedcoords
        
        //    else if (primitive == NKPrimitiveCube) {
        
        //    }
        
        _vertices = {
            {1.f, -1.f, -1.f},
            {1.f, 1.f, -1.f},
            {1.f, -1.f, 1.f},
            {1.f, -1.f, 1.f},
            {1.f, 1.f, -1.f},
            {1.f, 1.f, 1.f},
            {1.f, 1.f, -1.f},
            {-1.f, 1.f, -1.f},
            {1.f, 1.f, 1.f},
            {1.f, 1.f, 1.f},
            {-1.f, 1.f, -1.f},
            {-1.f, 1.f, 1.f},
            {-1.f, 1.f, -1.f},
            {-1.f, -1.f, -1.f},
            {-1.f, 1.f, 1.f},
            {-1.f, 1.f, 1.f},
            {-1.f, -1.f, -1.f},
            {-1.f, -1.f, 1.f},
            {-1.f, -1.f, -1.f},
            {1.f, -1.f, -1.f},
            {-1.f, -1.f, 1.f},
            {-1.f, -1.f, 1.f},
            {1.f, -1.f, -1.f},
            {1.f, -1.f, 1.f},
            {1.f, 1.f, 1.f},
            {-1.f, 1.f, 1.f},
            {1.f, -1.f, 1.f},
            {1.f, -1.f, 1.f},
            {-1.f, 1.f, 1.f},
            {-1.f, -1.f, 1.f},
            {1.f, -1.f, -1.f},
            {-1.f, -1.f, -1.f},
            {1.f, 1.f, -1.f},
            {1.f, 1.f, -1.f},
            {-1.f, -1.f, -1.f},
            {-1.f, 1.f, -1.f}
        };
        
        _normals = {
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f}
        };
        
        _texCoords = {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}
        };
    }
    
    interlaceAndBuffer(bufferMode);
    
}

NKVertexBuffer::NKVertexBuffer(GLint stacks, GLint slices, GLfloat squash){
    
    unsigned int colorIncrement = 0;
    unsigned int blue = 1.;
    unsigned int red = 1.;
    unsigned int green = 1.;
    
    colorIncrement = 1./stacks;
    
    float m_Scale = 1.;
    
    int numElements = (slices*2+2) * (stacks);
    
    // Vertices
    V3t *vertices = (V3t*)calloc(numElements,sizeof(V3t));
    F1t *vPtr = (F1t*)&vertices[0];
    
    // Normal pointers for lighting
    V3t *vertexNormals = (V3t*)calloc(numElements,sizeof(V3t));
    F1t *nPtr = (F1t*)&vertexNormals[0];
    
    V3t *textureCoords = (V3t*)calloc(numElements,sizeof(V3t));
    F1t *tPtr = (F1t*)&textureCoords[0];
    
    // Color
    C4t *vertexColors = (C4t*)calloc(numElements, sizeof(C4t));
    F1t *cPtr = (F1t*)&vertexColors[0];
    
    unsigned int phiIdx, thetaIdx;
    
    for(phiIdx = 0; phiIdx < stacks; phiIdx++)
    {
        //starts at -pi/2 goes to pi/2
        //the first circle
        float phi0 = M_PI * ((float)(phiIdx+0) * (1.0/(float)(stacks)) - 0.5);
        //second one
        float phi1 = M_PI * ((float)(phiIdx+1) * (1.0/(float)(stacks)) - 0.5);
        float cosPhi0 = cos(phi0);
        float sinPhi0 = sin(phi0);
        float cosPhi1 = cos(phi1);
        float sinPhi1 = sin(phi1);
        
        float cosTheta, sinTheta;
        //longitude
        for(thetaIdx = 0; thetaIdx < slices; thetaIdx++)
        {
            float theta = -2.0*M_PI * ((float)thetaIdx) * (1.0/(float)(slices - 1));
            cosTheta = cos(theta);
            sinTheta = sin(theta);
            
            //We're generating a vertical pair of points, such
            //as the first point of stack 0 and the first point of stack 1
            //above it. this is how triangle_strips work,
            //taking a set of 4 vertices and essentially drawing two triangles
            //at a time. The first is v0-v1-v2 and the next is v2-v1-v3, and so on.
            
            //get x-y-x of the first vertex of stack
            vPtr[0] = m_Scale*cosPhi0 * cosTheta;
            vPtr[1] = m_Scale*sinPhi0 * squash;
            vPtr[2] = m_Scale*(cosPhi0 * sinTheta);
            //the same but for the vertex immediately above the previous one.
            vPtr[3] = m_Scale*cosPhi1 * cosTheta;
            vPtr[4] = m_Scale*sinPhi1 * squash;
            vPtr[5] = m_Scale*(cosPhi1 * sinTheta);
            
            nPtr[0] = cosPhi0 * cosTheta;
            nPtr[1] = sinPhi0;
            nPtr[2] = cosPhi0 * sinTheta;
            nPtr[3] = cosPhi1 * cosTheta;
            nPtr[4] = sinPhi1;
            nPtr[5] = cosPhi1 * sinTheta;
            
            if(tPtr!=nullptr){
                GLfloat texX = (float)thetaIdx * (1.0f/(float)(slices-1));
                tPtr[0] = texX;
                tPtr[1] = (float)(phiIdx + 0) * (1.0f/(float)(stacks));
                
                tPtr[2] = 0;
                
                tPtr[3] = texX;
                tPtr[4] = (float)(phiIdx + 1) * (1.0f/(float)(stacks));
                // 5
                tPtr[5] = 0;
            }
            
            cPtr[0] = red;
            cPtr[1] = green;
            cPtr[2] = blue;
            cPtr[4] = red;
            cPtr[5] = green;
            cPtr[6] = blue;
            cPtr[3] = cPtr[7] = 1.;
            
            cPtr += 2*4;
            vPtr += 2*3;
            nPtr += 2*3;
            
            if(tPtr != nullptr) tPtr += 2*3;
        }
        
        //Degenerate triangle to connect stacks and maintain winding order
        
        vPtr[0] = vPtr[3] = vPtr[-3];
        vPtr[1] = vPtr[4] = vPtr[-2];
        vPtr[2] = vPtr[5] = vPtr[-1];
        
        nPtr[0] = nPtr[3] = nPtr[-3];
        nPtr[1] = nPtr[4] = nPtr[-2];
        nPtr[2] = nPtr[5] = nPtr[-1];
        
        tPtr[0] = tPtr[3] = tPtr[-3];
        tPtr[1] = tPtr[4] = tPtr[-2];
        tPtr[2] = tPtr[5] = tPtr[-1];
        
    }
    
    std::copy(vertices, vertices+numElements, std::back_inserter(_vertices));
    std::copy(vertexNormals, vertexNormals+numElements, std::back_inserter(_normals));
    std::copy(textureCoords, textureCoords+numElements, std::back_inserter(_texCoords));
    std::copy(vertexColors, vertexColors+numElements, std::back_inserter(_colors));
    
    interlaceAndBuffer();
    
}

NKVertexBuffer::NKVertexBuffer(F1t width, F1t height, F1t depth, int resX, int resY, int resZ ) {
    // mesh only available as triangles //
    
    resX = resX + 1;
    resY = resY + 1;
    resZ = resZ + 1;
    
    if( resX < 2 ) resX = 0;
    if( resY < 2 ) resY = 0;
    if( resZ < 2 ) resZ = 0;
    
    // halves //
    float halfW = width * .5f;
    float halfH = height * .5f;
    float halfD = depth * .5f;
    
    V3t vert;
    V3t texcoord;
    V3t normal;
    U1t vertOffset = 0;
    
    // TRIANGLES //
    
    // Front Face //
    normal = {0,0,1};
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            vert.x = texcoord.x * width - halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = halfD;
            
            _vertices.push_back(vert);
            _texCoords.push_back(texcoord);
            _normals.push_back(normal);
            _colors.push_back({texcoord.x, texcoord.y, 1, 1});
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            _indices.push_back((y)*resX + x + vertOffset);
            _indices.push_back((y)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x + vertOffset);
            
            // second triangle //
            _indices.push_back((y)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = (U1t)_vertices.size();
    
    // Right Side Face //
    normal = {1, 0, 0};
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resZ; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            //vert.x = texcoord.x * width - halfW;
            vert.x = halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = texcoord.x * -depth + halfD;
            
            _vertices.push_back(vert);
            _texCoords.push_back(texcoord);
            _normals.push_back(normal);
            _colors.push_back({texcoord.x, texcoord.y, 1, 1});
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resZ-1; x++) {
            // first triangle //
            _indices.push_back((y)*resZ + x + vertOffset);
            _indices.push_back((y)*resZ + x+1 + vertOffset);
            _indices.push_back((y+1)*resZ + x + vertOffset);
            
            // second triangle //
            _indices.push_back((y)*resZ + x+1 + vertOffset);
            _indices.push_back((y+1)*resZ + x+1 + vertOffset);
            _indices.push_back((y+1)*resZ + x + vertOffset);
        }
    }
    
    vertOffset = (U1t)_vertices.size();
    
    // Left Side Face //
    normal = {-1, 0, 0};
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resZ; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            //vert.x = texcoord.x * width - halfW;
            vert.x = -halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = texcoord.x * depth - halfD;
            
            _vertices.push_back(vert);
            _texCoords.push_back(texcoord);
            _normals.push_back(normal);
            _colors.push_back({texcoord.x, texcoord.y, 1, 1});
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resZ-1; x++) {
            // first triangle //
            _indices.push_back((y)*resZ + x + vertOffset);
            _indices.push_back((y)*resZ + x+1 + vertOffset);
            _indices.push_back((y+1)*resZ + x + vertOffset);
            
            // second triangle //
            _indices.push_back((y)*resZ + x+1 + vertOffset);
            _indices.push_back((y+1)*resZ + x+1 + vertOffset);
            _indices.push_back((y+1)*resZ + x + vertOffset);
        }
    }
    
    vertOffset = (U1t)_vertices.size();
    
    
    // Back Face //
    normal = {0, 0, -1};
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            vert.x = texcoord.x * -width + halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = -halfD;
            
            _vertices.push_back(vert);
            _texCoords.push_back(texcoord);
            _normals.push_back(normal);
            _colors.push_back({texcoord.x, texcoord.y, 1, 1});
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            _indices.push_back((y)*resX + x + vertOffset);
            _indices.push_back((y)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x + vertOffset);
            
            // second triangle //
            _indices.push_back((y)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = (U1t)_vertices.size();
    
    // Top Face //
    normal = {0, -1, 0};
    // add the vertexes //
    for(int iy = 0; iy < resZ; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resZ-1.f));
            
            vert.x = texcoord.x * width - halfW;
            //vert.y = texcoord.y * height - halfH;
            vert.y = -halfH;
            vert.z = texcoord.y * depth - halfD;
            
            _vertices.push_back(vert);
            _texCoords.push_back(texcoord);
            _normals.push_back(normal);
            _colors.push_back({texcoord.x, texcoord.y, 1, 1});
        }
    }
    
    for(int y = 0; y < resZ-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            _indices.push_back((y)*resX + x + vertOffset);
            _indices.push_back((y)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x + vertOffset);
            
            // second triangle //
            _indices.push_back((y)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = (U1t)_vertices.size();
    
    // Bottom Face //
    normal = {0, 1, 0};
    // add the vertexes //
    for(int iy = 0; iy < resZ; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resZ-1.f));
            
            vert.x = texcoord.x * width - halfW;
            //vert.y = texcoord.y * height - halfH;
            vert.y = halfH;
            vert.z = texcoord.y * -depth + halfD;
            
            _vertices.push_back(vert);
            _texCoords.push_back(texcoord);
            _normals.push_back(normal);
            _colors.push_back({texcoord.x, texcoord.y, 1, 1});
        }
    }
    
    for(int y = 0; y < resZ-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            _indices.push_back((y)*resX + x + vertOffset);
            _indices.push_back((y)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x + vertOffset);
            
            // second triangle //
            _indices.push_back((y)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x+1 + vertOffset);
            _indices.push_back((y+1)*resX + x + vertOffset);
        }
    }
}

//shared_ptr<NKVertexBuffer> NKVertexBuffer::sphereWithStacks(GLint stacks, GLint slices, GLfloat squash)
//{
//
//    if (NKVertexBuffer::vboPrimitiveCache[NKPrimitiveSphere]) return vboPrimitiveCache[NKPrimitiveSphere];
//
//    unsigned int colorIncrement = 0;
//    unsigned int blue = 1.;
//    unsigned int red = 1.;
//    unsigned int green = 1.;
//
//    colorIncrement = 1./stacks;
//
//    float m_Scale = 1.;
//
//    int numElements = (slices*2+2) * (stacks);
//
//    // Vertices
//    V3t *vertices = (V3t*)calloc(numElements,sizeof(V3t));
//    F1t *vPtr = (F1t*)&vertices[0];
//
//    // Normal pointers for lighting
//    V3t *vertexNormals = (V3t*)calloc(numElements,sizeof(V3t));
//    F1t *nPtr = (F1t*)&vertexNormals[0];
//
//    V2t *textureCoords = (V2t*)calloc(numElements,sizeof(V2t));
//    F1t *tPtr = (F1t*)&textureCoords[0];
//
//    // Color
//    C4t *vertexColors = (C4t*)calloc(numElements, sizeof(C4t));
//    F1t *cPtr = (F1t*)&vertexColors[0];
//
//    unsigned int phiIdx, thetaIdx;
//
//    for(phiIdx = 0; phiIdx < stacks; phiIdx++)
//    {
//        //starts at -pi/2 goes to pi/2
//        //the first circle
//        float phi0 = M_PI * ((float)(phiIdx+0) * (1.0/(float)(stacks)) - 0.5);
//        //second one
//        float phi1 = M_PI * ((float)(phiIdx+1) * (1.0/(float)(stacks)) - 0.5);
//        float cosPhi0 = cos(phi0);
//        float sinPhi0 = sin(phi0);
//        float cosPhi1 = cos(phi1);
//        float sinPhi1 = sin(phi1);
//
//        float cosTheta, sinTheta;
//        //longitude
//        for(thetaIdx = 0; thetaIdx < slices; thetaIdx++)
//        {
//            float theta = -2.0*M_PI * ((float)thetaIdx) * (1.0/(float)(slices - 1));
//            cosTheta = cos(theta);
//            sinTheta = sin(theta);
//
//            //We're generating a vertical pair of points, such
//            //as the first point of stack 0 and the first point of stack 1
//            //above it. this is how triangle_strips work,
//            //taking a set of 4 vertices and essentially drawing two triangles
//            //at a time. The first is v0-v1-v2 and the next is v2-v1-v3, and so on.
//
//            //get x-y-x of the first vertex of stack
//            vPtr[0] = m_Scale*cosPhi0 * cosTheta;
//            vPtr[1] = m_Scale*sinPhi0 * squash;
//            vPtr[2] = m_Scale*(cosPhi0 * sinTheta);
//            //the same but for the vertex immediately above the previous one.
//            vPtr[3] = m_Scale*cosPhi1 * cosTheta;
//            vPtr[4] = m_Scale*sinPhi1 * squash;
//            vPtr[5] = m_Scale*(cosPhi1 * sinTheta);
//
//            nPtr[0] = cosPhi0 * cosTheta;
//            nPtr[1] = sinPhi0;
//            nPtr[2] = cosPhi0 * sinTheta;
//            nPtr[3] = cosPhi1 * cosTheta;
//            nPtr[4] = sinPhi1;
//            nPtr[5] = cosPhi1 * sinTheta;
//
//            if(tPtr!=nullptr){
//                GLfloat texX = (float)thetaIdx * (1.0f/(float)(slices-1));
//                tPtr[0] = texX;
//                tPtr[1] = (float)(phiIdx + 0) * (1.0f/(float)(stacks));
//                tPtr[2] = texX;
//                tPtr[3] = (float)(phiIdx + 1) * (1.0f/(float)(stacks));
//            }
//
//            cPtr[0] = red;
//            cPtr[1] = green;
//            cPtr[2] = blue;
//            cPtr[4] = red;
//            cPtr[5] = green;
//            cPtr[6] = blue;
//            cPtr[3] = cPtr[7] = 1.;
//
//            cPtr += 2*4;
//            vPtr += 2*3;
//            nPtr += 2*3;
//
//            if(tPtr != nullptr) tPtr += 2*2;
//        }
//
//        //Degenerate triangle to connect stacks and maintain winding order
//
//        vPtr[0] = vPtr[3] = vPtr[-3];
//        vPtr[1] = vPtr[4] = vPtr[-2];
//        vPtr[2] = vPtr[5] = vPtr[-1];
//
//        nPtr[0] = nPtr[3] = nPtr[-3];
//        nPtr[1] = nPtr[4] = nPtr[-2];
//        nPtr[2] = nPtr[5] = nPtr[-1];
//
//        if(tPtr != nullptr){
//            tPtr[0] = tPtr[2] = tPtr[-2];
//            tPtr[1] = tPtr[3] = tPtr[-1];
//        }
//
//    }
//
//    NKPrimitiveArray *elements = (NKPrimitiveArray*)calloc(numElements, sizeof(NKPrimitiveArray));
//
//    for (int i = 0; i < numElements; i++) {
//        memcpy(&elements[i].vertex, &vertices[i], sizeof(V3t));
//        memcpy(&elements[i].normal, &vertexNormals[i], sizeof(V3t));
//        memcpy(&elements[i].texCoord, &textureCoords[i], sizeof(V2t));
//        memcpy(&elements[i].color, &vertexColors[i], sizeof(C4t));
//    }
//
//    free(vertices);
//    free(vertexNormals);
//    free(textureCoords);
//    free(vertexColors);
//
//    nkLog("V2t: %lu V3t: %lu V4t: %lu ", sizeof(V2t), sizeof(V3t), sizeof(V4t));
//    nkLog("NKPrimitiveArraySize: %lu", sizeof(NKPrimitiveArray));
//
//    auto buf = make_shared<NKVertexBuffer>(sizeof(NKPrimitiveArray)*numElements, numElements, elements, NKVertexBuffer::primitiveSetupBlock());
//
//    //buf.drawMode = GL_LINES;
//
//    free(elements);
//
//    vboPrimitiveCache[NKPrimitiveSphere] = buf;
//
//    return buf;
//
//}
//
//+(instancetype)lodSphere:(int)levels {
//
//    int detail = 3;
//
//    int totalCount = 0;
//    int currentOffset = 0;
//    int numLevels = levels;
//
//    int *offsets = malloc(sizeof(int) * levels);
//    int *sizes = malloc(sizeof(int) * levels);
//
//    for (int i = 0; i < numLevels; i++) {
//
//        int stacks = (numLevels - i) * detail;
//        int slices = (numLevels - i) * detail + 2;
//
//        int numElements = (slices*2+2) * (stacks);
//
//        offsets[i] = totalCount;
//        sizes[i] = numElements;
//
//        totalCount += numElements;
//
//    }
//
//    float squash = 1.;
//
//   // NSLog(@"init vert %d total elements", totalCount);
//
//    NKPrimitiveArray *elements = (NKPrimitiveArray*)calloc(totalCount, sizeof(NKPrimitiveArray));
//
//    for (int i = 0; i < numLevels; i++) {
//
//        int stacks = (numLevels - i) * detail;
//        int slices = (numLevels - i) * detail + 2;
//        int numElements = (slices*2+2) * (stacks);
//
//        unsigned int colorIncrement = 0;
//        unsigned int blue = 1.;
//        unsigned int red = 1.;
//        unsigned int green = 1.;
//
//        colorIncrement = 1./stacks;
//
//        float m_Scale = 1.;
//
//        // Vertices
//        V3t *vertices = (V3t*)calloc(numElements,sizeof(V3t));
//        F1t *vPtr = (F1t*)&vertices[0];
//
//        // Normal pointers for lighting
//        V3t *vertexNormals = (V3t*)calloc(numElements,sizeof(V3t));
//        F1t *nPtr = (F1t*)&vertexNormals[0];
//
//        V2t *textureCoords = (V2t*)calloc(numElements,sizeof(V2t));
//        F1t *tPtr = (F1t*)&textureCoords[0];
//
//        // Color
//        C4t *vertexColors = (C4t*)calloc(numElements, sizeof(C4t));
//        F1t *cPtr = (F1t*)&vertexColors[0];
//
//        unsigned int phiIdx, thetaIdx;
//
//        for(phiIdx = 0; phiIdx < stacks; phiIdx++)
//        {
//            //starts at -pi/2 goes to pi/2
//            //the first circle
//            float phi0 = M_PI * ((float)(phiIdx+0) * (1.0/(float)(stacks)) - 0.5);
//            //second one
//            float phi1 = M_PI * ((float)(phiIdx+1) * (1.0/(float)(stacks)) - 0.5);
//            float cosPhi0 = cos(phi0);
//            float sinPhi0 = sin(phi0);
//            float cosPhi1 = cos(phi1);
//            float sinPhi1 = sin(phi1);
//
//            float cosTheta, sinTheta;
//            //longitude
//            for(thetaIdx = 0; thetaIdx < slices; thetaIdx++)
//            {
//                float theta = -2.0*M_PI * ((float)thetaIdx) * (1.0/(float)(slices - 1));
//                cosTheta = cos(theta);
//                sinTheta = sin(theta);
//
//                //We're generating a vertical pair of points, such
//                //as the first point of stack 0 and the first point of stack 1
//                //above it. this is how triangle_strips work,
//                //taking a set of 4 vertices and essentially drawing two triangles
//                //at a time. The first is v0-v1-v2 and the next is v2-v1-v3, and so on.
//
//                //get x-y-x of the first vertex of stack
//                vPtr[0] = m_Scale*cosPhi0 * cosTheta;
//                vPtr[1] = m_Scale*sinPhi0 * squash;
//                vPtr[2] = m_Scale*(cosPhi0 * sinTheta);
//                //the same but for the vertex immediately above the previous one.
//                vPtr[3] = m_Scale*cosPhi1 * cosTheta;
//                vPtr[4] = m_Scale*sinPhi1 * squash;
//                vPtr[5] = m_Scale*(cosPhi1 * sinTheta);
//
//                nPtr[0] = cosPhi0 * cosTheta;
//                nPtr[1] = sinPhi0;
//                nPtr[2] = cosPhi0 * sinTheta;
//                nPtr[3] = cosPhi1 * cosTheta;
//                nPtr[4] = sinPhi1;
//                nPtr[5] = cosPhi1 * sinTheta;
//
//                if(tPtr!=nullptr){
//                    GLfloat texX = (float)thetaIdx * (1.0f/(float)(slices-1));
//                    tPtr[0] = texX;
//                    tPtr[1] = (float)(phiIdx + 0) * (1.0f/(float)(stacks));
//                    tPtr[2] = texX;
//                    tPtr[3] = (float)(phiIdx + 1) * (1.0f/(float)(stacks));
//                }
//
//                cPtr[0] = red;
//                cPtr[1] = green;
//                cPtr[2] = blue;
//                cPtr[3] = cPtr[7] = 1.;
//                cPtr[4] = red;
//                cPtr[5] = green;
//                cPtr[6] = blue;
//
//
//                cPtr += 2*4;
//                vPtr += 2*3;
//                nPtr += 2*3;
//
//                if(tPtr != nullptr) tPtr += 2*2;
//            }
//            //        blue+=colorIncrement;
//            //        red-=colorIncrement;
//
//            //Degenerate triangle to connect stacks and maintain winding order
//
//            vPtr[0] = vPtr[3] = vPtr[-3];
//            vPtr[1] = vPtr[4] = vPtr[-2];
//            vPtr[2] = vPtr[5] = vPtr[-1];
//
//            nPtr[0] = nPtr[3] = nPtr[-3];
//            nPtr[1] = nPtr[4] = nPtr[-2];
//            nPtr[2] = nPtr[5] = nPtr[-1];
//
//            if(tPtr != nullptr){
//                tPtr[0] = tPtr[2] = tPtr[-2];
//                tPtr[1] = tPtr[3] = tPtr[-1];
//            }
//
//        }
//
//       // NSLog(@"copy %d elements", numElements);
//
//        for (int i = currentOffset; i < currentOffset + numElements; i++) {
//            memcpy(&elements[i].vertex, &vertices[(i-currentOffset)], sizeof(V3t));
//            memcpy(&elements[i].normal, &vertexNormals[(i-currentOffset)], sizeof(V3t));
//            memcpy(&elements[i].texCoord, &textureCoords[(i-currentOffset)], sizeof(V2t));
//            memcpy(&elements[i].color, &vertexColors[(i-currentOffset)], sizeof(C4t));
//        }
//
//        free(vertices);
//        free(vertexNormals);
//        free(textureCoords);
//        free(vertexColors);
//
//        // NSLog(@"add to NKPrimitiveArraySize: %lu", numElements * sizeof(NKPrimitiveArray));
//
//        currentOffset += numElements;
//    }
//
//    //NSLog(@"V2t: %lu V3t: %lu V4t: %lu ", sizeof(V2t), sizeof(V3t), sizeof(V4t));
//
//
//    NKVertexBuffer *buf = [[NKVertexBuffer alloc] initWithSize:sizeof(NKPrimitiveArray)*totalCount numberOfElements:numLevels data:elements setup:[NKVertexBuffer primitiveSetupBlock]];
//
//    buf.elementOffset = offsets;
//    buf.elementSize = sizes;
//
//
//    free(elements);
//
//    return buf;
//
//}
//
//+(instancetype)pointSprite {
//    GLfloat point[3] = {0,0,0};
//    NKVertexBuffer *buf = [[NKVertexBuffer alloc] initWithSize:sizeof(point) numberOfElements:1 data:point setup:newGeometrySetupBlock{
//        glEnableVertexAttribArray(NKS_V4_POSITION);
//        glVertexAttribPointer(NKS_V4_POSITION, 3, GL_FLOAT, GL_FALSE,
//                              24, BUFFER_OFFSET(0));
//    }];
//
//    return buf;
//}
//
//
//+(instancetype)cubeWithWidthSections:(int)resX height:(int)resY depth:(int) resZ {
//
//    int numElements = (resX * 2) * (resY* 2) * (resZ * 2) + 3;
//
//    V3t *vertices = (V3t*)calloc(numElements,sizeof(V3t));
//    F1t *vPtr = (F1t*)vertices;
//
//    V3t *normals = (V3t*)calloc(numElements,sizeof(V3t));
//    F1t *nPtr = (F1t*)normals;
//
//    V2t *texcoords = (V2t*)calloc(numElements,sizeof(V2t));
//
//    C4t *colors = (C4t*)calloc(numElements, sizeof(C4t));
//
//    V3t vert;
//    V2t texcoord;
//    V3t normal;
//    C4t color;
//    int vertOffset = 0;
//
//    int elementCounter = 0;
//
//    // TRIANGLES //
//    float rx = resX;
//    float ry = resY;
//    float rz = resZ;
//
//    // Front Face //
//    normal = V3Make(0, 0, 1);
//    // add the vertexes //
//    for(int iy = 0; iy < resY; iy++) {
//        //for (int s = 0; s < 2; s++) {
//        for(int ix = 0; ix < resX*2; ix++) {
//
//            switch (iy % 2) {
//                case 0:
//                    vert.x = (ix / 2) / (resX - 1) * 2 - 1.;
//                    vert.y = (iy / ry + (ix % 2) / ry) * 2 - 1.;
//                    vert.z = 1 * 2 - 1.;
//                    break;
//
//                case 1:
//                    vert.x = (((resX-1) - ix) / 2) / (resX - 1) * 2 - 1.;
//                    vert.y = (iy / ry + (ix % 2) / ry) * 2 - 1.;
//                    vert.z = 1 * 2 - 1;
//                    break;
//
//                default:
//                    break;
//            }
//
//            texcoord.x = vert.x;
//            texcoord.y = vert.y;
//
//            *(vertices + elementCounter) = vert;
//            *(texcoords + elementCounter) = texcoord;
//            *(normals + elementCounter) = normal;
//
//            vPtr+=3;
//            nPtr+=3;
//            elementCounter++;
//        }
//
//        //Degenerate triangle to connect stacks and maintain winding order
//
//        vPtr[0] = vPtr[-1];
//        vPtr[1] = vPtr[-1];
//        vPtr[2] = vPtr[-1];
//
//        nPtr[0] = nPtr[-1];
//        nPtr[1] = nPtr[-1];
//        nPtr[2] = nPtr[-1];
//
//        vPtr+=3;
//        nPtr+=3;
//
//        elementCounter++;
//    }
//
//    numElements = elementCounter;
//
//    NSLog(@"cube with %d vertices", numElements);
//    NKPrimitiveArray *elements = (NKPrimitiveArray*)calloc(numElements, sizeof(NKPrimitiveArray));
//
//    for (int i = 0; i < numElements; i++) {
//        memcpy(&elements[i].vertex, &vertices[i], sizeof(V3t));
//        memcpy(&elements[i].normal, &normals[i], sizeof(V3t));
//        memcpy(&elements[i].texCoord, &texcoords[i], sizeof(V2t));
//        memcpy(&elements[i].color, &colors[i], sizeof(C4t));
//    }
//
//    free(vertices);
//    free(normals);
//    free(texcoords);
//    free(colors);
//
////    NSLog(@"V2t: %lu V3t: %lu V4t: %lu ", sizeof(V2t), sizeof(V3t), sizeof(V4t));
////    NSLog(@"NKPrimitiveArraySize: %lu", sizeof(NKPrimitiveArray));
//
//    NKVertexBuffer *buf = [[NKVertexBuffer alloc] initWithSize:sizeof(NKPrimitiveArray)*numElements numberOfElements:numElements data:elements setup:[NKVertexBuffer primitiveSetupBlock]];
//
//    //buf.drawMode = GL_LINES;
//
//    free(elements);
//
//    return buf;
//
//}
//

//+(instancetype)segmentedBezier:(NKVector3Stack*)bezier {
//
//    NKVertexBuffer *buf = [[NKVertexBuffer alloc]init];
//
//    buf.lineWidth = 0.1;
//
//    buf.updateBlock = ^(NKVertexBuffer* buffer){
//        NSLog(@"calculate points");
//        [buffer updateBufferWithBezier:bezier lineWidths:buffer.lineWidths];
//    };
//
//    [buf update];
//
//    return buf;
//}



//-(instancetype)initWithVertexData:(const GLvoid *)data numberOfElements:(GLuint)numElements ofSize:(GLsizeiptr)size {
//
//    return [self initWithSize:size numberOfElements:numElements data:data setup:newGeometrySetupBlock{
//        glEnableVertexAttribArray(NKS_V4_POSITION);
//        glVertexAttribPointer(NKS_V4_POSITION, 3, GL_FLOAT, GL_FALSE,
//                              sizeof(V3t), BUFFER_OFFSET(0));
//    }];
//
//}

#pragma mark - GEOMETRY getters

V6t NKVertexBuffer::boundingSizeForVertexSet(vector<NKVertexBuffer*> set) {
    
    float minx = 1000000., maxx = -1000000.;
    float miny = 1000000., maxy = -1000000.;
    float minz = 1000000., maxz = -1000000.;
    
    for (auto& buf : set) {
        
        const V3t* vertices = buf->vertices();
        
        for (int i = 0; i < buf->_vertices.size(); i++){ // FIND LARGEST VALUE
            
            if (vertices[i].x < minx)
                minx = vertices[i].x;
            if (vertices[i].x > maxx)
                maxx = vertices[i].x;
            
            if (vertices[i].y < miny)
                miny = vertices[i].y;
            if (vertices[i].y > maxy)
                maxy = vertices[i].y;
            
            if (vertices[i].z < minz)
                minz = vertices[i].z;
            if (vertices[i].z > maxz)
                maxz = vertices[i].z;
            
        };
    }
    
    V6t box;
    
    box.x = V2Make(minx, maxx);
    box.y = V2Make(miny, maxy);
    box.z = V2Make(minz, maxz);
    
    return box;
    
}

V3t NKVertexBuffer::centerOfBoundingSize(V6t box) {
    return V3Make((box.x.min+box.x.max)*.5, (box.y.min+box.y.max)*.5, (box.z.min+box.z.max)*.5);
}

V3t NKVertexBuffer::sizeOfBoundingSize(V6t box) {
    float width = fabsf(box.x.max - box.x.min);
    float height = fabsf(box.y.max - box.y.min);
    float depth = fabsf(box.z.max - box.z.min);
    
    return V3Make(width, height, depth);
}

void NKVertexBuffer::normalizeAndCenter() {
    V6t mySize = boundingSizeForVertexSet(vector<NKVertexBuffer*> {this});
    
    V3t center = centerOfBoundingSize(mySize);
    
    V3t modelSize = sizeOfBoundingSize(mySize);
    
    V3t modelInverse = V3Divide(V3MakeF(2.), modelSize);
    
    V3t offsetNormalized = V3Divide(V3MultiplyScalar(center, 2.), modelSize);
    
    //V3t center = V3Divide(V3MultiplyScalar(center), modelSize);
    _center = center;
    
    NKLogV3("my center :", _center);
    
    for (int p = 0; p < _vertices.size(); p++){
        _vertices[p] = V3Subtract(V3Multiply(_vertices[p], modelInverse),offsetNormalized);
        //vertices[p] = V3Multiply(vertices[p], modelInverse);
        //NKLogV3(@"normalized vertex", vertices[p]);
    }
    
}

V3t NKVertexBuffer::normalizeForGroupWithSize(F1t unitSize, V6t groupBoundingBox, bool center) {
    
    //NKLogV3(@"group offset Normalized", offsetNormalized);
    // store for later ratio
    V6t myBoundingSize = boundingSizeForVertexSet(vector<NKVertexBuffer*> {this});
    
    V3t mySize = sizeOfBoundingSize(myBoundingSize);
    
    normalizeAndCenter();
    
    V3t groupSize = sizeOfBoundingSize(groupBoundingBox);
    
    V3t groupCenter = centerOfBoundingSize(groupBoundingBox);
    
    NKLogV3("groupCenter: ", groupCenter);
    
    V3t groupAspect = V3UnitRetainAspect(groupSize);
    
    V3t aspectExpand = V3MultiplyScalar(V3Multiply(groupAspect, V3Divide(mySize, groupSize)), unitSize);
    
    
    V3t myRelative;
    NKLogV3("my relative center", myRelative = V3Subtract(_center, groupCenter ));
    
    _center = V3MultiplyScalar(V3DivideScalar(myRelative, V3Largest(groupSize)), unitSize*2.);
    
    NKLogV3("my center normalized:", _center);
    
    //    _center = V3MakeF(0);
    //
    //    _center = V3Subtract(V3Divide(V3MultiplyScalar(_center, unitSize), groupSize), groupCenterNormalized);
    //
    //    _center = V3Divide(_center, aspectExpand);
    
    //return V3Make(2., 2., 2.);
    NKLogV3("my final size:", aspectExpand);
    return aspectExpand;
}

#pragma mark - BUFFERING / BINDING

void NKVertexBuffer::interlaceAndBuffer(GLenum bufferModeOrNull) {
    
    
    ULt size = _vertices.size();
    
    assert(size != 0);
    
    _stride = 0;
    
    
    if (_vertices.size()){
        _verticesOffset = _stride;
        _stride+=3;
    }
    if (_normals.size() == size){
        _normalsOffset = _stride;
        _stride+=3;
    }
    if (_texCoords.size()){
        _texCoordsOffset = _stride;
        _stride+=3;
    }
    if (_colors.size() == size){
        _colorsOffset = _stride;
        _stride+=4;
    }
    if (_tangents.size() == size){
        _tangentsOffset = _stride;
        _stride+=3;
    }
    if (_biNormals.size() == size){
        _biNormalsOffset = _stride;
        _stride+=3;
    }
    if (_boneWeights.size() == size){
        _boneWeightsOffset = _stride;
        _stride+=1;
    }
    
    if (_indices.size()) {
        indexBuffer = NKIndexBuffer(_indices);
    }
    
    
    F1t interlaceData [_stride*_vertices.size()];
    
    
    for (int i = 0; i < _vertices.size(); i++){
        memcpy(interlaceData+(i*_stride),&_vertices[i], sizeof(V3t));
        if (_normals.size() == size)
            memcpy(interlaceData+(i*_stride)+_normalsOffset, &_normals[i], sizeof(V3t));
        if (_texCoords.size() == size)
            memcpy(interlaceData+(i*_stride)+_texCoordsOffset, &_texCoords[i], sizeof(V3t));
        if (_colors.size() == size)
            memcpy(interlaceData+(i*_stride)+_colorsOffset, &_colors[i], sizeof(C4t));
        if (_tangents.size() == size)
            memcpy(interlaceData+(i*_stride)+_tangentsOffset, &_tangents[i], sizeof(V3t));
        if (_biNormals.size() == size)
            memcpy(interlaceData+(i*_stride)+_biNormalsOffset, &_biNormals[i], sizeof(V3t));
        if (_boneWeights.size() == size)
            memcpy(interlaceData+(i*_stride)+_boneWeightsOffset, &_boneWeights[i], sizeof(V3t));
    }
    
    
    
    printf("<INTERLACED> \n");
    for (int i = 0; i < _vertices.size(); i++){
        for (int e = 0; e < _stride; e++){
            printf("%1.2f ", interlaceData[i*_stride + e]);
        }
        printf("\n");
    }
    printf("\n </INTERLACED> \n");
    
    _geometrySetupBlock = [this, size](){
        nkLog("** buffer VBO *( %d )*", _vertexArray);
        nkLog("%d elements, %lu bytes, %lu totalBytes", _stride, _stride*sizeof(F1t), _vertices.size()*sizeof(F1t)*_stride);
        
        
        //        printf("<DATA> \n");
        
        printf("has vertices");
        glEnableVertexAttribArray(NKS_V4_POSITION);
        glVertexAttribPointer(NKS_V4_POSITION, 3, GL_FLOAT, GL_FALSE,
                              sizeof(F1t)*_stride, BUFFER_OFFSET(0));
        //_vertices.print();
        
        if (_normals.size() == size) {
            printf(", normals");
            //_normals.print();
            glEnableVertexAttribArray(NKS_V3_NORMAL);
            glVertexAttribPointer(NKS_V3_NORMAL, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(F1t)*_stride, BUFFER_OFFSET(sizeof(F1t)*_normalsOffset));
        }
        
        if (_texCoords.size() == size){
            printf(", texcoords");
            glEnableVertexAttribArray(NKS_V2_TEXCOORD);
            glVertexAttribPointer(NKS_V2_TEXCOORD, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(F1t)*_stride, BUFFER_OFFSET(sizeof(F1t)*_texCoordsOffset));
        }
        
        if (_colors.size() == size) {
            printf(", colors");
            //           _colors.print();
            glEnableVertexAttribArray(NKS_V4_COLOR);
            glVertexAttribPointer(NKS_V4_COLOR, 4, GL_FLOAT, GL_FALSE,
                                  sizeof(F1t)*_stride, BUFFER_OFFSET(sizeof(F1t)*_colorsOffset));
        }
        
        if (_tangents.size() == size) {
            printf(", tangents");
            //            _tangents.print();
            glEnableVertexAttribArray(NKS_V3_TANGENT);
            glVertexAttribPointer(NKS_V3_TANGENT, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(F1t)*_stride, BUFFER_OFFSET(sizeof(F1t)*_tangentsOffset));
        }
        
        if (_biNormals.size() == size) {
            printf(", bi normals");
            //            _biNormals.print();
            glEnableVertexAttribArray(NKS_V3_BINORMAL);
            glVertexAttribPointer(NKS_V3_BINORMAL, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(F1t)*_stride, BUFFER_OFFSET(sizeof(F1t)*_biNormalsOffset));
        }
        
        if (_boneWeights.size() == size) {
            //            _boneWeights.print();
            printf(", bone weights");
            glEnableVertexAttribArray(NKS_F1_BONE_WEIGHT);
            glVertexAttribPointer(NKS_F1_BONE_WEIGHT, 1, GL_FLOAT, GL_FALSE,
                                  sizeof(F1t)*_stride, BUFFER_OFFSET(sizeof(F1t)*_boneWeightsOffset));
        }
        
        //        printf("\n");
        //
        //        printf("\n </DATA> \n");
        
    };
    
    bufferData(_stride*_vertices.size()*sizeof(F1t), interlaceData, bufferModeOrNull);
    
}

void NKVertexBuffer::bufferData(GLsizeiptr size,
                                const GLvoid * data,
                                GLenum mode){
    
    if (mode == 0) {
        mode = GL_STATIC_DRAW;
    }
    
    nkLog("load vertex buffer with: %lu vertices, %d bytes", _vertices.size(), size);
    
    //glEnable(GL_DEPTH_TEST);
#if (TARGET_IOS || TARGET_RASPBERRY_PI)
    glGenVertexArraysOES(1, &_vertexArray);
    glBindVertexArrayOES(_vertexArray);
#elif NK_USE_ARB_EXT
    glGenVertexArraysAPPLE(1, &_vertexArray);
    glBindVertexArrayAPPLE(_vertexArray);
#else
    glGenVertexArrays(1, &_vertexArray);
    glBindVertexArray(_vertexArray);
#endif
    
    nkGetGLError();
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    
    nkGetGLError();
    
    glBufferData(GL_ARRAY_BUFFER,
                 size,
                 data,
                 mode);
    
    nkGetGLError();
    
    _geometrySetupBlock();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    nkGetGLError();
    
#if (NK_USE_GLES && !TARGET_EMSCRIPTEN)
    glBindVertexArrayOES(0);
#else
#ifdef NK_USE_ARB_EXT
    glBindVertexArrayAPPLE(0);
#else
    glBindVertexArray(0);
#endif
#endif
    
    //#warning put this back at some point
    //   glDeleteBuffers(GL_ARRAY_BUFFER, &_vertexBuffer);
    
    nkGetGLError();
    
}

void NKVertexBuffer::updateBuffer() {
    
    ULt size = _stride * _vertices.size();
    
    if (size) {
        
        F1t interlacedData[size];
        
        for (int i = 0; i < _vertices.size(); i++){
            if (_vertices.size())
                memcpy(interlacedData+(i*_stride),_vertices.data()+i, sizeof(V3t));
            if (_normals.size())
                memcpy(interlacedData+(i*_stride)+_normalsOffset, _vertices.data()+i, sizeof(V3t));
            if (_texCoords.size())
                memcpy(interlacedData+(i*_stride)+_texCoordsOffset, _texCoords.data()+i, sizeof(V3t));
            if (_colors.size())
                memcpy(interlacedData+(i*_stride)+_colorsOffset, _colors.data()+i, sizeof(C4t));
            if (_tangents.size())
                memcpy(interlacedData+(i*_stride)+_tangentsOffset, _tangents.data()+i, sizeof(V3t));
            if (_biNormals.size())
                memcpy(interlacedData+(i*_stride)+_biNormalsOffset, _biNormals.data()+i, sizeof(V3t));
            if (_boneWeights.size())
                memcpy(interlacedData+(i*_stride)+_boneWeightsOffset, _boneWeights.data()+i, sizeof(V3t));
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        
        glBufferSubData(GL_ARRAY_BUFFER,0,
                        size*sizeof(F1t),
                        interlacedData);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    }
    else {
        printf("attempting to buffer nullptr vertex data \n");
    }
    
}


static const U1t VerticesInSegment = 2;

void NKVertexBuffer::update() {
    if (_updateBlock) {
        _updateBlock(this);
    }
}

U1t NKVertexBuffer::numberOfSegmentsForBezier(B12t* bezier)
{
    F1t segments = B12Length(*bezier);
    return (U1t)ceilf(sqrt(segments * segments * 0.6 + 225.0));
}

SegmentSubdivider NKVertexBuffer::subdividerForBezier(B12t* bez)
{
    // a p1 p2 b
    V3t a = bez->v[0];
    V3t p1 = bez->v[1];
    V3t p2 = bez->v[2];
    V3t b = bez->v[3];
    
    return [=](float t){
        float nt = 1.0f - t;
        
        SegmentSubdivision subdivision;
        subdivision.v1 = V3Make(a.x * nt * nt * nt  +  3.0 * p1.x * nt * nt * t  +  3.0 * p2.x * nt * t * t  +  b.x * t * t * t,
                                a.y * nt * nt * nt  +  3.0 * p1.y * nt * nt * t  +  3.0 * p2.y * nt * t * t  +  b.y * t * t * t,
                                0);
        
        V3t tangent = V3Make(-3.0 * a.x * nt * nt  +  3.0 * p1.x * (1.0 - 4.0 * t + 3.0 * t * t)  +  3.0 * p2.x * (2.0 * t - 3.0 * t * t)  +  3.0 * b.x * t * t,
                             -3.0 * a.y * nt * nt  +  3.0 * p1.y * (1.0 - 4.0 * t + 3.0 * t * t)  +  3.0 * p2.y * (2.0 * t - 3.0 * t * t)  +  3.0 * b.y * t * t,
                             0);
        
        subdivision.v2 = V3Normalize(V3Make(-tangent.y, tangent.x, 0));
        
        return subdivision;
    };
}



void NKVertexBuffer::updateBufferWithBezier(V3Stack& bezier , F1Stack& lineWidths){
    
    bool isNew = true;
    
    if (_vertices.size()) {
        _vertices.clear();
        _indices.clear();
        isNew = false;
    }
    
    U1t startVertex = 0;
    
    for (int i = 0; i < bezier.size(); i+=4) {
        
        B12t *bez = (B12t *)(&bezier.data()[i]);
        
        U1t segmentsCount = numberOfSegmentsForBezier(bez);
        SegmentSubdivider subdivider = subdividerForBezier(bez);
        
        int vertsInSeg = 0;
        int indInSeg = 0;
        
        for (int seg = 0; seg <= segmentsCount; seg++) {
            SegmentSubdivision subdivision = subdivider((double)seg/(double)segmentsCount);
            
            _vertices.push_back(V3Add(subdivision.v1, V3MultiplyScalar(subdivision.v2, + lineWidths.lastObject())));
            //_vertices.append(V3Add(subdivision.v1, V3MultiplyScalar(subdivision.v2, + lineWidths.lastObject())));
            
            //NKLogV3(@"start", newVertices.last);
            
            _vertices.push_back(V3Add(subdivision.v1, V3MultiplyScalar(subdivision.v2, - lineWidths.lastObject())));
            //_vertices.append(V3Add(subdivision.v1, V3MultiplyScalar(subdivision.v2, - lineWidths.lastObject())));
            
            //NKLogV3(@"end", newVertices.last);
            
            vertsInSeg++;
            vertsInSeg++;
        }
        
        for (int seg = 0; seg < segmentsCount; seg++) {
            
            
            _indices.push_back(startVertex + 0);
            _indices.push_back(startVertex + 2);
            _indices.push_back(startVertex + 1);
            
            _indices.push_back(startVertex + 1);
            _indices.push_back(startVertex + 2);
            _indices.push_back(startVertex + 3);
            
            startVertex += VerticesInSegment;
            
            indInSeg += 6;
        }
        
        startVertex += VerticesInSegment;
        
        // NSLog(@"segments is bez: %lu vertices %d indices %d", (unsigned long)segmentsCount, vertsInSeg, indInSeg);
        
        // [newIndexData appendU1:startVertex];
        // [newIndexData appendU1:startVertex];
        
    }
    
    if (isNew) {
        interlaceAndBuffer(GL_DYNAMIC_DRAW);
    }
    else {
        indexBuffer.update();
        updateBuffer();
    }
    
    
}

void NKVertexBuffer::bind()
{
#if (TARGET_IOS || TARGET_RASPBERRY_PI)
    glBindVertexArrayOES(_vertexArray);
#elif NK_USE_ARB_EXT
    glBindVertexArrayAPPLE(_vertexArray);
#else
    glBindVertexArray(_vertexArray);
#endif
    nkGetGLError();
}

void emulateVAO(){
#ifdef TARGET_OPENGLES
	glEnableVertexAttribArray(ofShader::POSITION_ATTRIBUTE);
	glVertexAttribPointer(ofShader::POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), vertexData.getVerticesPointer());

	useNormals &= (vertexData.getNumNormals()>0);
	if(useNormals){
		glEnableVertexAttribArray(ofShader::NORMAL_ATTRIBUTE);
		glVertexAttribPointer(ofShader::NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, sizeof(ofVec3f), vertexData.getNormalsPointer());
	}else{
		glDisableVertexAttribArray(ofShader::NORMAL_ATTRIBUTE);
	}

	useColors &= (vertexData.getNumColors()>0);
	if(useColors){
		glEnableVertexAttribArray(ofShader::COLOR_ATTRIBUTE);
		glVertexAttribPointer(ofShader::COLOR_ATTRIBUTE, 4,GL_FLOAT, GL_FALSE, sizeof(ofFloatColor), vertexData.getColorsPointer());
	}else{
		glDisableVertexAttribArray(ofShader::COLOR_ATTRIBUTE);
	}

	useTextures &= (vertexData.getNumTexCoords()>0);
	if(useTextures){
		glEnableVertexAttribArray(ofShader::TEXCOORD_ATTRIBUTE);
		glVertexAttribPointer(ofShader::TEXCOORD_ATTRIBUTE,2, GL_FLOAT, GL_FALSE, sizeof(ofVec2f), vertexData.getTexCoordsPointer());
	}else{
		glDisableVertexAttribArray(ofShader::TEXCOORD_ATTRIBUTE);
	}


	setAttributes(true,useColors,useTextures,useNormals);

	GLenum drawMode;
	switch(renderType){
	case OF_MESH_POINTS:
		drawMode = GL_POINTS;
		break;
	case OF_MESH_WIREFRAME:
		drawMode = GL_LINES;
		break;
	case OF_MESH_FILL:
		drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
		break;
	default:
		drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
		break;
	}

	if(vertexData.getNumIndices()){
		glDrawElements(drawMode, vertexData.getNumIndices(),GL_UNSIGNED_SHORT,vertexData.getIndexPointer());
	}else{
		glDrawArrays(drawMode, 0, vertexData.getNumVertices());
	}
#endif
}
void NKVertexBuffer::unbind()
{
#if (TARGET_IOS || TARGET_RASPBERRY_PI)
    glBindVertexArrayOES(0);
#elif NK_USE_ARB_EXT
    glBindVertexArrayAPPLE(0);
#else
    glBindVertexArray(0);
#endif
}

NKVertexBuffer::~NKVertexBuffer()
{

    glDeleteBuffers(1, &_vertexBuffer);
#if (TARGET_IOS || TARGET_RASPBERRY_PI)
    glDeleteVertexArraysOES(1, &_vertexArray);
#elif NK_USE_ARB_EXT
    glDeleteVertexArraysAPPLE(1, &_vertexArray);
#else
    glDeleteVertexArrays(1, &_vertexArray);
#endif

}


