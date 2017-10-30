#include "melangeAlienDef.h"

using namespace melange;

Bool LoadSaveC4DScene(const char *fn, const char *fnback)
{
    return false;
}

// create objects, material and layers for the new C4D scene file
Bool BaseDocument::CreateSceneToC4D(Bool selectedonly)
{
    return false;
}

//////////////////////////////////////////////////////////////////////////
// ALIEN ALLOCATION FUNCTIONS
//////////////////////////////////////////////////////////////////////////

// allocate a object/element for the root/list (this is fileformat related, to support/load all objects you should not change this function)
BaseObject *AlienRootObject::AllocObject(Int32 id)
{
    BaseObject *baseobject = nullptr;
    return baseobject;
}

// allocate a layer for the root/list
LayerObject *AlienRootLayer::AllocObject()
{
    return nullptr;
}

// allocate a material for the root/list
BaseMaterial *AlienRootMaterial::AllocObject(Int32 Mtype)
{
    return nullptr;
}

// allocate a renderdata for the root/list
RenderData *AlienRootRenderData::AllocObject()
{
    return nullptr;
}

// allocate the list/root elements itself
RootMaterial *AllocAlienRootMaterial()
{
    return NewObj(AlienRootMaterial);
}
RootObject *AllocAlienRootObject()
{
    return NewObj(AlienRootObject);
}
RootLayer *AllocAlienRootLayer()
{
    return NewObj(AlienRootLayer);
}
RootRenderData *AllocAlienRootRenderData()
{
    return NewObj(AlienRootRenderData);
}
RootViewPanel *AllocC4DRootViewPanel()
{
    return NewObj(RootViewPanel);
}
LayerObject *AllocAlienLayer()
{
    return NewObj(AlienLayer);
}

// allocate the scene objects (primitives, camera, light, etc.)
// here you can remove all the objects you don't want to support
NodeData *AllocAlienObjectData(Int32 id, Bool &known)
{
    return nullptr;
}

// allocate the plugin tag elements data (only few tags have additional data which is stored in a NodeData)
NodeData *AllocAlienTagData(Int32 id, Bool &known)
{
    return nullptr;
}

/// Allocates the shader types.
NodeData* AllocAlienShaderData(melange::Int32 id, melange::Bool& known)
{
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// IMPORT Example
// the following are the definitions of the "Alien" EXECUTE functions
//////////////////////////////////////////////////////////////////////////

// this Execute function will be called first after calling CreateSceneFromC4D()
// e.g. this can be used to handle document informations, units, renderdata or fps as shown below

Bool AlienBaseDocument::Execute()
{
    return true;
}

// Execute function for the self defined Deformer objects
Bool AlienDeformerObjectData::Execute()
{
    return false;
}

// Execute function for the self defined FFD object
Bool AlienFFDObjectData::Execute()
{
    return false;
}

// Execute function for the self defined Null object
Bool AlienNullObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Foreground object
Bool AlienForegroundObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Background object
Bool AlienBackgroundObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Floor object
Bool AlienFloorObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Sky object
Bool AlienSkyObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Sky shader object
Bool AlienSkyShaderObjectData::Execute()
{
    return true;
}

// Execute function for the self defined CAJoint object
Bool AlienCAJointObjectData::Execute()
{
    return true;
}

// Execute function for the self defined CASkin object
Bool AlienCASkinObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Environment object
Bool AlienEnvironmentObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Environment object
Bool AlienBoolObjectData::Execute()
{
    return false;
}

// Execute function for the self defined Environment object
Bool AlienExtrudeObjectData::Execute()
{
    return false;
}

// Execute function for the self defined instance object
Bool AlienInstanceObjectData::Execute()
{
    return true;
}

// Execute function for the self defined XRef object
Bool AlienXRefObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Polygon object
Bool AlienPolygonObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Layer
Bool AlienLayer::Execute()
{
    return true;
}

// Execute function for the self defined Camera object
Bool AlienCameraObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Spline object
Bool AlienSplineObject::Execute()
{
    return true;
}

// Execute function for the self defined Primitive objects
Bool AlienPrimitiveObjectData::Execute()
{
    return true;
}

// Execute function for the self defined light objects
Bool AlienLightObjectData::Execute()
{
    return true;
}

// Execute function for the self defined Material
Bool AlienMaterial::Execute()
{
    return true;
}

// Execute function for the self defined plugin material
Bool AlienBaseMaterial::Execute()
{
    return true;
}
