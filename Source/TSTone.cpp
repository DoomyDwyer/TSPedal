/*
  ==============================================================================
    TSTone.cpp
    Created: 6 Feb 2021 1:59:16pm
    Author:  Hack Audio
  ==============================================================================
*/

#include "TSTone.h"



float TSTone::processSample(float Vi, int c){
    float Vo = b0*Vi + b1*x1[c] + b2*x2[c] + b3*x3[c] + b4*x4[c];
    float vx = Vi/(R6*Gz) + x1[c]/Gz + x2[c]*R2/(G2*Gz*P1);
    x1[c] = (2.f/R1)*(vx) - x1[c];
    x2[c] = (2.f/R2)*(vx/Gr + x2[c]*(P1+R5)/Gr) - x2[c];
    x3[c] = (2.f/R3)*(vx/Gs + x3[c]*(P2+R8)/Gs) - x3[c];
    x4[c] = 2*Vo/R11 + x4[c];
    return Vo;
}

void TSTone::prepare(float newFs){
    
    if (Fs != newFs){
        Fs = newFs;
        updateCoefficients();
    }
    
}

void TSTone::setKnobs(float toneKnob, float outputKnob){
    bool updateFlag = false;
    
    if (potTone != toneKnob){
        potTone = 0.0000001f + 0.999998f*toneKnob;
        updateFlag = true;
    }
    
    if (potOut != outputKnob){
        potOut = 0.000001f + 0.999999f*outputKnob;
        updateFlag = true;
    }
    
    if (updateFlag){
        updateCoefficients();
    }
}

void TSTone::updateCoefficients(){
    
    Ts = 1.f/Fs;

    R1 = Ts/(2.f*C1);
    R2 = Ts/(2.f*C2);
    R3 = Ts/(2.f*C3);
    R4 = Ts/(2.f*C4);
    
    R10 = 100e3 * (1.f-potOut);
    R11 = 100e3 * potOut;

    P1 = 20000.f * potTone;
    P2 = 20000.f * (1.f-potTone);
    
    // Grouped resistances
    G2 = 1.f + R2/P1 + R5/P1;
    G3 = 1.f + R3/P2 + R8/P2;
    Gx = 1.f + R7/(G3*P2);
    Gz = (1.f/R1 + 1.f/R6 + 1.f/(G2*P1));
    Go = (1.f + R10/R11 + R9/R11 + R4/R11);
    Gr = 1.f + P1/R2 + R5/R2;
    Gs = 1.f + P2/R3 + R8/R3;
    
    b0 = Gx/(Go*R6*Gz);
    b1 = Gx/(Go*Gz);
    b2 = Gx*R2/(G2*Gz*Go*P1);
    b3 = -R3*R7/(Go*G3*P2);
    b4 = -R4/Go;

       
    //Fs = 48000.f;
    //Ts = 1.f/Fs;
    //
    //C1 = .22e-6;
    //R1 = Ts/(2.f*C1);
    //
    //C2 = .22e-6;
    //R2 = Ts/(2.f*C2);
    //
    //C3 = .22e-6;
    //R3 = Ts/(2.f*C3);
    //
    //C4 = 1e-6;
    //R4 = Ts/(2.f*C4);
    //
    //R5 = 220.f;
    //R6 = 1000.f;
    //R7 = 1000.f;
    //R8 = 220.f;
    //
    //R9 = 1000.f;
    //potOut = 1.f; // [0.000001 - 1] (-120 dB to 0 dB)
    //R10 = 100e3 * (1.f-potOut);
    //R11 = 100e3 * potOut;
    //
    //potTone = 0.5f; // Cannot be exactly 0 or 1
    //P1 = 20000.f * potTone;
    //P2 = 20000.f * (1.f-potTone);
    //
    //// Grouped Resistances
    //G2 = 1.f + R2/P1 + R5/P1;
    //G3 = 1.f + R3/P2 + R8/P2;
    //Gx = 1.f + R7/(G3*P2);
    //Gz = (1.f/R1 + 1.f/R6 + 1.f/(G2*P1));
    //Go = (1.f + R10/R11 + R9/R11 + R4/R11);
    //Gr = 1.f + P1/R2 + R5/R2;
    //Gs = 1.f + P2/R3 + R8/R3;
    //
    //// Filter coefficients
    //b0 = Gx/(Go*R6*Gz);
    //b1 = Gx/(Go*Gz);
    //b2 = Gx*R2/(G2*Gz*Go*P1);
    //b3 = -R3*R7/(Go*G3*P2);
    //b4 = -R4/Go;

}