#ifndef _XNZ_H_
#define _XNZ_H_


#ifdef __cplusplus
extern "C" {
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNZ_INLINE
#ifdef _WIN32
#define XNZ_INLINE __inline
#else
#define XNZ_INLINE
#endif
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNZ_DECLSPEC
#ifdef _WIN32
#define XNZ_DECLSPEC __declspec
#else
#define XNZ_DECLSPEC
#endif
#endif

//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNZ_ALIGN//(X)
#ifdef _WIN32
#define XNZ_ALIGN(X) (align(X))
#else
#define XNZ_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

// ********************************** Unsigned Byte Vector Types *************************************** //

//packed_float2, packed_float3, packed_float4 equivalents

XNZ_DECLSPEC XNZ_ALIGN(4) typedef union xnz_byte4_packed// 40 bytes
{
	XNZ_DECLSPEC XNZ_ALIGN(4) struct { unsigned char x, y, z, w; };
    XNZ_DECLSPEC XNZ_ALIGN(4) struct { unsigned char r, g, b, a; };
	XNZ_DECLSPEC XNZ_ALIGN(4) unsigned char  vector[4];
	//XNZ_DECLSPEC XNZ_ALIGN(4) __m128 simd;

}xnz_byte4_packed;

typedef xnz_byte4_packed xnz_packed_byte4;


XNZ_DECLSPEC XNZ_ALIGN(4) typedef union xnz_byte3_packed
{
	XNZ_DECLSPEC XNZ_ALIGN(4) struct { unsigned char x, y, z; };
	XNZ_DECLSPEC XNZ_ALIGN(4) unsigned char	vector[3];
}xnz_byte3_packed;

typedef xnz_byte3_packed xnz_byte3_packed;


XNZ_DECLSPEC XNZ_ALIGN(4) typedef union xnz_byte2_packed
{
	XNZ_DECLSPEC XNZ_ALIGN(4) struct { unsigned char x, y; };
	XNZ_DECLSPEC XNZ_ALIGN(4) unsigned char vector[2];
}xnz_byte2_packed;

typedef xnz_byte2_packed xnz_byte2_packed;


//float2, float3, float4 equivalents

XNZ_DECLSPEC XNZ_ALIGN(16) typedef struct  xnz_byte4 // 48 bytes (multiple of 16 because of simd variable)
{
	union{
	XNZ_DECLSPEC XNZ_ALIGN(16) struct { unsigned char x, y, z, w; };
    XNZ_DECLSPEC XNZ_ALIGN(16) struct { unsigned char r, g, b, a; };
	XNZ_DECLSPEC XNZ_ALIGN(16) unsigned char	vector[4];
//#if defined(XNZ_USE_SIMD)
//       XNZ_DECLSPEC XNZ_ALIGN(16) __m128 simd;
//#endif
	XNZ_DECLSPEC XNZ_ALIGN(16) xnz_byte4_packed packed;		//will allow use of assignment (copy) operator for vbos
	};

}xnz_byte4;


/*XNZ_DECLSPEC XNZ_ALIGN(16)*/ typedef union xnz_byte3
{
	/*XNZ_DECLSPEC XNZ_ALIGN(16)*/ struct { unsigned char x, y, z; };
	/*XNZ_DECLSPEC XNZ_ALIGN(16)*/ unsigned char vector[3];

}xnz_byte3;


XNZ_DECLSPEC XNZ_ALIGN(16) typedef union xnz_byte2
{
	XNZ_DECLSPEC XNZ_ALIGN(16) struct { unsigned char x, y; };
	XNZ_DECLSPEC XNZ_ALIGN(16) unsigned char vector[2];
	XNZ_DECLSPEC XNZ_ALIGN(16) xnz_byte2_packed packed;		//will allow use of assignment (copy) operator for vbos

}xnz_byte2;


//linked list node structs

XNZ_DECLSPEC XNZ_ALIGN(16) typedef struct xnz_byte4_node// 48 bytes (multiple of 16 because of simd variable)
{
	XNZ_DECLSPEC XNZ_ALIGN(16) unsigned char x,y,z,w;
	XNZ_DECLSPEC XNZ_ALIGN(16) struct xnz_byte4_node * next;		//will allow use of assignment (copy) operator for vbos

}xnz_byte4_node;




// ********************************** Unsigned Integer Vector Types *************************************** //

//packed_float2, packed_float3, packed_float4 equivalents

XNZ_DECLSPEC XNZ_ALIGN(4) typedef union xnz_uint2_packed
{
    XNZ_DECLSPEC XNZ_ALIGN(4) struct { unsigned int x, y; };
    XNZ_DECLSPEC XNZ_ALIGN(4) unsigned int vector[2];

}xnz_uint2_packed; 

//typedef xnz_uint2_packed xnz_uint2_packed;


XNZ_DECLSPEC XNZ_ALIGN(4) typedef union xnz_uint4_packed // 40 bytes
{
    XNZ_DECLSPEC XNZ_ALIGN(4) struct {
        unsigned int x, y;
        union { unsigned int z; unsigned int width; };
        union { unsigned int w; unsigned int height; };
    };
    //CR_MATH_DECLSPEC CR_MATH_ALIGN(4) struct { unsigned int x, y, width, height; };
    XNZ_DECLSPEC XNZ_ALIGN(4) unsigned int  vector[4];
    //CR_MATH_DECLSPEC CR_MATH_ALIGN(4) __m128 simd;

}xnz_uint4_packed;

//float2, float3, float4 equivalents

XNZ_DECLSPEC XNZ_ALIGN(16) typedef union xnz_uint2
{
    XNZ_DECLSPEC XNZ_ALIGN(16) struct { unsigned int x, y; };
    XNZ_DECLSPEC XNZ_ALIGN(16) unsigned int vector[2];
    XNZ_DECLSPEC XNZ_ALIGN(16) xnz_uint2_packed packed;        //will allow use of assignment (copy) operator for vbos

}xnz_uint2;

XNZ_DECLSPEC XNZ_ALIGN(16) typedef struct  xnz_uint4 // 48 bytes (multiple of 16 because of simd variable)
{
	union
    {
        XNZ_DECLSPEC XNZ_ALIGN(4)  struct { unsigned int     x, y,   z, w; };
        XNZ_DECLSPEC XNZ_ALIGN(16) struct { xnz_uint2_packed origin, size; }; //will allow use of assignment (copy) operator for vbos
        XNZ_DECLSPEC XNZ_ALIGN(16) unsigned int	vector[4];
#if defined(XNZ_USE_SIMD)
        XNZ_DECLSPEC CR_MATH_ALIGN(16) __m128 simd;
#endif
        XNZ_DECLSPEC XNZ_ALIGN(16) xnz_uint4_packed packed;		//will allow use of assignment (copy) operator for vbos
	};

}xnz_uint4;


// ********************************** Float Vector Types *************************************** //

//packed_float2, packed_float3, packed_float4 equivalents
XNZ_DECLSPEC XNZ_ALIGN(4) typedef union xnz_float2_packed
{
    XNZ_DECLSPEC XNZ_ALIGN(4) struct { float x, y; };
    XNZ_DECLSPEC XNZ_ALIGN(4) float vector[2];
}xnz_float2_packed;

typedef xnz_float2_packed xnz_packed_float2;



XNZ_DECLSPEC XNZ_ALIGN(4) typedef union xnz_float3_packed
{
    XNZ_DECLSPEC XNZ_ALIGN(4) struct { float x, y, z; };
    XNZ_DECLSPEC XNZ_ALIGN(4) float  vector[3];
}xnz_float3_packed;


XNZ_DECLSPEC XNZ_ALIGN(4) typedef union xnz_float4_packed // 40 bytes
{
    XNZ_DECLSPEC XNZ_ALIGN(4) struct { float x, y, z, w; };
    XNZ_DECLSPEC XNZ_ALIGN(4) struct { float r, g, b, a; };
    XNZ_DECLSPEC XNZ_ALIGN(4) float  vector[4];
    XNZ_DECLSPEC XNZ_ALIGN(4) xnz_float3_packed float3;		//will allow use of assignment (copy) operator for vbos
	//CR_MATH_DECLSPEC CR_MATH_ALIGN(4) __m128 simd;

}xnz_float4_packed;

typedef xnz_float4_packed xnz_packed_float4;



//float2, float3, float4 equivalents

XNZ_DECLSPEC XNZ_ALIGN(16) typedef union xnz_float2
{
    XNZ_DECLSPEC XNZ_ALIGN(16) struct { float x, y; };
    XNZ_DECLSPEC XNZ_ALIGN(16) float vector[2];
    XNZ_DECLSPEC XNZ_ALIGN(16) xnz_packed_float2 packed;     //will allow use of assignment (copy) operator for vbos
    
}xnz_float2;

XNZ_DECLSPEC XNZ_ALIGN(16) typedef union xnz_float3
{
    XNZ_DECLSPEC XNZ_ALIGN(16) struct { float x, y, z; };
    XNZ_DECLSPEC XNZ_ALIGN(16) float vector[3];
    XNZ_DECLSPEC XNZ_ALIGN(4)  xnz_float3_packed packed;        //will allow use of assignment (copy) operator for vbos
    XNZ_DECLSPEC XNZ_ALIGN(16) xnz_float2 float2;
}xnz_float3;

XNZ_DECLSPEC XNZ_ALIGN(16) typedef union xnz_float_affine_transform
{
    XNZ_DECLSPEC XNZ_ALIGN(16) struct { float a,b,c,d,e,f; };
    XNZ_DECLSPEC XNZ_ALIGN(16) float  vector[6];
#if defined(XNZ_USE_SIMD)
    XNZ_DECLSPEC XNZ_ALIGN(16) __m128 simd;
#endif
    XNZ_DECLSPEC XNZ_ALIGN(16) xnz_float4_packed packed;     //will allow use of assignment (copy) operator for vbos
    XNZ_DECLSPEC XNZ_ALIGN(16) xnz_float3 float3;            //will allow use of assignment (copy) operator for vbos
}xnz_float_affine_transform;


//linked list node structs

/*
CR_MATH_DECLSPEC CR_MATH_ALIGN(16) typedef struct xnz_uint4_node// 48 bytes (multiple of 16 because of simd variable)
{
    CR_MATH_DECLSPEC CR_MATH_ALIGN(16) uint32_t x, y, z, w;
    CR_MATH_DECLSPEC CR_MATH_ALIGN(16) struct xnz_uint4_node* next;		//will allow use of assignment (copy) operator for vbos

}xnz_uint4_node;

#define XNZ_RECT_ZERO       { 0,0,0,1 }
static  xnz_uint4 xnz_uint4_zero = XNZ_RECT_ZERO;
*/


/***
*  xnz_byte_utils.h
*  XNZ
*
*  Created by Joe Moulton on 1/8/17
*  Copyright � 2017 Abstract Embedded. All rights reserved.
***/

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

#	define __WINDOWS__

#endif

/** compatibility header for endian.h
 * This is a simple compatibility shim to convert
 * BSD/Linux endian macros to the Mac OS X equivalents.
 * It is public domain.
 * */

 //8 bit stubs are here for posterity, such as when using x-macro expansion for serializing with a function macro
#define xnz_htobe8(x) x
#define xnz_htole8(x) x
#define xnz_be8toh(x) x
#define xnz_le8toh(x) x

//Platform specific byte conversion functions
#ifdef __APPLE__

#include <libkern/OSByteOrder.h>

#define xnz_htobe16(x) OSSwapHostToBigInt16(x)
#define xnz_htole16(x) OSSwapHostToLittleInt16(x)
#define xnz_be16toh(x) OSSwapBigToHostInt16(x)
#define xnz_le16toh(x) OSSwapLittleToHostInt16(x)

#define xnz_htobe32(x) OSSwapHostToBigInt32(x)
#define xnz_htole32(x) OSSwapHostToLittleInt32(x)
#define xnz_be32toh(x) OSSwapBigToHostInt32(x)
#define xnz_le32toh(x) OSSwapLittleToHostInt32(x)

#define xnz_htobe64(x) OSSwapHostToBigInt64(x)
#define xnz_htole64(x) OSSwapHostToLittleInt64(x)
#define xnz_be64toh(x) OSSwapBigToHostInt64(x)
#define xnz_le64toh(x) OSSwapLittleToHostInt64(x)

#elif defined(__OpenBSD__)

#	include <sys/endian.h>

#	define xnz_be16toh(x) betoh16(x)
#	define xnz_le16toh(x) letoh16(x)

#	define xnz_be32toh(x) betoh32(x)
#	define xnz_le32toh(x) letoh32(x)

#	define xnz_be64toh(x) betoh64(x)
#	define xnz_le64toh(x) letoh64(x)

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#	include <sys/endian.h>

#	define xnz_be16toh(x) betoh16(x)
#	define xnz_le16toh(x) letoh16(x)

#	define xnz_be32toh(x) betoh32(x)
#	define xnz_le32toh(x) letoh32(x)

#	define xnz_be64toh(x) betoh64(x)
#	define xnz_le64toh(x) letoh64(x)

#elif defined(__WINDOWS__)

#include <stdlib.h>
//#	include <winsock2.h>
//#	include <sys/param.h>
//#pragma comment(lib, "Ws2_32.lib")
#	if BYTE_ORDER == LITTLE_ENDIAN

/*
#		define xnz_htobe16(x) htons(x)
#		define xnz_htole16(x) (x)
#		define xnz_be16toh(x) ntohs(x)
#		define xnz_le16toh(x) (x)

#		define xnz_htobe32(x) htonl(x)
#		define xnz_htole32(x) (x)
#		define xnz_be32toh(x) ntohl(x)
#		define xnz_le32toh(x) (x)

#		define xnz_htobe64(x) htonll(x)
#		define xnz_htole64(x) (x)
#		define xnz_be64toh(x) ntohll(x)
#		define xnz_le64toh(x) (x)
 */

#		define xnz_htobe16(x) _byteswap_ushort(x)
#		define xnz_htole16(x) (x)
#		define xnz_be16toh(x) _byteswap_ushort(x)
#		define xnz_le16toh(x) (x)

#		define xnz_htobe32(x) _byteswap_ulong(x)
#		define xnz_htole32(x) (x)
#		define xnz_be32toh(x) _byteswap_ulong(x)
#		define xnz_le32toh(x) (x)

#		define xnz_htobe64(x) _byteswap_uint64(x)
#		define xnz_htole64(x) (x)
#		define xnz_be64toh(x) _byteswap_uint64(x)
#		define xnz_le64toh(x) (x)


#	elif BYTE_ORDER == BIG_ENDIAN

        /* that would be xbox 360 */
#		define xnz_htobe16(x) (x)
#		define xnz_htole16(x) __builtin_bswap16(x)
#		define xnz_be16toh(x) (x)
#		define xnz_le16toh(x) __builtin_bswap16(x)

#		define xnz_htobe32(x) (x)
#		define xnz_htole32(x) __builtin_bswap32(x)
#		define xnz_be32toh(x) (x)
#		define xnz_le32toh(x) __builtin_bswap32(x)

#		define xnz_htobe64(x) (x)
#		define xnz_htole64(x) __builtin_bswap64(x)
#		define xnz_be64toh(x) (x)
#		define xnz_le64toh(x) __builtin_bswap64(x)

#	else

#		error byte order not supported

#	endif

#	define __BYTE_ORDER    BYTE_ORDER
#	define __BIG_ENDIAN    BIG_ENDIAN
#	define __LITTLE_ENDIAN LITTLE_ENDIAN
#	define __PDP_ENDIAN    PDP_ENDIAN

#else

//#	error platform not supported

#endif

/***
*  xnz_types.h
*  XNZ
*
*  Created by Joe Moulton on 4/1/24
*  Copyright � 2024 Abstract Embedded. All rights reserved.
***/


typedef enum XNG_IMAGE_API_STATUS
{
    XNG_IMAGE_API_SUCCESS                    = 0,
    XNG_IMAGE_API_INVALID_FILE                = 1,
    XNG_IMAGE_API_NULL_CONTAINER            = 2,
    XNG_IMAGE_API_INCOMPATIBLE_FOURCC        = 3,
    XNG_IMAGE_API_INPUT_FILE_ERROR            = 4,
    XNG_IMAGE_API_INVALID_FILE_STRUCTURE    = 5,
    XNG_IMAGE_API_UNSUPPORTED_FORMAT        = 6,
    XNG_IMAGE_API_UNHANDLED_PIXEL_FORMAT    = 7,
    XNG_IMAGE_API_UNHANDLED_FOURCC_FORMAT   = 8,
    XNG_IMAGE_API_UNABLE_TO_READ_DATA        = 9,
    XNG_IMAGE_API_BUFFER_IS_NULL            = 10
}XNG_IMAGE_API_STATUS;


/***
*  xnz_bitstream.h
*  XNZ
*
*  Created by Joe Moulton on 4/1/24
*  Copyright � 2024 Abstract Embedded. All rights reserved.
***/

#include <stdint.h>
#include <assert.h>
#ifdef _WIN32
#include <malloc.h>
#else
#include <stdlib.h>     /* malloc, free, rand */
#include <alloca.h>
#endif

//BITSTREAM COMPILE OPTIONS
#define XNZ_BITSTREAM_QUEUE 0
//#define XNZ_BITSTREAM_DEBUG

typedef uint32_t xnz_bs_size;
typedef uint32_t xnz_bs_len;
typedef uint32_t xnz_bs_reg;
typedef uint32_t xnz_bs_mask;

//static const N_PREFETCH_BYTES = sizeof(xnz_bs_reg);
//static const N_PREFETCH_BITS  = (sizeof(xnz_bs_reg) * 8);

typedef struct xnz_bitstream // 44 or 56 bytes
{
    //pointers
    uint8_t* buffer;           // 8 bytes, stream chunk byte start
    uint8_t* pointer;          // 8 bytes, stream chunk byte position

#if XNZ_BITSTREAM_QUEUE
    uint8_t*    next_buffer;   // 8 bytes
    xnz_bs_size next_capacity; // 4 bytes
#endif

    //sizes
    xnz_bs_size capacity;      // 4 bytes, stream chunk size in bytes
    xnz_bs_size length;        // 4 bytes, stream chunk size in bits + prev chunk overflow bits

    //lengths
    xnz_bs_len position;       // 4 bytes, stream bit position
    xnz_bs_len offset;         // 4 bytes, accumulation register position bit position
        
    //registers
    xnz_bs_reg prefetch;       // 4 bytes
    xnz_bs_reg accumulator;    // 4 bytes

    //bitwise op helpers
    xnz_bs_mask mask;          // 4 bytes, can be eliminated

}xnz_bitstream;

//prefetch up to 4 bytes from the src buffer
/* V1
#define xnz_bitstream_prefetch(_bs) \
    (_bs->prefetch) = 0; \
    if (((uint32_t) (_bs->pointer - _bs->buffer)) < (_bs->capacity + 0)) \
        (_bs->prefetch) |= (*(_bs->pointer + 0)); \
    if (((uint32_t) (_bs->pointer - _bs->buffer)) < (_bs->capacity + 1)) \
        (_bs->prefetch) |= (*(_bs->pointer + 1) << 8); \
    if (((uint32_t) (_bs->pointer - _bs->buffer)) < (_bs->capacity + 2)) \
        (_bs->prefetch) |= (*(_bs->pointer + 2) << 16); \
    if (((uint32_t) (_bs->pointer - _bs->buffer)) < (_bs->capacity + 3)) \
        (_bs->prefetch) |= (*(_bs->pointer + 3) << 24);
*/

#define xnz_bitstream_prefetch(_bs) \
 uint32_t nbytes = (uint32_t)(_bs->pointer - _bs->buffer); \
     if ( nbytes < _bs->capacity + 3) _bs->prefetch = (*(_bs->pointer + 0)) | (*(_bs->pointer + 1) << 8) | (*(_bs->pointer + 2) << 16) | (*(_bs->pointer + 3) << 24); \
else if ( nbytes < _bs->capacity + 2) _bs->prefetch = (*(_bs->pointer + 0)) | (*(_bs->pointer + 1) << 8) | (*(_bs->pointer + 2) << 16); \
else if ( nbytes < _bs->capacity + 1) _bs->prefetch = (*(_bs->pointer + 0)) | (*(_bs->pointer + 1) << 8); \
else if ( nbytes < _bs->capacity + 0) _bs->prefetch = (*(_bs->pointer + 0));


/* V3
#define xnz_bitstream_prefetch(_bs) \
 uint32_t nbytes = (uint32_t)(_bs->pointer - _bs->buffer);  _bs->prefetch = 0; \
     if ( nbytes < _bs->capacity + 3) memcpy(&_bs->prefetch, _bs->pointer, 4); \
else if ( nbytes < _bs->capacity + 2) memcpy(&_bs->prefetch, _bs->pointer, 3); \
else if ( nbytes < _bs->capacity + 1) memcpy(&_bs->prefetch, _bs->pointer, 2); \
else if ( nbytes < _bs->capacity + 0) memcpy(&_bs->prefetch, _bs->pointer, 1);
*/

//#define xnz_bitstream_prefetch(_bs) _bs->prefetch = 0; memcpy(&_bs->prefetch, _bs->pointer, MIN(_bs->capacity + 3 - (uint32_t)(_bs->pointer - _bs->buffer), 4));

/*
#define xnz_bitstream_fetch(_bs) do { \
    (_bs->accumulator) = 0; \
    if (((uint32_t) (_bs->pointer - _bs->buffer)) < (_bs->capacity + 0)) \
        (_bs->accumulator) |= (*(_bs->pointer + 0) << 0); \
    if (((uint32_t) (_bs->pointer - _bs->buffer)) < (_bs->capacity + 1)) \
        (_bs->accumulator) |= (*(_bs->pointer + 1) << 8); \
    if (((uint32_t) (_bs->pointer - _bs->buffer)) < (_bs->capacity + 2)) \
        (_bs->accumulator) |= (*(_bs->pointer + 2) << 16); \
    if (((uint32_t) (_bs->pointer - _bs->buffer)) <(_bs->capacity + 3)) \
        (_bs->accumulator) |= (*(_bs->pointer + 3) << 24); \
    xnz_bitstream_prefetch(_bs); \
} while(0)
*/

#define xnz_bitstream_shift(_bs, _nbits) \
    _bs->accumulator <<= _nbits; \
    _bs->position += _nbits; \
    _bs->offset   += _nbits; \
    if (_bs->offset < 32) { \
        _bs->mask = ((1 << _nbits) - 1); \
        _bs->accumulator |= ((_bs->prefetch) & _bs->mask); \
        _bs->prefetch >>= _nbits; \
    } else { \
        _bs->mask = ((1 << (32 - (_bs->offset - _nbits))) - 1); \
        _bs->accumulator |= ((_bs->prefetch) & _bs->mask); \
        _bs->prefetch >>= (32 - (_bs->offset - _nbits)); \
        _bs->offset -= 32; \
        _bs->pointer += 4; \
        xnz_bitstream_prefetch(_bs); \
        if (_bs->offset) { \
            _bs->mask = ((1 << _bs->offset) - 1); \
            _bs->accumulator |= ((_bs->prefetch) & _bs->mask) << (32 - ((_bs->offset+32) - _nbits)); \
            _bs->prefetch >>= _bs->offset; \
        } \
    }

#ifdef XNZ_BITSTREAM_DEBUG
void xnz_bitstream_print(xnz_bitstream* bs)
{
    fprintf(stderr, "bs->position = %d\n",    bs->position);
    fprintf(stderr, "bs->offset = %d\n",      bs->offset);
    fprintf(stderr, "bs->capacity = %d\n",    bs->capacity);
    fprintf(stderr, "bs->length = %d\n",      bs->length);
    //fprintf(stderr, "bs->accumulator = %d\n", bs->accumulator);
}
#endif

static void xnz_bitstream_attach_prefetch(xnz_bitstream* bs, uint8_t* buffer, uint32_t capacity)
{
    assert(bs->position == 0);
    //assert(bs->length - bs->position <= 32);

    bs->buffer      = buffer;
    bs->pointer     = bs->buffer;

#if XNZ_BITSTREAM_QUEUE
    bs->next_buffer = NULL;
    bs->next_capacity = 0;
#endif

    bs->position    = 0;
    bs->capacity    = capacity;
    bs->length      = bs->capacity * 8;

    bs->mask        = 0;
    bs->offset      = 0;

    bs->accumulator = 0;
    bs->prefetch    = 0;

    //load first four bytes into prefetch register
    xnz_bitstream_prefetch(bs);

    /*
    unsigned* codebits = xnz_convert_to_binary((unsigned*)&(bs->prefetch), 3 * 8);
    uint32_t bit; for (bit = 3 * 8; bit--;)
    {
        if (bit % 8 == 7) printf(" ");
        fprintf(stderr, "%u", +codebits[bit]);
    }
    free(codebits);
    printf("\n");
    */
}

static void xnz_bitstream_attach(xnz_bitstream* bs, uint8_t* buffer, uint32_t capacity)
{

#if XNZ_BITSTREAM_QUEUE

    bs->next_buffer = NULL;
    bs->next_capacity = 0;
    
    if ((int32_t)bs->length - (int32_t)bs->position > 32) //hate the cast here but it's necessary
    {
        //delay reattach to occur during read bit(s) by "queuing" the next stream buffer
#ifdef XNZ_BITSTREAM_DEBUG
        fprintf(stderr, "xnz_bitstream_attach::queue\n");
#endif

        bs->next_buffer   = buffer;
        bs->next_capacity = capacity;
        return;
    }

#endif

    // attach/reattach immediately
    {
        
#ifdef XNZ_BITSTREAM_DEBUG
        fprintf(stderr, "xnz_bitstream_attach::immediate\n");
        assert(bs->length - bs->position <= 32);
#endif

        //assert if bitstream was init'd with bitstream_attach_prefetch then reattach should never be called w position == 0
        //bc it indicates that we decoded exactly the amount of bytes we needed and there was no prefetch overlap
        //assert(bs->position != 0);  
        
        bs->buffer = buffer;
        bs->pointer = bs->buffer - 4; //subtract 4 bytes to force prefetch to occur at start of buffer on bitstream_shift
        //bs->next_buffer = NULL;
        //bs->next_capacity = 0;

        bs->offset = 32 - (bs->length - bs->position);
        bs->capacity = capacity;
        bs->length = (bs->length - bs->position) + bs->capacity * 8;
        bs->position = 0;
    }

}

static uint8_t xnz_bitstream_read_bit(xnz_bitstream* bs)
{
#if XNZ_BITSTREAM_QUEUE
    if (bs->position >= bs->length && bs->next_buffer)
    {
        //reattach
        assert(bs->next_capacity > 0);
        xnz_bitstream_attach(bs, bs->next_buffer, bs->capacity);
    }
#endif

#ifdef XNZ_BITSTREAM_DEBUG
    assert(bs->position < bs->length); //never read beyond max bits in current stream chunk of bits
    //this conditional is necessary because of negative reattach offset for automatic prefetech (i suppose i could just add 4 to the assert statement)
    if (bs->pointer > bs->buffer)  assert(((uint32_t)(bs->pointer - bs->buffer)) <= bs->capacity + 4);
#endif

    //read bit by shifiting from prefetch into accumulation register
    xnz_bitstream_shift(bs, 1);
    return (uint8_t)(bs->accumulator & (uint32_t)0x01);
}

static uint32_t xnz_bitstream_read_bits(xnz_bitstream* bs, uint32_t nbits)
{
#if XNZ_BITSTREAM_QUEUE
    if (bs->position >= bs->length && bs->next_buffer)
    {
        //reattach
        assert(bs->next_capacity > 0);
        xnz_bitstream_attach(bs, bs->next_buffer, bs->next_capacity);
    }
#endif

#ifdef XNZ_BITSTREAM_DEBUG
    assert(nbits <= 32); //Ensure we aren't asking for more bits than accumulation register can handle
    assert(bs->position < bs->length); //never read beyond max bits in current stream chunk of bits
    //this conditional is necessary because of negative reattach offset for automatic prefetech (i suppose i could just add 4 to the assert statement)
    if (bs->pointer > bs->buffer) assert(((uint32_t)(bs->pointer - bs->buffer)) <= bs->capacity + 4);
#endif

    //read bits by shifiting from prefetch into accumulation register
    xnz_bitstream_shift(bs, nbits);
    return (bs->accumulator & ((1 << nbits) - 1));
}

/***
*  xnz.h
*  XNZ
*
*  Created by Joe Moulton on 4/1/24
*  Copyright � 2024 Abstract Embedded. All rights reserved.
***/

//# pragma mark -- Common Decompression Struct Defines

//INFLATE COMPILE OPTIONS
//#define XNZ_DEBUG
//#define XNZ_HUFF_DEBUG
//#define XNZ_LOG_BLOCKS

typedef enum xnz_status
{
    XNZ_STATUS_ERROR = -1,
    XNZ_STATUS_OK    =  0,
    XNZ_STATUS_END   =  1
}xnz_status;


typedef enum xnz_stream_operation
{
    /* Encode to a compressed stream */
    XNZ_STREAM_ENCODE = 0,
  
    /* Decode from a compressed stream */
    XNZ_STREAM_DECODE = 1,

} xnz_stream_operation;


/* Bits for the flags in compression_stream_process. */
typedef enum xnz_stream_flags
{
    XNZ_STREAM_FINALIZE = 0x0001,
} xnz_stream_flags;

typedef enum xnz_block_type
{
    XNZ_BLOCK_RAW      = 0x0000,  // raw      [bytes]
    XNZ_BLOCK_FIXED    = 0x0001,  // fixed    [huff codes]
    XNZ_BLOCK_DYNAMIC  = 0x0001,  // dynamic  [huff codes]
    XNZ_BLOCK_CUSTOM   = 0x0001,  // custom   [compression TBD]
    XNZ_BLOCK_CONTINUE = 0x0004,  // continue [block decode]
    XNZ_BLOCK_MAX      = 0x0005
}xnz_block_type;

/*XNZ_DECLSPEC XNZ_ALIGN(16)*/ 
typedef struct xnz_huff_node      //24 bytes
{
    uint32_t              symbol; // 4 bytes, code sequence
    uint32_t              len;    // 4 bytes, code sequence length in bits, unused
    struct xnz_huff_node* left;   // 8 bytes
    struct xnz_huff_node* right;  // 8 bytes
}xnz_huff_node;


//# Compact representation of the length code value(257 - 285), length rangeand number
//# of extra bits to use in LZ77 compression(See Section 3.2.5 of RFC 1951)

/*
length_code_ranges = [
    [257, 0, 3, 3], [258, 0, 4, 4], [259, 0, 5, 5], [260, 0, 6, 6], [261, 0, 7, 7],
    [262, 0, 8, 8], [263, 0, 9, 9], [264, 0, 10, 10], [265, 1, 11, 12], [266, 1, 13, 14],
    [267, 1, 15, 16], [268, 1, 17, 18], [269, 2, 19, 22], [270, 2, 23, 26], [271, 2, 27, 30],
    [272, 2, 31, 34], [273, 3, 35, 42], [274, 3, 43, 50], [275, 3, 51, 58], [276, 3, 59, 66],
    [277, 4, 67, 82], [278, 4, 83, 98], [279, 4, 99, 114], [280, 4, 115, 130], [281, 5, 131, 162],
    [282, 5, 163, 194], [283, 5, 195, 226], [284, 5, 227, 257], [285, 0, 258, 258]
]

//#Construct a lookup table mapping length codes to(num_bits, lower_bound) pairs
length_codes = {}
for code, num_bits, lower_bound, upper_bound in length_code_ranges :
    for i in range(lower_bound, upper_bound + 1) :
        length_codes[code] = (num_bits, lower_bound)

    int code; for (code = 0; code < 286; code++)
    {


    }

//# Compact representation of the distance code value(0 - 31), distance range and number
//# of extra bits to use in LZ77 compression(See Section 3.2.5 of RFC 1951)
distance_code_ranges = [
    [0, 0, 1, 1], [1, 0, 2, 2], [2, 0, 3, 3], [3, 0, 4, 4], [4, 1, 5, 6],
    [5, 1, 7, 8], [6, 2, 9, 12], [7, 2, 13, 16], [8, 3, 17, 24], [9, 3, 25, 32],
    [10, 4, 33, 48], [11, 4, 49, 64], [12, 5, 65, 96], [13, 5, 97, 128], [14, 6, 129, 192],
    [15, 6, 193, 256], [16, 7, 257, 384], [17, 7, 385, 512], [18, 8, 513, 768], [19, 8, 769, 1024],
    [20, 9, 1025, 1536], [21, 9, 1537, 2048], [22, 10, 2049, 3072], [23, 10, 3073, 4096], [24, 11, 4097, 6144],
    [25, 11, 6145, 8192], [26, 12, 8193, 12288], [27, 12, 12289, 16384], [28, 13, 16385, 24576], [29, 13, 24577, 32768],
]

//#Construct a lookup table mapping distance codes to(num_bits, lower_bound) pairs
distance_codes = {}
for code, num_bits, lower_bound, upper_bound in distance_code_ranges :
    for i in range(lower_bound, upper_bound + 1) :
        distance_codes[code] = (num_bits, lower_bound)
*/

#define N_FIXED_LL_CODES    288
#define N_FIXED_DIST_CODES  32
#define N_DYNAMIC_CL_CODES  19

//binary [huffman code] trees required num_codes * 2 - 1 (because the root node is only needed once)
#define N_FIXED_LL_NODES      ((N_FIXED_LL_CODES * 2) - 1)
#define N_FIXED_DIST_NODES  ((N_FIXED_DIST_CODES * 2) - 1)
#define N_DYNAMIC_CL_NODES  ((N_DYNAMIC_CL_CODES * 2) - 1)

#define XNZ_DEFLATE_BUFFER_SIZE 32768

//Fixed length code offsets and ranges as defined by Katz are used only with length symbols (ie values > 256) in the literal-length code table
static xnz_uint2 RFC_1951_FIXED_LENGTH_CODES[29] =
{
    //{0, XXX}  // 256 (terminating symbol)
    {0, 3},     // 257
    {0, 4},     // 258
    {0, 5},     // 259
    {0, 6},     // 260
    {0, 7},     // 261
    {0, 8},     // 262
    {0, 9},     // 263
    {0, 10},    // 264
    {1, 11},    // 265
    {1, 13},    // 266
    {1, 15},    // 267
    {1, 17},    // 268
    {2, 19},    // 269
    {2, 23},    // 270
    {2, 27},    // 271
    {2, 31},    // 272
    {3, 35},    // 273
    {3, 43},    // 274
    {3, 51},    // 275
    {3, 59},    // 276
    {4, 67},    // 277
    {4, 83},    // 278
    {4, 99},    // 279
    {4, 115},   // 280
    {5, 131},   // 281
    {5, 163},   // 282
    {5, 195},   // 283
    {5, 227},   // 284
    {0, 258}    // 285
    //{0, XXX}  // 286
    //{0, XXX}  // 287
};

//Fixed distance code offsets and ranges as defined by Katz are given for 30/32 possible dist symbols (2 unused/reserved beyond 32768 range)
static xnz_uint2 RFC_1951_FIXED_DIST_CODES[30] =
{
    {0, 1},       // 0
    {0, 2},       // 1
    {0, 3},       // 2
    {0, 4},       // 3
    {1, 5},       // 4
    {1, 7},       // 5
    {2, 9},       // 6
    {2, 13},      // 7
    {3, 17},      // 8
    {3, 25},      // 9
    {4, 33},      // 10
    {4, 49},      // 11
    {5, 65},      // 12
    {5, 97},      // 13
    {6, 129},     // 14
    {6, 193},     // 15
    {7, 257},     // 16
    {7, 385},     // 17
    {8, 513},     // 18
    {8, 769},     // 19
    {9, 1025},    // 20
    {9, 1537},    // 21
    {10, 2049},   // 22
    {10, 3073},   // 23
    {11, 4097},   // 24
    {11, 6145},   // 25
    {12, 8193},   // 26
    {12, 12289},  // 27
    {13, 16385},  // 28
    {13, 24577}   // 29
    //{14, XXX}   // 39
    //{14, XXX}   // 31
};

/*
typedef struct 
{
    You are partially responsible for management of the dst_ptr,
    dst_size, src_ptr, and src_size fields.  You must initialize
    them to describe valid memory buffers before making a call to
    compression_stream_process. compression_stream_process will update
    these fields before returning to account for the bytes of the src
    and dst buffers that were successfully processed.

    uint8_t       * dst_ptr;
    size_t          dst_size;
    const uint8_t * src_ptr;
    size_t          src_size;
  
    //The stream state object is managed by the compression_stream functions.
    //You should not ever directly access this field.
    void          * __nullable state;

} compression_stream;
*/

//This struct needs more abstraction to make it decompression algorithm agnostic
//(ie the fixed sizes of its tree structures and history buffer are for RFC 1951 only)
typedef struct xnz_block_stream //96 bytes + DEFLATE trees/history
{
    //public
    uint8_t* src_ptr;              // 8 bytes
    uint8_t* dst_ptr;              // 8 bytes

    union
    {
        uint8_t* history;          // 8 bytes
        uint8_t* scratch;
    };
    
    //Sizes
    size_t   src_size;             // 8 bytes
    size_t   dst_size;             // 8 bytes

    //private state
    size_t   nHistoryBytes;        // 8 bytes, total decompressed stream bytes
    size_t   nBlockBytesDecoded;   // 8 bytes, total decompressed block  bytes per decode_block call
    
    //Lengths
    uint32_t nBlocksProcessed;     // 4 bytes, total compression blocks processed
    
    //"base" class
    xnz_bitstream bs;              // 40 or 52 bytes
    
    /********************************* RFC 1951 DEFLATE *********************************/ 
    
    //allocating circular history buffer of decoded bytes is now client responsibility 
    //as it is too large for the stack and also algorithm dependent 
    //uint8_t history[HISTORY_BUFFER_SIZE];  // 32768    bytes

    //RFC 1951 Algorithm Dependent Prefix Code Trees
    //dist tree defined first bc it is a multiple of 2^5 to maintain struct property alignment
    xnz_huff_node dist_tree[ N_FIXED_DIST_NODES ];   // 24 * 32  bytes
    xnz_huff_node   ll_tree[ N_FIXED_LL_NODES   ];   // 24 * 288 bytes
    //xnz_huff_node cl_tree[ N_DYNAMIC_CL_NODES ];   // 24 * 19  bytes

    //track dynamic streaming cl tree state
    uint32_t  dist_code_lengths[N_FIXED_DIST_CODES]; //store dist lengths decoded from cl tree
    uint32_t      ll_code_lengths[N_FIXED_LL_CODES]; //store ll   lengths decoded from cl treee

    //track prefix code unpack state for fixed and dynamic block types
    xnz_huff_node* ll_node;
    xnz_huff_node* dist_node;

    //track dynamic block cl_code lengths read + cl_codes read 
    uint32_t  lengths_read;  //# cl code lengths read at the beginning of block
    uint32_t  codes_read;    //# ll/dist lengths decoded from the cl tree
    uint32_t  last_symbol;   //the last cl symbol parsed when parsing the cl tree
    //uint32_t  clBitCount;  //# of bits parsed for all symbols in the cl tree (debug only)

    //These are no longer in use
    uint32_t  max_cl_code_length;
    uint32_t  max_ll_code_length;    // 4 bytes, when decoding streamed incomplete buffers these properties ensure
    uint32_t  max_dist_code_length;  // 4 bytes, the bitstream will be prevented from reading partial prefix codes

    //track dynamic block first 14 bytes
    uint32_t  hlit;
    uint32_t  hdist;
    uint32_t  hclen;

    //track current block type
    uint8_t   bfinal;
    uint8_t   btype;

}xnz_block_stream;

typedef xnz_block_stream xnz_compression_stream;

//#pragma mark -- CTConnection API Method Function Pointer Definitions
typedef xnz_status (*xnz_block_stream_func)(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags);

//forward declare xnz inflate block functions
static xnz_status xnz_inflate_raw_block(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags);
static xnz_status xnz_inflate_fixed_block(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags);
static xnz_status xnz_inflate_dynamic_block(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags);
static xnz_status xnz_inflate_custom_block(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags);
static xnz_status xnz_inflate_prefix_codes(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags);

static const xnz_block_stream_func XNZ_INFLATE_BLOCKS[XNZ_BLOCK_MAX] = 
{ 
    xnz_inflate_raw_block,
    xnz_inflate_fixed_block,
    xnz_inflate_dynamic_block,
    xnz_inflate_custom_block,
    xnz_inflate_prefix_codes 
};

//FYI:  client must free returned memory
static unsigned* xnz_convert_to_binary(unsigned* data, unsigned int dataSizeInBits)
{
    unsigned
        input = *data,
        n_bits = dataSizeInBits,
        * bits = (unsigned*)malloc(sizeof(unsigned) * n_bits),
        bit = 0;

    for (bit = 0; bit < n_bits; ++bit, input >>= 1)
        bits[bit] = input & 1;

    return bits;
}


static void xnz_print_huff_node(xnz_huff_node* node, xnz_uint2* code_table, const char* prefix)
{
    if (!node) {
        fprintf(stderr, " NULL NODE\n");
        return;
    }

    if (node->symbol != UINT32_MAX) 
        fprintf(stderr, "%s%d\n", prefix, node->symbol);
    else
    {
        fprintf(stderr, "%s\n", prefix); //print null terminated prefix str for current node

        //char* prefixStrL = alloca(strlen(prefix) + node->left->len  + 1 + 1);
        //char* prefixStrR = alloca(strlen(prefix) + node->right->len + 1 + 1);

        //convert big endian uint32_t to arrays bytes each representing a single bit
        //unsigned* prefixCodeBitsL = altz_convert_to_binary((unsigned*)&code_table[node->left->symbol],  sizeof(uint32_t) * 8);
        //unsigned* prefixCodeBitsR = altz_convert_to_binary((unsigned*)&code_table[node->right->symbol], sizeof(uint32_t) * 8);

        //allocate memory for children prefix strings
        char* prefixCodeStrL = alloca(strlen(prefix) + 4 + 1);  //numChars = numBits + colon + terminating character
        char* prefixCodeStrR = alloca(strlen(prefix) + 4 + 1); //numChars = numBits + colon + terminating character

        //copy parent node prefix str to child node prefix strings
        sprintf(prefixCodeStrL, "%.*s|   ", (int)strlen(prefix), prefix);
        sprintf(prefixCodeStrR, "%.*s|   ", (int)strlen(prefix), prefix);

        //uint32_t bit; //copy children prefix code bits to children prefix code strings
        //for (bit = node->left->len;  bit--;) sprintf(stderr, prefixCodeStrL + strlen(prefix) + (node->left->len  - bit), "%u", +prefixCodeBitsL[bit]); //fprintf(stderr, "%u", +bits[bit]);
        //for (bit = node->right->len; bit--;) sprintf(stderr, prefixCodeStrR + strlen(prefix) + (node->right->len - bit), "%u", +prefixCodeBitsR[bit]); //fprintf(stderr, "%u", +bits[bit]);

        //terminate children prefix code strings
        prefixCodeStrL[strlen(prefix) + 4] = '\0';
        prefixCodeStrR[strlen(prefix) + 4] = '\0';

        //free(prefixCodeBitsL);
        //free(prefixCodeBitsR);

        //recursively print children
        xnz_print_huff_node(node->left,  code_table, prefixCodeStrL);
        xnz_print_huff_node(node->right, code_table, prefixCodeStrR);
    }
}

#ifdef XNZ_HUFF_DEBUG
static void xnz_print_huff_tree(xnz_huff_node* root, xnz_uint2* code_table)
{
    //assert(root->len > 0);
    //assert(code_table[root->symbol].x == root->len);

    
    //unsigned* prefixCodeBits = altz_convert_to_binary((unsigned*)&code_table[0], sizeof(uint32_t) * 8);
    char* prefixCodeStr = "\0";// alloca(root->len + 1); //numChars = numBits + terminating character

    //uint32_t bit; for (bit = root->len; bit--;) sprintf(stderr, prefixCodeStr, "%u", +prefixCodeBits[bit]); //fprintf(stderr, "%u", +bits[bit]);
    //prefixCodeStr[root->len] = '\0';

    xnz_print_huff_node(root, code_table, prefixCodeStr);

    //free(prefixCodeBits);

}
#endif

static xnz_huff_node* xnz_add_huff_node(xnz_huff_node* tree, xnz_huff_node * root, uint32_t code_bits, uint32_t num_root_code_bits, uint32_t num_code_bits, uint32_t symbol, /*uint32_t treeDepth,*/ uint32_t* nodeCount)
{
    /*
    if len(code) == 0:
    if root is not None :
        return None
        return HuffmanTreeNode(symbol = symbol)
    else :
        bit = code[0]
        if root is None :
    root = HuffmanTreeNode()
        if bit == 0 :
            root.left = huffman_add_node(root.left, code[1:], symbol)
        else :
            root.right = huffman_add_node(root.right, code[1:], symbol)
            return root
    */

    if( num_code_bits == 0 ) //base recursion case (ie designate a leaf node for the corresponding symbol)
    {
        //all base recursion cases will require memory for a symbol leaf node
        tree[(*nodeCount)] = (xnz_huff_node){ symbol, 0, NULL, NULL };

        if (root) //if the tentative leaf we arrived at already has instantiated node memory...
        {
            if (root->symbol != UINT32_MAX) //the instantiated node memory has already been initialized
            {
                return &tree[(*nodeCount)++];

                if (root->symbol == symbol) assert(1 == 0); //sanity check

                //sort symbols with equal lengths in ascending order
                if (symbol < root->symbol) root->left  = &tree[(*nodeCount)++];
                else                       root->right = &tree[(*nodeCount)++];
            }
            else //the instantiated node memory has not been initialized
            {
                //this case should never happen
                assert(1 == 0);
                //return &tree[(*nodeCount)++];
                return NULL;
                //root->symbol = symbol;
                //assert(root->left == NULL);
                //assert(root->right == NULL);
            }
            return root;
        }
                
        //symbol leaf
        return &tree[(*nodeCount)++];
    }
    else
    {
        xnz_huff_node* rootNode = root;
        uint8_t bit = ( ((code_bits >> (num_code_bits - 1)) & 1) != 0); //get most significant bit of input prefix code
        
        if (!rootNode) //if the node passed in was null get memory for a new node
        {
            //initialize node memory to unassigned value UINT32_MAX
            //tree[(symbol * treeDepth) + (num_root_code_bits - num_code_bits)] = (xnz_huff_node){UINT32_MAX, num_code_bits, NULL, NULL};
            //rootNode = &tree[(symbol * treeDepth) + (num_root_code_bits - num_code_bits)];
            tree[(*nodeCount)] = (xnz_huff_node){ UINT32_MAX, num_code_bits, NULL, NULL };
            rootNode = &tree[(*nodeCount)];
            (*nodeCount)++;
        }

#ifdef XNZ_HUFF_DEBUG
        unsigned* prefixCodeBits = xnz_convert_to_binary((unsigned*)&code_bits, sizeof(uint32_t) * 8);
        fprintf(stderr, "Symbol[%d]: ", symbol);
        uint32_t i; for (i= num_code_bits; i--;) fprintf(stderr, "%u", +prefixCodeBits[i]);
        fprintf(stderr, "\n");
        free(prefixCodeBits);
#endif

        //eliminate most significant bit in the code to populate children
        if (bit == 0) rootNode->left  = xnz_add_huff_node(tree, rootNode->left,  code_bits /*|= (1 << (num_code_bits - 1))*/, num_root_code_bits, num_code_bits - 1, symbol, nodeCount);
        else          rootNode->right = xnz_add_huff_node(tree, rootNode->right, code_bits /*|= (1 << (num_code_bits - 1))*/, num_root_code_bits, num_code_bits - 1, symbol, nodeCount);
        
        return rootNode;
    }

    assert(1 == 0);
    return NULL;
}

//Build a huffman tree using the input code table
static xnz_huff_node* xnz_build_huff_tree(xnz_huff_node* tree, xnz_uint2* code_table, size_t code_table_len)// size_t treeDepth)
{
    
    /*
for i in range(len(code_table)) :
    num_bits, code_bits = code_table[i]
    if num_bits == 0 : continue //Code isn't used
    //Convert the integer code_bits to a list of bits (with the highest order bit first)
    code_sequence = [(code_bits >> (num_bits - j - 1) & 1) for j in range(num_bits)]
    root = huffman_add_node(root, code_sequence, i)
    if root is None : raise BuildHuffmanException("Symbol 0x%02x (code %s) terminates at an internal node" % (i, ''.join(int(b) for b in code_sequence)))
    */

    //assume tree memory for all nodes already exists and as such does not need to be allocated
    xnz_huff_node* root = NULL;
    //tree[0] = (xnz_huff_node){ UINT32_MAX, 0, NULL, NULL };
    //root = &tree[0];
    //assert(root); //root should be defined as input
    uint32_t nodeCount = 0;

    //i is the symbol
    uint32_t i; for (i = 0; i < code_table_len; i++)
    {
        uint32_t num_bits  = code_table[i].x;
        uint32_t code_bits = code_table[i].y; 
        if (num_bits == 0)
        {
            //Q:  We just populated the code table and x never equals 0 perhaps this is only used for compression case 2?
            //A:  Yes, it is observed this occurs for CL huff tree for dynamic type compression 2
            continue; //code is not used
        }

#ifdef XNZ_HUFF_DEBUG
        unsigned* prefixCodeBits = xnz_convert_to_binary((unsigned*)&code_bits,  sizeof(uint32_t) * 8);
        fprintf(stderr, "Code[%d]: ", i); 
        uint32_t bit; for (bit = num_bits; bit--;) fprintf(stderr, "%u", +prefixCodeBits[bit]);
        fprintf(stderr, "\n");
        free(prefixCodeBits);
#endif

        //Convert the integer code_bits to a list of bits (with the highest order bit first)
        //code_sequence = [(code_bits >> (num_bits - j - 1) & 1) for j in range(num_bits)];
        root = xnz_add_huff_node(tree, root, code_bits, num_bits, num_bits, i, &nodeCount);
        assert(root != NULL); //Exception!!!
        //raise BuildHuffmanException("Symbol 0x%02x (code %s) terminates at an internal node" % (i, ''.join(int(b) for b in code_sequence)))

    }
        
    return root;
}

/*
* def decode_huffman(stream, output_buffer, ll_tree, dist_tree):
    ll_path = []
    node = ll_tree
    if decode_blocks:
        print('%12s-- Decoded data --'%'')
    while 1:
        b = stream.read_bit()
        ll_path.append(b)
        if b == 0:
            node = node.left
        else:
            node = node.right
        if node is None:
            raise DecodingException("Reached invalid state while decoding (code lengths were probably encoded incorrectly)")
        if node.symbol == -1:
            continue

        # Terminal literal/length node
        symbol = node.symbol
        if decode_blocks:
            if symbol >= 33 and symbol <= 127:
                # If this symbol is a printable character, print both the hex and character representation
                print("%12s%s: 0x%02x (%s)"%('',''.join(str(b) for b in ll_path), symbol, chr(symbol)))
            else:
                print("%12s%s: 0x%02x"%('',''.join(str(b) for b in ll_path), symbol))
        if symbol == 256:
            break #End of stream marker
        if symbol > 256:
            # Symbols > 256 are length codes
            length_code = symbol
            if decode_blocks:
                print("%16s0x%02x is Length code %d"%('',symbol,symbol))
            num_extra_bits, lower_bound = length_codes[length_code]
            length_offset = stream.read_bits(num_extra_bits)
            length = length_offset + lower_bound
            if decode_blocks:
                print("%16s%d extra length bits: Offset %d. Total length %d"%('',num_extra_bits,length_offset, length))
            dist_path = []
            dist_node = dist_tree
            if dist_node is None:
                raise DecodingException("Can't decode length/distance pair: No distance codes exist")
            while dist_node.symbol == -1:
                b = stream.read_bit()
                dist_path.append(b)
                if b == 0:
                    dist_node = dist_node.left
                else:
                    dist_node = dist_node.right
            dist_symbol = dist_node.symbol
            if decode_blocks:
                print("%16sDistance code %s: %d"%('',''.join(str(b) for b in dist_path), dist_symbol))
            num_extra_bits, lower_bound = distance_codes[dist_symbol]
            distance_offset = stream.read_bits(num_extra_bits)
            distance = distance_offset + lower_bound
            if decode_blocks:
                print("%16s%d extra distance bits: Offset %d. Total distance %d"%('',num_extra_bits,distance_offset, distance))
            output_buffer.write_lz77_length_distance(length,distance)
            
        else:
            output_buffer.write_byte(symbol)
        node = ll_tree
        ll_path = []
*/

//static xnz_status xnz_inflate_prefix_codes(xnz_block_stream* stream, xnz_bitstream* bs, xnz_huff_node* ll_tree, xnz_huff_node* dist_tree, uint32_t flags, size_t * nBlockBytesDecoded)
static xnz_status xnz_inflate_prefix_codes(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags)

{
    //uint32_t      ll_path = 0;
    //uint32_t      llBitCount   = 0;
    //uint32_t      distBitCount = 0;
    //stream->ll_node = ll_tree;
    
    xnz_bs_size bs_offset = 4 * (1 - (flags & XNZ_STREAM_FINALIZE)); // 4 or 0
    stream->btype         = XNZ_BLOCK_CONTINUE;
    
    
    //read ll codes until ll_symbol == 256
    //process ll code
    //  ll_symbol > 256
    //      read any extra ll bits
    //      read bits until dist code is found
    //      read any extra dist bits
    //      get decoded bytes from history buffer
    //  ll_symbol < 256
    //      write decoded symbol
    //if ((stream->bs.position + stream->max_ll_code_length >= stream->bs.length)) assert(1 == 0);            //limit bitstream reads

    while( (stream->ll_node->symbol - 256) && ((stream->bs.pointer - stream->bs.buffer) + bs_offset < stream->bs.capacity)) //keep reading prefix codes until end of stream code reached
    {
        //when decoding streamed chunks of bytes, break early so as to avoid ending up halfway through a ll/dist code
        //WARNING:  nLimitBits = (max_ll_code_len + max_dist_code_len + max_dist_extra_bits) is insufficient for arithmetic modulo bitstream prefetch/accumulation size (eg 14 + 7 + 13 is >= 32)
        //if ( (stream->bs.position + nLimitBits > stream->bs.length) ) break;                  // break when remaining stream bits < greatest length of a prefix code in bits (results in png filtering artifacts)
        //if( ((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity) ) break;    // break when bitstream has prefetched the final bytes from available stream bytes
        //if (flags & XNZ_STREAM_FINALIZE) {}
        //else if (/*(stream->bs.position + nLimitBits >= stream->bs.length) && */((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity) ) break;
        //if ((stream->bs.position + stream->max_ll_code_length + 5 >= stream->bs.length)) assert(1 == 0);            //limit bitstream reads

        //if there is a midstream symbol waiting to be processed...
        if (stream->ll_node->symbol + 1 > 257 /*&& stream->ll_node->symbol != UINT32_MAX*/)                 // Symbols > 256 are length codes
        {
            stream->dist_node          = stream->dist_tree; //assert(stream->dist_node);                    //Can't decode length/distance pair: No distance codes exist

            uint32_t length_code       = stream->ll_node->symbol;                                           //get offset and range properties of length code from LUT to calculate the actual length 
            uint32_t ll_num_extra_bits = RFC_1951_FIXED_LENGTH_CODES[length_code-257].x;
            int32_t  ll_lower_bound    = RFC_1951_FIXED_LENGTH_CODES[length_code-257].y;

            //ensure thre are enough bits in stream remaining to read both ll_num_extra_bits and max_dist_code_length
            //if ((stream->bs.position + ll_num_extra_bits /* + stream->max_dist_code_length */ >= stream->bs.length)) assert(1 == 0);            //limit bitstream reads

            //read n bits corresponding to length offset, convert to 4 byte int type, calculate length
            int32_t length_offset      = ll_num_extra_bits > 0 ? xnz_bitstream_read_bits(bs, ll_num_extra_bits) : 0;
            int32_t length             = length_offset + ll_lower_bound; //insert debug statement           

            //ensure there are enough bits in stream remaining to read an entire dist code
            //if (flags & XNZ_STREAM_FINALIZE) {}
            //else if ((stream->bs.position + stream->max_dist_code_length >= stream->bs.length)) assert(1 == 0); //limit bitstream reads

            //build path of bits corresponding to distance symbol by appending bit to path
            //using the xnz_bitstream uint32_t accumulator to store the path instead of an intermediate dist_path var
            while (stream->dist_node->symbol == UINT32_MAX) { stream->dist_node = xnz_bitstream_read_bits(bs, 1) ? stream->dist_node->right : stream->dist_node->left; }

            uint32_t dist_symbol         = stream->dist_node->symbol; //assert(dist_symbol >= 0 && dist_symbol <= 29);      //get offset and range properties of dist code from LUT to calculate the actual distance
            uint32_t dist_num_extra_bits = RFC_1951_FIXED_DIST_CODES[dist_symbol].x;
            int32_t  dist_lower_bound    = RFC_1951_FIXED_DIST_CODES[dist_symbol].y;

            //ensure there are enough bits to read dist_num_extra bits
            //if ((stream->bs.position + dist_num_extra_bits >= stream->bs.length)) assert(1 == 0);         //limit bitstream reads

            //read n bits corresponding to distance offset, convert to 4 byte int type, calculate distance
            int32_t distance_offset      = dist_num_extra_bits > 0 ? xnz_bitstream_read_bits(bs, dist_num_extra_bits) : 0;
            int32_t distance             = distance_offset + dist_lower_bound; //insert debug statement             
            //fprintf(stderr, "match %d %d\n", length, distance);

            //Note: historyLen was initially defined as size_t (int64) which failed to subtract properly with distance (int32)
            int32_t historyLen = MIN((int32_t)stream->nHistoryBytes, XNZ_DEFLATE_BUFFER_SIZE); //assert(distance <= historyLen);
            int i; for (i = 0; i < length; i++)
            {
                historyLen = stream->nHistoryBytes % XNZ_DEFLATE_BUFFER_SIZE; //needs to be recalculated each loop iteration
                uint8_t pastHistoryByte = stream->history[(historyLen - distance) < 0 ? (historyLen - distance) + XNZ_DEFLATE_BUFFER_SIZE : (historyLen - distance)];
                stream->history[historyLen] = pastHistoryByte; stream->nHistoryBytes++; //write byte to end of circular history buffer
                stream->dst_ptr[stream->nBlockBytesDecoded++] = pastHistoryByte;        //write byte to stream decode output buffer
            }

            //reset bit array representing literal-length path and dist path for next loop iteration
            //ll_path = llBitCount = distBitCount = 0;
            stream->ll_node   = stream->ll_tree; //reserve traversal to root node
            stream->dist_node = NULL;            //stream->dist_tree;
            //stream->last_symbol = UINT32_MAX;
        }
        else if (stream->ll_node->symbol < 256)//write symbol byte to output buffer
        {   
            uint8_t symbol_byte = stream->ll_node->symbol & 0xFF;
            //TO DO: calculate cyclic rendundancy check value
            //if (!crc) crc = binascii.crc32(bbytes)
            //else      crc = binascii.crc32(bbytes, self.crc)
            stream->history[stream->nHistoryBytes % XNZ_DEFLATE_BUFFER_SIZE] = symbol_byte; stream->nHistoryBytes++;  //write byte to end of circular history bufferr
            stream->dst_ptr[stream->nBlockBytesDecoded++] = symbol_byte;                                              //write byte to stream decode output buffer
        
            //reset bit array representing literal-length path and dist path for next loop iteration
            //ll_path = llBitCount = distBitCount = 0;
            stream->ll_node   = stream->ll_tree; //reserve traversal to root node
            stream->dist_node = NULL;            //stream->dist_tree;
            //stream->last_symbol = UINT32_MAX;
        }
        
        //build path of bits corresponding to literal-length symbol by appending bit to path
        //using the xnz_bitstream uint32_t accumulator to store the path instead of an intermediate ll_path var
#ifdef XNZ_DEBUG
        uint8_t bit = xnz_bitstream_read_bit(bs); stream->ll_node = bit ? stream->ll_node->right : stream->ll_node->left; assert(stream->ll_node); fprintf(stderr, "%u", bit);
        if (stream->ll_node->symbol == UINT32_MAX) continue; //Continue until a Terminal literal / length node is found
        //assert(stream->ll_node->symbol >= 0 && stream->ll_node->symbol <= 285); fprintf(stderr, "\tll_symbol = %d\n", stream->last_symbol);
#else
        size_t remainingBits = stream->bs.length - stream->bs.position;
        while (stream->ll_node->symbol == UINT32_MAX && remainingBits--) { stream->ll_node = xnz_bitstream_read_bits(bs, 1) ? stream->ll_node->right : stream->ll_node->left; /*assert(stream->ll_node);*/ }
        //assert(stream->ll_node->symbol >= 0 && stream->ll_node->symbol <= 285); fprintf(stderr, "\tll_symbol = %d\n", stream->last_symbol);
#endif        

    }

#ifdef XNZ_BITSTREAM_DEBUG
    fprintf(stderr, "wrap bits = %d\n", stream->bs.position - stream->bs.length);
    xnz_bitstream_print(&stream->bs);
#endif

    return XNZ_STATUS_OK + !(stream->ll_node->symbol - 256); //block was either partially decoded or end of block was reached
}

static xnz_status xnz_inflate_raw_block(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags)
{
    assert(1 == 0);
    return XNZ_STATUS_ERROR;
}

static xnz_status xnz_inflate_fixed_block(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags)
{
    //Type 01: Block uses the built - in Huffman code to encode data
    int i = 0;

    // Mapping of code values (0 - 287) to (num_bits, code_bits) pairs
    // RFC 1951 gives this table of code lengths for literal-length prefix codes
    xnz_uint2 ll_codes[N_FIXED_LL_CODES];
    xnz_uint2 dist_codes[N_FIXED_DIST_CODES];

    for (; i < 144; i++) ll_codes[i] = (xnz_uint2){ 8, 0b00110000  + (i      ) };
    for (; i < 256; i++) ll_codes[i] = (xnz_uint2){ 9, 0b110010000 + (i - 144) };
    for (; i < 280; i++) ll_codes[i] = (xnz_uint2){ 7, 0b0000000   + (i - 256) };
    for (; i < 288; i++) ll_codes[i] = (xnz_uint2){ 8, 0b11000000  + (i - 280) };

    // Mapping of distance code values (0 - 31) to (num_bits, code_bits) pairs
    // Note that the distance code for the fixed Huffman code is just the regular
    // (big-endian) binary encoding of the values 0 - 31
    for (i=0; i < N_FIXED_DIST_CODES; i++) dist_codes[i] = (xnz_uint2){ 5, i };

    //zero memory for huffman tree nodes
    memset(stream->ll_tree,   0, N_FIXED_LL_CODES   * sizeof(xnz_huff_node));
    memset(stream->dist_tree, 0, N_FIXED_DIST_CODES * sizeof(xnz_huff_node));
    for (i = 0; i < N_FIXED_LL_CODES;   i++) stream->ll_tree[i].symbol   = UINT32_MAX;
    for (i = 0; i < N_FIXED_DIST_CODES; i++) stream->dist_tree[i].symbol = UINT32_MAX;
   
    //build the literal-length and distance huffman trees, respectively
    //from the code tables generated using the RFC "standard algorithm" for fixed type decompression 
    xnz_huff_node* ll_tree_root   = xnz_build_huff_tree(stream->ll_tree,   ll_codes,   N_FIXED_LL_CODES);
    xnz_huff_node* dist_tree_root = xnz_build_huff_tree(stream->dist_tree, dist_codes, N_FIXED_DIST_CODES);

    //assert(ll_tree_root == &stream->ll_tree[0]);
    //assert(dist_tree_root == &stream->dist_tree[0]);

#ifdef XNZ_HUFF_DEBUG
    //xnz_print_huff_tree(ll_tree_root, ll_codes);
    //xnz_print_huff_tree(dist_root, dist_codes);
#endif

    assert(1 == 0);
    //uint32_t nLimitBits = (flags & XNZ_STREAM_FINALIZE) ? 0 : (stream->max_ll_code_length + stream->max_dist_code_length);
    //return xnz_inflate_prefix_codes(stream, &stream->bs, &stream->ll_tree[0], &stream->dist_tree[0], flags, &stream->nBlockBytesDecoded);
    return xnz_inflate_prefix_codes(stream, &stream->bs, flags);
}


// This algorithm is based on the pseudocode in RFC 1951 (Step are numbered as per Section 3.2.2)
// It dynamically creates CL, LL and DIST {len, code} tables to be used to populate huffman trees for decoding
static xnz_uint2* code_lengths_to_code_table(uint32_t* code_lengths, xnz_uint2* code_table, uint32_t nCodeLengths, uint32_t * max_length)
{
    uint32_t i;

    /***
     *  Step 1
     *
     *  Count the number of codes for each code length.
     *  Let bl_count[N] be the number of codes of length N, N >= 1.
     ***/
    
        //  Find max code length in array of code_lengths
        for (i = 0; i < nCodeLengths; i++) { if (code_lengths[i] > *max_length) *max_length = code_lengths[i]; };

        //allocate an array of length counts (frequencies) of size max_length + 1
        *max_length += 1; // increment will be undone at the end of this function
        uint32_t* length_counts = (uint32_t*)alloca((*max_length) * sizeof(uint32_t));
        memset(length_counts, 0, (*max_length) * sizeof(uint32_t)); //zeroing memory is critical!

        //populate length_counts (frequencies)
        for (i = 0; i < nCodeLengths; i++) { if (code_lengths[i] > 0) length_counts[code_lengths[i]] += 1; }

    /***
     *  Step 2
     ***/

        uint32_t code = 0;
        length_counts[0] = 0;

        uint32_t * next_code = (uint32_t*)alloca((*max_length) * sizeof(uint32_t));
        memset(next_code, 0, (*max_length) * sizeof(uint32_t)); //zeroing memory is critical!
        for (i = 1; i < *max_length; i++) //i represents nbits
        {
            code = (code + length_counts[i - 1]) << 1;
            next_code[i] = code;
        }


    /***
     *  Step 3
     ***/

        //xnz_uint2* code_table = (xnz_uint2*)malloc(sizeof(xnz_uint2) * nCodeLengths);
        memset(code_table, 0, sizeof(xnz_uint2) * nCodeLengths); //zeroing memory is critical!
        for (i=0; i<nCodeLengths; i++)  
        { 
            uint32_t length = code_lengths[i];
            if (length != 0 )
            {
                code_table[i] = (xnz_uint2){length, next_code[length]};
                next_code[length] += 1;
            }
        }
    
    /***
     *  Return Output(s)
     ***/
        
        *max_length -= 1; // decrement to undo increment above
        return code_table;

}

static xnz_status xnz_inflate_dynamic_block(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags)
{
    int i;

    //RFC 1951 ordering of code lengths stored in dynamic block header
    uint32_t CL_code_length_encoding_order[N_DYNAMIC_CL_CODES] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

    //xnz_uint2 cl_codes[N_DYNAMIC_CL_CODES]         = {0};
    xnz_uint2  ll_codes[N_FIXED_LL_CODES]            = {0};
    xnz_uint2  dist_codes[N_FIXED_DIST_CODES]        = {0};

    //uint32_t cl_code_lengths[N_DYNAMIC_CL_CODES]   = {0};
    //uint32_t ll_code_lengths[N_FIXED_LL_CODES]     = {0};
    //uint32_t dist_code_lengths[N_FIXED_DIST_CODES] = {0};

    xnz_huff_node* cl_node                           = stream->dist_tree;         //the ptr we'll use to iterate cl symbols
    xnz_huff_node* cl_tree                           = stream->dist_tree;         //reuse memory to minimize stack pressure
    uint32_t*      cl_code_lengths                   = stream->dist_code_lengths; //reuse memory to minimize stack pressure
    xnz_uint2*     cl_codes                          = dist_codes;                //reuse memory to minimize stack pressure

    //uint32_t  max_cl_code_length = 0;
    //uint32_t  sum_cl_code_length = 0;
    
    //uint32_t max_ll_code_length   = 0;
    //uint32_t max_dist_code_length = 0;

    //track current block type
    stream->btype = 0x2;
    
    /*
    #First 14 bits: three size values(little endian)
    hlit = stream.read_bits(5) # ((number of LL codes) - 257)
    num_ll_codes = hlit + 257
    decode_print("Number of LL codes: %d (HLIT = %d)" % (num_ll_codes, hlit))
    hdist = stream.read_bits(5) # ((number of dist codes) - 1)
    num_dist_codes = hdist + 1
    decode_print("Number of dist codes: %d (HDIST = %d)" % (num_dist_codes, hdist))
    hclen = stream.read_bits(4) # ((number of code length codes) - 4)
    num_cl_codes = hclen + 4
    decode_print("Number of code length (CL) codes: %d (HCLEN = %d)" % (num_cl_codes, hclen))
    */

    //stream hlit, hdist and hclen values should only be greater than 0 between block start and block end
    if (stream->hlit == 0) //if this is set, then the first 14 bits have already been read from stream
    {
        stream->max_cl_code_length = 0; //on new block init this to 0
        //assert(bs->length - bs->position > 14);

        //ensure we have enough bytes to read the cl code length table at minimum 
        //before we start reading bits from stream at the beginning of a block
        //if this value is < 32 we don't need to use next_buffer bitstream queueing
        //if (/*!stream->bs.next_buffer &&*/ bs->length - bs->position < 14)
        if ((stream->bs.position + 14 >= stream->bs.length) && ((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity)) return XNZ_STATUS_OK; //ll/dist code lengths were only partially decoded

        stream->hlit  = xnz_bitstream_read_bits(bs, 5) + 257;
        stream->hdist = xnz_bitstream_read_bits(bs, 5) + 1;
        stream->hclen = xnz_bitstream_read_bits(bs, 4) + 4;

#if defined (XNZ_LOG_BLOCKS) || defined(XNZ_DEBUG) || defined(XNZ_HUFF_DEBUG)
        fprintf(stderr, "Number of LL codes: %d (HLIT)\n", stream->hlit);
        fprintf(stderr, "Number of dist codes: %d (HDIST)\n", stream->hdist);
        fprintf(stderr, "Number of code length (CL) codes: %d (HCLEN)\n", stream->hclen);
        assert(stream->hclen <= 19);
#endif
        //assert(bs->length - bs->position > stream->hclen * 3);// || bs->next_buffer);
        stream->lengths_read = 0; //prepare for reading cl symbol lengths    
        memset(cl_code_lengths, 0, N_DYNAMIC_CL_CODES * sizeof(uint32_t));
    }

    if( stream->lengths_read < stream->hclen )
    {
        /*
        #Next bits: num_cl_codes*3 bits (up to 19 3-bit CL code lengths)
        #The lengths are stored in a weird order:
        CL_code_length_encoding_order = [16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15]
        CL_code_lengths = [0]*19
        for idx in CL_code_length_encoding_order[0:num_cl_codes]:
            CL_code_lengths[idx] = stream.read_bits(3)
        decode_print("CL code lengths (0 - 18): " + ' '.join(str(i) for i in CL_code_lengths))
        CL_codes = code_lengths_to_code_table(CL_code_lengths)
        */

        //read hclen 3-bit CL table code lengths from the stream in the CL symbol ordering above
        for (i = stream->lengths_read; i < stream->hclen; i++)
        {
            //when decoding streamed chunks of bytes, break early so as to avoid ending up halfway through a cl code
            //if ((stream->bs.position + 3 > stream->bs.length))                         assert(1 == 0);    // break when remaining stream bits < length of a cl code length in bits (ie 3 bits)
            //if (((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity)) assert(1 == 0);    // break when bitstream has prefetched the final bytes from available stream bytes
            if ((stream->bs.position + 3 >= stream->bs.length) && ((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity)) return XNZ_STATUS_OK; //cl code lengths were only partially decoded
            cl_code_lengths[CL_code_length_encoding_order[stream->lengths_read++]] = xnz_bitstream_read_bits(bs, 3);
            //sum_cl_code_length += CL_code_lengths[CL_code_length_encoding_order[stream->lengths_read]];

#ifdef XNZ_DEBUG
            fprintf(stderr, "HCLEN[%d] = %d    ! ", CL_code_length_encoding_order[i], cl_code_lengths[CL_code_length_encoding_order[i]]);
            unsigned* codebits = xnz_convert_to_binary((unsigned*)(&cl_code_lengths[CL_code_length_encoding_order[i]]), 3);
            uint32_t bit; for (bit = 3; bit--;) fprintf(stderr, "%u", +codebits[bit]);
            free(codebits);
            fprintf(stderr, "\n");
#endif
        }

#ifdef XNZ_DEBUG
        //debug print
        fprintf(stderr, "CL code lengths (0 - 18):");
        for (i = 0; i < N_DYNAMIC_CL_CODES; i++) fprintf(stderr, " %d", cl_code_lengths[i]);
        fprintf(stderr, "\n");
#endif

        //generate code table for CL huff tree
        /*xnz_uint2* CL_codes = */code_lengths_to_code_table(cl_code_lengths, cl_codes, N_DYNAMIC_CL_CODES, &stream->max_cl_code_length);// hclen);

#ifdef XNZ_DEBUG
    //debug print
        fprintf(stderr, "CL codes:\n");
        for (i = 0; i < N_DYNAMIC_CL_CODES; i++)
        {
            //length, encoded_bits = CL_codes[i]
            if (cl_codes[i].x == 0) continue;

            fprintf(stderr, "    %d (%d): ", i, cl_codes[i].x);// binary_string_big_endian(encoded_bits, length))
            unsigned* codebits = xnz_convert_to_binary((unsigned*)(&cl_codes[i].y), cl_codes[i].x);
            uint32_t bit; for (bit = cl_codes[i].x; bit--;) fprintf(stderr, "%u", +codebits[bit]);
            free(codebits);
            fprintf(stderr, "\n");
        }
#endif

        //init values to prepare for reading cl tree
        stream->codes_read  = 0;
        stream->last_symbol = UINT32_MAX;
        //stream->clBitCount = 0;

        //build_huffman_tree(CL_codes);
        memset(cl_tree, 0, N_DYNAMIC_CL_CODES * sizeof(xnz_huff_node));           //zero memory for huffman tree nodes
        for (i = 0; i < N_DYNAMIC_CL_CODES; i++) cl_tree[i].symbol = UINT32_MAX;  //initialize nodes with "uninitialized" value
        xnz_huff_node* CL_tree_root = xnz_build_huff_tree(cl_tree, cl_codes, N_DYNAMIC_CL_CODES);// hclen);        
        //assert(CL_tree_root == &cl_tree[0]);

        memset(stream->dist_code_lengths, 0, N_FIXED_DIST_CODES * sizeof(uint32_t));
        memset(stream->ll_code_lengths,   0, N_FIXED_LL_CODES   * sizeof(uint32_t));

        //the length of all symbols in the symbol cl code length table can't be estimated
        //but we can ensure that the the bitstream has at least bits for the sum all symbol lengths or max_cl_code_length...
        //if this value is < 32 we don't need to use next_buffer bitstream queueing
        if ((stream->bs.position + stream->max_cl_code_length + 7 >= stream->bs.length) && ((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity)) return XNZ_STATUS_OK;

    }
    //assert(stream->max_cl_code_length > 0);

    //Next bits: (num_ll_codes + num_dist_codes) code lengths for the LL and distance codes (both codes are encoded together, using the CL code)
    //decode ll_code_lengths and dist_code_lengths using CL_code huff tree
    while ((stream->codes_read < stream->hlit) && ((stream->bs.pointer - stream->bs.buffer) + 4 < stream->bs.capacity))
    {
        uint32_t repeat_count = 0;
        size_t   remainingBits = stream->bs.length - stream->bs.position;
        cl_node = &cl_tree[0]; //reset

        //build path of bits corresponding to literal-length symbol by appending bit to path
        //using the xnz_bitstream uint32_t accumulator to store the path instead of an intermediate ll_path var
        while (cl_node->symbol == UINT32_MAX && remainingBits--) { cl_node = xnz_bitstream_read_bits(bs, 1) ? cl_node->right : cl_node->left; /*assert(cl_node);*/ }

        if (cl_node->symbol < 16) // The symbol is an actual length
        {
            stream->last_symbol = cl_node->symbol; //set last symbol to current symbol 
            repeat_count = 1;
        }
        else if (cl_node->symbol == 16)
        {
            //stream->last_symbol = stream->last_symbol; //#Leave last_symbol unchanged
            repeat_count = 3 + xnz_bitstream_read_bits(bs, 2); // Repeat the previous symbol between 3 and 6 times based on a two bit value
        }
        else if (cl_node->symbol == 17)
        {
            stream->last_symbol = 0; //set last symbol to 0
            repeat_count = 3 + xnz_bitstream_read_bits(bs, 3); //# Repeat a zero length between 3 and 10 times based on a three bit value
        }
        else if (cl_node->symbol == 18)
        {
            stream->last_symbol = 0; //set last symbol to 0
            repeat_count = 11 + xnz_bitstream_read_bits(bs, 7); //# Repeat a zero length between 11 and 138 times based on a seven bit value
        }

        for (i = 0; i < repeat_count; i++) stream->ll_code_lengths[stream->codes_read + i] = stream->last_symbol;
        stream->codes_read += repeat_count;

        //when decoding streamed chunks of bytes, break early so as to avoid ending up halfway through a cl code
        //if ( (stream->bs.position + max_cl_code_length > stream->bs.length) )       assert(1==0);    // break when remaining stream bits < greatest length of a cl code in bits
        //if( ((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity) ) assert(1==0);    // break when bitstream has prefetched the final bytes from available stream bytes
        //if (/*(stream->bs.position + stream->max_cl_code_length + 7 >= stream->bs.length) && */((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity)) return XNZ_STATUS_OK; //ll/dist code lengths were only partially decoded

    }
    if (stream->codes_read < stream->hlit) return XNZ_STATUS_OK;
    //assert(stream->codes_read >= stream->hlit);

    while((stream->codes_read < stream->hlit + stream->hdist) && ((stream->bs.pointer - stream->bs.buffer) + 4 < stream->bs.capacity))
    {
        uint32_t repeat_count = 0;
        size_t   remainingBits = stream->bs.length - stream->bs.position;
        cl_node = &cl_tree[0]; //reset

        //build path of bits corresponding to literal-length symbol by appending bit to path
        //using the xnz_bitstream uint32_t accumulator to store the path instead of an intermediate ll_path var
        while (cl_node->symbol == UINT32_MAX && remainingBits--) { cl_node = xnz_bitstream_read_bits(bs, 1) ? cl_node->right : cl_node->left; /*assert(cl_node);*/ }

        if (cl_node->symbol < 16) // The symbol is an actual length
        {
            stream->last_symbol = cl_node->symbol; //set last symbol to current symbol 
            repeat_count = 1;
        }
        else if (cl_node->symbol == 16)
        {
            //stream->last_symbol = stream->last_symbol; //#Leave last_symbol unchanged
            repeat_count = 3 + xnz_bitstream_read_bits(bs, 2); // Repeat the previous symbol between 3 and 6 times based on a two bit value
        }
        else if (cl_node->symbol == 17)
        {
            stream->last_symbol = 0; //set last symbol to 0
            repeat_count = 3 + xnz_bitstream_read_bits(bs, 3); //# Repeat a zero length between 3 and 10 times based on a three bit value
        }
        else if (cl_node->symbol == 18)
        {
            stream->last_symbol = 0; //set last symbol to 0
            repeat_count = 11 + xnz_bitstream_read_bits(bs, 7); //# Repeat a zero length between 11 and 138 times based on a seven bit value
        }

        for (i = 0; i < repeat_count; i++) stream->dist_code_lengths[stream->codes_read - stream->hlit + i] = stream->last_symbol;
        stream->codes_read += repeat_count;

        //when decoding streamed chunks of bytes, break early so as to avoid ending up halfway through a cl code
        //if ( (stream->bs.position + max_cl_code_length > stream->bs.length) )       assert(1==0);    // break when remaining stream bits < greatest length of a cl code in bits
        //if( ((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity) ) assert(1==0);    // break when bitstream has prefetched the final bytes from available stream bytes
        //if (/*(stream->bs.position + stream->max_cl_code_length + 7 >= stream->bs.length) && */((stream->bs.pointer - stream->bs.buffer) + 4 >= stream->bs.capacity)) return XNZ_STATUS_OK; //ll/dist code lengths were only partially decoded
    }
    if (stream->codes_read < stream->hlit + stream->hdist) return XNZ_STATUS_OK;
    //assert(stream->codes_read == stream->hlit + stream->hdist);
    stream->last_symbol = UINT32_MAX;

    //fprintf(stderr, "CL Code Symbol Table size in bits: %d\n", stream->clBitCount);

    //After we've finished reading the CL code table we have enough information to construct the ll and dist huff trees
    //so we don't have to worry about reading anymore bits from the stream until we've reached inflate_prefix_codes

    /*xnz_uint2* ll_codes = */  code_lengths_to_code_table(stream->ll_code_lengths,   ll_codes,   N_FIXED_LL_CODES,   &stream->max_ll_code_length);
    /*xnz_uint2* dist_codes = */code_lengths_to_code_table(stream->dist_code_lengths, dist_codes, N_FIXED_DIST_CODES, &stream->max_dist_code_length);

#ifdef XNZ_DEBUG
    fprintf(stderr, "LL codes:\n");
    for (i=0; i< N_FIXED_LL_CODES; i++)
    { 
        //length, encoded_bits = ll_codes[i]
        if (ll_codes[i].x == 0) continue;

        fprintf(stderr, "    %d: ", i);// binary_string_big_endian(encoded_bits, length))
        unsigned* codebits = xnz_convert_to_binary((unsigned*)(&ll_codes[i].y), ll_codes[i].x);
        uint32_t bit; for (bit = ll_codes[i].x; bit--;) fprintf(stderr, "%u", +codebits[bit]);
        free(codebits);
        fprintf(stderr, "\n");
    }

    fprintf(stderr, "dist codes:");
    for (i = 0; i < N_FIXED_DIST_CODES; i++)
    {
        //length, encoded_bits = dist_codes[i]
        if (dist_codes[i].x == 0) continue;

        fprintf(stderr, "    %d: ", i);// binary_string_big_endian(encoded_bits, length))
        unsigned* codebits = xnz_convert_to_binary((unsigned*)(&dist_codes[i].y), dist_codes[i].x);
        uint32_t bit; for (bit = dist_codes[i].x; bit--;) fprintf(stderr, "%u", +codebits[bit]);
        free(codebits);
        fprintf(stderr, "\n");
    }
#endif

    //build the literal-length and distance huffman trees, respectively
    //from the code tables generated using the RFC "standard algorithm" for fixed type decompression 

    memset(stream->dist_tree, 0, N_FIXED_DIST_CODES * sizeof(xnz_huff_node));                               //zero memory for huffman tree nodes
    for (i = 0; i < N_FIXED_DIST_CODES; i++) stream->dist_tree[i].symbol = UINT32_MAX;                      //initialize nodes with "uninitialized" value
    xnz_huff_node* dist_tree_root = xnz_build_huff_tree(stream->dist_tree, dist_codes, N_FIXED_DIST_CODES);

    memset(stream->ll_tree, 0, N_FIXED_LL_CODES * sizeof(xnz_huff_node));
    for (i = 0; i < N_FIXED_LL_CODES;   i++) stream->ll_tree[i].symbol   = UINT32_MAX;
    xnz_huff_node* ll_tree_root = xnz_build_huff_tree(stream->ll_tree, ll_codes, N_FIXED_LL_CODES);

    //assert(ll_tree_root == &stream->ll_tree[0]);
    //assert(dist_tree_root == &stream->dist_tree[0]);
      
    //assert(stream->max_ll_code_length   > 0);
    //assert(stream->max_dist_code_length > 0);

    stream->ll_node   = stream->ll_tree;
    stream->dist_node = NULL;  //this gets set in the prefix_codes func
    stream->hlit = stream->hdist = stream->hclen = stream->codes_read = stream->lengths_read = 0;   //imperative that we set these back to 0 on block end

    //uint32_t nLimitBits = (flags & XNZ_STREAM_FINALIZE) ? 0 : (stream->max_ll_code_length + stream->max_dist_code_length); //add dist max extra bits
    //return xnz_inflate_prefix_codes(stream, &stream->bs, &stream->ll_tree[0], &stream->dist_tree[0], flags, &stream->nBlockBytesDecoded);
    return xnz_inflate_prefix_codes(stream, &stream->bs, flags);

}

static xnz_status xnz_inflate_custom_block(xnz_block_stream* stream, xnz_bitstream* bs, uint32_t flags)
{
    assert(1==0);
    return XNZ_STATUS_ERROR;
}

/* * *
*  Rule #1:  Anything considered a "number" was pushed into the compressed stream with least significant bit first:      b0, b1, b2, b3, b4, b5, b6, b7 ...
*            Anything considered a "code"   was pushed into the compressed stream with most  significant bit first:  ... b7, b6, b5, b3, b4, b2, b1, b0
*
*  Arrange / Read src block buffer as bitstream in order as follows :
*
*      ... BYTE 2 | BYTE 1 | BYTE 0
*
*  BYTE 2:  HCLEN[3,3] | 3 bit code lengths that encode the Huffman that is used to compress the other two Huffman trees
*  BYTE 1:  HDIST | HCLEN[0,2]
*  BYTE 0:  HLIT | BTYPE | BFINAL
*
*  uint8_t hlit;   // # of literals/length codes - 257, 5 bits
*  uint8_t hdist;  // # of distance codes - 1, 5 bits
*  uint8_t hclen;  // # of length codes that are used to encode the Huffman tree that will encode the other 2 trees - 4, 4 bits
* * */
static uint8_t xnz_inflate_block_header(xnz_block_stream* stream, uint8_t * bfinal)
{
    //Process the first byte independently from the stream
    //uint8_t hlit   = (stream->src_ptr[0] & 0b11111000) >> 3; //5 bits
    //uint8_t btype  = (stream->src_ptr[0] & 0b00000110) >> 1; //2 bits
    //uint8_t bfinal = (stream->src_ptr[0] & 0b00000001) >> 0; //1 bit

    /*
    //Observe binary
    unsigned* codebits = xnz_convert_to_binary((unsigned*)stream->src_buf, 3*8);
    uint32_t bit; for (bit = 3 * 8; bit--;)
    {
        if (bit % 8 == 7) fprintf(stderr, " ");
        fprintf(stderr, "%u", +codebits[bit]);
    }
    free(codebits);
    fprintf(stderr, "\n");
    */
    
    //Process first 3 bytes of stream
    //uint8_t bfinal = xnz_bitstream_read_bit(bs);
    //uint8_t btype  = xnz_bitstream_read_bits(bs, 2);

    /*
    //Debug bitstream
    int i; for (i = 0; i < 3; i++)
    {
        uint8_t bit = xnz_bitstream_read_bits(&bs, 8); //stream->src_buf[0] & 0x01;

        fprintf(stderr, " ");

        fprintf(stderr, "%u", bit);
        //uint8_t bit = xnz_bitstream_read_bits(&bs, 2); //stream->src_buf[0] & 0x01;
        //fprintf(stderr, "%u", bit >> 1);
        //fprintf(stderr, "%u", bit & 0x01);
    }
    */

    
    //Read first 3 bytes DEFLATE block type
    *bfinal = xnz_bitstream_read_bits(&stream->bs, 1);
    return    xnz_bitstream_read_bits(&stream->bs, 2);
}


static xnz_status xnz_inflate_block(xnz_block_stream* stream, uint32_t flags, uint8_t btype)
{
    xnz_status status          = XNZ_STATUS_ERROR; //assume error
    stream->nBlockBytesDecoded = 0;                //must be reset each block or stream pump
    //uint32_t nLimitBits = (flags & XNZ_STREAM_FINALIZE) ? 0 : (stream->max_ll_code_length + stream->max_dist_code_length);

    //Block Decode Event Switch
    /*
         if (btype == 0x0) status = xnz_inflate_raw_block(stream, &stream->bs, flags);     //raw      [bytes]
    else if (btype == 0x1) status = xnz_inflate_fixed_block(stream, &stream->bs, flags);   //fixed    [huff codes]
    else if (btype == 0x2) status = xnz_inflate_dynamic_block(stream, &stream->bs, flags); //dynamic  [huff codes]
    else if (btype == 0x3) status = xnz_inflate_custom_block(stream, &stream->bs, flags);  //custom   [compression TBD]
    else if (btype == 0x4) status = xnz_inflate_prefix_codes(stream, &stream->bs, flags);  //continue [block decode]
    //status = xnz_inflate_prefix_codes(stream, &stream->bs, &stream->ll_tree[0], &stream->dist_tree[0], flags, &stream->nBlockBytesDecoded);
    else if (btype != 0x4) assert(1==0);                                                   //unknown  [error]
    */

    status = XNZ_INFLATE_BLOCKS[btype](stream, &stream->bs, flags); //Optimized Block Decode Event Switch
    stream->dst_ptr += stream->nBlockBytesDecoded;                  //update dst_ptr for client feedback indicating # decompressed bytes written
    stream->src_ptr += stream->bs.pointer - stream->bs.buffer;      //update src_ptr for client feedback indicating #   compressed bytes consumed
    return status;                                                  //(status == XNZ_STATUS_OK && bfinal) ? XNZ_STATUS_END : status;
}

/*
static xnz_status xnz_block_stream_init(uint8_t* src_buf, size_t src_size, uint8_t* dst_buf, size_t dst_size)
{

    return XNZ_STATUS_OK;
}
*/

//XNZ Public Reentrant Stream API [will internally use the XNZ Block Stream API to decode non-contiguous buffers of DEFLATE blocks]
static xnz_status xnz_stream_inflate(xnz_block_stream* stream, uint32_t flags)
{
    xnz_status status            = XNZ_STATUS_ERROR;
    uint8_t    bfinal            = 0; //block final flag
    uint8_t    btype             = 0; //block type enum

    size_t     nBytesDecoded     = 0; // total bytes decoded as a result of this call

    //capture dst_buf on entry
    uint8_t*   dst_buf           = stream->dst_ptr;
    size_t     dst_size          = stream->dst_size;

    //assert(stream->src_ptr);  //invalid if no compressed src buffer was passed to the bitstream
    //assert(stream->history);  //invalid if no memory for RFC 1951 history buffer available

    //Bitstream Event Loop Switch
    //  Key off bitstream state to indicate how decoding of blocks in stream should progress
    /*
    if( !stream->bs.buffer )
    {
        //on first client call to xnz_stream_inflate the bitstream needs to be initialized
        
        // Initialize bit-stream using client provided compressed byte-stream ptr
        //    for blocks after the first block offset into the bitstream needs to be maintained
        //    bitstream inherently relies on uint32 (can't pass size_t)
        xnz_bitstream_attach_prefetch(&stream->bs, stream->src_ptr, (uint32_t)stream->src_size);

        //  If we are at the beginning of a deflate block read the first 3 bits of the block header then call inflate_block
        fprintf(stderr, "xnz_stream_inflate read header bits\n");
        bfinal = xnz_bitstream_read_bit(&stream->bs);     // read final block bit
        btype  = xnz_bitstream_read_bits(&stream->bs, 2); // read block type bit
    }
    else if*/ //assert(stream->bs.buffer != stream->src_ptr);
    {
        uint32_t stream_pos = stream->bs.position; //grab the bitstream position before it gets overwritten by reattach

        //Initialize/Reinitialize bit-stream using client updated compressed byte-stream ptr:
        //  on subsequent clients calls to xnz_stream_inflate the bistream needs to be
        //  properly "reinitialized" without overwriting the previously prefetched stream bits
        xnz_bitstream_attach(&stream->bs, stream->src_ptr, (uint32_t)stream->src_size);

        //If previous call to inflate_block exhausted all stream bytes without reaching end of block
        //then continue decoding the current block, otherwise start decoding a new block
#ifdef XNZ_DEBUG
        if(stream_pos == 0)
        {
            fprintf(stderr, "xnz_stream_inflate read header bits (finalize = %d)\n", flags & XNZ_STREAM_FINALIZE);
            //If we are at the beginning of a deflate block read the first 3 bits of the block header then call inflate_block
            bfinal = xnz_bitstream_read_bit(&stream->bs);     // read final block bit
            btype  = xnz_bitstream_read_bits(&stream->bs, 2); // read block type  bit
        }
        else
        {
            fprintf(stderr, "xnz_stream_inflate continue (finalize = %d)\n", flags & XNZ_STREAM_FINALIZE);
            bfinal = flags & XNZ_STREAM_FINALIZE;
            btype  = stream->btype;// XNZ_BLOCK_CONTINUE;
        }
#else
        stream->bfinal = stream_pos ? stream->bfinal : xnz_bitstream_read_bits(&stream->bs, 1);      // read final block bit  or client flag
                btype  = stream_pos ? stream->btype  : xnz_bitstream_read_bits(&stream->bs, 2);  // read block type  bits or current stream block type
#endif
    }
    /*
    else
    {
        //fprintf(stderr, "xnz_stream_inflate finalize\n");
        assert(1 == 0);
        //If the same buffer ptr was passed to xnz_stream_deflate then continue decoding the same block at the current bit-stream position
        //TO DO: ensure this case unequivocally only occurs when the client is reentering after the final block has returned STATUS_OK
        assert(flags & XNZ_STREAM_FINALIZE); //For parity with OSX API client should have passed finalize flag for every ending call to xnz_stream_inflate
        flags  = XNZ_STREAM_FINALIZE;        //FINALIZE will ensure  reading all the way to the end of stream in inflate_prefix_codes
        btype  = XNZ_BLOCK_CONTINUE;        //CONTINUE will prevent reading a block header to construct huffman trees
        bfinal = 1;                          //BFINAL   will prevent entering the final while loop below
    }
    */

    //  Whether the bitstream position is at beginning of a deflate block or in the middle of one stream bits are always decoded by calling inflate_block which returns:
    //  STATUS_OK:  as many stream bits as possible were decompressed to bytes (the current stream bit position is in the middle of a block)
    //  STATUS_END: all bits in the block were decompressed                    (the current stream bit position is at the end    of a block)
    status = xnz_inflate_block(stream, flags, btype);                          // ambiguous block start
    //assert(status != XNZ_STATUS_ERROR);                                        // expect STATUS_OK or STATUS_END

    nBytesDecoded += (stream->dst_ptr - (dst_buf + nBytesDecoded));
#ifdef XNZ_LOG_BLOCKS
    fprintf(stderr, "Block[%u] = %zu bytes decoded (status = %d)\n", stream->nBlocksProcessed, nBytesDecoded, status);
#endif
        
    //inflate RFC 1951 blocks until compressed stream bits are exhausted or final block is decoded
    while( status == XNZ_STATUS_END && !stream->bfinal )
    {
        stream->nBlocksProcessed += 1; //increment block when xnz_inflate_block tells us it has finished decoding a block
        //return xnz_stream_inflate(stream, flags); //iterate stream inflate again; DONT USE THIS
        
        //some minimum threshold is needed to guarantee enough bytes are available to build huffman trees
        //if (stream->nBlocksProcessed == 104) xnz_bitstream_print(&stream->bs);
        /*
        if (stream->bs.length - stream->bs.position < 14 + 19 * 3 + 512)
        {
            //reduce capacity for next block start
            stream->bs.length   -= stream->bs.position;
            stream->bs.position -= stream->bs.position;

            return XNZ_STATUS_OK;
        }
        */

        //stream src ptr remains the same throughout the duration of a single xnz_stream_inflate call
        //stream.src_ptr = (uint8_t*)&chunk->blob;
        //stream.src_size = xnz_be32toh(chunk->length);
        
        //stream dst ptr holding decoded bytes gets updated to prepare for next block decode call
        stream->dst_ptr  = dst_buf  + nBytesDecoded;
        stream->dst_size = dst_size - nBytesDecoded;

        //decode next DEFLATE block
        stream->bfinal = xnz_bitstream_read_bits(&stream->bs, 1); // read final block bit
        btype  = xnz_bitstream_read_bits(&stream->bs, 2);         // read block type bit
        status = xnz_inflate_block(stream, flags, btype);         // definitive block start
        //assert(status != XNZ_STATUS_ERROR);                     // expect STATUS_OK or STATUS_END

#ifdef XNZ_LOG_BLOCKS
        fprintf(stderr, "Block[%u] = %lld bytes decoded (status = %d)\n", stream->nBlocksProcessed, (stream->dst_ptr - (dst_buf + nBytesDecoded)), status);
#endif
        nBytesDecoded += (stream->dst_ptr - (dst_buf + nBytesDecoded));
    }

    //for next reentry, we need some way to indicate whether we ended previous call mid-block or end of block
    //if( status == XNZ_STATUS_END ) stream->bs.position = 0;
    stream->bs.position = (1 - status) * stream->bs.position + status * stream->bs.position;

    return status;
}

//XNZ Public Oneshot Buffer API [will internally use the XNZ Block Stream API to decode an entire contiguous buffer of DEFLATE blocks]
static size_t xnz_inflate_buffer(uint8_t* src_buf, size_t src_size, uint8_t* dst_buf, size_t dst_size, uint8_t * history_buffer)
{
    xnz_status status         = XNZ_STATUS_ERROR;
    uint8_t    bfinal         = 0; //block final flag
    uint8_t    btype          = 0; //block type enum
    
    size_t nBytesDecoded      = 0; // total bytes decoded throughout this call
    //size_t nBlocksProcessed   = 0;

    xnz_block_stream stream = {0};
    //status = xnz_block_stream_init(&stream, COMPRESSION_STREAM_DECODE, COMPRESSION_ZLIB);
    //assert(status != COMPRESSION_STATUS_ERROR);
    stream.src_ptr  = src_buf;
    stream.src_size = src_size - 4; //subtract zlib trailer
    stream.dst_ptr  = dst_buf;
    stream.dst_size = dst_size;
    stream.history = history_buffer ? history_buffer : alloca(XNZ_DEFLATE_BUFFER_SIZE);

    // Initialize bit-stream using client provided compressed byte-stream ptr
    //    for blocks after the first block offset into the bitstream needs to be maintained
    //    bitstream inherently relies on uint32 (can't pass size_t)
    xnz_bitstream_attach(&stream.bs, stream.src_ptr, (uint32_t)stream.src_size);

    // Decode first DEFLATE block
    bfinal = xnz_bitstream_read_bits(&stream.bs, 1);                    // read final block bit
    btype  = xnz_bitstream_read_bits(&stream.bs, 2);                    // read block type bit
    status = xnz_inflate_block(&stream, XNZ_STREAM_FINALIZE, btype);    // definitive block start
    assert(status != XNZ_STATUS_ERROR);                                 // expected STATUS_END
    assert(status == XNZ_STATUS_END);                                   // decode to end of block always expected
    
    nBytesDecoded += (stream.dst_ptr - (dst_buf + nBytesDecoded));
    stream.nBlocksProcessed += 1;

#ifdef XNZ_LOG_BLOCKS
    fprintf(stderr, "Block[%u] = %zu bytes decoded\n", stream.nBlocksProcessed-1, nBytesDecoded);
#endif

    // Decode DEFLATE blocks until compressed bytes are exhausted or final block is decoded
    while( status == XNZ_STATUS_END && !bfinal )
    {
        //stream src ptr remains the same throughout entire oneshote decode call
        //stream.src_ptr = (uint8_t*)&chunk->blob;
        //stream.src_size = xnz_be32toh(chunk->length);
        
        //stream dst ptr holding decoded bytes gets updated to prepare for next block decode call
        stream.dst_ptr  = dst_buf  + nBytesDecoded;
        stream.dst_size = dst_size - nBytesDecoded;

        //decode next DEFLATE block
        bfinal = xnz_bitstream_read_bits(&stream.bs, 1);                 // read final block bit
        btype  = xnz_bitstream_read_bits(&stream.bs, 2);                 // read block type bit
        status = xnz_inflate_block(&stream, XNZ_STREAM_FINALIZE, btype); // definitive block start
        assert(status != XNZ_STATUS_ERROR);                              // expected status end
        assert(status == XNZ_STATUS_END);                                // decode to end of block always expected

        nBytesDecoded += (stream.dst_ptr - (dst_buf + nBytesDecoded));
        stream.nBlocksProcessed += 1;
        
#ifdef XNZ_LOG_BLOCKS
        fprintf(stderr, "Block[%u] = %zu bytes decoded\n", stream.nBlocksProcessed-1, nBytesDecoded);
#endif
        
    }
    
    return nBytesDecoded;
}

#ifdef __cplusplus
}
#endif

#endif
