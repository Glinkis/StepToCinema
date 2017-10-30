#include "melangeAlienDef.h"

using namespace melange;

// create objects, material and layers for the new C4D scene file
Bool BaseDocument::CreateSceneToC4D(Bool selectedonly)
{
    return false;
}

////////////////////////////////
// ALIEN ALLOCATION FUNCTIONS //
////////////////////////////////

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

// Execute function for the self defined Layer
Bool AlienLayer::Execute()
{
    return true;
}
