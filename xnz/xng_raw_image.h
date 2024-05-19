#ifndef _XNG_RAW_IMAGE_H
#define _XNG_RAW_IMAGE_H


/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

 /* We are building or calling CRPrimitives as a static library */
#ifndef XNG_RAW_API
#define XNG_RAW_API static
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_RAW_INLINE
#ifdef _WIN32
#define XNG_RAW_INLINE __inline
#else
#define XNG_RAW_INLINE
#endif
#endif
    
//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_RAW_DECLSPEC
#ifdef _WIN32
#define XNG_RAW_DECLSPEC __declspec
#else
#define XNG_RAW_DECLSPEC
#endif
#endif
    
//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_RAW_ALIGN
#ifdef _WIN32
#define XNG_RAW_ALIGN(X) (align(X))
#else
#define XNG_RAW_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif
    
#ifndef XNG_RAW_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_RAW_PACK_ATTRIBUTE
#pragma pack(push, 1)
#else
#define XNG_RAW_PACK_ATTRIBUTE __attribute__ ((packed))
#endif
#endif

#include "crUtils/cr_byte_utils.h"
#include "crUtils/cr_file_utils.h"
#include <math.h>
#include <assert.h>

XNG_RAW_API XNG_RAW_INLINE void xng_image_buffer_import_bytes(unsigned char ** imageBuffer, unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, const char * filepath);
XNG_RAW_API XNG_RAW_INLINE void xng_image_buffer_export_bytes(unsigned char * imageBuffer,  unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, const char * filepath);

XNG_RAW_API XNG_RAW_INLINE void xng_image_buffer_import_bytes(unsigned char** imageBuffer, unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, const char* filepath)
{
	FILE* file;
	size_t fileLength;

	unsigned long buffer_size;
	buffer_size = width * height * numChannels * (depth / 8);

	//allocate memory for the buffer, but first we might as well open the file and ensure the buffer size agrees

	/* open the file for reading bytes*/

#ifdef _WIN32 // || LINUX
	errno = fopen_s(&file, filepath, "rb"); // open the file in binary mode and check the result //

	if (errno != 0)
	{
		fprintf(stderr, "\nxng_image_buffer_import_bytes unable to open file with error: %d\n", errno);
		//return XNG_RAW_API_INVALID_FILE;
	}
#else
	file = fopen(filepath, "rb");
#endif

	assert(file);

	//seek to end of file to get byte offset to know the total size of file
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);

	//ensure the image file is larger than the buffer we've allocated to store it in
	assert(fileLength <= buffer_size);

	//Now allocate memory for the buffer and read
	// TO DO:  ensure buffer is 16 byte aligned when allocated
	(*imageBuffer) = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));

	//seek back to beginning of file for reading
	fseek(file, 0, SEEK_SET); //rewind(file);

	//read the entire image into buffer as one chunk of bytes
	fread((*imageBuffer), buffer_size, 1, file);


	/* close the file */
	fclose(file);
}

XNG_RAW_API XNG_RAW_INLINE void xng_image_buffer_export_bytes(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, const char* filepath)
{
	FILE* file;

	unsigned int buffer_size;
	buffer_size = width * height * numChannels * (depth / 8);

	//try to append to file first
	file = fopen(filepath, "ab");

	//if not create file for writing
	if (!file)
	{
		/* open the file for writing bytes*/
		file = fopen(filepath, "wb");
	}

	assert(file);

	//write the contents of the buffer
	fwrite(imageBuffer, buffer_size, 1, file);

	//close the file
	fclose(file);
}

#endif
