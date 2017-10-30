#include "StepToCinema.h"

using namespace melange;

// overload this function and fill in your own unique data
void GetWriterInfo(Int32 &id, String &appname)
{
    // unique plugin id from plugincafe
    id = 1040064;
    appname = "StepToCinema";
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
    const char *fn = "/Projects/Misc/StepToCinema/resources/battery.stp";
    const char *fnback = "/Projects/Misc/StepToCinema/resources/battery.c4d";
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
    StepConverter::ConvertStepFile(fn, fnback);

    // to keep the console window open wait here for input
    char value;
    printf("\n <press return to exit>");
    scanf("%c", &value);
}

//////////////////////////////////////////////////////////////////////////
