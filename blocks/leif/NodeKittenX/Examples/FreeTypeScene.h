//
// Created by Leif Shackelford on 4/25/15.
//

#ifndef NODEKITTENX_FREETYPESCENE_H
#define NODEKITTENX_FREETYPESCENE_H

#include "NodeKitten.h"

class FreeTypeScene : public NKSceneNode {

public:

    FreeTypeScene(S2t size) : NKSceneNode(size){};
    ~FreeTypeScene(){};

    void moveToView() override;
    void customDraw() override;
    void handleEvent(NKEvent& event) override;
};

#endif /* defined(NODEKITTENX_FREETYPESCENE_H) */
