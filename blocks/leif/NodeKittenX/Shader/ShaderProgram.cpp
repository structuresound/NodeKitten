//
//  NOCShaderProgram.m
//  Nature of Code
//
//  Created by William Lindmeier on 2/2/13.
//  Copyright (c) 2013 wdlindmeier. All rights reserved.
//

#include "ShaderProgram.h"
#include "ShaderTools.h"
#include "ShaderModule.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "Node.h"

// EMSCRIPTEN / WEB_GL IN SAFARI

//    OES_texture_float
//    GL_OES_texture_float
//    OES_texture_float_linear
//    GL_OES_texture_float_linear
//    OES_texture_half_float
//    GL_OES_texture_half_float
//    OES_texture_half_float_linear
//    GL_OES_texture_half_float_linear
//    OES_standard_derivatives
//    GL_OES_standard_derivatives
//    EXT_shader_texture_lod
//    GL_EXT_shader_texture_lod
//    WEBKIT_EXT_texture_filter_anisotropic
//    GL_WEBKIT_EXT_texture_filter_anisotropic
//    OES_vertex_array_object
//    GL_OES_vertex_array_object
//    OES_element_index_uint
//    GL_OES_element_index_uint
//    WEBGL_lose_context
//    GL_WEBGL_lose_context
//    WEBGL_compressed_texture_s3tc
//    GL_WEBGL_compressed_texture_s3tc
//    WEBGL_depth_texture
//    GL_WEBGL_depth_texture
//    ANGLE_instanced_arrays
//    GL_ANGLE_instanced_arrays

Shader* Shader::activeShader = nullptr;
shared_ptr<Shader> Shader::fillShader = nullptr;

map<string, shared_ptr<Shader>> Shader::programCache;
map<UB4t, Node*> Shader::nodeMap;

static GLubyte GLUBYTE_MAX = UCHAR_MAX;

void Shader::glInit(){
    fillShader = Shader::shaderNamed("uColorLightShader",NKS_COLOR_MODE_UNIFORM,0, 1 );
}

void Shader::newUIDColorForNode(Node *node){
    
    for (int i = 0; i < INT_MAX; i++) {
        auto test = UB4t {random(GLUBYTE_MAX), random(GLUBYTE_MAX), random(GLUBYTE_MAX), GLUBYTE_MAX};
        if (!nodeMap[test]) {
            node->uidColor = test;
            nodeMap[test] = node;
            return;
        }
    }
    
    nkLog("failed to get UID color");
}

Node* Shader::nodeForColor(UB4t color){
    return nodeMap[color];
}

vector<bool> Shader::vertexAttributeState {0,0,0,0,0,0,0};

shared_ptr<Shader> Shader::shaderNamed(string name){
    auto existing = Shader::programCache[name];
    //if (!existing) nkLog("error : no shader prog found for name : %s", name);
    return existing;
}

shared_ptr<Shader> Shader::shaderNamed(string name, NKS_COLOR_MODE colorMode, U1t numTextures, int numLights, int batchSize){
    
    auto existing = Shader::shaderNamed(name);
    if (existing) return existing;
    
    auto newShader = make_shared<Shader>(name, colorMode, numTextures, numLights, batchSize);
    
    Shader::programCache[name]=newShader;
    
    nkAssert(newShader->load(), "ERROR LOADING SHADER");
    
    nkLog("shader *%d* named: %s", newShader->glName, name.c_str());
    
    nkLog("</SHADER>");
    
    return newShader;
}

shared_ptr<Shader> Shader::shaderNamed(string name, vector<ShaderModule> modules, int batchSize){
    
    auto existing = Shader::shaderNamed(name);
    if (existing) return existing;
    
    auto newShader = make_shared<Shader>(name, modules, batchSize);
    
    Shader::programCache[name]=newShader;
    
    if (!newShader->load()){
        nkAssert(0, "ERROR LOADING SHADER");
    }
    
    nkLog("shader *%d* named: %s", newShader->glName, name.c_str());
    
    nkLog("</SHADER>");
    
    return newShader;
}

Shader::Shader() {
    nkLog("<SHADER>");
}

Shader::Shader(string name_, vector<ShaderModule> modules_, int batchSize) : Shader(){
    
    name = name_;
    _batchSize = batchSize;
    
    modules = move(modules_);
    
    if (hasUniformNamed(NKS_V2_BATCH_POSITION)) {
        _uniforms.push_back(nksu(NKS_PRECISION_HIGH, NKS_TYPE_M16, NKS_M16_MVP));
    }
    else {
        _uniforms.push_back(nksua(NKS_PRECISION_HIGH, NKS_TYPE_M16, NKS_M16_MVP, batchSize));
    }
    
    // INSTANCE ID's for batch
    
    if (batchSize) {
        extensions = vector<string>{nks(NKS_EXT_DRAW_INSTANCED),nks(NKS_EXT_GPU_SHADER)};
    }
#if NK_USE_WEBGL
    extensions.push_back("#extension WEBKIT_EXT_texture_filter_anisotropic : enable");
    extensions.push_back("#extension WEBGL_depth_texture : enable");
    extensions.push_back("#extension OES_element_index_uint : enable");
    extensions.push_back("#extension OES_vertex_array_object : enable");
#endif
    
    /////// COMPILE /////////
    
    calculateCommonVertexVaryings();
}

Shader::Shader(string name_, NKS_COLOR_MODE colorMode, U1t numTex, int numLights, int batchSize) : Shader() {
    
    name = name_;
    _batchSize = batchSize;
    _colorMode = colorMode;
    
    _uniforms.push_back(nksua(NKS_PRECISION_HIGH, NKS_TYPE_M16, NKS_M16_MVP, batchSize));
    
    // INSTANCE ID's for batch
    
    
    if (batchSize) {
        extensions = vector<string>{nks(NKS_EXT_DRAW_INSTANCED),nks(NKS_EXT_GPU_SHADER)};
    }
    
#if NK_USE_WEBGL
    extensions.push_back("#extension WEBKIT_EXT_texture_filter_anisotropic : enable");
    extensions.push_back("#extension WEBGL_depth_texture : enable");
    extensions.push_back("#extension OES_element_index_uint : enable");
    extensions.push_back("#extension OES_vertex_array_object : enable");
#endif
    
    // ADD MODULES
    
    if (colorMode != NKS_COLOR_MODE_NONE) {
        modules.push_back(ShaderModule::colorModule(colorMode, batchSize));
    }
    if (numTex != 0) {
        modules.push_back(ShaderModule::textureModule(numTex));
    }
    
    if (numLights) {
        //#if NK_USE_GLES
        //        modules.push_back(ShaderModule::lightModule(false, batchSize));
        //#else
        modules.push_back(ShaderModule::lightModule(true, batchSize)); //!NK_USE_GLES
        //#endif
    }
    
    /////// COMPILE /////////
    
    calculateCommonVertexVaryings();
}

void Shader::calculateCommonVertexVaryings() {
    
    for (auto& m : modules){
        if (m.colorMode > NKS_COLOR_MODE_NONE) {
            if (_colorMode == NKS_COLOR_MODE_NONE) {
                _colorMode = m.colorMode;
            }
            else nkAssert((_colorMode == m.colorMode), "MODULES DON'T AGREE ON COLOR MODE");
        }
        if (m.numTexures > 0) {
            if (_numTextures == 0) {
                _numTextures = m.numTexures;
            }
            else nkAssert((_numTextures == m.numTexures), "MODULES DON'T AGREE ON MAX TEXURES");
        }
    }
    
    _attributes.push_back(nksa(NKS_TYPE_V4, NKS_V4_POSITION));
    
    if (hasUniformNamed(NKS_M9_NORMAL)) {
        _attributes.push_back(nksa(NKS_TYPE_V3, NKS_V3_NORMAL));
    }
    if (hasVaryingNamed(NKS_V2_TEXCOORD)) {
        _attributes.push_back(nksa(NKS_TYPE_V2, NKS_V2_TEXCOORD));
    }
    
    if (_colorMode == NKS_COLOR_MODE_VERTEX) {
        _attributes.push_back(nksa(NKS_TYPE_V4, NKS_V4_COLOR));
    }
    
    // ADD COMMON VERTEX VARYINGS FOR MODULES
    
    // NORMAL MATRIX
    
    if (hasUniformNamed(NKS_M9_NORMAL)) {
        if (_batchSize) {
#if (NK_USE_GLES && !NK_USE_WEBGL)
            _vertMain.push_back("v_normal = normalize(u_normalMatrix[gl_InstanceIDEXT] * a_normal);");
#else
            _vertMain.push_back("v_normal = normalize(u_normalMatrix[gl_InstanceID] * a_normal);");
#endif
        }
        else {
            _vertMain.push_back("v_normal = normalize(u_normalMatrix * a_normal);");
        }
    }
    
    // EYE POSITION USING MODEL MATRIX
    
    if (hasUniformNamed(NKS_M16_MV)) {
        if (hasVaryingNamed(NKS_V4_POSITION)){
            if (_batchSize) {
#if (NK_USE_GLES && !NK_USE_WEBGL)
                _vertMain.push_back("v_position = u_modelViewMatrix[gl_InstanceIDEXT] * a_position;");
#else
                _vertMain.push_back("v_position = u_modelViewMatrix[gl_InstanceID] * a_position;");
#endif
            }
            else {
                _vertMain.push_back("v_position = u_modelViewMatrix * a_position;");
            }
        }
    }
    // MODEL VIEW PROJECTION MATRIX
    
    if (hasUniformNamed(NKS_V2_BATCH_POSITION)){
#if (NK_USE_GLES && !NK_USE_WEBGL)
        _vertMain.push_back("gl_Position = u_modelViewProjectionMatrix * vec4(a_position.xy * u_batchScale[gl_InstanceIDEXT] + u_batchPosition[gl_InstanceIDEXT], a_position.z, 1);");
#else
        _vertMain.push_back("gl_Position = u_modelViewProjectionMatrix[gl_InstanceID] * vec4((a_position.xy * u_batchPosition[gl_InstanceID]), a_position.z, 1);");
#endif
    }
    else if (hasUniformNamed(NKS_M16_MVP)) {
        
        if (_batchSize) {
#if (NK_USE_GLES && !NK_USE_WEBGL)
            _vertMain.push_back("gl_Position = u_modelViewProjectionMatrix[gl_InstanceIDEXT] * a_position;");
#else
            _vertMain.push_back("gl_Position = u_modelViewProjectionMatrix[gl_InstanceID] * a_position;");
#endif
        }
        else {
            _vertMain.push_back("gl_Position = u_modelViewProjectionMatrix * a_position;");
        }
        
    }
    
}

vector<string> Shader::uniformNames() {
    
    vector<string> names;
    
    for (auto& v : _uniforms) {
        names.push_back(v.nameString());
    }
    
    return names;
}

string Shader::vertexSource() {
    
    string shader = NKS_GLSL_VERSION;
    
    shader += "\n//*** NK VERTEX SHADER ***//\n";
#if NK_USE_GL3
    shader += "#version 330 core \n";
#else
    for (auto& s : extensions) {
        shader += s;
        shader += "\n";
    }
#endif
    
    
#if NK_USE_GLES
    shader += "precision highp float; \n";
#endif
    
    for (auto& m : modules){
        shader += m.types;
        shader += "\n";
    }
    
    for (auto& v : _attributes) {
        shader += v.declarationStringForSection(NKS_VERTEX_SHADER);
        shader += "\n";
    }
    
    vector<ShaderVariable> uniforms;
    vector<ShaderVariable> varyings;
    
    for (auto& v : _uniforms) {
        shader += v.declarationStringForSection(NKS_VERTEX_SHADER);
        shader += "\n";
        uniforms.push_back(v);
    }
    
    for (auto& v : _varyings) {
        shader += v.declarationStringForSection(NKS_VERTEX_SHADER);
        shader += "\n";
        varyings.push_back(v);
    }
    
    for (auto& m : modules){
        for (auto& v : m.uniforms) {
            if (!_c::contains(uniforms, v)) {
                shader += v.declarationStringForSection(NKS_VERTEX_SHADER);
                shader += "\n";
                uniforms.push_back(v);
            }
        }
        for (auto& v : m.varyings) {
            if (!_c::contains(varyings, v)) {
                shader += v.declarationStringForSection(NKS_VERTEX_SHADER);
                shader += "\n";
                varyings.push_back(v);
            }
        }
    }
    
    shader += "void main() { \n";
    
    vector<string> mainStrings;
    
    for (auto& s : _vertMain){
        if (!_c::contains(mainStrings, s)) {
            shader += s;
            shader += "\n";
            mainStrings.push_back(s);
        }
    }
    
    for (auto& m : modules){
        for (auto& s : m.vertexMain){
            if (!_c::contains(mainStrings, s)) {
                shader += s;
                shader += "\n";
                mainStrings.push_back(s);
            }
        }
    }
    
    shader += "}\n";
    
    return shader;
}

string Shader::fragmentSource() {
    
    string shader = NKS_GLSL_VERSION;
    
    shader += "\n//*** NK FRAGMENT SHADER ***//\n";
    shader += "\n";
    
#if NK_USE_GL3
    shader += "#version 330 core \n";
    shader += "layout ( location = 0 ) out vec4 FragColor; \n";
#else
    for (auto& s : extensions) {
        shader += s;
        shader += "\n";
    }
#endif
    
    
#if NK_USE_GLES
    shader += "precision highp float; \n";
#else
    
#endif
    
    for (auto& m : modules){
        shader += m.types;
        shader += "\n";
    }
    
    vector<ShaderVariable> uniforms;
    vector<ShaderVariable> varyings;
    
    for (auto& v : _uniforms) {
        shader += v.declarationStringForSection(NKS_FRAGMENT_SHADER);
        shader += "\n";
        uniforms.push_back(v);
    }
    for (auto& v : _varyings) {
        shader += v.declarationStringForSection(NKS_FRAGMENT_SHADER);
        shader += "\n";
        varyings.push_back(v);
    }
    
    for (auto& m : modules){
        for (auto& v : m.uniforms) {
            if (!_c::contains(uniforms, v)) {
                shader += v.declarationStringForSection(NKS_FRAGMENT_SHADER);
                shader += "\n";
                uniforms.push_back(v);
            }
        }
        for (auto& v : m.varyings) {
            if (!_c::contains(varyings, v)) {
                shader += v.declarationStringForSection(NKS_FRAGMENT_SHADER);
                shader += "\n";
                varyings.push_back(v);
            }
        }
    }
    
    for (auto& m : modules){
        for (auto& f : m.fragFunctions) {
            if (f.constants.length()) {
                shader+= f.constants + "\n";
            }
            if (f.function.length()) {
                shader+= f.functionString() + "\n";
            }
        }
    }
    
    
    shader += "void main() { \n";
    
    shader += "vec4 inputColor = vec4(1.0); \n";
    
    for (auto& s : _fragMain){
        shader += s;
        shader += "\n";
    }
    
    shader += nks(NKS_V4_GL_FRAG_COLOR) + " = ";
    
    vector<ShaderFunction> fragFunctions;
    
    for (auto& module : modules) {
        for (auto& function : module.fragFunctions) {
            fragFunctions.push_back(function);
        }
    }
    
    for (int i = (int)fragFunctions.size()-1; i >= 0 ; i--){
        
        auto& function = fragFunctions[i];
        
        if (i == 0) {
            shader += function.name + "(inputColor";
            for (int p = 0; p < fragFunctions.size(); p++){
                shader += ")";
            }
            shader += ";\n";
        }
        else {
            shader += function.name + "(";
        }
    }
    
//    for (int i = (int)modules.size()-1; i >= 0 ; i--){
//        
//        auto& module = modules[i];
//        
//        auto& function = module.fragFunctions[0];
//        
//        if (i == 0) {
//            shader += function.name + "(inputColor";
//            for (int p = 0; p < modules.size(); p++){
//                shader += ")";
//            }
//            shader += ";\n";
//        }
//        else {
//            shader += function.name + "(";
//        }
//    }
    
    shader += "}\n";
    
    return shader;
}

bool Shader::load()
{
    
    GLuint vertShader, fragShader;
    
    // Create shader program.
    glName = glCreateProgram();
    
    // Create and compile vertex shader.
    string vertSource {vertexSource()};
    string fragSource {fragmentSource()};
    
#if NK_LOG_GL
    nkLog("vertex source: %s",vertSource.c_str());
#endif
    
    if ( !compileShader(&vertShader,GL_VERTEX_SHADER,vertSource))
    {
        nkAssert(0, "Failed to compile VERTEX shader:");
    }
    
#if NK_LOG_GL
    nkLog("fragment source: %s",fragSource.c_str());
#endif
    
    // Create and compile vertex shader.
    if ( !compileShader(&fragShader,GL_FRAGMENT_SHADER,fragSource))
    {
        nkAssert(0, "Failed to compile FRAGMENT shader:");
    }
    
    GetGLError();
    
    // Attach vertex shader to program.
    glAttachShader(glName, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(glName, fragShader);
    
    GetGLError();
    
#pragma mark - bind attribute string
#warning ATTRIB PROBLEM HERE ? ->
    //int numAttributes = 0;
    //#if TARGET_OS_IPHONE
    
    // SELECTIVE
    
    for (auto& v : _attributes) {
        glEnableVertexAttribArray(v.name);
        glBindAttribLocation(glName, v.name, v.nameString().c_str());
    }
    
    GetGLError();
    
    // Link program.
    if ( !linkProgram(glName) )
    {
        
        nkLog("Failed to link program: %s", name.c_str());
        
        if (vertShader)
        {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader)
        {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (glName)
        {
            glDeleteProgram(glName);
            glName = 0;
        }
        
        return false;
    }
    
    GetGLError();
    
    for (auto& v : _attributes) {
        
        v.glLocation = glGetAttribLocation(glName, v.nameString().c_str());
        if (v.glLocation) {
            //glEnableVertexAttribArray(v.glLocation);
        }
        
#if NK_LOG_GL
        nkLog("Attribute location %d, %s",v.glLocation, v.nameString().c_str());
#endif
    }
    
    GetGLError();
    
    for (auto& v : _uniforms) {
        int uniLoc = glGetUniformLocation(glName, v.nameString().c_str());
        if (uniLoc > -1)
        {
            v.glLocation = uniLoc;//_uniformLocations[uniName] = @(uniLoc);
            //NSLog(@"Uniform location %d, %@",v.glLocation, v.nameString);
        }
        else
        {
            nkLog("WARNING: Couldn't find location for uniform named: %s", v.nameString().c_str());
        }
    }
    
    for (auto& m : modules) {
        
        for (auto& v : m.uniforms) {
            int uniLoc = glGetUniformLocation(glName, v.nameString().c_str());
            if (uniLoc > -1)
            {
                v.glLocation = uniLoc;//_uniformLocations[uniName] = @(uniLoc);
                //NSLog(@"Uniform location %d, %@",v.glLocation, v.nameString);
            }
            else
            {
                nkLog("WARNING: Couldn't find location for uniform named: %s", v.nameString().c_str());
            }
        }
        
    }
    
    GetGLError();
    
    if (hasUniformNamed(NKS_LIGHT)) {
        
        nkLog("getting positions for Light properties");
        
        uniformNamed(NKS_LIGHT).glLocation = glGetUniformLocation(glName, "u_light[0].position");
//        
//        NSArray *members = @[@"isEnabled",@"isLocal",@"isSpot",@"ambient",@"color",@"position",@"halfVector",@"coneDirection",
//                             @"spotCosCutoff", @"spotExponent",@"constantAttenuation",@"linearAttenuation",@"quadraticAttenuation"];
//        
//        for (NSString *member in members) {
//            
//            NSString *name = [@"u_light." stringByAppendingString:member];
//            int uniLoc = glGetUniformLocation(self.glPointer, [name UTF8String]);
//            if (uniLoc > -1)
//            {
//                NSLog(@"Uniform location %d, %@",uniLoc, name);
//            }
//            
//        }
        
    }
    // Store the locations in an immutable collection
    
    // Release vertex and fragment shaders.
    if (vertShader)
    {
        glDetachShader(glName, vertShader);
        glDeleteShader(vertShader);
    }
    
    if (fragShader)
    {
        glDetachShader(glName, fragShader);
        glDeleteShader(fragShader);
    }
    
    GetGLError();
    
    return true;
    
}

bool Shader::compileShader(GLuint *shader, GLenum type, string& shaderSource){
    
    GLint status;
    const GLchar *source;
    
    source = (GLchar *)shaderSource.c_str();
    if (!source)
    {
        nkLog("Failed to load vertex shader: %s", name.c_str());
        return false;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if NK_LOG_GL
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        nkLog("Shader compile log:\n -> %s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
    
}

bool Shader::linkProgram(GLuint prog){
    
    GLint status;
    glLinkProgram(prog);
    
#if NK_LOG_GL
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        nkLog("Program link log:\n -> %s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        return false;
    }
    
    return true;
}

bool Shader::validateProgram(GLuint prog) {
    
    GLint logLength, status;
    
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        nkLog("Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0)
    {
        return false;
    }
    
    return true;
}

#pragma mark - QUERY

bool Shader::operator==(const Shader& other){
    return glName == other.glName;
}

bool Shader::hasAttributeNamed(NKS_ENUM name){
    for (auto& v : _attributes){
        if (v.name == name) return true;
    }
    return false;
}


ShaderVariable& Shader::attributeNamed(NKS_ENUM name){
    
    for (auto& v : _attributes){
        if (v.name == name) return v;
    }
    return _attributes[0];
}

bool Shader::hasUniformNamed(NKS_ENUM name){
    for (auto& module : modules) {
        for (auto& v : module.uniforms){
            if (v.name == name) return true;
        }
    }
    
    for (auto& v : _uniforms){
        if (v.name == name) return true;
    }
    
    return false;
}

ShaderVariable& Shader::uniformNamed(NKS_ENUM name) {
    for (auto& module : modules) {
        for (auto& v : module.uniforms){
            if (v.name == name) return v;
        }
    }
    
    for (auto& v : _uniforms){
        if (v.name == name) return v;
    }
    
    return _uniforms[0];
}

bool Shader::hasVaryingNamed(NKS_ENUM name){
    for (auto& module : modules) {
        for (auto& v : module.varyings){
            if (v.name == name) return true;
        }
    }
    
    for (auto& v : _varyings){
        if (v.name == name) return true;
    }
    
    return false;
}

ShaderVariable& Shader::varyingNamed(NKS_ENUM name) {
    for (auto& module : modules) {
        for (auto& v : module.varyings){
            if (v.name == name) return v;
        }
    }
    
    for (auto& v : _varyings){
        if (v.name == name) return v;
    }
    
    return _varyings[0];
}


ShaderVariable& Shader::vertVarNamed(NKS_ENUM name) {
    for (auto& v : _vertVars){
        if (v.name == name) return v;
    }
    return _vertVars[0];
}

ShaderVariable& Shader::fragVarNamed(NKS_ENUM name) {
    for (auto& v : _fragVars){
        if (v.name == name) return v;
    }
    return _fragVars[0];
}

void Shader::unload()
{
    if (glName)
    {
        nkLog("unload shader %d", glName);
        glDeleteProgram(glName);
        glName = 0;
    }
}

Shader::~Shader(){
    unload();
}

bool Shader::use()
{
    if (this == activeShader) return false;
    activeShader = this;
    
    glUseProgram(glName);
    
    nkGetGLError();
    
    // prep globals
    
    if  (hasUniformNamed(NKS_S2D_TEXTURE)){
        uniformNamed(NKS_S2D_TEXTURE).bindI1(0);
        glUniform1i(uniformNamed(NKS_S2D_TEXTURE).glLocation, 0);
        nkGetGLError();
    }
    
    for (auto& m : modules) {
        m.updateUniforms();
        nkGetGLError();
    }
    
    nkGetGLError();
    
    return true;
}

#define gl_debug

//- (void)enableAttribute3D:(NSString *)attribName withArray:(const GLvoid*)arrayValues
//{
//    NSNumber *attrVal = self.attributes[attribName];
//    assert(attrVal);
//    GLuint attrLoc = [attrVal intValue];
//    glVertexAttribPointer(attrLoc, 3, GL_FLOAT, GL_FALSE, 0, arrayValues);
//    glEnableVertexAttribArray(attrLoc);
//}
//
//- (void)enableAttribute2D:(NSString *)attribName withArray:(const GLvoid*)arrayValues
//{
//    NSNumber *attrVal = self.attributes[attribName];
//    assert(attrVal);
//    GLuint attrLoc = [attrVal intValue];
//    glVertexAttribPointer(attrLoc, 2, GL_FLOAT, GL_FALSE, 0, arrayValues);
//    glEnableVertexAttribArray(attrLoc);
//}

//- (void)disableAttributeArray:(NSString *)attribName
//{
//    NSNumber *attrVal = self.attributes[attribName];
//    assert(attrVal);
//    GLuint attrLoc = [attrVal intValue];
//    glDisableVertexAttribArray(attrLoc);
//}

