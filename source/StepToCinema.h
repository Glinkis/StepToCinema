/////////////////////////////////////////////////////////////
// MELANGE SDK - command line example                      //
/////////////////////////////////////////////////////////////
// (c) MAXON Computer GmbH, all rights reserved            //
/////////////////////////////////////////////////////////////

#ifndef STEPTOCINEMA_H__
#define STEPTOCINEMA_H__

// the file "alien_def.h" contains the "alien" classes you can define by yourself
// in these classes you can store your own additional data and
// you have to define the Execute() functions (defined in StepToCinema.cpp)
// which will be called by CreateSceneFromC4D() after successful loading of the scene
// this makes it easy for you to transform a whole c4d scene to your own 3d object types
#include "alien_def.h"

// include needed standard and cinema definitions
#include <stdio.h>
#include "c4d.h"

#endif // STEPTOCINEMA_H__
