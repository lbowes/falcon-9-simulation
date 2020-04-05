#include "VertFlipScreenQuad.h"


VertFlipScreenQuad::VertFlipScreenQuad(irr::video::IVideoDriver& vidDriver, irr::core::dimension2du monitorResolution) :
    mRenderTarget(nullptr),
    mVidDriverHandle(vidDriver) {

    using namespace irr::video;
    using namespace irr::core;

    mRenderTarget = vidDriver.addRenderTargetTexture(monitorResolution, "VertFlippedScreenQuad");

    const SColor white = SColor(255, 255, 255, 255);
    mVertices[0] = S3DVertex(-1, -1, 0, 0, 0, 0, white, 0, 1);
    mVertices[1] = S3DVertex(-1, 1, 0, 0, 0, 0, white, 0, 0);
    mVertices[2] = S3DVertex(1, 1, 0, 0, 0, 0, white, 1, 0);
    mVertices[3] = S3DVertex(1, -1, 0, 0, 0, 0, white, 1, 1);

    mIndices[0] = 0;
    mIndices[1] = 1;
    mIndices[2] = 2;
    mIndices[3] = 2;
    mIndices[4] = 3;
    mIndices[5] = 0;

    matrix4 invertY;
    invertY.buildTextureTransform(0.0f, vector2df(0, 0), vector2df(0, 1.0f), vector2df(1.0f, -1.0f));

    mMaterial.setTexture(0, mRenderTarget);
    mMaterial.setTextureMatrix(0, invertY);
    mMaterial.Wireframe = false;
    mMaterial.Lighting = false;
    mMaterial.ZWriteEnable = false;
    mMaterial.MaterialType = EMT_SOLID;
    mMaterial.TextureLayer[0].BilinearFilter = false;
}


void VertFlipScreenQuad::drawAll() {
    using namespace irr::video;
    using namespace irr::core;

    mVidDriverHandle.setMaterial(mMaterial);
    mVidDriverHandle.setTransform(ETS_PROJECTION, IdentityMatrix);
    mVidDriverHandle.setTransform(ETS_VIEW, IdentityMatrix);
    mVidDriverHandle.setTransform(ETS_WORLD, IdentityMatrix);

    mVidDriverHandle.drawIndexedTriangleList(&mVertices[0], 4, &mIndices[0], 2);
}


irr::video::ITexture& VertFlipScreenQuad::getRenderTarget() const {
    return *mRenderTarget;
}
