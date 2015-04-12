//
//  NKMeshNode.m
//  nike3dField
//
//  Created by Chroma Developer on 3/31/14.
//
//


#include "NKMeshNode.h"

#include "NKShaderProgram.h"
#include "NKSceneNode.h"
#include "NKCamera.h"
#include "NKShaderProgram.h"
#include "NKVertexBuffer.h"

NKMeshNode* NKMeshNode::_fboSurface;

NKMeshNode& NKMeshNode::fboSurface()
{
    if (!_fboSurface) {
        _fboSurface = new NKMeshNode(NKPrimitiveRect, nullptr, NKColor(1), V3MakeF(1));
        _fboSurface->setShader(NKShaderProgram::shaderNamed(nks(NKS_PASSTHROUGH_TEXTURE_SHADER),{NKShaderModule::textureModule(1)}));
        
        _fboSurface->forceOrthographic = true;
        _fboSurface->setUsesDepth(false);
        _fboSurface->_cullFace = NKCullFaceFront;
        _fboSurface->_blendMode = NKBlendModeNone;
    }
    return *_fboSurface;
};

shared_ptr<NKMeshNode> NKMeshNode::meshNodeWithPrimitive(NKPrimitive primitive, shared_ptr<NKTexture> texture, NKColor color, V3t size){
    return make_shared<NKMeshNode>(primitive, texture, color, size);
}

shared_ptr<NKMeshNode> NKMeshNode::meshNodeWithObj(string objName, V3t size, bool normalize, bool anchor){
    return make_shared<NKMeshNode>(objName, size, normalize, anchor);
}

shared_ptr<NKMeshNode> NKMeshNode::meshNodeWithVbo(shared_ptr<NKVertexBuffer> buffer, GLenum drawMode, shared_ptr<NKTexture> texture, NKColor color, V3t size) {
    return make_shared<NKMeshNode>(buffer, drawMode, texture, color, size);
}

string NKMeshNode::stringForPrimitive(NKPrimitive primitive) {
    
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

NKMeshNode::NKMeshNode(shared_ptr<NKTexture> texture, NKColor color, V3t size){
    _size = size;
    
    if (_size.z == 0) {
        setSize(V3Make(_size.x, _size.y, 1.));
    }
    
    setAlpha(1.0);
    
    if (texture) {
        _textures = {texture};
        _numTextures = 1;
#warning Do TEX Manager
        //[NKTextureManager addNode:self forTexture:texture];
    }
    
    _color = color;
}

NKMeshNode::NKMeshNode(NKPrimitive primitive, shared_ptr<NKTexture> texture, NKColor color, V3t size):
    NKMeshNode(texture, color, size)
{

        _primitiveType = primitive;
    
        
        switch (primitive) {
                
            case NKPrimitiveSphere: case NKPrimitiveLODSphere:
                _vertexBuffer = NKVertexBuffer::sphereWithStacks(16, 16, 1.0);
                _drawMode = GL_TRIANGLE_STRIP;
                setCullFace(NKCullFaceFront);
                break;
                
            case NKPrimitiveCube:
                _vertexBuffer = NKVertexBuffer::defaultCube();
                _drawMode = GL_TRIANGLES;
                setCullFace(NKCullFaceBack);
                break;
                
            case NKPrimitive8VCube:
                _vertexBuffer = NKVertexBuffer::cube8v();
                _drawMode = GL_TRIANGLES;
                setCullFace(NKCullFaceBack);
                break;
                
            case NKPrimitiveRect:
                _vertexBuffer = NKVertexBuffer::defaultRect();
                _drawMode = GL_TRIANGLE_STRIP;
                setCullFace(NKCullFaceFront);
                break;
                
            case NKPrimitivePlane:
                _vertexBuffer = NKVertexBuffer::planeWithSubdivisions((int)(size.x), (int)(size.x));
                _drawMode = GL_TRIANGLE_STRIP;
                setCullFace(NKCullFaceFront);
                break;
                
                
            case NKPrimitiveAxes:
                _vertexBuffer = NKVertexBuffer::axes();
                _drawMode = GL_LINES;
                break;
                
            default:
                break;
                
        }
    
        //nkLog("add primitive: %s to mesh cache with %lu vertices", NKMeshNode::stringForPrimitive(primitive).c_str(), (unsigned long)_vertexBuffer->numVertices());

}

NKMeshNode::NKMeshNode(shared_ptr<NKVertexBuffer> buffer, GLenum drawMode, shared_ptr<NKTexture> texture, NKColor color, V3t size) :
NKMeshNode(texture, color, size)
{
        _cullFace = NKCullFaceNone;
        _vertexBuffer = buffer;
        _drawMode = drawMode;
}

#warning DO OBJ

NKMeshNode::NKMeshNode(string objName, V3t size, bool normalize, bool anchor){
//    
//    NKVertexBuffer *buf;
//    
//    if ([[NKStaticDraw meshesCache]objectForKey:name]) {
//        buf = [[NKStaticDraw meshesCache]objectForKey:name];
//        NSString *textureName;
//        
//        if (!textureName) {
//            self = [self initWithVertexBuffer:buf drawMode:GL_TRIANGLES texture:nil color:NKWHITE size:buf.boundingBoxSize];
//        }
//        else{
//            self = [self initWithVertexBuffer:buf drawMode:GL_TRIANGLES texture:[NKTexture textureWithImageNamed:textureName] color:NKWHITE size:buf.boundingBoxSize];
//        }
//        self.cullFace = NKCullFaceBack;
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
//                    NKLogV3(@"min", V3Make(minx, miny, minz));
//                    NKLogV3(@"max", V3Make(maxx, maxy, maxz));
//                    NKLogV3(@"center", V3Make((minx+maxx), (miny+maxy), (minz+maxz)));
//                    
//                    V3t modelSize = V3Make(width, height, depth);
//                    
//                    V3t modelInverse = V3Divide(V3MakeF(2.), modelSize);
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
//                        V3t vertex = V3Make([[c objectAtIndex:cq - 3] floatValue], [[c objectAtIndex:cq - 2] floatValue], [[c objectAtIndex:cq - 1] floatValue]);
//                        
//                        if (anchor) {
//                            V3t offset = V3Make((minx+maxx), (miny+maxy), (minz+maxz));
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
//                        V3t vertex = V3Make([[c objectAtIndex:cq - 3] floatValue], [[c objectAtIndex:cq - 2] floatValue], [[c objectAtIndex:cq - 1] floatValue]);
//                        if (anchor) {
//                            V3t offset = V3Make((minx+maxx), (miny+maxy), (minz+maxz));
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
//                buf = [[NKVertexBuffer alloc] initWithSize:sizeof(NKPrimitiveArray)*numElements numberOfElements:numElements data:buffer setup:[NKVertexBuffer primitiveSetupBlock]];
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
//                self = [self initWithVertexBuffer:buf drawMode:GL_TRIANGLES texture:[NKTexture textureWithImageNamed:textureName] color:NKWHITE size:buf.boundingBoxSize];
//            }
//            
//            self.cullFace = NKCullFaceBack;
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
void NKMeshNode::setColor(NKColor color){
#warning do new shader if color changed
    NKNode::setColor(color);
}

void NKMeshNode::chooseShader() {
    if (!_shader) {
        
        if (_numTextures) {
            if (_numTextures) {
                shared_ptr<NKTexture> t = _textures[0];
                
#warning do video
                //            if ([t isKindOfClass:[NKVideoTexture class]]) {
                //                    self.shader = [NKShaderProgram newShaderNamed:@"videoTextureShader" colorMode:NKS_COLOR_MODE_UNIFORM numTextures:-1 numLights:1 withBatchSize:0];
                //            }
                //            else {
                _shader = NKShaderProgram::shaderNamed("uColorTextureLightShader",NKS_COLOR_MODE_UNIFORM,_numTextures,1);
                //            }
            }
        }
        else {
            _shader = NKShaderProgram::shaderNamed("uColorLightShader",NKS_COLOR_MODE_UNIFORM,0, 1 );
        }
        
    }
}

void NKMeshNode::setScene(NKSceneNode *scene){
    NKNode::setScene(scene);

    if (!_shader) {
        //nkLog("new mesh choose shader");
        chooseShader();
    }
    
}
void NKMeshNode::setTexture(shared_ptr<NKTexture> texture){
    
    if (texture) {

        _textures = {texture};
        
        if (!_numTextures){
            chooseShader();
             printf("set to some, num existing tex: %lu, choode sh \n", _textures.size());
        }
        
        _numTextures = 1;
        
        _textures = {texture};
       
    }
    
    else {
        _numTextures = 0;
        
        
        if (_textures.size()) {
            printf("set to nil, num existing tex: %lu, choode sh \n", _textures.size());
            chooseShader();
            _textures.clear();
        }
        
        
    }

}

int NKMeshNode::lodForDistance() {
    
    float distance = V3Distance(scene()->camera->globalPosition(), globalPosition()) * .125;
    
    float size = _size.width;
    
    int lod = 0;
    
    if (size < distance) {
        
        float diff = (distance - size) / distance;
        
        lod = diff * ((float)_vertexBuffer->size());
    }
    
    return lod;
}

void NKMeshNode::setupViewMatrix() {
    
    M16t mvp;
    
    if (forceOrthographic) {
        mvp = M16Multiply(scene()->camera->orthographicMatrix(),M16Multiply(M16MakeLookAt(V3Make(0, 0, 1), V3MakeF(0), V3Make(0, 1, 0)),M16ScaleWithV3(M16IdentityMake(), _size)));
    }
    
    else {
        M16t modelView = M16Multiply(scene()->camera->viewMatrix(),M16ScaleWithV3(globalTransform(), _size));
        
        if(scene()->activeShader()->hasUniformNamed(NKS_M16_MV) ){
            scene()->activeShader()->uniformNamed(NKS_M16_MV).bindM16(modelView);
        }
        
        if(scene()->activeShader()->hasUniformNamed(NKS_M9_NORMAL) ){
            scene()->activeShader()->uniformNamed(NKS_M9_NORMAL).bindM9(M16GetInverseNormalMatrix(modelView));
        }
        
        mvp = M16Multiply(scene()->camera->projectionMatrix(),modelView);
    }
    
    scene()->activeShader()->uniformNamed(NKS_M16_MVP).bindM16(mvp);
    
    nkGetGLError();
    
}

void NKMeshNode::bindTextures(){
    
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
            if (scene()->activeShader()->hasUniformNamed(NKS_TEXTURE_RECT_SCALE)) {
                scene()->activeShader()->uniformNamed(NKS_TEXTURE_RECT_SCALE).bindV2(V2MakeF(1.0));
            }
            
            nkGetGLError();
            
            //            }
            scene()->boundTexture = _textures[0].get();
        }
    }
    
    nkGetGLError();
}


//-(void)pushStyle {
//    
//    [super pushStyle];
//    
//    if (_lineWidth != 0) {
//#if NK_USE_GL3
//        [[self.scene.activeShader uniformNamed:NKS_F1_GL_LINEWIDTH] bindF1:_lineWidth];
//#else
//        glLineWidth(_lineWidth);
//#endif
//    }
//    if (_pointSize) {
//        glPointSize(_pointSize);
//    }
//    
//}

V6t NKMeshNode::getAabb() {
    return _vertexBuffer->boundingBox();
}

void NKMeshNode::customDraw(){
    
    if (_color.data.a || _numTextures) {
        setupViewMatrix();
        pushStyle();

        if (scene()->activeShader()->hasUniformNamed(NKS_V4_COLOR)){
            if (!_color.data.a) {
                scene()->activeShader()->uniformNamed(NKS_V4_COLOR).bindV4(V4MakeF(0.0));
            }else {
                scene()->activeShader()->uniformNamed(NKS_V4_COLOR).bindV4(glColor());
            }
        }
        
        nkGetGLError();
        
        bindTextures();

        if (scene()->boundVertexBuffer != _vertexBuffer.get()) {
            _vertexBuffer->bind();
            scene()->boundVertexBuffer = _vertexBuffer.get();
        }
        
        if (_vertexBuffer->indexBuffer.size()){
            //_vertexBuffer->indexBuffer.bind();
            
            //nkLog("draw %d indexed triangles", (int)_vertexBuffer->indexBuffer.size() / 3);
            
            glDrawElements(_drawMode, (int)_vertexBuffer->indexBuffer.size(), GL_UNSIGNED_INT, _vertexBuffer->indexBuffer.data());
            //_vertexBuffer->indexBuffer.unbind();
            nkGetGLError();
        }
        else{
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
        
    }
    
}

void NKMeshNode::drawBoundingBox() {
    
    shared_ptr<NKVertexBuffer> v = NKVertexBuffer::edges8v();
    
    v->bind();
    scene()->boundVertexBuffer = v.get();
    
    auto tmp = scene()->activeShader();
    
    auto uniformShader = NKShaderProgram::shaderNamed("boxShader",NKS_COLOR_MODE_UNIFORM,0, 0 );
    
    nkGetGLError();
    
    scene()->setActiveShader(uniformShader.get());
    scene()->activeShader()->uniformNamed(NKS_V4_COLOR).bindV4({1.0,1.0,1.0,1.0});
    
    M16t mvp;
    
    V6t bounds = _vertexBuffer->boundingBox();
    V3t center = _vertexBuffer->centerOfBoundingSize(bounds);
    V3t size = _vertexBuffer->sizeOfBoundingSize(bounds);
    
    M16t trans = M16TranslateWithV3(globalTransform(), center);
    M16t modelView = M16Multiply(scene()->camera->viewMatrix(),M16ScaleWithV3(trans, size*_size));
    
    if(scene()->activeShader()->hasUniformNamed(NKS_M16_MV) ){
        scene()->activeShader()->uniformNamed(NKS_M16_MV).bindM16(modelView);
    }
    
    if(scene()->activeShader()->hasUniformNamed(NKS_M9_NORMAL) ){
        scene()->activeShader()->uniformNamed(NKS_M9_NORMAL).bindM9(M16GetInverseNormalMatrix(modelView));
    }
    
    mvp = M16Multiply(scene()->camera->projectionMatrix(),modelView);
    
    scene()->activeShader()->uniformNamed(NKS_M16_MVP).bindM16(mvp);
    
    nkGetGLError();
    glDrawElements(GL_LINES, (int)v->indexBuffer.size(), GL_UNSIGNED_INT, v->indexBuffer.data());
    //glDrawArrays(GL_LINES, 0, (int)v->size());
    
    scene()->setActiveShader(tmp);
    
    nkGetGLError();
}

void NKMeshNode::customDrawWithHitShader() {
    
    setupViewMatrix();

    scene()->activeShader()->uniformNamed(NKS_V4_COLOR).bindV4(uidColor->c4Color());
    
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

NKMeshNode::~NKMeshNode(){
#warning TEX MAN
//    for (auto t : _textures){
//        [NKTextureManager removeNode:self forTexture:t];
//    }
}

