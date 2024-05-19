#ifndef XNZ_SGI_IMAGE_H
#define XNZ_SGI_IMAGE_H


#include "crSystem/cr_file.h" //CRFile
#include "stdint.h"           //uint types

#ifdef __cplusplus
extern "C" {
#endif

    //only define bool if not previoulsy defined by CoreRender crWindow global header
    //and not using C++
#ifndef __cplusplus
//typedef unsigned char bool;
#ifndef bool
#define bool int
#define true 1
#define false 0
#endif
#endif


/* We are building or calling crMath as a static library */
#ifndef XNZ_SGI_API
#define XNZ_SGI_API static
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNZ_SGI_INLINE
#ifdef _WIN32
#define XNZ_SGI_INLINE __inline
#else
#define XNZ_SGI_INLINE
#endif
#endif

//__decspec doesn't exist in C89, __declspec is MSVC specific
#ifndef XNZ_SGI_DECLSPEC
#ifdef _WIN32
#define XNZ_SGI_DECLSPEC __declspec
#else
#define XNZ_SGI_DECLSPEC
#endif
#endif

//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNZ_SGI_ALIGN
#ifdef _WIN32
#define XNZ_SGI_ALIGN(X) 
#else
#define XNZ_SGI_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif

#ifndef CR_PACK_ATTRIBUTE
#ifdef _WIN32
#define CR_PACK_ATTRIBUTE
#pragma pack(push, 1)
#else
#define CR_PACK_ATTRIBUTE __attribute__ ((packed))
#endif
#endif


//XNZ_SGI_API XNZ_SGI_INLINE void xng_load_sgi_image(xng_image* img);


//SGI Iris RGB Image


/***
 ***    UNIVERSAL SERIALIZE FUNCTION FOR ALL RECORDS DATA FIELDS PARSED BY X-MACROS
 ***
 ***    Converts FLT Record Big Endian Data Types to host format (e.g. Intel = Little Endian) using system level conversion calls
 ***/
static void xnz_sgi_field_serialize(void* data, unsigned int elementSizeInBits, unsigned int numElements)
{
    int i;
    //if the data type is 8 bits in size, such as char, then there is no need to convert it
    //because endianness only affects byte order and char is a single byte data type
    if (elementSizeInBits == 8) return;

    /*
    if (elementSizeInBits == 64)
    {
        uint64_t* doubleData = (uint64_t*)data;
        *doubleData = cr_be64toh(*doubleData);
        for (i = 1; i < numElements; i++)
        {
            doubleData[i] = cr_be64toh(doubleData[i]);
        }

    }
    else */
    if (elementSizeInBits == 32)
    {
        uint32_t* intData = (uint32_t*)data;
        *intData = cr_be32toh(*intData);
        for (i = 1; i < numElements; i++)
        {
            intData[i] = cr_be32toh(intData[i]);
        }

    }
    else if (elementSizeInBits == 16)
    {
        uint16_t* shortData = (uint16_t*)data;
        *shortData = cr_be16toh(*shortData);
        for (i = 1; i < numElements; i++)
        {
            shortData[i] = cr_be16toh(shortData[i]);
        }
    }
}


/***
 ***    PRINT FUNCTIONS FOR ALL [SUPPORTED] RECORD TYPES
 ***/
#pragma mark -- PRINT


 /***
  ***    UNIVERSAL PRINT FUNCTION FOR ALL RECORD DATA FIELDS PARSED BY X-MACROS
  ***/
static void xnz_sgi_field_print(char* formatString, char* catString, char* fieldName, void* data, unsigned int elementSizeInBits, unsigned int numElements/*, char * formatSpecifier */, bool floatingPoint)
{
    int i, bit;
    unsigned* bits;

    if (strstr(fieldName, "reserved") != NULL)
        return;

    /*
    if (elementSizeInBits == 64)
    {
        if (floatingPoint)
        {
            double* doubleData = (double*)data;
            fprintf(stderr, formatString, *doubleData);
            for (i = 1; i < numElements; i++)
            {
                fprintf(stderr, catString, doubleData[i]);
            }
        }
        else
        {
            uint64_t* intData = (uint64_t*)data;
            fprintf(stderr, formatString, *intData);
            for (i = 1; i < numElements; i++)
            {
                fprintf(stderr, catString, intData[i]);
            }
        }
    }
    else */
    if (elementSizeInBits == 32)
    {

        if (strstr(fieldName, "flags") != NULL)
        {
            fprintf(stderr, "\n\t%s = ", fieldName);
            bits = cr_convert_to_binary((unsigned*)data, elementSizeInBits);

            for (bit = elementSizeInBits; bit--;)
                fprintf(stderr, "%u", +bits[bit]);

            free(bits);
        }
        else
        {
            if (floatingPoint)
            {
                float* floatData = (float*)data;
                fprintf(stderr, formatString, *floatData);
                for (i = 1; i < numElements; i++)
                {
                    fprintf(stderr, catString, floatData[i]);
                }
            }
            else
            {
                uint32_t* intData = (uint32_t*)data;
                fprintf(stderr, formatString, *intData);
                for (i = 1; i < numElements; i++)
                {
                    fprintf(stderr, catString, intData[i]);
                }
            }


        }
    }
    else if (elementSizeInBits == 16)
    {
        if (strstr(fieldName, "flags") != NULL)
        {
            fprintf(stderr, "\n\t%s = ", fieldName);
            bits = cr_convert_to_binary((unsigned*)data, elementSizeInBits);

            for (bit = elementSizeInBits; bit--;)
                fprintf(stderr, "%u", +bits[bit]);

            free(bits);
        }
        else
        {
            short* shortData = (short*)data;
            fprintf(stderr, formatString, *shortData);
            for (i = 1; i < numElements; i++)
            {
                fprintf(stderr, catString, shortData[i]);
            }
        }
    }
    else
    {

        //how to differentiate between 8-bit integers stored as chars and and chararacter arrays which are strings?
        //solution is to assign %u instead of percent %s for static char arrays defined using x macros and print each element consecutively

        //arrays of 8-bit ints currently unsupported
        char* charData = (char*)data;
        if (numElements > 1)
            fprintf(stderr, formatString, charData);
        else
            fprintf(stderr, formatString, *charData);
        //for(int i = 1; i< numElements; i++)
        //{
        //    printf(catString, shortData[i]);
        //}
    }

}




/*
typedef struct XNZ_SGI_IMAGE_HEADER
{
    uint8_t   rle;
    int32_t   bytesPerChannel;
    int32_t   bitsPerChannel;
    int16_t   dimension;
    uint32_t  tablen;
    uint32_t* offsetTable;
    uint32_t* lengthTable;
    uint8_t * tmp;
} XNZ_SGI_IMAGE_HEADER;
*/

#define XNZ_SGI_HEADER_FIELDS \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint8_t,   8,  2,  magicNumber, magicNumber[2], "%s") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint8_t,   8,  1,  rle,         rle,            "%u") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint8_t,   8,  1,  bpc,         bpc,            "%d") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint16_t, 16,  1,  dimension,   dimension,      "%hu") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint16_t, 16,  1,  xsize,       xsize,          "%hu") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint16_t, 16,  1,  ysize,       ysize,          "%hu") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint16_t, 16,  1,  nsize,       nsize,          "%hu") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, int32_t,  32,  1,  pixmin,      pixmin,         "%d") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, int32_t,  32,  1,  pixmax,      pixmax,         "%d") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint8_t,   8,  1,  dummy,       dummy,          "%u") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, char,      8,  80, name,        name[80],       "%s") \
XNZ_SGI_HEADER_FIELD(XNZ_SGI_HEADER, uint8_t,   8,  1,  colormmap,   colormmap,      "%s")

//--- define the structure, the X macro will be expanded once per field
XNZ_SGI_ALIGN(1) typedef struct CR_PACK_ATTRIBUTE XNZ_SGI_HEADER
{
    //the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNZ_SGI_HEADER_FIELD(xnz_sgi_struct, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
        XNZ_SGI_HEADER_FIELDS
#undef  XNZ_SGI_HEADER_FIELD
} XNZ_SGI_HEADER;


//Serialize specific record types;  X-MACROS are used to iterate over each field in each flt record struct with minimal code
static void xnz_sgi_header_serialize(void* sgi_header_data)
{

#define XNZ_SGI_HEADER_FIELD(xnz_sgi_struct, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
        xnz_sgi_field_serialize(&(((xnz_sgi_struct *)sgi_header_data)->varName), dataTypeSizeInBits, numElements);
        XNZ_SGI_HEADER_FIELDS
#undef  XNZ_SGI_HEADER_FIELD

}

static void xnz_sgi_header_print(void* sgi_header_data)
{
    char formatString[256];
    char catString[256];

#define XNZ_SGI_HEADER_FIELD(xnz_sgi_struct, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
        sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
        sprintf(catString, ", %s", stringFormat); \
        xnz_sgi_field_print(formatString, catString, #varName, &(((xnz_sgi_struct *)sgi_header_data)->varName), dataTypeSizeInBits, numElements, strncmp(#datatype, "float", 5) == 0 || strncmp(#datatype, "double", 6) == 0 );
        XNZ_SGI_HEADER_FIELDS
#undef XNZ_SGI_HEADER_FIELD

}


//#pragma mark -- SGI ARCHIVE DEFINITION 

typedef struct XNZ_SGI_IMAGE
{
    union {
        struct {
            union{
                XNZ_SGI_HEADER*        header;
                void*                  buffer;
            };
            size_t size;
        };
        CRFile file;
    };
    
    //xng_image            image;
    
    uint32_t*            offsetTable;  //rle offset table of size tablen
    uint32_t*            lengthTable;  //rle length table of size tablen
 
    void*                data;          //encoded or raw scanline data
    
    uint32_t             tablen;

    ///////////////////////////////////

    uint32_t            rowPitch;
    uint32_t            nScanlines;

    uint8_t             depth;
    uint8_t             nSrcChannels;
    uint8_t             nDstChannels;

}XNZ_SGI_IMAGE;




XNZ_SGI_API XNZ_SGI_INLINE void xnz_sgi_open(XNZ_SGI_IMAGE* archive, char* sgiFilePath)
{
    //1 OPEN THE FILE FOR READING AND GET FILESIZE USING LSEEK
    archive->file.fd   = cr_file_open(sgiFilePath);
    archive->file.size = cr_file_size(archive->file.fd);

    fprintf(stderr, "\nxng_sgi_open::PNG File Size =  %lu bytes\n", archive->file.size);

    //2 MAP THE FILE TO BUFFER FOR READING
#ifndef _WIN32
    archive->file.buffer = cr_file_map_to_buffer(&(archive->file.buffer), archive->file.size, PROT_READ, MAP_SHARED | MAP_NORESERVE, archive->file.fd, 0);
    if (madvise(archive->file.buffer, (size_t)archive->file.size, MADV_SEQUENTIAL | MADV_WILLNEED) == -1) {
        printf("\nread madvise failed\n");
    }
#else
    archive->file.mFile = cr_file_map_to_buffer(&(archive->file.buffer), archive->file.size, PROT_READ, MAP_SHARED | MAP_NORESERVE, archive->file.fd, 0);
#endif

    archive->file.path = sgiFilePath;


    //3  READ THE PNG (.png) file signature
    //archive->HDR = (XNZ_SGI_HEADER*)archive->file.buffer;

    //rgb file is in Big Endian byte order.  stbi functions will get in the requested format and convert to host format
    //if ((int)stbi__get8(s) != 1 || (int)stbi__get8(s) != 218) { printf("\nNot RGB Header\n"); return stbi__errpuc("not RGB", "Corrupt RGB"); }
    assert(archive->header->magicNumber[0] == 1);
    assert(archive->header->magicNumber[1] == 218);


    //serailzie sgi header
    //memcpy(&(archive->info), archive->header, sizeof(XNZ_SGI_HEADER));
    //xnz_sgi_header_serialize(&(archive->info));
    //xnz_sgi_header_print(&(archive->info));

    //DIMENSION - describes the number of dimensions in the data stored in the image file.  The only allowed values are 1, 2, or 3. 
    //If this value is 1, the image file consists of only 1 channel and only 1 scanline(row).  The length of this scanline is given by the value of XSIZE below. 
    //If this value is 2, the file consists of a single channel with a number of scanlines.    The width and height of the image are given by the values of XSIZE and YSIZE below.
    //If this value is 3, the file consists of a number of channels.                           The width and height of the image are given by the values of XSIZE and YSIZE below.  The number of channels is given by the value of ZSIZE below.
    uint16_t dimension    = cr_be16toh(archive->header->dimension);
    archive->rowPitch     = cr_be16toh(archive->header->xsize);                         //XSIZE
    archive->nScanlines   = (dimension >= 2) ? cr_be16toh(archive->header->ysize) : 1;  //YSIZE
    archive->nSrcChannels = (dimension == 3) ? cr_be16toh(archive->header->nsize) : 1;  //ZSIZE
    archive->depth        = 8 * archive->header->bpc;                                   //BPC
    archive->tablen       = 0;                                                          //rle table(s) size

    if( archive->header->rle == 1 )
    {
        uint32_t   tableIndex     = 0;
        uint32_t * bufferPosition = (uint32_t*)(archive->file.buffer + 512); //lengh of header + dummy bytes

        //length of each table is determined by img.height * img.numChannels
        archive->tablen = archive->nScanlines * archive->nSrcChannels;// *sizeof(uint32_t);
        
        //allocate memory to store table data, must free memory when finished file load
        archive->offsetTable = (uint32_t*)malloc(archive->tablen * sizeof(uint32_t));
        archive->lengthTable = (uint32_t*)malloc(archive->tablen * sizeof(uint32_t));

        //read the two tables containing the offset
        //read the table entry by entry as big endian, converting to host format (little endian)
        for( tableIndex = 0; tableIndex < archive->tablen; tableIndex++)
        {
            archive->offsetTable[tableIndex] = cr_be32toh(*bufferPosition); bufferPosition++;
            //printf("\n offset[%u]:  %u", tableIndex, info->offsetTable[tableIndex]);
        }
        
        for( tableIndex = 0; tableIndex < archive->tablen; tableIndex++)
        {
            archive->lengthTable[tableIndex] = cr_be32toh(*bufferPosition); bufferPosition++;
            //printf("\n length[%u]:  %u", tableIndex, info->offsetTable[tableIndex]);
        }
        
        /*
        for( channelIndex = 0; channelIndex<s->img_n; channelIndex++)
        {
            //read the file offset table for the scanlines
            for(tableIndex = channelIndex * info->tablen; tableIndex<info->tablen*(channelIndex+1); tableIndex++ )
            {
                info->offsetTable[tableIndex] = stbi__get64be(s);
                printf("\n offset[%lu]:  %lu", tableIndex, info->offsetTable[tableIndex]);
            }
        }
        
        for( channelIndex = 0; channelIndex<s->img_n; channelIndex++)
        {
            //read the table containing length in bytes of each scanline
            for(tableIndex = channelIndex * info->tablen; tableIndex<info->tablen*(channelIndex+1); tableIndex++ )
            {
                info->lengthTable[tableIndex] = stbi__get64be(s);
                printf("\n length[%lu]:  %lu", tableIndex, info->offsetTable[tableIndex]);
            }
        }
         */

    }
    
    //record pointer to start of scanline data
    archive->data = archive->file.buffer + 512 + archive->tablen * 2 * sizeof(uint32_t);
}



static void xnz_sgi_read_rle_scanline(XNZ_SGI_IMAGE *archive, uint8_t* tmp, uint8_t* dst_buf, int scanlineIndex, int channelIndex)
{
    unsigned char *iPtr, *oPtr, pixel;
    int count;
    
    unsigned long tableIndex, fileOffset, rleScanlineLength;
    
    //retrieve the file offset of the channel/row index scanline from the table
    tableIndex        = scanlineIndex + channelIndex * archive->nScanlines;
    fileOffset        = archive->offsetTable[tableIndex];
    rleScanlineLength = archive->lengthTable[tableIndex];
    
    //seek to the file offset of the scanline we need
    //fseek((FILE*)(s->io_user_data), fileOffset, SEEK_SET);
    uint8_t* scanlinePosition = archive->file.buffer + fileOffset;

    //read the rle scanline entry into a temporary buffer which must be preallocated to a large enough size
    //fread(tmp, (unsigned int)rleScanlineLength, 1, (FILE*)(s->io_user_data));
    memcpy(tmp, scanlinePosition, (uint32_t)rleScanlineLength);

    iPtr = tmp;
    oPtr = dst_buf;
    for (;;) {
        pixel = *iPtr++;
        count = (int)(pixel & 0x7F);
        if (!count) {
            return;
        }
        if (pixel & 0x80) {
            while (count--) {
                *oPtr++ = *iPtr++;
            }
        } else {
            pixel = *iPtr++;
            while (count--) {
                *oPtr++ = pixel;
            }
        }
    }
}


XNZ_PNG_API XNZ_PNG_INLINE size_t xnz_sgi_decode_scanlines(XNZ_SGI_IMAGE* archive, uint8_t* dst_buf, size_t dst_size)
{
    int i, j;
    uint32_t  scanlineWidth  = archive->rowPitch * archive->header->bpc;
    uint32_t  scanlineIndex  = 0;
    uint32_t  bufferSize     = 0;
    uint32_t  bufferIndex    = 0;

    uint8_t*  bufferPosition = (uint8_t*)archive->data;
    uint8_t** channelBuffers = (uint8_t**)malloc(sizeof(uint8_t*) * archive->nSrcChannels);

    if( archive->header->rle == 1 ) //scanline rle unpack
    {
        //allocate memory for one scanline per channel
        bufferSize = scanlineWidth; //size of single scanline
        for(i=0; i<archive->nSrcChannels; i++) channelBuffers[i] = (uint8_t*)malloc(bufferSize);

        //create a temporary buffer of row size
        uint8_t* tmp = (uint8_t*)malloc(scanlineWidth * 256);

        //iterate over each scanline of the target buffer (i.e. the height/number of rows of the image)
        for (scanlineIndex=0; scanlineIndex <archive->nScanlines; scanlineIndex++)
        {
            //read the image bytes as a contiguous chunk for each channel
            for(i=0; i<archive->nSrcChannels; i++) xnz_sgi_read_rle_scanline(archive, tmp, channelBuffers[i], scanlineIndex, i);
            
            //copy the channel buffers to the target buffer
            //TO DO: unwrap double for loop
            for (bufferIndex=0; bufferIndex<bufferSize; bufferIndex++)
            {
                for(j=0;j<archive->nDstChannels;j++)
                {
                    //if channel index is less than num target channels and num img channels then we are guaranteed to have read that channel from file
                    
                    //assume the num target buffer channels is always greater than or equal to the number of image channels
                    //so, if the channel buffers exist, we can copy its values to the target buffer
                    if(archive->nDstChannels > archive->nSrcChannels)
                    {
                        if( archive->nSrcChannels == 1) //case:  1 input channel
                        {
                            if( j == 3 ) dst_buf[j] = 0xff;
                            else         dst_buf[j] = channelBuffers[0][bufferIndex];
                        }
                        else if(archive->nSrcChannels == 2) //case: 2 input channels, consider them to be a grayscale value and an alpha
                        {
                            if( j == 3 ) dst_buf[j] = channelBuffers[1][bufferIndex];
                            else         dst_buf[j] = channelBuffers[0][bufferIndex];
                        }
                        else if(archive->nSrcChannels == 3 ) //case: 3 input channels, consider them to be rgb
                        {
                            if (j == 3) dst_buf[j] = 0xff;
                            else        dst_buf[j] = channelBuffers[j][bufferIndex];
                            
                        }
                    }
                    else //if( channelBuffers[j] )
                    {
                        //if( j==3 )          //case: 3 input channels, consider them to be rgb
                        //    outPtr[j] = 0xff;
                        //else
                            dst_buf[j] = channelBuffers[j][bufferIndex];
                    }
                }
                
                dst_buf += archive->nDstChannels;

            }
        }
                
        //cleanup intermediate file data
        if( tmp ) free(tmp);

    }
    else
    {
        //allocate memory to read the raw channel bytes from the image
        bufferSize = scanlineWidth * archive->nScanlines; //size of single channel image
        for(i=0; i<archive->nSrcChannels; i++) channelBuffers[i] = (uint8_t*)malloc(bufferSize);
  
        //read the image bytes as a contiguous chunk for each channel
        for(i=0; i<archive->nSrcChannels; i++) { memcpy(channelBuffers[i], bufferPosition, bufferSize); bufferPosition += bufferSize; }
        
        //copy the channel buffers to the target buffer
        //TO DO: unwrap double for loop
        for (bufferIndex=0; bufferIndex<bufferSize; bufferIndex++)
        {
            for(j=0;j<archive->nDstChannels;j++)
            {
                //if channel index is less than num target channels and num img channels
                //then we are guaranteed to have read that channel from file
                
                //assume the num target buffer channels is always greater than or equal to the number of image channels
                //so, if the channel buffers exist, we can copy its values to the target buffer
                if(archive->nDstChannels > archive->nSrcChannels)
                {
                    if(archive->nSrcChannels == 1) //case:  1 input channel
                    {
                        if( j == 3 ) dst_buf[j] = 0xff;
                        else         dst_buf[j] = channelBuffers[0][bufferIndex];
                        
                    }
                    else if(archive->nSrcChannels == 2) //case: 2 input channels, consider them to be a grayscale value and an alpha
                    {
                        if( j == 3 ) dst_buf[j] = channelBuffers[1][bufferIndex];
                        else         dst_buf[j] = channelBuffers[0][bufferIndex];
                    }
                    else if(archive->nSrcChannels == 3 ) //case: 3 input channels, consider them to be rgb
                    {
                        if( j==3 ) dst_buf[j] = 0xff;
                        else       dst_buf[j] = channelBuffers[j][bufferIndex];
                    }
                }
                else //if( channelBuffers[j] )
                    dst_buf[j] = channelBuffers[j][bufferIndex];
                
            }
            
            dst_buf += archive->nDstChannels;

        }

    }
    
    /*
    for (rowIndex=0; rowIndex<(unsigned int)s->img_y; rowIndex++)
    {

        //assimilate channel buffers into target buffer
        for(i = 0; i<rowWidth; i++)
        {
            for(j=0;j<target;j++)
            {
                //if channel index is less than num target channels and num img channels
                //then we are guaranteed to have read that channel from file
                
                //assume the num target buffer channels is always greater than or equal to the number of image channels
                //so, if the channel buffers exist, we can copy its values to the target buffer
                if( target > s->img_n)
                {
                    if( s->img_n == 1) //case:  1 input channel
                    {
                        if( j == 3 )
                            outPtr[j] = 0xff;
                        else
                            outPtr[j] = channelBuffers[0][i];
                        
                    }
                    else if( s->img_n == 2) //case: 2 input channels, consider them to be a grayscale value and an alpha
                    {
                        if( j == 3 )
                            outPtr[j] = channelBuffers[1][i];
                        else
                            outPtr[j] = channelBuffers[0][i];
                    }
                    else if( s->img_n == 3 )
                    {
                        if( j==3 )          //case: 3 input channels, consider them to be rgb
                            outPtr[j] = 0xff;
                        else
                            outPtr[j] = channelBuffers[j][i];

                    }
                }
                else //if( channelBuffers[j] )
                    outPtr[j] = channelBuffers[j][i];
                
            }
            
            outPtr += target;
        }
        
        //outPtr += rowWidth;

    }
    */
    
    //free the channel buffers for use in next loop iteration
    for(i=0; i<archive->nSrcChannels; i++)
    {
        free(channelBuffers[i]);
        channelBuffers[i] = NULL;
    }

    free(channelBuffers);
    channelBuffers = NULL;
    
    return scanlineIndex;
}





XNZ_SGI_API XNZ_SGI_INLINE void xnz_sgi_close(XNZ_SGI_IMAGE* archive)
{
    if (archive->lengthTable) free(archive->lengthTable);
    if (archive->offsetTable) free(archive->offsetTable);

    archive->lengthTable = NULL;
    archive->offsetTable = NULL;

    CRFileClose(&(archive->file));

    //xnz_file_unmap(crate->file.mFile, crate->file.buffer);
    //xnz_file_close(crate->file.fd);
}






#endif
