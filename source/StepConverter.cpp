#include <iostream>
#include <fstream>
#include <string>
#include "StepConverter.h"

Bool StepConverter::ConvertStepFile(const char *filename, const char *output)
{
    if (!filename)
    {
        return false;
    }

    printf(" - File: \"%s\"", filename);

    if (!GeFExist(filename))
    {
        printf("\n file is not existing");
        printf("\n aborting...");
        return false;
    }

    BaseFile* file = BaseFile::Alloc();

    Bool openfile = file->Open(filename, FILEOPEN_READ, FILEDIALOG_ANY, BYTEORDER_MOTOROLA, MACTYPE_CINEMA, MACCREATOR_CINEMA);
    if (openfile)
    {
        printf("\n File opened successfully!");
    }
    else
    {
        printf("\n File could not be opened");
        printf("\n aborting...");
        return false;
    }

    Int64 filelength = file->GetLength();
    if (filelength > 0)
    {
        printf("\n File is ");
        printf("%i", (int)filelength);
        printf(" bytes");
    }
    else
    {
        printf("\n File has zero length");
        printf("\n aborting...");
        return false;
    }

    printf("\n");
    Char* txt = new Char;
    while (file->ReadChar(txt))
    {
        std::cout << txt;
    }

    BaseDocument* c4dDoc = BaseDocument::Alloc();

    SaveDocument(c4dDoc, output, SAVEDOCUMENTFLAGS_0);

    return true;
}