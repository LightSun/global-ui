//
// Created by Administrator on 2020/11/22 0022.
//

#include <math.h>
#include <float.h>
#include "VelocityTracker.h"
#include "core/SynchronizedPool.h"

#define TMIN(a, b) (a < b ? a : b)

using namespace h7;

static SynchronizedPool<VelocityTracker> sPool;

VelocityTracker* VelocityTracker::obtain(){
    auto ptr = sPool.acquire();
    ptr->reset();
    return ptr;
}
void VelocityTracker::recycle(){
    sPool.release(this);
}
//====================
_VelocityTracker::_VelocityTracker(){

}
void _VelocityTracker::computeCurrentVelocity(int units, float maxVelocity){
     auto vx = getVelocityX(units);
     if(vx > maxVelocity){
         vx = maxVelocity;
     }
     auto vy = getVelocityY(units);
     if(vy > maxVelocity){
         vy = maxVelocity;
     }
     velocityX = vx;
     velocityY = vy;
}

void _VelocityTracker::computeCurrentVelocity(int units){
    computeCurrentVelocity(units, FLT_MAX);
}

void _VelocityTracker::reset(){
    deltaX = 0;
    deltaY = 0;
    numSamples = 0;
    for (int i = 0; i < _VT_DEF_SAMPLE_SIZE; i++) {
        meanX[i] = 0;
        meanY[i] = 0;
        meanTime[i] = 0;
    }
    lastTime = 0;
}

void _VelocityTracker::add(float x, float y, unsigned long long timeStamp){
    if(lastTime == 0){
        start(x, y, timeStamp);
    }else{
        update(x, y, timeStamp);
    }
}
void _VelocityTracker::start(float x, float y, unsigned long long timeStamp) {
    lastX = x;
    lastY = y;
    lastTime = timeStamp;
    numSamples = 0;
}

void _VelocityTracker::update(float x, float y, unsigned long long currTime) {
    deltaX = x - lastX;
    deltaY = y - lastY;
    lastX = x;
    lastY = y;
    long long deltaTime = currTime - lastTime;
    lastTime = currTime;
    int index = numSamples % _VT_DEF_SAMPLE_SIZE;
    meanX[index] = deltaX;
    meanY[index] = deltaY;
    meanTime[index] = deltaTime;
    numSamples++;
}

float _VelocityTracker::getVelocityX(int units) {
    int c = TMIN(_VT_DEF_SAMPLE_SIZE, numSamples);
    float meanX = getAverage(this->meanX, c);
    float meanTime = getAverage(this->meanTime, c) * 1.0/ units;
    if (meanTime == 0) return 0;
    return meanX / meanTime;
}

float _VelocityTracker::getVelocityY(int units) {
    int c = TMIN(_VT_DEF_SAMPLE_SIZE, numSamples);
    float meanY = getAverage(this->meanY, c);
    float meanTime = getAverage(this->meanTime, c) * 1.0/ units;
    if (meanTime == 0) return 0;
    return meanY / meanTime;
}

float _VelocityTracker::getAverage(float *values, int numSamples) {
   // numSamples = bx::min(sampleSize, numSamples);
    float sum = 0;
    for (int i = 0; i < numSamples; i++) {
        sum += values[i];
    }
    return sum / numSamples;
}

long _VelocityTracker::getAverage(long long *values, int numSamples) {
    //numSamples = bx::min(sampleSize, numSamples);
    long sum = 0;
    for (int i = 0; i < numSamples; i++) {
        sum += values[i];
    }
    if (numSamples == 0) return 0;
    return sum / numSamples;
}

float _VelocityTracker::getSum(float *values, int numSamples) {
    //numSamples = bx::min(sampleSize, numSamples);
    float sum = 0;
    for (int i = 0; i < numSamples; i++) {
        sum += values[i];
    }
    if (numSamples == 0) return 0;
    return sum;
}
