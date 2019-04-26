#pragma once

#include <chrono>

namespace goomy {

using clock = std::chrono::steady_clock;

}

namespace goomy::internal {

class EngineBase {
  public:
    EngineBase();

    bool isRunning() const;
    void start();
    void shutdown();

    void frameTick();
    const clock::time_point &getStartTime() const;
    const clock::time_point &getFrameTime() const;
    const clock::time_point &getPreviousFrameTime() const;
    const clock::duration &getFrameDuration() const;
    const clock::duration &getAge() const;
    float deltaTime() const;

  private:
    bool running;

    clock::time_point startTime;
    clock::time_point frameTime;
    clock::time_point previousFrameTime;
    clock::duration frameDuration;
    clock::duration age;
};

}