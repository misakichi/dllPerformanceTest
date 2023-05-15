#pragma once

#define VEC_NS Dll
#ifdef CLASS_SPEC
#undef CLASS_SPEC
#endif
#if defined(_EXE)
#define CLASS_SPEC	__declspec(dllimport)
#elif defined(_DLL)
#define CLASS_SPEC	__declspec(dllexport)
#else
#define CLASS_SPEC	
#endif

#include "../shared/vecN.h"

template class CLASS_SPEC Dll::Vector<float, 3>;

#undef CLASS_SPEC
#define CLASS_SPEC	

#define VEC_NS Dll_NoExport
#include "../shared/vecN.h"

