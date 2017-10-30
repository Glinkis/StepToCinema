#ifndef MELANGE_ALIEN_DEF_H__
#define MELANGE_ALIEN_DEF_H__

#include "c4d.h"
#include "applicationIdDefinitions.h"

//////////////////////////////////////////////////////////////////////////
namespace melange
{
    //definition of self defined element classes  (spline,null,camera,light etc.) derived from C4D classes

    class AlienBaseDocument : public BaseDocument
    {
    public:
        virtual Bool Execute();
    };

    //object list/root (list which contains all scene objects)
    class AlienRootObject : public RootObject
    {
    public:
        virtual BaseObject *AllocObject(Int32 id);
    };

    //layer list/root (list which contains all scene layers)
    class AlienRootLayer : public RootLayer
    {
        INSTANCEOF(AlienRootLayer, RootLayer)
    public:
        virtual LayerObject *AllocObject();
    };

    //material list/root (list which contains all scene materials)
    class AlienRootMaterial : public RootMaterial
    {
        INSTANCEOF(AlienRootMaterial, RootMaterial);
    public:
        virtual BaseMaterial *AllocObject(Int32 Mtype);
    };

    class AlienRootRenderData : public RootRenderData
    {
        INSTANCEOF(AlienRootRenderData, RootRenderData)
    public:
        virtual RenderData *AllocObject();
    };

    //self defined layer with own functions and members
    class AlienLayer : public LayerObject
    {
        INSTANCEOF(AlienLayer, LayerObject)
    public:
        Int32 layId;
        AlienLayer() : LayerObject()
        {
        }
        virtual Bool Execute();
    };
}

#endif // MELANGE_ALIEN_DEF_H__
