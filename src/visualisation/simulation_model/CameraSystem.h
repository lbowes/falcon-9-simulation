#ifndef CAMERASYSTEM_H_
#define CAMERASYSTEM_H_

class CameraSystem {
private:
    float mAspectRatio;

public:
    CameraSystem();
    ~CameraSystem() = default;

    void handleInput();
    void updateAspectRatio(float aspectRatio);
    void update();
};

#endif // CAMERASYSTEM_H_

