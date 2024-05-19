//
//  xnz_icc_profile.h
//  crImage
//
//  Created by Joe Moulton on 3/23/24.
//  Copyright © 2024 Abstract Embedded. All rights reserved.
//


#ifndef xnz_icc_profile_h
#define xnz_icc_profile_h

//Header Includes
#include "crSystem.h" //for cr_file

//Implementation Includes
//#include <endian.h>

#include "stdlib.h"
#include "string.h"

#include "xnz.h"

/***
 ***    ICC PROFILE HEADER STRUCT
 ***/

#define XNZ_ICC_HEADER_FIELDS \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint32_t, 32, 1,  size, size, "%d") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint32_t, 32, 1,  type, type, "%d") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  4,  version, version[4], "%u") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  4,  class, class[4], "%.4s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  4,  colorspace, colorspace[4], "%.4s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  4,  PCS, PCS[4], "%.4s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  12, dateCreated, dateCreated[12], "%s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint32_t, 32, 1,  acsp, acsp, "%x") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  4,  platform, platform[4], "%.4s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint32_t, 32, 1,  flags, flags, "%d") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  4,  manufacturer, manufacturer[4], "%.4s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  4,  model, model[4], "%.4s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint64_t, 64, 1,  attributes, attributes, "%.4s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint32_t, 32, 1,  renderingIntent, renderingIntent, "%x") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint32_t, 32, 3,  nCIEXYZ, nCIEXYZ[3], "%d") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  4,  creatorSignature, creatorSignature[4], "%.4s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  16, ID, ID[16], "%.16s") \
XNZ_ICC_HEADER_FIELD(XNZ_ICC_HEADER, uint8_t,  8,  28, reserved, reserved[28], "%s")

//--- define the structure, the X macro will be expanded once per field
XNG_IMAGE_DECLSPEC  XNG_IMAGE_ALIGN(1) typedef struct XNG_IMAGE_PACK_ATTRIBUTE XNZ_ICC_HEADER
{
//the compiler will expand each field in the XNZ_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNZ_ICC_HEADER_FIELD(xnz_flt_record_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
    XNZ_ICC_HEADER_FIELDS
#undef XNZ_ICC_HEADER_FIELD
} XNZ_ICC_HEADER;

XNG_IMAGE_DECLSPEC XNG_IMAGE_ALIGN(1) typedef struct XNG_IMAGE_PACK_ATTRIBUTE XNZ_ICC_TAG
{
    uint8_t  name[4];
    uint32_t offset;
    uint32_t length;
}XNZ_ICC_TAG;


typedef struct XNZ_ICC_PROFILE
{
    union {
        struct {
            union{
                XNZ_ICC_HEADER*         header;
                void*                  buffer;
            };
            size_t size;
        };
        CRFile file;
    };
    
    XNZ_ICC_TAG* tags;
    uint32_t    nTags;

}XNZ_ICC_PROFILE;

//serialize record data
static void xnz_icc_header_serialize(void* icc_struct_data);

//print record data (DEBUG)
static void xnz_icc_header_print(void * icc_struct_data);
//void xnz_flt_group_record_print(void * flt_record_data);

#pragma mark -- SERIALIZE

/***
 ***    UNIVERSAL SERIALIZE FUNCTION FOR ALL RECORDS DATA FIELDS PARSED BY X-MACROS
 ***
 ***    Converts FLT Record Big Endian Data Types to host format (e.g. Intel = Little Endian) using system level conversion calls
 ***/
static void xnz_icc_struct_field_serialize(void* data, unsigned int elementSizeInBits, unsigned int numElements)
{
    int i;
    //if the data type is 8 bits in size, such as char, then there is no need to convert it
    //because endianness only affects byte order and char is a single byte data type
    if (elementSizeInBits == 8)
        return;


    /*
    if( numElements > 1 )
    {
        if( elementSizeInBits == 64 )
        {
            unsigned long * doubleData = (unsigned long *)data;
            for(int i = 0; i< numElements; i++)
            {
                doubleData[i] = xnz_be64toh(doubleData[i]);
            }

        }
        else if( elementSizeInBits == 32 )
        {
            unsigned int * intData = (unsigned int *)data;
            for(int i = 0; i< numElements; i++)
            {
                intData[i] = xnz_be32toh(intData[i]);
            }

        }
        else if( elementSizeInBits == 16 )
        {
            short * shortData = (short *)data;
            for(int i = 0; i< numElements; i++)
            {
                shortData[i] = xnz_be16toh(shortData[i]);
            }
        }
    }
    else
    {
     */
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

    //}
}

/*
float ReverseFloat( const float inFloat )
{
    float retVal;
    char *floatToConvert = ( char* ) & inFloat;
    char *returnFloat = ( char* ) & retVal;

    // swap the bytes into a temporary buffer
    returnFloat[0] = floatToConvert[3];
    returnFloat[1] = floatToConvert[2];
    returnFloat[2] = floatToConvert[1];
    returnFloat[3] = floatToConvert[0];

    return retVal;
}

double ReverseDouble(double d)
{
    double a;
    unsigned char *dst = (unsigned char *)&a;
    unsigned char *src = (unsigned char *)&d;

    dst[0] = src[7];
    dst[1] = src[6];
    dst[2] = src[5];
    dst[3] = src[4];
    dst[4] = src[3];
    dst[5] = src[2];
    dst[6] = src[1];
    dst[7] = src[0];

    return a;
}
*/

//Serialize specific record types;  X-MACROS are used to iterate over each field in each flt record struct with minimal code
static void xnz_icc_header_serialize(void* icc_struct_data)
{
#define XNZ_ICC_HEADER_FIELD(xnz_icc_struct_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    xnz_icc_struct_field_serialize(&(((xnz_icc_struct_data *)icc_struct_data)->varName), dataTypeSizeInBits, numElements);
    XNZ_ICC_HEADER_FIELDS
#undef XNZ_ICC_HEADER_FIELD

}

/***
 ***    UNIVERSAL PRINT FUNCTION FOR ALL RECORD DATA FIELDS PARSED BY X-MACROS
 ***/
static void xnz_icc_struct_field_print(char* formatString, char* catString, char* fieldName, void* data, unsigned int elementSizeInBits, unsigned int numElements/*, char * formatSpecifier */, bool floatingPoint)
{
    int i, bit;
    unsigned* bits;

    if (strstr(fieldName, "reserved") != NULL)
        return;

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
            if (strstr(fieldName, "attributes") != NULL)
            {
                fprintf(stderr, "\n\t%s = ", fieldName);
                bits = xnz_convert_to_binary((unsigned*)data, elementSizeInBits);

                for (bit = elementSizeInBits; bit--;)
                    fprintf(stderr, "%u", +bits[bit]);

                free(bits);
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
    }
    else if (elementSizeInBits == 32)
    {

        if (strstr(fieldName, "flags") != NULL)
        {
            fprintf(stderr, "\n\t%s = ", fieldName);
            bits = xnz_convert_to_binary((unsigned*)data, elementSizeInBits);

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
            bits = xnz_convert_to_binary((unsigned*)data, elementSizeInBits);

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

        if (strstr(fieldName, "version") != NULL)
        {
            //arrays of 8-bit ints currently unsupported
            uint8_t* charData = (uint8_t*)data;
            fprintf(stderr, formatString, *charData);
            for (i = 1; i < numElements; i++)
            {
                fprintf(stderr, catString, charData[i]);
            }


        }
        else {
            //arrays of 8-bit ints currently unsupported
            char* charData = (char*)data;
            if (numElements > 1)
                fprintf(stderr, formatString, charData);
            else
                fprintf(stderr, formatString, *charData);
        }
    }

}

//sprintf (formatString, "\t%s = %s\n", #varName, stringFormat); \
//printf(formatString, ((xnz_flt_record_data *)flt_record_data)->varName);

static void xnz_icc_header_print(void* icc_struct_data)
{
    char formatString[256];
    char catString[256];

#define XNZ_ICC_HEADER_FIELD(xnz_icc_struct_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
    xnz_icc_struct_field_print(formatString, catString, #varName, &(((xnz_icc_struct_data *)icc_struct_data)->varName), dataTypeSizeInBits, numElements, strncmp(#datatype, "float", 5) == 0 || strncmp(#datatype, "double", 6) == 0 );
    XNZ_ICC_HEADER_FIELDS
#undef XNZ_ICC_HEADER_FIELD

    fprintf(stderr, "\n");
}

#endif /* xnz_icc_profile_h */
