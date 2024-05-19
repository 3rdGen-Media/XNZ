#ifndef XNG_H
#define XNG_H

#ifdef __cplusplus
extern "C" {
#endif

//Std C Header Includes
//For Win32 it is imperative that stdio gets included
//#include <stdio.h>

#ifdef _WIN32  //IF WIN32

#define NOMINMAX
#define NTDDI_VERSION NTDDI_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7 

// standard definitions
#define STRICT                                                  // enable strict type-checking of Windows handles
#define WIN32_LEAN_AND_MEAN                                     // allow the exclusion of uncommon features
#define WINVER                                          _WIN32_WINNT_WIN7  // allow the use of Windows XP specific features
#define _WIN32_WINNT                                    _WIN32_WINNT_WIN7  // allow the use of Windows XP specific features
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES         1       // use the new secure functions in the CRT
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT   1       // use the new secure functions in the CRT

#include <windows.h>            // fundamental Windows header file

//#define _USE_MATH_DEFINES	//holy crap!!! must define this on ms windows to get M_PI definition!
//#include <math.h>

#define THREADPROC WINAPI   //What does this do again?
#endif

//only define bool if not previoulsy defined by CoreRender crWindow global header
//and not using C++
#ifndef __cplusplus
//typedef unsigned char bool;
#ifndef bool
#define bool  int
#define true  1
#define false 0
#endif
#endif

//only define bool if not previoulsy defined by CoreRender crWindow global header
//and not using C++
#ifndef __cplusplus
//typedef unsigned char bool;
#ifndef bool
#define bool  int
#define true  1
#define false 0
#endif
#endif

/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

/* If we are we on Views, we want a single define for it.
 */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
 #define _WIN32
#endif /* _WIN32 */


#if defined(XNG_IMAGE_DLL) && defined(_XNG_IMAGE_BUILD_DLL)
 /* CRGC_DLL must be defined by applications that are linking against the DLL
  * version of the CRGC library.  _CRGC_BUILD_DLL is defined by the CRGC
  * configuration header when compiling the DLL version of the library.
  */
 #error "You may not have both XNG_IMAGE_DLL and _XNG_IMAGE_BUILD_DLL defined"
#endif

/* XNG_IMAGE_API is used to declare public API functions for export
 * from the DLL / shared library / dynamic library.
 */
#if defined(_WIN32) && defined(_XNG_IMAGE_BUILD_DLL)
 /* We are building CRPrimitives as a Win32 DLL */
 #define XNG_IMAGE_API __declspec(dllexport)
#elif defined(_WIN32) && defined(XNG_IMAGE_DLL)
 /* We are calling CRPrimitives as a Win32 DLL */
 #define XNG_IMAGE_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(_XNG_IMAGE_BUILD_DLL)
 /* We are building XNG_IMAGE as a shared / dynamic library */
 #define XNG_IMAGE_API __attribute__((visibility("default")))
#else
 /* We are building or calling CRPrimitives as a static library */
 #define XNG_IMAGE_API
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_IMAGE_INLINE
#ifdef _WIN32
#define XNG_IMAGE_INLINE __inline
#else
#define XNG_IMAGE_INLINE
#endif
#endif
    
    //inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_IMAGE_DECLSPEC
#ifdef _WIN32
#define XNG_IMAGE_DECLSPEC __declspec
#else
#define XNG_IMAGE_DECLSPEC
#endif
#endif
    
    //align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_IMAGE_ALIGN//(X)
#ifdef _WIN32
#define XNG_IMAGE_ALIGN(X) (align(X))
#else
#define XNG_IMAGE_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif
    
#ifndef XNG_IMAGE_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_IMAGE_PACK_ATTRIBUTE
#pragma pack(push, 1)
    //#define CR_TEXT_PACK_ATTRIBUTE( __Declaration__ ) __Declaration__ __attribute__((packed))
#else
    //#define CR_TEXT_PACK_ATTRIBUTE ((packed, aligned(4)))
#define XNG_IMAGE_PACK_ATTRIBUTE __attribute__ ((packed))
    //#define CR_TEXT_PACK_ATTRIBUTE( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#endif
#endif
    //CONFIGURATION -- THESE ARE THE ONLY MACROS YOU SHOULD EDIT

//typically, this should be defined by the project file, but you can define it here to explicitly enable
//memory alignment sanity checking for release builds
//#define XNG_IMAGE_DEBUG		

//END CONFIGURATION -- DON'T EDIT ANYTHING ELSE BEYOND THIS POINT 

//include std c headers
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) && defined(_DEBUG)
#include <vld.h> 
#endif

#include "xnz/xnz.h"

//XNG Image Public API
#include "xnz/xng_image.h"

//XNG Base
#include "xnz/xng_image_formats.h"
#include "xnz/xng_image_buffer.h" //this must come after cr_image.h

//XNG Font Public API
#include "xnz/xng_ttf_font.h"
#include "xnz/xng_font.h"

//XNG Image Types
#include "xnz/xng_fpf_image.h"
#include "xnz/xng_raw_image.h"

//XNG Font Types
//#include "xnz/xng_ttf_font.h"

//XNZ Image Types
#include "xnz/xnz_icc_profile.h"
#include "xnz/xnz_bmp_image.h"
#include "xnz/xnz_dds_image.h"
#include "xnz/xnz_png_image.h"
#include "xnz/xnz_sgi_image.h"


#ifdef __cplusplus
}
#endif

#endif //_CRIMAGE_H
