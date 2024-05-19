#ifndef XNG_TTF_FONT_H
#define XNG_TTF_FONT_H


#include "crSystem/cr_file.h" //CRFile
//#include "stdint.h"           //uint types

/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

 /* We are building or calling XNG_FPF_IMAGE as a static library */
#ifndef XNG_TTF_API
#define XNG_TTF_API static
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_TTF_INLINE
#ifdef _WIN32
#define XNG_TTF_INLINE __inline
#else
#define XNG_TTF_INLINE
#endif
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_TTF_DECLSPEC
#ifdef _WIN32
#define XNG_TTF_DECLSPEC __declspec
#else
#define XNG_TTF_DECLSPEC
#endif
#endif

//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_TTF_ALIGN//(X)
#ifdef _WIN32
#define XNG_TTF_ALIGN(X) (align(X))
#else
#define XNG_TTF_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif

#ifndef XNG_TTF_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_TTF_PACK_ATTRIBUTE
//#pragma pack(push, 1)
//#define XNG_TTF_PACK_ATTRIBUTE( __Declaration__ ) __Declaration__ __attribute__((packed))
#else
#define XNG_TTF_PACK_ATTRIBUTE __attribute__ ((packed))
//#define XNG_TTF_PACK_ATTRIBUTE( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#endif
#endif

#include "crUtils/cr_byte_utils.h"
#include <stdio.h> // printf
//#include <stdint.h>

#ifndef _WIN32 //I don't understand why VS compiler can chain headers in xng.h but Xcode can't...
//TO DO:  should probably move types into xnz_types.h
#include "xng_image_buffer.h" //xng_fpf_buffer_draw_line_from_points
//#include "xnz.h"            //xnz_affine_transform
#endif

typedef enum XNG_TTF_TABLE_OPCODES
{
	//XNG_TTF_DIRECTORY_TABLE,
	//XNG_TTF_OFFSET_TABLE,
	XNG_TTF_HEAD_TABLE,
	XNG_TTF_HHEA_TABLE,
	XNG_TTF_HMTX_TABLE,
	XNG_TTF_VHEA_TABLE,
	XNG_TTF_VMTX_TABLE,
	XNG_TTF_MAXP_TABLE,
	XNG_TTF_LOCA_TABLE,
	XNG_TTF_CMAP_TABLE,
	XNG_TTF_GLYF_TABLE,
	XNG_TTF_NAME_TABLE,
	XNG_TTF_CTX_TABLE,
	XNG_TTF_NUM_TABLE_OPCODES

}XNG_TTF_TABLE_OPCODES;

static const char* XNG_TTF_TABLE_TAGS[] =
{
	//"dir ", //not stirctly a name from the TTF specification, but we want to maintain a 1:1 mapping with opcodes below
	//"off ",
	"head",
	"hhea",
	"hmtx",
	"vhea",
	"vmtx",
	"maxp",
	"loca",
	"cmap",
	"glyf",
	"name",
	"ctx "

};

typedef enum XNG_TTF_SIMPLE_GLYPH_FLAGS
{
	XNG_TTF_ON_CURVE_POINT = 0x01, //Bit 0: If set, the point is on the curve; //otherwise, it is off the curve.

	XNG_TTF_X_SHORT_VECTOR = 0x02, //Bit 1: If set, the corresponding x-coordinate 
      							   //is 1 byte long. If not set, it is two bytes long. 
								   //For the sign of this value, see the description of the 
								   //X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR flag.

 	XNG_TTF_Y_SHORT_VECTOR = 0x04, //Bit 2: If set, the corresponding y-coordinate 
   								   //is 1 byte long. If not set, it is two bytes long. 
								   //For the sign of this value, see the description of the 
								   //Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR flag.

	XNG_TTF_REPEAT_FLAG    = 0x08, //Bit 3: If set, the next byte (read as unsigned) specifies 
								   //the number of additional times this flag is to be repeated — 
								   //that is, the number of additional logical flag entries inserted 
								   //after this entry. In this way, the number of flags listed can be 
								   //smaller than the number of points in the glyph description.

	XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR = 0x10, //Bit 4: This flag has two meanings, depending on how the X_SHORT_VECTOR flag is set. 
														 //If X_SHORT_VECTOR is set, this bit describes the sign of the value, with 1 equalling
														 //positive and 0 negative. If X_SHORT_VECTOR is not set and this bit is set, then the 
														 //current x-coordinate is the same as the previous x-coordinate. If X_SHORT_VECTOR is 
														 //not set and this bit is also not set, the current x-coordinate is a signed 16-bit delta vector.

	XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR = 0x20, //Bit 5: This flag has two meanings, depending on how the Y_SHORT_VECTOR flag is set. 
														 //If Y_SHORT_VECTOR is set, this bit describes the sign of the value, with 1 equalling 
														 //positive and 0 negative. If Y_SHORT_VECTOR is not set and this bit is set, then the 
														 //current y-coordinate is the same as the previous y-coordinate. If Y_SHORT_VECTOR is 
														 //not set and this bit is also not set, the current y-coordinate is a signed 16-bit delta vector.

	XNG_TTF_X_VALUE_SERIALIZED					 = 0x40, //Bit 6:  Typically this value is reserved and set to 0, but we will use it in a custom implementation 
														 //to indicate that the x delta value associated with this flag has already been serialized to system byte format
														 //in our memory representation 

	XNG_TTF_Y_VALUE_SERIALIZED					 = 0x80  //Bit 7:  Typically this value is reserved and set to 0, but we will use it in a custom implementation 
														 //to indicate that the x value associated with this flag has been serialized to system byte format
														 //in our memory representation

	//XNG_TTF_SIMPLE_GLYPH_RESERVED				 = 0xC0  //Bits 6 and 7 are reserved: set to zero.

}XNG_TTF_SIMPLE_GLYPH_FLAGS;

typedef enum XNG_TTF_COMPOUND_GLYPH_FLAGS
{
	XNG_TTF_ARG_1_AND_2_ARE_WORDS	  = 0x0001, //Bit 0: If this is set, the arguments are 16-bit (uint16 or int16); otherwise, they are bytes (uint8 or int8).

	XNG_TTF_ARGS_ARE_XY_VALUES		  = 0x0002, //Bit 1: If this is set, the arguments are signed xy values; otherwise, they are unsigned point numbers.

	XNG_TTF_ROUND_XY_TO_GRID		  = 0x0004, //Bit 2: For the xy values if the preceding is true.

	XNG_TTF_WE_HAVE_A_SCALE			  = 0x0008, //Bit 3: This indicates that there is a simple scale for the component. Otherwise, scale = 1.0.

	XNG_TTF_OBSOLETE_FLAG			  = 0x0010, //Bit 4: obsolete; set to zero 

	XNG_TTF_MORE_COMPONENTS			  = 0x0020, //Bit 5: Indicates at least one more glyph after this one.

	XNG_TTF_WE_HAVE_AN_X_AND_Y_SCALE  = 0x0040, //Bit 5: Indicates at least one more glyph after this one.

	XNG_TTF_WE_HAVE_A_TWO_BY_TWO	  = 0x0080, //Bit 7: There is a 2 by 2 transformation that will be used to scale the component.

	XNG_TTF_WE_HAVE_INSTRUCTIONS	  = 0x0100, //Bit 8: Following the last component are instructions for the composite character.

	XNG_TTF_USE_MY_METRICS			  = 0x0200, //Bit 9: If set, this forces the aw and lsb (and rsb) for the composite to be equal to those from this original glyph. This works for hinted and unhinted characters.

	XNG_TTF_OVERLAP_COMPOUND		  = 0x0400, //Bit 10: If set, the components of the compound glyph overlap. 
												//Use of this flag is not required in OpenType — that is, it 
												//is valid to have components overlap without having this flag set. 
												//It may affect behaviors in some platforms, however. 
												//(See Apple's specification for details regarding behavior in Apple platforms.)

	XNG_TTF_SCALED_COMPONENT_OFFSET	  = 0x0800, //Bit 11: The composite is designed to have the component offset scaled.

	XNG_TTF_UNSCALED_COMPONENT_OFFSET = 0x1000, //Bit 12: The composite is designed not to have the component offset scaled.

	XNG_TTF_COMPOUND_GLYPH_RESERVED	  = 0xE010  //Bits 4, 13, 14 and 15 are reserved: set to 0.

}XNG_TTF_COMPOUND_GLYPH_FLAGS;

typedef enum XNG_TTF_BEZIER_CONTOUR_FLAGS
{
	XNG_TTF_FIRST_ON_CURVE_POINT  = 0x01, //Bit 0: If set, the point is on the curve; 
										  //otherwise, it is off the curve.

	XNG_TTF_FIRST_OFF_CURVE_POINT = 0x02, //Bit 1: If set, the corresponding x-coordinate 
										  //is 1 byte long. If not set, it is two bytes long. 
										  //For the sign of this value, see the description of the 
										  //X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR flag.

	XNG_TTF_LAST_OFF_CURVE_POINT = 0x04,  //Bit 2: If set, the corresponding y-coordinate 
  										  //is 1 byte long. If not set, it is two bytes long. 
										  //For the sign of this value, see the description of the 
										  //Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR flag.

	XNG_TTF_CONTOUR_PATH_CLOSING = 0x08,  //Bit 3: If set, the next byte (read as unsigned) specifies 
										  //the number of additional times this flag is to be repeated — 
										  //that is, the number of additional logical flag entries inserted 
										  //after this entry. In this way, the number of flags listed can be 
										  //smaller than the number of points in the glyph description.

	XNG_TTF_CONTOUR_PATH_ALL_DONE = 0x10, //Bit 4: This flag has two meanings, depending on how the X_SHORT_VECTOR flag is set. 
										  //If X_SHORT_VECTOR is set, this bit describes the sign of the value, with 1 equalling
										  //positive and 0 negative. If X_SHORT_VECTOR is not set and this bit is set, then the 
										  //current x-coordinate is the same as the previous x-coordinate. If X_SHORT_VECTOR is 
										  //not set and this bit is also not set, the current x-coordinate is a signed 16-bit delta vector.

	//XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR	= 0x20, //Bit 5: This flag has two meanings, depending on how the Y_SHORT_VECTOR flag is set. 
															//If Y_SHORT_VECTOR is set, this bit describes the sign of the value, with 1 equalling 
															//positive and 0 negative. If Y_SHORT_VECTOR is not set and this bit is set, then the 
															//current y-coordinate is the same as the previous y-coordinate. If Y_SHORT_VECTOR is 
															//not set and this bit is also not set, the current y-coordinate is a signed 16-bit delta vector.

	//XNG_TTF_X_VALUE_SERIALIZED					= 0x40, //Bit 6:  Typically this value is reserved and set to 0, but we will use it in a custom implementation 
															//to indicate that the x delta value associated with this flag has already been serialized to system byte format
															//in our memory representation 

	//XNG_TTF_Y_VALUE_SERIALIZED					= 0x80  //Bit 7:  Typically this value is reserved and set to 0, but we will use it in a custom implementation 
															//to indicate that the x value associated with this flag has been serialized to system byte format
															//in our memory representation

	//XNG_TTF_SIMPLE_GLYPH_RESERVED					= 0xC0  //Bits 6 and 7 are reserved: set to zero.

}XNG_TTF_BEZIER_CONTOUR_FLAGS;


#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

//#ifdef IS_BIG_ENDIAN
//#define CR_FOURCC(a,b,c,d) ( cr_is_little_endian() ? (((uint32_t)d) | ((uint32_t)c)<<8 | ((uint32_t)b)<<16 | ((uint32_t)a)<<24 ) : (((uint32_t)a) | ((uint32_t)b)<<8 | ((uint32_t)c)<<16 | ((uint32_t)d)<<24 ) ) 
//#else
//#define CR_FOURCC(a,b,c,d) (((uint32_t)a) | ((uint32_t)b)<<8 | ((uint32_t)c)<<16 | ((uint32_t)d)<<24 )
//#endif

#define HEX_DIGIT(digit) "0123456789ABCDEF"[(digit)]
#define MAKE_HEX(value) HEX_DIGIT((value) < 16? (value): ((value)/16)%16), \
                        HEX_DIGIT((value) < 16? 16: (value)%16),           \
                        0

//#define MINOR_VERSION_BIN 13
//char const version[] = { MAKE_HEX(MINOR_VERSION_BIN) };

#define XNG_TTF_HEAD_TABLE_TAG {'h', 'e', 'a', 'd'}

typedef struct CR_UINT32_STRUCT
{
	char c[4];
	uint32_t i;

}CR_UINT32_STRUCT;

typedef struct xng_ttf_data_type_longHorMetric {
	uint16_t advanceWidth;
	int16_t leftSideBearing;
}xng_ttf_data_type_longHorMetric;
/*
static const CR_UINT32_STRUCT XNG_TTF_HEAD_TABLE_TAG_STRUCT = XNG_TTF_HEAD_TABLE_TAG;

static const uint32_t XNG_TTF_TABLE_FOURCC[] =
{
	'head',
	'name',
	'ctx '
};

*/

//static const int hemp = xnz_be32toh(32);

/***
 ***    OFFSET TABLE (OPCODE 2)
 ***/
 //#pragma mark -- OFFSET TABLE (OPCODE 2)

 /*
 #define XNG_TTF_OFFSET_TABLE_FIELDS \
 XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint32_t, 32, 1, scalerType, scalerType, "%u") \
 XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint16_t, 16, 1, numTables, numTables, "%hu") \
 XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint16_t, 16, 1, searchRange, searchRange, "%hu") \
 XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint16_t, 16, 1, entrySelector, entrySelector, "%hu") \
 XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint16_t, 16, 1, rangeShift, rangeShift, "%hu")

 //--- define the structure, the X macro will be expanded once per field
 typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_offset_table
 {
 //the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
 #define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	 XNG_TTF_OFFSET_TABLE_FIELDS
 #undef XNG_TTF_TABLE_FIELD
 } xng_ttf_offset_table;
 */

 /***
  ***    DIRECTORY TABLE (OPCODE 2)
  ***/
  //#pragma mark -- DIRECTORY TABLE (OPCODE 2)

typedef int16_t xng_ttf_data_type_shortFrac;		//16-bit signed fraction
typedef int32_t xng_ttf_data_type_Fixed;			//16.16-bit signed fixed-point number
typedef int16_t xng_ttf_data_type_FWord;			//16-bit signed integer that describes a quantity in FUnits, the smallest measurable distance in em space.
typedef uint16_t xng_ttf_data_type_uFWord;		//16-bit unsigned integer that describes a quantity in FUnits, the smallest measurable distance in em space.
typedef int16_t xng_ttf_data_type_F2Dot14;		//16-bit signed fixed number with the low 14 bits representing fraction.
typedef int64_t xng_ttf_data_type_longDateTime;  //The long internal format of a date in seconds since 12:00 midnight, January 1, 1904. It is represented as a signed 64-bit integer

#ifdef _WIN32
#pragma pack(push, 4)
#endif

#define XNG_TTF_DIRECTORY_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, sfntVersion, sfntVersion, "0x%.8x") \
XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint16_t, 16, 1, numTables, numTables, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint16_t, 16, 1, searchRange, searchRange, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint16_t, 16, 1, entrySelector, entrySelector, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint16_t, 16, 1, rangeShift, rangeShift, "%hu")
//XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, tag, tag, "%u") \
//XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, checksum, checksum, "%u") \
//XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, offset, offset, "%u") \
//XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, length, length, "%u")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_directory_table
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_DIRECTORY_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_directory_table;


//#ifdef _WIN32
//#pragma pack(pop)   //end struct single byte packing
//#endif


//we can maintain a uint32 data type for "tag" property, but interpret as 4-byte character code read directly in byte order from file (no reformatting to OS byte ordering necessary))
#define XNG_TTF_OFFSET_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint32_t, 8, 4, tag, tag, "%.4s") \
XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint32_t, 32, 1, checksum, checksum, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint32_t, 32, 1, offset, offset, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_offset_table, uint32_t, 32, 1, length, length, "%u")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_offset_table
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_OFFSET_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_offset_table;


/*
#ifdef _WIN32
#pragma pack(pop)   //end struct single byte packing
#endif
*/

#define XNG_TTF_HEADER_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, xng_ttf_data_type_Fixed, 16, 2, version, version, "%hd.hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, xng_ttf_data_type_Fixed, 16, 2, fontRevision, fontRevision, "%hd.hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, uint32_t, 32, 1, checkSumAdjustment, checkSumAdjustment, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, uint32_t, 32, 1, magicNumber, magicNumber, "0x%.8x") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, uint16_t, 16, 1, flags, flags, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, uint16_t, 16, 1, unitsPerEm, unitsPerEm, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, xng_ttf_data_type_longDateTime, 64, 1, created, created, "%lld") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, xng_ttf_data_type_longDateTime, 64, 1, modified, modified, "%lld") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, xng_ttf_data_type_FWord, 16, 1, xMin, xMin, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, xng_ttf_data_type_FWord, 16, 1, yMin, yMin, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, xng_ttf_data_type_FWord, 16, 1, xMax, xMax, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, xng_ttf_data_type_FWord, 16, 1, yMax, yMax, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, uint16_t, 16, 1, macStyle, macStyle, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, uint16_t, 16, 1, lowestRecPPEM, lowestRecPPEM, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, int16_t, 16, 1, fontDirectionHint, fontDirectionHint, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, int16_t, 16, 1, indexToLocFormat, indexToLocFormat, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_head_table, int16_t, 16, 1, glyphDataFormat, glyphDataFormat, "%hd")
//XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, tag, tag, "%u") \
//XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, checksum, checksum, "%u") \
//XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, offset, offset, "%u") \
//XNG_TTF_TABLE_FIELD(xng_ttf_directory_table, uint32_t, 32, 1, length, length, "%u")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_head_table
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_HEADER_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_head_table;

#define XNG_TTF_HHEA_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_Fixed, 16, 2, version, version, "%hd.hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_FWord, 16, 1, ascent, ascent, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_FWord, 16, 1, descent, descent, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_FWord, 16, 1, lineGap, lineGap, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_uFWord, 16, 1, advanceWidthMax, advanceWidthMax, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_FWord, 16, 1, minLeftSideBearing, minLeftSideBearing, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_FWord, 16, 1, minRightSideBearing, minRightSideBearing, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_FWord, 16, 1, xMaxExtent, xMaxExtent, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, int16_t, 16, 1, caretSlopeRise, caretSlopeRise, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, int16_t, 16, 1, caretSlopeRun, caretSlopeRun, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, xng_ttf_data_type_FWord, 16, 1, caretOffset, caretOffset, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, int16_t, 16, 4,  reserved, reserved[4], "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, int16_t, 16, 1, metricDataFormat, metricDataFormat, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_hhea_table, uint16_t, 16, 1, numOfLongHorMetrics, numOfLongHorMetrics, "%hu")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_hhea_table
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_HHEA_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_hhea_table;

#define XNG_TTF_VHEA_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_Fixed, 16, 2, version, version, "%hd.hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_FWord, 16, 1, vertTypoAscender, vertTypoAscender, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_FWord, 16, 1, vertTypoDescender, vertTypoDescender, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_FWord, 16, 1, vertTypoLineGap, vertTypoLineGap, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_uFWord, 16, 1, advanceHeightMax, advanceHeightMax, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_FWord, 16, 1, minTopSideBearing, minTopSideBearing, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_FWord, 16, 1, minBottomSideBearing, minBottomSideBearing, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_FWord, 16, 1, yMaxExtent, yMaxExtent, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, int16_t, 16, 1, caretSlopeRise, caretSlopeRise, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, int16_t, 16, 1, caretSlopeRun, caretSlopeRun, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, xng_ttf_data_type_FWord, 16, 1, caretOffset, caretOffset, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, int16_t, 16, 4,  reserved, reserved[4], "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, int16_t, 16, 1, metricDataFormat, metricDataFormat, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_vhea_table, uint16_t, 16, 1, numOfLongVerMetrics, numOfLongVerMetrics, "%hu")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_vhea_table
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_VHEA_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_vhea_table;

#define XNG_TTF_MAXP_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, xng_ttf_data_type_Fixed, 16, 2, version, version, "%hd.hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, numGlyphs, numGlyphs, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxPoints, maxPoints, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxContours, maxContours, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxComponentPoints, maxComponentPoints, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxComponentContours, maxComponentContours, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxZones, maxZones, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxTwilightPoints, maxTwilightPoints, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxStorage, maxStorage, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxFunctionDefs, maxFunctionDefs, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxInstructionDefs, maxInstructionDefs, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxStackElements, maxStackElements, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxSizeOfInstructions, maxSizeOfInstructions, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxComponentElements, maxComponentElements, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_maxp_table, uint16_t, 16, 1, maxComponentDepth, maxComponentDepth, "%hu")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_maxp_table
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_MAXP_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_maxp_table;

#define XNG_TTF_LOCA_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_loca_table, unsigned char*, sizeof(unsigned char*), 1, offsets, offsets, "%s")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_loca_table
{

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_LOCA_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD

} xng_ttf_loca_table;



#define XNG_TTF_CMAP_ENCODING_RECORD_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_encoding_record, uint16_t, 16, 1, platformID, platformID, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_encoding_record, uint16_t, 16, 1, encodingID, encodingID, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_encoding_record, uint32_t, 32, 1, offset, offset, "%u")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_cmap_encoding_record
{

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_CMAP_ENCODING_RECORD_FIELDS
#undef XNG_TTF_TABLE_FIELD

} xng_ttf_cmap_encoding_record;

#ifdef _WIN32
#pragma pack(push, 1)
#endif

#define XNG_TTF_CMAP_FORMAT0_MAPPING_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format0_mapping_table, uint16_t, 16, 1, format, format, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format0_mapping_table, uint16_t, 16, 1, length, length, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format0_mapping_table, uint16_t, 16, 1, language, language, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format0_mapping_table, uint8_t, 8, 256, glyphIdArray, glyphIdArray[256], "%c")
//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(1) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_cmap_format0_mapping_table
{

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_CMAP_FORMAT0_MAPPING_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD

} xng_ttf_cmap_format0_mapping_table;

#define XNG_TTF_CMAP_FORMAT4_MAPPING_TABLE_HEADER_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format4_mapping_table_header, uint16_t, 16, 1, format, format, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format4_mapping_table_header, uint16_t, 16, 1, length, length, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format4_mapping_table_header, uint16_t, 16, 1, language, language, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format4_mapping_table_header, uint16_t, 16, 1, segCountX2, segCountX2, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format4_mapping_table_header, uint16_t, 16, 1, searchRange, searchRange, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format4_mapping_table_header, uint16_t, 16, 1, entrySelector, entrySelector, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format4_mapping_table_header, uint16_t, 16, 1, rangeShift, rangeShift, "%hu")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(1) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_cmap_format4_mapping_table_header
{

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_CMAP_FORMAT4_MAPPING_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD

} xng_ttf_cmap_format4_mapping_table_header;

#define XNG_TTF_CMAP_FORMAT6_MAPPING_TABLE_HEADER_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format6_mapping_table_header, uint16_t, 16, 1, format, format, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format6_mapping_table_header, uint16_t, 16, 1, length, length, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format6_mapping_table_header, uint16_t, 16, 1, language, language, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format6_mapping_table_header, uint16_t, 16, 1, firstCode, firstCode, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format6_mapping_table_header, uint16_t, 16, 1, entryCount, entryCount, "%hu")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(1) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_cmap_format6_mapping_table_header
{

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_CMAP_FORMAT6_MAPPING_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD

} xng_ttf_cmap_format6_mapping_table_header;

#define XNG_TTF_CMAP_FORMAT12_MAPPING_TABLE_HEADER_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format12_mapping_table_header, uint16_t, 16, 1, format, format, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format12_mapping_table_header, uint16_t, 16, 1, reserved, reserved, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format12_mapping_table_header, uint32_t, 32, 1, length, length, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format12_mapping_table_header, uint32_t, 32, 1, language, language, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_format12_mapping_table_header, uint32_t, 32, 1, numGroups, numGroups, "%u")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(1) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_cmap_format12_mapping_table_header
{

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_CMAP_FORMAT12_MAPPING_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD

} xng_ttf_cmap_format12_mapping_table_header;

#define XNG_TTF_CMAP_SEQUENTIAL_MAP_GROUP_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_sequential_map_group, uint32_t, 32, 1, startCharCode, startCharCode, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_sequential_map_group, uint32_t, 32, 1, endCharCode, endCharCode, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_sequential_map_group, uint32_t, 32, 1, startGlyphID, startGlyphID, "%u")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(1) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_cmap_sequential_map_group
{

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_CMAP_SEQUENTIAL_MAP_GROUP_FIELDS
#undef XNG_TTF_TABLE_FIELD

} xng_ttf_cmap_sequential_map_group;

#ifdef _WIN32
#pragma pack(pop)
#endif

#define XNG_TTF_CMAP_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_table, uint16_t, 16, 1, version, version, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_cmap_table, uint16_t, 16, 1, numberSubtables, numberSubtables, "%hu")
//XNG_TTF_TABLE_FIELD(xng_ttf_cmap_table, xng_ttf_cmap_encoding_record*, sizeof(xng_ttf_cmap_encoding_record*), 1, encodingTables, encodingTables, "%s") \
//XNG_TTF_TABLE_FIELD(xng_ttf_cmap_table, void*, sizeof(void*), 1, mappingTables, mappingTables, "%s")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_cmap_table
{

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_CMAP_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD

} xng_ttf_cmap_table;


//If the number of contours is positive or zero, it is a single glyph;
//If the number of contours less than zero, the glyph is compound
#define XNG_TTF_GLYF_TABLE_HEADER_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_glyf_table_header, int16_t, 16, 1, numberOfContours, numberOfContours, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_glyf_table_header, xng_ttf_data_type_FWord, 16, 1, xMin, xMin, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_glyf_table_header, xng_ttf_data_type_FWord, 16, 1, yMin, yMin, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_glyf_table_header, xng_ttf_data_type_FWord, 16, 1, xMax, xMax, "%hd") \
XNG_TTF_TABLE_FIELD(xng_ttf_glyf_table_header, xng_ttf_data_type_FWord, 16, 1, yMax, yMax, "%hd")



//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(1) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_glyf_table_header
{

#ifdef _WIN32
#pragma pack(push, 1)
#endif

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_GLYF_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD

#ifdef _WIN32
#pragma pack(pop)
#endif

}xng_ttf_glyf_table_header;



#define XNG_TTF_SIMPLE_GLYF_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_simple_glyf, uint16_t*, sizeof(uint16_t*), 1, endPtsOfContours, endPtsOfContours, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_simple_glyf, uint16_t, 16, 1, instructionLength, instructionLength, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_simple_glyf, uint8_t*, sizeof(uint8_t*), 1, instructions, instructions, "%s") \
XNG_TTF_TABLE_FIELD(xng_ttf_simple_glyf, uint8_t*, sizeof(uint8_t*), 1, flags, flags, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_simple_glyf, void*, sizeof(void*), 1, length, length, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_simple_glyf, void*, sizeof(void*), 1, offset, offset, "%u")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_simple_glyf
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_SIMPLE_GLYF_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_simple_glyf;


#define XNG_TTF_COMPOUND_GLYF_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_compound_glyf, uint16_t*, sizeof(uint16_t*), 1, endPtsOfContours, endPtsOfContours, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_compound_glyf, uint16_t, 16, 1, instructionLength, instructionLength, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_compound_glyf, uint8_t*, sizeof(uint8_t*), 1, instructions, instructions, "%s") \
XNG_TTF_TABLE_FIELD(xng_ttf_compound_glyf, uint8_t*, sizeof(uint8_t*), 1, flags, flags, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_compound_glyf, void*, sizeof(void*), 1, length, length, "%u") \
XNG_TTF_TABLE_FIELD(xng_ttf_compound_glyf, void*, sizeof(void*), 1, offset, offset, "%u")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_compound_glyf
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_COMPOUND_GLYF_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_compound_glyf;

#define XNG_TTF_GLYF_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_glyf_table, void*, sizeof(void*), 1, data, data, "%s")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_glyf_table
{

	//#ifdef _WIN32
	//#pragma pack(push, 1)
	//#endif

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_GLYF_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD


		//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
		XNG_TTF_GLYF_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD


		//#ifdef _WIN32
		//#pragma pack(pop)
		//#endif	

}xng_ttf_glyf_table;



XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct xng_ttf_glyph_bounding_box
{
	int16_t left, top, right, bottom, width, height;

}xng_ttf_glyph_bounding_box;

XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct xng_ttf_glyph_metrics
{
	int16_t ascent, descent;
	uint16_t advanceWidth;
	int16_t leftSideBearing;
	int16_t rightSideBearing;
	uint16_t advanceHeight;
	int16_t topSideBearing;
	int16_t bottomSideBearing;

}xng_ttf_glyph_metrics;

XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct xng_ttf_glyph
{
	//union{
	//	xng_ttf_glyf_table_header header;
	//};
	xng_ttf_glyf_table_header* header;
	//int16_t numContours; // < 0 indicates compound glyph; >= 0 indicates simple glyph
	//xng_ttf_data_type_FWord xMin;
	//xng_ttf_data_type_FWord yMin;
	//xng_ttf_data_type_FWord xMax;
	//xng_ttf_data_type_FWord yMax;

	uint16_t index;
	uint32_t offset;

	uint16_t* endPtsOfContours;
	uint16_t numContourPoints;

	uint16_t numInstructions;
	uint8_t* instructions;

	uint16_t numFlags;
	uint8_t* flags;    // a pointer to the flags in teh glyph buffer serialized from file

	void* xDeltas;		// a pointer to the x delta values in the glyph buffer serialized from file
	void* yDeltas;		// a pointer to the y delta values in the glyph buffer serialized from file

	float point_size;
	float scale;

	xng_ttf_glyph_bounding_box bb;
	xng_ttf_glyph_metrics metrics;

	xnz_float_affine_transform affine_transform;


	//uint16_t advanceWidth;
	//int16_t  leftSideBearing;
	//uint16_t advanceHeight;
	//int16_t  topSideBearing;

	int16_t* xCoords;
	int16_t* yCoords;

	float* accumulation_buffer;



}xng_ttf_glyph;

//since NAME_TABLE_HEADER IS AN UNOFFICIAL HELPER SUBTABLE OF OUR OWN CREATION (rather than part of the TTF/OTF spec)
//we set its packing to be aligned to be padded to 0 additonal bytes
//since it is the first subtable of the larger name table, we can leave its alignment set to the same as that of the larger table
#define XNG_TTF_NAME_TABLE_HEADER_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_name_table, uint16_t, 16, 1, format, format, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_name_table, uint16_t, 16, 1, count, count, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_name_table, uint16_t, 16, 1, stringOffset, stringOffset, "%hu")


#ifdef _WIN32
#pragma pack(push, 1)
#endif

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(1) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_name_table_header
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_NAME_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_name_table_header;

#ifdef _WIN32
#pragma pack(pop)
#endif



/*
The supported platform identifier (platformID) codes are given in the following table. PlatformID codes have been assigned for Unicode, Macintosh, and Microsoft. PlatformID codes 240 through 255 have been reserved for user-defined platforms and are not available for registration. Names with platformIDs
other than 0, 1, and 2 are ignored on OS X and iOS.

Platform ID		Platform	Platform-specific ID
0				Unicode		Indicates Unicode version.
1				Macintosh	QuickDraw Script Manager code.
2				(reserved; do not use)
3				Microsoft	Microsoft encoding.

The platform ID 2 was originally to use with ISO/IEC 10646, but that use is now deprecated, as ISO/IEC 10646 and Unicode have identical character code assignments.
*/
/*
To fully specify an encoding, it is necessary to state not only the platform identifier code, platformID, but also the platform-specific encoding, platformSpecificID, among all of the possible encodings for the specified platform. The Macintosh script manager codes (platformID = 1 for Macintosh) are given in the following table.

Macintosh Platform-specific Encoding Identifiers

Platform-specific ID code	Script
0	Roman				17	Malayalam
1	Japanese			18	Sinhalese
2	Traditional Chinese	19	Burmese
3	Korean				20	Khmer
4	Arabic				21	Thai
5	Hebrew				22	Laotian
6	Greek				23	Georgian
7	Russian				24	Armenian
8	RSymbol				25	Simplified Chinese
9	Devanagari			26	Tibetan
10	Gurmukhi			27	Mongolian
11	Gujarati			28	Geez
12	Oriya				29	Slavic
13	Bengali				30	Vietnamese
14	Tamil				31	Sindhi
15	Telugu				32	(Uninterpreted)
16	Kannada
*/
/*
Unicode Platform-specific Encoding Identifiers

Platform-specific ID code	Meaning

0	Default semantics
1	Version 1.1 semantics
2	ISO 10646 1993 semantics (deprecated)
3	Unicode 2.0 or later semantics (BMP only)
4	Unicode 2.0 or later semantics (non-BMP characters allowed)
5	Unicode Variation Sequences
6	Full Unicode coverage (used with type 13.0 cmaps by OpenType)
*/
#define XNG_TTF_NAME_RECORD_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_name_record, uint16_t, 16, 1, platformID, platformID, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_name_record, uint16_t, 16, 1, platformSpecificID, platformSpecificID, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_name_record, uint16_t, 16, 1, languageID, languageID, "0x%.4x") \
XNG_TTF_TABLE_FIELD(xng_ttf_name_record, uint16_t, 16, 1, nameID, nameID, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_name_record, uint16_t, 16, 1, length, length, "%hu") \
XNG_TTF_TABLE_FIELD(xng_ttf_name_record, uint16_t, 16, 1, offset, offset, "%hu")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_name_record
{
	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_NAME_RECORD_FIELDS
#undef XNG_TTF_TABLE_FIELD
} xng_ttf_name_record;


#define XNG_TTF_NAME_TABLE_FIELDS \
XNG_TTF_TABLE_FIELD(xng_ttf_name_table, xng_ttf_name_record*, sizeof(xng_ttf_name_record*), 1, nameRecords, nameRecords, "%s") \
XNG_TTF_TABLE_FIELD(xng_ttf_name_table, unsigned char*, sizeof(unsigned char*), 1, names, names, "%s")

//--- define the structure, the X macro will be expanded once per field
XNG_TTF_DECLSPEC XNG_TTF_ALIGN(4) typedef struct XNG_TTF_PACK_ATTRIBUTE xng_ttf_name_table
{

#ifdef _WIN32
#pragma pack(push, 1)
#endif

	//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
	XNG_TTF_NAME_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD

#ifdef _WIN32
#pragma pack(pop)
#endif	


#ifdef _WIN32
#pragma pack(push, 1)
#endif

		//the compiler will expand each field in the CR_FLT_HEADER_RECORD_FIELDS macro to define the fields of the struct, supercool!
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) datatype varDeclaration;
		XNG_TTF_NAME_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD


#ifdef _WIN32
#pragma pack(pop)
#endif	


} xng_ttf_name_table;



#ifdef _WIN32
#pragma pack(pop)   //end struct single byte packing
#endif

/***
 ***    PUBLIC FUNCTIONS
 ***/
 //#pragma mark -- PUBLIC FUNCTIONS

 //serialize
 //void xng_ttf_offset_table_serialize(void * ttf_table_data);
static void xng_ttf_directory_table_serialize(void* ttf_table_data);
static void xng_ttf_offset_table_serialize(void* ttf_table_data);
static xng_ttf_head_table* xng_ttf_head_table_serialize(void* ttf_table_data);

static xng_ttf_hhea_table* xng_ttf_hhea_table_serialize(void* ttf_table_data);
static void xng_ttf_hmtx_table_serialize(void* ttf_table_data, uint16_t numOfLongHorMetrics, uint16_t numGlyphs);
static void xng_ttf_hmtx_table_get_metrics_for_glyph_at_index(void* ttf_table_data, uint16_t numOfLongHorMetrics, uint16_t numGlyphs, xng_ttf_glyph* glyph);

static void xng_ttf_vhea_table_serialize(void* ttf_table_data);
static void xng_ttf_vmtx_table_serialize(void* ttf_table_data, uint16_t numOfLongVerMetrics, uint16_t numGlyphs);
static void xng_ttf_vmtx_table_get_metrics_for_glyph_at_index(void* ttf_table_data, uint16_t numOfLongVerMetrics, uint16_t numGlyphs, xng_ttf_glyph* glyph);

static xng_ttf_maxp_table* xng_ttf_maxp_table_serialize(void* ttf_table_data);

//since loca table format can be either uint16_t or uint32_t we return a void* pointer instead of a dedicated struct
static void* xng_ttf_loca_table_serialize(void* ttf_table_data, uint16_t numGlyphs, int16_t indexToLocFormat);
//since cmap entries can be of various cases and size we return a void* pointer instead of a dedicated struct
static void* xng_ttf_cmap_table_serialize(void* ttf_table_data);

//after loading the TTF file, use this method to read the cmap table to get the glyph index associated with a certain character code
XNG_TTF_API uint32_t xng_ttf_cmap_glyph_index_for_char_code(void* ttf_table_data, unsigned long char_code, uint16_t* glyphIndex);

//serialize glyf table entries with two sequential methods:
//1) for simple glyf tables, serializes all data up to the flags property and stores a pointer to flags the xng_ttf_glyph struct if present
//   for compound glyfs, serializes the compound glyf table entry entirely
static void xng_ttf_glyf_table_serialize_header_data(void* ttf_table_data, xng_ttf_glyph* glyph);
//2) for simple glyf tables, will read header data as serialized by xng_ttf_glyf_table_serialize_header_data
//   and then commence reading through flags to serialize the simple glyf contour point data
//   if this is the first time the contour point data is read it will be serialized and the corresponding reserved bits on the flags 
//   wil be set to indicate that it has been so it is not reserialized on subsequent passes.  
//   for compound glyfs, will read the compound glyf data as serialized by xng_ttf_glyf_table_serialize_header_data
//   
//   this function should always follow an initial call to xng_ttf_glyf_table_serialize_header_data
XNG_TTF_API uint32_t xng_ttf_glyf_table_draw_contour_data(int16_t indexToLocFormat, void* loca_table, void* glyf_table, void* ttf_table_data, xng_ttf_glyph* glyph);//, xng_ttf_glyph_bounding_box * glyph_bb);
XNG_TTF_API uint32_t xng_ttf_glyf_table_serialize_and_draw_contour_data(int16_t indexToLocFormat, void* loca_table, void* glyf_table, void* ttf_table_data, xng_ttf_glyph* glyph);//, xng_ttf_glyph_bounding_box * glyph_bb);
//OR serialize a simple glyf table or compund glyf table all at once
//uint32_t xng_ttf_glyf_table_serialize(void * ttf_table_data, xng_ttf_glyph * glyph);



XNG_TTF_API XNG_TTF_INLINE void xng_ttf_accumulate_sse(const float* in, uint8_t* out, uint32_t n);

//all glyf entries in the glyf table start with some common data properties
//xng_ttf_glyf_table_header* xng_ttf_glyf_table_header_serialize(void * ttf_table_data);
//but a glyf data after the shared header properties can be "simple" or "compound" containing different data/
//void xng_ttf_simple_glyf_table_serialize(void * ttf_table_data, uint16_t numContours);
//void xng_ttf_compound_glyf_table_serialize(void * ttf_table_data);

//serialize name table with three sequential methods
static void xng_ttf_name_table_header_serialize(void* ttf_table_data);
static void xng_ttf_name_record_serialize(void* ttf_table_data);
static void xng_ttf_name_strings_serialize(void* ttf_table_data);

//void cr_flt_object_record_serialize(void * flt_record_data);
//void cr_flt_face_record_serialize(void * flt_record_data);
//void cr_flt_record_serialize(cr_flt_record * flt_record);

//print
//void xng_ttf_offset_table_print(void * ttf_data);
static void xng_ttf_directory_table_print(void* ttf_data);
static void xng_ttf_offset_table_print(void* ttf_data);
static void xng_ttf_head_table_print(void* ttf_data);
static void xng_ttf_hhea_table_print(void* ttf_data);

static void xng_ttf_cmap_format4_mapping_table_header_print(void* ttf_data);
static void xng_ttf_cmap_format6_mapping_table_header_print(void* ttf_data);

static void xng_ttf_maxp_table_print(void* ttf_data);
static void xng_ttf_loca_table_print(void* ttf_table_data, uint16_t numGlyphs, int16_t indexToLocFormat);
static void xng_ttf_cmap_encoding_record_print(void* ttf_data);

static void xng_ttf_glyf_table_header_print(void* ttf_data);

static void xng_ttf_name_table_print(void* ttf_data);



/*
#include "../crText.h"
#include "crUtils/cr_byte_utils.h"
#include <math.h>
#include "crMath.h"
#include "xng.h"
*/


//#pragma mark -- SERIALIZE

/***
 ***    UNIVERSAL SERIALIZE FUNCTION FOR ALL TTF TABLE STRUCTURE FIELDS PARSED BY X-MACROS
 ***
 ***    Converts TTF Table Big Endian Data Types to host format (e.g. Intel = Little Endian) using system level conversion calls
 ***/
static void xng_ttf_table_serialize(void * data, unsigned int elementSizeInBits, unsigned int numElements, char * datatype)
{
	unsigned int i;
    //if the data type is 8 bits in size, such as char, then there is no need to convert it
    //because endianness only affects byte order and char is a single byte data type
    if(elementSizeInBits == 8 )
        return;
    
 
    if( elementSizeInBits == 64 )
    {
		if( strcmp(datatype, "xng_ttf_data_type_longDateTime") == 0 )
		{
			int64_t* intData = (int64_t*)data;
			*intData = xnz_be64toh(*intData);
			for(i = 1; i< numElements; i++)
			{
				intData[i] = xnz_be64toh(intData[i]);
			}
		}
		/*
		else
		{
			double * doubleData = (double*)data;
			*doubleData = xnz_be64toh(*doubleData);
			for(i = 1; i< numElements; i++)
			{
				doubleData[i] = xnz_be64toh(doubleData[i]);
			}
		}
		*/
            
    }
    else if( elementSizeInBits == 32 )
    {

		if( strcmp(datatype, "xng_ttf_data_type_Fixed") == 0 )
		{
			int32_t* intData = (int32_t*)data;
			*intData = xnz_be32toh(*intData);
			for(i = 1; i< numElements; i++)
			{
				intData[i] = xnz_be32toh(intData[i]);
			}
		}
		else
		{
			uint32_t * intData = (uint32_t *)data;
			*intData = xnz_be32toh(*intData);
			for(i = 1; i< numElements; i++)
			{
				intData[i] = xnz_be32toh(intData[i]);
			}
		}
            
    }
    else if( elementSizeInBits == 16 )
    {

		
		if( strcmp(datatype, "xng_ttf_data_type_Fixed") == 0 )
		{
			int16_t* intData = (int16_t*)data;
			*intData = xnz_be16toh(*intData);
			for(i = 1; i< numElements; i++)
			{
				intData[i] = xnz_be16toh(intData[i]);
			}
		}
		else if( strcmp(datatype, "xng_ttf_data_type_FWord") == 0 )
		{
			int16_t* intData = (int16_t*)data;
			*intData = xnz_be16toh(*intData);
			for(i = 1; i< numElements; i++)
			{
				intData[i] = xnz_be16toh(intData[i]);
			}
		}
		else
		{
			uint16_t * shortData = (uint16_t *)data;
			*shortData = xnz_be16toh(*shortData);
			for(i = 1; i< numElements; i++)
			{
				shortData[i] = xnz_be16toh(shortData[i]);
			}
		}
    }
        
   
}

//void xng_ttf_serialize
/*
void xng_ttf_offset_table_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements);
    XNG_TTF_OFFSET_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}
*/

static void xng_ttf_directory_table_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_DIRECTORY_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
}

static void xng_ttf_offset_table_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_OFFSET_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
}

static xng_ttf_head_table * xng_ttf_head_table_serialize(void * ttf_table_data)
{
	xng_ttf_head_table * head_table = (xng_ttf_head_table*)ttf_table_data;
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_HEADER_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD

	return head_table;
}

static xng_ttf_hhea_table * xng_ttf_hhea_table_serialize(void * ttf_table_data)
{
	xng_ttf_hhea_table * hhea_table = (xng_ttf_hhea_table*)ttf_table_data;
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_HHEA_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD

	return hhea_table;
}

static void xng_ttf_hmtx_table_serialize(void * ttf_table_data, uint16_t numOfLongHorMetrics, uint16_t numGlyphs)
{
	uint16_t i;
	uint32_t tableBytesRead;
	int16_t * leftSideBearings; 
	//If the longHorMetric array is less than the total number of glyphs, then that array is followed 
	//by an array for the left side bearing values of the remaining glyphs. The number of elements in 
	//the left side bearing will be derived from numberOfHMetrics plus the numGlyphs field in the 'maxp' table.
	xng_ttf_data_type_longHorMetric * longHorMetrics;
	
	longHorMetrics = (xng_ttf_data_type_longHorMetric*)ttf_table_data;

	printf("\nnumOfLongHorMetrics = %hu\n", numOfLongHorMetrics);
	printf("\nnumGlyphs = %hu\n", numGlyphs);


	for( i=0; i<numOfLongHorMetrics; i++)
	{
		longHorMetrics[i].advanceWidth = xnz_be16toh(longHorMetrics[i].advanceWidth);
		longHorMetrics[i].leftSideBearing = xnz_be16toh(longHorMetrics[i].leftSideBearing);
	}

	tableBytesRead = numOfLongHorMetrics * sizeof(uint16_t) * 2;

	//if( numOfLongHorMetrics < numGlyphs )
	//{
		leftSideBearings = (int16_t*)((uint8_t*)ttf_table_data + tableBytesRead); 
		for( i=0; i<numGlyphs - numOfLongHorMetrics; i++)
		{
			leftSideBearings[i] = xnz_be16toh(leftSideBearings[i]);
		}
	//}
}

static void xng_ttf_hmtx_table_get_metrics_for_glyph_at_index(void * ttf_table_data, uint16_t numOfLongHorMetrics, uint16_t numGlyphs, xng_ttf_glyph * glyph)
{
	xng_ttf_data_type_longHorMetric * longHorMetrics;
	int16_t * leftSideBearings;
	longHorMetrics = (xng_ttf_data_type_longHorMetric*)ttf_table_data;

	assert(glyph);
	if( glyph->index > numOfLongHorMetrics - 1 )
	{
		leftSideBearings = (int16_t*)((uint8_t*)ttf_table_data + numOfLongHorMetrics * sizeof(uint16_t) * 2); 

		glyph->metrics.advanceWidth = longHorMetrics[numOfLongHorMetrics-1].advanceWidth;
		glyph->metrics.leftSideBearing = leftSideBearings[glyph->index - numOfLongHorMetrics - 1];

		printf("advanceWidth1 = %hu\n", longHorMetrics[numOfLongHorMetrics-1].advanceWidth);
		//printf("advanceWidth = %f\n", longHorMetrics[numOfLongHorMetrics-1].advanceWidth);

	}
	else
	{
		glyph->metrics.advanceWidth = longHorMetrics[glyph->index].advanceWidth;
		glyph->metrics.leftSideBearing = longHorMetrics[glyph->index].leftSideBearing;

		printf("advanceWidth2 = %hu\n", longHorMetrics[glyph->index].advanceWidth);

	}

}

static void xng_ttf_vhea_table_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_VHEA_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
}

static void xng_ttf_vmtx_table_serialize(void * ttf_table_data, uint16_t numOfLongVerMetrics, uint16_t numGlyphs)
{
	uint16_t i;
	uint32_t tableBytesRead;
	int16_t * topSideBearings; 
	//If the longHorMetric array is less than the total number of glyphs, then that array is followed 
	//by an array for the left side bearing values of the remaining glyphs. The number of elements in 
	//the left side bearing will be derived from numberOfHMetrics plus the numGlyphs field in the 'maxp' table.
	xng_ttf_data_type_longHorMetric * longVerMetrics;
	
	longVerMetrics = (xng_ttf_data_type_longHorMetric*)ttf_table_data;

	for( i=0; i<numOfLongVerMetrics; i++)
	{
		longVerMetrics[i].advanceWidth = xnz_be16toh(longVerMetrics[i].advanceWidth);
		longVerMetrics[i].leftSideBearing = xnz_be16toh(longVerMetrics[i].leftSideBearing);
	}

	tableBytesRead = numOfLongVerMetrics * sizeof(uint16_t) * 2;

	//if( numOfLongHorMetrics < numGlyphs )
	//{
		topSideBearings = (int16_t*)((uint8_t*)ttf_table_data + tableBytesRead); 
		for( i=0; i<numGlyphs - numOfLongVerMetrics; i++)
		{
			topSideBearings[i] = xnz_be16toh(topSideBearings[i]);
		}
	//}
}


static void xng_ttf_vmtx_table_get_metrics_for_glyph_at_index(void * ttf_table_data, uint16_t numOfLongVerMetrics, uint16_t numGlyphs, xng_ttf_glyph * glyph)
{
	xng_ttf_data_type_longHorMetric * longVerMetrics;
	int16_t * topSideBearings;
	longVerMetrics = (xng_ttf_data_type_longHorMetric*)ttf_table_data;

	assert(glyph);
	if( glyph->index > numOfLongVerMetrics - 1 )
	{
		topSideBearings = (int16_t*)((uint8_t*)ttf_table_data + numOfLongVerMetrics * sizeof(uint16_t) * 2); 

		glyph->metrics.advanceHeight = longVerMetrics[numOfLongVerMetrics-1].advanceWidth;
		glyph->metrics.topSideBearing = topSideBearings[glyph->index - numOfLongVerMetrics - 1];

		//printf("advanceWidth1 = %hu\n", longHorMetrics[numOfLongHorMetrics-1].advanceWidth);
		//printf("advanceWidth = %f\n", longHorMetrics[numOfLongHorMetrics-1].advanceWidth);

	}
	else
	{
		glyph->metrics.advanceHeight = longVerMetrics[glyph->index].advanceWidth;
		glyph->metrics.topSideBearing = longVerMetrics[glyph->index].leftSideBearing;

		//printf("advanceWidth2 = %hu\n", longHorMetrics[glyph->index].advanceWidth);

	}

}

static xng_ttf_maxp_table * xng_ttf_maxp_table_serialize(void * ttf_table_data)
{
	xng_ttf_maxp_table * maxp_table = (xng_ttf_maxp_table*)ttf_table_data;
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(maxp_table->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_MAXP_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD

	return  maxp_table;
}

static void* xng_ttf_loca_table_serialize(void * ttf_table_data, uint16_t numGlyphs, int16_t indexToLocFormat)
{
	int i;
	//xng_ttf_loca_table * loca_table = (xng_ttf_loca_table*)ttf_table_data;

	if( indexToLocFormat == 0 )
	{
		uint16_t * offsets = (uint16_t*)(ttf_table_data);

		//add 1 for the "EXTRA" glyph
		for( i=0; i<numGlyphs+1; i++)
		{
			offsets[i] = xnz_be16toh(offsets[i]);
		}

	}
	else if( indexToLocFormat == 1)
	{
		uint32_t * offsets = (uint32_t*)(ttf_table_data);
		
		//add 1 for the "EXTRA" glyph
		for( i=0; i<numGlyphs+1; i++)
		{
			offsets[i] = xnz_be32toh(offsets[i]);
			printf("\n numGlyphs = %hu \n", numGlyphs);
			printf("\n glyf offset @ %d = %u\n", i, offsets[i]);
		}
	}
	else
		printf("\nxng_ttf_loca_table_serialize Error:  Undefined indexToLocFormat (%hd)\n", indexToLocFormat);

	return ttf_table_data;

}

static void xng_ttf_cmap_encoding_record_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_CMAP_ENCODING_RECORD_FIELDS
#undef XNG_TTF_TABLE_FIELD
}


/*
0	Default semantics
1	Version 1.1 semantics
2	ISO 10646 1993 semantics (deprecated)
3	Unicode 2.0 or later semantics (BMP only)
4	Unicode 2.0 or later semantics (non-BMP characters allowed)
5	Unicode Variation Sequences
6	Full Unicode coverage (used with type 13.0 cmaps by OpenType)
*/
typedef enum XNG_TTF_UNICODE_PLATFORM_ENCODING_IDS
{
	XNG_TTF_ENCODING_DEFAULT					= 0,
	XNG_TTF_ENCODING_1_1						= 1,
	XNG_TTF_ENCODING_ISO						= 2,
	XNG_TTF_ENCODING_BMP						= 3,
	XNG_TTF_ENCDOING_NON_BMP					= 4,
	XNG_TTF_ENCODING_VARIATION				= 5,
	XNG_TTF_ENCODING_FULL_COVERAGE			= 6
}XNG_TTF_UNICODE_PLATFORM_ENCODING_IDS;


typedef enum XNG_TTF_WINDOWS_PLATFORM_ENCODING_IDS
{
	XNG_TTF_ENCODING_SYMBOL					= 0,
	XNG_TTF_ENCODING_UNICODE_BMP				= 1,
	XNG_TTF_ENCODING_SHIFTJIS				= 2,
	XNG_TTF_ENCODING_PRC						= 3,
	XNG_TTF_ENCODING_BIG5					= 4,
	XNG_TTF_ENCODING_WANSUNG					= 5,
	XNG_TTF_ENCODING_JOHAB					= 6,
	//XNG_TTF_ENCODING_RESERVED				= 7,
	//XNG_TTF_ENCODING_RESERVED				= 8,
	//XNG_TTF_ENCODING_RESERVED				= 9,
	XNG_TTF_ENCODING_UNICODE_UCS4			= 10


}XNG_TTF_WINDOWS_PLATFORM_ENCODING_IDS;

static void xng_ttf_cmap_format4_mapping_table_header_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_CMAP_FORMAT4_MAPPING_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD
}

static uint32_t xng_ttf_cmap_glyph_index_from_format0_mapping_table(uint8_t * ttf_table_data, unsigned long char_code, uint16_t * glyphIndex)
{

	//table has already been serialized
	xng_ttf_cmap_format0_mapping_table * mapping_table = (xng_ttf_cmap_format0_mapping_table*)ttf_table_data;

	//this table only supports a consecutive range of 256 glyph ids
	if( char_code > 255 )
		*glyphIndex = mapping_table->glyphIdArray[0];
	else
		*glyphIndex = 0; //set to missing index;
	
	return (uint32_t)sizeof(xng_ttf_cmap_format0_mapping_table);
}

static uint32_t xng_ttf_cmap_format0_mapping_table_serialize(void * ttf_table_data)
{
	xng_ttf_cmap_format0_mapping_table * mapping_table;
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_CMAP_FORMAT0_MAPPING_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD

	mapping_table = (xng_ttf_cmap_format0_mapping_table*)ttf_table_data;

	//printf("\ntable length =  %hu \n", mapping_table->length );
	//printf("struct length = %u \n", sizeof(xng_ttf_cmap_format0_mapping_table));
	//printf("struct length % 4 = %u \n", sizeof(xng_ttf_cmap_format0_mapping_table)%4);

	assert ( mapping_table->length == sizeof(xng_ttf_cmap_format0_mapping_table) );

	return (uint32_t)sizeof(xng_ttf_cmap_format0_mapping_table);
}

static uint32_t xng_ttf_cmap_glyph_index_from_format4_mapping_table(uint8_t * ttf_table_data, unsigned long char_code, uint16_t * glyphIndex)
{
	uint16_t segCount;//, reservedPad;
	unsigned long tableBytesRead;  
	uint16_t *endCode, *startCode, *idRangeOffset;//, *glyphIndexArray;  
	size_t segCountIndex;
	int16_t *idDelta;
	xng_ttf_cmap_format4_mapping_table_header * mapping_table_header;
	//uint8_t * table_bytes;
	
	//xng_ttf_cmap_format4_mapping_table_header_serialize(ttf_table_data);

	mapping_table_header = (xng_ttf_cmap_format4_mapping_table_header*)ttf_table_data; 

	tableBytesRead = sizeof(xng_ttf_cmap_format4_mapping_table_header);
	
	//get segcount
	segCount = mapping_table_header->segCountX2 / 2;
	//printf("\n seg count = %hu\n", segCount);

	//read all array fields dependent on seg count dynamic size

	//1) Read End characterCode for each segment, last=0xFFFF.
	endCode = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		if( char_code <= endCode[segCountIndex] )
		{
			break;
		}

	}
	tableBytesRead += sizeof(uint16_t) * segCount;
	
	
	
	//2) Read reservedPad	shoud be Set to 0.
	//endCode[segCount] = xnz_be16toh(endCode[segCount]);	
	tableBytesRead += sizeof(uint16_t);

	//3) Read Start character code for each segment. Check that char_code is >= startCode
	startCode = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );

	//glyph index not found in segment
	if(( segCountIndex == segCount - 1) || !(char_code >= startCode[segCountIndex] ) )
		return 0;

	/*
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		startCode[segCountIndex] = xnz_be16toh(startCode[segCountIndex]);
		//printf("\n start code %hu = %hu\n", segCountIndex, startCode[segCountIndex] );
	}
	*/
	tableBytesRead += sizeof(uint16_t) * segCount;

	//4) Read Delta for all character codes in segment.
	idDelta = (int16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	/*
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		idDelta[segCountIndex] = xnz_be16toh(idDelta[segCountIndex]);
	}
	*/
	tableBytesRead += sizeof(int16_t) * segCount;

	//5) Offsets into glyphIdArray or 0
	idRangeOffset = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	
	//tableBytesRead += sizeof(uint16_t) * numGlyphIndices;
	
	//maxGlyphIndexAddress = 0;
	//glyphIndexAddress = 0;
	/*
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{

		//idRangeOffset[segCountIndex] = xnz_be16toh(idRangeOffset[segCountIndex]);

		
	}
	*/
	//numGlyphIndices += 1;
	//tableBytesRead += sizeof(uint16_t) * segCount;

	if( idRangeOffset[segCountIndex] == 0 )
	{
		*glyphIndex = idDelta[segCountIndex] + char_code;
		return 1;
	}
	else
	{
		*glyphIndex = *( &idRangeOffset[segCountIndex] + idRangeOffset[segCountIndex] / 2 + (char_code - startCode[segCountIndex]) );

		if( glyphIndex != 0 )
			glyphIndex += idDelta[segCountIndex]; 

		return 1;
	}

	//tableBytesRead += sizeof(uint16_t) * segCount;
	//glyphIndexArray = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );


	//5) Read glyph index array of arbitrary length
	/*
	glyphIndexArray = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	numGlyphIndices = (maxGlyphIndexAddress - (size_t)glyphIndexArray)/2 + 1; //add one because glyphIndexAddress points to the last glyph index in the glpyhIndexArray, but we want to calculate total size
	for( glyphIndex = 0; glyphIndex < numGlyphIndices; glyphIndex++ )
	{
		glyphIndexArray[glyphIndex] = xnz_be16toh(glyphIndexArray[glyphIndex]);
	}
	tableBytesRead += sizeof(uint16_t) * numGlyphIndices;
	printf("\nnumGlyphIndices = %lu\n", numGlyphIndices);

	xng_ttf_cmap_format4_mapping_table_header_print(ttf_table_data);
	printf("\ntableBytesRead = %lu\n", tableBytesRead);

	assert(mapping_table_header->length == tableBytesRead);
	*/


	//this should never execute
	return 0;
	

}


static uint32_t xng_ttf_cmap_format4_mapping_table_serialize(void * ttf_table_data)
{
	uint16_t segCount;//, reservedPad;
	unsigned long tableBytesRead;  
	uint16_t *endCode, *startCode, *idRangeOffset, *glyphIndexArray;  
	size_t maxGlyphIndexAddress, glyphIndexAddress, glyphIndex, numGlyphIndices,  segCountIndex;
	int16_t *idDelta;
	xng_ttf_cmap_format4_mapping_table_header * mapping_table_header;
	
	xng_ttf_cmap_format4_mapping_table_header_serialize(ttf_table_data);

	mapping_table_header = (xng_ttf_cmap_format4_mapping_table_header*)ttf_table_data; 

	tableBytesRead = sizeof(xng_ttf_cmap_format4_mapping_table_header);
	
	//get segcount
	segCount = mapping_table_header->segCountX2 / 2;
	//printf("\n seg count = %hu\n", segCount);

	//read all array fields dependent on seg count dynamic size

	//1) Read End characterCode for each segment, last=0xFFFF.
	endCode = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		endCode[segCountIndex] = xnz_be16toh(endCode[segCountIndex]);
		//printf("\n end code %hu = %hu\n", segCountIndex, endCode[segCountIndex] );
	}

	tableBytesRead += sizeof(uint16_t) * segCount;
	//	printf("\n end code = %hd\n",  0xFFFF );

	//endCode[segCountIndex] = xnz_be16toh(endCode[segCountIndex]);
	//assert((int16_t)endCode[segCountIndex] == 0xFFFF);

	//printf("\n end code = %hu\n",  0xFFFF );

	//2) Read reservedPad	shoud be Set to 0.
	endCode[segCount] = xnz_be16toh(endCode[segCount]);
	
	assert( endCode[segCount] == 0 );
	
	tableBytesRead += sizeof(uint16_t);

	//3) Read Start character code for each segment.https://superuser.com/questions/593847/how-many-memory-addresses-can-we-get-with-a-32-bit-processor-and-1gb-ram
	startCode = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		startCode[segCountIndex] = xnz_be16toh(startCode[segCountIndex]);
		//printf("\n start code %hu = %hu\n", segCountIndex, startCode[segCountIndex] );
	}
	tableBytesRead += sizeof(uint16_t) * segCount;

	//4) Read Delta for all character codes in segment.
	idDelta = (int16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		idDelta[segCountIndex] = xnz_be16toh(idDelta[segCountIndex]);
	}
	tableBytesRead += sizeof(int16_t) * segCount;

	//5) Offsets into glyphIdArray or 0
	idRangeOffset = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	maxGlyphIndexAddress = 0;
	glyphIndexAddress = 0;
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{

		idRangeOffset[segCountIndex] = xnz_be16toh(idRangeOffset[segCountIndex]);

		//get the maximum glyph index to determine how many glyph entries follow this array
		if( segCountIndex != segCount-1 )
		{
			//calculate glyph index
			if( idRangeOffset[segCountIndex] != 0 )
			{
				//calculate the glyph index for the endCode in each segment
				//assert(1 == 0 );
				//the following two statements are supposed to be equivalent accoridng to the TTF documentation, but only the second one gives the correct address
				//glyphIndexAddress = (size_t)(idRangeOffset[segCountIndex] + 2 * (endCode[segCountIndex] - startCode[segCountIndex]) + &idRangeOffset[segCountIndex]);
				glyphIndexAddress =  (size_t)( &idRangeOffset[segCountIndex] + idRangeOffset[segCountIndex] / 2 + (endCode[segCountIndex] - startCode[segCountIndex]) );

			}

			//if it is greater the current numGlyphIndices, store it
			if( glyphIndexAddress > maxGlyphIndexAddress )
				maxGlyphIndexAddress = glyphIndexAddress;
		}

	}
	//numGlyphIndices += 1;
	tableBytesRead += sizeof(uint16_t) * segCount;

	//printf("\ntableBytesRead = %lu\n", tableBytesRead);

	//5) Read glyph index array of arbitrary length
	if( maxGlyphIndexAddress > 0 )
	{
		glyphIndexArray = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
		numGlyphIndices = (maxGlyphIndexAddress - (size_t)glyphIndexArray)/2 + 1; //add one because glyphIndexAddress points to the last glyph index in the glpyhIndexArray, but we want to calculate total size
		for( glyphIndex = 0; glyphIndex < numGlyphIndices; glyphIndex++ )
		{
			glyphIndexArray[glyphIndex] = xnz_be16toh(glyphIndexArray[glyphIndex]);
		}
		tableBytesRead += sizeof(uint16_t) * numGlyphIndices;

	}
	//printf("\nnumGlyphIndices = %lu\n", numGlyphIndices);

	//xng_ttf_cmap_format4_mapping_table_header_print(ttf_table_data);
	printf("\ncmap format4 table length = %hu\n", mapping_table_header->length);
	printf("\ntableBytesRead = %lu\n", tableBytesRead);


	//assert(mapping_table_header->length == tableBytesRead);

	return tableBytesRead;
	
	/*
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		idRangeOffset[segCountIndex] = xnz_be16toh(idRangeOffset[segCountIndex]);
	}
	tableBytesRead += sizeof(uint16_t) * segCount;
	*/
}


static void xng_ttf_cmap_format6_mapping_table_header_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_CMAP_FORMAT6_MAPPING_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD
}


static uint32_t xng_ttf_cmap_glyph_index_from_format6_mapping_table(uint8_t * ttf_table_data, unsigned long char_code, uint16_t * glyphIndex)
{
	uint16_t entryCount;
	uint32_t  tableBytesRead;//, numGlyphIndices;//, glyphIndex;
	uint16_t *glyphIndexArray;
	//int16_t *idDelta;
	xng_ttf_cmap_format6_mapping_table_header * mapping_table_header;
	
	//xng_ttf_cmap_format6_mapping_table_header_serialize(ttf_table_data);

	mapping_table_header = (xng_ttf_cmap_format6_mapping_table_header*)ttf_table_data;

	tableBytesRead = sizeof(xng_ttf_cmap_format6_mapping_table_header);
	
	//1) Read the glyphIndexArray of size entry count
	glyphIndexArray = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	//tableBytesRead += sizeof(uint16_t) * entryCount;

	//get entryCount
	entryCount = mapping_table_header->entryCount;

	if( char_code >= mapping_table_header->firstCode && char_code <= mapping_table_header->firstCode + mapping_table_header->entryCount )
	{
		*glyphIndex = glyphIndexArray[char_code - mapping_table_header->firstCode];
		return 1;
	}

	return 0;

}

static uint32_t xng_ttf_cmap_format6_mapping_table_serialize(void * ttf_table_data)
{
	uint16_t entryCount;
	uint32_t entryCountIndex, tableBytesRead;//, numGlyphIndices, glyphIndex;
	uint16_t *glyphIndexArray;
	//int16_t *idDelta;
	xng_ttf_cmap_format6_mapping_table_header * mapping_table_header;
	
	xng_ttf_cmap_format6_mapping_table_header_serialize(ttf_table_data);

	mapping_table_header = (xng_ttf_cmap_format6_mapping_table_header*)ttf_table_data;

	tableBytesRead = sizeof(xng_ttf_cmap_format6_mapping_table_header);
	
	//get entryCount
	entryCount = mapping_table_header->entryCount;

	//read all array fields dependent on seg count dynamic size

	//1) Read GlpyphIndexArray of size entryCount
	glyphIndexArray = (uint16_t*)( ((uint8_t*)ttf_table_data) + tableBytesRead );
	for( entryCountIndex = 0; entryCountIndex<entryCount; entryCountIndex++)
	{
		glyphIndexArray[entryCountIndex] = xnz_be16toh(glyphIndexArray[entryCountIndex]);
	}
	tableBytesRead += sizeof(uint16_t) * entryCount;

	//printf("\nmapping_table_header length = %hu\n", mapping_table_header->length);
	//xng_ttf_cmap_format6_mapping_table_header_print(ttf_table_data);
	//printf("\ntableBytesRead = %u\n", tableBytesRead);

	//assert(mapping_table_header->length == tableBytesRead);

	return tableBytesRead;
	
	/*
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		idRangeOffset[segCountIndex] = xnz_be16toh(idRangeOffset[segCountIndex]);
	}
	tableBytesRead += sizeof(uint16_t) * segCount;
	*/
}

static void xng_ttf_cmap_format12_mapping_table_header_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_CMAP_FORMAT12_MAPPING_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD
}

static void xng_ttf_cmap_sequential_map_group_serialize(void * ttf_table_data)
{
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_CMAP_SEQUENTIAL_MAP_GROUP_FIELDS
#undef XNG_TTF_TABLE_FIELD
}


static uint32_t xng_ttf_cmap_glyph_index_from_format12_mapping_table(uint8_t * ttf_table_data, unsigned long char_code, uint16_t * glyphIndex)
{

	unsigned long tableBytesRead;  
	uint32_t mapGroupIndex;
	//uint16_t *endCode, *startCode, *idRangeOffset, *glyphIndexArray;  
	//size_t maxGlyphIndexAddress, glyphIndexAddress, glyphIndex, numGlyphIndices,  segCountIndex;
	//int16_t *idDelta;

	//table has already been serialized
	xng_ttf_cmap_sequential_map_group * map_groups;
	xng_ttf_cmap_format12_mapping_table_header * mapping_table_header = (xng_ttf_cmap_format12_mapping_table_header*)ttf_table_data; 

	tableBytesRead = sizeof(xng_ttf_cmap_format12_mapping_table_header);
	
	map_groups = (xng_ttf_cmap_sequential_map_group*)((uint8_t*)ttf_table_data + sizeof(xng_ttf_cmap_format12_mapping_table_header));

	*glyphIndex = 0; //prepare for the fact that we might not find the glyph index in this table
	for( mapGroupIndex = 0; mapGroupIndex < mapping_table_header->numGroups; mapGroupIndex++)
	{
		if( char_code < map_groups[mapGroupIndex].endCharCode )
		{
			*glyphIndex = map_groups[mapGroupIndex].startGlyphID + (char_code - map_groups[mapGroupIndex].startCharCode);
			break;
		}
		//serialize each map group index
		//xng_ttf_cmap_sequential_map_group_serialize( &(map_groups[mapGroupIndex) );
	}
	
	tableBytesRead += mapping_table_header->numGroups * sizeof(xng_ttf_cmap_sequential_map_group);

	assert ( mapping_table_header->length == tableBytesRead);


	return tableBytesRead;
}

static uint32_t xng_ttf_cmap_format12_mapping_table_serialize(void * ttf_table_data)
{
	//uint16_t segCount;//, reservedPad;
	unsigned long tableBytesRead;  
	uint32_t mapGroupIndex;
	//uint16_t *endCode, *startCode, *idRangeOffset, *glyphIndexArray;  
	//size_t maxGlyphIndexAddress, glyphIndexAddress, glyphIndex, numGlyphIndices,  segCountIndex;
	//int16_t *idDelta;
	xng_ttf_cmap_sequential_map_group * map_groups;
	xng_ttf_cmap_format12_mapping_table_header * mapping_table_header;
	xng_ttf_cmap_format12_mapping_table_header_serialize(ttf_table_data);
	mapping_table_header = (xng_ttf_cmap_format12_mapping_table_header*)ttf_table_data; 

	tableBytesRead = sizeof(xng_ttf_cmap_format12_mapping_table_header);
	
	map_groups = (xng_ttf_cmap_sequential_map_group*)((uint8_t*)ttf_table_data + sizeof(xng_ttf_cmap_format12_mapping_table_header));

	for( mapGroupIndex = 0; mapGroupIndex < mapping_table_header->numGroups; mapGroupIndex++)
	{
		//serialize each map group index
		xng_ttf_cmap_sequential_map_group_serialize( &(map_groups[mapGroupIndex]) );
	}

	tableBytesRead += mapping_table_header->numGroups * sizeof(xng_ttf_cmap_sequential_map_group);

	assert ( mapping_table_header->length == tableBytesRead);


	return tableBytesRead;
	
	/*
	for( segCountIndex = 0; segCountIndex<segCount; segCountIndex++)
	{
		idRangeOffset[segCountIndex] = xnz_be16toh(idRangeOffset[segCountIndex]);
	}
	tableBytesRead += sizeof(uint16_t) * segCount;
	*/
}

//returns nonzero if the glyph index is found, 0 otherwise
static uint32_t xng_ttf_cmap_glyph_index_from_mapping_table(uint8_t * ttf_table_data, unsigned long char_code, uint16_t *glyphIndex)
{
	uint16_t format;
	uint16_t * table16 = (uint16_t*)ttf_table_data;
	
	//format = xnz_be16toh(table16[0]);
	format = table16[0];

	//printf("\nFormat = %hu\n", format);
	assert(format == 0 || format == 4 || format == 6 || format == 12 );

	switch(format) 
	{

			case 0: 
				return xng_ttf_cmap_glyph_index_from_format0_mapping_table(ttf_table_data, char_code, glyphIndex);
				break;
			case 4:  //When building a symbol font for Windows, the platform ID should be 3 and the encoding ID should be 0.
				return xng_ttf_cmap_glyph_index_from_format4_mapping_table(ttf_table_data, char_code, glyphIndex);
				break;
			case 6:  //When building a Unicode font for Windows, the platform ID should be 3 and the encoding ID should be 1. 
				return xng_ttf_cmap_glyph_index_from_format6_mapping_table(ttf_table_data, char_code, glyphIndex);
				break;
			case 12:
				return xng_ttf_cmap_glyph_index_from_format12_mapping_table(ttf_table_data, char_code, glyphIndex);
				break;

				break;
			
			default :
				return 0;
	}

	return 0;

}


static uint32_t xng_ttf_cmap_glyph_index_for_char_code(void * ttf_table_data, unsigned long char_code, uint16_t * glyphIndex)
{
	bool foundGlyphIndex;
	uint8_t * bytes;
	//uint16_t glyphIndex;
	xng_ttf_cmap_table * cmap_table;
	uint32_t numTableBytes;
	uint32_t tableIndex;
	uint8_t * table_bytes;

	xng_ttf_cmap_encoding_record * encoding_records;
	foundGlyphIndex = false;
	cmap_table = (xng_ttf_cmap_table*)ttf_table_data;
	table_bytes = (uint8_t*)ttf_table_data;
	//cmap_table->version = xnz_be16toh(cmap_table->version);
	//cmap_table->numberSubtables = xnz_be16toh(cmap_table->numberSubtables);

	numTableBytes = sizeof(uint16_t)*2;

	//cmap_table->encodingTables = (xng_ttf_cmap_encoding_record*)(((uint8_t*)ttf_table_data) + numTableBytes);

	//printf("\n numberSubtables = %hu\n", cmap_table->numberSubtables);

	//bytes = (((uint8_t*)ttf_table_data) + numTableBytes);
	//cmap_table->encodingTables = (xng_ttf_cmap_encoding_record*)bytes;
	encoding_records = (xng_ttf_cmap_encoding_record*)(((uint8_t*)ttf_table_data) + numTableBytes);

	numTableBytes += sizeof(xng_ttf_cmap_encoding_record) * cmap_table->numberSubtables;
	//cmap_table->encodingTables = (xng_ttf_cmap_encoding_record*)(((uint8_t*)ttf_table_data) + numTableBytes - cmap_table->numberSubtables * sizeof(xng_ttf_cmap_encoding_record));
	
	//find the mapping table corresponding to our platformID in the encoding table
	for( tableIndex = 0; tableIndex < cmap_table->numberSubtables;  tableIndex++ )
	{

#ifdef _WIN32
		if( encoding_records[tableIndex].platformID == 3 )
#else
        if( encoding_records[tableIndex].platformID == 3 )
#endif
		{
			//use the offset from the start of the encoding table to locate the corresponding mapping table
			//printf("\n offset = %u\n", cmap_table->encodingTables[tableIndex].offset);
			if( xng_ttf_cmap_glyph_index_from_mapping_table( table_bytes + encoding_records[tableIndex].offset, char_code, glyphIndex ) )
			{
				foundGlyphIndex = true;
				break;
			}
		}

	}



	//if we didn't find the platform we are on we can do another check for unicode
	if( !foundGlyphIndex )
	{
		//find the mapping table corresponding to our platformID in the encoding table
		for( tableIndex = 0; tableIndex < cmap_table->numberSubtables;  tableIndex++ )
		{

	#ifdef _WIN32
			if( encoding_records[tableIndex].platformID == 0 )
			{
				//use the offset from the start of the encoding table to locate the corresponding mapping table
				//printf("\n offset = %u\n", cmap_table->encodingTables[tableIndex].offset);
				if( xng_ttf_cmap_glyph_index_from_mapping_table( (void*)(((uint8_t*)ttf_table_data) + encoding_records[tableIndex].offset), char_code, glyphIndex ) )
				{
					foundGlyphIndex = true;
					break;
				}
			}
	#endif
		}

	}
	
	return foundGlyphIndex;
	//if( foundGlyphIndex == true )
	//	return 1;
	//else
	//	return 0;

	
	//cmap_table->mappingTables = (void*)(((uint8_t*)ttf_table_data) + numTableBytes);

	//serialize the cmap mapping subtables
	/*
	for( tableIndex = 0; tableIndex < cmap_table->numberSubtables;  tableIndex++ )
	{
		numTableBytes += xng_ttf_cmap_mapping_table_serialize((void*)(((uint8_t*)ttf_table_data) + numTableBytes), cmap_table->encodingTables[tableIndex].platformID, cmap_table->encodingTables[tableIndex].encodingID );
	}
	*/

}



static uint32_t xng_ttf_cmap_mapping_table_serialize(void * ttf_table_data)//, uint16_t platformID, uint16_t encodingID)
{
	//TTF Fonts should all have a Unicode cmap, 
	//either with a Unicode platformID or a Microsoft platformID and appropriate platformSpecificID.

	//Fonts should not have more than one cmap with a Unicode platformID and a type other than 14. 
	//If a font does such multiple cmaps, only one will be used and the others ignored. Which one is used is not defined.

	//Note that because the 'cmap' table uses offsets to the actual data for any platform/platformSpecificID, 
	//it's possible for a type 0/4 and type 3/10 cmap to have the same actual data, not just identical data.

	//The Macintosh platformID is only useful for backwards compatibility with QuickDraw. 
	//If a font lacks one, the system will synthesize one as required using a Unicode cmap as the data source.

	//platformID values other than 0, 1, or 3 are allowed but cmaps using them will be ignored.

	/*
	if( platformID == 0 ) //Unicode
	{
		//Unicode Variation Sequences supported by the font should be specified in the 'cmap' table using a format 14 subtable. 
		//A format 14 subtable must only be used under platform ID 0 and encoding ID 5.
	}
	else if( platformID == 1 ) //Macintosh
	{
		//When the platformID is 1 (Macintosh), the platformSpecificID is a QuickDraw script code.

		//Note that the use of the Macintosh platformID is currently discouraged. Subtables with a Macintosh platformID are only required for backwards compatibility with QuickDraw and will be synthesized from Unicode-based subtables if ever needed.

		//When building a font that will be used on the Macintosh, the platform ID should be 1 and the encoding ID should be 0.

	}
	else if( platformID == 3 ) //Windows
	{
		//Microsoft strongly recommends using a BMP Unicode 'cmap' for all fonts. However, other non-Unicode encodings are also used in existing fonts with the Windows platform. 

		
		
		

		switch(encodingID) {

			case XNG_TTF_ENCODING_SYMBOL:  //When building a symbol font for Windows, the platform ID should be 3 and the encoding ID should be 0.
				break;
			case XNG_TTF_ENCODING_UNICODE_BMP:  //When building a Unicode font for Windows, the platform ID should be 3 and the encoding ID should be 1. 
				break;
			case XNG_TTF_ENCODING_SHIFTJIS:
				break;
			case XNG_TTF_ENCODING_PRC:
				break;
			case XNG_TTF_ENCODING_BIG5:
				break;
			case XNG_TTF_ENCODING_WANSUNG: //not available in Macintosh spec?
				break;
			case XNG_TTF_ENCODING_JOHAB:
				break;
			case XNG_TTF_ENCODING_UNICODE_UCS4:
				//If a font has a 3/10 cmap (Windows, UCS-4), it should also have a 3/1 (Windows, BMP-only) 
				//cmap as well for backwards compatibility with Windows XP. The two should have identical mappings for Unicode's Basic Multilingual Plane.

				break;
			
			default :
				return 0;
		}

	}
	*/
	uint16_t format;
	uint16_t * table16 = (uint16_t*)ttf_table_data;
	//format = *table16;
	format = xnz_be16toh(table16[0]);

	printf("\nFormat = %hu\n", format);
	//printf("\nFormat = %hu\n", xnz_be16toh(format));

	assert(format == 0 || format == 4 || format == 6 || format == 12 );

	switch(format) 
	{
			case 0:
				return xng_ttf_cmap_format0_mapping_table_serialize(ttf_table_data);
				break;
			case 4:  //When building a symbol font for Windows, the platform ID should be 3 and the encoding ID should be 0.
				return xng_ttf_cmap_format4_mapping_table_serialize(ttf_table_data);
				break;
			case 6:  //When building a Unicode font for Windows, the platform ID should be 3 and the encoding ID should be 1. 
				return xng_ttf_cmap_format6_mapping_table_serialize(ttf_table_data);
				break;
			case 12:
				return xng_ttf_cmap_format12_mapping_table_serialize(ttf_table_data);
				break;
			
			default :
				return 0;
	}


    return 0;
}


static int compare_cmap_encoding_record (const void * elem1, const void * elem2)
{
	//xng_ttf_offset_table *table1, *table2;
	//table1 = (xng_ttf_offset_table*)elem1;
	//table2 = (xng_ttf_offset_table*)elem2;


	//if( table1->offset < table2->offset)
	if( ((xng_ttf_cmap_encoding_record*)elem1)->offset < ((xng_ttf_cmap_encoding_record*)elem2)->offset )
		return -1;
	//else
	return 1;

    //return 0;
}


static void* xng_ttf_cmap_table_serialize(void * ttf_table_data)
{
	uint8_t * bytes;
	xng_ttf_cmap_encoding_record * encoding_records;
	//void* encoding_tables;
	xng_ttf_cmap_table * cmap_table;
	uint32_t numTableBytes;
	uint32_t tableIndex;

	cmap_table = (xng_ttf_cmap_table*)ttf_table_data;

	cmap_table->version = xnz_be16toh(cmap_table->version);
	cmap_table->numberSubtables = xnz_be16toh(cmap_table->numberSubtables);

	numTableBytes = sizeof(uint16_t)*2;

	//bytes = (((uint8_t*)ttf_table_data) + numTableBytes);
	//cmap_table->encodingTables
	encoding_records = (xng_ttf_cmap_encoding_record*)(((uint8_t*)ttf_table_data) + numTableBytes);
	//encoding_tables = (((uint8_t*)ttf_table_data) + numTableBytes);
	printf("\n numberSubtables = %hu\n", cmap_table->numberSubtables);

	//serialize the cmap encoding subtables
	for( tableIndex = 0; tableIndex < cmap_table->numberSubtables;  tableIndex++ )
	{
		xng_ttf_cmap_encoding_record_serialize(&(encoding_records[tableIndex]));
		xng_ttf_cmap_encoding_record_print((void*)(((uint8_t*)ttf_table_data) + numTableBytes));
		numTableBytes += sizeof(xng_ttf_cmap_encoding_record);
	}

	//bytes = (((uint8_t*)ttf_table_data) + numTableBytes - cmap_table->numberSubtables * sizeof(xng_ttf_cmap_encoding_record));
	//cmap_table->encodingTables = (xng_ttf_cmap_encoding_record*)bytes;
	//encoding_records = (xng_ttf_cmap_encoding_record*)(((uint8_t*)ttf_table_data) + numTableBytes - cmap_table->numberSubtables * sizeof(xng_ttf_cmap_encoding_record));
	//cmap_table->mappingTables = (void*)(((uint8_t*)ttf_table_data) + numTableBytes);

	//we need to sort the encoding tables so we can traverse the cmap mapping tables in order
	qsort ((void*)(encoding_records), cmap_table->numberSubtables, sizeof(xng_ttf_cmap_encoding_record), compare_cmap_encoding_record);
	xng_ttf_cmap_encoding_record_print(&(encoding_records[0]));
	numTableBytes += xng_ttf_cmap_mapping_table_serialize((void*)(((uint8_t*)ttf_table_data) + encoding_records[0].offset) );//, cmap_table->encodingTables[tableIndex].platformID, cmap_table->encodingTables[tableIndex].encodingID );

	//serialize the cmap mapping subtables IN ORDER (so keep in mind they won't map to the encoding tables above if encoding tables are out of order)
	for( tableIndex = 1; tableIndex < cmap_table->numberSubtables;  tableIndex++ )
	{
		//printf("\n numTableBytes = %u\n", numTableBytes);
		//printf("\n offset = %u\n", cmap_table->encodingTables[tableIndex].offset);
		//assert(numTableBytes == cmap_table->encodingTables[tableIndex].offset);
			xng_ttf_cmap_encoding_record_print(&(encoding_records[tableIndex]));

		//ensure we only serialize each cmap table once !!!
		if( encoding_records[tableIndex].offset > encoding_records[tableIndex-1].offset )
			numTableBytes += xng_ttf_cmap_mapping_table_serialize((void*)(((uint8_t*)ttf_table_data) + encoding_records[tableIndex].offset) );//, cmap_table->encodingTables[tableIndex].platformID, cmap_table->encodingTables[tableIndex].encodingID );
		
	}

	return ttf_table_data;

}

static xng_ttf_glyf_table_header * xng_ttf_glyf_table_header_serialize(void * ttf_table_data)
{
	xng_ttf_glyf_table_header * glyf_header = (xng_ttf_glyf_table_header *)ttf_table_data;
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(/*&(((xng_ttf_table_data *)ttf_table_data)->varName)*/ &(glyf_header->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_GLYF_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD

	return glyf_header;
}

//modifies the 
//returns the total bytes read
static uint32_t xng_ttf_glyf_serialize_coord_delta(uint8_t flag, uint8_t byteFlag, uint8_t deltaFlag, uint8_t ** axisCoordinateDeltas, int16_t * deltaSum, uint8_t numRepeatFlags, uint8_t serialize )
{
	uint8_t repeatFlagIndex;
	//uint32_t byteSum;
	//int16_t serializedAxisCoordDelta = 0;

	//serialize corresponding contour point x-coordinate
	if ( flag & byteFlag ) 
	{
		//printf("\n one byte\n ");

		//x or y value is only one byte (uint8_t)
        if ( flag & deltaFlag ) 
		{
			//value is to be applied as positive delta
			for( repeatFlagIndex = 0 ; repeatFlagIndex<numRepeatFlags; repeatFlagIndex++ )
			{
				*deltaSum += (int16_t)((*axisCoordinateDeltas)[repeatFlagIndex]);
			}
			//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
		} 
		else 
		{
			//value is to be applied as negative delta
			//value -= file.getUint8();
			
			for( repeatFlagIndex = 0 ; repeatFlagIndex<numRepeatFlags; repeatFlagIndex++ )
			{
				*deltaSum -= (int16_t)((*axisCoordinateDeltas)[repeatFlagIndex]);
			}
			//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas) * -1;

        }
		
		//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
		//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
		*axisCoordinateDeltas = *axisCoordinateDeltas + sizeof(uint8_t) * numRepeatFlags;
		
		//add to the byte sum passed as input to this function for convenience
		//if( byteSum )
		//	(*byteSum) += numRepeatFlags * sizeof(uint8_t);

		return numRepeatFlags;//*sizeof(uint8_t)//byteSum;
    } 
	else if ( !(flag & deltaFlag) ) 
	{
		//x or y value is two bytes (int16_t) and needs to be serialized to system byte format
		int16_t * axisCoordDelta = (int16_t*)(*axisCoordinateDeltas);

		if( serialize > 0 )
		{
			for( repeatFlagIndex = 0 ; repeatFlagIndex<numRepeatFlags; repeatFlagIndex++ )
			{
				axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
				*deltaSum += axisCoordDelta[repeatFlagIndex];
			}
			//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
		}
		else
		{
			for( repeatFlagIndex = 0 ; repeatFlagIndex<numRepeatFlags; repeatFlagIndex++ )
			{
				//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
				*deltaSum += axisCoordDelta[repeatFlagIndex];
			}
		}
		//serializedAxisCoordDelta = *axisCoordDelta;
		
		//printf("\n two bytes\n");

		//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
		(*axisCoordinateDeltas) = (*axisCoordinateDeltas) + sizeof(uint16_t) * numRepeatFlags;
		
		//add to the byte sum passed as input to this function for convenience
		//if( byteSum )
		//	(*byteSum) += sizeof(uint16_t);

		return numRepeatFlags * sizeof(uint16_t);//serializedAxisCoordDelta;


    } 
	else
	{
		//in this case, there is no change between the last contour point x value and this one
        //that is, the x value delta 0 and not present represented by actual data in the table
		//so there is nothing to serialize
		//printf("\n same as last \n");

		//serializedAxisCoordDelta = 0;

		//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
		//*axisCoordinateDeltas = *axisCoordinateDeltas + sizeof(uint16_t);
		
		//add to the byte sum passed as input to this function for convenience
		//*byteSum += 0;//sizeof(uint16_t);

		return 0;

    }
	

	return 0;
}

//this function will just serialize the simple glyf table up to the flags property, but will not serialize the actual
//glyf contour point data
static uint32_t xng_ttf_simple_glyf_table_header_serialize(void * ttf_table_data, int16_t numContours, xng_ttf_glyph * glyph)
{
	uint32_t totalBytes;
	uint32_t n;//, ui;
	//uint32_t flagIndex, numFlags;//, xIndex, yIndex;
	uint32_t numContourPoints;
	uint16_t instructionLength;

	//store contour x and y coordinates as sums of deltas
	//as we read through the coordinate buffers
	//int16_t xcoord, ycoord;
	
	unsigned * bits;
	unsigned int bit;
	uint8_t *flags, *instructions;//, *xcoordDeltas, *ycoordDeltas;
	uint16_t * endPtsOfContours = (uint16_t*)ttf_table_data;

	//keep a count of the total bytes we've read in this function
	totalBytes = 0;

	//serialize endPtsOfContours array of indices to pt arrays
	//we add 1 to the numContours to serialize the following instructionLength var
	//printf("\n\n numContours = %hu\n", numContours);

	numContourPoints = 0;
	for( n = 0; n < numContours; n++ )
	{
		//convert contour end points to system byte format
		endPtsOfContours[n] = xnz_be16toh(endPtsOfContours[n]);

		//get the maximum contour point index as we read through the contour end point indices
		//to learn how many contour points there are in total
		if( endPtsOfContours[n] > numContourPoints )
		{
			numContourPoints = endPtsOfContours[n];
		}
	}

	//numContourPoints is an index value, so we must add we to get total number  
	numContourPoints += 1;
	totalBytes += sizeof(uint16_t) * numContours;

	endPtsOfContours[numContours] = xnz_be16toh(endPtsOfContours[numContours]);
	//serialize instructionLength
	instructionLength = endPtsOfContours[numContours];
	totalBytes += sizeof(uint16_t);

	instructions = (uint8_t*)(ttf_table_data) + totalBytes;
	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//push past the instructions
	//for( n=0; n<instructionLength; n++ )
	//{
	//
	//}

	//printf("\n\ninstruction length = %hu\n", instructionLength);
	totalBytes += instructionLength;// * sizeof(uint8_t);

	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//get a pointer to the flags portion of the simple glyf table
	//we must read through the flags to count their total number
	flags = (uint8_t*)(ttf_table_data) + totalBytes;

	if( glyph )
	{
		//glyph->numContours = numContours;
		glyph->endPtsOfContours = endPtsOfContours;

		glyph->numInstructions = instructionLength;
		glyph->instructions = instructions;

		//glyph->numFlags = numFlags;
		glyph->flags = flags;

		//glyph->xDeltas;
		//glyph->yDeltas;
		
	}

	return totalBytes;

	
}

//#define MAX(x, y) ((x) > (y) ? (x) : (y))
//#define MIN(x, y) ((x) < (y) ? (x) : (y))

// SSE3 instrinsics for cumulative sum and conversion to pixels
#ifdef XNG_TTF_USE_SIMD_SSE
XNG_TTF_API XNG_TTF_INLINE void xng_ttf_accumulate_sse(const float *in, uint8_t *out, uint32_t n) {
 
  uint32_t i;
  __m128 y;
  __m128i z;
  __m128 offset = _mm_setzero_ps();
  __m128i mask = _mm_set1_epi32(0x0c080400);
  __m128 sign_mask = _mm_set1_ps(-0.f);
  for (i = 0; i < n; i += 4) {
    __m128 x = _mm_load_ps(&in[i]);
    x = _mm_add_ps(x, _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(x), 4)));
    x = _mm_add_ps(x, _mm_shuffle_ps(_mm_setzero_ps(), x, 0x40));
    x = _mm_add_ps(x, offset);
    y = _mm_andnot_ps(sign_mask, x);  // fabs(x)
    y = _mm_min_ps(y, _mm_set1_ps(1.0f));
    y = _mm_mul_ps(y, _mm_set1_ps(255.0f));
    z = _mm_cvtps_epi32(y);
    z = _mm_shuffle_epi8(z, mask);
    _mm_store_ss((float *)&out[i], _mm_castsi128_ps(z));
    offset = _mm_shuffle_ps(x, x, _MM_SHUFFLE(3, 3, 3, 3));
  }
}
#endif 

static unsigned int sqrtI( unsigned long sqrtArg )
{
unsigned int answer, x;
unsigned long temp;
 
 
if ( sqrtArg == 0 ) return 0; // undefined result
if ( sqrtArg == 1 ) return 1; // identity
answer = 0;                     // integer square root
for( x=0x8000; x>0; x=x>>1 )
{                                 // 16 bit shift
answer |= x;             // possible bit in root
temp = answer * answer; // fast unsigned multiply
if (temp == sqrtArg) break; // exact, found it
if (temp > sqrtArg) answer ^= x; // too large, reverse bit
}
return answer; // approximate root
}

/*
// this computes 1 / sqrt(number)
float Q_rsqrt( float number )
{
        long i;
        float x2, y;
        const float threehalfs = 1.5F;
 
        x2 = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
 
        return y;
}
*/

static void xng_fpf_buffer_draw_quadratic_from_points(float * image_buffer, int16_t *width, int16_t * height, xnz_float2 *p0, xnz_float2 *p1, xnz_float2 *p2)
{
	/*
	//println!("draw_quad {} {} {}", p0, p1, p2);
        let devx = p0.x - 2.0 * p1.x + p2.x;
        let devy = p0.y - 2.0 * p1.y + p2.y;
        let devsq = devx * devx + devy * devy;
        if devsq < 0.333 {
            self.draw_line(p0, p2);
            return
        }
        let tol = 3.0;
        let n = 1 + (tol * (devx * devx + devy * devy)).sqrt().sqrt().floor() as usize;
        //println!("n = {}", n);
        let mut p = *p0;
        let nrecip = recip(n as f32);
        let mut t = 0.0;
        for _i in 0 .. n - 1 {
            t += nrecip;
            let pn = Point::lerp(t, &Point::lerp(t, p0, p1), &Point::lerp(t, p1, p2));
            self.draw_line(&p, &pn);
            p = pn;
        }
        self.draw_line(&p, p2);
		*/
		
		float devx, devy, devsq, tol, nrecip, t;
		unsigned int n, i;
		xnz_float2 p, pn, lerp1, lerp2;

		devx = (p0->x - p1->x) + (p2->x-p1->x);//p0->x - 2.f * p1->x + p2->x;
		devy = (p0->y - p1->y) + (p2->y-p1->y) ;//p0->y - 2.f * p1->y + p2->y;
		devsq = devx * devx + devy * devy;
		//printf("\ndraw quadratic from {%f, %f} to {%f, %f} to {%f, %f}\n", p0->x, p0->y, p1->x, p1->y, p2->x, p2->y);
		//devsq = (p0->x - p1->x) + (p2->x-p1->x) + (p0->x - p1->x) + (p2->x-p1->x)  ;
		//devsq += (p2->x+p1->x) * (p2->y-p1->y);

		if( devsq < 0.333f )
		{
			xng_fpf_buffer_draw_line_from_points(image_buffer, width, height, p0, p2);
			return ;//CR_IMAGE_API_SUCCESS;
		}
		
		//cr_image_buffer_draw_line_from_xnz_float2_points(image_buffer, width, height, p0, p1);
		//cr_image_buffer_draw_line_from_xnz_float2_points(image_buffer, width, height, p1, p2);

		//return CR_IMAGE_API_SUCCESS;
		
		tol = 3.f;
		n = 1 + (uint32_t)( sqrt(sqrt(tol * devsq)) );
		p = *p0;
		nrecip = 1.f / (float)n;
		t = 0.f;

		for( i=0; i < n-1; i++ )
		{
			t += nrecip;
			//lerp1 = xnz_float2_lerp(t, p0, p1);
			lerp1.vector[0] = p0->vector[0] + t * (p1->x - p0->x);
			lerp1.vector[1] = p0->vector[1] + t * (p1->y - p0->y);
			//lerp2 = xnz_float2_lerp(t, p1, p2);
			lerp2.vector[0] = p1->vector[0] + t * (p2->x - p1->x);
			lerp2.vector[1] = p1->vector[1] + t * (p2->y - p1->y);
			//pn = xnz_float2_lerp(t, &lerp1, &lerp2);
			pn.vector[0] = lerp1.vector[0] + t * (lerp2.x - lerp1.x);
			pn.vector[1] = lerp1.vector[1] + t * (lerp2.y - lerp1.y);
			
			xng_fpf_buffer_draw_line_from_points(image_buffer, width, height, &p, &pn);
			p = pn;

		}

		xng_fpf_buffer_draw_line_from_points(image_buffer, width, height, &p, p2);
		
		return;
}


static void xng_ttf_glyf_draw_closing_contour_point_to_buffer(xng_ttf_glyph * glyph, uint8_t flag, uint8_t * bezierFlag, xnz_float2 *current_point, xnz_float2 *last_point, xnz_float2 *first_on_curve_point, xnz_float2 *first_off_curve_point, xnz_float2 *last_off_curve_point)
{
		//if( contourClosing )
		//{
				//printf("\nContour closing \n");
				/*
				if self.closing {
                if self.alldone {
                    return None
                } else {
                    match (self.first_offcurve, self.last_offcurve) {
                        (None, None) => {
                            self.alldone = true;
                            return Some(LineTo(self.first_oncurve.unwrap()))
                        },
                        (None, Some(last_offcurve)) => {
                            self.alldone = true;
                            return Some(QuadTo(last_offcurve, self.first_oncurve.unwrap()))
                        },
                        (Some(first_offcurve), None) => {
                            self.alldone = true;
                            return Some(QuadTo(first_offcurve, self.first_oncurve.unwrap()))
                        },
                        (Some(first_offcurve), Some(last_offcurve)) => {
                            self.last_offcurve = None;
                            return Some(QuadTo(last_offcurve, Point::lerp(0.5, &last_offcurve, &first_offcurve)))
                        }
                    }
                }
				*/


				if( ((*bezierFlag) & XNG_TTF_FIRST_OFF_CURVE_POINT) ) //if the first off curve point has been set then..
				{

					if( ((*bezierFlag) & XNG_TTF_LAST_OFF_CURVE_POINT) )  // if the last off_curve point has been set
					{
						
						//QuadTo(last_offcurve, Point::lerp(0.5, &last_offcurve, &first_offcurve))
						/*
						//define the points we need to render the quadratic bezier curve
						xnz_float2 transform_last, transform_last_off_curve, lerp, transform_lerp, transform_first_off_curve, transform_first_on_curve;
						//printf("\nClosing QuadTo(last_off_curve, lerp(last_off_curve, first_off_curve))\n");

						//amass and transform the points we need to render the quadratic curve
						transform_last = xnz_float2_affine_transform(&(glyph->affine_transform), last_point);
						transform_last_off_curve = xnz_float2_affine_transform(&(glyph->affine_transform), last_off_curve_point);
						//transform_current = xnz_float2_affine_transform(&(glyph->affine_transform), &current_point);
						lerp  = xnz_float2_lerp(0.5f, last_off_curve_point, first_off_curve_point);
						transform_lerp = xnz_float2_affine_transform(&(glyph->affine_transform), &lerp);

						//draw the quadratic bezier curve using our transformed control points
						cr_image_buffer_draw_quadratic_from_xnz_float2_points(glyph->accumulation_buffer, glyph->bb.width, glyph->bb.height, &transform_last, &transform_last_off_curve, &transform_lerp);
						*/

						xnz_float2 lerp;// = xnz_float2_lerp(0.5f, last_off_curve_point, first_off_curve_point);
						lerp.vector[0] = last_off_curve_point->vector[0] + 0.5f * (first_off_curve_point->x - last_off_curve_point->x);
						lerp.vector[1] = last_off_curve_point->vector[1] + 0.5f * (first_off_curve_point->y - last_off_curve_point->y);

						xng_fpf_buffer_draw_quadratic_from_points(glyph->accumulation_buffer, &(glyph->bb.width), &(glyph->bb.height), last_point, last_off_curve_point, &lerp);
						
						//set the last_point holder to the last bezier control point
						*last_point = lerp;//xnz_float2_lerp(0.5, &last_off_curve_point, &current_point);

						//unset the last off curve point
						//last_off_curve_point.x =  last_off_curve_point.y = 0;
						(*bezierFlag) &= ~XNG_TTF_LAST_OFF_CURVE_POINT;

						
						//QuadTo(first_offcurve, self.first_oncurve.unwrap())
						/*
						//amass and transform the points we need to render the quadratic curve
						transform_last = xnz_float2_affine_transform(&(glyph->affine_transform), last_point);
						transform_first_off_curve = xnz_float2_affine_transform(&(glyph->affine_transform), first_off_curve_point);
						transform_first_on_curve = xnz_float2_affine_transform(&(glyph->affine_transform), first_on_curve_point);

						//draw the quadratic bezier curve using our transformed control points
						cr_image_buffer_draw_quadratic_from_xnz_float2_points(glyph->accumulation_buffer, glyph->bb.width, glyph->bb.height, &transform_last, &transform_first_off_curve, &transform_first_on_curve);
						*/
						
						xng_fpf_buffer_draw_quadratic_from_points(glyph->accumulation_buffer, &(glyph->bb.width), &(glyph->bb.height), last_point, first_off_curve_point, first_on_curve_point);

						//set the last_point holder to the last bezier control point
						*last_point = *first_on_curve_point;
						
						
					}
					//if the last_off_curve_point has not been set
					//if( !((*bezierFlag) & XNG_TTF_LAST_OFF_CURVE_POINT) ) //&& !(flag & XNG_TTF_ON_CURVE_POINT ) )
					else
					{
						
						//QuadTo(first_offcurve, self.first_oncurve.unwrap())

						//define the points we need to render the quadratic bezier curve
						/*
						xnz_float2 transform_last, transform_first_off_curve, transform_first_on_curve;
						//printf("\nClosing QuadTo(first_off_curve, first_on_curve)\n");

						//amass and transform the points we need to render the quadratic curve
						transform_last = xnz_float2_affine_transform(&(glyph->affine_transform), last_point);
						transform_first_off_curve = xnz_float2_affine_transform(&(glyph->affine_transform), first_off_curve_point);
						transform_first_on_curve =xnz_float2_affine_transform(&(glyph->affine_transform), first_on_curve_point);

						//draw the quadratic bezier curve using our transformed control points
						cr_image_buffer_draw_quadratic_from_xnz_float2_points(glyph->accumulation_buffer, glyph->bb.width, glyph->bb.height, &transform_last, &transform_first_off_curve, &transform_first_on_curve);
						*/

						xng_fpf_buffer_draw_quadratic_from_points(glyph->accumulation_buffer, &(glyph->bb.width), &(glyph->bb.height), last_point, first_off_curve_point, first_on_curve_point);

						//set the last_point to the last quadratic bezier control point value
						*last_point = *first_on_curve_point;
						

					}

				}

				//check if the first off curve point hasn't been set then...
				//if( !((*bezierFlag) & XNG_TTF_FIRST_OFF_CURVE_POINT) )
				else
				{
					if( ((*bezierFlag) & XNG_TTF_LAST_OFF_CURVE_POINT) )  // if the last off_curve point has been set
					{
						// QuadTo(last_offcurve, self.first_oncurve.unwrap())
						
						//define the points we need to render the quadratic bezier curve
						/*
						xnz_float2 transform_last, transform_last_off_curve, transform_first_on_curve;
						//printf("\nClosing QuadTo(last_off_curve, first_on_curve)\n");

						//amass and transform the points we need to render the quadratic curve
						transform_last = xnz_float2_affine_transform(&(glyph->affine_transform), last_point);
						transform_last_off_curve = xnz_float2_affine_transform(&(glyph->affine_transform), last_off_curve_point);
						transform_first_on_curve =xnz_float2_affine_transform(&(glyph->affine_transform), first_on_curve_point);

						//draw the quadratic bezier curve using our transformed control points
						cr_image_buffer_draw_quadratic_from_xnz_float2_points(glyph->accumulation_buffer, glyph->bb.width, glyph->bb.height, &transform_last, &transform_last_off_curve, &transform_first_on_curve);
						*/
						
						xng_fpf_buffer_draw_quadratic_from_points(glyph->accumulation_buffer, &(glyph->bb.width), &(glyph->bb.height), last_point, last_off_curve_point, first_on_curve_point);

						//set the last_point to the last quadratic bezier control point value
						*last_point = *first_on_curve_point;
						
					}
					//if the last_off_curve_point has not been set
					//if( !((*bezierFlag) & XNG_TTF_LAST_OFF_CURVE_POINT) ) //&& !(flag & XNG_TTF_ON_CURVE_POINT ) )
					else
					{
						
						//LineTo(first_oncurve)

						//amass and transform the points we need to render the line
						/*
						xnz_float2 transform_last, transform_first_on_curve;
						//printf("\nClosing LineTo(first_on_curve)\n");
						//amass and transform the points we need to render the quadratic curve
						transform_last = xnz_float2_affine_transform(&(glyph->affine_transform), last_point);
						transform_first_on_curve =xnz_float2_affine_transform(&(glyph->affine_transform), first_on_curve_point);

						
						//draw line from last point to first on curve point
						cr_image_buffer_draw_line_from_xnz_float2_points(glyph->accumulation_buffer, glyph->bb.width, glyph->bb.height, &transform_last, &transform_first_on_curve, true );
						*/

						xng_fpf_buffer_draw_line_from_points(glyph->accumulation_buffer, &(glyph->bb.width), &(glyph->bb.height), last_point, first_on_curve_point);

						//set the last point holder to the value of first_on_curve_point
						*last_point = *first_on_curve_point;
						
					}
					
				}
				

				//there are no remaining points in this contour, it is closing
				/*
				contourIndex++;
				numCurrentContourPoints = endPtsOfContours[contourIndex] + 1;
				*/
				*bezierFlag = 0;
				//printf("\nContour closed \n");
				return;
			//}
}

static void xng_ttf_glyf_draw_contour_point_to_buffer(xng_ttf_glyph * glyph, uint8_t flag, uint8_t * bezierFlag, xnz_float2 *current_point, xnz_float2 *last_point, xnz_float2 *first_on_curve_point, xnz_float2 *first_off_curve_point, xnz_float2 *last_off_curve_point)//, bool contourClosing)
{

	

			if( ((*bezierFlag) & XNG_TTF_FIRST_ON_CURVE_POINT) )//the first_on_curve_point has already been set
			{
				//else if the last_off_curve_point has already beens set
				if( ((*bezierFlag) & XNG_TTF_LAST_OFF_CURVE_POINT) )
				{
					if( (flag & XNG_TTF_ON_CURVE_POINT ) ) //if the point is on the curve 
					{
						
						//QuadTo(last_offcurve, p)

						//define the points we need to render the quadratic bezier curve
						/*
						xnz_float2 transform_last, transform_last_off_curve, transform_current;
						
						//amass and transform the points we need to render the quadratic curve
						transform_last = xnz_float2_affine_transform(&(glyph->affine_transform), last_point);
						transform_last_off_curve = xnz_float2_affine_transform(&(glyph->affine_transform), last_off_curve_point);
						transform_current = xnz_float2_affine_transform(&(glyph->affine_transform), current_point);

						//draw the quadratic bezier curve using our transformed control points
						cr_image_buffer_draw_quadratic_from_xnz_float2_points(glyph->accumulation_buffer, glyph->bb.width, glyph->bb.height, &transform_last, &transform_last_off_curve, &transform_current);
						*/

						xng_fpf_buffer_draw_quadratic_from_points(glyph->accumulation_buffer, &(glyph->bb.width), &(glyph->bb.height), last_point, last_off_curve_point, current_point);
						
						//set the last_point holder to the last quadratic bezier control point value
						*last_point = *current_point;

						//unset the last off curve point
						//last_off_curve_point.x =  last_off_curve_point.y = 0;
						(*bezierFlag) &= ~XNG_TTF_LAST_OFF_CURVE_POINT;
						//printf("\nQuadTo(last_off_curve, lerp(last_off_curve, current_point))");
						//printf("\nUnset(last_off_curve)\n");
						
					}
					//if the point is NOT on the curve
					//if( !(flag & XNG_TTF_ON_CURVE_POINT ) )
					else
					{
						
						// QuadTo(last_offcurve, Point::lerp(0.5, &last_offcurve, &p));

						//define the points we need to render the quadratic bezier curve
						/*
						xnz_float2 transform_last_off_curve, transform_last, transform_current, lerp, transform_lerp;

						//amass and transform the points we need to render the quadratic curve
						transform_last = xnz_float2_affine_transform(&(glyph->affine_transform), last_point);
						transform_last_off_curve = xnz_float2_affine_transform(&(glyph->affine_transform), last_off_curve_point);
						transform_current = xnz_float2_affine_transform(&(glyph->affine_transform), current_point);
						lerp  = xnz_float2_lerp(0.5f, last_off_curve_point, current_point);
						transform_lerp = xnz_float2_affine_transform(&(glyph->affine_transform), &lerp);//xnz_float2_lerp(0.5f, &transform_last_off_curve, &transform_current);//xnz_float2_affine_transform(&(glyph->affine_transform), &lerp);
						
						//draw the quadratic bezier curve using our transformed control points
						cr_image_buffer_draw_quadratic_from_xnz_float2_points(glyph->accumulation_buffer, glyph->bb.width, glyph->bb.height, &transform_last, &transform_last_off_curve, &transform_lerp);
						*/

						xnz_float2 lerp;//  = xnz_float2_lerp(0.5f, last_off_curve_point, current_point);
						lerp.vector[0] = last_off_curve_point->vector[0] + 0.5f * (current_point->x - last_off_curve_point->x);
						lerp.vector[1] = last_off_curve_point->vector[1] + 0.5f * (current_point->y - last_off_curve_point->y);
						
						xng_fpf_buffer_draw_quadratic_from_points(glyph->accumulation_buffer, &(glyph->bb.width), &(glyph->bb.height), last_point, last_off_curve_point, &lerp);
			
						//set the last_point holder to the last bezier control point
						*last_point = lerp;//xnz_float2_lerp(0.5, &last_off_curve_point, &current_point);

						//set the last off curve point to the current_point
						*last_off_curve_point = *current_point;
						(*bezierFlag) |= XNG_TTF_LAST_OFF_CURVE_POINT;
						//printf("\nQuadTo(last_off_curve, lerp(last_off_curve, current_point))");
						//printf("\nSet(last_off_curve)\n");
						
						
					}
					

				}
				//if the last_off_curve_point has not been set
				//if( !((*bezierFlag) & XNG_TTF_LAST_OFF_CURVE_POINT) ) //&& !(flag & XNG_TTF_ON_CURVE_POINT ) )
				else
				{
					if( (flag & XNG_TTF_ON_CURVE_POINT ) )
					{
						
						//LineTo(p)
						/*
						xnz_float2 transform_last, transform_current;

						transform_last = xnz_float2_affine_transform(&(glyph->affine_transform), last_point);
						transform_current =xnz_float2_affine_transform(&(glyph->affine_transform), current_point);
						cr_image_buffer_draw_line_from_xnz_float2_points(glyph->accumulation_buffer, glyph->bb.width, glyph->bb.height, &transform_last, &transform_current, true );
						*/

						xng_fpf_buffer_draw_line_from_points(glyph->accumulation_buffer, &(glyph->bb.width), &(glyph->bb.height), last_point, current_point );
						
						*last_point = *current_point;
						//(*bezierFlag) &= ~XNG_TTF_LAST_OFF_CURVE_POINT;
						//printf("\nLineTo(current_on_curve)\n");
						//if (n > 3 )
						//	break;
						
					}
					//if the point is NOT on the curve
					//if( !(flag & XNG_TTF_ON_CURVE_POINT ) )
					else
					{
						
						//set the last off curve point for the first time
						*last_off_curve_point = *current_point;
						(*bezierFlag) |= XNG_TTF_LAST_OFF_CURVE_POINT;
						//printf("\nNO DRAW or MOVE");
						//printf("\nSet(last_off_curve)\n");
						
					}
					

				}
				



			}

			//if the first on curve point hasn't been set then...
			//if( !((*bezierFlag) & XNG_TTF_FIRST_ON_CURVE_POINT) )
			else
			{
				//check if the corresponding point(s) is/are on the curve
				if( flag & XNG_TTF_ON_CURVE_POINT)
				{
					
					//if it is on the curve then we have found the first on curve point
					//first_on_curve_point.x = xcoord;
					//first_on_curve_point.y = ycoord;
					*first_on_curve_point = *current_point;
					(*bezierFlag) |= XNG_TTF_FIRST_ON_CURVE_POINT; //make sure to set flags indicating we've set this point for the first time

					//MoveTo
					//printf("\nMove To(first_on_curve)");
					//printf("\nSet(first_on_curve)\n");

					*last_point = *current_point;
					
				}
				else if( ((*bezierFlag) & XNG_TTF_FIRST_OFF_CURVE_POINT) )//if the first_off_curve_point has been set then the current point is an on curve lerp point 
				{
					
					xnz_float2 midp;
					//midp = xnz_float2_lerp(0.5f, first_off_curve_point, current_point);
					midp.vector[0] = first_off_curve_point->vector[0] + 0.5f * (current_point->x - first_off_curve_point->x);
					midp.vector[1] = first_off_curve_point->vector[1] + 0.5f * (current_point->y - first_off_curve_point->y);

					//(MoveTo(midp));
					*first_on_curve_point = midp;
					*last_off_curve_point = *current_point;
					
					(*bezierFlag) |= XNG_TTF_FIRST_ON_CURVE_POINT;  //fyi, this needs to be set here to ensure proper logic on subsequent passes
					(*bezierFlag) |= XNG_TTF_LAST_OFF_CURVE_POINT;
					
					*last_point = midp;

					//printf("\nMove To(lerp(first_off_curve, current_on_curve_midpoint))");
					//printf("\nSet(first_on_curve)");
					//printf("\nSet(last_off_curve)\n");
					
				}
				//check if the first off curve point hasn't been set then...
				else //if( !((*bezierFlag) & XNG_TTF_FIRST_OFF_CURVE_POINT) )
				{
					
					*first_off_curve_point = *current_point;
					(*bezierFlag) |= XNG_TTF_FIRST_OFF_CURVE_POINT; //make sure to set flags indicating we've set this point for the first time
					//printf("\nNO DRAW or MOVE");
					//printf("\nSet(first_off_curve)\n");
					
				}
				

			}

			//return;
			/*
			numCurrentContourPoints--;

			if(  numCurrentContourPoints == 0 )
			{
				printf("\nContour %d closing \n", contourIndex);
			*/
			
	

}

//this function assumes values up to simple glyf table flags property has already been serialized into our memory representation
static uint32_t xng_ttf_simple_glyf_table_draw_contour_data(void * ttf_table_data, xng_ttf_glyph * glyph, xnz_float_affine_transform * glyph_transform)
{

	int16_t delta;//, value;
	unsigned char value, ivalue;
	//cr_float4 controlPoint;
	int16_t xCoords[256], yCoords[256]; //arrays for holding x and y coordinates for drawing to accumulating buffer when reading repeating flags
	xnz_float2 first_on_curve_point,  first_off_curve_point, last_off_curve_point, current_point, last_point;
	uint8_t serializeX, serializeY;  //flag to indicate whether x and y delta values have already been serialized in our memory representation
	uint32_t totalBytes;
	uint32_t n;//, ui;
	uint16_t numContourPoints;
	uint16_t instructionLength;
	uint8_t bezierFlag;// = 0;

	uint16_t x_size;// y_size;  //vars to hold size of x and y delta vector elements as we read through array of uint8_t flags
	//uint16_t xIndex, yIndex, flagIndex;
	uint16_t numXDeltaVectorBytes, numFlags, flagIndex;  //vars to hold counts of num elements in flags array and x and y delta vectors as we read through array of uint8_t flags
	uint8_t flag, repeatCount, repeatFlagIndex;
	uint8_t *flagPtr;
	//store contour x and y coordinates as sums of deltas
	//as we read through the coordinate buffers
	int16_t xcoord, ycoord;
	int16_t numContours;
	//uint32_t pointIndex;
	
	//unsigned * bits;
	//unsigned int bit;
	uint8_t *flags, *instructions, *xcoordDeltas, *ycoordDeltas;
	int16_t contourIndex;
	int16_t numCurrentContourPoints;

	uint16_t * endPtsOfContours = (uint16_t*)ttf_table_data;

	//we need to store data in a glyph
	assert(glyph);

	//keep a count of the total bytes we've read in this function
	totalBytes = 0;

	//serialize endPtsOfContours array of indices to pt arrays
	//we add 1 to the numContours to serialize the following instructionLength var
	//printf("\n\n numContours = %hu\n", numContours);

	numContours = glyph->header->numberOfContours;
	numContourPoints = 0;
	//printf("\n numContours = %hu\n", numContours);

	/*
	for( contourIndex = 0; contourIndex < numContours; contourIndex++ )
	{
		//this function assumes values up to simple glyf table flags property has already been serialized
		endPtsOfContours[n] = xnz_be16toh(endPtsOfContours[n]);
		//printf("\n numContourPoints[%hd] = %hu\n", contourIndex, endPtsOfContours[contourIndex]);
		//get the maximum contour point index as we read through the contour end point indices
		//to learn how many contour points there are in total
		//if( endPtsOfContours[contourIndex] > numContourPoints )
		//{
		//	numContourPoints = endPtsOfContours[contourIndex];
		//}
	}
	*/

	//numContourPoints is an index value, so we must add we to get total number  
	numContourPoints = endPtsOfContours[numContours - 1] + 1;
	totalBytes += sizeof(uint16_t) * numContours;

	//this function assumes values up to simple glyf table flags property has already been serialized
	//endPtsOfContours[numContours] = xnz_be16toh(endPtsOfContours[numContours]);
	
	//serialize instructionLength
	instructionLength = endPtsOfContours[numContours];
	totalBytes += sizeof(uint16_t);

	instructions = (uint8_t*)(ttf_table_data) + totalBytes;	
	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//push past the instructions
	//for( n=0; n<instructionLength; n++ )
	//{
	//
	//}

	//printf("\n\ninstruction length = %hu\n", instructionLength);
	totalBytes += instructionLength;// * sizeof(uint8_t);

	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//get a pointer to the flags portion of the simple glyf table
	//we must read through the flags to count their total number
	flags = (uint8_t*)(ttf_table_data) + totalBytes;
	
	//printf("\nnumPoints =  %hu\n", numPoints);
	//system("pause");
	flagIndex = 0;
    
	//printf("\n\n numContourPoints = %u\n", numContourPoints);

	numXDeltaVectorBytes = 0;
	x_size = 1; //size in bytes (will be either 1 or 2 as we read through flags array )

	//each flag needs to be associated with one of the glyfs contour points
	//so we iterate over the number of contour points (whose data we haven't read yet)
	//while we read each flag, to get the total number of flags
	//we will need a second pass to get the point data, but we can determine the lenght of the x delta vector
	//so we can know where the offset to the y delta vector starts and only use one additional loop

	flagPtr = flags;
	//printf("\nnumContourPoints = %hu\n", numContourPoints);
	for( n = numContourPoints; n--; ) 
	{
		//uint8_t repeatFlagIndex;
		//get the flag associated with the current contour point index
		//and increment the flag index to access the flags array
		flagPtr = &(flags[flagIndex++]);

		/*	
		bits = cr_convert_to_binary((unsigned *)(flags + n), 8);
    
		printf("\nFlag %u = %u\n\n", n, flags[n]);
        for(bit = 8; bit--;)
            printf("%u", +bits[bit]);
        
		printf("\n", n);
        
        free(bits);
		*/
		
		//if this uint8_t flag's bit 0 (LSB) is set to 1
		//then the corresponding point that this flag points to is
		//on the bezier curve, otherwise it lies off the curve
		//if( flag & XNG_TTF_ON_CURVE_POINT)
		//{
			//TO DO: add/store on curve point
		//}

		//x_size = 0;
		value = ((*flagPtr & XNG_TTF_X_SHORT_VECTOR) >> 1);
		x_size = value + 2 * (1-value) * ( 1 - ((*flagPtr & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 4) );
		//x_size += (flag * XNG_TTF_X_SHORT_VECTOR) >> 1;

		/*
		//count the size of the of x-coordinate delta values to get the total size of the x delta vector
		if ( flag & XNG_TTF_X_SHORT_VECTOR ) 
		{
			x_size = 1;
			//numXDeltaVectorBytes += x_size;
		} 
		else if ( !(flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) ) 
		{
			x_size = 2;
			//numXDeltaVectorBytes += x_size;
		}
		else 
			x_size = 0 ;//continue;
		//else //x_size hasn't changed from the last value
			//x_size = x_size;

		*/
		//numXDeltaVectorBytes += x_size;

		//value = (*flagPtr & XNG_TTF_REPEAT_FLAG) >> 3;
		repeatCount = ((*flagPtr & XNG_TTF_REPEAT_FLAG) >> 3) * flags[flagIndex]; 
		flagIndex += (*flagPtr & XNG_TTF_REPEAT_FLAG) >> 3;

		n -= repeatCount;
		numXDeltaVectorBytes += x_size + x_size * repeatCount;


    }

	//printf("\nAfter read flags 1st time \n");
	numFlags = flagIndex;
	totalBytes += numFlags;//* sizeof(uint8_t);
	flagIndex = 0;

	//printf("\n\nnumFlags = %u\n", numFlags);

	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//now that we know how many flags there are we can find the offset to the x-coordinate values in the simple glyf table
	//read the x coordinates
	xcoordDeltas = (uint8_t*)(ttf_table_data) + totalBytes;
	ycoordDeltas = xcoordDeltas + numXDeltaVectorBytes; 
	//xIndex = 0;
	xcoord =  ycoord = 0;
	xCoords[0]= yCoords[0] = 0;
	//if( glyph )
	//{
		//glyph->xDeltas = xcoordDeltas;
		//glyph->yDeltas = ycoordDeltas;
		//glyph->xCoords = (int16_t*)malloc( sizeof(uint16_t) * numContourPoints );
		//glyph->yCoords = (int16_t*)malloc( sizeof(uint16_t) * numContourPoints );

	//}

	//serialize to system byte order if we haven't set the custom flag on the reserved bit
	//in our ttf memory representation
	//serializeX = !(flags[0] & XNG_TTF_X_VALUE_SERIALIZED);
	//serializeY = !(flags[0] & XNG_TTF_Y_VALUE_SERIALIZED);
	//printf("\nserialize x value = %d\n", serialize);

	contourIndex = 0;
	numCurrentContourPoints = endPtsOfContours[contourIndex] + 1; //add one since the array hold indices
	printf("\numContourPoints = %hu\n", numContourPoints);
	printf("\numCurrentContourPoints = %hu\n", numCurrentContourPoints);

	//pointIndex = 0;
	bezierFlag = 0;
	//we need to read through each flag again so we can use them
	//to serialize the x and y contour point coordinates of variable length
	for( n=numContourPoints; n--; )
	{
		
		//get the flag associated with the current contour point index
		//and increment the flag index to access the flags array
		flagPtr = &(flags[flagIndex++]); //get current flag, advance flag index
		//get the next flag's byte to get the repeat count
        //value = (*flagPtr & XNG_TTF_REPEAT_FLAG) >> 3;//flags[flagIndex++];  /get whether repeat flag is 0 or 1

		repeatCount = 1;

		if( *flagPtr & XNG_TTF_REPEAT_FLAG )
			repeatCount = 1 + flags[flagIndex++]; //get the actual repeat count in the next flag if repeat flag was 1
	
		//flagIndex += value; //advance the flag index by repeat count if the repeat flag was 1
		//serialize the delta value and add it to the running point sum
		//xcoord += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_X_SHORT_VECTOR, XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, &xcoordDeltas, &totalBytes, serialize);
		//totalBytes += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_X_SHORT_VECTOR, XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, &xcoordDeltas, &xcoord, 1, serialize);
		
		//uint32_t byteSum;
		//int16_t serializedAxisCoordDelta = 0;

		//serialize corresponding contour point x-coordinate
		if ( *flagPtr & XNG_TTF_X_SHORT_VECTOR )
		{
			
			//deltax = ((xcoordDeltas)[0]);
			//value = (*flagPtr & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 4;///255;
			//ivalue = 1 - value;
			//xCoords[0] += xcoordDeltas[0] * value - xcoordDeltas[0] * (ivalue);
			//xcoord += xcoordDeltas[0] * value - xcoordDeltas[0] * (1-value);
			//if( value !=  flag & (flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR));
			
			//if( value )
			//	printf("\nvalue 1 = %c\n", value);
			//printf("\nvalue 2 = %x\n", (flag << XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR));
			//x or y value is only one byte (uint8_t)
			/*
			if ( flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR ) 
			{
				//value is to be applied as positive delta
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					xcoord += (int16_t)((xcoordDeltas)[0]);
				//}
				//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
			} 
			else 
			{
				//value is to be applied as negative delta
				//value -= file.getUint8();
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					xcoord -= (int16_t)((xcoordDeltas)[0]);
				//}
				//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas) * -1;

			}
			*/

			//x or y value is only one byte (uint8_t)
				if ( *flagPtr & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR ) 
				{
					xCoords[0] += xcoordDeltas[0] ;//* value - xcoordDeltas[0] * (ivalue);
			
					//value is to be applied as positive delta
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
					
						//xcoord += (int16_t)((xcoordDeltas)[repeatFlagIndex] * value - (xcoordDeltas)[repeatFlagIndex] * ivalue);		
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xCoords[repeatFlagIndex-1] + (int16_t)((xcoordDeltas)[repeatFlagIndex]) ;//* value - (int16_t)((xcoordDeltas)[repeatFlagIndex] * ivalue) ;
					}
					//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
				} 
				else 
				{
					//value is to be applied as negative delta
					//value -= file.getUint8();
					xCoords[0] -= xcoordDeltas[0] ;//* value - xcoordDeltas[0] * (ivalue);
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
					
						//xcoord += (int16_t)((xcoordDeltas)[repeatFlagIndex] * value - (xcoordDeltas)[repeatFlagIndex] * ivalue);		
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xCoords[repeatFlagIndex-1] - (int16_t)((xcoordDeltas)[repeatFlagIndex]) ;//* value - (int16_t)((xcoordDeltas)[repeatFlagIndex] * ivalue) ;
					}
				}
		
				//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
				//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
				xcoordDeltas += repeatCount;//+ sizeof(uint8_t) * 1;
		
			//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
			//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
			//xcoordDeltas += 1;// xcoordDeltas + 1;//+ sizeof(uint8_t) * 1;
		
			//add to the byte sum passed as input to this function for convenience
			//if( byteSum )
			//	(*byteSum) += numRepeatFlags * sizeof(uint8_t);

			//totalBytes += 1;// numRepeatFlags;//*sizeof(uint8_t)//byteSum;


		} 
		else if ( !(*flagPtr & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) ) 
		{
			//delta = xnz_be16toh((int16_t)xcoordDeltas[0]);
			//delta =  xnz_be16toh(*(int16_t*)xcoordDeltas) ;//(serializeX ? xnz_be16toh(*(int16_t*)xcoordDeltas) : *(int16_t*)xcoordDeltas);
			//value = (flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 4;///255;
			//xcoord += delta;

			int16_t * axisCoordDelta = (int16_t*)(xcoordDeltas);
			xCoords[0] += axisCoordDelta[0];
				//if( serializeX > 0 )
				//{
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
						//xcoord +=  xnz_be16toh(axisCoordDelta[repeatFlagIndex]);//axisCoordDelta[repeatFlagIndex];
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xCoords[repeatFlagIndex-1] + axisCoordDelta[repeatFlagIndex];
					}
					//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
				/*
				}
				else
				{
					for( repeatFlagIndex = 0 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
						xcoord += axisCoordDelta[repeatFlagIndex];
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xcoord;
					}
				}
				*/
				//serializedAxisCoordDelta = *axisCoordDelta;
		
				//printf("\n two bytes\n");

				//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
				xcoordDeltas += sizeof(uint16_t) * (repeatCount);

			//if( serializeX ) 
			//{
				//int16_t * axisCoordDelta = (int16_t*)(xcoordDeltas);
				//axisCoordDelta[0] = delta;//xnz_be16toh(axisCoordDelta[0]);
			//}

			/*
			//x or y value is two bytes (int16_t) and needs to be serialized to system byte format
			int16_t * axisCoordDelta = (int16_t*)(xcoordDeltas);

			if( serializeX > 0 )
			{
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					axisCoordDelta[0] = xnz_be16toh(axisCoordDelta[0]);
					xcoord += axisCoordDelta[0];
				//}
				//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
			}
			else
			{
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					xcoord += axisCoordDelta[0];
				//}
			}
			//serializedAxisCoordDelta = *axisCoordDelta;
			*/
			//printf("\n two bytes\n");

			//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
			//xcoordDeltas += sizeof(uint16_t);
		
			//add to the byte sum passed as input to this function for convenience
			//if( byteSum )
			//	(*byteSum) += sizeof(uint16_t);

			//totalBytes += sizeof(uint16_t);//serializedAxisCoordDelta;


		}
		else
		{
			//xCoords[0] += xCoords[0];
			//memset(&(glyph->xCoords[n+1]), xcoord, sizeof(uint16_t) * repeatFlagIndex); 
			for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
			{
					//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					//xcoord += axisCoordDelta[repeatFlagIndex];
					//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
					xCoords[repeatFlagIndex] = xCoords[repeatFlagIndex-1];
			}
				
		}

		//glyph->xCoords[n] = xcoord;

		//serialize corresponding contour point x-coordinate
		if ( *flagPtr & XNG_TTF_Y_SHORT_VECTOR ) 
		{
			//value = (*flagPtr & XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) >> 5;///255;
			//ivalue = 1 - value;
			//ycoord += ycoordDeltas[0] * value - ycoordDeltas[0] * (1-value);
			//yCoords[0] += ycoordDeltas[0] * value - ycoordDeltas[0] * (ivalue);
			/*
			//x or y value is only one byte (uint8_t)
			if ( flag & XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR ) 
			{
				//value is to be applied as positive delta
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					ycoord += (int16_t)((ycoordDeltas)[0]);
				//}
				//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
			} 
			else 
			{
				//value is to be applied as negative delta
				//value -= file.getUint8();
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					ycoord -= (int16_t)((ycoordDeltas)[0]);
				//}
				//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas) * -1;

			}
			*/
		
			//x or y value is only one byte (uint8_t)
				if ( *flagPtr & XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR )
				{
					yCoords[0] += ycoordDeltas[0] ;//* value - ycoordDeltas[0] * (ivalue);

					//current_point.vector[0] = glyph_transform->a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[0]) + glyph_transform->f; 
					//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[0]) + glyph->affine_transform.f; 
				
					//value is to be applied as positive delta
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//ycoord += (int16_t)((ycoordDeltas)[repeatFlagIndex] * value - (ycoordDeltas)[repeatFlagIndex] * ivalue);		
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						yCoords[repeatFlagIndex] = yCoords[repeatFlagIndex-1] + (int16_t)((ycoordDeltas)[repeatFlagIndex]);// * value - (ycoordDeltas)[repeatFlagIndex] * ivalue);

						//current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f; 
						//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[repeatFlagIndex]) + glyph->affine_transform.f; 
					}
					//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
				} 
				else 
				{
					//value is to be applied as negative delta
					//value -= file.getUint8();
					yCoords[0] -= ycoordDeltas[0] ;

					//current_point.vector[0] = glyph_transform->a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[0]) + glyph_transform->f; 
					//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[0]) + glyph->affine_transform.f; 
				
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//ycoord += (int16_t)((ycoordDeltas)[repeatFlagIndex] * value - (ycoordDeltas)[repeatFlagIndex] * ivalue);		
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;


						yCoords[repeatFlagIndex] = yCoords[repeatFlagIndex-1] - (int16_t)((ycoordDeltas)[repeatFlagIndex]);// * value - (ycoordDeltas)[repeatFlagIndex] * ivalue);

						//current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f; 
						//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[repeatFlagIndex]) + glyph->affine_transform.f; 
					}
					//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas) * -1;

				}

				//memcpy(&(glyph->xCoords[pointIndex]), &(xCoords[0]), repeatCount * sizeof(int16_t));
				//memcpy(&(glyph->yCoords[pointIndex]), &(yCoords[0]), repeatCount * sizeof(int16_t));
				//pointIndex += repeatCount;

				//yCoords[0] = yCoords[repeatFlagIndex-1];

				//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
				//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
				ycoordDeltas += repeatCount;//+ sizeof(uint8_t) * 1;
		
							//numCurrentContourPoints -= repeatCount;

			//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
			//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
			//ycoordDeltas += 1;//ycoordDeltas + 1;//+ sizeof(uint8_t) * 1;
		
			//add to the byte sum passed as input to this function for convenience
			//if( byteSum )
			//	(*byteSum) += numRepeatFlags * sizeof(uint8_t);

			//totalBytes += 1;// numRepeatFlags;//*sizeof(uint8_t)//byteSum;
		} 
		else if ( !(*flagPtr & XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) ) 
		{
			//delta = xnz_be16toh((int16_t)xcoordDeltas[0]);
			//delta = xnz_be16toh(*(int16_t*)ycoordDeltas);//(serializeY ? xnz_be16toh(*(int16_t*)ycoordDeltas) : *(int16_t*)ycoordDeltas);
			//value = (flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 4;///255;
			//ycoord += delta;
			
			int16_t * axisCoordDelta = (int16_t*)(ycoordDeltas);
			yCoords[0] += axisCoordDelta[0];

					//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[0]) + glyph->affine_transform.f; 

				//if( serializeX > 0 )
				//{
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						yCoords[repeatFlagIndex] = yCoords[repeatFlagIndex-1] + axisCoordDelta[repeatFlagIndex];

						//current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f; 
						//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[repeatFlagIndex]) + glyph->affine_transform.f; 
					}
					//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
				/*
				}
				else
				{
					for( repeatFlagIndex = 0 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
						xcoord += axisCoordDelta[repeatFlagIndex];
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xcoord;
					}
				}
				*/
				//serializedAxisCoordDelta = *axisCoordDelta;
				//memcpy(&(glyph->xCoords[pointIndex]), &(xCoords[0]), repeatCount * sizeof(int16_t));
				//memcpy(&(glyph->yCoords[pointIndex]), &(yCoords[0]), repeatCount * sizeof(int16_t));
				//pointIndex += repeatCount;
				//printf("\n two bytes\n");
					//yCoords[0] = yCoords[repeatFlagIndex-1];

				//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
				ycoordDeltas += sizeof(uint16_t) * (repeatCount);

			//if( serializeY ) 
			//{
				//int16_t * axisCoordDelta = (int16_t*)(ycoordDeltas);
				//axisCoordDelta[0] = delta;//xnz_be16toh(axisCoordDelta[0]);
			//}
			/*
			//x or y value is two bytes (int16_t) and needs to be serialized to system byte format
			int16_t * axisCoordDelta = (int16_t*)(ycoordDeltas);

			if( serializeY > 0 )
			{
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					axisCoordDelta[0] = xnz_be16toh(axisCoordDelta[0]);
					ycoord += axisCoordDelta[0];
				//}
				//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
			}
			else
			{
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					ycoord += axisCoordDelta[0];
				//}
			}
			//serializedAxisCoordDelta = *axisCoordDelta;
			*/
			//printf("\n two bytes\n");

			//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
			//ycoordDeltas += sizeof(uint16_t);

			//add to the byte sum passed as input to this function for convenience
			//if( byteSum )
			//	(*byteSum) += sizeof(uint16_t);

			//totalBytes += sizeof(uint16_t);//serializedAxisCoordDelta;

							//numCurrentContourPoints -= repeatCount;

		}
		else
		{


			//numCurrentContourPoints -= repeatCount;

			//yCoords[0] = yCoords[repeatFlagIndex-1];
			for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
			{
					//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					//xcoord += axisCoordDelta[repeatFlagIndex];
					//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
					yCoords[repeatFlagIndex] = yCoords[repeatFlagIndex-1];
			}
				
		}


		//draw the points we just collected to our temporary buffers on the stack
		//into the floatin point accumulation buffer
		for(repeatFlagIndex = 0; repeatFlagIndex < repeatCount; repeatFlagIndex++ )
		{
			current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
			current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f; 

			//draw bezier control point or line
			xng_ttf_glyf_draw_contour_point_to_buffer(glyph, *flagPtr, &bezierFlag, &current_point, &last_point, &first_on_curve_point, &first_off_curve_point, &last_off_curve_point); 
			
			//decrement the numCurrentContourPoints var to indicate how many points in the current contour we've iterated
			numCurrentContourPoints--;

			//check if the end of the current contour we are iterating has been reached
			if( numCurrentContourPoints == 0 )
			{
				//draw the connection between the last point of the contour to its first point
				xng_ttf_glyf_draw_closing_contour_point_to_buffer(glyph, *flagPtr, &bezierFlag, &current_point, &last_point, &first_on_curve_point, &first_off_curve_point, &last_off_curve_point); 
						
				//check if we have iterated the last contour
				++contourIndex;
				if( !(contourIndex < numContours  ) )
				{
					break;
				}

				//if we haven't iterated the last contour, get the amount of points defining the next contour
				numCurrentContourPoints = endPtsOfContours[contourIndex] - endPtsOfContours[contourIndex-1] ;//+ 1;
				//printf("\nnumCurrentContourPoints = %hd\n", numCurrentContourPoints); 
			}
		}
		//printf("\repeatCount = %u\n", (uint32_t)repeatCount);

		//printf("\numCurrentContourPoints = %hu\n", numCurrentContourPoints);

			
		assert( !(numCurrentContourPoints < 0) );
		
		//set the last coordinate to the beginning of our temporary stack buffer
		//so we can continue the glyph delta point sums on the next loop iteration
		xCoords[0] = xCoords[repeatFlagIndex-1];
		yCoords[0] = yCoords[repeatFlagIndex-1];

		//yCoords[0] = yCoords[repeatFlagIndex-1];

		//printf("\n repeatCount =  %u\n", repeatCount);
		//skip over the contour point indices in the for loop
		//for which the current flag repeats
        n -= repeatCount-1;

		
        

	}

	//assert(numCurrentContourPoints == 0 );
	//set current_point
			//current_point.x = xcoord;
			//current_point.y = ycoord;

			//draw bezier control point or line
			//if( numCurrentContourPoints > 0 )
			//	xng_ttf_glyf_draw_point_to_buffer(glyph, flag, &bezierFlag, &current_point, &last_point, &first_on_curve_point, &first_off_curve_point, &last_off_curve_point, --numCurrentContourPoints == 0); 

			//printf("\n numCurrentContourPoints =  %u\n", numCurrentContourPoints);
	//printf("\n\ntotalBytes after xcoords = %u\n", totalBytes);
	
	//printf("\n\ntotalBytes after ycoords = %u\n", totalBytes);

	//set the custom flags on the reserved bit of the first flag byte to 
	//indicate that we've arleady serialized to system byte order
	//flags[0] |= XNG_TTF_X_VALUE_SERIALIZED;
	//flags[0] |= XNG_TTF_Y_VALUE_SERIALIZED;

	/*
	//if( glyph )
	//{
		//glyph->numContours = numContours;
		glyph->endPtsOfContours = endPtsOfContours;

		glyph->numContourPoints = numContourPoints;

		glyph->numInstructions = instructionLength;
		glyph->instructions = instructions;

		glyph->numFlags = numFlags;
		glyph->flags = flags;

		//glyph->xDeltas;
		//glyph->yDeltas;
		
	//}
	*/
	return totalBytes;
} 


//this function assumes values up to simple glyf table flags property has already been serialized into our memory representation
static uint32_t xng_ttf_simple_glyf_table_serialize_and_draw_contour_data(void * ttf_table_data, xng_ttf_glyph * glyph, xnz_float_affine_transform * glyph_transform)
{

	int16_t delta;//, value;
	unsigned char value, ivalue;
	//cr_float4 controlPoint;
	int16_t xCoords[256], yCoords[256]; //arrays for holding x and y coordinates for drawing to accumulating buffer when reading repeating flags
	xnz_float2 first_on_curve_point,  first_off_curve_point, last_off_curve_point, current_point, last_point;
	uint8_t serializeX, serializeY;  //flag to indicate whether x and y delta values have already been serialized in our memory representation
	uint32_t totalBytes;
	uint32_t n;//, ui;
	uint16_t numContourPoints;
	uint16_t instructionLength;
	uint8_t bezierFlag;// = 0;

	uint16_t x_size;// y_size;  //vars to hold size of x and y delta vector elements as we read through array of uint8_t flags
	//uint16_t xIndex, yIndex, flagIndex;
	uint16_t numXDeltaVectorBytes, numFlags, flagIndex;  //vars to hold counts of num elements in flags array and x and y delta vectors as we read through array of uint8_t flags
	uint8_t flag, repeatCount, repeatFlagIndex;
	uint8_t *flagPtr;
	//store contour x and y coordinates as sums of deltas
	//as we read through the coordinate buffers
	int16_t xcoord, ycoord;
	int16_t numContours;
	//uint32_t pointIndex;
	
	//unsigned * bits;
	//unsigned int bit;
	uint8_t *flags, *instructions, *xcoordDeltas, *ycoordDeltas;
	int16_t contourIndex;
	int16_t numCurrentContourPoints;

	uint16_t * endPtsOfContours = (uint16_t*)ttf_table_data;

	//we need to store data in a glyph
	assert(glyph);

	//keep a count of the total bytes we've read in this function
	totalBytes = 0;

	//serialize endPtsOfContours array of indices to pt arrays
	//we add 1 to the numContours to serialize the following instructionLength var
	//printf("\n\n numContours = %hu\n", numContours);

	numContours = glyph->header->numberOfContours;
	numContourPoints = 0;
	//printf("\n numContours = %hu\n", numContours);

	/*
	for( contourIndex = 0; contourIndex < numContours; contourIndex++ )
	{
		//this function assumes values up to simple glyf table flags property has already been serialized
		endPtsOfContours[n] = xnz_be16toh(endPtsOfContours[n]);
		//printf("\n numContourPoints[%hd] = %hu\n", contourIndex, endPtsOfContours[contourIndex]);
		//get the maximum contour point index as we read through the contour end point indices
		//to learn how many contour points there are in total
		//if( endPtsOfContours[contourIndex] > numContourPoints )
		//{
		//	numContourPoints = endPtsOfContours[contourIndex];
		//}
	}
	*/

	//numContourPoints is an index value, so we must add we to get total number  
	numContourPoints = xnz_be16toh(endPtsOfContours[numContours - 1]) + 1;
	totalBytes += sizeof(uint16_t) * numContours;

	//this function assumes values up to simple glyf table flags property has already been serialized
	//endPtsOfContours[numContours] = xnz_be16toh(endPtsOfContours[numContours]);
	
	//serialize instructionLength
	instructionLength = xnz_be16toh(endPtsOfContours[numContours]);
	totalBytes += sizeof(uint16_t);

	instructions = (uint8_t*)(ttf_table_data) + totalBytes;	
	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//push past the instructions
	//for( n=0; n<instructionLength; n++ )
	//{
	//
	//}

	//printf("\n\ninstruction length = %hu\n", instructionLength);
	totalBytes += instructionLength;// * sizeof(uint8_t);

	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//get a pointer to the flags portion of the simple glyf table
	//we must read through the flags to count their total number
	flags = (uint8_t*)(ttf_table_data) + totalBytes;
	
	//printf("\nnumPoints =  %hu\n", numPoints);
	//system("pause");
	flagIndex = 0;
    
	//printf("\n\n numContourPoints = %u\n", numContourPoints);

	numXDeltaVectorBytes = 0;
	x_size = 1; //size in bytes (will be either 1 or 2 as we read through flags array )

	//each flag needs to be associated with one of the glyfs contour points
	//so we iterate over the number of contour points (whose data we haven't read yet)
	//while we read each flag, to get the total number of flags
	//we will need a second pass to get the point data, but we can determine the lenght of the x delta vector
	//so we can know where the offset to the y delta vector starts and only use one additional loop

	flagPtr = flags;
	//printf("\nnumContourPoints = %hu\n", numContourPoints);
	for( n = numContourPoints; n--; ) 
	{
		//uint8_t repeatFlagIndex;
		//get the flag associated with the current contour point index
		//and increment the flag index to access the flags array
		flagPtr = &(flags[flagIndex++]);

		/*	
		bits = cr_convert_to_binary((unsigned *)(flags + n), 8);
    
		printf("\nFlag %u = %u\n\n", n, flags[n]);
        for(bit = 8; bit--;)
            printf("%u", +bits[bit]);
        
		printf("\n", n);
        
        free(bits);
		*/
		
		//if this uint8_t flag's bit 0 (LSB) is set to 1
		//then the corresponding point that this flag points to is
		//on the bezier curve, otherwise it lies off the curve
		//if( flag & XNG_TTF_ON_CURVE_POINT)
		//{
			//TO DO: add/store on curve point
		//}

		//x_size = 0;
		value = ((*flagPtr & XNG_TTF_X_SHORT_VECTOR) >> 1);
		x_size = value + 2 * (1-value) * ( 1 - ((*flagPtr & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 4) );
		//x_size += (flag * XNG_TTF_X_SHORT_VECTOR) >> 1;

		/*
		//count the size of the of x-coordinate delta values to get the total size of the x delta vector
		if ( flag & XNG_TTF_X_SHORT_VECTOR ) 
		{
			x_size = 1;
			//numXDeltaVectorBytes += x_size;
		} 
		else if ( !(flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) ) 
		{
			x_size = 2;
			//numXDeltaVectorBytes += x_size;
		}
		else 
			x_size = 0 ;//continue;
		//else //x_size hasn't changed from the last value
			//x_size = x_size;

		*/
		//numXDeltaVectorBytes += x_size;

		//value = (*flagPtr & XNG_TTF_REPEAT_FLAG) >> 3;
		repeatCount = ((*flagPtr & XNG_TTF_REPEAT_FLAG) >> 3) * flags[flagIndex]; 
		flagIndex += (*flagPtr & XNG_TTF_REPEAT_FLAG) >> 3;

		n -= repeatCount;
		numXDeltaVectorBytes += x_size + x_size * repeatCount;


    }

	//printf("\nAfter read flags 1st time \n");
	numFlags = flagIndex;
	totalBytes += numFlags;//* sizeof(uint8_t);
	flagIndex = 0;

	//printf("\n\nnumFlags = %u\n", numFlags);

	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//now that we know how many flags there are we can find the offset to the x-coordinate values in the simple glyf table
	//read the x coordinates
	xcoordDeltas = (uint8_t*)(ttf_table_data) + totalBytes;
	ycoordDeltas = xcoordDeltas + numXDeltaVectorBytes; 
	//xIndex = 0;
	xcoord =  ycoord = 0;
	xCoords[0]= yCoords[0] = 0;
	//if( glyph )
	//{
		//glyph->xDeltas = xcoordDeltas;
		//glyph->yDeltas = ycoordDeltas;
		//glyph->xCoords = (int16_t*)malloc( sizeof(uint16_t) * numContourPoints );
		//glyph->yCoords = (int16_t*)malloc( sizeof(uint16_t) * numContourPoints );

	//}

	//serialize to system byte order if we haven't set the custom flag on the reserved bit
	//in our ttf memory representation
	//serializeX = !(flags[0] & XNG_TTF_X_VALUE_SERIALIZED);
	//serializeY = !(flags[0] & XNG_TTF_Y_VALUE_SERIALIZED);
	//printf("\nserialize x value = %d\n", serialize);

	contourIndex = 0;
	numCurrentContourPoints = xnz_be16toh(endPtsOfContours[contourIndex]) + 1; //add one since the array hold indices
	printf("\numContourPoints = %hu\n", numContourPoints);
	printf("\numCurrentContourPoints = %hu\n", numCurrentContourPoints);

	//pointIndex = 0;
	bezierFlag = 0;
	//we need to read through each flag again so we can use them
	//to serialize the x and y contour point coordinates of variable length
	for( n=numContourPoints; n--; )
	{
		
		//get the flag associated with the current contour point index
		//and increment the flag index to access the flags array
		flagPtr = &(flags[flagIndex++]); //get current flag, advance flag index
		//get the next flag's byte to get the repeat count
        //value = (*flagPtr & XNG_TTF_REPEAT_FLAG) >> 3;//flags[flagIndex++];  /get whether repeat flag is 0 or 1

		repeatCount = 1;

		if( *flagPtr & XNG_TTF_REPEAT_FLAG )
			repeatCount = 1 + flags[flagIndex++]; //get the actual repeat count in the next flag if repeat flag was 1
	
		//flagIndex += value; //advance the flag index by repeat count if the repeat flag was 1
		//serialize the delta value and add it to the running point sum
		//xcoord += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_X_SHORT_VECTOR, XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, &xcoordDeltas, &totalBytes, serialize);
		//totalBytes += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_X_SHORT_VECTOR, XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, &xcoordDeltas, &xcoord, 1, serialize);
		
		//uint32_t byteSum;
		//int16_t serializedAxisCoordDelta = 0;

		//serialize corresponding contour point x-coordinate
		if ( *flagPtr & XNG_TTF_X_SHORT_VECTOR )
		{
			
			//deltax = ((xcoordDeltas)[0]);
			//value = (*flagPtr & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 4;///255;
			//ivalue = 1 - value;
			//xCoords[0] += xcoordDeltas[0] * value - xcoordDeltas[0] * (ivalue);
			//xcoord += xcoordDeltas[0] * value - xcoordDeltas[0] * (1-value);
			//if( value !=  flag & (flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR));
			
			//if( value )
			//	printf("\nvalue 1 = %c\n", value);
			//printf("\nvalue 2 = %x\n", (flag << XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR));
			//x or y value is only one byte (uint8_t)
			/*
			if ( flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR ) 
			{
				//value is to be applied as positive delta
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					xcoord += (int16_t)((xcoordDeltas)[0]);
				//}
				//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
			} 
			else 
			{
				//value is to be applied as negative delta
				//value -= file.getUint8();
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					xcoord -= (int16_t)((xcoordDeltas)[0]);
				//}
				//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas) * -1;

			}
			*/

			//x or y value is only one byte (uint8_t)
				if ( *flagPtr & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR ) 
				{
					xCoords[0] += xcoordDeltas[0] ;//* value - xcoordDeltas[0] * (ivalue);
			
					//value is to be applied as positive delta
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
					
						//xcoord += (int16_t)((xcoordDeltas)[repeatFlagIndex] * value - (xcoordDeltas)[repeatFlagIndex] * ivalue);		
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xCoords[repeatFlagIndex-1] + (int16_t)((xcoordDeltas)[repeatFlagIndex]) ;//* value - (int16_t)((xcoordDeltas)[repeatFlagIndex] * ivalue) ;
					}
					//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
				} 
				else 
				{
					//value is to be applied as negative delta
					//value -= file.getUint8();
					xCoords[0] -= xcoordDeltas[0] ;//* value - xcoordDeltas[0] * (ivalue);
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
					
						//xcoord += (int16_t)((xcoordDeltas)[repeatFlagIndex] * value - (xcoordDeltas)[repeatFlagIndex] * ivalue);		
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xCoords[repeatFlagIndex-1] - (int16_t)((xcoordDeltas)[repeatFlagIndex]) ;//* value - (int16_t)((xcoordDeltas)[repeatFlagIndex] * ivalue) ;
					}
				}
		
				//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
				//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
				xcoordDeltas += repeatCount;//+ sizeof(uint8_t) * 1;
		
			//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
			//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
			//xcoordDeltas += 1;// xcoordDeltas + 1;//+ sizeof(uint8_t) * 1;
		
			//add to the byte sum passed as input to this function for convenience
			//if( byteSum )
			//	(*byteSum) += numRepeatFlags * sizeof(uint8_t);

			//totalBytes += 1;// numRepeatFlags;//*sizeof(uint8_t)//byteSum;


		} 
		else if ( !(*flagPtr & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) ) 
		{
			//delta = xnz_be16toh((int16_t)xcoordDeltas[0]);
			//delta =  xnz_be16toh(*(int16_t*)xcoordDeltas) ;//(serializeX ? xnz_be16toh(*(int16_t*)xcoordDeltas) : *(int16_t*)xcoordDeltas);
			//value = (flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 4;///255;
			//xcoord += delta;

			int16_t * axisCoordDelta = (int16_t*)(xcoordDeltas);
			xCoords[0] += xnz_be16toh(axisCoordDelta[0]);
				//if( serializeX > 0 )
				//{
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
						//xcoord +=  xnz_be16toh(axisCoordDelta[repeatFlagIndex]);//axisCoordDelta[repeatFlagIndex];
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xCoords[repeatFlagIndex-1] + xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					}
					//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
				/*
				}
				else
				{
					for( repeatFlagIndex = 0 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
						xcoord += axisCoordDelta[repeatFlagIndex];
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xcoord;
					}
				}
				*/
				//serializedAxisCoordDelta = *axisCoordDelta;
		
				//printf("\n two bytes\n");

				//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
				xcoordDeltas += sizeof(uint16_t) * (repeatCount);

			//if( serializeX ) 
			//{
				//int16_t * axisCoordDelta = (int16_t*)(xcoordDeltas);
				//axisCoordDelta[0] = delta;//xnz_be16toh(axisCoordDelta[0]);
			//}

			/*
			//x or y value is two bytes (int16_t) and needs to be serialized to system byte format
			int16_t * axisCoordDelta = (int16_t*)(xcoordDeltas);

			if( serializeX > 0 )
			{
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					axisCoordDelta[0] = xnz_be16toh(axisCoordDelta[0]);
					xcoord += axisCoordDelta[0];
				//}
				//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
			}
			else
			{
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					xcoord += axisCoordDelta[0];
				//}
			}
			//serializedAxisCoordDelta = *axisCoordDelta;
			*/
			//printf("\n two bytes\n");

			//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
			//xcoordDeltas += sizeof(uint16_t);
		
			//add to the byte sum passed as input to this function for convenience
			//if( byteSum )
			//	(*byteSum) += sizeof(uint16_t);

			//totalBytes += sizeof(uint16_t);//serializedAxisCoordDelta;


		}
		else
		{
			//xCoords[0] += xCoords[0];
			//memset(&(glyph->xCoords[n+1]), xcoord, sizeof(uint16_t) * repeatFlagIndex); 
			for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
			{
					//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					//xcoord += axisCoordDelta[repeatFlagIndex];
					//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
					xCoords[repeatFlagIndex] = xCoords[repeatFlagIndex-1];
			}
				
		}

		//glyph->xCoords[n] = xcoord;

		//serialize corresponding contour point x-coordinate
		if ( *flagPtr & XNG_TTF_Y_SHORT_VECTOR ) 
		{
			//value = (*flagPtr & XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) >> 5;///255;
			//ivalue = 1 - value;
			//ycoord += ycoordDeltas[0] * value - ycoordDeltas[0] * (1-value);
			//yCoords[0] += ycoordDeltas[0] * value - ycoordDeltas[0] * (ivalue);
			/*
			//x or y value is only one byte (uint8_t)
			if ( flag & XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR ) 
			{
				//value is to be applied as positive delta
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					ycoord += (int16_t)((ycoordDeltas)[0]);
				//}
				//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
			} 
			else 
			{
				//value is to be applied as negative delta
				//value -= file.getUint8();
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					ycoord -= (int16_t)((ycoordDeltas)[0]);
				//}
				//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas) * -1;

			}
			*/
		
			//x or y value is only one byte (uint8_t)
				if ( *flagPtr & XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR )
				{
					yCoords[0] += ycoordDeltas[0] ;//* value - ycoordDeltas[0] * (ivalue);

					//current_point.vector[0] = glyph_transform->a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[0]) + glyph_transform->f; 
					//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[0]) + glyph->affine_transform.f; 
				
					//value is to be applied as positive delta
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//ycoord += (int16_t)((ycoordDeltas)[repeatFlagIndex] * value - (ycoordDeltas)[repeatFlagIndex] * ivalue);		
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						yCoords[repeatFlagIndex] = yCoords[repeatFlagIndex-1] + (int16_t)((ycoordDeltas)[repeatFlagIndex]);// * value - (ycoordDeltas)[repeatFlagIndex] * ivalue);

						//current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f; 
						//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[repeatFlagIndex]) + glyph->affine_transform.f; 
					}
					//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas);
				} 
				else 
				{
					//value is to be applied as negative delta
					//value -= file.getUint8();
					yCoords[0] -= ycoordDeltas[0] ;

					//current_point.vector[0] = glyph_transform->a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[0]) + glyph_transform->f; 
					//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[0]) + glyph->affine_transform.f; 
				
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//ycoord += (int16_t)((ycoordDeltas)[repeatFlagIndex] * value - (ycoordDeltas)[repeatFlagIndex] * ivalue);		
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;


						yCoords[repeatFlagIndex] = yCoords[repeatFlagIndex-1] - (int16_t)((ycoordDeltas)[repeatFlagIndex]);// * value - (ycoordDeltas)[repeatFlagIndex] * ivalue);

						//current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f; 
						//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[repeatFlagIndex]) + glyph->affine_transform.f; 
					}
					//serializedAxisCoordDelta = (int16_t)(**axisCoordinateDeltas) * -1;

				}

				//memcpy(&(glyph->xCoords[pointIndex]), &(xCoords[0]), repeatCount * sizeof(int16_t));
				//memcpy(&(glyph->yCoords[pointIndex]), &(yCoords[0]), repeatCount * sizeof(int16_t));
				//pointIndex += repeatCount;

				//yCoords[0] = yCoords[repeatFlagIndex-1];

				//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
				//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
				ycoordDeltas += repeatCount;//+ sizeof(uint8_t) * 1;
		
							//numCurrentContourPoints -= repeatCount;

			//byteSum = /*sizeof(uint8_t) */ numRepeatFlags;
			//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
			//ycoordDeltas += 1;//ycoordDeltas + 1;//+ sizeof(uint8_t) * 1;
		
			//add to the byte sum passed as input to this function for convenience
			//if( byteSum )
			//	(*byteSum) += numRepeatFlags * sizeof(uint8_t);

			//totalBytes += 1;// numRepeatFlags;//*sizeof(uint8_t)//byteSum;
		} 
		else if ( !(*flagPtr & XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) ) 
		{
			//delta = xnz_be16toh((int16_t)xcoordDeltas[0]);
			//delta = xnz_be16toh(*(int16_t*)ycoordDeltas);//(serializeY ? xnz_be16toh(*(int16_t*)ycoordDeltas) : *(int16_t*)ycoordDeltas);
			//value = (flag & XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 4;///255;
			//ycoord += delta;
			
			int16_t * axisCoordDelta = (int16_t*)(ycoordDeltas);
			yCoords[0] += xnz_be16toh(axisCoordDelta[0]);

					//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[0]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
					//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[0]) + glyph->affine_transform.f; 

				//if( serializeX > 0 )
				//{
					for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						yCoords[repeatFlagIndex] = yCoords[repeatFlagIndex-1] + xnz_be16toh(axisCoordDelta[repeatFlagIndex]);

						//current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f; 
						//current_point.vector[0] = glyph->affine_transform.a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph->affine_transform.e;
						//current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph->affine_transform.d * (float)(yCoords[repeatFlagIndex]) + glyph->affine_transform.f; 
					}
					//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
				/*
				}
				else
				{
					for( repeatFlagIndex = 0 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
					{
						//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
						xcoord += axisCoordDelta[repeatFlagIndex];
						//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
						xCoords[repeatFlagIndex] = xcoord;
					}
				}
				*/
				//serializedAxisCoordDelta = *axisCoordDelta;
				//memcpy(&(glyph->xCoords[pointIndex]), &(xCoords[0]), repeatCount * sizeof(int16_t));
				//memcpy(&(glyph->yCoords[pointIndex]), &(yCoords[0]), repeatCount * sizeof(int16_t));
				//pointIndex += repeatCount;
				//printf("\n two bytes\n");
					//yCoords[0] = yCoords[repeatFlagIndex-1];

				//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
				ycoordDeltas += sizeof(uint16_t) * (repeatCount);

			//if( serializeY ) 
			//{
				//int16_t * axisCoordDelta = (int16_t*)(ycoordDeltas);
				//axisCoordDelta[0] = delta;//xnz_be16toh(axisCoordDelta[0]);
			//}
			/*
			//x or y value is two bytes (int16_t) and needs to be serialized to system byte format
			int16_t * axisCoordDelta = (int16_t*)(ycoordDeltas);

			if( serializeY > 0 )
			{
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					axisCoordDelta[0] = xnz_be16toh(axisCoordDelta[0]);
					ycoord += axisCoordDelta[0];
				//}
				//*axisCoordDelta = xnz_be16toh(*axisCoordDelta);
			}
			else
			{
				//for( repeatFlagIndex = 0 ; repeatFlagIndex<1; repeatFlagIndex++ )
				//{
					//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					ycoord += axisCoordDelta[0];
				//}
			}
			//serializedAxisCoordDelta = *axisCoordDelta;
			*/
			//printf("\n two bytes\n");

			//advance the axis coordinate deltas buffer pointer passed as input to this function for convenience
			//ycoordDeltas += sizeof(uint16_t);

			//add to the byte sum passed as input to this function for convenience
			//if( byteSum )
			//	(*byteSum) += sizeof(uint16_t);

			//totalBytes += sizeof(uint16_t);//serializedAxisCoordDelta;

							//numCurrentContourPoints -= repeatCount;

		}
		else
		{


			//numCurrentContourPoints -= repeatCount;

			//yCoords[0] = yCoords[repeatFlagIndex-1];
			for( repeatFlagIndex = 1 ; repeatFlagIndex<repeatCount; repeatFlagIndex++ )
			{
					//axisCoordDelta[repeatFlagIndex] = xnz_be16toh(axisCoordDelta[repeatFlagIndex]);
					//xcoord += axisCoordDelta[repeatFlagIndex];
					//glyph->xCoords[n + repeatFlagIndex + 1] = xcoord;
					yCoords[repeatFlagIndex] = yCoords[repeatFlagIndex-1];
			}
				
		}


		//draw the points we just collected to our temporary buffers on the stack
		//into the floatin point accumulation buffer
		for(repeatFlagIndex = 0; repeatFlagIndex < repeatCount; repeatFlagIndex++ )
		{
			current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) /*+ glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex])*/ + glyph_transform->e;
			current_point.vector[1] = /*glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) +*/ glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f; 

			//draw bezier control point or line
			xng_ttf_glyf_draw_contour_point_to_buffer(glyph, *flagPtr, &bezierFlag, &current_point, &last_point, &first_on_curve_point, &first_off_curve_point, &last_off_curve_point); 
			
			//decrement the numCurrentContourPoints var to indicate how many points in the current contour we've iterated
			numCurrentContourPoints--;

			//check if the end of the current contour we are iterating has been reached
			if( numCurrentContourPoints == 0 )
			{
				//draw the connection between the last point of the contour to its first point
				xng_ttf_glyf_draw_closing_contour_point_to_buffer(glyph, *flagPtr, &bezierFlag, &current_point, &last_point, &first_on_curve_point, &first_off_curve_point, &last_off_curve_point); 
						
				//check if we have iterated the last contour
				++contourIndex;
				if( !(contourIndex < numContours  ) )
				{
					break;
				}

				//if we haven't iterated the last contour, get the amount of points defining the next contour
				numCurrentContourPoints = xnz_be16toh(endPtsOfContours[contourIndex]) - xnz_be16toh(endPtsOfContours[contourIndex-1]) ;//+ 1;
				//printf("\nnumCurrentContourPoints = %hd\n", numCurrentContourPoints); 
			}
		}
		//printf("\repeatCount = %u\n", (uint32_t)repeatCount);

		//printf("\numCurrentContourPoints = %hu\n", numCurrentContourPoints);

			
		assert( !(numCurrentContourPoints < 0) );
		
		//set the last coordinate to the beginning of our temporary stack buffer
		//so we can continue the glyph delta point sums on the next loop iteration
		xCoords[0] = xCoords[repeatFlagIndex-1];
		yCoords[0] = yCoords[repeatFlagIndex-1];

		//yCoords[0] = yCoords[repeatFlagIndex-1];

		//printf("\n repeatCount =  %u\n", repeatCount);
		//skip over the contour point indices in the for loop
		//for which the current flag repeats
        n -= repeatCount-1;

		
        

	}

	//assert(numCurrentContourPoints == 0 );
	//set current_point
			//current_point.x = xcoord;
			//current_point.y = ycoord;

			//draw bezier control point or line
			//if( numCurrentContourPoints > 0 )
			//	xng_ttf_glyf_draw_point_to_buffer(glyph, flag, &bezierFlag, &current_point, &last_point, &first_on_curve_point, &first_off_curve_point, &last_off_curve_point, --numCurrentContourPoints == 0); 

			//printf("\n numCurrentContourPoints =  %u\n", numCurrentContourPoints);
	//printf("\n\ntotalBytes after xcoords = %u\n", totalBytes);
	
	//printf("\n\ntotalBytes after ycoords = %u\n", totalBytes);

	//set the custom flags on the reserved bit of the first flag byte to 
	//indicate that we've arleady serialized to system byte order
	//flags[0] |= XNG_TTF_X_VALUE_SERIALIZED;
	//flags[0] |= XNG_TTF_Y_VALUE_SERIALIZED;

	/*
	//if( glyph )
	//{
		//glyph->numContours = numContours;
		glyph->endPtsOfContours = endPtsOfContours;

		glyph->numContourPoints = numContourPoints;

		glyph->numInstructions = instructionLength;
		glyph->instructions = instructions;

		glyph->numFlags = numFlags;
		glyph->flags = flags;

		//glyph->xDeltas;
		//glyph->yDeltas;
		
	//}
	*/
	return totalBytes;
} 

static uint32_t xng_ttf_simple_glyf_table_serialize(void * ttf_table_data, int16_t numContours, xng_ttf_glyph * glyph)
{
	uint8_t serialize;
	uint32_t totalBytes;
	uint32_t n;//, ui;
	uint32_t flagIndex, numFlags, xIndex, yIndex;
	uint32_t numContourPoints;
	uint16_t instructionLength;

	//store contour x and y coordinates as sums of deltas
	//as we read through the coordinate buffers
	int16_t xcoord, ycoord;
	
	unsigned * bits;
	unsigned int bit;
	uint8_t *flags, *instructions, *xcoordDeltas, *ycoordDeltas;
	uint16_t * endPtsOfContours = (uint16_t*)ttf_table_data;

	//keep a count of the total bytes we've read in this function
	totalBytes = 0;

	//serialize endPtsOfContours array of indices to pt arrays
	//we add 1 to the numContours to serialize the following instructionLength var
	//printf("\n\n numContours = %hu\n", numContours);

	numContourPoints = 0;
	for( n = 0; n < numContours; n++ )
	{
		//convert contour end points to system byte format
		endPtsOfContours[n] = xnz_be16toh(endPtsOfContours[n]);

		//get the maximum contour point index as we read through the contour end point indices
		//to learn how many contour points there are in total
		if( endPtsOfContours[n] > numContourPoints )
		{
			numContourPoints = endPtsOfContours[n];
		}
	}

	//numContourPoints is an index value, so we must add we to get total number  
	numContourPoints += 1;

	totalBytes += sizeof(uint16_t) * numContours;

	endPtsOfContours[numContours] = xnz_be16toh(endPtsOfContours[numContours]);
	//serialize instructionLength
	instructionLength = endPtsOfContours[numContours];
	totalBytes += sizeof(uint16_t);

	instructions = (uint8_t*)(ttf_table_data) + totalBytes;
	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//push past the instructions
	//for( n=0; n<instructionLength; n++ )
	//{
	//
	//}

	//printf("\n\ninstruction length = %hu\n", instructionLength);
	totalBytes += instructionLength;// * sizeof(uint8_t);

	//	printf("\n\ntotalBytes = %u\n", totalBytes);

	//get a pointer to the flags portion of the simple glyf table
	//we must read through the flags to count their total number
	flags = (uint8_t*)(ttf_table_data) + totalBytes;
	
	//printf("\nnumPoints =  %hu\n", numPoints);
	//system("pause");
	flagIndex = 0;
    
	//printf("\n\n numContourPoints = %u\n", numContourPoints); 

	//each flag needs to be associated with one of the glyfs contour points
	//so we iterate over the number of contour points (whose data we haven't read yet)
	//while we read each flag, to get the total number of flags
	for( n = 0; n < numContourPoints; n++ ) 
	{
		//get the flag associated with the current contour point index
		//and increment the flag index to access the flags array
		uint8_t flag = flags[flagIndex++];

		/*	
		bits = cr_convert_to_binary((unsigned *)(flags + n), 8);
    
		printf("\nFlag %u = %u\n\n", n, flags[n]);
        for(bit = 8; bit--;)
            printf("%u", +bits[bit]);
        
		printf("\n", n);
        
        free(bits);
		*/
		
        
		//if this uint8_t flag's bit 0 (LSB) is set to 1
		//then the corresponding point that this flag points to is
		//on the bezier curve, otherwise it lies off the curve
		if( flag & XNG_TTF_ON_CURVE_POINT)
		{
			//TO DO: add/store on curve point	
		}

		//if this uint8_t flag's repeat bit (bit 3) is set to 1
		//the next byte should be read as unsigned (ie uint8_t)
		//and specifies the number of times the current flag should be repeated
		//which i guess is only a performance optimization if the number
		//of consecutive flags is greater than 2
        if ( flag & XNG_TTF_REPEAT_FLAG ) 
		{
			//get the next flag's byte to get the repeat count
            uint8_t repeatCount = flags[flagIndex++];

			/*
			printf("\nRepeat Flag %u = %u\n\n", n+1, flags[n+1]);
			bits = cr_convert_to_binary((unsigned *)(flags + n+1), 8);
    
	        for(bit = 8; bit--;)
				printf("%u", +bits[bit]);
                
	        free(bits);
			*/
			
			//assert repeat count is valid
            assert(repeatCount > 0);

			//skip over the contour point indices in the for loop
			//for which the current flag repeats
            n += repeatCount;

			/*
            while( repeatCount > 0 ) 
			{
           		//TO DO: add/store on curve point
				repeatCount--;
			}
			*/

        }
    }

	//printf("\nAfter read flags 1st time \n");


	numFlags = flagIndex;
	totalBytes += numFlags * sizeof(uint8_t);
	flagIndex = 0;

	//printf("\n\nnumFlags = %u\n", numFlags);

	//printf("\n\ntotalBytes = %u\n", totalBytes);

	//now that we know how many flags there are we can find the offset to the x-coordinate values in the simple glyf table
	//read the x coordinates
	xcoordDeltas = (uint8_t*)(ttf_table_data) + totalBytes;
	xIndex = 0;
	xcoord = 0;

	if( glyph )
	{
		glyph->xDeltas = xcoordDeltas;
		glyph->xCoords = (int16_t*)malloc( sizeof(uint16_t) * numContourPoints );
	}

	//serialize to system byte order if we haven't set the custom flag on the reserved bit
	//in our ttf memory representation
	serialize = !(flags[0] & XNG_TTF_X_VALUE_SERIALIZED);
	//printf("\nserialize x value = %d\n", serialize);

	//we need to read through each flag again so we can use them
	//to serialize the x and y contour point coordinates of variable length
	for( n=0; n<numContourPoints; n++ )
	{
		//get the flag associated with the current contour point index
		//and increment the flag index to access the flags array
		uint8_t flag = flags[flagIndex++];
        
		//if this uint8_t flag's bit 0 (LSB) is set to 1
		//then the corresponding point that this flag points to is
		//on the bezier curve, otherwise it lies off the curve
		if( flag & XNG_TTF_ON_CURVE_POINT)
		{
			//TO DO: add/store on curve point	
		}

		/*
		bits = cr_convert_to_binary((unsigned *)(flags + n), 8);

		printf("\n", n);

		//printf("\nFlag %u = %u\n\n", n, flags[n]);
        for(bit = 8; bit--;)
            printf("%u", +bits[bit]);
        
		printf("\n", n);
        
        free(bits);
		*/

		//serialize the delta value and add it to the running point sum
		//xcoord += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_X_SHORT_VECTOR, XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, &xcoordDeltas, &totalBytes, serialize);
		totalBytes += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_X_SHORT_VECTOR, XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, &xcoordDeltas, &xcoord, 1, serialize);
		if( glyph )
		{
			glyph->xCoords[n] = xcoord;

			if( xcoord < glyph->header->xMin )
				glyph->header->xMin = xcoord;
			else if( xcoord  > glyph->header->xMax )
				glyph->header->xMax = xcoord;
		}
		//TO DO: store the current contour point x-value
        //points[i][name] = value;

		//if this uint8_t flag's repeat bit (bit 3) is set to 1
		//the next byte should be read as unsigned (ie uint8_t)
		//and specifies the number of times the current flag should be repeated
		//which i guess is only a performance optimization if the number
		//of consecutive flags is greater than 2
        if ( flag & XNG_TTF_REPEAT_FLAG ) 
		{
			//get the next flag's byte to get the repeat count
            uint8_t repeatCount = flags[flagIndex++];

			/*
			printf("\nRepeat Flag %u = %u\n\n", n+1, flags[n+1]);
			bits = cr_convert_to_binary((unsigned *)(flags + n+1), 8);
    
	        for(bit = 8; bit--;)
				printf("%u", +bits[bit]);
                
	        free(bits);
			*/
			
			//assert repeat count is valid
            assert(repeatCount > 0);

			//skip over the contour point indices in the for loop
			//for which the current flag repeats
            //n += repeatCount;

            while( repeatCount-- ) 
			{
				//printf("\n repeat");

				n++;
				//serialize the delta value and add it to the running point sum
				//xcoord += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_X_SHORT_VECTOR, XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, &xcoordDeltas, &totalBytes, serialize);
				totalBytes += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_X_SHORT_VECTOR, XNG_TTF_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, &xcoordDeltas, &xcoord, 1, serialize);
				if( glyph )
				{
					glyph->xCoords[n] = xcoord;

					if( xcoord < glyph->header->xMin )
						glyph->header->xMin = xcoord;
					else if( xcoord  > glyph->header->xMax )
						glyph->header->xMax = xcoord;
				}
				//TO DO: store the current contour point x-value
				//points[i][name] = value;

           		//TO DO: add/store on curve point
				//repeatCount--;
			}
			

        }

	}


	//printf("\n\ntotalBytes after xcoords = %u\n", totalBytes);


	//now that we know we've read through the xcoord deltas
	//we know where the ycoord deltas start
	ycoordDeltas = (uint8_t*)(ttf_table_data) + totalBytes;
	yIndex = 0;
	ycoord = 0;

	if( glyph )
	{
		glyph->yDeltas = ycoordDeltas;
		glyph->yCoords = (int16_t*)malloc( sizeof(uint16_t) * numContourPoints );
	}

	//serialize to system byte order if we haven't set the custom flag on the reserved bit
	//in our ttf memory representation
	serialize = !(flags[0] & XNG_TTF_Y_VALUE_SERIALIZED);
	//printf("\nserialize y value = %d\n", serialize);

	flagIndex = 0;
	//we need to read through each flag again so we can use them
	//to serialize the x and y contour point coordinates of variable length
	for( n=0; n<numContourPoints; n++ )
	{
		//get the flag associated with the current contour point index
		//and increment the flag index to access the flags array
		uint8_t flag = flags[flagIndex++];
        
		//if this uint8_t flag's bit 0 (LSB) is set to 1
		//then the corresponding point that this flag points to is
		//on the bezier curve, otherwise it lies off the curve
		if( flag & XNG_TTF_ON_CURVE_POINT)
		{
			//TO DO: add/store on curve point	
		}


		/*
		bits = cr_convert_to_binary((unsigned *)(flags + n), 8);
    
		printf("\n", n);

		//printf("\nFlag %u = %u\n\n", n, flags[n]);
        for(bit = 8; bit--;)
            printf("%u", +bits[bit]);
        
		printf("\n", n);
        
        free(bits);
		*/

		//serialize the delta value and add it to the running point sum
		//ycoord += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_Y_SHORT_VECTOR, XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR, &ycoordDeltas, &totalBytes, serialize);
		totalBytes += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_Y_SHORT_VECTOR, XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR, &ycoordDeltas, &ycoord, 1, serialize);
		if( glyph )
		{
			glyph->yCoords[n] = ycoord;

			if( ycoord < glyph->header->yMin )
				glyph->header->yMin = ycoord;
			else if( ycoord  > glyph->header->yMax )
				glyph->header->yMax = ycoord;				
					
		}	
		//TO DO: store the current contour point x-value
        //points[i][name] = value;

		//if this uint8_t flag's repeat bit (bit 3) is set to 1
		//the next byte should be read as unsigned (ie uint8_t)
		//and specifies the number of times the current flag should be repeated
		//which i guess is only a performance optimization if the number
		//of consecutive flags is greater than 2
        if ( flag & XNG_TTF_REPEAT_FLAG ) 
		{
			//get the next flag's byte to get the repeat count
            uint8_t repeatCount = flags[flagIndex++];

			/*
			printf("\nRepeat Flag %u = %u\n\n", n+1, flags[n+1]);
			bits = cr_convert_to_binary((unsigned *)(flags + n+1), 8);
    
	        for(bit = 8; bit--;)
				printf("%u", +bits[bit]);
                
	        free(bits);
			*/
			
			//assert repeat count is valid
            assert(repeatCount > 0);

			//skip over the contour point indices in the for loop
			//for which the current flag repeats
            //n += repeatCount;

			
            while( repeatCount-- ) 
			{
				//printf("\n repeat");

				n++;
				//serialize the delta value and add it to the running point sum
				//ycoord += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_Y_SHORT_VECTOR, XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR, &ycoordDeltas, &totalBytes, serialize);
				totalBytes += xng_ttf_glyf_serialize_coord_delta(flag, XNG_TTF_Y_SHORT_VECTOR, XNG_TTF_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR, &ycoordDeltas, &ycoord, 1, serialize);
				if( glyph )
				{
					glyph->yCoords[n] = ycoord;

					if( ycoord < glyph->header->yMin )
						glyph->header->yMin = ycoord;
					else if( ycoord  > glyph->header->yMax )
						glyph->header->yMax = ycoord;				
					
				}	
				//TO DO: store the current contour point x-value
				//points[i][name] = value;

           		//TO DO: add/store on curve point
				//repeatCount--;
			}
			

        }

	}
	//printf("\n\ntotalBytes after ycoords = %u\n", totalBytes);

	
	//set the custom flags on the reserved bit of the first flag byte to 
	//indicate that we've arleady serialized to system byte order
	flags[0] |= XNG_TTF_X_VALUE_SERIALIZED;
	flags[0] |= XNG_TTF_Y_VALUE_SERIALIZED;

	if( glyph )
	{
		//glyph->numContours = numContours;
		glyph->endPtsOfContours = endPtsOfContours;

		glyph->numInstructions = instructionLength;
		glyph->instructions = instructions;

		glyph->numFlags = numFlags;
		glyph->flags = flags;

		glyph->xDeltas = xcoordDeltas;
		glyph->yDeltas = ycoordDeltas;
		
	}


	return totalBytes;
} 

typedef struct xng_ttf_component_glyf_transform
{
	float a,b,c,d,e,f;
}xng_ttf_component_glyf_transform;

#define XNG_TTF_GLYF_TRANSFORM_IDENTITY { 1, 0, 0, 1, 0, 0 }

typedef struct xng_ttf_component_glyf
{
	uint16_t flags, glyphIndex;
	uint16_t srcPointIndex, destPointIndex;
	xnz_float_affine_transform transform;
}xng_ttf_component_glyf;

static float xng_ttf_int16_to_F2Dot14(int16_t input)
{
	return (float)input / (float)(1 << 14);
}

static uint32_t xng_ttf_compound_glyf_table_draw_contour_data(int16_t indexToLocFormat, void* loca_table, void* glyf_table, void * ttf_table_data, xng_ttf_glyph * glyph)
{
	//uint16_t flags, glyphIndex;
	uint16_t component_flags;

	uint16_t arg1, arg2;
	uint32_t totalBytes;
	uint32_t numComponents;
	uint16_t numInstr;

	xng_ttf_glyf_table_header component_glyf_header;
	xng_ttf_glyph component_glyf;// = {0};
	xnz_float_affine_transform component_transform;// = XNG_TTF_GLYF_TRANSFORM_IDENTITY;//{1,0,0,1,0,0};

	uint16_t * table16;
	uint8_t * table8 = (uint8_t*)ttf_table_data;

	component_transform.a = component_transform.d = 1;
	component_transform .b = component_transform .c = component_transform .e = component_transform .f = 0;

	totalBytes = 0;
	numComponents = 0;
	do {
		uint16_t srcPointIndex, destPointIndex;
		uint32_t tableIndex;
		uint16_t arg1, arg2;
		table16 = (uint16_t*)(table8 + totalBytes);
		
		memset(&component_glyf, 0, sizeof(xng_ttf_glyph));
		component_glyf.affine_transform = component_transform;
		tableIndex = 0;
		
		//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
		component_flags = table16[tableIndex++];
		
		//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
		component_glyf.index = table16[tableIndex++];

		totalBytes += sizeof(uint16_t) * 2;

				//we need to use the component glyph index to access the offset to the component glyph in the ttf font glyf table
		if( indexToLocFormat == 0 )
		{
			uint16_t * offsets = (uint16_t*)(loca_table);
			component_glyf.offset = offsets[component_glyf.index] * 2;
		}
		else
		{
			uint32_t * offsets = (uint32_t*)(loca_table);
			component_glyf.offset = offsets[component_glyf.index];
		}
			
		/*Normally we would read the header associated with the simple or compound glyph to get its bounding box, advance width, advance height, leftSideBearing and topSideBearing*/
		/*However, because this is a compound glyph and we have already read the header for the head compound glyph, we will push past the header data*/
		/*and just use the component glyf index to find component glyf offset offset via the font->loca_table and use that offset to look up the corresponding glyph in the glyph table to*/
		/*continue to read contour data for each component glyf recursively until a simple glyf is reached*/
	
		//for drawing contours in a accumulation and bitmap buffer of size glyph.bb.width * glyph.bb.height, 
		//we will set each component glyf bounding box to the size of the head compound glyf bounding box
		component_glyf.bb = glyph->bb;
		
		//get the glyph table header to get its bounding box in units per em (FontUnits)
		//note that it has already been serialized to system byte format in the ram buffer
		component_glyf_header = *(xng_ttf_glyf_table_header*)((uint8_t*)(glyf_table) + component_glyf.offset);
		//component_glyf_header.numberOfContours = xnz_be16toh(component_glyf_header.numberOfContours);
		//component_glyf_header.xMin = xnz_be16toh(component_glyf_header.xMin);
		//component_glyf_header.yMin = xnz_be16toh(component_glyf_header.yMin);
		//component_glyf_header.xMax = xnz_be16toh(component_glyf_header.xMax);
		//component_glyf_header.yMax = xnz_be16toh(component_glyf_header.yMax);		
		component_glyf.header = &component_glyf_header; 

		if ( component_flags & XNG_TTF_ARG_1_AND_2_ARE_WORDS) 
		{
			arg1 /*= table16[tableIndex]*/ = table16[tableIndex]; //arg1
			tableIndex++;
			arg2 /*= table16[tableIndex]*/ = table16[tableIndex]; //arg2
			tableIndex++;

			totalBytes += sizeof(uint16_t) * 2;
			//(int16 or FWord) argument1;
			//(int16 or FWord) argument2;
		} 
		else 
		{
			arg1 = *(table8 + totalBytes);
			arg2 = *(table8 + totalBytes + 1);
			
			//uint16 arg1and2; /* (arg1 << 8) | arg2 */
			totalBytes += sizeof(uint8_t) * 2;
			tableIndex++;

		}

		//TO DO: store component glyf information
		
		if ( component_flags & XNG_TTF_ARGS_ARE_XY_VALUES ) {
            component_glyf.affine_transform.e = arg1;
            component_glyf.affine_transform.f = arg2;
        } else {
            destPointIndex = arg1;
            srcPointIndex = arg2;
        }
		

		if ( component_flags & XNG_TTF_WE_HAVE_A_SCALE ) 
		{
			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_glyf.affine_transform.a = component_glyf.affine_transform.d = xng_ttf_int16_to_F2Dot14(table16[tableIndex++]);
			//F2Dot14  scale;    /* Format 2.14 */

			totalBytes += sizeof(uint16_t);
		} 
		else if ( component_flags & XNG_TTF_WE_HAVE_AN_X_AND_Y_SCALE ) 
		{
			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_glyf.affine_transform.a = xng_ttf_int16_to_F2Dot14(table16[tableIndex++]);

			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_glyf.affine_transform.d = xng_ttf_int16_to_F2Dot14(table16[tableIndex++]);

			totalBytes += sizeof(uint16_t) * 2;

			//F2Dot14  xscale;    /* Format 2.14 */
			//F2Dot14  yscale;    /* Format 2.14 */
		}
		else if ( component_flags & XNG_TTF_WE_HAVE_A_TWO_BY_TWO ) 
		{
			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_glyf.affine_transform.a = xng_ttf_int16_to_F2Dot14(table16[tableIndex++]);

			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_glyf.affine_transform.b = xng_ttf_int16_to_F2Dot14(table16[tableIndex++]);

			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_glyf.affine_transform.c = xng_ttf_int16_to_F2Dot14(table16[tableIndex++]);

			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_glyf.affine_transform.d = xng_ttf_int16_to_F2Dot14(table16[tableIndex++]);

			totalBytes += sizeof(uint16_t) * 4;


			//F2Dot14  xscale;    /* Format 2.14 */
			//F2Dot14  scale01;   /* Format 2.14 */
			//F2Dot14  scale10;   /* Format 2.14 */
			//F2Dot14  yscale;    /* Format 2.14 */
		}
		else
		{
			
			 // TODO: handle non-ARGS_ARE_XY_VALUES case
			//assert(1 == 0 );

			component_glyf.affine_transform.a = 1.f;
			component_glyf.affine_transform.b = 0.f;
			component_glyf.affine_transform.c = 1.f;
			component_glyf.affine_transform.d = -1.f;
			component_glyf.affine_transform.e = ((float)component_glyf.header->xMin * -1.f) ;
			component_glyf.affine_transform.f = -((float)component_glyf.header->yMax * -1.f)  ;
			

		}
		//concatenate the compound glyph transform derived from scale and bounding box from the glyph header
		//that translates from em square into positive bitmap coordinates for outline drawing
		//with the transform we just read for the component glyph 
		//
		//	a: t1.a * t2.a + t1.c * t2.b,
        //    b: t1.b * t2.a + t1.d * t2.b,
        //    c: t1.a * t2.c + t1.c * t2.d,
        //    d: t1.b * t2.c + t1.d * t2.d,
        //    e: t1.a * t2.e + t1.c * t2.f + t1.e,
        //    f: t1.b * t2.e + t1.d * t2.f + t1.f,
		
		component_glyf.affine_transform.a = glyph->affine_transform.a * component_glyf.affine_transform.a + glyph->affine_transform.c * component_glyf.affine_transform.b;
		component_glyf.affine_transform.b = glyph->affine_transform.b * component_glyf.affine_transform.a + glyph->affine_transform.d * component_glyf.affine_transform.b;
		component_glyf.affine_transform.c = glyph->affine_transform.a * component_glyf.affine_transform.c + glyph->affine_transform.c * component_glyf.affine_transform.d;
		component_glyf.affine_transform.d = glyph->affine_transform.b * component_glyf.affine_transform.c + glyph->affine_transform.d * component_glyf.affine_transform.d;
		component_glyf.affine_transform.e = glyph->affine_transform.a * component_glyf.affine_transform.e + glyph->affine_transform.c * component_glyf.affine_transform.f + glyph->affine_transform.e;
		component_glyf.affine_transform.f = glyph->affine_transform.b * component_glyf.affine_transform.e + glyph->affine_transform.d * component_glyf.affine_transform.f + glyph->affine_transform.f;

		component_glyf.accumulation_buffer = glyph->accumulation_buffer;
	
		/*
		//calculate the bounding box for the glyph at unitsPerEm resolution
		glyph->bb.width = glyph->header->xMax - glyph->header->xMin;//glyph.bb.right - glyph.bb.left;
		glyph->bb.height = glyph->header->yMax - glyph->header->yMin;//glyph.bb.top - glyph.bb.bottom;

			xng_ttf_hmtx_table_get_metrics_for_glyph_at_index(font->hmtx_table, font->numLongHorMetrics, font->numGlyphs, glyph);
		//xng_ttf_vmtx_table_get_metrics_for_glyph_at_index(font->vmtx_table, font->numLongVerMetrics, font->numGlyphs, glyph);

		if( font->vmtx_table )
			xng_ttf_vmtx_table_get_metrics_for_glyph_at_index(font->vmtx_table, font->numLongVerMetrics, font->numGlyphs, glyph);
		//there is no vertical table data present, we must emulate a la truetype
		//else if ( face->os2.version != 0xFFFFU )
		//{
		  //*tsb = (FT_Short)( face->os2.sTypoAscender - yMax );
		  //*ah  = (FT_UShort)FT_ABS( face->os2.sTypoAscender -
			//						face->os2.sTypoDescender );
		//}
		else
		{
			glyph->topSideBearing  = font->hhea_table->ascent - glyph->header->yMax;
			glyph->advanceHeight = font->hhea_table->ascent - font->hhea_table->descent;

			// *tsb = (FT_Short)( face->horizontal.Ascender - yMax );
			// *ah  = (FT_UShort)FT_ABS( face->horizontal.Ascender -
			//                      face->horizontal.Descender );
		}
		*/

		//recursively process the component glyf
		//  if it is a compound glyf this function will be run on the component glyf
		//  if it is simple glyf then it the component glyf contour will be drawn to the accumulation buffer
		xng_ttf_glyf_table_draw_contour_data(indexToLocFormat, loca_table, glyf_table, (void*)((uint8_t*)glyf_table + component_glyf.offset + sizeof(xng_ttf_glyf_table_header)), &component_glyf);//, &glyph_bb );

		numComponents++;
    }while ( component_flags & XNG_TTF_MORE_COMPONENTS );
	

	numInstr = 0;
	if (component_flags & XNG_TTF_WE_HAVE_INSTRUCTIONS){
		
		table16 = (uint16_t*)(table8 + totalBytes);
		//table16[0] = xnz_be16toh(table16[0]);
		numInstr = table16[0];

		totalBytes += sizeof(uint16_t);

		totalBytes += numInstr;
		//uint8 instr[numInstr]
	}
	//printf("\nNum Instructions = %hu\n", numInstr);
	//printf("\n Num Components = %u\n", numComponents);
	//printf("\n Total Component Glyf Bytes = %u\n", totalBytes);

	return totalBytes;

}

static const xnz_float_affine_transform CR_FLOAT_AFFINE_TRANSFORM_IDENTITY = {1.f, 0.f, 0.f, 1.f, 0.f, 0.f};
static const xnz_float_affine_transform CR_FLOAT_AFFINE_TRANSFORM_FLIP_VERTICAL = {1.f, 0.f, 0.f, 1.f, 0.f, 0.f};

static uint32_t xng_ttf_compound_glyf_table_serialize_and_draw_contour_data(int16_t indexToLocFormat, void* loca_table, void* glyf_table, void * ttf_table_data, xng_ttf_glyph * glyph)
{
	//uint16_t flags, glyphIndex;
	uint16_t component_flags;

	//uint16_t arg1, arg2;
	uint32_t totalBytes;
	uint32_t numComponents;
	uint16_t numInstr;

	xng_ttf_glyf_table_header component_glyf_header;
	xng_ttf_glyph component_glyf;
	xnz_float_affine_transform component_transform, compound_transform;

	int16_t * table16; //Note:  Must be signed integer for XNG_TTF_ARGS_ARE_XY_VALUES
	uint8_t * table8 = (uint8_t*)ttf_table_data;

	memset(&component_glyf, 0, sizeof(xng_ttf_glyph));
	
	compound_transform				= CR_FLOAT_AFFINE_TRANSFORM_IDENTITY;
	component_transform				= CR_FLOAT_AFFINE_TRANSFORM_IDENTITY;
	component_glyf.affine_transform = CR_FLOAT_AFFINE_TRANSFORM_IDENTITY;
	
	totalBytes = 0;
	numComponents = 0;
	do {
		uint16_t srcPointIndex, destPointIndex;
		uint32_t tableIndex;
		int16_t arg1, arg2;
		table16 = (int16_t*)(table8 + totalBytes);
		
		
		//component_glyf.affine_transform = component_transform;
		tableIndex = 0;
		
		//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
		component_flags = xnz_be16toh(table16[tableIndex++]);
		
		//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
		component_glyf.index = xnz_be16toh(table16[tableIndex++]);

		totalBytes += sizeof(uint16_t) * 2;

		//we need to use the component glyph index to access the offset to the component glyph in the ttf font glyf table
		if( indexToLocFormat == 0 )
		{
			uint16_t * offsets = (uint16_t*)(loca_table);
			component_glyf.offset = offsets[component_glyf.index] * 2;
		}
		else
		{
			uint32_t * offsets = (uint32_t*)(loca_table);
			component_glyf.offset = offsets[component_glyf.index];
		}
			
		/*Normally we would read the header associated with the simple or compound glyph to get its bounding box, advance width, advance height, leftSideBearing and topSideBearing*/
		/*However, because this is a compound glyph and we have already read the header for the head compound glyph, we will push past the header data*/
		/*and just use the component glyf index to find component glyf offset offset via the font->loca_table and use that offset to look up the corresponding glyph in the glyph table to*/
		/*continue to read contour data for each component glyf recursively until a simple glyf is reached*/
	
		//for drawing contours in a accumulation and bitmap buffer of size glyph.bb.width * glyph.bb.height, 
		//we will set each component glyf bounding box to the size of the head compound glyf bounding box
		component_glyf.bb = glyph->bb;
		
		//get the glyph table header to get its bounding box in units per em (FontUnits)
		//note that it has already been serialized to system byte format in the ram buffer
		component_glyf_header = *(xng_ttf_glyf_table_header*)((uint8_t*)(glyf_table) + component_glyf.offset);
		component_glyf_header.numberOfContours = xnz_be16toh(component_glyf_header.numberOfContours);
		component_glyf_header.xMin = xnz_be16toh(component_glyf_header.xMin);
		component_glyf_header.yMin = xnz_be16toh(component_glyf_header.yMin);
		component_glyf_header.xMax = xnz_be16toh(component_glyf_header.xMax);
		component_glyf_header.yMax = xnz_be16toh(component_glyf_header.yMax);		
		component_glyf.header = &component_glyf_header; 

		printf("\n numContours = %hd\n", component_glyf_header.numberOfContours);
		printf("xMin = %hd\n", component_glyf_header.xMin);
		printf("xMax = %hd\n", component_glyf_header.xMax);
		printf("yMin = %hd\n", component_glyf_header.yMin);
		printf("yMax = %hd\n", component_glyf_header.yMax);


		if ( component_flags & XNG_TTF_ARG_1_AND_2_ARE_WORDS) 
		{
			arg1 /*= table16[tableIndex]*/ = xnz_be16toh(table16[tableIndex]); //arg1
			tableIndex++;
			arg2 /*= table16[tableIndex]*/ = xnz_be16toh(table16[tableIndex]); //arg2
			tableIndex++;

			totalBytes += sizeof(uint16_t) * 2;
			//(int16 or FWord) argument1;
			//(int16 or FWord) argument2;
		} 
		else 
		{
			arg1 = *(table8 + totalBytes);
			arg2 = *(table8 + totalBytes + 1);
			
			//uint16 arg1and2; /* (arg1 << 8) | arg2 */
			totalBytes += sizeof(uint8_t) * 2;
			tableIndex++;

		}

		component_transform = CR_FLOAT_AFFINE_TRANSFORM_IDENTITY;
		if ( component_flags & XNG_TTF_ARGS_ARE_XY_VALUES ) 
		{
			if (component_flags && XNG_TTF_ROUND_XY_TO_GRID)
			{
				//assert(arg1 <= glyph->header->xMax - glyph->header->xMin);
				//assert(arg2 <= glyph->header->yMax - glyph->header->yMin);
				
				//if (arg1 > glyph->header->xMax - glyph->header->xMin) arg1 = 0;//component_glyf_header.xMax - component_glyf_header.xMin;
				//if (arg2 > glyph->header->yMax - glyph->header->yMin) arg2 = 0;//component_glyf_header.yMax - component_glyf_header.yMin;
			}

            component_transform.e = arg1;
            component_transform.f = arg2;
        } 
		else 
		{
			assert(1==0);
            destPointIndex = arg1;
            srcPointIndex = arg2;
        }
		

		if ( component_flags & XNG_TTF_WE_HAVE_A_SCALE ) 
		{
			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_transform.a = component_transform.d = xng_ttf_int16_to_F2Dot14(xnz_be16toh(table16[tableIndex++]));
			//F2Dot14  scale;    /* Format 2.14 */

			totalBytes += sizeof(uint16_t);
		} 
		else if ( component_flags & XNG_TTF_WE_HAVE_AN_X_AND_Y_SCALE ) 
		{
			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_transform.a = xng_ttf_int16_to_F2Dot14(xnz_be16toh(table16[tableIndex++]));

			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_transform.d = xng_ttf_int16_to_F2Dot14(xnz_be16toh(table16[tableIndex++]));

			totalBytes += sizeof(uint16_t) * 2;

			//F2Dot14  xscale;    /* Format 2.14 */
			//F2Dot14  yscale;    /* Format 2.14 */
		}
		else if ( component_flags & XNG_TTF_WE_HAVE_A_TWO_BY_TWO ) 
		{
			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_transform.a = xng_ttf_int16_to_F2Dot14(xnz_be16toh(table16[tableIndex++]));

			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_transform.b = xng_ttf_int16_to_F2Dot14(xnz_be16toh(table16[tableIndex++]));

			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_transform.c = xng_ttf_int16_to_F2Dot14(xnz_be16toh(table16[tableIndex++]));

			//table16[tableIndex] = xnz_be16toh(table16[tableIndex]);
			component_transform.d = xng_ttf_int16_to_F2Dot14(xnz_be16toh(table16[tableIndex++]));

			totalBytes += sizeof(uint16_t) * 4;


			//F2Dot14  xscale;    /* Format 2.14 */
			//F2Dot14  scale01;   /* Format 2.14 */
			//F2Dot14  scale10;   /* Format 2.14 */
			//F2Dot14  yscale;    /* Format 2.14 */
		}
		else
		{
			
			//component_transform = CR_FLOAT_AFFINE_TRANSFORM_IDENTITY;
			//component_transform.e = -component_glyf.affine_transform.e;
            //component_transform.f = -component_glyf.affine_transform.f;
			 
			// TODO: handle non-ARGS_ARE_XY_VALUES case
			//assert(1 == 0 );

			//component_glyf.affine_transform.a = 1.f;
			//component_glyf.affine_transform.b = 0.f;
			//component_glyf.affine_transform.c = 0.f;
			//component_glyf.affine_transform.d = -1.f;
			//component_glyf.affine_transform.e = ((float)component_glyf.header->xMin * -1.f) ;
			//component_glyf.affine_transform.f = -((float)component_glyf.header->yMax * -1.f)  ;
			
			component_transform = CR_FLOAT_AFFINE_TRANSFORM_IDENTITY;

			//component_glyf.affine_transform = CR_FLOAT_AFFINE_TRANSFORM_IDENTITY;

		}


		component_transform.e = (float)arg1;
		component_transform.f = (float)arg2;


		printf("\ncomponent_transform.e = %f\n", component_transform.e);
		printf("\ncomponent_transform.f = %f\n", component_transform.f);

		//concatenate the compound glyph transform derived from scale and bounding box from the glyph header
		//that translates from em square into positive bitmap coordinates for outline drawing
		//with the transform we just read for the component glyph 
		//
		//	a: t1.a * t2.a + t1.c * t2.b,
        //    b: t1.b * t2.a + t1.d * t2.b,
        //    c: t1.a * t2.c + t1.c * t2.d,
        //    d: t1.b * t2.c + t1.d * t2.d,
        //    e: t1.a * t2.e + t1.c * t2.f + t1.e,
        //    f: t1.b * t2.e + t1.d * t2.f + t1.f,
		
		
		//apply the local component glyf transform to the running compound transform
		//component_glyf.affine_transform.a = compound_transform.a * component_transform.a + compound_transform.c * component_transform.b;
		//component_glyf.affine_transform.b = compound_transform.b * component_transform.a + compound_transform.d * component_transform.b;
		//component_glyf.affine_transform.c = compound_transform.a * component_transform.c + compound_transform.c * component_transform.d;
		//component_glyf.affine_transform.d = compound_transform.b * component_transform.c + compound_transform.d * component_transform.d;
		//component_glyf.affine_transform.e = compound_transform.a * component_transform.e + compound_transform.c * component_transform.f + compound_transform.e;
		//component_glyf.affine_transform.f = compound_transform.b * component_transform.e + compound_transform.d * component_transform.f + compound_transform.f;
		

		//store the compound transform for the next iteration
		compound_transform = component_glyf.affine_transform;

		compound_transform = component_transform;

		//apply the global glyf transform to the component glyf transform
		component_glyf.affine_transform.a = glyph->affine_transform.a * compound_transform.a + glyph->affine_transform.c * compound_transform.b;
		component_glyf.affine_transform.b = glyph->affine_transform.b * compound_transform.a + glyph->affine_transform.d * compound_transform.b;
		component_glyf.affine_transform.c = glyph->affine_transform.a * compound_transform.c + glyph->affine_transform.c * compound_transform.d;
		component_glyf.affine_transform.d = glyph->affine_transform.b * compound_transform.c + glyph->affine_transform.d * compound_transform.d;
		component_glyf.affine_transform.e = glyph->affine_transform.a * compound_transform.e + glyph->affine_transform.c * compound_transform.f + glyph->affine_transform.e;
		component_glyf.affine_transform.f = glyph->affine_transform.b * compound_transform.e + glyph->affine_transform.d * compound_transform.f + glyph->affine_transform.f;


		component_glyf.accumulation_buffer = glyph->accumulation_buffer;
	
		/*
		//calculate the bounding box for the glyph at unitsPerEm resolution
		glyph->bb.width = glyph->header->xMax - glyph->header->xMin;//glyph.bb.right - glyph.bb.left;
		glyph->bb.height = glyph->header->yMax - glyph->header->yMin;//glyph.bb.top - glyph.bb.bottom;

			xng_ttf_hmtx_table_get_metrics_for_glyph_at_index(font->hmtx_table, font->numLongHorMetrics, font->numGlyphs, glyph);
		//xng_ttf_vmtx_table_get_metrics_for_glyph_at_index(font->vmtx_table, font->numLongVerMetrics, font->numGlyphs, glyph);

		if( font->vmtx_table )
			xng_ttf_vmtx_table_get_metrics_for_glyph_at_index(font->vmtx_table, font->numLongVerMetrics, font->numGlyphs, glyph);
		//there is no vertical table data present, we must emulate a la truetype
		//else if ( face->os2.version != 0xFFFFU )
		//{
		  //*tsb = (FT_Short)( face->os2.sTypoAscender - yMax );
		  //*ah  = (FT_UShort)FT_ABS( face->os2.sTypoAscender -
			//						face->os2.sTypoDescender );
		//}
		else
		{
			glyph->topSideBearing  = font->hhea_table->ascent - glyph->header->yMax;
			glyph->advanceHeight = font->hhea_table->ascent - font->hhea_table->descent;

			// *tsb = (FT_Short)( face->horizontal.Ascender - yMax );
			// *ah  = (FT_UShort)FT_ABS( face->horizontal.Ascender -
			//                      face->horizontal.Descender );
		}
		*/

		//recursively process the component glyf
		//  if it is a compound glyf this function will be run on the component glyf
		//  if it is simple glyf then it the component glyf contour will be drawn to the accumulation buffer
		
		//if( numComponents == 0 )
		xng_ttf_glyf_table_serialize_and_draw_contour_data(indexToLocFormat, loca_table, glyf_table, (void*)((uint8_t*)glyf_table + component_glyf.offset + sizeof(xng_ttf_glyf_table_header)), &component_glyf);//, &glyph_bb );

		numComponents++;
    }while ( component_flags & XNG_TTF_MORE_COMPONENTS );
	

	numInstr = 0;
	if (component_flags & XNG_TTF_WE_HAVE_INSTRUCTIONS){
		
		table16 = (uint16_t*)(table8 + totalBytes);
		//table16[0] = xnz_be16toh(table16[0]);
		numInstr = xnz_be16toh(table16[0]);

		totalBytes += sizeof(uint16_t);

		totalBytes += numInstr;
		//uint8 instr[numInstr]
	}
	//printf("\nNum Instructions = %hu\n", numInstr);
	//printf("\n Num Components = %u\n", numComponents);
	//printf("\n Total Component Glyf Bytes = %u\n", totalBytes);

	return totalBytes;

}

static void xng_ttf_glyf_table_serialize_header_data(void * ttf_table_data, xng_ttf_glyph * glyph)
{
	xng_ttf_glyf_table_header * glyf_header;
	uint32_t numGlyfTableBytes = 0;

	//read the common glyf table properties defined by xng_ttf_glyf_table_header
	glyf_header = xng_ttf_glyf_table_header_serialize( (void*)(ttf_table_data) );
	if( glyph )
	{
		glyph->header = glyf_header;//->numberOfContours;
	}

	//printf("\nxng_ttf_glyf_table_header size = %d", sizeof(xng_ttf_glyf_table_header));
	numGlyfTableBytes += sizeof(xng_ttf_glyf_table_header);

	/*
	if( glyf_header->numberOfContours < 0 )
	{
		//printf("\n Is compound glyf header\n");

		//is a compound glyf
		numGlyfTableBytes += xng_ttf_compound_glyf_table_serialize( (void*) ((uint8_t*)ttf_table_data + sizeof(xng_ttf_glyf_table_header)), glyph );
	}
	else
	{
		//printf("\n Is simple glyf header \n");
		//is a simple glyf
		numGlyfTableBytes += xng_ttf_simple_glyf_table_header_serialize( (void*) ((uint8_t*)ttf_table_data + sizeof(xng_ttf_glyf_table_header)),  glyf_header->numberOfContours, glyph );
		//printf("\nxng_ttf_simple_glyf_table_serialize\n");

	}
	*/

    return ;//numGlyfTableBytes;

}

static uint32_t xng_ttf_glyf_table_draw_contour_data(int16_t indexToLocFormat, void* loca_table, void* glyf_table, void * ttf_table_data, xng_ttf_glyph * glyph)//, xng_ttf_glyph_bounding_box * glyph_bb)
{
	//xng_ttf_glyf_table_header * glyf_header;
	uint32_t numGlyfTableBytes = 0;

	/*
	//read the common glyf table properties defined by xng_ttf_glyf_table_header
	glyf_header = (xng_ttf_glyf_table_header*)ttf_table_data;
	if( glyph )
	{
		glyph->header = glyf_header;//->numberOfContours;
	}
	*/
	//printf("\nxng_ttf_glyf_table_header size = %d", sizeof(xng_ttf_glyf_table_header));
	//numGlyfTableBytes += sizeof(xng_ttf_glyf_table_header);

	if( glyph->header->numberOfContours < 0 )
	{
		//printf("\n Is compound glyf \n");
		//is a compound glyf
		//assert(1 == 0 );
		numGlyfTableBytes += xng_ttf_compound_glyf_table_draw_contour_data( indexToLocFormat, loca_table, glyf_table, /*(void*) ((uint8_t*)*/ ttf_table_data /*+ sizeof(xng_ttf_glyf_table_header))*/, glyph );
	}
	else
	{
		//printf("\n Is simple glyf \n");
		//is a simple glyf
		numGlyfTableBytes += xng_ttf_simple_glyf_table_draw_contour_data( /*(void*) ((uint8_t*)*/ ttf_table_data /*+ sizeof(xng_ttf_glyf_table_header))*/,  glyph, &(glyph->affine_transform) );
	}

	return numGlyfTableBytes;

}


static uint32_t xng_ttf_glyf_table_serialize_and_draw_contour_data(int16_t indexToLocFormat, void* loca_table, void* glyf_table, void * ttf_table_data, xng_ttf_glyph * glyph)//, xng_ttf_glyph_bounding_box * glyph_bb)
{
	//xng_ttf_glyf_table_header * glyf_header;
	uint32_t numGlyfTableBytes = 0;

	/*
	//read the common glyf table properties defined by xng_ttf_glyf_table_header
	glyf_header = (xng_ttf_glyf_table_header*)ttf_table_data;
	if( glyph )
	{
		glyph->header = glyf_header;//->numberOfContours;
	}
	*/
	//printf("\nxng_ttf_glyf_table_header size = %d", sizeof(xng_ttf_glyf_table_header));
	//numGlyfTableBytes += sizeof(xng_ttf_glyf_table_header);

	if( glyph->header->numberOfContours < 0 )
	{
		//printf("\n Is compound glyf \n");
		//is a compound glyf
		//assert(1 == 0 );
		numGlyfTableBytes += xng_ttf_compound_glyf_table_serialize_and_draw_contour_data( indexToLocFormat, loca_table, glyf_table, /*(void*) ((uint8_t*)*/ ttf_table_data /*+ sizeof(xng_ttf_glyf_table_header))*/, glyph );
	}
	else
	{
		//printf("\n Is simple glyf \n");
		//is a simple glyf
		numGlyfTableBytes += xng_ttf_simple_glyf_table_serialize_and_draw_contour_data( /*(void*) ((uint8_t*)*/ ttf_table_data /*+ sizeof(xng_ttf_glyf_table_header))*/,  glyph, &(glyph->affine_transform) );
	}

	return numGlyfTableBytes;

}
/*
//returns the size of the glyf table for convenience, because you read the 'loca' table glyf offsets
//then it is unknown how large each glyf table entry is until the compressed glyf entry data of variable size is read
uint32_t xng_ttf_glyf_table_serialize(void * ttf_table_data, xng_ttf_glyph * glyph)
{
	xng_ttf_glyf_table_header * glyf_header;
	uint32_t numGlyfTableBytes = 0;

	//read the common glyf table properties defined by xng_ttf_glyf_table_header
	glyf_header = xng_ttf_glyf_table_header_serialize( (void*)(ttf_table_data) );
	if( glyph )
	{
		glyph->header = glyf_header;//->numberOfContours;
	}
	printf("\nxng_ttf_glyf_table_header size = %d", sizeof(xng_ttf_glyf_table_header));
	numGlyfTableBytes += sizeof(xng_ttf_glyf_table_header);

	if( glyf_header->numberOfContours < 0 )
	{
		printf("\n Is compound glyf \n");

		//is a compound glyf
		numGlyfTableBytes += xng_ttf_compound_glyf_table_serialize( (void*) ((uint8_t*)ttf_table_data + sizeof(xng_ttf_glyf_table_header)), glyph );
	}
	else
	{
		printf("\n Is simple glyf \n");
		//is a simple glyf
		numGlyfTableBytes += xng_ttf_simple_glyf_table_serialize( (void*) ((uint8_t*)ttf_table_data + sizeof(xng_ttf_glyf_table_header)),  glyf_header->numberOfContours, glyph );
		//printf("\nxng_ttf_simple_glyf_table_serialize\n");

	}

	return numGlyfTableBytes;

}
*/

static void xng_ttf_name_table_header_serialize(void * ttf_table_data)
{
	//int i;
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_NAME_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD

}

static void xng_ttf_name_record_serialize(void * ttf_table_data)
{
	//int i;
#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
    XNG_TTF_NAME_RECORD_FIELDS
#undef XNG_TTF_TABLE_FIELD

}

static void xng_ttf_name_strings_serialize(void * ttf_table_data)
{
	int i, j;
	xng_ttf_name_record* name_record;
	xng_ttf_name_table* nameTable = (xng_ttf_name_table*)ttf_table_data;

	//print each name record in a for loop
	for(i = 0; i<nameTable->count; i++)
	{
		name_record = (xng_ttf_name_record*)(&(nameTable->nameRecords) + i * sizeof(xng_ttf_name_record)); 
		
		if( name_record->platformID == 3 )
		{
			uint16_t* bytes = (uint16_t*)(nameTable->names + name_record->offset );
			
			for( j = 0; j < name_record->length/2; j++)
			{ 
				bytes[j] = xnz_be16toh(bytes[j]);
			}
		}
	}
}

//#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
//	xng_ttf_table_serialize(&(((xng_ttf_table_data *)ttf_table_data)->varName), dataTypeSizeInBits, numElements, #datatype);
//    XNG_TTF_NAME_TABLE_FIELDS
//#undef XNG_TTF_TABLE_FIELD


/***
 ***    PRINT FUNCTIONS FOR ALL [SUPPORTED] RECORD TYPES
 ***/
//#pragma mark -- PRINT

/***
 ***    UNIVERSAL PRINT FUNCTION FOR ALL RECORD DATA FIELDS PARSED BY X-MACROS
 ***/
static void xng_ttf_table_field_print(char * formatString, char * catString, char * fieldName, void * data, unsigned int elementSizeInBits, unsigned int numElements/*, char * formatSpecifier */, bool floatingPoint, char * dataType )
{
	int i, bit;
	unsigned * bits;
    
    //if (strstr(fieldName, "reserved") != NULL)
    //    return;
    
    if( elementSizeInBits == 64 )
    {
        if( floatingPoint )
        {
            double * doubleData = (double *)data;
            printf(formatString, *doubleData);
            for(i = 1; i< numElements; i++)
            {
                printf(catString, doubleData[i]);
            }
        }
        else
        {
			if( strcmp(dataType, "xng_ttf_data_type_longDateTime") == 0 )
			{
				uint64_t* intData = (uint64_t*)data;
				printf("\n\t%s = ", fieldName);
				printf("%lld",  intData[0]);  
			}
			else
			{
				uint64_t * intData = (uint64_t *)data;
				printf(formatString, *intData);
				for(i = 1; i< numElements; i++)
				{
					printf(catString, intData[i]);
				}
			}
        }
    }
    else if( elementSizeInBits == 32 )
    {
            
        if (strstr(fieldName, "flags") != NULL)
        {
            printf("\n\t%s = ", fieldName);
            bits = cr_convert_to_binary((unsigned *)data, elementSizeInBits);
    
            for(bit = elementSizeInBits; bit--;)
                printf("%u", +bits[bit]);
                
            free(bits);
        }
		else if( strcmp(dataType, "xng_ttf_data_type_Fixed") == 0 )
		{
			int16_t* intData = (int16_t*)data;
			printf("\n\t%s = ", fieldName);
			printf("%hd.%hd",  intData[0], intData[1]);  
		}
        else
        {
            if( floatingPoint )
            {
                float * floatData = (float *)data;
                printf(formatString, *floatData);
                for(i = 1; i< numElements; i++)
                {
                    printf(catString, floatData[i]);
                }
            }
            else
            {
                uint32_t * intData = (uint32_t *)data;
                printf(formatString, *intData);
                for(i = 1; i< numElements; i++)
                {
                    printf(catString, intData[i]);
                }
            }
                

        }
    }
    else if( elementSizeInBits == 16 )
    {
        if (strstr(fieldName, "flags") != NULL || strcmp(fieldName, "macStyle") == 0 )
        {
            printf("\n\t%s = ", fieldName);
            bits = cr_convert_to_binary((unsigned *)data, elementSizeInBits);
                
            for(bit = elementSizeInBits; bit--;)
                printf("%u", +bits[bit]);
                
            free(bits);
        }
		else if( strcmp(dataType, "xng_ttf_data_type_Fixed") == 0 )
		{
			int16_t* intData = (int16_t*)data;
			printf("\n\t%s = ", fieldName);
			printf("%hd.%hd",  intData[0], intData[1]);  
		}
		else if( strcmp(dataType, "xng_ttf_data_type_FWord") == 0 )
		{
			int16_t* intData = (int16_t*)data;
			printf("\n\t%s = ", fieldName);
			printf("%hd",  intData[0]);  
		}
        else
        {
            short * shortData = (short *)data;
            printf(formatString, *shortData);
            for(i = 1; i< numElements; i++)
            {
                printf(catString, shortData[i]);
            }
        }
    }
    else
    {
        //how to differentiate between 8-bit integers stored as chars and and chararacter arrays which are strings?
        //solution is to assign %u instead of percent %s for static char arrays defined using x macros and print each element consecutively
		if( strcmp(dataType, "xng_ttf_data_type_Fixed") == 0 )
		{
			int16_t* intData = (int16_t*)data;
			printf("\n\t%s = ", fieldName);
			printf("%hd.%hd",  intData[0], intData[1]);  
		}
		else if( strcmp(dataType, "xng_ttf_data_type_longDateTime") == 0 )
		{
			int64_t* intData = (int64_t*)data;
			printf("\n\t%s = ", fieldName);
			printf("%I64d",  intData[0]);  
		}
		else
		{
			//arrays of 8-bit ints currently unsupported
			char * charData = (char *)data;
			if( numElements > 1)
				printf(formatString, charData);
			else
				printf(formatString, *charData);
			//for(int i = 1; i< numElements; i++)	
			//{
			//    printf(catString, shortData[i]);
			//}
		}
    }

}

//sprintf (formatString, "\t%s = %s\n", #varName, stringFormat); \
//printf(formatString, ((cr_flt_record_data *)flt_record_data)->varName);
/*
void xng_ttf_offset_table_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
    xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0 );
	XNG_TTF_OFFSET_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}
*/
static void xng_ttf_directory_table_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_DIRECTORY_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}


static void xng_ttf_offset_table_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_OFFSET_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}


static void xng_ttf_head_table_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_HEADER_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}

static void xng_ttf_hhea_table_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_HHEA_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}

static void xng_ttf_cmap_format4_mapping_table_header_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_CMAP_FORMAT4_MAPPING_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}

static void xng_ttf_cmap_format6_mapping_table_header_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_CMAP_FORMAT6_MAPPING_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}

static void xng_ttf_maxp_table_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_MAXP_TABLE_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}

static void xng_ttf_cmap_encoding_record_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_CMAP_ENCODING_RECORD_FIELDS
#undef XNG_TTF_TABLE_FIELD
    
}


static void xng_ttf_glyf_table_header_print(void *ttf_data)
{
	    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_GLYF_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD
    


}

static void xng_ttf_name_table_header_print(void * ttf_data)
{
    char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_NAME_TABLE_HEADER_FIELDS
#undef XNG_TTF_TABLE_FIELD

    

}

static void xng_ttf_name_record_print(void * ttf_data)
{
	char formatString[256];
    char catString[256];

#define XNG_TTF_TABLE_FIELD(xng_ttf_table_data, datatype, dataTypeSizeInBits, numElements, varName, varDeclaration, stringFormat) \
    sprintf (formatString, "\n\t%s = %s", #varName, stringFormat); \
    sprintf(catString, ", %s", stringFormat); \
	xng_ttf_table_field_print(formatString, catString, #varName, &(((xng_ttf_table_data *)ttf_data)->varName), dataTypeSizeInBits, numElements, strcmp(#datatype, "float") == 0 || strcmp(#datatype, "double") == 0, #datatype );
	XNG_TTF_NAME_RECORD_FIELDS
#undef XNG_TTF_TABLE_FIELD

}

static void xng_ttf_name_records_print(void * ttf_data)
{
	int i;//, j;
	xng_ttf_name_record* name_record;
	xng_ttf_name_table* nameTable = (xng_ttf_name_table*)ttf_data;

	//print each name record in a for loop
	for(i = 0; i<nameTable->count; i++)
	{
		xng_ttf_name_record_print( &(nameTable->nameRecords) + i * sizeof(xng_ttf_name_record) );
		printf("\n");

		name_record = (xng_ttf_name_record*)(&(nameTable->nameRecords) + i * sizeof(xng_ttf_name_record)); 
		
		if( name_record->platformID == 1 )
		{
			printf("\n%.*s\n", name_record->length, nameTable->names + name_record->offset );
		}
		else if( name_record->platformID == 3 )
		{
			uint16_t* bytes = (uint16_t*)(nameTable->names + name_record->offset );

			/*
			for( j = 0; j < name_record->length/2; j++)
			{ 
				bytes[j] = xnz_be16toh(bytes[j]);
			}
			*/

			printf("\n%.*S\n", name_record->length/2,  bytes );
		}
	}

}

static void xng_ttf_loca_table_print( void* ttf_table_data, uint16_t numGlyphs, int16_t indexToLocFormat)
{
	int i;
	//xng_ttf_loca_table * loca_table = (xng_ttf_loca_table*)ttf_table_data;

	if( indexToLocFormat == 0 )
	{
		uint16_t * offsets = (uint16_t*)(ttf_table_data);

		//add 1 for the "EXTRA" glyph
		for( i=0; i<numGlyphs+1; i++)
		{

			printf("\nGlyph %d Offset = %u\n", i, (uint32_t)(offsets[i]) * 2);
			//offsets[i] = xnz_be16toh(offsets[i]);
		}

	}
	else if( indexToLocFormat == 1)
	{
		uint32_t * offsets = (uint32_t*)(ttf_table_data);
		
		//add 1 for the "EXTRA" glyph
		for( i=0; i<numGlyphs+1; i++)
		{
			printf("\nGlyph %d Offset = %u\n", i, offsets[i]);
			//offsets[i] = xnz_be32toh(offsets[i]);
		}
	}
	else
		printf("\nxng_ttf_loca_table_print Error:  Undefined indexToLocFormat (%hd)\n", indexToLocFormat);

}

static void xng_ttf_name_table_print(void * ttf_data)
{
	xng_ttf_name_table_header_print(ttf_data);
	printf("\n");
	xng_ttf_name_records_print(ttf_data);
    
}


#endif 
