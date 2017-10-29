#include <iostream>
#include <fstream>
#include <stdio.h>
#include "StepToCinema.h"
#include "loadAndSaveC4D.h"

using namespace melange;

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

Bool ConvertStepFile(const char *file, const char *output)
{
    if (!file)
    {
        return false;
    }

    printf(" - File: \"%s\"", file);

    if (!GeFExist(file))
    {
        printf("\n   file is not existing\n   aborting...");
        return false;
    }

    LocalFileTime mt;
    if (GeGetFileTime(Filename(file), GE_FILETIME_CREATED, &mt))
    {
        printf("\n   + created    : %02d/%02d/%d", mt.month, mt.day, mt.year);
    }
    if (GeGetFileTime(Filename(file), GE_FILETIME_MODIFIED, &mt))
    {
        printf("\n   + modified   : %02d/%02d/%d", mt.month, mt.day, mt.year);
    }

    BaseDocument* c4dDoc = BaseDocument::Alloc();

    // Open file, read contents, and insert stuff into the cinema scene here.

    SaveDocument(c4dDoc, output, SAVEDOCUMENTFLAGS_0);

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
    char *fn = "D:/Projects/StepToCinema/resources/battery.stp";
    char *fnback = "D:/Projects/StepToCinema/resources/battery.c4d";

    // use string passed as argument if available
    if (argc > 1 && strlen(argv[1]) > 8)
    {
        fn = argv[1];
    }

#endif

    // load and save the file
    ConvertStepFile(fn, fnback);

    // to keep the console window open wait here for input
    char value;
    printf("\n <press return to exit>");
    scanf("%c", &value);
}

//////////////////////////////////////////////////////////////////////////
