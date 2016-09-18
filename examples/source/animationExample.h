#pragma once

#include <nodeKitten/nodeKitten.h>

class AnimationExample : public Scene {
public:
    
    AnimationExample(S2t size) : Scene(size){};

    void moveToView() override;
    void handleUXEvent(std::shared_ptr<UXEvent> event) override;
};