#ifndef STEP_CONVERTER_H__
#define STEP_CONVERTER_H__

#include "c4d.h"

using namespace melange;

static class StepConverter
{
public:
    static Bool ConvertStepFile(const char *filename, const char *output);
};

#endif // STEP_CONVERTER_H__
