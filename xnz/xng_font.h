#ifndef _XNG_FONT_H_
#define _XNG_FONT_H_


/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

 /* If we are we on Views, we want a single define for it.
  */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
#define _WIN32
#endif /* _WIN32 */


#if defined(XNG_FONT_DLL) && defined(_XNG_FONT_BUILD_DLL)
  /* CRGC_DLL must be defined by applications that are linking against the DLL
   * version of the CRGC library.  _CRGC_BUILD_DLL is defined by the CRGC
   * configuration header when compiling the DLL version of the library.
   */
#error "You may not have both XNG_FONT_DLL and _XNG_FONT_BUILD_DLL defined"
#endif

   /* XNG_FONT_API is used to declare public API functions for export
	* from the DLL / shared library / dynamic library.
	*/
#if defined(_WIN32) && defined(_XNG_FONT_BUILD_DLL)
	/* We are building CRPrimitives as a Win32 DLL */
#define XNG_FONT_API __declspec(dllexport)
#elif defined(_WIN32) && defined(XNG_FONT_DLL)
	/* We are calling CRPrimitives as a Win32 DLL */
#define XNG_FONT_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(_XNG_FONT_BUILD_DLL)
	/* We are building XNG_FONT as a shared / dynamic library */
#define XNG_FONT_API __attribute__((visibility("default")))
#else
	/* We are building or calling CRPrimitives as a static library */
#define XNG_FONT_API extern
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_FONT_INLINE
#ifdef _WIN32
#define XNG_FONT_INLINE __inline
#else
#define XNG_FONT_INLINE
#endif
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_FONT_DECLSPEC
#ifdef _WIN32
#define XNG_FONT_DECLSPEC __declspec
#else
#define XNG_FONT_DECLSPEC
#endif
#endif

//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_FONT_ALIGN
#ifdef _WIN32
#define XNG_FONT_ALIGN(X) (align(X))
#else
#define XNG_FONT_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif

#ifndef XNG_FONT_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_FONT_PACK_ATTRIBUTE
#pragma pack(push, 1)
#else
#define XNG_FONT_PACK_ATTRIBUTE __attribute__ ((packed))
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

//#include "crUtils/cr_byte_utils.h"
//#include "crUtils/cr_file_utils.h"
//#include <math.h>
#include "crMath.h"
#include "xng_ttf_font.h"

typedef enum xng_font_parse_status
{
	xng_font_status_ok = 0,				 // No error

	xng_font_status_file_not_found,		 // File was not found during load_file()
	xng_font_status_io_error,			 // Error reading from file/stream
	xng_font_status_out_of_memory,		 // Could not allocate memory
	xng_font_status_internal_error,		 // Internal error occurred

	xng_font_status_unrecognized_tag,	 // Parser could not determine tag type

	xng_font_status_bad_pi,				 // Parsing error occurred while parsing document declaration/processing instruction
	xng_font_status_bad_comment,	     // Parsing error occurred while parsing comment
	xng_font_status_bad_cdata,			 // Parsing error occurred while parsing CDATA section
	xng_font_status_bad_doctype,		 // Parsing error occurred while parsing document type declaration
	xng_font_status_bad_pcdata,			 // Parsing error occurred while parsing PCDATA section
	xng_font_status_bad_start_element,	 // Parsing error occurred while parsing start element tag
	xng_font_status_bad_attribute,		 // Parsing error occurred while parsing element attribute
	xng_font_status_bad_end_element,	 // Parsing error occurred while parsing end element tag
	xng_font_status_end_element_mismatch,// There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag)

	xng_font_status_append_invalid_root, // Unable to append nodes since root type is not node_element or node_document (exclusive to xml_node::append_buffer)

	xng_font_status_no_document_element	 // Parsing resulted in a document without element nodes
}xng_font_parse_status;

typedef enum XNG_FONT_FORMAT
{
	XNG_FONT_FORMAT_TTF = 0,
	XNG_FONT_FORMAT_OTF = 1

}XNG_FONT_FORMAT;


/*
The scaler type
The scaler type is used by OS X and iOS to determine which scaler to use for this font, that is, to determine how to extract glyph data from the font. Different font scalers wrap different font formats within the basic structure of a TrueType font; the scaler type in the offset subtable of the font's directory is used to indicate which scaler should be used with a particular font. (Non-TrueType fonts housed within the same structure as a TrueType font are referred to as "sfnt-housed fonts.")
The values 'true' (0x74727565) and 0x00010000 are recognized by OS X and iOS as referring to TrueType fonts. The value 'typ1' (0x74797031) is recognized as referring to the old style of PostScript font housed in a sfnt wrapper. The value 'OTTO' (0x4F54544F) indicates an OpenType font with PostScript outlines (that is, a 'CFF ' table instead of a 'glyf' table). Other values are not currently supported.
Fonts with TrueType outlines produced for OS X or iOS only are encouraged to use 'true' (0x74727565) for the scaler type value. Fonts for Windows or Adobe products must use 0x00010000.
*/

typedef struct xng_font_atlas
{
	float    fontSize;
	
	uint16_t w, h;	  // total atlas image size
	uint16_t mw, mh;  // max glyph dimensions

	uint16_t glyphIndices[128];
	uint32_t glyphOffsets[128];

	//a cache for lookup data for each font character in the font atlas
	struct character_info 
	{
		float ax; // advance.x
		float ay; // advance.y

		float bw; // bitmap.width;
		float bh; // bitmap.rows;

		float bl; // bitmap left side bearing
		float bt; // bitmap_top  top side bearing
		float bb; // bitmap bottom side bearing

		float tx; // x offset of glyph in texture coordinates
		float ty; // x offset of glyph in texture coordinates

	} c[128];

	float    minTopSideBearing;
	float    ascent, descent;

	float*   accumulation_buffer;
	uint8_t* bitmap_buffer;

}xng_font_atlas;

typedef struct xng_font
{
	union {
		struct {
			union {
				xng_ttf_directory_table* directory_table;
				void* buffer;
			};
			size_t size;
		};
		CRFile file;
	};


	//a single ttf font directory table
	//xng_ttf_directory_table directory_table;

	//a malloc allocated array of ttf offset tables
	xng_ttf_offset_table* offset_tables;

	//a malloc allocated array of bytes containing all ttf file data after offset_tables
	unsigned char*		  table_data;

	xng_ttf_head_table* head_table;
	//these are the tables that need to be referenced for use
	//after the file is initially serialized, in order to read and render glyfs appropriately
	//they are pointers to locations in table_data
	//void * hhea_table;
	xng_ttf_hhea_table* hhea_table;
	xng_ttf_vhea_table* vhea_table;

	void* hmtx_table;
	void* vmtx_table;
	xng_ttf_maxp_table* maxp_table;
	void* cmap_table;
	void* loca_table;
	void* glyf_table;

	//store ttf font glyf metadata needed later as we read and render glyfs
	uint16_t unitsPerEm;
	int16_t  indexToLocFormat; //describes whether loca table glyf offsets are stored as uint16_t (0) or uint32_t (1)
	uint16_t numGlyphs;		   //describes the number of glyf entries (both simple and compound) in the ttf glyf table
	uint16_t numLongHorMetrics;
	uint16_t numLongVerMetrics;
	uint16_t numTables;		   //copy of directory table numtables unserialized

	//store some platform metadata
	xnz_float2 display_dimensions; //must populate this field to initialize font properly
	cr_ulong2  display_resolution; //must populate this field to initialize font properly

	//store some font global metadata
	XNG_FONT_FORMAT	sourceFormat;
	//unsigned long sourceFileSize;

}xng_font;



XNG_FONT_API XNG_FONT_INLINE xng_font_parse_status xng_font_open(xng_font* font, const char* fontPath);

//Internal Public API
XNG_FONT_API XNG_FONT_INLINE void				   xng_font_load_glyph(xng_font* font, xng_ttf_glyph* glyph, unsigned long char_code, float font_size, float* dpi);
//XNG_FONT_API XNG_FONT_INLINE void				   xng_font_draw_glyph_bitmap_for_glyph_index(xng_font* font, xng_ttf_glyph* glyph, uint16_t glyphIndex, float font_size, float* dpi, float** accumulation_buffer, uint8_t** bitmap_buffer);
XNG_FONT_API XNG_FONT_INLINE void				   xng_font_draw_glyph_bitmap_for_char_code(xng_font* font, xng_ttf_glyph* glyph, unsigned long char_code, float font_size, float* dpi, float** accumulation_buffer);// uint8_t** bitmap_buffer);
XNG_FONT_API XNG_FONT_INLINE void*				   xng_font_get_ttf_table(xng_font* font, uint32_t table_id, xng_ttf_offset_table** offset_table_ptr);

//External Public API
XNG_FONT_API XNG_FONT_INLINE void				   xng_prepare_font_atlas(xng_font* font, xng_font_atlas* atlas);
XNG_FONT_API XNG_FONT_INLINE void				   xng_load_font_atlas(xng_font* font, xng_font_atlas* atlas, uint8_t** bitmap_buffer);


XNG_FONT_API XNG_FONT_INLINE xng_font_parse_status xng_font_close(xng_font* font);	


#ifdef __cplusplus
}
#endif

#endif //_XNG_FONT_H_
