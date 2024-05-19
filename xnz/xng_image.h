#ifndef _XNG_IMAGE_H_
#define _XNG_IMAGE_H_


/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

/* If we are we on Views, we want a single define for it.
 */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
 #define _WIN32
#endif /* _WIN32 */


#if defined(XNG_IMAGE_DLL) && defined(_XNG_IMAGE_BUILD_DLL)
 /* CRGC_DLL must be defined by applications that are linking against the DLL
  * version of the CRGC library.  _CRGC_BUILD_DLL is defined by the CRGC
  * configuration header when compiling the DLL version of the library.
  */
 #error "You may not have both XNG_IMAGE_DLL and _XNG_IMAGE_BUILD_DLL defined"
#endif

/* XNG_IMAGE_API is used to declare public API functions for export
 * from the DLL / shared library / dynamic library.
 */
#if defined(_WIN32) && defined(_XNG_IMAGE_BUILD_DLL)
 /* We are building CRPrimitives as a Win32 DLL */
 #define XNG_IMAGE_API __declspec(dllexport)
#elif defined(_WIN32) && defined(XNG_IMAGE_DLL)
 /* We are calling CRPrimitives as a Win32 DLL */
 #define XNG_IMAGE_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(_XNG_IMAGE_BUILD_DLL)
 /* We are building XNG_IMAGE as a shared / dynamic library */
 #define XNG_IMAGE_API __attribute__((visibility("default")))
#else
 /* We are building or calling CRPrimitives as a static library */
 #define XNG_IMAGE_API extern
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_IMAGE_INLINE
#ifdef _WIN32
#define XNG_IMAGE_INLINE __inline
#else
#define XNG_IMAGE_INLINE
#endif
#endif
    
//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_IMAGE_DECLSPEC
#ifdef _WIN32
#define XNG_IMAGE_DECLSPEC __declspec
#else
#define XNG_IMAGE_DECLSPEC
#endif
#endif
    
//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_IMAGE_ALIGN
#ifdef _WIN32
#define XNG_IMAGE_ALIGN(X) (align(X))
#else
#define XNG_IMAGE_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif
    
#ifndef XNG_IMAGE_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_IMAGE_PACK_ATTRIBUTE
#pragma pack(push, 1)
#else
#define XNG_IMAGE_PACK_ATTRIBUTE __attribute__ ((packed))
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

//#include "../crMesh.h"
//#include "crMath.h"
//#include "xnz.h"

//#include <math.h>
#include <limits.h>
#include "xng_image_buffer.h"

typedef enum XNG_IMAGE_FORMAT
{
    XNG_IMAGE_FORMAT_BYTES				    = 0,
    XNG_IMAGE_FORMAT_BMP				    = 1,
    XNG_IMAGE_FORMAT_DDS                    = 2,
    XNG_IMAGE_FORMAT_FPF                    = 3,
    XNG_IMAGE_FORMAT_PNG				    = 4,
    XNG_IMAGE_FORMAT_SGI			        = 5,
}XNG_IMAGE_FORMAT;

typedef struct xng_image
{
    unsigned int width, height, depth, numChannels, size;
    unsigned int pixelInternalFormat, pixelFormat, pixelType;
    unsigned int mtlIndex, numMipMaps, mipMapSize;
    XNG_IMAGE_FORMAT format;
    XNG_IMAGE_API_STATUS status;
    unsigned char * pixelData;
    char * filepath;
    char * extension;
}xng_image;


typedef struct xng_binary_node
{
    xnz_uint4 rect;
    struct xng_binary_node* child[2];
    bool filled; // true if this node is a leaf and is filled
    //int imageID
    int index;
}xng_binary_node;


static xng_binary_node* xng_binary_node_pack(xng_binary_node* pnode, xnz_uint2* texture_atlas_size, xnz_uint2* bitmap_size, xng_binary_node* nodes_vector, uint32_t* node_vector_index)
{
    xng_binary_node* retNode;
    if (pnode->filled) {
        // The node is filled, not gonna fit anything else here
        assert(!pnode->child[0] && !pnode->child[1]);
        return NULL;
    }
    else if (pnode->child[0] && pnode->child[1]) {
        // Non-leaf, try inserting to the left and then to the right
        retNode = xng_binary_node_pack(pnode->child[0], texture_atlas_size, bitmap_size, nodes_vector, node_vector_index);
        if (retNode != NULL) {
            return retNode;
        }
        return xng_binary_node_pack(pnode->child[1], texture_atlas_size, bitmap_size, nodes_vector, node_vector_index);
    }
    else {
        // This is an unfilled leaf - let's see if we can fill it
        xnz_uint2 realSize;
        realSize.x = pnode->rect.size.x;
        realSize.y = pnode->rect.size.y;

        // If we're along a boundary, calculate the actual size
        if (pnode->rect.origin.x + pnode->rect.size.x == INT_MAX) {
            realSize.x = texture_atlas_size->x - pnode->rect.origin.x;
        }
        if (pnode->rect.origin.y + pnode->rect.size.y == INT_MAX) {
            realSize.y = texture_atlas_size->y - pnode->rect.origin.y;
        }

        if (pnode->rect.size.x == bitmap_size->x && pnode->rect.size.y == bitmap_size->y) {
            // Perfect size - just pack into this node
            pnode->filled = true;
            return pnode;
        }
        else if (realSize.x < bitmap_size->x || realSize.y < bitmap_size->y) {
            // Not big enough
            return NULL;
        }
        else {
            // Large enough - split until we get a perfect fit
            //TextureNode* left,
            //TextureNode* right;
            bool verticalSplit;
            int remainX, remainY;
            // Determine how much space we'll have left if we split each way
            remainX = realSize.x - bitmap_size->x;
            remainY = realSize.y - bitmap_size->y;

            //(otherwise, gotta split this node and create some kids)
            pnode->child[0] = &(nodes_vector[(*node_vector_index)++]);
            pnode->child[1] = &(nodes_vector[(*node_vector_index)++]);

            // pnod

            // Split the way that will leave the most room
            verticalSplit = remainX < remainY;
            if (remainX == 0 && remainY == 0) {
                // Edge case - we are are going to hit the border of
                // the texture atlas perfectly, split at the border instead
                if (pnode->rect.size.x > pnode->rect.size.y) {
                    verticalSplit = false;
                }
                else {
                    verticalSplit = true;
                }
            }

            if (verticalSplit) {
                // Split vertically (left is top)
                //left = new TextureNode(node->origin, glm::ivec2(node->size.x, size.y);
                //right = new TextureNode(glm::ivec2(node->origin.x, node->origin.y + size.y), glm::ivec2(node->size.x, node->size.y - size.y));

                pnode->child[0]->rect.origin = pnode->rect.origin;
                pnode->child[0]->rect.size.x = pnode->rect.size.x;
                pnode->child[0]->rect.size.y = bitmap_size->y;

                pnode->child[1]->rect.origin.x = pnode->rect.origin.x;
                pnode->child[1]->rect.origin.y = pnode->rect.origin.y + bitmap_size->y;
                pnode->child[1]->rect.size.x = pnode->rect.size.x;
                pnode->child[1]->rect.size.y = pnode->rect.size.y - bitmap_size->y;
            }
            else
            {
                // Split horizontally
                //left = new TextureNode(node->origin, glm::ivec2(size.x, node->size.y);
                //right = new TextureNode(glm::ivec2(node->origin.x + size.x, node->origin.y), glm::ivec2(node->size.x - size.x, node->size.y));

                pnode->child[0]->rect.origin = pnode->rect.origin;
                pnode->child[0]->rect.size.x = bitmap_size->x;
                pnode->child[0]->rect.size.y = pnode->rect.size.y;

                pnode->child[1]->rect.origin.x = pnode->rect.origin.x + bitmap_size->x;
                pnode->child[1]->rect.origin.y = pnode->rect.origin.y;
                pnode->child[1]->rect.size.x = pnode->rect.size.x - bitmap_size->x;
                pnode->child[1]->rect.size.y = pnode->rect.size.y;
            }

            //node->left = std::unique_ptr<TextureNode>(left);
            //node->right = std::unique_ptr<TextureNode>(right);
            return xng_binary_node_pack(pnode->child[0], texture_atlas_size, bitmap_size, nodes_vector, node_vector_index);
        }
    }
}

XNG_IMAGE_API XNG_IMAGE_INLINE void          xng_print_image_details(xng_image * img);

//compute image size
XNG_IMAGE_API unsigned int                   xng_image_compute_pixel_format_components(unsigned int pixelFormat);
XNG_IMAGE_API XNG_IMAGE_INLINE unsigned int  xng_image_compute_pixel_size_in_bits(unsigned int format,unsigned int type);
XNG_IMAGE_API unsigned int                   xng_image_compute_row_size_in_bytes(int width,unsigned int pixelFormat, unsigned inttype,int packing);
XNG_IMAGE_API XNG_IMAGE_INLINE unsigned int  xng_image_compute_size_in_bytes ( int width, int height, int depth,
                                                                          unsigned int pixelFormat, unsigned int pixelType,
                                                                          int packing, int slice_packing, int image_packing);
XNG_IMAGE_API XNG_IMAGE_INLINE unsigned int  xng_image_compute_size( xng_image * img);


//Image creation

//Image Create with Source File Constructors
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image     xng_load_image(const char * filepath);					//automatically uncompress compressed formats
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image     xng_load_image_flipped(const char * filepath);         //unused, TO DO: xnz not implemented
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image     xng_load_image_compressed(const char * filepath);		//leave compressed formats compressed (TO DO: xnz not implemented)

XNG_IMAGE_API XNG_IMAGE_INLINE xng_image     xng_copy_image(xng_image* img);                        //create new memory and copy image params, path and pixel data

//Image Init with Source Format Constructors
XNG_IMAGE_API XNG_IMAGE_INLINE void          xng_load_dds_image_uncompressed(xng_image* img);
XNG_IMAGE_API XNG_IMAGE_INLINE void          xng_load_png_image(xng_image * img);
XNG_IMAGE_API XNG_IMAGE_INLINE void          xng_load_sgi_image(xng_image* img);

XNG_IMAGE_API XNG_IMAGE_INLINE void          xng_load_ttf_image(xng_image* img);


//Image Create with Buffer Convenience Constructors
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image     xng_image_buffer_create(unsigned int width, unsigned int height, unsigned int numChannels, unsigned int depth, XNG_IMAGE_FORMAT imageFormat);
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image     xng_image_with_color(unsigned int width, unsigned int height, unsigned int bytesPerPixel, unsigned char color[4]);
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image     xng_image_with_checkerboard(unsigned int width, unsigned int height, unsigned int bytesPerPixel);

//Resize Constructor
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image     xng_image_load_resize(const char* filepath, float width_percent, float height_percent /*stbir_filter filter, stbir_edge edge, stbir_colorspace colorspace, const char* output_filename*/);

//Draw Methods (really just a copy from one subimage to another)
XNG_IMAGE_API XNG_IMAGE_INLINE void          xng_image_draw_image_in_rect(xng_image* dst, xng_image* src, xnz_uint4* rect);

//When do I use this??
XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS xng_image_export_to_source_format(xng_image * image, const char * filepath);

//Cleanup
XNG_IMAGE_API XNG_IMAGE_INLINE void          xng_image_free(xng_image* img);


// *** Public API Functions *** //

XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS  xng_image_draw_uint_pixels(xng_image * image, xnz_uint2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed *color_value);
XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS  xng_image_draw_float_pixels(xng_image* image, xnz_float2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed color_value);

XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS  xng_image_draw_circles(xng_image* image, xnz_uint2* circle_centers, unsigned int numCircles, unsigned int radius, xnz_byte4_packed* color_value);
XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS  xng_image_draw_circles_uint16(xng_image * image, uint16_t* circle_center_x_values, uint16_t* circle_center_y_values, unsigned int numCircles, unsigned int radius, xnz_byte4_packed *color_value);


#ifdef __cplusplus
}
#endif

#endif //_XNG_IMAGE_H_
