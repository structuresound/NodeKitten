//
// Created by Leif Shackelford on 4/25/15.
//

#ifndef NODEKITTENX_FREETYPESCENE_H
#define NODEKITTENX_FREETYPESCENE_H

#include <nodeKitten/core.h>

class FreeTypeTest : public Scene {

public:

    FreeTypeTest(S2t size) : Scene(size){};
    ~FreeTypeTest(){};

    void moveToView() override;
    void customDraw() override;
    void handleUXEvent(shared_ptr<UXEvent> event) override;
    void afterResize() override;
};

#endif /* defined(NODEKITTENX_FREETYPESCENE_H) */
