//
//  MeshBatcher.h
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#ifndef __MeshBatcher_h_
#define __MeshBatcher_h_

#include "Mesh.h"

namespace Shader {
  class Program;
}

class MeshBatcher : public Mesh
{
    GLuint spritesToDraw {0};
    bool _is2d {false};
    
public:
    
    using Mesh::Mesh;
    
    static std::shared_ptr<MeshBatcher> nodeWithObj(std::string objName, V3t size = V3(1.0), bool normalize = true, bool anchor = false);
    static std::shared_ptr<MeshBatcher> nodeWithPrimitive(NKPrimitive primitive, std::shared_ptr<Texture> texture, Color color, V3t size);
    static std::shared_ptr<MeshBatcher> nodeWithVbo(std::shared_ptr<VertexBuffer> buffer, GLenum drawMode, std::shared_ptr<Texture> texture, Color color, V3t size);
    
    M16Stack mvpStack;
    M16Stack mvStack;
    M9Stack normalStack;
    V4Stack childColors;
    
    // 2d
    V2Stack positionStack;
    F1Stack scaleStack;

    std::shared_ptr<Shader::Program> hitShader;

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    void setTexture(shared_ptr<Texture> texture) override;
#endif

    void setIs2d(bool is2d){_is2d = is2d;};
    void chooseShader() override;
    void customDraw() override;
    void drawChildren() override;
    void drawWithHitShader();
    
    void drawGeometry(U1t start, U1t spritesInBatch, bool useColor);
};

#endif

