//
// Created by Administrator on 2020/11/22 0022.
//

#pragma once

#include <unordered_map>
#include <memory>
#include <float.h>

namespace h7 {

#define _VT_DEF_SAMPLE_SIZE 10
#define ACTIVE_POINTER_ID 0

    struct _VelocityTracker {
        float lastX, lastY;
        float deltaX, deltaY;
        unsigned long long lastTime;
        int numSamples;
        float meanX[_VT_DEF_SAMPLE_SIZE];
        float meanY[_VT_DEF_SAMPLE_SIZE];
        long long meanTime[_VT_DEF_SAMPLE_SIZE]; //

        float velocityX {0};
        float velocityY {0};

    public:
        _VelocityTracker();

        //units: 1 means pixels per millisecond, 1000 means pixels per second
        void computeCurrentVelocity(int units, float maxVelocity);

        void computeCurrentVelocity(int units);

        void add(float x, float y, unsigned long long currTimeMills);

        void reset();

        static float getAverage(float *values, int numSamples);

        static long getAverage(long long *values, int numSamples);

        static float getSum(float *values, int numSamples);

    private:
        void start(float x, float y, unsigned long long timeStamp);

        void update(float x, float y, unsigned long long currTime);

        float getVelocityX(int units);

        float getVelocityY(int units);

    };

    class VelocityTracker{
    public:
        using Map = std::unordered_map<int,_VelocityTracker>;
        using SpVelocityTracker = std::shared_ptr<VelocityTracker>;

        static VelocityTracker* obtain();
        void recycle();

        void add(int pointer_id, float x, float y, unsigned long long currTimeMills){
            auto it = m_map.find(pointer_id);
            if(it != m_map.end()){
                it->second.add(x, y, currTimeMills);
            }else{
                _VelocityTracker impl;
                impl.add(x, y, currTimeMills);
                m_map[pointer_id] = std::move(impl);
            }
        }

        void clear(){
            m_map.clear();
        }

        void reset(){
            auto it = m_map.begin();
            while(it != m_map.end()){
                it->second.reset();
                ++it;
            }
        }

        float getXVelocity(int pointer_id){
            auto it = m_map.find(pointer_id);
            if(it != m_map.end()){
                return it->second.velocityX;
            }
            return 0;
        }
        float getYVelocity(int pointer_id){
            auto it = m_map.find(pointer_id);
            if(it != m_map.end()){
                return it->second.velocityY;
            }
            return 0;
        }
        void computeCurrentVelocity(int units, float maxVelocity){
            auto it = m_map.begin();
            while(it != m_map.end()){
                it->second.computeCurrentVelocity(units, maxVelocity);
                ++it;
            }
        }
        void computeCurrentVelocity(int units){
            computeCurrentVelocity(units, FLT_MAX);
        }

    private:
        Map m_map;
    };

}
