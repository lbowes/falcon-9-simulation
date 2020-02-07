#ifndef HIGHPRECISIONSCENENODE_H_
#define HIGHPRECISIONSCENENODE_H_

#include <ISceneNode.h>
#include <chrono/core/ChCoordsys.h>


// This struct is used to solve the problem of graphical floating point errors at large distances from the origin.
// Instead of applying a translation to Irrlicht's internal camera node, we instead offset all other ISceneNodes by the
// camera position, so that floating point precision errors only occur at large distances (where they are not visible).

struct HighPrecisionSceneNode {
    irr::scene::ISceneNode* internalNode;
    chrono::ChCoordsys<> transform_world;
};

#endif // HIGHPRECISIONSCENENODE_H_

