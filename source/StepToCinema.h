/**
 * StepToCinema is a file converter
 * that converts files from the STEP
 * (https://en.wikipedia.org/wiki/ISO_10303-21)
 * format into Cinema4D files.
 */

#ifndef STEP_TO_CINEMA_H__
#define STEP_TO_CINEMA_H__

// the file "melangeAlienDef.h" contains the "alien" classes you can define by yourself
// in these classes you can store your own additional data and
// you have to define the Execute() functions (defined in StepToCinema.cpp)
// which will be called by CreateSceneFromC4D() after successful loading of the scene
// this makes it easy for you to transform a whole c4d scene to your own 3d object types
#include "melange/melangeAlienDef.h"
#include "definitions\applicationIdDefinitions.h"

// include needed standard and cinema definitions
#include <stdio.h>
#include "c4d.h"

#endif // STEP_TO_CINEMA_H__
