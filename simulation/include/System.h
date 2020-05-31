#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <chrono/physics/ChSystemNSC.h>


class System {
private:
    chrono::ChSystemNSC mSystem;
    std::shared_ptr<chrono::ChBodyAuxRef> mCube;

public:
    System();
    ~System() = default;

    void advanceTimeBy(double dt);
    void serialiseState();

private:
    void setupInitialState();
};


#endif // SYSTEM_H_

