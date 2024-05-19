#ifndef XNG_FPF_IMAGE_H
#define XNG_FPF_IMAGE_H


/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

 /* We are building or calling XNG_FPF_IMAGE as a static library */
#ifndef XNG_FPF_API
#define XNG_FPF_API static
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_FPF_INLINE
#ifdef _WIN32
#define XNG_FPF_INLINE __inline
#else
#define XNG_FPF_INLINE
#endif
#endif
    
//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_FPF_DECLSPEC
#ifdef _WIN32
#define XNG_FPF_DECLSPEC __declspec
#else
#define XNG_FPF_DECLSPEC
#endif
#endif
    
//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_FPF_ALIGN
#ifdef _WIN32
#define XNG_FPF_ALIGN(X) (align(X))
#else
#define XNG_FPF_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif
    
#ifndef XNG_FPF_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_FPF_PACK_ATTRIBUTE
#pragma pack(push, 1)
#else
#define XNG_FPF_PACK_ATTRIBUTE __attribute__ ((packed))
#endif
#endif

#include "crUtils/cr_byte_utils.h"
#include "crUtils/cr_file_utils.h"
#include <math.h>

#ifdef _WIN32
#pragma pack(push, 1)
#endif


XNG_FPF_DECLSPEC XNG_FPF_ALIGN(1) typedef struct XNG_FPF_PACK_ATTRIBUTE cr_fpf_image_header /* 14 bytes */
{
	uint32_t width, height, depth, numChannels, size;
	
	//unsigned char type[2];		 /* Magic identifier          */
	//unsigned int size;             /* Header size in bytes      */
	//int width, height;             /* Width and height of image */
	//unsigned short int planes;     /* Number of colour planes   */
	//unsigned short int bits;       /* Bits per pixel            */
	//unsigned int compression;      /* Compression type          */
	//unsigned int imagesize;        /* Image size in bytes       */
	//int xresolution, yresolution;  /* Pixels per meter          */
	//unsigned int ncolours;         /* Number of colours         */
	//unsigned int importantcolours; /* Important colours         */
} cr_fpf_image_header;

#ifdef _WIN32
#pragma pack(pop)
#endif


XNG_FPF_API XNG_FPF_INLINE void xng_load_fpf_image(xng_image* img);
XNG_FPF_API XNG_FPF_INLINE void xng_image_buffer_export_to_fpf(float* imageBuffer, unsigned int width, unsigned int height,  unsigned int depth, unsigned int numChannels, const char* filepath);

XNG_FPF_API XNG_FPF_INLINE void xng_load_fpf_image(xng_image* img)
{
	FILE* file;
#if defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != fopen_s(&file, img->filepath, "rb")) file = 0;
#else
	file = fopen(img->filepath, "rb");
#endif

	uint32_t nPixels = 0;
	float* floatPixels = NULL;
	cr_fpf_image_header fpf_header = { 0 };

	fread(&fpf_header, sizeof(cr_fpf_image_header), 1, file); nPixels = fpf_header.width * fpf_header.height;
	assert(fpf_header.size = nPixels * fpf_header.numChannels * fpf_header.depth / 8);

	if (fpf_header.numChannels == 3)
	{
		//3 channel images need to be converted to 4 channels for most graphics formats
		floatPixels = (float*)malloc(fpf_header.size + (nPixels * sizeof(float)));
		int pIndex; for (pIndex = 0; pIndex < nPixels; pIndex++)
		{
			fread(&floatPixels[pIndex * 4], 3 * sizeof(float), 1, file);
			floatPixels[pIndex * 4 + 3] = 1.f;
		}
	}
	else
	{
		floatPixels = (float*)malloc(fpf_header.size);
		fread(floatPixels, fpf_header.size, 1, file);



	}

	//FPF image files have been defined by me to be RGBA format on disk (similar to bmp, png, jpg, etc)
	//if we request loading as BGRA perform twiddle
	if (img->pixelFormat == XNG_BGRA)
	{
		int pIndex; for (pIndex = 0; pIndex < nPixels; pIndex++)
		{
			//convert floating point rgba pixels to bgra
			float tmp = floatPixels[pIndex * 4];
			floatPixels[pIndex * 4] = floatPixels[pIndex * 4 + 2];
			floatPixels[pIndex * 4 + 2] = tmp;
		}
	}
	fclose(file);

	//TO DO:  make fpf_header and xng_image header the same definition?
	img->width = fpf_header.width;
	img->height = fpf_header.height;
	img->depth = fpf_header.depth;
	img->numChannels = fpf_header.numChannels;
	img->size = fpf_header.size;
	img->format = XNG_IMAGE_FORMAT_FPF;
	img->pixelData = (unsigned char*)floatPixels;

}



XNG_FPF_API XNG_FPF_INLINE void xng_image_buffer_export_to_fpf(float* imageBuffer, unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, const char* filepath)
{
	FILE* file;
	char* filePath;

	//TODO:  upgrade xng_image to use uint64_t at least for size
	uint32_t buffer_size = width * height * numChannels * (depth / 8);

	cr_fpf_image_header fpf_header = { 0 };
	fpf_header.width = width;
	fpf_header.height = height;
	fpf_header.depth = depth;
	fpf_header.numChannels = numChannels;
	fpf_header.size = buffer_size;

	//check if filepath already has 
	filePath = (char*)malloc(strlen(filepath) + 1);
	strcpy(filePath, filepath);

	if (strcmp(cr_find_file_extension(filePath), "fpf") != 0 && strcmp(cr_find_file_extension(filePath), "FPF") != 0) strcat(filePath, ".fpf");

	/* open the file for writing*/
	file = fopen(filePath, "wb");
	assert(file);

	//write the header
	fwrite(&fpf_header, sizeof(fpf_header), 1, file);

	//write the image buffer bytes
	fwrite(imageBuffer, buffer_size, 1, file);

	/* close the file*/
	fclose(file);
	free(filePath);
}


#endif
