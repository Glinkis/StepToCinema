#include "StepToCinema.h"

using namespace melange;

// global temporary variables for this export example of c4d scene file
BaseDocument* myInternalDoc = nullptr;	// pointer to the imported C4D document use to demonstrate the export, this should be your own type of document

// overload this function and fill in your own unique data
void GetWriterInfo(Int32 &id, String &appname)
{
    // register your own pluginid once for your exporter and enter it here under id
    // this id must be used for your own unique ids
    // 	Bool AddUniqueID(Int32 appid, const Char *const mem, Int32 bytes);
    // 	Bool FindUniqueID(Int32 appid, const Char *&mem, Int32 &bytes) const;
    // 	Bool GetUniqueIDIndex(Int32 idx, Int32 &id, const Char *&mem, Int32 &bytes) const;
    // 	Int32 GetUniqueIDCount() const;
    id = 1040064;
    appname = "StepToCinema";
}

// example function to get the load progress
inline void myLoadProgressFunction(Int32 status, void *udata)
{
    Int32 test = status; // read percentage of the file (0 - 100)
    printf("%d", (int)test);
    printf("%%");
    printf(".");
}

// example function to get the save progress
inline void mySaveProgressFunction(Int32 status, void *udata)
{
    Int32 test = status; // actual written file size in bytes
    printf("%d", (int)test);
    printf(".");
}

// loads the scene with name <fn> and if cache exist saves it as <fnback>
static Bool LoadSaveC4DScene(const char *fn, const char *fnback)
{
    if (!fn)
    {
        return false;
    }

    // alloc C4D document and file
    AlienBaseDocument *C4Ddoc = NewObj(AlienBaseDocument);
    HyperFile *C4Dfile = NewObj(HyperFile);

    if (!C4Ddoc || !C4Dfile)
    {
        DeleteObj(C4Ddoc);
        DeleteObj(C4Dfile);
        return false;
    }

    // set a callback function for load progress
    //C4Dfile->SetLoadStatusCallback(myLoadProgressFunction,nullptr);

    printf("\n # Dokument #\n");
    printf(" - File: \"%s\"", fn);

    // check if the file exists
    if (!GeFExist(fn))
    {
        printf("\n   file is not existing\n   aborting...");

        DeleteObj(C4Ddoc);
        DeleteObj(C4Dfile);

        return false;
    }

    LocalFileTime mt;
    if (GeGetFileTime(Filename(fn), GE_FILETIME_CREATED, &mt))
        printf("\n   + created    : %02d/%02d/%d", mt.month, mt.day, mt.year);
    if (GeGetFileTime(Filename(fn), GE_FILETIME_MODIFIED, &mt))
        printf("\n   + modified   : %02d/%02d/%d", mt.month, mt.day, mt.year);

    // do we know the file type ?
    if (GeIdentifyFile(fn) != IDENTIFYFILE_SCENE)
    {
        printf("\n   + identified : unknown file type\n   aborting...");

        DeleteObj(C4Ddoc);
        DeleteObj(C4Dfile);

        return false;
    }
    else
    {
        printf("\n   + identified : Cinema4D Project File\n");
    }

    // open the file for read
    if (C4Dfile->Open(DOC_IDENT, fn, FILEOPEN_READ))
    {
        // read all chunks
        if (C4Ddoc->ReadObject(C4Dfile, true))
        {
            printf("\n   Read scene: SUCCEEDED (%d/%d bytes - fileversion: %d)\n", (int)C4Dfile->GetPosition(), (int)C4Dfile->GetLength(), (int)C4Dfile->GetFileVersion());
        }
        else
        {
            printf("\n   Read scene: FAILED (Error:%d) (%d/%d bytes - fileversion: %d)\n", (int)C4Dfile->GetError(), (int)C4Dfile->GetPosition(), (int)C4Dfile->GetLength(), (int)C4Dfile->GetFileVersion());
        }
    }
    else
    {
        // failed to open
        if (C4Dfile->GetError() == FILEERROR_WRONG_VALUE)
        {
            printf("\n   Read scene: FAILED (Error:%d)- unknown filetype or version:\n               \"%s\"\n", (int)C4Dfile->GetError(), fn);
        }
        else
        {
            printf("\n   Read scene: FAILED (Error:%d)- Could not open file \"%s\"\n", (int)C4Dfile->GetError(), fn);
        }

        DeleteObj(C4Ddoc);
        DeleteObj(C4Dfile);

        return false;
    }

    // close the file
    C4Dfile->Close();

    // free the file
    DeleteObj(C4Dfile);

    // checks for existing polygon caches (the option you can find in cinema Preferences->File->Option "Save Poylgons for Melange")
    if (C4Ddoc->HasCaches())
    {
        printf("   Has caches: true\n");
    }
    else
    {
        printf("   Has caches: false\n");
    }

    // calls all Execute() functions of the document (and prints scene information which is for demonstration purposes only)
    C4Ddoc->CreateSceneFromC4D();

    // if we have a fnback name save the file
    if (fnback)
    {
        // if we have a preview image we can access and save it
        if (C4Ddoc->GetDocPreviewBitmap().GetBw() > 0)
        {
            Filename fnimage(fnback);
            fnimage.SetSuffix("jpg");
            C4Ddoc->GetDocPreviewBitmap().Save(fnimage, FILTER_JPG, nullptr, SAVEBIT_0);
        }

        // create new file for export
        HyperFile *newC4Dfile = NewObj(HyperFile);
        if (!newC4Dfile)
        {
            DeleteObj(C4Ddoc);
            return false;
        }

        // open file for write
        if (!newC4Dfile->Open(DOC_IDENT, fnback, FILEOPEN_WRITE))
        {
            DeleteObj(C4Ddoc);
            DeleteObj(newC4Dfile);
            return false;
        }

        // set a callback function for save progress
        //newC4Dfile->SetSaveStatusCallback(mySaveProgressFunction,nullptr);

        // create new C4D base document (for export we don't need a alien document)
        BaseDocument *newC4DDoc = BaseDocument::Alloc();
        if (!newC4DDoc)
        {
            DeleteObj(C4Ddoc);
            newC4Dfile->Close();
            DeleteObj(newC4Dfile);
            return false;
        }

        // to copy a whole C4D document you can use this CopyTo() function (which is also available for every object)
        //C4Ddoc->CopyTo( (PrivateChunk*)newC4DDoc, COPYFLAGS_0, nullptr );

        // use the just loaded C4D document as data source
        myInternalDoc = C4Ddoc;

        // fill the document (copy data from loaded objects to new objects)
        newC4DDoc->CreateSceneToC4D(true);

        // write the document
        newC4DDoc->Write(newC4Dfile);

        // close the C4D file
        newC4Dfile->Close();

        // clear global document pointer
        myInternalDoc = nullptr;

        // free the new created and exported C4D document
        BaseDocument::Free(newC4DDoc);

        // free the new C4D file
        DeleteObj(newC4Dfile);
    }

    // free the loaded C4D alien document
    DeleteObj(C4Ddoc);

    return true;
}

//////////////////////////////////////////////////////////////////////////
// EXPORT example
// the following functions are regarding EXPORT of a C4D scene file
//////////////////////////////////////////////////////////////////////////

// creates a new C4D Material and set some parameters
static Bool BuildMaterialToC4D(AlienMaterial* pOrgMat, BaseDocument* doc)
{
    if (!doc)
    {
        return false;
    }

    AlienMaterial *newMaterial = nullptr;

    // Create new material
    newMaterial = NewObj(AlienMaterial);
    if (!newMaterial)
    {
        return false;
    }
    // Set some Material properties
    String str = pOrgMat->GetName();
    newMaterial->SetName(str);

    // we only support data of type Mmaterial here
    if (pOrgMat->GetType() == Mmaterial)
    {
        // copy color and set the color values
        GeData data;
        if (pOrgMat->GetParameter(MATERIAL_COLOR_COLOR, data))
        {
            newMaterial->SetParameter(MATERIAL_COLOR_COLOR, data.GetVector());
        }
        newMaterial->SetChannelState(CHANNEL_COLOR, pOrgMat->GetChannelState(CHANNEL_COLOR));

        // Get original shader
        BaseShader* orgShader = pOrgMat->GetShader(MATERIAL_COLOR_SHADER);
        if (orgShader)
        {
            // create new shader on new material
            newMaterial->MakeBitmapShader(MATERIAL_COLOR_SHADER, orgShader->GetFileName());
        }

        // copy id -> needed for reassignmet of materials in the new scene
        newMaterial->matId = pOrgMat->matId;
    }

    // add material to document
    doc->InsertMaterial(newMaterial);

    return true;
}

// go through all original Layers and create new Layers and copy the color
static Bool BuildLayerToC4D(AlienLayer* myInternalLayer, AlienLayer* parentLayer, BaseDocument* doc)
{
    AlienLayer *newLayer = nullptr;

    // go through all layer
    while (myInternalLayer)
    {
        // create new layer
        newLayer = NewObj(AlienLayer);
        if (!newLayer)
        {
            return false;
        }

        // set layer color property
        newLayer->SetName(myInternalLayer->GetName());
        GeData data;
        if (myInternalLayer->GetParameter(ID_LAYER_COLOR, data))
        {
            newLayer->SetParameter(ID_LAYER_COLOR, data);
        }

        // copy id -> needed for reassignment of layers in the new scene
        newLayer->layId = myInternalLayer->layId;

        // add layer to C4D export scene
        doc->InsertLayer(newLayer, parentLayer);

        // also add children of the layer
        BuildLayerToC4D((AlienLayer*)myInternalLayer->GetDown(), newLayer, doc);

        // go to next layer
        myInternalLayer = (AlienLayer*)myInternalLayer->GetNext();
    }

    return true;
}

// go through all original Objects and create new Objects (only a few type are handled)
static BaseObject* CreateObjectToC4D(BaseObject* myInternalObject, BaseObject* pParent, BaseDocument* doc)
{
    if (!myInternalObject || !doc)
    {
        return nullptr;
    }

    BaseObject *newObject = nullptr;
    Int32 i;

    // copy spline object
    if (myInternalObject->GetType() == Ospline)
    {
        // create new spline object
        newObject = BaseObject::Alloc(Ospline);
        if (!newObject)
        {
            return nullptr;
        }

        SplineObject* pOrgSpline = (SplineObject*)myInternalObject;
        SplineObject* pNewSpline = (SplineObject*)newObject;

        // set some spline parameter
        GeData data;
        if (pOrgSpline->GetParameter(SPLINEOBJECT_TYPE, data))
        {
            pNewSpline->SetParameter(SPLINEOBJECT_TYPE, data.GetInt32());
        }
        if (pOrgSpline->GetParameter(SPLINEOBJECT_CLOSED, data))
        {
            pNewSpline->SetParameter(SPLINEOBJECT_CLOSED, data.GetInt32());
        }

        // copy the spline points from the original spline to the copy
        pNewSpline->ResizeObject(pOrgSpline->GetPointCount(), pOrgSpline->GetSegmentCount());

        // get array pointer
        Vector *vadrnew = pNewSpline->GetPointW();
        const Vector *vadr = pOrgSpline->GetPointR();
        if (!vadrnew || !vadr)
        {
            return nullptr;
        }

        // fill point array
        for (i = 0; i < pOrgSpline->GetPointCount(); i++)
        {
            vadrnew[i] = vadr[i];
        }
    }
    // create polygon object and set points & polygons
    else if (myInternalObject->GetType() == Opolygon)
    {
        // create new polygon object
        newObject = BaseObject::Alloc(Opolygon);
        if (!newObject)
        {
            return nullptr;
        }

        // copy the polygons and points from the original polygon object to the copy
        ((PolygonObject*)newObject)->ResizeObject(((PolygonObject*)myInternalObject)->GetPointCount(), ((PolygonObject*)myInternalObject)->GetPolygonCount(), 0);

        // get point addresses
        Vector *vadrnew = ((PolygonObject*)newObject)->GetPointW();
        const Vector *vadr = ((PolygonObject*)myInternalObject)->GetPointR();
        if (!vadrnew || !vadr)
        {
            return nullptr;
        }

        // get polygon addresses
        CPolygon *padrnew = ((PolygonObject*)newObject)->GetPolygonW();
        const CPolygon *padr = ((PolygonObject*)myInternalObject)->GetPolygonR();
        if (!padrnew || !padr)
        {
            return nullptr;
        }

        // fill point array
        for (i = 0; i < ((PolygonObject*)myInternalObject)->GetPointCount(); i++)
        {
            vadrnew[i] = vadr[i];
        }

        // fill polygon array
        for (i = 0; i < ((PolygonObject*)myInternalObject)->GetPolygonCount(); i++)
        {
            padrnew[i] = padr[i];
        }

        // add texture tag and material if the internal element had one
        if (myInternalObject->GetTag(Ttexture))
        {
            TextureTag *myInternalTag = (TextureTag*)myInternalObject->GetTag(Ttexture);
            TextureTag *tNewTag = (TextureTag*)newObject->GetTag(Ttexture);

            if (!tNewTag)
            {
                tNewTag = NewObj(TextureTag);
                if (tNewTag)
                {
                    newObject->InsertTag(tNewTag);
                }
            }

            if (!tNewTag)
            {
                return nullptr;
            }

            // add material
            GeData data;
            AlienMaterial *mat = nullptr;
            if (myInternalTag->GetParameter(TEXTURETAG_MATERIAL, data))
                mat = (AlienMaterial*)data.GetLink();
            if (mat)
            {
                // get mat id (stored in the data of the object)
                PolygonObjectData* pData = (PolygonObjectData*)myInternalObject->GetNodeData();
                Int32 matID = ((AlienPolygonObjectData*)pData)->matid;

                // link the material with the matid and id of the material
                AlienMaterial* pMat = (AlienMaterial*)doc->GetFirstMaterial();
                for (; pMat; pMat = (AlienMaterial*)pMat->GetNext())
                {
                    if (pMat->matId == matID)
                    {
                        break;
                    }
                }
                if (pMat)
                {
                    BaseLink link;
                    link.SetLink(pMat);
                    tNewTag->SetParameter(TEXTURETAG_MATERIAL, link);
                }
            }
        }

        // add layer if the internal element had one
        if (myInternalObject->GetLayerObject())
        {
            // get mat id (stored in the data of the object)
            PolygonObjectData* pData = (PolygonObjectData*)myInternalObject->GetNodeData();
            Int32 layID = ((AlienPolygonObjectData*)pData)->layid;

            // link the layer with id of material and id of layer
            AlienLayer* pLay = (AlienLayer*)doc->GetFirstLayer();
            for (; pLay; pLay = (AlienLayer*)pLay->GetNext())
            {
                if (pLay->layId == layID)
                {
                    break;
                }
            }
            if (pLay)
            {
                newObject->SetLayerObject(pLay);
            }
        }
    }
    // create light object and set color
    else if (myInternalObject->GetType() == Olight)
    {
        // create new light object
        newObject = BaseObject::Alloc(Olight);
        if (!newObject)
        {
            return nullptr;
        }

        GeData data;
        if (myInternalObject->GetParameter(LIGHT_COLOR, data))
        {
            newObject->SetParameter(LIGHT_COLOR, data.GetVector());
        }
    }
    // create camera object and set projection
    else if (myInternalObject->GetType() == Ocamera)
    {
        // create new camera object
        newObject = BaseObject::Alloc(Ocamera);
        if (!newObject)
        {
            return nullptr;
        }

        GeData data;
        if (myInternalObject->GetParameter(CAMERA_PROJECTION, data))
        {
            newObject->SetParameter(CAMERA_PROJECTION, data);
        }
    }
    // create floor object
    else if (myInternalObject->GetType() == Ofloor)
    {
        // create new floor object
        newObject = BaseObject::Alloc(Ofloor);
        if (!newObject)
        {
            return nullptr;
        }
    }
    // create foreground object
    else if (myInternalObject->GetType() == Oforeground)
    {
        // create new object
        newObject = BaseObject::Alloc(Oforeground);
        if (!newObject)
        {
            return nullptr;
        }
    }
    // create background object
    else if (myInternalObject->GetType() == Obackground)
    {
        // create new object
        newObject = BaseObject::Alloc(Obackground);
        if (!newObject)
        {
            return nullptr;
        }
    }
    // create sky object
    else if (myInternalObject->GetType() == Osky)
    {
        // create new object
        newObject = BaseObject::Alloc(Osky);
        if (!newObject)
        {
            return nullptr;
        }
    }
    // create environment object
    else if (myInternalObject->GetType() == Oenvironment)
    {
        // create new object
        newObject = BaseObject::Alloc(Oenvironment);
        if (!newObject)
        {
            return nullptr;
        }
    }
    // create Null object
    else if (myInternalObject->GetType() == Onull)
    {
        // create new null object
        newObject = BaseObject::Alloc(Onull);
        if (!newObject)
        {
            return nullptr;
        }
    }

    // we don't know the type
    if (!newObject)
    {
        return nullptr;
    }

    // add the new element to the scene (under the parent element)
    doc->InsertObject(newObject, pParent);

    // set the name
    newObject->SetName(myInternalObject->GetName());

    // set the object matrices
    newObject->SetMl(myInternalObject->GetMl());
    newObject->SetMg(myInternalObject->GetMg());

    return newObject;
}

// go through all elements and build the objects for export
static BaseObject* BuildObjectsToC4D(BaseObject* obj, BaseObject* pNewParent, BaseDocument* doc)
{
    BaseObject* pNewObj = nullptr;

    while (obj)
    {
        // create object
        pNewObj = CreateObjectToC4D(obj, pNewParent, doc);

        // children
        BuildObjectsToC4D((BaseObject*)obj->GetDown(), pNewObj, doc);

        // next object
        obj = (BaseObject*)obj->GetNext();
    }

    return pNewParent;
}

// create objects, material and layers for the new C4D scene file
Bool BaseDocument::CreateSceneToC4D(Bool selectedonly)
{
    // we need the pointer to the loaded C4D scene file as we don't have our own internal data for export
    if (!myInternalDoc)
    {
        return false;
    }

    // create MATERIALS
    AlienMaterial* pMat = (AlienMaterial*)myInternalDoc->GetFirstMaterial();
    for (pMat = (AlienMaterial*)myInternalDoc->GetFirstMaterial(); pMat; pMat = (AlienMaterial*)pMat->GetNext())
    {
        if (!BuildMaterialToC4D(pMat, this))
        {
            return false;
        }
    }
    // create LAYERS
    if (!BuildLayerToC4D((AlienLayer*)myInternalDoc->GetFirstLayer(), nullptr, this))
    {
        return false;
    }

    // create OBJECTS
    if (!BuildObjectsToC4D(myInternalDoc->GetFirstObject(), nullptr, this))
    {
        return false;
    }

    printf("\n scene build was successful\n");

    return true;
}


//////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION
//////////////////////////////////////////////////////////////////////////

#ifdef __MAC
#include <uuid/uuid.h>
#endif

int main(int argc, Char* argv[])
{
#if (defined __ppc__) || (defined __ppc64__)
    printf("PPC code:\n");
#else
    printf("INTEL code:\n");
#endif

    // security check for OSX
#ifdef __MAC
    Int32 s1 = sizeof(uuid_t);
    Int32 s2 = sizeof(GeMarker);
    if (s1 != s2)
    {
        CriticalStop();
    }
#endif

    Char* versionStr = GetLibraryVersion().GetCStringCopy();
    printf("\n ___________________________________\n");
    printf("\n   %s Commandline Tool", versionStr);
    printf("\n ___________________________________\n");
    DeleteMem(versionStr);

#ifdef __MAC
    const char *fn = "/test.c4d";
    const char *fnback = "/test_out.c4d";
#else

    // filename for import and export C4D files
    char *fn = "D:\\scene_IN.c4d";
    char *fnback = "D:\\scene_OUT.c4d";

    // use string passed as argument if available
    if (argc > 1 && strlen(argv[1]) > 8)
    {
        fn = argv[1];
    }

#endif

    // load and save a c4d scene file
    Bool res = LoadSaveC4DScene(fn, fnback);
    if (res && fnback)
    {
        // load the just saved c4d file again (testing purposes only)
        printf("\n\n ##### ReRead saved file: \n\n");
        LoadSaveC4DScene(fnback, nullptr);
    }

    // to keep the console window open wait here for input
    char value;
    printf("\n <press return to exit>");
    scanf("%c", &value);
}

//////////////////////////////////////////////////////////////////////////
