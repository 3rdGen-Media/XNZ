#ifndef XNZ_BMP_IMAGE_H
#define XNZ_BMP_IMAGE_H

/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

/* We are building or calling CRPrimitives as a static library */
#ifndef XNG_BMP_API
#define XNG_BMP_API static
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_BMP_INLINE
#ifdef _WIN32
#define XNG_BMP_INLINE __inline
#else
#define XNG_BMP_INLINE
#endif
#endif
    
    //inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_BMP_DECLSPEC
#ifdef _WIN32
#define XNG_BMP_DECLSPEC __declspec
#else
#define XNG_BMP_DECLSPEC
#endif
#endif
    
    //align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_BMP_ALIGN//(X)
#ifdef _WIN32
#define XNG_BMP_ALIGN(X) (align(X))
#else
#define XNG_BMP_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif
    
#ifndef XNG_BMP_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_BMP_PACK_ATTRIBUTE
#pragma pack(push, 1)
//#define CR_TEXT_PACK_ATTRIBUTE( __Declaration__ ) __Declaration__ __attribute__((packed))
#else
//#define CR_TEXT_PACK_ATTRIBUTE ((packed, aligned(4)))
#define XNG_BMP_PACK_ATTRIBUTE __attribute__ ((packed))
//#define CR_TEXT_PACK_ATTRIBUTE( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#endif
#endif

#include "crUtils/cr_byte_utils.h"
#include "crUtils/cr_file_utils.h"
#include <math.h>

#ifdef _WIN32
#pragma pack(push, 1)
#endif


XNG_BMP_DECLSPEC XNG_BMP_ALIGN(1) typedef struct XNG_BMP_PACK_ATTRIBUTE cr_bmp_image_header /* 14 bytes */
{
   unsigned char type[2];			        /* Magic identifier            */
   unsigned int size;                       /* File size in bytes          */
   unsigned short int reserved1, reserved2;
   unsigned int offset;                     /* Offset to image data, bytes */
} cr_bmp_image_header;


XNG_BMP_DECLSPEC XNG_BMP_ALIGN(1) typedef struct XNG_BMP_PACK_ATTRIBUTE cr_bmp_image_info_header /* 40 bytes */
{
   unsigned int size;               /* Header size in bytes      */
   int width,height;                /* Width and height of image */
   unsigned short int planes;       /* Number of colour planes   */
   unsigned short int bits;         /* Bits per pixel            */
   unsigned int compression;        /* Compression type          */
   unsigned int imagesize;          /* Image size in bytes       */
   int xresolution,yresolution;     /* Pixels per meter          */
   unsigned int ncolours;           /* Number of colours         */
   unsigned int importantcolours;   /* Important colours         */
} cr_bmp_image_info_header;


#ifdef _WIN32
#pragma pack(pop)
#endif

XNG_BMP_API XNG_BMP_INLINE void xnz_image_buffer_export_to_bmp(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, const char * filepath);
XNG_BMP_API XNG_BMP_INLINE void xnz_image_buffer_export_to_bmp(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, const char* filepath)
{
	//BMP files are always little-endian

	//The first byte is 'B' (0x42), the second byte is 'M' (0x4D)

	//A little endian uint16_t would see this as 0x4D42
	FILE* file;
	char* filePath;
	cr_bmp_image_header bmp_header;
	cr_bmp_image_info_header bmp_info_header;
	unsigned int buffer_size;
	unsigned int color_palette_size;
	unsigned int i;
	unsigned int image_buffer_offset;
	//unsigned char rmod4;
	unsigned char numPadBytes;
	unsigned int y, n;
	unsigned int padding;
	uint8_t color[4];

	unsigned int NUMBER_OF_COLORS = (unsigned int)powf(2.f, (float)(depth * numChannels));

	color_palette_size = 0;

	if (numChannels == 1)
		color_palette_size = 4 * NUMBER_OF_COLORS;

	//printf("\ncolor_palette_size = %u\n", color_palette_size);

	//const int COLOR_PALETTE_SIZE = NUMBER_OF_COLORS * sizeof(RGBQUAD);
	//const int HEADER_OFFSET = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + COLOR_PALETTE_SIZE;
	//const int TOTAL_FILE_SIZE = HEADER_OFFSET + paddedsize;


	//printf("\ncr_bmp_image_header size = %d\n", sizeof(cr_bmp_image_header));
	assert(sizeof(cr_bmp_image_header) == 14);
	//printf("\ncr_bmp_image_info_header size = %d\n", sizeof(cr_bmp_image_info_header));
	assert(sizeof(cr_bmp_image_info_header) == 40);

	buffer_size = width * height * numChannels * (depth / 8);

	//if( numChannels == 1 )

	//printf("\nbuffer_size = %u\n", buffer_size);

	image_buffer_offset = sizeof(cr_bmp_image_header) + sizeof(cr_bmp_image_info_header) + color_palette_size;


	//rmod4 = image_buffer_offset % 4;
	numPadBytes = 0;
	//if( rmod4 != 0 )
	//{
		//numPadBytesPerLine = 2;//4 - (((width * numChannels * depth)/8) % 4);
		//image_buffer_offset += numPadBytes;

		//buffer_size += numPadBytesPerLine * height;

		//image_buffer_offset -= 2;
	//}

	//numPadBytes = 2;
	//buffer_size += numPadBytes;

	padding = 4 - ((width * numChannels * (depth / 8)) % 4);
	if (padding == 4) padding = 0;

	buffer_size += padding * height;

	//printf("\n padding = %u\n", padding);

	bmp_header.type[0] = 'B';
	bmp_header.type[1] = 'M';
	bmp_header.size = cr_htole16(image_buffer_offset/*sizeof(cr_bmp_image_header) + sizeof( cr_bmp_image_info_header) + color_palette_size + numPadBytes*/ + buffer_size);
	bmp_header.reserved1 = 0;
	bmp_header.reserved2 = 0;
	bmp_header.offset = cr_htole16(image_buffer_offset);

	//printf("\nbmp_header.size = %u\n", bmp_header.size);
	//printf("\nbmp_header.offset = %u\n", bmp_header.offset);

	bmp_info_header.size = cr_htole16(sizeof(cr_bmp_image_info_header));
	bmp_info_header.width = cr_htole16(width);
	bmp_info_header.height = cr_htole16(height);
	bmp_info_header.planes = cr_htole16(1);
	bmp_info_header.bits = cr_htole16(depth * numChannels);
	bmp_info_header.compression = 0; //uncompressed for now
	bmp_info_header.imagesize = cr_htole16(buffer_size);
	bmp_info_header.xresolution = 0;//cr_htole16(353);
	bmp_info_header.yresolution = 0;//cr_htole16(353);
	bmp_info_header.ncolours = cr_htole16(NUMBER_OF_COLORS);
	bmp_info_header.importantcolours = 0;//cr_htole16(0);


	//check if filepath already has 

	filePath = (char*)malloc(strlen(filepath) + 1);
	strcpy(filePath, filepath);

	if (strcmp(cr_find_file_extension(filePath), "bmp") != 0 && strcmp(cr_find_file_extension(filePath), "BMP") != 0) strcat(filePath, ".bmp");

	/* open the file for writing*/
	file = fopen(filePath, "wb");

	assert(file);
	//write the bitmap header
	fwrite(&bmp_header, sizeof(cr_bmp_image_header), 1, file);

	/*
	//write the pad bytes to achieve necessary multiple of four byte offset into image buffer
	if( numPadBytes > 0 )
	{
		memset(color, 0, 4);
		for( i = 0; i< numPadBytes; i++ )
		{
			fwrite(color, 1, 1, file);
		}

	}
	*/
	//write the bitmap info header
	fwrite(&bmp_info_header, sizeof(cr_bmp_image_info_header), 1, file);


	//color[0] = 255;
	//color[3] = 255;
	//printf("\n number of colors = %u \n", NUMBER_OF_COLORS);
	color[0] = 0;
	//write the greyscale color ramp
	if (color_palette_size > 0)// numChannels == 1 && depth == 8)//color_palette_size > 0 )
	{
		for (i = 0; i < NUMBER_OF_COLORS; i++)
		{
			memset(color, i, 4);
			fwrite(color, 4, 1, file);
		}
	}

	/*
	//write the pad bytes to achieve necessary multiple of four byte offset into image buffer
	if( numPadBytes > 0 )
	{
		memset(color, 0, 4);
		for( i = 0; i< numPadBytes; i++ )
		{
			fwrite(color, 1, 1, file);
		}

	}
	*/

	memset(color, 0, 4);
	for (y = 0; y < height; y++)
	{

		//for( x = 0; x<width; x++)
		//{
		fwrite(imageBuffer + y * (width * numChannels * (depth / 8)), width * numChannels * (depth / 8), 1, file);


		if (padding > 0)
		{

			//if(width%2)
			//{
			for (n = 0; n < padding; n++)
			{
				fwrite(color, 1, 1, file);
				//fprintf(file, "%c", 0);
			}
			//}
		}

		//}

	}


	//write the image buffer bytes
	//fwrite(imageBuffer, buffer_size, 1, file);

	/*
	if( numPadBytes > 0 )
	{
		memset(color, 0, 4);
		for( i = 0; i< numPadBytes; i++ )
		{
			fwrite(color, 1, 1, file);
		}

	}
	*/


	/* close the file*/
	fclose(file);


	free(filePath);

}


#endif //_CR_BMP_IMAGE_H
