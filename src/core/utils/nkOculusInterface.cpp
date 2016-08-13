//
//  OculusInterface.m
//  SimpleOculus
//
//  Created by Robby Kraft on 2/1/14.
//  Copyright (c) 2014 Robby Kraft. All rights reserved.
//

#include "nkOculusInterface.h"

#if NK_USE_OVR

NKOculusInterface::NKOculusInterface(){
    IPD = .15f;    // for full-screen 1280x800: .08-FOV75  .14-FOV90  .2-FOV120

    System::Init();

    pFusionResult = new SensorFusion();
    pManager = *DeviceManager::Create();
    pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
    if (pHMD){
        InfoLoaded = pHMD->GetDeviceInfo(&Info);
        pSensor = *pHMD->GetSensor();
    }
    else{
        pSensor = *pManager->EnumerateDevices<SensorDevice>().CreateDevice();
    }
    if (pSensor){
        pFusionResult->AttachToSensor(pSensor);
    }

    log();

    _orientation = M16IdentityMake();
}

void NKOculusInterface::updateWithTimeSinceLast(F1t dt){

    if(pSensor){
        Quatf quaternion = pFusionResult->GetOrientation();
        float yaw, pitch, roll;
        quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

        _orientation = getRotationMatrix(quaternion);
    }
}

NKOculusInterface::~NKOculusInterface(){
    pSensor.Clear();
    pHMD.Clear();
    pManager.Clear();
    delete pFusionResult;
    System::Destroy();
}

void NKOculusInterface::log(){
    printf("----- Oculus Console -----\n");
    if (pHMD)
        printf(" [x] HMD Found\n");
    else
        printf(" [ ] HMD Not Found\n");
    if (pSensor)
        printf(" [x] Sensor Found\n");
    else
        printf(" [ ] Sensor Not Found\n");
    printf("--------------------------\n");
    if (InfoLoaded)
    {
        printf(" DisplayDeviceName: %s\n", Info.DisplayDeviceName);
        printf(" ProductName: %s\n", Info.ProductName);
        printf(" Manufacturer: %s\n", Info.Manufacturer);
        printf(" Version: %d\n", Info.Version);
        printf(" HResolution: %d\n", Info.HResolution);
        printf(" VResolution: %d\n", Info.VResolution);
        printf(" HScreenSize: %f\n", Info.HScreenSize);
        printf(" VScreenSize: %f\n", Info.VScreenSize);
        printf(" VScreenCenter: %f\n", Info.VScreenCenter);
        printf(" EyeToScreenDistance: %f\n", Info.EyeToScreenDistance);
        printf(" LensSeparationDistance: %f\n", Info.LensSeparationDistance);
        printf(" InterpupillaryDistance: %f\n", Info.InterpupillaryDistance);
        printf(" DistortionK[0]: %f\n", Info.DistortionK[0]);
        printf(" DistortionK[1]: %f\n", Info.DistortionK[1]);
        printf(" DistortionK[2]: %f\n", Info.DistortionK[2]);
        printf("--------------------------\n");
    }
    if(pSensor){
        Quatf quaternion = pFusionResult->GetOrientation();
        float yaw, pitch, roll;
        quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);
        printf(" Yaw: %f, Pitch: %f, Roll: %f\n", RadToDegree(yaw), RadToDegree(pitch), RadToDegree(roll));
    }
}

// consider
// static Matrix4f LookAtRH(const Vector3f& eye, const Vector3f& at, const Vector3f& up);
// or
// static Matrix4f LookAtLH(const Vector3f& eye, const Vector3f& at, const Vector3f& up);
// to replace below

//  quat to matrix conversion from OpenSceneGraph (C) 1998-2006 Robert Osfield
M16t NKOculusInterface::getRotationMatrix(const Quatf q)
{
    M16t _mat;
    double length2 =  q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
    if (fabs(length2) <= DBL_MIN)
    {
        _mat.colRow[0][0] = 1.0; _mat.colRow[1][0] = 0.0; _mat.colRow[2][0] = 0.0;
        _mat.colRow[0][1] = 0.0; _mat.colRow[1][1] = 1.0; _mat.colRow[2][1] = 0.0;
        _mat.colRow[0][2] = 0.0; _mat.colRow[1][2] = 0.0; _mat.colRow[2][2] = 1.0;
    }
    else
    {
        double rlength2;
        // normalize quat if required.
        // We can avoid the expensive sqrt in this case since all 'coefficients' below are products of two q components.
        // That is a square of a square root, so it is possible to avoid that
        if (length2 != 1.0)
        {
            rlength2 = 2.0/length2;
        }
        else
        {
            rlength2 = 2.0;
        }

        // Source: Gamasutra, Rotating Objects Using Quaternions
        //
        //http://www.gamasutra.com/features/19980703/quaternions_01.htm

        double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

        // calculate coefficients
        x2 = rlength2*q.x;
        y2 = rlength2*q.y;
        z2 = rlength2*q.z;

        xx = q.x * x2;
        xy = q.x * y2;
        xz = q.x * z2;

        yy = q.y * y2;
        yz = q.y * z2;
        zz = q.z * z2;

        wx = q.w * x2;
        wy = q.w * y2;
        wz = q.w * z2;

        // Note.  Gamasutra gets the matrix assignments inverted, resulting
        // in left-handed rotations, which is contrary to OpenGL and OSG's
        // methodology.  The matrix assignment has been altered in the next
        // few lines of code to do the right thing.
        // Don Burns - Oct 13, 2001
        _mat.colRow[0][0] = 1.0 - (yy + zz);
        _mat.colRow[1][0] = xy - wz;
        _mat.colRow[2][0] = xz + wy;


        _mat.colRow[0][1] = xy + wz;
        _mat.colRow[1][1] = 1.0 - (xx + zz);
        _mat.colRow[2][1] = yz - wx;

        _mat.colRow[0][2] = xz - wy;
        _mat.colRow[1][2] = yz + wx;
        _mat.colRow[2][2] = 1.0 - (xx + yy);
    }

#if 0
    _mat.M[0][3] = 0.0;
    _mat.M[1][3] = 0.0;
    _mat.M[2][3] = 0.0;

    _mat.M[3][0] = 0.0;
    _mat.M[3][1] = 0.0;
    _mat.M[3][2] = 0.0;
    _mat.M[3][3] = 1.0;
#endif
    return _mat;
}

#endif
