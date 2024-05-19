#include "../xng.h"
//#include "xng_image.h"
//#include "xnz_png_image.h"
//#include "xnz_dds_image.h"
#include "crUtils/cr_file_utils.h"
#include "crUtils/cr_byte_utils.h"

#include <stdio.h>      // printf


//TO DO: REMOVE WOKE STB
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "stbi_DDS_aug.h"
//#include "stbi_DDS_aug_c.h"

#define STBIR_NO_SIMD
#define STBIR_PROGRESS_REPORT stbir_progress
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_STATIC
#include "stb_image_resize2.h"


XNG_IMAGE_API XNG_IMAGE_INLINE void xng_print_image_details(xng_image * img)
{
    if( img )
    {
        fprintf(stderr, "\n***xng_image->filepath = %s\n", img->filepath);
        fprintf(stderr, "\n***xng_image->filepath = %s\n", img->filepath);
        fprintf(stderr, "***xng_image->width = %d\n", img->width);
        fprintf(stderr, "***xng_image->height = %d\n", img->height);
        fprintf(stderr, "***xng_image->depth = %d\n", img->depth);
        fprintf(stderr, "***xng_image->numChannels = %d\n", img->numChannels);
        fprintf(stderr, "***xng_image->size = %d\n", img->size);
    
        if( img->pixelData ) fprintf(stderr, "***xng_image->pixelData = VALID MEMORY\n");
        else                 fprintf(stderr, "***xng_image->pixelData = NULL\n");
        
    }
    else fprintf(stderr, "xng_image_print_details ::  Unable to print image.  Image context is NULL\n");
}

XNG_IMAGE_API XNG_IMAGE_INLINE unsigned int xng_image_compute_size( xng_image * img)
{
    return xng_image_compute_size_in_bytes(img->width, img->height, img->depth, img->pixelFormat, img->pixelType, 1, 1, 1);
}

XNG_IMAGE_API XNG_IMAGE_INLINE unsigned int xng_image_compute_size_in_bytes
    ( int width, int height, int depth,
      unsigned int pixelFormat, unsigned int pixelType,
      int packing, int slice_packing, int image_packing)
{
    unsigned int size;

    if( width < 1 )  width = 1;
    if( height < 1 ) height = 1;
    if( depth < 1 )  depth = 1;

    if( pixelFormat >= XNG_COMPRESSED_RGB_S3TC_DXT1_EXT &&
        pixelFormat <= XNG_COMPRESSED_RGBA_S3TC_DXT5_EXT )
    {
        width = (width + 3) & ~3;
        height = (height + 3) & ~3;
    }
    if( pixelFormat >= XNG_COMPRESSED_RED_RGTC1_EXT &&
        pixelFormat <= XNG_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT )
    {
        width = (width + 3) & ~3;
        height = (height + 3) & ~3;
    }
    // compute size of one row
    size = xng_image_compute_row_size_in_bytes( width, pixelFormat, pixelType, packing );

    //printf("\nrow size:   %d\n", size);

    // now compute size of slice
    size *= height;
    size += slice_packing - 1;
    size -= size % slice_packing;

    // compute size of whole image
    size *= depth;
    size += image_packing - 1;
    size -= size % image_packing;

    return size;
}

XNG_IMAGE_API unsigned int xng_image_compute_pixel_format_components(unsigned int pixelFormat)
{
    switch(pixelFormat)
    {
        case(XNG_COMPRESSED_RGB_S3TC_DXT1_EXT): return 3;
        case(XNG_COMPRESSED_RGBA_S3TC_DXT1_EXT): return 4;
        case(XNG_COMPRESSED_RGBA_S3TC_DXT3_EXT): return 4;
        case(XNG_COMPRESSED_RGBA_S3TC_DXT5_EXT): return 4;
        case(XNG_COMPRESSED_SIGNED_RED_RGTC1_EXT): return 1;
        case(XNG_COMPRESSED_RED_RGTC1_EXT):   return 1;
        case(XNG_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT): return 2;
        case(XNG_COMPRESSED_RED_GREEN_RGTC2_EXT): return 2;    
        case(XNG_COLOR_INDEX): return 1;
        case(XNG_STENCIL_INDEX): return 1;
        case(XNG_DEPTH_COMPONENT): return 1;
        case(XNG_RED): return 1;
        case(XNG_GREEN): return 1;
        case(XNG_BLUE): return 1;
        case(XNG_ALPHA): return 1;
        case(XNG_ALPHA8I_EXT): return 1;
        case(XNG_ALPHA8UI_EXT): return 1;
        case(XNG_ALPHA16I_EXT): return 1;
        case(XNG_ALPHA16UI_EXT): return 1;
        case(XNG_ALPHA32I_EXT): return 1;
        case(XNG_ALPHA32UI_EXT): return 1;
        case(XNG_ALPHA16F_ARB): return 1;
        case(XNG_ALPHA32F_ARB): return 1;
        case(XNG_RGB): return 3;
        case(XNG_BGR): return 3;
        case(XNG_RGB8I_EXT): return 3;
        case(XNG_RGB8UI_EXT): return 3;
        case(XNG_RGB16I_EXT): return 3;
        case(XNG_RGB16UI_EXT): return 3;
        case(XNG_RGB32I_EXT): return 3;
        case(XNG_RGB32UI_EXT): return 3;
        case(XNG_RGB16F_ARB): return 3;
        case(XNG_RGB32F_ARB): return 3;
        case(XNG_RGBA16F_ARB): return 4;
        case(XNG_RGBA32F_ARB): return 4;
        case(XNG_RGBA): return 4;
        case(XNG_BGRA): return 4;
        case(XNG_RGBA8): return 4;
        case(XNG_LUMINANCE): return 1;
        case(XNG_LUMINANCE4): return 1;
        case(XNG_LUMINANCE8): return 1;
        case(XNG_LUMINANCE12): return 1;
        case(XNG_LUMINANCE16): return 1;
        case(XNG_LUMINANCE8I_EXT): return 1;
        case(XNG_LUMINANCE8UI_EXT): return 1;
        case(XNG_LUMINANCE16I_EXT): return 1;
        case(XNG_LUMINANCE16UI_EXT): return 1;
        case(XNG_LUMINANCE32I_EXT): return 1;
        case(XNG_LUMINANCE32UI_EXT): return 1;
        case(XNG_LUMINANCE16F_ARB): return 1;
        case(XNG_LUMINANCE32F_ARB): return 1;
        case(XNG_LUMINANCE4_ALPHA4): return 2;
        case(XNG_LUMINANCE6_ALPHA2): return 2;
        case(XNG_LUMINANCE8_ALPHA8): return 2;
        case(XNG_LUMINANCE12_ALPHA4): return 2;
        case(XNG_LUMINANCE12_ALPHA12): return 2;
        case(XNG_LUMINANCE16_ALPHA16): return 2;
        case(XNG_INTENSITY): return 1;
        case(XNG_INTENSITY4): return 1;
        case(XNG_INTENSITY8): return 1;
        case(XNG_INTENSITY12): return 1;
        case(XNG_INTENSITY16): return 1;
        case(XNG_INTENSITY8UI_EXT): return 1;
        case(XNG_INTENSITY8I_EXT): return 1;
        case(XNG_INTENSITY16I_EXT): return 1;
        case(XNG_INTENSITY16UI_EXT): return 1;
        case(XNG_INTENSITY32I_EXT): return 1;
        case(XNG_INTENSITY32UI_EXT): return 1;
        case(XNG_INTENSITY16F_ARB): return 1;
        case(XNG_INTENSITY32F_ARB): return 1;
        case(XNG_LUMINANCE_ALPHA): return 2;
        case(XNG_LUMINANCE_ALPHA8I_EXT): return 2;
        case(XNG_LUMINANCE_ALPHA8UI_EXT): return 2;
        case(XNG_LUMINANCE_ALPHA16I_EXT): return 2;
        case(XNG_LUMINANCE_ALPHA16UI_EXT): return 2;
        case(XNG_LUMINANCE_ALPHA32I_EXT): return 2;
        case(XNG_LUMINANCE_ALPHA32UI_EXT): return 2;
        case(XNG_LUMINANCE_ALPHA16F_ARB): return 2;
        case(XNG_LUMINANCE_ALPHA32F_ARB): return 2;
        case(XNG_HILO_NV): return 2;
        case(XNG_DSDT_NV): return 2;
        case(XNG_DSDT_MAG_NV): return 3;
        case(XNG_DSDT_MAG_VIB_NV): return 4;
        case(XNG_RED_INTEGER_EXT): return 1;
        case(XNG_GREEN_INTEGER_EXT): return 1;
        case(XNG_BLUE_INTEGER_EXT): return 1;
        case(XNG_ALPHA_INTEGER_EXT): return 1;
        case(XNG_RGB_INTEGER_EXT): return 3;
        case(XNG_RGBA_INTEGER_EXT): return 4;
        case(XNG_BGR_INTEGER_EXT): return 3;
        case(XNG_BGRA_INTEGER_EXT): return 4;
        case(XNG_LUMINANCE_INTEGER_EXT): return 1;
        case(XNG_LUMINANCE_ALPHA_INTEGER_EXT): return 2;

        default:
        {
            printf("\nerror pixelFormat = 0x%08x\n", pixelFormat);
            return 0;
        }
    }        
}

XNG_IMAGE_API XNG_IMAGE_INLINE unsigned int xng_image_compute_pixel_size_in_bits(unsigned int format,unsigned int type)
{

    switch(format)
    {
        case(XNG_COMPRESSED_RGB_S3TC_DXT1_EXT): return 4;
        case(XNG_COMPRESSED_RGBA_S3TC_DXT1_EXT): return 4;
        case(XNG_COMPRESSED_RGBA_S3TC_DXT3_EXT): return 8;
        case(XNG_COMPRESSED_RGBA_S3TC_DXT5_EXT): return 8;
        case(XNG_COMPRESSED_SIGNED_RED_RGTC1_EXT): return 4;
        case(XNG_COMPRESSED_RED_RGTC1_EXT):   return 4;
        case(XNG_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT): return 8;
        case(XNG_COMPRESSED_RED_GREEN_RGTC2_EXT): return 8;
        default: break;
    }
    switch(format)
    {
        case(XNG_COMPRESSED_ALPHA):
        case(XNG_COMPRESSED_LUMINANCE):
        case(XNG_COMPRESSED_LUMINANCE_ALPHA):
        case(XNG_COMPRESSED_INTENSITY):
        case(XNG_COMPRESSED_RGB):
        case(XNG_COMPRESSED_RGBA):
            printf("\nImage::computePixelSizeInBits(format,type) : cannot compute correct size of compressed format (%d) returning 0.\n", format);
            return 0;
        default: break;
    }

    switch(format)
    {
        case(XNG_LUMINANCE4): return 4;
        case(XNG_LUMINANCE8): return 8;
        case(XNG_LUMINANCE12): return 12;
        case(XNG_LUMINANCE16): return 16;
        case(XNG_LUMINANCE4_ALPHA4): return 8;
        case(XNG_LUMINANCE6_ALPHA2): return 8;
        case(XNG_LUMINANCE8_ALPHA8): return 16;
        case(XNG_LUMINANCE12_ALPHA4): return 16;
        case(XNG_LUMINANCE12_ALPHA12): return 24;
        case(XNG_LUMINANCE16_ALPHA16): return 32;
        case(XNG_INTENSITY4): return 4;
        case(XNG_INTENSITY8): return 8;
        case(XNG_INTENSITY12): return 12;
        case(XNG_INTENSITY16): return 16;
        default: break;
    }

    switch(type)
    {
   
        case(XNG_BITMAP): return xng_image_compute_pixel_format_components(format);
        
        case(XNG_BYTE):
        case(XNG_UNSIGNED_BYTE): return 8*xng_image_compute_pixel_format_components(format);
        
        case(XNG_HALF_FLOAT_NV):
        case(XNG_SHORT):
        case(XNG_UNSIGNED_SHORT): return 16*xng_image_compute_pixel_format_components(format);
        
        case(XNG_INT):
        case(XNG_UNSIGNED_INT):
        case(XNG_FLOAT): return 32*xng_image_compute_pixel_format_components(format);
    
    
        case(XNG_UNSIGNED_BYTE_3_3_2): 
        case(XNG_UNSIGNED_BYTE_2_3_3_REV): return 8;
        
        case(XNG_UNSIGNED_SHORT_5_6_5):
        case(XNG_UNSIGNED_SHORT_5_6_5_REV):
        case(XNG_UNSIGNED_SHORT_4_4_4_4):
        case(XNG_UNSIGNED_SHORT_4_4_4_4_REV):
        case(XNG_UNSIGNED_SHORT_5_5_5_1):
        case(XNG_UNSIGNED_SHORT_1_5_5_5_REV): return 16;
        
        case(XNG_UNSIGNED_INT_8_8_8_8):
        case(XNG_UNSIGNED_INT_8_8_8_8_REV):
        case(XNG_UNSIGNED_INT_10_10_10_2):
        case(XNG_UNSIGNED_INT_2_10_10_10_REV): return 32;
        default: 
        {
            printf("\nerror type = \n");
            return 0;
        }
    }    

}




XNG_IMAGE_API unsigned int xng_image_compute_row_size_in_bytes(int width,unsigned int pixelFormat, unsigned int type,int packing)
{
    unsigned int pixelSize = xng_image_compute_pixel_size_in_bits(pixelFormat,type);
    int widthInBits = width*pixelSize;
    int packingInBits = packing*8;
    return (widthInBits/packingInBits + ((widthInBits%packingInBits)?1:0))*packing;
}

XNG_IMAGE_API XNG_IMAGE_INLINE void xng_image_free( xng_image *img )
{
    if( img->filepath ) free(img->filepath);
    img->filepath = NULL;

    if( img->pixelData ) stbi_image_free( img->pixelData );
    img->pixelData = NULL; //Wow, lack of this was causing error fopr duplicate textures
}

XNG_IMAGE_API XNG_IMAGE_INLINE void xng_image_draw_image_in_rect(xng_image* dst, xng_image* src, xnz_uint4 * rect)
{
    assert(src->width < dst->width+1);
    assert(src->height < dst->height+1);
    
    assert(rect->size.x == src->width);
    assert(rect->size.y == src->height);

    //image.pixelData = (unsigned char*)malloc(textureSize);

    //uint64_t nPixels = rect.size.x * rect.size.y;
    uint64_t dstOffset     = rect->origin.y * dst->width * dst->numChannels + rect->origin.x * dst->numChannels;
    unsigned char* pData   = &dst->pixelData[dstOffset];
    unsigned char* srcData = &src->pixelData[0];
    
    assert(srcData);
    //if( src->numChannels < dst->numChannels ) memset(dst->pixelData, 255, dst->width * dst->height * dst->numChannels);
    
    uint32_t nSrcPixels, srcRow, srcCol;
    
    for (srcRow = 0; srcRow < src->height; srcRow++)
    {
        for(srcCol = 0; srcCol < src->width; srcCol++)
        {
            memcpy(pData, srcData, dst->numChannels);
            
            pData+=dst->numChannels;
            srcData+=dst->numChannels;
        }
        
        //increment the dst ptr by the difference in row size between the dst and src images
        pData += (dst->width - src->width) * dst->numChannels;
    }
}
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image xng_image_with_color(unsigned int width, unsigned int height, unsigned int bytesPerPixel, unsigned char color[4])
{
    xng_image image = { 0 };

    const uint32_t rowPitch = width * bytesPerPixel;
    const uint32_t textureSize = rowPitch * height;

    image.pixelData = (unsigned char*)malloc(textureSize);
    unsigned char* pData = &image.pixelData[0];

    for (uint32_t n = 0; n < textureSize; n += bytesPerPixel) memcpy(&(pData[n]), color, bytesPerPixel);

    image.width       = width;
    image.height      = height;
    image.numChannels = bytesPerPixel;
    image.depth       = 8;
    image.size        = width * height * bytesPerPixel;

    return image;
}

// Generate a simple black and white checkerboard texture.
XNG_IMAGE_API XNG_IMAGE_INLINE xng_image xng_image_with_checkerboard(unsigned int width, unsigned int height, unsigned int bytesPerPixel)
{
    xng_image image = { 0 };


    const uint32_t rowPitch = width * bytesPerPixel;
    const uint32_t cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
    const uint32_t cellHeight = width >> 3;    // The height of a cell in the checkerboard texture.
    const uint32_t textureSize = rowPitch * height;

    image.pixelData = (unsigned char*)malloc(textureSize);
    memset(image.pixelData, 0, textureSize);
    unsigned char* pData = &image.pixelData[0];

    for (uint32_t n = 0; n < textureSize; n += bytesPerPixel)
    {
        uint32_t x = n % rowPitch;
        uint32_t y = n / rowPitch;
        uint32_t i = x / cellPitch;
        uint32_t j = y / cellHeight;

        if (i % 2 == j % 2)
        {
            pData[n] = 0x00;        // R
            pData[n + 1] = 0x00;    // G
            pData[n + 2] = 0x00;    // B
            pData[n + 3] = 0xff;    // A
        }
        else
        {
            pData[n] = 0xff;        // R
            pData[n + 1] = 0xff;    // G
            pData[n + 2] = 0xff;    // B
            pData[n + 3] = 0xff;    // A
        }
    }

    image.width = width;
    image.height = height;
    image.numChannels = bytesPerPixel;

    return image;
}

//XNG_IMAGE_API XNG_IMAGE_INLINE xng_image_resize(const char* filepath, float width_percent, float height_percent /*,stbir_filter filter, stbir_edge edge*/)// stbir_colorspace colorspace, const char* output_filename)

XNG_IMAGE_API XNG_IMAGE_INLINE xng_image xng_image_load_resize(const char * filepath, float width_percent, float height_percent /*,stbir_filter filter, stbir_edge edge*/)// stbir_colorspace colorspace, const char* output_filename)
{
    //int w, h, n;
;
    xng_image imgResize = {0};
    xng_image img       = xng_load_image(filepath);

    /*
    stbi_convert_iphone_png_to_rgb(1);
    //img->pixelData = stbi_load(img->filepath, (int*)&(img->width), (int*)&(img->height), (int*)&(img->numChannels), STBI_rgb_alpha);
    unsigned char* input_data = stbi_load(filepath, &w, &h, &n, STBI_rgb_alpha);
    if (!input_data)
    {
        printf("Input image could not be loaded\n");
        assert(1==0);
        return img;
    }
    */

    imgResize.filepath = (char*)malloc( (strlen(filepath)+1) * sizeof(char) );
    strcpy(imgResize.filepath, filepath);
    imgResize.filepath[strlen(filepath)] = '\0';

    //find file extension in filepath, point to it
    imgResize.extension = (char *)cr_find_file_extension(img.filepath);
    
    imgResize.width       = (int)((float)img.width  * width_percent);
    imgResize.height      = (int)((float)img.height * height_percent);
    imgResize.numChannels = img.numChannels;
    imgResize.depth       = img.depth;
    imgResize.size        = imgResize.numChannels * imgResize.width * imgResize.height * imgResize.depth / 8;
    imgResize.pixelData   = (unsigned char*)malloc(imgResize.size);
    memset(imgResize.pixelData, 0, imgResize.size);

    //determine pixel format and size because stb_image does not do this internally
         if( img.numChannels == 4 ) img.pixelFormat = XNG_RGBA;
    else if( img.numChannels == 3 ) img.pixelFormat = XNG_RGB;
    else if( img.numChannels == 2 )
    {
        fprintf(stderr, "\n img.numChannels == 2 not yet supported\n");
        assert(1==0);
    }
    else if( img.numChannels == 1) img.pixelFormat = XNG_RED;
    else
    {
        fprintf(stderr, "\n xng_image error:  img.numChannels == 0\n");
        assert(1==0);
    }
    
    imgResize.pixelType = XNG_UNSIGNED_BYTE;
    //img->numChannels = xng_image_compute_pixel_format_components(img->pixelFormat);
    //img.size = xng_image_compute_size(&img);

    imgResize.pixelFormat = img.pixelFormat;
    imgResize.pixelType   = img.pixelType;
    stbi_set_flip_vertically_on_load(false);    
    
    //Resize image to xng_image.pixelData
    //stbir_resize(input_data, w, h, 0, output_data, out_w, out_h, 0, STBIR_TYPE_UINT8, n, STBIR_ALPHA_CHANNEL_NONE, 0, edge, edge, filter, filter, colorspace, &g_context);
    stbir_resize(img.pixelData,       img.width,       img.height,  0,
           imgResize.pixelData, imgResize.width, imgResize.height,  0,
           STBIR_RGBA,          STBIR_TYPE_UINT8,
           STBIR_EDGE_ZERO,     STBIR_FILTER_POINT_SAMPLE);
    
    //stbi_image_free(input_data);
    xng_image_free(&img);
    return imgResize;
}

XNG_IMAGE_API XNG_IMAGE_INLINE xng_image xng_copy_image(xng_image * img)
{
    xng_image imgCopy = {0};
    
    imgCopy = *img; //copy params

    //create memory, copy filepath 
    imgCopy.filepath = (char*)malloc(strlen(img->filepath) + 1);
    strcpy(imgCopy.filepath, img->filepath);

    //create memory, copy pixel data
    imgCopy.pixelData = (char*)malloc(img->size);
    memcpy(imgCopy.pixelData, img->pixelData, img->size);

    return imgCopy;//return copy

}

XNG_IMAGE_API XNG_IMAGE_INLINE xng_image xng_load_image(const char * filepath)
{
    xng_image img = {0};

    //alloc mem for filepath
    img.filepath = (char*)malloc( (strlen(filepath)+1) * sizeof(char) );
    //img.filepath[strlen(filepath)] = '\0';
    strcpy(img.filepath, filepath);
    img.filepath[strlen(filepath)] = '\0';
    
    //find file extension in filepath, point to it
    img.extension = (char *)cr_find_file_extension(img.filepath);    

    //printf("\ndds file extension: %s\n", img.extension);
    //start with some assumptions about an image
    //img.depth = 1;
    //img.pixelFormat = XNG_RGB;
    //img.pixelType = XNG_UNSIGNED_BYTE;

    if( strcmp(img.extension, "png") == 0 || strcmp(img.extension, "PNG") == 0 )    //load PNG
    {
        //printf("\nLoading PNG:    %s\n", filepath);
        xng_load_png_image(&img);
        
    }
    else if (strcmp(img.extension, "ttf") == 0 || strcmp(img.extension, "TTF") == 0)
    {
        xng_load_ttf_image(&img);
    }
    else if( strcmp(img.extension, "jpg") == 0 || strcmp(img.extension, "JPG") == 0 || strcmp(img.extension, "jpeg") == 0 || strcmp(img.extension, "JPEG") == 0 )
    {
        assert(1==0);
        xng_load_png_image(&img);
    }
    else if( strcmp(img.extension, "bmp") == 0 || strcmp(img.extension, "BMP") == 0 )
    {
        //printf("\nLoading BMP:    %s\n", filepath);
        assert(1==0);
        xng_load_png_image(&img);
        
        //img.pixelFormat = XNG_RGB;
        //img.pixelType = XNG_UNSIGNED_BYTE;        
        //img.size = xng_image_compute_size(&img);

    }
    else if( (strcmp(img.extension, "rgb") == 0 || strcmp(img.extension, "RGB") == 0) ||
             (strcmp(img.extension, "rgba") == 0 || strcmp(img.extension, "RGBA") == 0))
    {
        //assert(1==0);
        xng_load_sgi_image(&img);
        
        //img.pixelFormat = XNG_RGB;
        //img.pixelType = XNG_UNSIGNED_BYTE;
        //img.size = xng_image_compute_size(&img);
        
    }
    else if( strcmp(img.extension, "dds") == 0 ||  strcmp(img.extension, "DDS") == 0)
    {
        xng_load_dds_image_uncompressed(&img);   
    }
    else if (strcmp(img.extension, "fpf") == 0 || strcmp(img.extension, "FPF") == 0)
    {
        img.pixelFormat = XNG_BGRA; //force load as bgra
        xng_load_fpf_image(&img);
    }

    //determine pixel format and size because stb_image does not do this internally
    if( img.numChannels  == 4 )
    {
        img.pixelFormat = XNG_RGBA;
    }
    else if( img.numChannels == 3 )
    {
        img.pixelFormat = XNG_RGB;
    }
    else if( img.numChannels == 2 )
    {
        fprintf(stderr, "\n img.numChannels == 2 not yet supported\n");
        assert(1==0);
    }
    else if( img.numChannels == 1)
    {
        img.pixelFormat = XNG_RED;
        //fprintf(stderr, "\n img.numChannels == 1 not yet supported\n");
        //assert(1==0);
    }
    else
    {
        fprintf(stderr, "\n xng_image error:  img.numChannels == 0\n");
        assert(1==0);
    }
    
    img.pixelType = XNG_UNSIGNED_BYTE;
    //img->numChannels = xng_image_compute_pixel_format_components(img->pixelFormat);
    //img.size = xng_image_compute_size(&img);
    
    stbi_set_flip_vertically_on_load(false);

    return img;
}

XNG_IMAGE_API XNG_IMAGE_INLINE xng_image xng_load_image_flipped(const char * filepath)
{
    stbi_set_flip_vertically_on_load(true);
    return xng_load_image(filepath);
}

XNG_IMAGE_API XNG_IMAGE_INLINE xng_image xng_load_image_compressed(const char * filepath)
{
    xng_image img = {0};

    //alloc mem for filepath
    img.filepath = (char*)malloc( (strlen(filepath)+1) * sizeof(char) );
    //img.filepath[strlen(filepath)] = '\0';
    strcpy(img.filepath, filepath);
    img.filepath[strlen(filepath)] = '\0';
    
    //find file extension in filepath, point to it
    img.extension = (char *)cr_find_file_extension(img.filepath);    

    //printf("\ndds file extension: %s\n", img.extension);
    //start with some assumptions about an image
    //img.depth = 1;
    //img.pixelFormat = XNG_RGB;
    //img.pixelType = XNG_UNSIGNED_BYTE;
    /*
    if( strcmp(img.extension, "png") == 0 )    //load PNG
    {
        //printf("\nLoading PNG:    %s\n", filepath);
        
        xng_load_png_image(&img);
        

    }
    else if( strcmp(img.extension, "bmp") == 0 )
    {
        //printf("\nLoading BMP:    %s\n", filepath);
        
        xng_load_png_image(&img);
        
        img.pixelFormat = XNG_RGB;
        img.pixelType = XNG_UNSIGNED_BYTE;
        
        img.size = xng_image_compute_size(&img);

    }
    else */if( strcmp(img.extension, "dds") == 0)
    {
        //printf("\nLoading DDS:    %s\n", filepath);
        xnz_load_dds_image_compressed(&img);
    }

    //determine pixel format and size because stb_image does not do this internally
    /*
    if( img.numChannels  == 4 )
    {
        img.pixelFormat = XNG_RGBA;
    }
    else if( img.numChannels == 3 )
    {
        img.pixelFormat = XNG_RGB;
    }
    else if( img.numChannels == 2 )
    {
        printf("\n img.numChannels == 2 not yet supported\n");
        
    }
    else if( img.numChannels == 1)
    {
        printf("\n img.numChannels == 1 not yet supported\n");
    }
    else
    {
        printf("\n xng_image error:  img.numChannels == 0\n");
    }
    

    img.pixelType = XNG_UNSIGNED_BYTE;
       
    //img.numChannels = xng_image_compute_pixel_format_components(img->pixelFormat);
    img.size = xng_image_compute_size(&img);
    */


    return img;

}

//#pragma mark -- Image Init with Source Format Constructors


XNG_IMAGE_API XNG_IMAGE_INLINE void xng_load_dds_image_uncompressed(xng_image* img)
{
    if (!img)
    {
        printf("\nimage is null\n");
        return;
    }

    assert(1 == 0);

    //printf("\n Image filepath: %s", img->filepath);
    //load and decode png from file
    img->pixelData = stbi_dds_load(img->filepath, (int*)&(img->width), (int*)&(img->height), (int*)&(img->numChannels), STBI_rgb_alpha);

    // OR if you have already read the image file data into a buffer:
    //unsigned char* pixeldata2 = stbi_load_from_memory(bufferWithImageData, bufferLength, &width, &height, &bytesPerPixel, 0);

    //if(pixeldata2 == NULL) printf("Some error happened: %sn", stbi_failure_reason());
}

XNG_IMAGE_API XNG_IMAGE_INLINE void xng_load_ttf_image(xng_image* img)
{
    if (!img) return;

    float  _retinaScale  = 1.f;
    xng_font       font  = {0};
    xng_font_atlas atlas = {72.f * _retinaScale};

    xng_font_open(&font, img->filepath);
    //xng_font_parse_status fontStatus = xng_font_load(&font, img->filepath); assert(fontStatus == xng_font_status_ok);
    
    //for _retina scale displays we render at the retina scale
    //the screen dpi stays the same because it hasn't changed, but our number of logical screen pixels has
    font.display_dimensions = (xnz_float2){ 621., 341. };  //  cr_display_get_dimensions();
    font.display_resolution = (cr_ulong2){ 3840, 2160 };     //backing width and backing height have already incorporated retinaScale factor

    //determine font atlas texture size and glyph uv placements
    xng_prepare_font_atlas(&font, &atlas);

    //create xng_image bitmap pixel buffer for blitting glyphs from the accumulation buffer
    //it will be sized to store all glyphs in an atlas at the given font size
    img->numChannels = 1;
    img->width       = atlas.w;
    img->height      = atlas.h;
    img->depth       = 8;
    img->size        = img->numChannels * img->width * img->height * img->depth / 8;

    //allocate memory for xng_image pixel buffer to return to be returned to client
    size_t accSize    = (size_t)atlas.mw * (size_t)atlas.mh * sizeof(float);
    size_t bitmapSize = img->size; 
    img->pixelData    = (uint8_t*)malloc(bitmapSize); memset(img->pixelData, 0, bitmapSize);
    
    //allocate memory for floating point accumulation buffer for drawing ttf glyphs, place on atlas for draw
    //it only needs to be as large as the max observed glyph dimensions for the given font size
    atlas.accumulation_buffer = (float*)malloc(accSize); memset(atlas.accumulation_buffer, 0, accSize);

    xng_load_font_atlas(&font, &atlas, &img->pixelData);

    xng_font_close(&font);

    free(atlas.accumulation_buffer);
}


XNG_IMAGE_API XNG_IMAGE_INLINE void xng_load_png_image(xng_image * img)
{
    if( !img ) return;

    //load and decode png from file
    //stbi_convert_iphone_png_to_rgb(1);
    //img->pixelData = stbi_load(img->filepath, (int*)&(img->width), (int*)&(img->height), (int*)&(img->numChannels), STBI_rgb_alpha);

    XNZ_PNG_IMAGE archive = {0};
    xnz_png_open(&archive, img->filepath);
    
    //PNG Uses RFC1950 Deflate Wrapped Zlib data, but Apple Compression uses RFC1951 so we push past the zlib header to get raw deflate data

    size_t pngSize = archive.nSrcChannels * archive.rowPitch * archive.nScanlines * archive.depth/8;
    
    img->numChannels = archive.nSrcChannels == 3 ? 4 : archive.nSrcChannels; //1, 2 & 4 channels supported
    img->width       = archive.rowPitch;
    img->height      = archive.nScanlines;
    img->depth       = archive.depth;
    img->size        = img->numChannels * img->width * img->height * img->depth/8;

    //Allocate memory for xng_image pixel buffer to return to be returned to client
    size_t filteredSize = pngSize + archive.rowPitch;                      //assume 1 extra filter byte per scanline
    size_t bufferSize   = MAX(img->size + archive.rowPitch, filteredSize); //Warning: for 3 to 4 channel expansion rowPitch must be added to the dst buffer size as well
    img->pixelData      = (uint8_t*)malloc( bufferSize );
    memset( img->pixelData, 0, bufferSize);
    uint8_t * pBuffer = img->pixelData + (bufferSize - filteredSize);

    //Undo ZLIB Compression to get PNG Filtered scanlines
    xnz_png_inflate_blobs(&archive, pBuffer, filteredSize);
    
    //Undo PNG Filtering to get raw pixel scanlines
    xnz_png_filter(pBuffer, archive.rowPitch, archive.nScanlines, archive.depth, archive.nSrcChannels);
    
    if( bufferSize > filteredSize )
    {
        uint32_t srcBytesPerPixel = archive.depth/8 * archive.nSrcChannels;
        uint32_t dstBytesPerPixel = img->depth/8 * img->numChannels;
        uint32_t dstOffset = 0;
        uint32_t byteIndex; for(byteIndex=0; byteIndex < pngSize; byteIndex += srcBytesPerPixel )
        {
            memmove(img->pixelData + dstOffset, pBuffer + byteIndex, srcBytesPerPixel);
            img->pixelData[dstOffset + srcBytesPerPixel] = 255; //set expanded dst channels (ie alpha) to 1
            dstOffset += dstBytesPerPixel;
        }
    }
    
    xnz_png_close(&archive);
    
}


XNG_IMAGE_API XNG_IMAGE_INLINE void xng_load_sgi_image(xng_image* img)
{
    if (!img) return;

    //load and decode png from file
    //stbi_convert_iphone_png_to_rgb(1);
    //img->pixelData = stbi_load(img->filepath, (int*)&(img->width), (int*)&(img->height), (int*)&(img->numChannels), STBI_rgb_alpha);

    XNZ_SGI_IMAGE archive = {0};
    xnz_sgi_open(&archive, img->filepath);

    size_t sgiSize = archive.nSrcChannels * archive.rowPitch * archive.nScanlines * archive.depth / 8;

    //flip_vertically = ((int)s->img_y) > 0;
    //s->img_y = abs((int)s->img_y);

    //if (req_comp && req_comp >= 3) target = req_comp; // we can directly decode 3 or 4
    //else                           target = s->img_n; // if they want monochrome, we'll post-convert

    img->numChannels = archive.nSrcChannels == 3 ? 4 : archive.nSrcChannels; //1, 2 & 4 channels supported
    img->width       = archive.rowPitch;
    img->height      = archive.nScanlines;
    img->depth       = archive.depth;
    img->size        = img->numChannels * img->width * img->height * img->depth / 8;

    archive.nDstChannels = img->numChannels;

    //Allocate memory for xng_image pixel buffer to return to be returned to client
    size_t filteredSize = sgiSize;// +archive.rowPitch;                    //assume 1 extra filter byte per scanline
    size_t bufferSize = MAX(img->size, filteredSize); //Warning: for 3 to 4 channel expansion rowPitch must be added to the dst buffer size as well
    img->pixelData = (uint8_t*)malloc(bufferSize);
    memset(img->pixelData, 0, bufferSize);
    uint8_t* pBuffer = img->pixelData;// +(bufferSize - filteredSize);

    xnz_sgi_decode_scanlines(&archive, pBuffer, bufferSize);

    xnz_sgi_close(&archive);
}


//#pragma mark -- Image Create with Buffer Convenience Constructors

XNG_IMAGE_API XNG_IMAGE_INLINE xng_image xng_image_buffer_create(unsigned int width, unsigned int height, unsigned int numChannels, unsigned int depth, XNG_IMAGE_FORMAT imageFormat)
{
    xng_image image = {0};
    
    // TO DO:  check input values
    
    image.width = width;
    image.height = height;
    image.numChannels = numChannels;
    image.depth = depth;
    image.format = imageFormat;
    
    //calculate total bytes, even if bit depth of a single channel is less than 8 bits (ie 1 byte)
    image.size = width * height * numChannels * (depth/8);
    
    image.pixelData =  (unsigned char*)malloc( image.size);//(unsigned char*)_aligned_malloc( image.size, 16 );
    memset( image.pixelData, 0, image.size ); //set to black
    
    return image;
    
}




XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS xng_image_export_to_source_format(xng_image * image, const char * filepath)
{
    if( !image ) return XNG_IMAGE_API_NULL_CONTAINER;

    if( !image->pixelData ) return XNG_IMAGE_API_BUFFER_IS_NULL;

    switch( image->format )
    {
        case XNG_IMAGE_FORMAT_BYTES:
            xng_image_buffer_export_bytes(image->pixelData, image->width, image->height,image->depth, image->numChannels, filepath);
            break;
        case XNG_IMAGE_FORMAT_BMP:
            xnz_image_buffer_export_to_bmp(image->pixelData, image->width, image->height, image->depth, image->numChannels, filepath);
            break;

        default:
            return XNG_IMAGE_API_UNSUPPORTED_FORMAT;

    }

    return XNG_IMAGE_API_SUCCESS;
}



// *** Public API Functions *** //

XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS xng_image_draw_uint_pixels(xng_image* image, xnz_uint2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed* color_value)
{

    // TO DO:  check input values
    if (!image->pixelData) return XNG_IMAGE_API_BUFFER_IS_NULL;

    printf("x:  %d, y:  %d, z:  %d, w:  %d\n", color_value->x, color_value->y, color_value->z, color_value->w);
    xng_image_buffer_rasterize_value_to_uint2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, point_coordinates, numCoordinates, color_value);

    return XNG_IMAGE_API_SUCCESS;

}


XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS xng_image_draw_circles_uint16(xng_image* image, uint16_t* circle_center_x_values, uint16_t* circle_center_y_values, unsigned int numCircles, unsigned int radius, xnz_byte4_packed* color_value)
{

    // TO DO:  check input values
    if (!image->pixelData) return XNG_IMAGE_API_BUFFER_IS_NULL;

    //printf("x:  %d, y:  %d, z:  %d, w:  %d\n",  color_value->x, color_value->y, color_value->z, color_value->w);
    xng_image_buffer_rasterize_circles_from_uint16_x_y_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, false, circle_center_x_values, circle_center_y_values, numCircles, radius, color_value);
    //xng_image_buffer_rasterize_value_to_uint2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, point_coordinates, numCoordinates, color_value);

    return XNG_IMAGE_API_SUCCESS;

}

XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS xng_image_draw_circles(xng_image* image, xnz_uint2* circle_centers, unsigned int numCircles, unsigned int radius, xnz_byte4_packed* color_value)
{

    // TO DO:  check input values
    if (!image->pixelData) return XNG_IMAGE_API_BUFFER_IS_NULL;

    //printf("x:  %d, y:  %d, z:  %d, w:  %d\n",  color_value->x, color_value->y, color_value->z, color_value->w);
    xng_image_buffer_rasterize_circles_from_uint2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, false, circle_centers, numCircles, radius, color_value);
    //xng_image_buffer_rasterize_value_to_uint2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, point_coordinates, numCoordinates, color_value);

    return XNG_IMAGE_API_SUCCESS;

}

XNG_IMAGE_API XNG_IMAGE_INLINE XNG_IMAGE_API_STATUS xng_image_draw_float_pixels(xng_image* image, xnz_float2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed color_value)
{

    // TO DO:  check input values
    if (!image->pixelData) return XNG_IMAGE_API_BUFFER_IS_NULL;

    xng_image_buffer_rasterize_value_to_float2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, point_coordinates, numCoordinates, color_value);

    return XNG_IMAGE_API_SUCCESS;

}

