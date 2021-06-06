#pragma once

// #define TGFEXT       // TGF2, MMF2, MMF2 Dev
   #define MMFEXT       // MMF2, MMF2 Dev
// #define PROEXT       // MMF2 Dev only

#include    "Edif.h"
#include	"Resource.h"

// edPtr : Used at edittime and saved in the MFA/CCN/EXE files

#define MAX_IMAGES 256

struct EDITDATA
{
	// Header - required
	extHeader		eHeader;

	// Object's data
	short			swidth;
	short			sheight;

	WORD			images[MAX_IMAGES];
	short			imageCount;
};

#include "Extension.h"
