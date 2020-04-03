#ifndef VERTFLIPSCREENQUAD_H_
#define VERTFLIPSCREENQUAD_H_

#include <ITexture.h>
#include <IVideoDriver.h>
#include <S3DVertex.h>
#include <SMaterial.h>


// "Vertically-flipped screen quad"
// This class serves as an intermediate layer between the scene and the final output rendered to the screen. By default,
// Irrlicht flips the Y-coordinates of textures that are rendered to, which needs to be undone before rendering the
// result to the screen.

class VertFlipScreenQuad {
private:
    irr::video::S3DVertex mVertices[4];
    unsigned short mIndices[6];
    irr::video::ITexture* mRenderTarget;
    irr::video::SMaterial mMaterial;
    irr::video::IVideoDriver& mVidDriverHandle;

public:
    VertFlipScreenQuad(irr::video::IVideoDriver& vidDriver, irr::core::dimension2du monitorResolution);
    ~VertFlipScreenQuad() = default;

    void drawAll();
    irr::video::ITexture& getRenderTarget() const;
};

#endif

