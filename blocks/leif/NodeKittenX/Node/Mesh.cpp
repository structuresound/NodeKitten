//
//  Mesh.m
//  nike3dField
//
//  Created by Chroma Developer on 3/31/14.
//
//
#include "Mesh.h"

#include "ShaderProgram.h"
#include "Scene.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"

using namespace std;
using namespace Shader;

Mesh* Mesh::_fboSurface {nullptr};

void Mesh::glInit() {
  _fboSurface = new Mesh(NKPrimitiveRect, Texture::blankTexture(), Color(1), V3(1));
  _fboSurface->shader = Program::textureFill;
  
  _fboSurface->setForceOrthographic(true);
  _fboSurface->setCullFace(GLCullFaceFront);
  _fboSurface->setBlendMode(GLBlendModeFBO);
  _fboSurface->setIgnoresLights(true);
  _fboSurface->setUsesDepth(false);

}

Mesh& Mesh::fboSurface()
{
  return *_fboSurface;
};

void Mesh::fillRect(Scene* context, V4 rect, Color color, shared_ptr<Texture> texture){
  _fboSurface->_scene = context;
  _fboSurface->setColor(color);
  _fboSurface->position.set(rect.rectMidpoint());
  _fboSurface->size.set(V3(rect.size));
  if (texture) {
    _fboSurface->setTexture(texture);
    Program::textureFill->use();
  }
  else {
    Program::fill->use();
  }
  _fboSurface->customDraw();
}

shared_ptr<Mesh> Mesh::nodeWithPrimitive(NKPrimitive primitive, shared_ptr<Texture> texture, Color color, V3t size){
  shared_ptr<Mesh> ret (new Mesh(primitive, texture, color, size));
  return ret;
}

shared_ptr<Mesh> Mesh::nodeWithObj(string objName, V3t size, bool normalize, bool anchor){
  shared_ptr<Mesh> ret (new Mesh(objName, size, normalize, anchor));
  return ret;
}

shared_ptr<Mesh> Mesh::nodeWithVbo(shared_ptr<VertexBuffer> buffer, GLenum drawMode, shared_ptr<Texture> texture, Color color, V3t size) {
  shared_ptr<Mesh> ret (new Mesh(buffer, drawMode, texture, color, size));
  return ret;
}

string Mesh::stringForPrimitive(NKPrimitive primitive) {
  
  switch (primitive) {
    case NKPrimitiveRect:
      return "NKPrimitiveRect";
      break;
      
    case NKPrimitivePlane:
      return "NKPrimitivePlane";
      break;
      
    case NKPrimitiveSphere:
      return "NKPrimitiveSphere";
      break;
      
    case NKPrimitiveLODSphere:
      return "NKPrimitiveLODSphere";
      break;
      
    case NKPrimitiveCube:
      return "NKPrimitiveCube";
      break;
      
    case NKPrimitive8VCube:
      return "NKPrimitive8VCube";
      break;
      
    case NKPrimitiveBoundingBox:
      return "NKPrimitiveBoundingBox";
      break;
      
    case NKPrimitiveAxes:
      return "NKPrimitiveAxes";
      break;
      
    default:
      return "NKPrimitiveNone";
      break;
  }
  
}

Mesh::Mesh(shared_ptr<Texture> texture, Color color, V3t size_)
{
  size.set(size_);
  setAlpha(1.0);
  setColor(color);
  setTexture(texture);
}

Mesh::Mesh(NKPrimitive primitive, shared_ptr<Texture> texture, Color color, V3t size_):
Mesh(texture, color, size_)
{
  
  _primitiveType = primitive;
  
  
  switch (primitive) {
      
    case NKPrimitiveSphere: case NKPrimitiveLODSphere:
      _vertexBuffer = VertexBuffer::sphereWithStacks(16, 16, 1.0);
      _drawMode = GL_TRIANGLE_STRIP;
      setCullFace(GLCullFaceFront);
      break;
      
    case NKPrimitiveCube:
      _vertexBuffer = VertexBuffer::defaultCube();
      _drawMode = GL_TRIANGLES;
      setCullFace(GLCullFaceBack);
      break;
      
    case NKPrimitive8VCube:
      _vertexBuffer = VertexBuffer::cube8v();
      _drawMode = GL_TRIANGLES;
      setCullFace(GLCullFaceBack);
      break;
      
    case NKPrimitiveRect:
      _vertexBuffer = VertexBuffer::defaultRect();
      _drawMode = GL_TRIANGLE_STRIP;
      setCullFace(GLCullFaceFront);
      break;
      
    case NKPrimitivePlane:
      _vertexBuffer = VertexBuffer::planeWithSubdivisions((int)(size_.x)/4, (int)(size_.x)/4);
      _drawMode = GL_TRIANGLE_STRIP;
      setCullFace(GLCullFaceFront);
      break;
      
    case NKPrimitiveAxes:
      _vertexBuffer = VertexBuffer::axes();
      _drawMode = GL_LINES;
      break;
      
    default:
      break;
      
  }
  
  //nkLog("add primitive: %s to mesh cache with %lu vertices", Mesh::stringForPrimitive(primitive).c_str(), (unsigned long)_vertexBuffer->numVertices());
  
}

Mesh::Mesh(shared_ptr<VertexBuffer> buffer, GLenum drawMode, shared_ptr<Texture> texture, Color color, V3t size) :
Mesh(texture, color, size)
{
  setCullFace(GLCullFaceNone);
  _vertexBuffer = buffer;
  _drawMode = drawMode;
}

#warning DO OBJ

Mesh::Mesh(string objName, V3t size, bool normalize, bool anchor){
  //
  //    VertexBuffer *buf;
  //
  //    if ([[NKStaticDraw meshesCache]objectForKey:name]) {
  //        buf = [[NKStaticDraw meshesCache]objectForKey:name];
  //        NSString *textureName;
  //
  //        if (!textureName) {
  //            self = [self initWithVertexBuffer:buf drawMode:GL_TRIANGLES texture:nil color:NKWHITE size:buf.boundingBoxSize];
  //        }
  //        else{
  //            self = [self initWithVertexBuffer:buf drawMode:GL_TRIANGLES texture:[Texture textureWithImageNamed:textureName] color:NKWHITE size:buf.boundingBoxSize];
  //        }
  //        self.cullFace = GLCullFaceBack;
  //        return self;
  //    }
  //
  //    else {
  //
  //        NSString *textureName;
  //        NSUInteger numElements;
  //
  //        //NSString *filePath = [[NSBundle mainBundle] pathForResource:@"space_frigate_6" ofType:@"obj"];
  //        NSString *filePath = [[NSBundle mainBundle] pathForResource:name ofType:@"obj"];
  //        int p = 0;
  //        if (filePath) {
  //            NSError *error;
  //            NSString *myText = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:&error];
  //            if (myText) {
  //
  //                NSMutableArray *vertices = [[NSMutableArray alloc] init];
  //                NSMutableArray *expandedVertices = [[NSMutableArray alloc] init];
  //                NSMutableArray *texCoords = [[NSMutableArray alloc] init];
  //                NSMutableArray *expandedTexCoords = [[NSMutableArray alloc] init];
  //                NSMutableArray *normals = [[NSMutableArray alloc] init];
  //                NSMutableArray *expandedNormals = [[NSMutableArray alloc] init];
  //
  //                NSArray *lines = [myText componentsSeparatedByString:@"\n"];
  //                //NSLog(@"lines has %d lines", [lines count]);
  //                for (NSString *astring in lines){
  //                    if ([astring hasPrefix:@"v "]){
  //                        [vertices addObject:astring];
  //                    }
  //                    else if ([astring hasPrefix:@"vt "]){ // TEXCOORDS
  //                        [texCoords addObject:astring];
  //                    }
  //                    else if ([astring hasPrefix:@"vn "]){ // NORMAL
  //                        [normals addObject:astring];
  //                    }
  //
  //                    else if ([astring hasPrefix:@"f "]){
  //                        NSArray *iArray = [astring arrayWithNoSpaces];
  //                        int i;
  //                        //NSLog(@"xxx");
  //                        for (i = 1; i < [iArray count]; i++) {
  //                            NSArray *bArray = [[iArray objectAtIndex:i] componentsSeparatedByString:@"/"];
  //
  //                            [expandedVertices addObject:[vertices objectAtIndex:[[bArray objectAtIndex:0] intValue] - 1]];
  //                            [expandedTexCoords addObject:[texCoords objectAtIndex:[[bArray objectAtIndex:1] intValue] - 1]];
  //                            [expandedNormals addObject:[normals objectAtIndex:[[bArray objectAtIndex:2] intValue] - 1]];
  //                            //extract coords for tangent
  //                        }
  //                        //NSLog(@"end xxx");
  //                    }
  //                    //                else if ([astring hasPrefix:@"t "]){ // TEXTURE NAME
  //                    //                    NSArray *iArray = [astring arrayWithNoSpaces];
  //                    //                    textureName = [iArray objectAtIndex:1];
  //                    //                    NSLog(@"texture name is %@", textureName);
  //                    //
  //                    //                }
  //                    //                else if ([astring hasPrefix:@"ex "]){
  //                    //                    NSArray *iArray = [astring arrayWithNoSpaces];
  //                    //                    textureName = [iArray objectAtIndex:1];
  //                    //                    NSLog(@"texture name is %@", textureName);
  //                    //
  //                    //                }
  //
  //                    //}
  //
  //                }
  //
  //                numElements = [expandedVertices count];
  //
  //                NKPrimitiveArray *buffer = malloc(sizeof(NKPrimitiveArray)*numElements);
  //
  //                float minx = 1000000., maxx = -1000000.;
  //                float miny = 1000000., maxy = -1000000.;
  //                float minz = 1000000., maxz = -1000000.;
  //
  //                V3t finalModelSize;
  //
  //                if (normalize) {
  //
  //                    for (p = 0; p < [expandedVertices count]; p++){ // FIND LARGEST VALUE
  //
  //                        NSArray *c = [[expandedVertices objectAtIndex:p] arrayWithNoSpaces];
  //                        NSUInteger cq = [c count];
  //
  //                        //NSLog(@"v %@ %@ %@",[c objectAtIndex:cq - 3], [c objectAtIndex:cq -2],[c objectAtIndex:cq -1]);
  //
  //                        float t = [[c objectAtIndex:cq - 3] floatValue];
  //                        if (t < minx)
  //                            minx = t;
  //                        if (t > maxx)
  //                            maxx = t;
  //
  //
  //                        t = [[c objectAtIndex:cq -2] floatValue];
  //                        if (t < miny)
  //                            miny = t;
  //                        if (t > maxy)
  //                            maxy = t;
  //
  //
  //                        t = [[c objectAtIndex:cq -1] floatValue];
  //                        if (t < minz)
  //                            minz = t;
  //                        if (t > maxz)
  //                            maxz = t;
  //
  //                    };
  //
  //                    float width = fabsf(maxx - minx);
  //                    float height = fabsf(maxy - miny);
  //                    float depth = fabsf(maxz - minz);
  //
  //                    NKLogV3(@"min", V3(minx, miny, minz));
  //                    NKLogV3(@"max", V3(maxx, maxy, maxz));
  //                    NKLogV3(@"center", V3((minx+maxx), (miny+maxy), (minz+maxz)));
  //
  //                    V3t modelSize = V3(width, height, depth);
  //
  //                    V3t modelInverse = V3Divide(V3(2.), modelSize);
  //
  //                    V3t modelNormalized = V3UnitRetainAspect(modelSize);
  //
  //                    finalModelSize = V3Multiply(size, modelNormalized);
  //
  //                    //                NKLogV3(@"obj size:", modelSize);
  //                    //                NKLogV3(@"obj divisor:", modelInverse);
  //                    //                NKLogV3(@"normalized size", modelNormalized);
  //                    //                NKLogV3(@"normalized center", offsetNormalized);
  //
  //                    for (p = 0; p < [expandedVertices count]; p++){
  //                        // VERTICES
  //                        NSArray *c = [[expandedVertices objectAtIndex:p] arrayWithNoSpaces];
  //                        NSUInteger cq = [c count];
  //
  //                        V3t vertex = V3([[c objectAtIndex:cq - 3] floatValue], [[c objectAtIndex:cq - 2] floatValue], [[c objectAtIndex:cq - 1] floatValue]);
  //
  //                        if (anchor) {
  //                            V3t offset = V3((minx+maxx), (miny+maxy), (minz+maxz));
  //                            V3t offsetNormalized = V3Divide(offset, modelSize);
  //                            buffer[p].vertex = V3Subtract(V3Multiply(vertex, modelInverse),offsetNormalized);
  //                        }
  //                        else {
  //                            buffer[p].vertex = V3Multiply(vertex, modelInverse);
  //                        }
  //                    }
  //
  //                }
  //                else {
  //                    finalModelSize = size;
  //
  //                    for (p = 0; p < [expandedVertices count]; p++){
  //                        // VERTICES
  //                        NSArray *c = [[expandedVertices objectAtIndex:p] arrayWithNoSpaces];
  //                        NSUInteger cq = [c count];
  //                        V3t vertex = V3([[c objectAtIndex:cq - 3] floatValue], [[c objectAtIndex:cq - 2] floatValue], [[c objectAtIndex:cq - 1] floatValue]);
  //                        if (anchor) {
  //                            V3t offset = V3((minx+maxx), (miny+maxy), (minz+maxz));
  //                            buffer[p].vertex = V3Subtract(vertex,offset);
  //                        }
  //                        else {
  //                            buffer[p].vertex = vertex;
  //                        }
  //                    }
  //                }
  //                for (p = 0; p < [expandedVertices count]; p++){
  //                    // TEXCOORDS
  //                    NSArray *c =  [[expandedTexCoords objectAtIndex:p] componentsSeparatedByString:@" "];
  //                    NSUInteger cq = [c count];
  //                    buffer[p].texCoord.x = [[c objectAtIndex:cq-2] floatValue];
  //                    buffer[p].texCoord.y = [[c objectAtIndex:cq-1] floatValue];
  //                    // NORMALS
  //                    c =  [[expandedNormals objectAtIndex:p] componentsSeparatedByString:@" "];
  //                    cq = [c count];
  //                    buffer[p].normal.x = [[c objectAtIndex:cq -3] floatValue];
  //                    buffer[p].normal.y = [[c objectAtIndex:cq -2] floatValue];
  //                    buffer[p].normal.z = [[c objectAtIndex:cq - 1] floatValue];
  //                    // COLORS
  //                    buffer[p].color.r = 1.0;
  //                    buffer[p].color.g = 1.0;
  //                    buffer[p].color.b = 1.0;
  //                    buffer[p].color.a = 1.0;
  //                }
  //
  //                buf = [[VertexBuffer alloc] initWithSize:sizeof(NKPrimitiveArray)*numElements numberOfElements:numElements data:buffer setup:[VertexBuffer primitiveSetupBlock]];
  //
  //                buf.boundingBoxSize = finalModelSize;
  //
  //                [[NKStaticDraw meshesCache] setObject:buf forKey:name];
  //
  //                NSLog(@"cache obj named: %@", name);
  //
  //                free(buffer);
  //
  //                NKLogV3(@"object bounding box size", finalModelSize);
  //
  //            }
  //
  //            if (!textureName) {
  //                self = [self initWithVertexBuffer:buf drawMode:GL_TRIANGLES texture:nil color:NKWHITE size:buf.boundingBoxSize];
  //            }
  //            else{
  //                self = [self initWithVertexBuffer:buf drawMode:GL_TRIANGLES texture:[Texture textureWithImageNamed:textureName] color:NKWHITE size:buf.boundingBoxSize];
  //            }
  //
  //            self.cullFace = GLCullFaceBack;
  //
  //            return self;
  //
  //        }
  //
  //    }
  //
  //    return nil;
  //
}

void Mesh::chooseShader() {
  if (_numTextures) {
    if (_numTextures) {
      shared_ptr<Texture> t = _textures[0];
      
#warning do video
      //            if ([t isKindOfClass:[NKVideoTexture class]]) {
      //                    self.shader = [Shader newShaderNamed:@"videoTextureShader" colorMode:NKS_COLOR_MODE_UNIFORM numTextures:-1 numLights:1 withBatchSize:0];
      //            }
      //            else {
      shader = Program::shaderNamed("uColorTextureLightShader",NKS_COLOR_MODE_UNIFORM,_numTextures,1);
      //            }
    }
  }
  else {
    shader = Program::shaderNamed("uColorLightShader",NKS_COLOR_MODE_UNIFORM,0, 1 );
  }
}

void Mesh::setTexture(shared_ptr<Texture> texture){
  
  if (texture) {
    _textures = {texture};
    if (!_numTextures && shader){
      chooseShader();
      nkLog("set to %lu textures, choode sh \n", _textures.size());
    }
    _numTextures = 1;
    _textures = {texture};
  }
  else {
    _numTextures = 0;
    if (_textures.size() && shader) {
      chooseShader();
      _textures.clear();
      nkLog("set to %lu textures, choode sh \n", _textures.size());
    }
  }
  
}

int Mesh::lodForDistance() const {
  
  float distance = V3Distance(scene()->camera->base->globalPosition(), globalPosition()) * .125;
  
  float width = size.get().width;
  
  int lod = 0;
  
  if (width < distance) {
    
    float diff = (distance - width) / distance;
    
    lod = diff * ((float)_vertexBuffer->size());
  }
  
  return lod;
}

void Mesh::bindTextures() {
  
  if (_numTextures) {
    if (scene()->boundTexture != _textures[0].get()) {
      _textures[0]->bind();
#warning DO Video
      //            if ([_textures[0] isKindOfClass:[NKVideoTexture class]]) {
      //                //NSLog(@"binding tex scale: %f %f", [(NKVideoTexture*)_textures[0] size].x, [(NKVideoTexture*)_textures[0] size].y);
      //                NKVideoTexture* t = _textures[0];
      //
      //                [[self.scene.activeShader uniformNamed:NKS_TEXTURE_RECT_SCALE] bindV2:V2Make(t.width, t.height)];
      //            }
      //            else {
      if (GLState::activeShader()->hasUniformNamed(NKS_TEXTURE_RECT_SCALE)) {
        GLState::activeShader()->uniformNamed(NKS_TEXTURE_RECT_SCALE).bindV2(V2MakeF(1.0));
      }
      
      nkGetGLError();
      
      //            }
      scene()->boundTexture = _textures[0].get();
    }
  }
  
  nkGetGLError();
}

V6t Mesh::getAabb() const {
  return _vertexBuffer->boundingBox();
}

void Mesh::customDraw(){
  
  if (color().alpha || _numTextures) {
    
    pushStyle();
    
    if (GLState::activeShader()->hasUniformNamed(NKS_V4_COLOR)){
      if (color().alpha == 0) {
        GLState::activeShader()->uniformNamed(NKS_V4_COLOR).bindV4(CLEAR);
      }else {
        GLState::activeShader()->uniformNamed(NKS_V4_COLOR).bindV4(glColor());
      }
    }
    
    nkGetGLError();
    
    bindTextures();
    
    setupViewMatrix();
    
    if (_vertexBuffer->size() > 0) {
      
      _vertexBuffer->bind();
      
      nkGetGLError();
      
      if (_vertexBuffer->_indices.size() > 0){
        //nkLog("%p drawElements: mode %d, indices %d, vertices %d", (void*)&_vertexBuffer, _drawMode, _vertexBuffer->_indices.size(), _vertexBuffer->size());
        glDrawElements(_drawMode, (U1t)_vertexBuffer->_indices.size(), GL_UNSIGNED_INT, 0);
        nkGetGLError();
      }
      else {
        if (usesLOD) {
          int lod = lodForDistance();
          glDrawArrays(_drawMode, _vertexBuffer->elementOffset[lod], _vertexBuffer->elementSize[lod]);
        }
        else {
          glDrawArrays(_drawMode, 0, (int)_vertexBuffer->size());
        }
        nkGetGLError();
      }
      if (shouldDrawBoundingBox) {
        drawBoundingBox();
      }
      
#if NK_LOG_METRICS
      Scene::draws++;
#endif
    }
  }
}

void Mesh::setupViewMatrix() const {
  if (!GLState::activeShader()) return;
  
  if (forceOrthographic()) {
    GLState::activeShader()->uniformNamed(NKS_M16_MVP).bindM16(M16Multiply(scene()->orthographicMatrix(),
                                                                        M16Multiply(M16MakeLookAt(V3(0, 0, 1), V3(0), V3(0, 1, 0)),
                                                                                    M16ScaleWithV3(globalTransform.get(), size.get()))));
  }
  else {
    if(GLState::activeShader()->hasUniformNamed(NKS_M16_MV) ){
      GLState::activeShader()->uniformNamed(NKS_M16_MV).bindM16(modelViewMatrix.get());
    }
    
    if(GLState::activeShader()->hasUniformNamed(NKS_M9_NORMAL) ){
      GLState::activeShader()->uniformNamed(NKS_M9_NORMAL).bindM9(M16GetInverseNormalMatrix(modelViewMatrix.get()));
    }
    
    GLState::activeShader()->uniformNamed(NKS_M16_MVP).bindM16(modelViewProjectionMatrix.get());
  }
  
  nkGetGLError();
}

void Mesh::drawBoundingBox() {
  
  shared_ptr<VertexBuffer> v = VertexBuffer::edges8v();
  
  auto tmp = GLState::activeShader();
  
  auto uniformShader = Program::shaderNamed("boxShader",NKS_COLOR_MODE_UNIFORM,0, 0 );
  
  nkGetGLError();
  
  uniformShader->use();
  GLState::activeShader()->uniformNamed(NKS_V4_COLOR).bindV4({1.0,1.0,1.0,1.0});
  
  M16t mvp;
  
  V6t bounds = _vertexBuffer->boundingBox();
  V3t center = _vertexBuffer->centerOfBoundingSize(bounds);
  V3t boundsSize = _vertexBuffer->sizeOfBoundingSize(bounds);
  
  M16t trans = M16TranslateWithV3(globalTransform.get(), center);
  M16t modelView = M16Multiply(scene()->camera->viewMatrix.get(),M16ScaleWithV3(trans, size.get()*boundsSize));
  
  if(GLState::activeShader()->hasUniformNamed(NKS_M16_MV) ){
    GLState::activeShader()->uniformNamed(NKS_M16_MV).bindM16(modelView);
  }
  
  if(GLState::activeShader()->hasUniformNamed(NKS_M9_NORMAL) ){
    GLState::activeShader()->uniformNamed(NKS_M9_NORMAL).bindM9(M16GetInverseNormalMatrix(modelView));
  }
  
  mvp = M16Multiply(scene()->camera->projectionMatrix.get(),modelView);
  
  GLState::activeShader()->uniformNamed(NKS_M16_MVP).bindM16(mvp);
  
  nkGetGLError();
  
  v->bind();
  glDrawElements(GL_LINES, (int)v->_indices.size(), GL_UNSIGNED_INT, 0);
  
  tmp->use();
  
  nkGetGLError();
}

void Mesh::customDrawWithHitShader() const {
  GLState::activeShader()->uniformNamed(NKS_V4_COLOR).bindV4(Color{uidColor});
  
  if (scene()->boundVertexBuffer != _vertexBuffer.get()) {
    _vertexBuffer->bind();
    scene()->boundVertexBuffer = _vertexBuffer.get();
  }
  
  if (usesLOD) {
    int lod = lodForDistance();
    glDrawArrays(_drawMode, _vertexBuffer->elementOffset[lod], _vertexBuffer->elementSize[lod]);
  }
  else {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexBuffer->size());
  }
}

Mesh::~Mesh(){
#warning TEX MAN
  //    for (auto t : _textures){
  //        [TextureManager removeNode:self forTexture:t];
  //    }
}

