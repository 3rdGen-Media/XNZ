#ifndef _XNZ_DDS_IMAGE_H_
#define _XNZ_DDS_IMAGE_H_

/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

/* We are building or calling CRPrimitives as a static library */
#ifndef XNG_DDS_API
#define XNG_DDS_API static
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_DDS_INLINE
#ifdef _WIN32
#define XNG_DDS_INLINE __inline
#else
#define XNG_DDS_INLINE
#endif
#endif
    
//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_DDS_DECLSPEC
#ifdef _WIN32
#define XNG_DDS_DECLSPEC __declspec
#else
#define XNG_DDS_DECLSPEC
#endif
#endif
    
//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_DDS_ALIGN
#ifdef _WIN32
#define XNG_DDS_ALIGN(X) (align(X))
#else
#define XNG_DDS_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif
    
#ifndef XNG_DDS_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_DDS_PACK_ATTRIBUTE
#pragma pack(push, 1)
#else
#define XNG_DDS_PACK_ATTRIBUTE __attribute__ ((packed))
#endif
#endif

//header includes
#include "xng_image_formats.h"

//implementation includes
#include <stdio.h>      // printf
#include <stdlib.h>		// strtol
#include <ctype.h>		// isspace
#include <string.h>

/*
* FOURCC codes for DX compressed-texture pixel formats
*/
#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT2  (MAKEFOURCC('D','X','T','2'))
#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT4  (MAKEFOURCC('D','X','T','4'))
#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

/*
* FOURCC codes for 3dc compressed-texture pixel formats
*/
#define FOURCC_ATI1  (MAKEFOURCC('A','T','I','1'))
#define FOURCC_ATI2  (MAKEFOURCC('A','T','I','2'))


//
// DDSURFACEDESC2 flags that mark the validity of the struct data
//
#define DDSD_CAPS               0x00000001l     // default
#define DDSD_HEIGHT             0x00000002l        // default
#define DDSD_WIDTH              0x00000004l        // default
#define DDSD_PIXELFORMAT        0x00001000l        // default
#define DDSD_PITCH              0x00000008l     // For uncompressed formats
#define DDSD_MIPMAPCOUNT        0x00020000l
#define DDSD_LINEARSIZE         0x00080000l     // For compressed formats
#define DDSD_DEPTH              0x00800000l        // Volume Textures

//
// DDPIXELFORMAT flags
//
#define DDPF_ALPHAPIXELS        0x00000001l
#define DDPF_FOURCC             0x00000004l        // Compressed formats 
#define DDPF_RGB                0x00000040l        // Uncompressed formats
#define DDPF_ALPHA              0x00000002l
#define DDPF_COMPRESSED         0x00000080l
#define DDPF_LUMINANCE          0x00020000l
#define DDPF_BUMPLUMINANCE      0x00040000l        // L,U,V
#define DDPF_BUMPDUDV           0x00080000l        // U,V

//
// DDSCAPS flags
//
#define DDSCAPS_TEXTURE         0x00001000l     // default
#define DDSCAPS_COMPLEX         0x00000008l
#define DDSCAPS_MIPMAP          0x00400000l
#define DDSCAPS2_VOLUME         0x00200000l

//typedef unsigned int UI32;
//typedef int I32;

typedef struct  DDCOLORKEY
{
	/*
    DDCOLORKEY():
        dwColorSpaceLowValue(0),
        dwColorSpaceHighValue(0) {}
    */    
    unsigned int    dwColorSpaceLowValue;
    unsigned int    dwColorSpaceHighValue;
}DDCOLORKEY;

typedef struct DDPIXELFORMAT
{
	/*
    DDPIXELFORMAT():
        dwSize(0),
        dwFlags(0),
        dwFourCC(0),
        dwRGBBitCount(0),
        dwRBitMask(0),
        dwGBitMask(0),
        dwBBitMask(0),
        dwRGBAlphaBitMask(0) {}
    */  

    unsigned int    dwSize;
    unsigned int    dwFlags;
    unsigned int    dwFourCC;
    union
    {
        unsigned int    dwRGBBitCount;
        unsigned int    dwYUVBitCount;
        unsigned int    dwZBufferBitDepth;
        unsigned int    dwAlphaBitDepth;
        unsigned int    dwLuminanceBitDepth;
    };
    union
    {
        unsigned int    dwRBitMask;
        unsigned int    dwYBitMask;
    };
    union
    {
        unsigned int    dwGBitMask;
        unsigned int    dwUBitMask;
    };
    union
    {
        unsigned int    dwBBitMask;
        unsigned int    dwVBitMask;
    };
    union
    {
        unsigned int    dwRGBAlphaBitMask;
        unsigned int    dwYUVAlphaBitMask;
        unsigned int    dwRGBZBitMask;
        unsigned int    dwYUVZBitMask;
    };
}DDPIXELFORMAT;

typedef struct  DDSCAPS2
{
	/*
     DDSCAPS2():
        dwCaps(0),
        dwCaps2(0),
        dwCaps3(0),
        dwCaps4(0) {}
	*/
    unsigned int       dwCaps;
    unsigned int       dwCaps2;
    unsigned int       dwCaps3;
    union
    {
        unsigned int       dwCaps4;
        unsigned int       dwVolumeDepth;
    };
}DDSCAPS2;


typedef struct DDSURFACEDESC2
{
	/*
    DDSURFACEDESC2():
        dwSize(0),
        dwFlags(0),
        dwHeight(0),
        dwWidth(0), 
        lPitch(0),
        dwBackBufferCount(0),
        dwMipMapCount(0),
        dwAlphaBitDepth(0),
        dwReserved(0),     
        lpSurface(0),      
        dwTextureStage(0) {}      
     */  

    unsigned int         dwSize;
    unsigned int         dwFlags;
    unsigned int         dwHeight;
    unsigned int         dwWidth; 
    union                          
    {
        int              lPitch;
        unsigned int     dwLinearSize;
    };
    union
    {
        unsigned int      dwBackBufferCount;
        unsigned int      dwDepth;      
    };
    union
    {
        unsigned int     dwMipMapCount;
        unsigned int     dwRefreshRate;
    };
    unsigned int         dwAlphaBitDepth;
    unsigned int         dwReserved;     
    unsigned int        lpSurface;         //Fred Marmond: removed from pointer type to unsigned int for 64bits compatibility. it is unused data 
    DDCOLORKEY    ddckCKDestOverlay;      
    DDCOLORKEY    ddckCKDestBlt;           
    DDCOLORKEY    ddckCKSrcOverlay;        
    DDCOLORKEY    ddckCKSrcBlt;            
    DDPIXELFORMAT ddpfPixelFormat;         
    DDSCAPS2      ddsCaps;                 
    unsigned int dwTextureStage;          
}DDSURFACEDESC2;

XNG_DDS_API XNG_DDS_INLINE XNG_IMAGE_API_STATUS xnz_load_dds_image_compressed(xng_image * img);


XNG_DDS_API XNG_DDS_INLINE XNG_IMAGE_API_STATUS xnz_load_dds_image_compressed(xng_image* img)
{
	//declarations
	DDSURFACEDESC2 ddsd;
	char filecode[4];
	FILE* file;
	unsigned int filesize;

	//image properties
	int s;
	int t;
	int r;
	unsigned int dataType;
	unsigned int pixelFormat;
	unsigned int internalFormat;
	unsigned char* imageData;
	unsigned int size;
	unsigned int sizeWithMipmaps;

	int depth;
	bool found;
	bool usingAlpha;

	unsigned int i;

	struct xng_rgb_format format;
	//initializations
	filesize = 0;
	//ddsd = {0};
	memset(&ddsd, 0, sizeof(ddsd));
	imageData = (unsigned char*)NULL;

	//start function
	if (!img)
		return XNG_IMAGE_API_NULL_CONTAINER;
	if (!img->filepath)
		return XNG_IMAGE_API_INPUT_FILE_ERROR;

	//std::ifstream _istream( filename.c_str(), std::ios::in | std::ios::binary );

#ifdef _WIN32 // || LINUX
	errno = fopen_s(&file, img->filepath, "rb"); // open the file in binary mode and check the result //

	if (errno != 0)
	{
		printf("\ncr_dds_image unable to open file %d\n", errno);
		return XNG_IMAGE_API_INVALID_FILE;
	}
#else
	file = fopen(img->filepath, "rb");
#endif

	if (!file)
		return XNG_IMAGE_API_INPUT_FILE_ERROR;

	//read fourcc code
	//_istream.read(filecode, 4);
	fread(filecode, 4, 1, file);

	if (strncmp(filecode, "DDS ", 4) != 0) {
		return XNG_IMAGE_API_INCOMPATIBLE_FOURCC;
	}

	// Get the surface desc.
	fread((char*)(&ddsd), sizeof(DDSURFACEDESC2), 1, file);

	//Check valid structure sizes
	if (ddsd.dwSize != 124 && ddsd.ddpfPixelFormat.dwSize != 32)
	{
		return XNG_IMAGE_API_INVALID_FILE_STRUCTURE;
	}

	depth = 1;

	// Check for volume image
	if (ddsd.dwDepth > 0 && (ddsd.dwFlags & DDSD_DEPTH))
	{
		depth = ddsd.dwDepth;
	}

	// Retreive image properties.
	s = ddsd.dwWidth;
	t = ddsd.dwHeight;
	r = depth;
	dataType = XNG_UNSIGNED_BYTE;
	pixelFormat = 0;
	internalFormat = 0;

	// Handle some esoteric formats
	if (ddsd.ddpfPixelFormat.dwFlags & DDPF_BUMPDUDV)
	{
		printf("\nReadDDSFile warning: DDPF_BUMPDUDV format is not supported\n");
		return XNG_IMAGE_API_UNSUPPORTED_FORMAT;
	}
	if (ddsd.ddpfPixelFormat.dwFlags & DDPF_BUMPLUMINANCE)
	{
		printf("\nReadDDSFile warning: DDPF_BUMPLUMINANCE format is not supported\n");
		return XNG_IMAGE_API_UNSUPPORTED_FORMAT;
	}

	usingAlpha = ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS;

	// Uncompressed formats.
	if (ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB)
	{

		found = false;

		for (i = 0; i < sizeof(XNG_IMAGE_RGB_FORMATS) / sizeof(xng_rgb_format); i++)
		{
			format = XNG_IMAGE_RGB_FORMATS[i];
			if (ddsd.ddpfPixelFormat.dwRGBBitCount == format.bitCount &&
				ddsd.ddpfPixelFormat.dwRBitMask == format.rBitMask &&
				ddsd.ddpfPixelFormat.dwGBitMask == format.gBitMask &&
				ddsd.ddpfPixelFormat.dwBBitMask == format.bBitMask &&
				ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == format.aBitMask)
			{
				if (format.internalFormat != XNG_IMAGE_UNSUPPORTED_PIXEL_FORMAT &&
					format.pixelFormat != XNG_IMAGE_UNSUPPORTED_PIXEL_FORMAT &&
					format.dataType != XNG_IMAGE_UNSUPPORTED_PIXEL_FORMAT)
				{
					printf("\nReadDDSFile info : format = %s", format.name);// << f.name << std::endl;
					internalFormat = format.internalFormat;
					pixelFormat = format.pixelFormat;
					dataType = format.dataType;
					found = true;
					break;
				}
				else
				{
					printf("\nReadDDSFile info : %s format is not supported\n", format.name);
					return XNG_IMAGE_API_UNSUPPORTED_FORMAT;
				}
			}
		}

		if (!found)
		{
			printf("\nReadDDSFile warning: unhandled RGB pixel format in dds file, image not loaded\n");
			printf("\nReadDDSFile info : ddsd.ddpfPixelFormat.dwRGBBitCount     = %d", ddsd.ddpfPixelFormat.dwRGBBitCount);

			printf("\nReadDDSFile info : ddsd.ddpfPixelFormat.dwRBitMask		= 0x%08x", ddsd.ddpfPixelFormat.dwRBitMask);
			printf("\nReadDDSFile info : ddsd.ddpfPixelFormat.dwGBitMask        = 0x%08x", ddsd.ddpfPixelFormat.dwGBitMask);
			printf("\nReadDDSFile info : ddsd.ddpfPixelFormat.dwBBitMask        = 0x%08x", ddsd.ddpfPixelFormat.dwBBitMask);
			printf("\nReadDDSFile info : ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0x%08x\n", ddsd.ddpfPixelFormat.dwRGBAlphaBitMask);
			return XNG_IMAGE_API_UNHANDLED_PIXEL_FORMAT;
		}

	}
	else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_LUMINANCE)
	{
		internalFormat = usingAlpha ? XNG_LUMINANCE_ALPHA : XNG_LUMINANCE;
		pixelFormat = usingAlpha ? XNG_LUMINANCE_ALPHA : XNG_LUMINANCE;
		if (usingAlpha && ddsd.ddpfPixelFormat.dwLuminanceBitDepth == 8)
		{
			printf("\nReadDDSFile info : format = L4A4\n");
			pixelFormat = XNG_LUMINANCE4_ALPHA4; // invalid enumerant?
		}
		else if (usingAlpha && ddsd.ddpfPixelFormat.dwLuminanceBitDepth == 32)
		{
			printf("\nReadDDSFile info : format = L16A16\n");
			dataType = XNG_UNSIGNED_SHORT;
		}
		else if (!usingAlpha && ddsd.ddpfPixelFormat.dwLuminanceBitDepth == 16)
		{
			printf("\nReadDDSFile info : format = L16\n");
			dataType = XNG_UNSIGNED_SHORT;
		}
		else if (usingAlpha)
		{
			printf("\nReadDDSFile info : format = L8A8\n");
		}
		else
		{
			printf("\nReadDDSFile info : format = L8\n");
		}
	}
	else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHA)
	{
		printf("\nReadDDSFile info : format = ALPHA\n");
		internalFormat = XNG_ALPHA;
		pixelFormat = XNG_ALPHA;
	}
	// Compressed formats
	else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC)
	{
		switch (ddsd.ddpfPixelFormat.dwFourCC)
		{
		case FOURCC_DXT1:
			printf("\nReadDDSFile info : format = DXT1, usingAlpha=%d", usingAlpha);// <<usingAlpha<< std::endl;
			if (usingAlpha)
			{
				internalFormat = XNG_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				pixelFormat = XNG_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			}
			else
			{
				internalFormat = XNG_COMPRESSED_RGB_S3TC_DXT1_EXT;
				pixelFormat = XNG_COMPRESSED_RGB_S3TC_DXT1_EXT;
			}
			break;
		case FOURCC_DXT3:
			//printf("\nReadDDSFile info : format = DXT3\n");
			internalFormat = XNG_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			pixelFormat = XNG_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			//printf("\nReadDDSFile info : format = DXT5\n");
			internalFormat = XNG_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			pixelFormat = XNG_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		case FOURCC_ATI1:
			printf("\nReadDDSFile info : format = ATI1\n");
			internalFormat = XNG_COMPRESSED_RED_RGTC1_EXT;
			pixelFormat = XNG_COMPRESSED_RED_RGTC1_EXT;
			break;
		case FOURCC_ATI2:
			printf("\nReadDDSFile info : format = ATI2\n");
			internalFormat = XNG_COMPRESSED_RED_GREEN_RGTC2_EXT;
			pixelFormat = XNG_COMPRESSED_RED_GREEN_RGTC2_EXT;
			break;
		case 0x00000024: // A16B16G16R16
			printf("\nReadDDSFile info : format = A16B16G16R16\n");
			internalFormat = XNG_RGBA;
			pixelFormat = XNG_RGBA;
			dataType = XNG_UNSIGNED_SHORT;
			break;
		case 0x00000071: // A16B16G16R16F
			printf("\nReadDDSFile info : format = A16B16G16R16F\n");
			internalFormat = XNG_RGBA; // why no transparency?
			pixelFormat = XNG_RGBA;
			dataType = XNG_HALF_FLOAT_NV;
			break;
		case 0x0000006E: // Q16W16V16U16
			printf("\nReadDDSFile info : format = Q16W16V16U16\n");
			internalFormat = XNG_RGBA;
			pixelFormat = XNG_RGBA;
			dataType = XNG_UNSIGNED_SHORT;
			break;
		case 0x00000070: // G16R16F
			printf("\nReadDDSFile info : G16R16F format is not supported\n");
			return XNG_IMAGE_API_UNSUPPORTED_FORMAT;
			break;
		case 0x00000073: // G32R32F
			printf("\nReadDDSFile info : G32R32F format is not supported\n");
			return XNG_IMAGE_API_UNSUPPORTED_FORMAT;
			break;
		case 0x00000072: // R32F
			printf("\nReadDDSFile info : format = R32F\n");
			internalFormat = XNG_RGB;
			pixelFormat = XNG_RED;
			dataType = XNG_FLOAT;
			break;
		case 0x0000006F: // R16F
			printf("\nReadDDSFile info : format = R16F\n");
			internalFormat = XNG_RGB;
			pixelFormat = XNG_RED;
			dataType = XNG_HALF_FLOAT_NV;
			break;
		case 0x00000074: // A32B32G32R32F
			printf("\nReadDDSFile info : format = A32B32G32R32F\n");
			internalFormat = XNG_RGBA;
			pixelFormat = XNG_RGBA;
			dataType = XNG_FLOAT;
			break;
		case 0x00000075: // CxV8U8
			printf("\nReadDDSFile info : CxV8U8 format is not supported\n");
			return XNG_IMAGE_API_UNSUPPORTED_FORMAT;

		case MAKEFOURCC('U', 'Y', 'V', 'Y'): // not supported in OSG
		case MAKEFOURCC('U', 'Y', 'V', '2'): // not supported in OSG
		case MAKEFOURCC('R', 'G', 'B', 'G'): // R8G8_B8G8 -- what is it?
		case MAKEFOURCC('G', 'R', 'G', 'B'): // G8R8_G8B8 -- what is it?
			//break;

		default:
			printf("\nReadDDSFile warning: unhandled FOURCC pixel format (%c%c%c%c = 0x%08x) in dds file, image not loaded.\n", \
				(char)((ddsd.ddpfPixelFormat.dwFourCC & 0x000000ff)), \
				(char)((ddsd.ddpfPixelFormat.dwFourCC & 0x0000ff00) >> 8), \
				(char)((ddsd.ddpfPixelFormat.dwFourCC & 0x00ff0000) >> 16), \
				(char)((ddsd.ddpfPixelFormat.dwFourCC & 0xff000000) >> 24), \
				ddsd.ddpfPixelFormat.dwFourCC);

			return XNG_IMAGE_API_UNHANDLED_FOURCC_FORMAT;
		}
	}
	else
	{
		printf("\nReadDDSFile warning: unhandled pixel format (ddsd.ddpfPixelFormat.dwFlags = 0x%08x) in dds file, image not loaded.\n", \
			ddsd.ddpfPixelFormat.dwFlags);

		return XNG_IMAGE_API_UNHANDLED_PIXEL_FORMAT;
	}

	size = xng_image_compute_size_in_bytes(s, t, r, pixelFormat, dataType, 1, 1, 1);

	// Take care of mipmaps if any.
	sizeWithMipmaps = size;

	imageData = (unsigned char*)malloc(sizeWithMipmaps * sizeof(unsigned char));
	if (!imageData)
	{
		printf("\nReadDDSFile warning: imageData == NULL\n");
		return XNG_IMAGE_API_UNABLE_TO_READ_DATA;
	}

	fread((unsigned char*)imageData, size, 1, file);

	//#ifdef _WIN32
	if (ferror(file))
	{
		free(imageData);
		printf("\nReadDDSFile warning: couldn't read imageData\n");
		return XNG_IMAGE_API_UNABLE_TO_READ_DATA;
	}
	//#endif

	fclose(file);

	//}//end if(file)

	img->width = s;
	img->height = t;
	img->depth = r;
	img->pixelFormat = pixelFormat;
	img->numChannels = xng_image_compute_pixel_format_components(img->pixelFormat);
	img->size = sizeWithMipmaps;
	img->pixelInternalFormat = internalFormat;
	img->pixelType = dataType;
	img->pixelData = imageData;

	return XNG_IMAGE_API_SUCCESS;
}

#endif //_XNZ_DDS_IMAGE_H_
