#include "goomy/EngineBase.h"

namespace goomy::internal {

EngineBase::EngineBase()
    : running(false), startTime(clock::now()), frameTime(startTime),
      previousFrameTime(frameTime),
      frameDuration(frameTime - previousFrameTime), age(frameTime - startTime) {
}

bool EngineBase::isRunning() const {
    return running;
}

void EngineBase::start() {
    running = true;
}

void EngineBase::shutdown() {
    running = false;
}

void EngineBase::frameTick() {
    previousFrameTime = frameTime;
    frameTime = clock::now();
    frameDuration = frameTime - previousFrameTime;
    age = frameTime - startTime;
}

const clock::time_point &EngineBase::getStartTime() const {
    return startTime;
}

const clock::time_point &EngineBase::getFrameTime() const {
    return frameTime;
}

const clock::time_point &EngineBase::getPreviousFrameTime() const {
    return previousFrameTime;
}

const clock::duration &EngineBase::getFrameDuration() const {
    return frameDuration;
}

const clock::duration &EngineBase::getAge() const {
    return age;
}

}