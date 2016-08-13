//
//  OculusInterface.h
//  SimpleOculus
//
//  Created by Robby Kraft on 2/1/14.
//  Copyright (c) 2014 Robby Kraft. All rights reserved.
//

#include "../types/types.h"

#if NK_USE_OVR

#ifndef __NKOVR_h_
#define __NKOVR_h_

#include "OVR.h"


using namespace OVR;

class NKOculusInterface {

    Ptr<DeviceManager> pManager;
    Ptr<HMDDevice> pHMD;
    Ptr<SensorDevice> pSensor;
    SensorFusion* pFusionResult;
    HMDInfo Info;
    bool InfoLoaded;

    M16t _orientation;

    void log();
    M16t getRotationMatrix(const Quatf q);

public:

    void updateWithTimeSinceLast(F1t dt);

    NKOculusInterface();
    ~NKOculusInterface();

    float IPD;           // virtual interpupillary distance
};

#endif

#endif
