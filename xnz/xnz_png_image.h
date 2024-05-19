#ifndef _XNZ_PNG_IMAGE_H_
#define _XNZ_PNG_IMAGE_H_

#include "crSystem/cr_file.h" //CRFile
//#include "stdint.h"           //uint types

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
#ifndef XNZ_PNG_API
#define XNZ_PNG_API static
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNZ_PNG_INLINE
#ifdef _WIN32
#define XNZ_PNG_INLINE __inline
#else
#define XNZ_PNG_INLINE
#endif
#endif

//__decspec doesn't exist in C89, __declspec is MSVC specific
#ifndef XNZ_PNG_DECLSPEC
#ifdef _WIN32
#define XNZ_PNG_DECLSPEC __declspec
#else
#define XNZ_PNG_DECLSPEC
#endif
#endif

//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNZ_PNG_ALIGN
#ifdef _WIN32
#define XNZ_PNG_ALIGN(X) 
#else
#define XNZ_PNG_ALIGN(X) __attribute__ ((aligned(X)))
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


typedef enum PNG_COLOR_TYPE
{
    Greyscale      = 0,
    TrueColor      = 2,
    IndexedColor   = 3,
    GreyscaleAlpha = 4,
    TrueColorAlpha = 6
}PNG_COLOR_TYPE;

typedef enum PNG_FILTER_TYPE
{
    PNGFilterNone      = 0,
    PNGFilterSub       = 1,
    PNGFilterUp        = 2,
    PNGFilterAverage   = 3,
    PNGFilterPaeth     = 4,
    PNGFilterCount     = 5
}PNG_FILTER_TYPE;

//#pragma mark -- CTConnection API Method Function Pointer Definitions
typedef void (*png_filter_func)(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline);

//forward declare png filter functions
static void xnz_png_filter_noop(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline) { return; }
static void xnz_png_filter_sub(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline);
static void xnz_png_filter_up(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline);
static void xnz_png_filter_avg(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline);
static void xnz_png_filter_paeth(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline);

static const png_filter_func XNZ_PNG_FILTERS[PNGFilterCount] =
{
    xnz_png_filter_noop,
    xnz_png_filter_sub,
    xnz_png_filter_up,
    xnz_png_filter_avg,
    xnz_png_filter_paeth
};

/*
 *  Length --     A 4-byte unsigned integer giving the number of bytes in the chunk's data field. The length counts only the data field, not itself, the chunk type code, or the CRC. Zero is a valid length.
 *                Although encoders and decoders should treat the length as unsigned, its value must not exceed 231 bytes.
 *  Chunk Type -- A 4-byte chunk type code. For convenience in description and in examining PNG files, type codes are restricted to consist of uppercase and lowercase ASCII letters (A-Z and a-z, or 65-90 and
 *                97-122 decimal). However, encoders and decoders must treat the codes as fixed binary values, not character strings. For example, it would not be correct to represent the type code IDAT by the
 *                EBCDIC equivalents of those letters. Additional naming conventions for chunk types are discussed in the next section.
 *  Chunk Data -- The data bytes appropriate to the chunk type, if any. This field can be of zero length.
 *  CRC   --      A 4-byte CRC (Cyclic Redundancy Check) calculated on the preceding bytes in the chunk, including the chunk type code and chunk data fields, but not including the length field. The CRC is always
 *                present, even for chunks containing no data. See CRC algorithm.
 */


#ifdef _WIN32
#pragma pack(push, 1)
#endif

XNZ_PNG_ALIGN(4) typedef struct CR_PACK_ATTRIBUTE xnz_png_chunk
{
    uint32_t    length;
    uint32_t    type;
    //uint32_t    data;
    //uint32_t    crc;
}xnz_png_chunk;

/***
 ***    IHDR STRUCT (OPCODE 'IHDR')
 ***/
//#pragma mark -- IHDR RECORD (OPCODE 'IHDR')

#define XNZ_PNG_IHDR_FIELDS \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, uint8_t,  8,  8, magicNumber, magicNumber[8], "%s") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, uint32_t, 32, 1, length, length, "%d") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, uint32_t, 32, 1, type, type, "%d") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, uint32_t, 32, 1, width, width, "%d") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, uint32_t, 32, 1, height, height, "%d") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, int8_t,   8,  1, depth, depth, "%u") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, int8_t,   8,  1, color, color, "%u") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, int8_t,   8,  1, compression, compression, "%u") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, int8_t,   8,  1, filter, filter, "%u") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, int8_t,   8,  1, interlace, interlace, "%u") \
XNZ_PNG_IHDR_FIELD(XNZ_PNG_IHDR, uint32_t, 32, 1, crc, crc, "%u")

//--- define the structure, the X macro will be expanded once per field
XNZ_PNG_ALIGN(1) typedef struct CR_PACK_ATTRIBUTE XNZ_PNG_IHDR
{
//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNZ_PNG_IHDR_FIELD(xnz_flt_record_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
    XNZ_PNG_IHDR_FIELDS
#undef XNZ_PNG_IHDR_FIELD
} XNZ_PNG_IHDR;

/*
typedef struct xnz_png_signature
{
    unsigned char  magicNumber[8];
}xnz_png_signature;
*/

static const uint32_t PNG_IHDR_CHUNK_TYPE = 'IHDR';
static const uint32_t PNG_PLTE_CHUNK_TYPE = 'PLTE';
static const uint32_t PNG_sPLT_CHUNK_TYPE = 'sPLT';
static const uint32_t PNG_pHYs_CHUNK_TYPE = 'pHYs';

//ColorSpace Chunks
static const uint32_t PNG_cHRM_CHUNK_TYPE = 'cHRM';
static const uint32_t PNG_gAMA_CHUNK_TYPE = 'gAMA';
static const uint32_t PNG_iCCP_CHUNK_TYPE = 'iCCP';
static const uint32_t PNG_sRGB_CHUNK_TYPE = 'sRGB';

static const uint32_t PNG_IDAT_CHUNK_TYPE = 'IDAT';
static const uint32_t PNG_IEND_CHUNK_TYPE = 'IEND';


XNZ_PNG_ALIGN(1) typedef struct CR_PACK_ATTRIBUTE XNZ_PNG_PLTE
{
    uint32_t       length;
    uint32_t       type;
    xnz_byte3  *    rgb;
}XNZ_PNG_PLTE;

XNZ_PNG_ALIGN(1) typedef struct CR_PACK_ATTRIBUTE XNZ_PNG_pHYs
{
    uint32_t       length;
    uint32_t       type;
    uint32_t       ppx;
    uint32_t       ppy;
    uint32_t       unit;
    uint32_t       crc;
    
}XNZ_PNG_pHYs;


XNZ_PNG_ALIGN(1) typedef struct CR_PACK_ATTRIBUTE XNZ_PNG_cHRM
{
    uint32_t       length;
    uint32_t       type;
    uint32_t       wx, wy;
    uint32_t       rx, ry;
    uint32_t       gx, gy;
    uint32_t       bx, by;
    uint32_t       crc;
    
}XNZ_PNG_cHRM;


XNZ_PNG_ALIGN(1) typedef struct CR_PACK_ATTRIBUTE XNZ_PNG_IDAT
{
    uint32_t       length;
    uint32_t       type;
    uint16_t       blob;
}XNZ_PNG_IDAT;



typedef struct XNZ_PNG_IMAGE
{
    union {
        struct {
            union{
                XNZ_PNG_IHDR*          IHDR;
                void*                  buffer;
            };
            size_t size;
        };
        CRFile file;
    };
    
    xng_image            image;
    
    //xnz_png_chunk*       chunks;
    
    //Pallette Chunk(s)
    XNZ_PNG_PLTE *       PLTE;
    
    //Physical Dimension Chunk
    XNZ_PNG_pHYs *       pHYs;
    
    //Color Space Chunks
    XNZ_PNG_cHRM  *       cHRM;
    xnz_png_chunk *       gAMA;
    xnz_png_chunk *       iCCP;
    xnz_png_chunk *       sRGB;

    XNZ_PNG_IDAT *       IDAT;

    uint32_t            blobSize;
    uint32_t            nBlobChunks;   //Num IDAT Chunks
    
    uint32_t            nChunks;

    uint32_t            rowPitch;
    uint32_t            nScanlines;

    uint8_t             depth;
    uint8_t             nSrcChannels;
    uint8_t             nDstChannels;


}XNZ_PNG_IMAGE;

#ifdef _WIN32
#pragma pack(pop)   //end struct single byte packing
#endif

XNZ_PNG_API XNZ_PNG_INLINE void   xnz_png_open(XNZ_PNG_IMAGE* archive, char* pngFilePath);
XNZ_PNG_API XNZ_PNG_INLINE size_t xnz_png_inflate_blobs(XNZ_PNG_IMAGE* archive, uint8_t* dst_buf, size_t dst_size);
XNZ_PNG_API XNZ_PNG_INLINE void   xnz_png_filter(uint8_t* dst_buf, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels);
XNZ_PNG_API XNZ_PNG_INLINE void   xnz_png_close(XNZ_PNG_IMAGE* archive);

//#define _USE_MATH_DEFINES // for C
//#include <math.h>

#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */
#include <assert.h>

#ifdef __APPLE__
#include <compression.h>
#elif defined(_WIN32)
#include <malloc.h>     /* alloca */
//#pragma comment (lib, "Cabinet.lib")
//#include <compressapi.h>
#endif

#include "xnz.h"

/***
 ***    UNIVERSAL SERIALIZE FUNCTION FOR ALL RECORDS DATA FIELDS PARSED BY X-MACROS
 ***
 ***    Converts FLT Record Big Endian Data Types to host format (e.g. Intel = Little Endian) using system level conversion calls
 ***/
static void xnz_png_chunk_field_serialize(void* data, unsigned int elementSizeInBits, unsigned int numElements)
{
    int i;
    //if the data type is 8 bits in size, such as char, then there is no need to convert it
    //because endianness only affects byte order and char is a single byte data type
    if (elementSizeInBits == 8)
        return;

    if (elementSizeInBits == 64)
    {
        uint64_t* doubleData = (uint64_t*)data;
        *doubleData = xnz_be64toh(*doubleData);
        for (i = 1; i < numElements; i++)
        {
            doubleData[i] = xnz_be64toh(doubleData[i]);
        }

    }
    else if (elementSizeInBits == 32)
    {
        uint32_t* intData = (uint32_t*)data;
        *intData = xnz_be32toh(*intData);
        for (i = 1; i < numElements; i++)
        {
            intData[i] = xnz_be32toh(intData[i]);
        }

    }
    else if (elementSizeInBits == 16)
    {
        uint16_t* shortData = (uint16_t*)data;
        *shortData = xnz_be16toh(*shortData);
        for (i = 1; i < numElements; i++)
        {
            shortData[i] = xnz_be16toh(shortData[i]);
        }
    }
}


//Serialize specific record types;  X-MACROS are used to iterate over each field in each flt record struct with minimal code
static void xnz_png_ihdr_chunk_serialize(void* png_chunk_data)
{
#define XNZ_PNG_IHDR_FIELD(xnz_png_chunk_struct, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    xnz_png_chunk_field_serialize(&(((xnz_png_chunk_struct *)png_chunk_data)->varName), dataTypeSizeInBits, numElements); \
    XNZ_PNG_IHDR_FIELDS
#undef XNZ_PNG_IHDR_FIELD
}

static uint8_t history[XNZ_DEFLATE_BUFFER_SIZE];

XNZ_PNG_API XNZ_PNG_INLINE size_t xnz_png_inflate_blobs(XNZ_PNG_IMAGE* archive, uint8_t* dst_buf, size_t dst_size)
{
    size_t nDecodedBytes = 0;
    size_t nBlobsProcessed = 0;
    //size_t nCompressedBytes = 0;

    //create a RFC 1951 DEFLATE history buffer as per spec of size 32768
    //uint8_t* history = malloc(XNZ_DEFLATE_BUFFER_SIZE);

    //src_size is the length of the first IDAT blob chunk sans zlib header (and maybe sans trailer)
    uint8_t* src_buf = (uint8_t*)(&(archive->IDAT->blob)) + sizeof(uint16_t); //push past zlib header
    size_t   src_size = xnz_be32toh(archive->IDAT->length) - sizeof(uint16_t); //subtract length of zlib header

#if __APPLE__
    if (archive->nBlobChunks == 1) //use buffer style api oneshot decompression
    {
        nDecodedBytes = compression_decode_buffer(dst_buf, dst_size, src_buf, src_size, NULL, COMPRESSION_ZLIB);
        assert(nDecodedBytes > 0);
        assert(nDecodedBytes == dst_size);
    }
    else
    {
        //create a compression stream for decoding chunks
        compression_stream stream = { 0 };
        compression_status status = compression_stream_init(&stream, COMPRESSION_STREAM_DECODE, COMPRESSION_ZLIB);
        assert(status != COMPRESSION_STATUS_ERROR);

        stream.src_ptr = src_buf;
        stream.src_size = src_size;

        stream.dst_ptr = dst_buf;
        stream.dst_size = dst_size;

        //decode first IDAT blob chunk
        status = compression_stream_process(&stream, 0);
        assert(status != COMPRESSION_STATUS_ERROR);

        printf("Blob Compressed Bytes Read = %ld\n", stream.src_ptr - (uint8_t*)&archive->IDAT->blob);
        printf("Blob Bytes Decoded = %ld\n", (stream.dst_ptr - (dst_buf + nDecodedBytes)));

        nDecodedBytes += (stream.dst_ptr - (dst_buf + nDecodedBytes));

        uint8_t* bufferPosition = (uint8_t*)archive->IDAT + sizeof(xnz_png_chunk) + xnz_be32toh(archive->IDAT->length) + 4;
        int i; for (i = 1; i != archive->nBlobChunks; ++i)
        {
            XNZ_PNG_IDAT* chunk = (XNZ_PNG_IDAT*)bufferPosition;

            stream.src_ptr = (uint8_t*)&chunk->blob;
            stream.src_size = xnz_be32toh(chunk->length);
            stream.dst_ptr = dst_buf + nDecodedBytes;
            stream.dst_size = dst_size - nDecodedBytes;

            printf("Blob Chunk Length = %d\n", xnz_be32toh(chunk->length));

            status = compression_stream_process(&stream, i == archive->nBlobChunks - 1 ? COMPRESSION_STREAM_FINALIZE : 0);
            assert(status != COMPRESSION_STATUS_ERROR);

            printf("Blob Compressed Bytes Read = %ld\n", stream.src_ptr - (uint8_t*)&chunk->blob);
            printf("Blob Bytes Decoded = %ld\n", (stream.dst_ptr - (dst_buf + nDecodedBytes)));

            nDecodedBytes += (stream.dst_ptr - (dst_buf + nDecodedBytes));
            //if( i == archive->nBlobChunks -1 ) assert(1==0);
            bufferPosition += sizeof(xnz_png_chunk) + xnz_be32toh(chunk->length) + 4;
        }

        printf("nDecodedBytes = %lu\n", nDecodedBytes);
        printf("remaining bytes expected = %lu\n", dst_size - nDecodedBytes);

        //Appropriately formed streams may still need to be pumped to decode final bytes ...
        while (status != COMPRESSION_STATUS_END)
        {
            status = compression_stream_process(&stream, COMPRESSION_STREAM_FINALIZE);
            assert(status != COMPRESSION_STATUS_ERROR);

            printf("Blob Bytes Decoded = %ld\n", (stream.dst_ptr - (dst_buf + nDecodedBytes)));
            nDecodedBytes += (stream.dst_ptr - (dst_buf + nDecodedBytes));

        }

        assert(status == COMPRESSION_STATUS_END);
        //assert( nDecodedBytes == dst_size ); //we know we need exactly (w+1) * h * depth/8 * nChannels bytes for png filtering

        status = compression_stream_destroy(&stream);
        assert(status != COMPRESSION_STATUS_ERROR);

    }
#else
    if (archive->nBlobChunks == 1)
    {
        //XNZ oneshot buffer api mimics XNU oneshot buffer api decompression style 
        nDecodedBytes = xnz_inflate_buffer(src_buf, src_size, dst_buf, dst_size, history);
        assert(nDecodedBytes > 0);
        assert(nDecodedBytes == dst_size);
    }
    else
    {
        //XNZ stream api mimics XNU stream api decompression style
        xnz_compression_stream stream = { 0 };
        xnz_status             status = XNZ_STATUS_ERROR;

        //create a compression stream for decoding a stream of bits in chunks of bytes
        memset(&stream, 0, sizeof(xnz_compression_stream)); //it is important that the internal bitstream values are init'd to zero
        //status = xnz_block_stream_init(&stream, COMPRESSION_STREAM_DECODE, COMPRESSION_ZLIB);
        //assert(status != XNZ_STATUS_ERROR);
        stream.src_ptr  = src_buf;
        stream.src_size = src_size;
        stream.dst_ptr  = dst_buf;
        stream.dst_size = dst_size;
        stream.history  = history;

        //decode first IDAT blob chunk
        status = xnz_stream_inflate(&stream, 0);
        //assert(status != XNZ_STATUS_ERROR);

        //xnz stream api updates stream.src_ptr to reflect the # of compressed bytes processed
        //fprintf(stderr, "Blob Compressed Bytes Read = %lld\n", stream.src_ptr - src_buf);
        //fprintf(stderr, "Blob Bytes Decoded = %lld\n", (stream.dst_ptr - (dst_buf + nDecodedBytes)));

        nDecodedBytes += (stream.dst_ptr - (dst_buf + nDecodedBytes));
        //fprintf(stderr, "Block[%d] = %d bytes decoded\n", nBlocksProcessed, nBytesDecoded);
        //fprintf(stderr, "Blob Bytes Decoded = %ld\n", (stream.dst_ptr - (dst_buf + nDecodedBytes)));
        nBlobsProcessed += 1;
        //fprintf(stderr, "nDecodedBytes = %zd\n\n", nDecodedBytes);

        //deflate blobs until compressed bytes are exhausted
        uint8_t* bufferPosition = (uint8_t*)archive->IDAT + sizeof(xnz_png_chunk) + xnz_be32toh(archive->IDAT->length) + 4;
        int i; for (i = 1; i != archive->nBlobChunks; i++)
        {
            XNZ_PNG_IDAT* chunk = (XNZ_PNG_IDAT*)bufferPosition;

            stream.src_ptr = (uint8_t*)&chunk->blob;
            stream.src_size = xnz_be32toh(chunk->length);
            stream.dst_ptr = dst_buf + nDecodedBytes;
            stream.dst_size = dst_size - nDecodedBytes;

            //fprintf(stderr, "Blob Chunk Length = %d\n", xnz_be32toh(chunk->length));

            status = xnz_stream_inflate(&stream, i == archive->nBlobChunks - 1 ? XNZ_STREAM_FINALIZE : 0);
            //assert(status != XNZ_STATUS_ERROR);

            //fprintf(stderr, "Blob Compressed Bytes Read = %lld\n", stream.src_ptr - (uint8_t*)&chunk->blob  );
            //fprintf(stderr, "Blob[%zd] Bytes Decoded = %lld\n\n", nBlobsProcessed, (stream.dst_ptr - (dst_buf + nDecodedBytes))  );

            nDecodedBytes += (stream.dst_ptr - (dst_buf + nDecodedBytes));
            bufferPosition += sizeof(xnz_png_chunk) + xnz_be32toh(chunk->length) + 4;
            nBlobsProcessed += 1;
            //fprintf(stderr, "nDecodedBytes = %zu\n", nDecodedBytes);

        }

        //fprintf(stderr, "nDecodedBytes = %zu\n", nDecodedBytes  );
        //fprintf(stderr, "remaining bytes expected = %zu\n", dst_size - nDecodedBytes  );

        //Appropriately formed streams may still need to be pumped to decode final bytes ...
        while (status != XNZ_STATUS_END)
        {
            stream.dst_ptr = dst_buf + nDecodedBytes;
            stream.dst_size = dst_size - nDecodedBytes;

            status = xnz_stream_inflate(&stream, XNZ_STREAM_FINALIZE);
            //assert(status != XNZ_STATUS_ERROR);

            //fprintf(stderr, "Blob Bytes Decoded = %lld\n", (stream.dst_ptr - (dst_buf + nDecodedBytes)));
            nDecodedBytes += (stream.dst_ptr - (dst_buf + nDecodedBytes));
        }

        assert(status == XNZ_STATUS_END);
        //assert( nDecodedBytes == dst_size ); //we know we need exactly (w+1) * h * depth/8 * nChannels bytes for png filtering

        //status = xnz_stream_destroy(&stream);
        //assert(status != XNZ_STATUS_ERROR);

    }
#endif

    //free(history);

    return nDecodedBytes;
}


static void xnz_png_filter_sub(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline)
{
    // Filt(x)  = Unfilt(x) - Unfilt(a)
    // Unfilt(x) = Filt(x) + Unfilt(a)

    uint32_t byteIndex = 0;  //iterate each byte in the scanline
    //For all filters, the bytes "to the left of" the first pixel in a scanline shall be treated as being zero.
    memset(aPixel, 0, nChannels * depth / 8);
    for (byteIndex = 0; byteIndex < rowPitch * nChannels; byteIndex++)
    {
        uint8_t FiltX   = scanline[byteIndex];            //get   Filt(x)
        uint8_t UnfiltA = aPixel[byteIndex % nChannels];  //get   Unfilt(a)
        uint8_t UnfiltX = FiltX + UnfiltA;                //calc  Unfilt(x)
        scanline[byteIndex]           = UnfiltX;          //set   Unfilt(x)
        aPixel[byteIndex % nChannels] = UnfiltX;          //store Unfilt(a)
    }
}


static void xnz_png_filter_up(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline)
{
    // Filt(x)   = Unfilt(x) - Unfilt(b)
    // Unfilt(x) = Filt(x) + Unfilt(b)

    uint32_t byteIndex = 0;  //iterate each byte in the scanline
    //For all filters, the bytes "to the left of" the first pixel in a scanline shall be treated as being zero.
    //For filters that refer to the prior scanline, the entire prior scanline and bytes "to the left of" the first pixel
    //in the prior scanline shall be treated as being zeroes for the first scanline of a reduced image.
    memset(aPixel, 0, nChannels * depth / 8); //assert(prevScanline);
    for (byteIndex = 0; byteIndex < rowPitch * nChannels; byteIndex++)
    {
        uint8_t FiltX   = scanline[byteIndex];          //get   Filt(x)
      //uint8_t UnfiltA = aPixel[byteIndex%nChannels];  //get   Unfilt(a)
        uint8_t UnfiltB = prevScanline[byteIndex];      //get   Unfilt(b)
        uint8_t UnfiltX = FiltX + UnfiltB;              //calc  Unfilt(x)
        scanline[byteIndex] = UnfiltX;                  //set   Unfilt(x)
      //aPixel[byteIndex % nChannels] = UnfiltX;        //store Unfilt(a)
    }
}



static void xnz_png_filter_avg(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline)
{
    // Filt(x)   = Orig(x) - floor((Orig(a) + Orig(b)) / 2)
    // Unfilt(x) = Filt(x) + floor((Unfilt(a) + Unfilt(b)) / 2)

    uint32_t byteIndex = 0;  //iterate each byte in the scanline
    //For all filters, the bytes "to the left of" the first pixel in a scanline shall be treated as being zero.
    //For filters that refer to the prior scanline, the entire prior scanline and bytes "to the left of" the first pixel
    //in the prior scanline shall be treated as being zeroes for the first scanline of a reduced image.
    memset(aPixel, 0, nChannels * depth / 8); //assert(prevScanline);
    for (byteIndex = 0; byteIndex < rowPitch * nChannels; byteIndex++)
    {
        uint8_t FiltX   = scanline[byteIndex];                                               //get   Filt(x)
        uint8_t UnfiltA = aPixel[byteIndex % nChannels];                                     //get   Unfilt(a)
        uint8_t UnfiltB = prevScanline[byteIndex];                                           //get   Unfilt(b)
        uint8_t UnfiltX = FiltX + (uint8_t)floorf(((float)UnfiltA + (float)UnfiltB) / 2.f);  //calc  Unfilt(x)
        scanline[byteIndex]           = UnfiltX;                                             //set   Unfilt(x)
        aPixel[byteIndex % nChannels] = UnfiltX;                                             //store Unfilt(a)
    }
}

/*
static uint16_t PaethPredictor(uint8_t a, uint8_t b, uint8_t c)
{
    uint8_t Pr;
    int16_t p = (int16_t)a + (int16_t)b - (int16_t)c;
    int16_t pa = abs(p - (int16_t)a);
    int16_t pb = abs(p - (int16_t)b);
    int16_t pc = abs(p - (int16_t)c);
    if (pa <= pb && pa <= pc) Pr = a;
    else if (pb <= pc) Pr = b;
    else Pr = c;
    return (uint8_t)Pr;
}
*/


//Paeth predictor, used by PNG filter type 4
static uint8_t PaethPredictor(uint8_t a, uint8_t b, uint8_t c)
{
    // the subtractions of unsigned char cast it to a signed type.
    // With gcc, short is faster than int, with clang int is as fast (as of april 2023)
    int16_t pa = (b - c) < 0 ? -(b - c) : (b - c);
    int16_t pb = (a - c) < 0 ? -(a - c) : (a - c);
    // writing it out like this compiles to something faster than introducing a temp variable
    int16_t pc = (a + b - c - c) < 0 ? -(a + b - c - c) : (a + b - c - c);
    // return input value associated with smallest of pa, pb, pc (with certain priority if equal)
    if (pb < pa) { a = b; pa = pb; }
    return (pc < pa) ? c : a;
}


static void xnz_png_filter_paeth(uint8_t* scanline, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels, uint8_t* aPixel, uint8_t* cPixel, uint8_t* prevScanline)
{
    //  Filt(x) = Orig(x) - PaethPredictor(Orig(a), Orig(b), Orig(c))
    //  Unfilt(x) = Filt(x) + PaethPredictor(Unfilt(a), Unfilt(b), Unfilt(c))

    uint32_t byteIndex = 0;  //iterate each byte in the scanline
    //For all filters, the bytes "to the left of" the first pixel in a scanline shall be treated as being zero.
    //For filters that refer to the prior scanline, the entire prior scanline and bytes "to the left of" the first pixel
    //in the prior scanline shall be treated as being zeroes for the first scanline of a reduced image.
    memset(aPixel, 0, nChannels * depth / 8);
    memset(cPixel, 0, nChannels * depth / 8); //assert(prevScanline);
    for (byteIndex = 0; byteIndex < rowPitch * nChannels; byteIndex++)
    {
        uint8_t FiltX   = scanline[byteIndex];              //get   Filt(x)
        uint8_t UnfiltA = aPixel[byteIndex % nChannels];    //get   Unfilt(a)
        uint8_t UnfiltB = prevScanline[byteIndex];          //get   Unfilt(b)
        uint8_t UnfiltC = cPixel[byteIndex % nChannels];    //get   Unfilt(c)

        uint8_t UnfiltX = FiltX + (uint16_t)PaethPredictor(UnfiltA, UnfiltB, UnfiltC);

        scanline[byteIndex]           = UnfiltX;            //set   Unfilt(x)
        aPixel[byteIndex % nChannels] = UnfiltX;            //store Unfilt(a)
        cPixel[byteIndex % nChannels] = UnfiltB;            //store Unfilt(c)
    }
}

//x    the byte being filtered;
//a    the byte corresponding to x in the pixel immediately before the pixel containing x (or the byte immediately before x, when the bit depth is less than 8)
//b    the byte corresponding to x in the previous scanline
//c    the byte corresponding to b in the pixel immediately before the pixel containing b (or the byte immediately before b, when the bit depth is less than 8)

XNZ_PNG_API XNZ_PNG_INLINE void xnz_png_filter(uint8_t* dst_buf, uint32_t rowPitch, uint32_t nScanlines, uint8_t depth, uint8_t nChannels)
{
    uint32_t nChannelBytes        = (uint32_t)depth / 8 * (uint32_t)nChannels;
    uint32_t scanlineIndex        = 0;
    uint8_t* scanlinePosition     = dst_buf;
    uint8_t* prevScanlinePosition = NULL;
    
    uint8_t* aPixel = (uint8_t*)alloca(nChannelBytes); memset(aPixel, 0, nChannelBytes); //store the previous unfiltered pixel corresponding to current pixel at byte x
    uint8_t* cPixel = (uint8_t*)alloca(nChannelBytes); memset(cPixel, 0, nChannelBytes); //store the previous unfiltered pixel corresponding to current pixel at byte x from the previous scanline
    
    //parse first scanline
    uint8_t  filterType = *scanlinePosition; scanlinePosition++;
    XNZ_PNG_FILTERS[filterType](scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, cPixel, prevScanlinePosition); //execute scanline filter

    /*
    uint8_t  filterType = *scanlinePosition; scanlinePosition++;
         if (filterType == PNGFilterNone)    assert(1 == 0);
    else if (filterType == PNGFilterSub)       xnz_png_filter_sub(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, NULL,   NULL);
    else if (filterType == PNGFilterUp)      assert(1 == 0);//xnz_png_filter_up(   scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, NULL, prevScanlinePosition);
    else if (filterType == PNGFilterAverage)   xnz_png_filter_avg(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, NULL,   prevScanlinePosition);
    else if (filterType == PNGFilterPaeth)   xnz_png_filter_paeth(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, cPixel, prevScanlinePosition);
    else assert(1 == 0);
    */

    prevScanlinePosition = dst_buf + (scanlineIndex * rowPitch * nChannelBytes);
    memmove(prevScanlinePosition,  scanlinePosition,  rowPitch * nChannelBytes); //copy unfiltered scanline scanlineIndex bytes to the left
    //if( scanlineIndex < nScanlines /2 ) memset(prevScanlinePosition, 255, rowPitch * depth/8 * nChannels); //debug
    scanlinePosition += rowPitch * nChannelBytes; scanlineIndex++; //move to next scanline
    
    /*
    //parse second scanline
    filterType = *scanlinePosition; scanlinePosition++;
    if( filterType == PNGFilterNone )    assert(1==0);
    else if( filterType == PNGFilterSub )     xnz_png_filter_sub(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel);
    else if( filterType == PNGFilterUp )      xnz_png_filter_up(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, prevScanlinePosition);
    else if( filterType == PNGFilterAverage ) xnz_png_filter_avg(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, prevScanlinePosition);
    else if( filterType == PNGFilterPaeth )   xnz_png_filter_paeth(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, cPixel, prevScanlinePosition);
    else assert(1==0);

    prevScanlinePosition = dst_buf + (scanlineIndex * rowPitch * nChannelBytes);
    memmove(prevScanlinePosition, scanlinePosition, rowPitch * nChannelBytes);   //copy unfiltered scanline scanlineIndex bytes to the left
    //if( scanlineIndex < nScanlines /2 ) memset(prevScanlinePosition, 255, rowPitch * depth/8 * nChannels);
    scanlinePosition += rowPitch * nChannelBytes; //move to next scanline
    scanlineIndex++;
    */

    //parse remaining scanlines
    for (; scanlineIndex < nScanlines; scanlineIndex++)
    {
        filterType = *scanlinePosition; scanlinePosition++; 
        XNZ_PNG_FILTERS[filterType](scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, cPixel, prevScanlinePosition); //execute scanline filter

        /*
             if (filterType == PNGFilterNone)    {}//assert(1==0);
        else if (filterType == PNGFilterSub)     xnz_png_filter_sub(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, NULL,   NULL);
        else if (filterType == PNGFilterUp)       xnz_png_filter_up(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, NULL,   prevScanlinePosition);
        else if (filterType == PNGFilterAverage) xnz_png_filter_avg(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, NULL,   prevScanlinePosition);
        else if (filterType == PNGFilterPaeth) xnz_png_filter_paeth(scanlinePosition, rowPitch, nScanlines, depth, nChannels, aPixel, cPixel, prevScanlinePosition);
        else break;//assert(1 == 0);
        */

        prevScanlinePosition = dst_buf + (scanlineIndex * rowPitch * nChannelBytes);
        memmove(prevScanlinePosition,  scanlinePosition,  rowPitch * nChannelBytes); //copy unfiltered scanline scanlineIndex bytes to the left

        //if( scanlineIndex < nScanlines /2 ) memset(prevScanlinePosition, 255, rowPitch * depth/8 * nChannels); //debug
        //memcpy(cPixel, prevScanlinePosition - nChannelBytes, nChannelBytes);     //store cPixel value right before the start of previous scanline
        scanlinePosition += rowPitch * nChannelBytes; //move to next scanline
    }
    //free(aPixel);
    //free(cPixel);
}


/****
 *  If the iCCP chunk is present, the image samples conform to the colour space represented by the embedded ICC profile as defined by the International Color Consortium [ICC].
 *
 *  The colour space of the ICC profile shall be an RGB colour space for colour images (PNG colour types 2, 3, and 6), or a greyscale colour space for greyscale images (PNG colour types 0 and 4).
 *  A PNG encoder that writes the iCCP chunk is encouraged to also write gAMA and cHRM chunks that approximate the ICC profile, to provide compatibility with applications that do not use the iCCP chunk.
 *  When the iCCP chunk is present, PNG decoders that recognize it and are capable of colour management [ICC] shall ignore the gAMA and cHRM chunks and use the iCCP chunk instead and interpret it
 *  according to [ICC-1] and [ICC-1A]. PNG decoders that are used in an environment that is incapable of full-fledged colour management should use the gAMA and cHRM chunks if present.
 *
 *  A PNG datastream should contain at most one embedded profile, whether specified explicitly with an iCCP chunk or implicitly with an sRGB chunk.
 ***/
XNZ_PNG_API XNZ_PNG_INLINE void xnz_png_inflate_color_profile(XNZ_PNG_IMAGE* archive)
{
    if (archive->iCCP)
    {
        //printf("iCCP Size: %lu\n", sizeof(XNZ_ICC_HEADER));
        assert(sizeof(XNZ_ICC_HEADER) == 128);

        size_t nDecodedBytes = 0;
        XNZ_ICC_HEADER iccHeader = { 0 };
        XNZ_ICC_PROFILE iccProfile;
        iccProfile.header = &iccHeader;

        //Read the PNG ICC struct
        uint8_t* chunkData = (uint8_t*)archive->iCCP + sizeof(xnz_png_chunk);
        size_t nameLen = strlen((const char*)chunkData) + 1;
        fprintf(stderr, "iCCP Name: %s\n", chunkData);
        chunkData += nameLen;

        //Read the PNG ICC Struct Compression Method (will always be 0 == ZLIB inflate)
        uint8_t compressionMethod = *chunkData; chunkData++;
        assert(compressionMethod == 0);

        fprintf(stderr, "ICC ZLIB Blob Header: %hx\n", xnz_be16toh(*(uint16_t*)chunkData));
        uint32_t chunkSize = xnz_be32toh(archive->iCCP->length) - (uint32_t)nameLen - 1; //total compressed ICC Zlib wrapped blob size

        /*
        //assume there is only a single chunk to decompress
        uint8_t * iccBuffer = (uint8_t*)malloc(chunkSize * 2);
        memset(iccBuffer, 0, chunkSize * 2);

        nDecodedBytes = compression_decode_buffer(iccBuffer, chunkSize*2, chunkData + 2, chunkSize - 2, NULL, COMPRESSION_ZLIB);
        assert(nDecodedBytes > 0 );
        //assert(nDecodedBytes == dst_size);

        xnz_icc_header_serialize(iccBuffer);
        xnz_icc_header_print(iccBuffer);
        */

#ifdef __APPLE__
        //create a compression stream for decoding chunks
        compression_stream stream = { 0 };
        compression_status status = compression_stream_init(&stream, COMPRESSION_STREAM_DECODE, COMPRESSION_ZLIB);
        assert(status != COMPRESSION_STATUS_ERROR);

        //configure stream to decompress ICC header
        stream.src_ptr = chunkData + 2;
        stream.src_size = chunkSize - 2;
        stream.dst_ptr = (uint8_t*)iccProfile.header;
        stream.dst_size = sizeof(XNZ_ICC_HEADER);

        //decode blob ICC header chunk
        status = compression_stream_process(&stream, 0);
        assert(status != COMPRESSION_STATUS_ERROR);
        nDecodedBytes += sizeof(XNZ_ICC_HEADER) - stream.dst_size;
        assert(nDecodedBytes == sizeof(XNZ_ICC_HEADER));

        //serialize ICC header to system byte format
        xnz_icc_header_serialize(iccProfile.header);

        //use the ICC header size property to allocate memory for the entire ICC profile
        iccProfile.buffer = (uint8_t*)malloc(iccProfile.header->size);
        memset(iccProfile.buffer, 0, iccHeader.size);
        memcpy(iccProfile.buffer, &iccHeader, sizeof(XNZ_ICC_HEADER));

        xnz_icc_header_print(iccProfile.header); //verify header overwrite was successful

        //configure stream to decompress ICC profile
        stream.src_ptr = chunkData + 2;
        stream.src_size = (chunkSize - 2) - (stream.src_ptr - (chunkData + 2));//chunkSize - 2;
        stream.dst_ptr = iccProfile.buffer + sizeof(XNZ_ICC_HEADER);
        stream.dst_size = iccProfile.header->size - sizeof(XNZ_ICC_HEADER);

        //decode blob ICC profile chunk
        status = compression_stream_process(&stream, 0);
        assert(status != COMPRESSION_STATUS_ERROR);
        
        status = compression_stream_destroy(&stream);
        assert(status != COMPRESSION_STATUS_ERROR);
        
        free(iccProfile.buffer);
#else
        //assert(1 == 0);
        return;
#endif
        //printf("ICC Version = %u.%u.%u.%u\n", iccHeader->version[0], iccHeader->version[1], iccHeader->version[2], iccHeader->version[3] );

        //read TAG table entries: 4 + 12 * nTags bytes
        iccProfile.nTags = xnz_be32toh(*(uint32_t*)((uint8_t*)iccProfile.buffer + sizeof(XNZ_ICC_HEADER)));
        iccProfile.tags = (XNZ_ICC_TAG*)((uint8_t*)iccProfile.buffer + sizeof(XNZ_ICC_HEADER) + 4);
        int tagIndex; for (tagIndex = 0; tagIndex < iccProfile.nTags; tagIndex++)
        {
            fprintf(stderr, "Tag Signature:\t%.4s\n", iccProfile.tags[tagIndex].name);
            fprintf(stderr, "Tag Offset:\t%d\n", xnz_be32toh(iccProfile.tags[tagIndex].offset));
            fprintf(stderr, "Tag Length:\t%d\n", xnz_be32toh(iccProfile.tags[tagIndex].length));
        }

        //assert(1==0);
    }
    //else assert(1==0);

}


XNZ_PNG_API XNZ_PNG_INLINE void xnz_png_open(XNZ_PNG_IMAGE* archive, char* pngFilePath)
{
    //1 OPEN THE GBL OR GLTF FILE FOR READING AND GET FILESIZE USING LSEEK
    archive->file.fd   = cr_file_open(pngFilePath);
    archive->file.size = cr_file_size(archive->file.fd);
    archive->file.path = pngFilePath;

    fprintf(stderr, "\nxnz_png_open::PNG File Size =  %lu bytes\n", archive->file.size);

    //2 MAP THE FILE TO BUFFER FOR READING
#ifndef _WIN32
    archive->file.buffer = cr_file_map_to_buffer(&(archive->file.buffer), archive->file.size, PROT_READ, MAP_SHARED | MAP_NORESERVE, archive->file.fd, 0);
    if (madvise(archive->file.buffer, (size_t)archive->file.size, MADV_SEQUENTIAL | MADV_WILLNEED) == -1) {
        printf("\nread madvise failed\n");
    }
#else
    archive->file.mFile = cr_file_map_to_buffer(&(archive->file.buffer), archive->file.size, PROT_READ, MAP_SHARED | MAP_NORESERVE, archive->file.fd, 0);
#endif

    //3  READ THE PNG (.png) file signature
    archive->IHDR = (XNZ_PNG_IHDR*)archive->file.buffer;

    assert(archive->IHDR->magicNumber[0] == 0x89);
    assert(archive->IHDR->magicNumber[1] == 'P');
    assert(archive->IHDR->magicNumber[2] == 'N');
    assert(archive->IHDR->magicNumber[3] == 'G');
    assert(archive->IHDR->magicNumber[4] == '\r');
    assert(archive->IHDR->magicNumber[5] == '\n');
    assert(archive->IHDR->magicNumber[6] == 0x1a);
    assert(archive->IHDR->magicNumber[7] == '\n');

    /*
     //get some memory to serialize the record
     record = &(records[recordIndex]);

     //read 4 bytes into the struct containing the opcode and record length (in bytes) as shorts, respectively
     //fread(record, 4, 1, file);
     memcpy(record, bufferPosition, 4);
     bufferPosition += 4;

     //convert 16 bits from big endian to little endian
     record->opcode = xnz_be16toh(record->opcode);//(( (record.opcode & 0xff)<<8 | (record.opcode & 0xff00)>>8 ));
     record->length = xnz_be16toh(record->length);//(( (record.length & 0xff)<<8 | (record.length & 0xff00)>>8 ));

     //allocate memory for reading the FLT header record data
     //TO DO: create a pool of filesize - numRecords * sizeof(recordHeader) to grab this memory from
     recordDataLength = record->length - sizeof(char *);
     record->data = (char*)malloc(recordDataLength);
     */

     //  Read IHDR Chunk
     //
     //  The sample depth is the same as the bit depth except in the case of indexed-colour PNG images (colour type 3), in which the sample depth is always 8 bits (see 4.4: PNG image).
     //
     //  Compression method is a single-byte integer that indicates the method used to compress the image data. Only compression method 0 (deflate/inflate compression with a sliding window of at most 32768
     //  bytes) is defined in this International Standard. All conforming PNG images shall be compressed with this scheme.
     //
     //  Filter method is a single-byte integer that indicates the preprocessing method applied to the image data before compression. Only filter method 0 (adaptive filtering with five basic filter types) is
     //  defined in this International Standard. See clause 9: Filtering for details.
     //
     //  Interlace method is a single-byte integer that indicates the transmission order of the image data. Two values are defined in this International Standard: 0 (no interlace) or 1 (Adam7 interlace). See
     //  clause 8: Interlacing and pass extraction for details.
     //
    fprintf(stderr, "sizeof(XNZ_PNG_IHDR) == %zu\n", sizeof(XNZ_PNG_IHDR));
    fprintf(stderr, "sizeof(xnz_png_chunk) == %zu\n", sizeof(xnz_png_chunk));

    const int reserveBytes = 3;
    const int magicBytes = 8;
    const int crcBytes = 4;
    assert(xnz_be32toh(archive->IHDR->length) == sizeof(XNZ_PNG_IHDR) - magicBytes - sizeof(xnz_png_chunk) /*- reserveBytes*/ - crcBytes);
    assert(xnz_be32toh(archive->IHDR->type) == PNG_IHDR_CHUNK_TYPE);

    fprintf(stderr, "IHDR Chunk Length: %d\n", xnz_be32toh(archive->IHDR->length));
    fprintf(stderr, "IHDR Chunk Type: %d\n", xnz_be32toh(archive->IHDR->type));
    fprintf(stderr, "IHDR Width: %d\n", xnz_be32toh(archive->IHDR->width));
    fprintf(stderr, "IHDR Height: %d\n", xnz_be32toh(archive->IHDR->height));
    fprintf(stderr, "IHDR Depth: %d\n", archive->IHDR->depth);
    fprintf(stderr, "IHDR Color Type: %d\n", archive->IHDR->color);
    fprintf(stderr, "IHDR Compression Method: %d\n", archive->IHDR->compression);
    fprintf(stderr, "IHDR Filter Method: %d\n", archive->IHDR->filter);
    fprintf(stderr, "IHDR Interlace Method: %d\n", archive->IHDR->interlace);
    fprintf(stderr, "IHDR CRC: %u\n", archive->IHDR->crc);

    archive->rowPitch = xnz_be32toh(archive->IHDR->width);
    archive->nScanlines = xnz_be32toh(archive->IHDR->height);
    archive->depth = archive->IHDR->depth;

    //Greyscale samples represent luminance if the transfer curve is indicated (by gAMA, sRGB, or iCCP) or device-dependent greyscale if not.
    //RGB samples represent calibrated colour information if the colour space is indicated (by gAMA and cHRM, or sRGB, or iCCP) or uncalibrated device-dependent colour if not.
    if (archive->IHDR->color == 0) archive->nSrcChannels = archive->nDstChannels = 1; //Grayscale
    else if (archive->IHDR->color == 2) archive->nSrcChannels = archive->nDstChannels = 3; //Truecolor
    else if (archive->IHDR->color == 6) archive->nSrcChannels = archive->nDstChannels = 4; //Truecolor + Alpha
    else assert(1 == 0);

    assert(archive->IHDR->depth == 8);


    //  Read PLTE or sPLT Chunk
    //
    //  The PLTE chunk contains from 1 to 256 palette entries, each a three-byte series of the form: R8G8B8
    //  The number of entries is determined from the chunk length. A chunk length not divisible by 3 is an error.
    //  This chunk shall appear for colour type 3, and may appear for colour types 2 and 6; it shall not appear for colour types 0 and 4.
    //  There shall not be more than one PLTE chunk.
    /*
     if( archive->IHDR->color != 0 && archive->IHDR->color != 4 )
     {
     archive->PLTE = (XNZ_PNG_PLTE*)(archive->file.buffer + PLTE_Offset);

     printf("PLTE Chunk Length: %u\n", xnz_be32toh(archive->PLTE->length));
     printf("PLTE Chunk Type: %u\n",   xnz_be32toh(archive->PLTE->type));

     if( xnz_be32toh(archive->PLTE->type) == PNG_PLTE_CHUNK_TYPE)
     {
     assert(archive->PLTE->length%3 == 0);
     uint32_t palletteIndex; for(palletteIndex = 0; palletteIndex < archive->PLTE->length/3; palletteIndex++)
     {
     printf("rgb[%d] = %u, %u, %u", palletteIndex, archive->PLTE->rgb[palletteIndex].x, archive->PLTE->rgb[palletteIndex].y, archive->PLTE->rgb[palletteIndex].z);
     }

     IDAT_Offset = PLTE_Offset + sizeof(xnz_png_chunk) + xnz_be32toh(archive->PLTE->length) + crcBytes;
     }
     else if( xnz_be32toh(archive->PLTE->type) == PNG_sPLT_CHUNK_TYPE)
     {
     assert(1==0);
     }
     else
     {
     archive->PLTE = NULL;
     }
     }

     //  Read pHYs Chunk
     //
     //  The pHYs chunk specifies the intended pixel size or aspect ratio for display of the image
     //
     archive->pHYs = (XNZ_PNG_pHYs*)(archive->file.buffer + pHYs_Offset);

     printf("pHYs Chunk Length: %u\n", xnz_be32toh(archive->pHYs->length));
     printf("pHYs Chunk Type: %u\n",   xnz_be32toh(archive->pHYs->type));

     if( xnz_be32toh(archive->pHYs->type) == PNG_pHYs_CHUNK_TYPE)
     {
     printf("pHYs ppx: %d\n", archive->pHYs->ppx);
     printf("pHYs ppy: %d\n", archive->pHYs->ppy);
     printf("pHYs unit: %c\n", archive->pHYs->unit);
     printf("pHYs unit: %c\n", archive->pHYs->crc);
     }
     else archive->pHYs = NULL;

     //  Read cHRM Chunk
     //
     //  The cHRM chunk may be used to specify the 1931 CIE x,y chromaticities of the red, green, and blue display primaries used in the image,
     //  and the referenced white point. See Annex C: Gamma and chromaticity for more information. The iCCP and sRGB chunks provide more sophisticated support for colour management and control.
     //
     if( xnz_be32toh(archive->cHRM->type) == PNG_pHYs_CHUNK_TYPE)
     {
     printf("cHRM white: %d, %d\n", archive->cHRM->wx, archive->cHRM->wy);
     printf("cHRM red: %d, %d\n",   archive->cHRM->rx, archive->cHRM->ry);
     printf("cHRM green: %d, %d\n", archive->cHRM->gx, archive->cHRM->gy);
     printf("cHRM blue: %d, %d\n",  archive->cHRM->bx, archive->cHRM->by);
     }
     else archive->cHRM = NULL;

     */


    //fprintf(stderr, "\nSerializing PNG File Chunks...\n");

    //records = (xnz_flt_record*)archive->records;
    uint64_t bytesRead = 0;
    char* bufferPosition = archive->file.buffer + magicBytes + sizeof(xnz_png_chunk) + xnz_be32toh(archive->IHDR->length) + crcBytes;
    xnz_png_chunk* chunk = (xnz_png_chunk*)bufferPosition;

    //count chunks
    while (bytesRead < archive->file.size)
    {
        chunk = (xnz_png_chunk*)(bufferPosition + bytesRead);
        //uint32_t type = xnz_be32toh(chunk->type);
        //fprintf(stderr, "Chunk Length: %u\n", xnz_be32toh(chunk->length));
        //fprintf(stderr, "Chunk Type: %.*s\n", 4, (char*)&(chunk->type));

        if (xnz_be32toh(chunk->type) == PNG_PLTE_CHUNK_TYPE) archive->PLTE = (XNZ_PNG_PLTE*)chunk;
        //else if( xnz_be32toh(chunk->type) == PNG_pHYs_CHUNK_TYPE) archive->pHYs = (XNZ_PNG_pHYs*)chunk;
        else if (xnz_be32toh(chunk->type) == PNG_cHRM_CHUNK_TYPE) archive->cHRM = (XNZ_PNG_cHRM*)chunk;
        else if (xnz_be32toh(chunk->type) == PNG_gAMA_CHUNK_TYPE) archive->gAMA = (xnz_png_chunk*)chunk;
        else if (xnz_be32toh(chunk->type) == PNG_iCCP_CHUNK_TYPE) archive->iCCP = (xnz_png_chunk*)chunk;
        else if (xnz_be32toh(chunk->type) == PNG_sRGB_CHUNK_TYPE) archive->sRGB = (xnz_png_chunk*)chunk;
        else if (xnz_be32toh(chunk->type) == PNG_IDAT_CHUNK_TYPE)
        {
            if (!archive->IDAT) archive->IDAT = (XNZ_PNG_IDAT*)chunk;         //store first IDAT chunk
            archive->blobSize += xnz_be32toh(((XNZ_PNG_IDAT*)chunk)->length);   //sum total size of all IDAT chunk blobs
            archive->nBlobChunks++;                                           //count IDAT chunks
        }
        else if (xnz_be32toh(chunk->type) == PNG_IEND_CHUNK_TYPE)  break;

        bytesRead += sizeof(xnz_png_chunk) + xnz_be32toh(chunk->length) + 4;
        archive->nChunks++;
    }


    //  Read Color Profile Chunk(s)
    //
    //
    //
    xnz_png_inflate_color_profile(archive);


    //  Read IDAT Chunk
    //
    //   The IDAT chunk contains the actual image data which is the output stream of the compression algorithm. See clause 9: Filtering and clause 10: Compression for details.
    //   There may b1e multiple IDAT chunks; if so, they shall appear consecutively with no other intervening chunks. The compressed datastream is then the concatenation of the
    //   contents of the data fields of all the IDAT chunks.
    //
    //   Decompress the IDAT chunks as a single ZLIB blob using LZ77 compression based on the ZLIB header:
    //
    //  78 01 - No Compression/low
    //  78 5E - ??
    //  78 9C - Default Compression
    //  78 DA - Best Compression
    assert(archive->IDAT);
    //archive->IDAT->data = (uint8_t*)archive->IDAT + sizeof(xnz_png_chunk);
    fprintf(stderr, "IDAT ZLIB Blob # Chunks: %d\n", archive->nBlobChunks);
    fprintf(stderr, "IDAT ZLIB Blob Header: %hx\n", xnz_be16toh(archive->IDAT->blob));
}


XNZ_PNG_API XNZ_PNG_INLINE void xnz_png_close(XNZ_PNG_IMAGE* archive)
{
    CRFileClose(&(archive->file));
    //xnz_file_unmap(crate->file.mFile, crate->file.buffer);
    //xnz_file_close(crate->file.fd);
}



#ifdef __cplusplus
}
#endif

#endif //_XNZ_PNG_IMAGE_H_
