#ifndef _XNG_BUFFER_H_
#define _XNG_BUFFER_H_


/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

 /* We are building or calling CRPrimitives as a static library */
#ifndef XNG_BUFFER_API
#define XNG_BUFFER_API static
#endif

//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_BUFFER_INLINE
#ifdef _WIN32
#define XNG_BUFFER_INLINE __inline
#else
#define XNG_BUFFER_INLINE
#endif
#endif
    
//inline doesn't exist in C89, __inline is MSVC specific
#ifndef XNG_BUFFER_DECLSPEC
#ifdef _WIN32
#define XNG_BUFFER_DECLSPEC __declspec
#else
#define XNG_BUFFER_DECLSPEC
#endif
#endif
    
//align functions are diffent on windows vs iOS, Linux, etc.
#ifndef XNG_BUFFER_ALIGN
#ifdef _WIN32
#define XNG_BUFFER_ALIGN(X) (align(X))
#else
#define XNG_BUFFER_ALIGN(X) __attribute__ ((aligned(X)))
#endif
#endif
    
#ifndef XNG_BUFFER_PACK_ATTRIBUTE
#ifdef _WIN32
#define XNG_BUFFER_PACK_ATTRIBUTE
#pragma pack(push, 1)
#else
#define XNG_BUFFER_PACK_ATTRIBUTE __attribute__ ((packed))
#endif
#endif

//#include <stdlib.h>
//#include <stdint.h>

#ifndef _WIN32   //I don't understand why VS compiler can chain headers in xng.h but Xcode can't...
#include "xnz.h" //xnz types
#endif

#include <math.h>
#include <limits.h>


//XNG_BUFFER_API XNG_BUFFER_INLINE xng_image xng_image_buffer_create(unsigned int width, unsigned int height, unsigned int numChannels, unsigned int depth, XNG_BUFFER_FORMAT imageFormat);

// *** private static rasterize functions *** //

//static void * xng_image_buffer_rasterize_float2_points(unsigned char ** imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, cr_float4* point_coordinates, unsigned int numCoordinates, cr_float4 color_value);


// *** Byte Buffer API Functions *** //

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_buffer_rasterize_value_to_uint16_x_y_point(unsigned char * imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, uint16_t x, uint16_t y, xnz_byte4_packed *color_value);
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_buffer_rasterize_value_to_uint2_point(unsigned char * imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, xnz_uint2 *point_coordinate, xnz_byte4_packed *color_value);
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_buffer_rasterize_value_to_float2_point(unsigned char * imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, xnz_float2 *point_coordinate, xnz_byte4_packed color_value);
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_buffer_rasterize_value_to_uint2_points(unsigned char * imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, xnz_uint2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed *color_value);
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_buffer_rasterize_value_to_float2_points(unsigned char * imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, xnz_float2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed color_value);

// #define max(a,b) x ^ ((x ^ y) & -(x < y)) // max(x, y)
// #define min(x,y) y ^ ((x ^ y) & -(x < y)) // min(x, y);

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_buffer_rasterize_circles_from_uint16_x_y_points( unsigned char * imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bytesPerChannel, bool normalized, uint16_t* circle_center_x_values, uint16_t* circle_center_y_values, unsigned int numCircles, unsigned int radius, xnz_byte4_packed *color_value);
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_buffer_rasterize_circles_from_uint2_points( unsigned char * imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bytesPerChannel, bool normalized, xnz_uint2* circle_centers, unsigned int numCircles, unsigned int radius, xnz_byte4_packed *color_value);

// *** Floating Point Buffer API Functions *** //

XNG_BUFFER_API XNG_BUFFER_INLINE void xng_fpf_buffer_draw_line_from_points(float* image_buffer, int16_t* width, int16_t* height, xnz_float2* pointA, xnz_float2* pointB);


// *** Public API Functions *** //
/*
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_draw_uint_pixels(xng_image * image, xnz_uint2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed *color_value);
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_draw_float_pixels(xng_image* image, xnz_float2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed color_value);

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_draw_circles(xng_image* image, xnz_uint2* circle_centers, unsigned int numCircles, unsigned int radius, xnz_byte4_packed* color_value);
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS  xng_image_draw_circles_uint16(xng_image * image, uint16_t* circle_center_x_values, uint16_t* circle_center_y_values, unsigned int numCircles, unsigned int radius, xnz_byte4_packed *color_value);
*/

static XNG_BUFFER_INLINE void xng_floats_to_bytes_insitu(float* floats, unsigned int nFloats);


// *** private static rasterize functions *** //

//static void * xng_image_buffer_rasterize_float2_points(unsigned char ** imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, xng_float4* point_coordinates, unsigned int numCoordinates, xng_float4 color_value);


XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_buffer_rasterize_value_to_uint16_x_y_point(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, uint16_t x, uint16_t y, xnz_byte4_packed* color_value)
{
    //convert the normalized point to image coordinate space
    //unsigned int channelIndex;

    unsigned int pixelSizeInBytes;

    //unsigned char * imageBuffer;
    unsigned int colIndex = x;
    unsigned int rowIndex = y;

    //draw a red pixel where the point is
    /*
    printf("\n xng_image_buffer_rasterize_value_to_uint16_x_y_point \n");
    printf("x:  %d, y:  %hd\n", x, y);
    printf("x:  %d, y:  %d, z:  %d, w:  %d\n",  color_value->x, color_value->y, color_value->z, color_value->w);

    printf("numChannels = %d\n",  numChannels);
    */

    //imageBuffer = *imageBufferPtr;
    pixelSizeInBytes = numChannels;//* channelDepthInBytes;
    //channel depth in bytes == 1

    memcpy(&(imageBuffer[(rowIndex * width + colIndex) * pixelSizeInBytes]), color_value->vector, numChannels);
    /*
    for( channelIndex = 0; channelIndex < numChannels; channelIndex++)
    {
        imageBuffer[(rowIndex * width + colIndex)*pixelSizeInBytes + channelIndex] = color_value->vector[channelIndex];
    }
    */

    return XNG_IMAGE_API_SUCCESS;
}

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_buffer_rasterize_value_to_uint2_point(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, xnz_uint2* point_coordinate, xnz_byte4_packed* color_value)
{
    //convert the normalized point to image coordinate space
    //unsigned int channelIndex;

    unsigned int pixelSizeInBytes;

    //unsigned char * imageBuffer;
    unsigned int colIndex = (*point_coordinate).x;
    unsigned int rowIndex = (*point_coordinate).y;

    //draw a red pixel where the point is
    /*
    printf("\n xng_image_buffer_rasterize_value_to_uint2_point \n");
    printf("x:  %d, y:  %d\n", colIndex, rowIndex);
    printf("x:  %d, y:  %d, z:  %d, w:  %d\n",  color_value->x, color_value->y, color_value->z, color_value->w);

    printf("numChannels = %d\n",  numChannels);
    */

    //imageBuffer = *imageBufferPtr;
    pixelSizeInBytes = numChannels;//* channelDepthInBytes;
    //channel depth in bytes == 1

    memcpy(&(imageBuffer[(rowIndex * width + colIndex) * pixelSizeInBytes]), color_value->vector, numChannels);
    /*
    for( channelIndex = 0; channelIndex < numChannels; channelIndex++)
    {
        imageBuffer[(rowIndex * width + colIndex)*pixelSizeInBytes + channelIndex] = color_value->vector[channelIndex];
    }
    */

    return XNG_IMAGE_API_SUCCESS;
}

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_buffer_rasterize_value_to_float2_point(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, xnz_float2* point_coordinate, xnz_byte4_packed color_value)
{
    //convert the normalized point to image coordinate space
    //unsigned int channelIndex;

    unsigned int pixelSizeInBytes;

    //unsigned char * imageBuffer;
    unsigned int colIndex = (unsigned int)(*point_coordinate).x;
    unsigned int rowIndex = (unsigned int)(*point_coordinate).y;

    //printf("x:  %d, y:  %d", colIndex, rowIndex);
    //draw a red pixel where the point is

    //imageBuffer = *imageBufferPtr;
    pixelSizeInBytes = numChannels * channelDepthInBytes;
    //channel depth in bytes == 1

    memcpy(&(imageBuffer[(rowIndex * width + colIndex) * pixelSizeInBytes]), color_value.vector, numChannels);
    /*
    for( channelIndex = 0; channelIndex < numChannels; channelIndex++)
    {
        imageBuffer[(rowIndex * width + colIndex)*pixelSizeInBytes + channelIndex] = color_value.vector[channelIndex];
    }
    */

    return XNG_IMAGE_API_SUCCESS;
}

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_buffer_rasterize_value_to_uint2_points(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, xnz_uint2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed* color_value)
{
    int coordIndex;
    //allocate an RGBA8 char * buffer
    //unsigned int imageBufferSize = numChannels*channelDepthInBytes*width*height;
    //unsigned char * imageBuffer = (unsigned char*)malloc( imageBufferSize );

    //set the image buffer canvas color to white (and set alpha to 1.0f)
    //memset(imageBuffer, 255, imageBufferSize);

    //iterate over each normalizedCoordinates and draw it in the buffer canvas
    for (coordIndex = 0; coordIndex < numCoordinates; coordIndex++)
    {
        //printf("\nDrawing Coordinate Point %d:   (%f, %f)", coordIndex, coordinates[coordIndex].x, coordinates[coordIndex].y);
        //vd_setRGBA8Pixel(coordinates[coordIndex], width, height, pixelValue, imageBuffer);
        xng_image_buffer_rasterize_value_to_uint2_point(imageBuffer, width, height, numChannels, channelDepthInBytes, &(point_coordinates[coordIndex]), color_value);
    }

    return XNG_IMAGE_API_SUCCESS;
}

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_buffer_rasterize_value_to_float2_points(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int channelDepthInBytes, xnz_float2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed color_value)
{
    int coordIndex;
    //allocate an RGBA8 char * buffer
    //unsigned int imageBufferSize = 4*sizeof(char)*width*height;
    //unsigned char * imageBuffer = (unsigned char*)malloc( imageBufferSize );

    //set the image buffer canvas color to white (and set alpha to 1.0f)
    //memset(imageBuffer, 255, imageBufferSize);

    //iterate over each normalizedCoordinates and draw it in the buffer canvas
    for (coordIndex = 0; coordIndex < numCoordinates; coordIndex++)
    {
        //printf("\nDrawing Coordinate Point %d:   (%f, %f)", coordIndex, coordinates[coordIndex].x, coordinates[coordIndex].y);
        xng_image_buffer_rasterize_value_to_float2_point(imageBuffer, width, height, numChannels, channelDepthInBytes, &(point_coordinates[coordIndex]), color_value);
        //vd_setRGBA8Pixel(coordinates[coordIndex], width, height, pixelValue, imageBuffer);
    }

    return XNG_IMAGE_API_SUCCESS;
}

// #define max(a,b) x ^ ((x ^ y) & -(x < y)) // max(x, y)

// #define min(x,y) y ^ ((x ^ y) & -(x < y)) // min(x, y);

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_buffer_rasterize_circles_from_uint16_x_y_points(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bytesPerChannel, bool normalized, uint16_t* circle_center_x_values, uint16_t* circle_center_y_values, unsigned int numCircles, unsigned int radius, xnz_byte4_packed* color_value)
{
    //TO DO: implement normalized functionality

/*
    struct vd_rgba8 * pixelValue = (struct vd_rgba8*)malloc( sizeof(vd_rgba8) );
    pixelValue->r = (char)255;
    pixelValue->g = (char)0;
    pixelValue->b = (char)0;
    pixelValue->a = (char)255;
*/
//struct vd_rgba8 pixelValue;
//pixelValue.r = (char)255;
//pixelValue.g = (char)0;
//pixelValue.b = (char)0;
//pixelValue.a = (char)255;

//allocate an RGBA8 char * buffer
//unsigned int imageBufferSize = 4*sizeof(char)*width*height;

//unsigned char * imageBuffer = (unsigned char*)malloc( imageBufferSize );
//set the image buffer canvas color to white (and set alpha to 1.0f)
//memset(imageBuffer, 0, imageBufferSize);
    xnz_uint2 point;
    unsigned int r2, diameter, area, circleIndex;
    int i;
    r2 = radius * radius;       //r^2
    diameter = radius * 2;// << 1;     //2r = diameter
    area = 4 * r2 + diameter;// << 2;             //4r^2 + 2*radius

    for (circleIndex = 0; circleIndex < numCircles; circleIndex++)
    {
        unsigned int originX, originY;

        if (normalized)
        {
            originX = circle_center_x_values[circleIndex] * width;
            originY = circle_center_y_values[circleIndex] * height;
        }
        else
        {
            originX = circle_center_x_values[circleIndex];
            originY = circle_center_y_values[circleIndex];

            printf("center x:  %hd, y:  %hd\n", circle_center_x_values[circleIndex], circle_center_y_values[circleIndex]);

        }

        //vd_setRGBA8Pixel(originX, originY, width, height, pixelValue, imageBuffer);

        //ignore drawing any circles that have centers within radius of the edge of the image
        //if( originX <= radius || originY <= radius )
        //    continue;
        //else if ( originX + radius >= width || originY+radius >= height )
        //    continue;

        //every unit of circle area can be mapped back to an x,y pixel coordinate
        for (i = 0; i < area; i++)
        {
            int tx = (i / diameter) - radius;
            int ty = (i % diameter) - radius;

            //printf("\n#%d -- tx: %d, ty: %d", i, tx, ty);

            //if( ty > 0 )
            //    ty+=1;

            if (tx * tx + ty * ty <= r2)
            {
                point.x = originX + tx;
                point.y = originY + ty;

                if ((int)originX + tx < 0)
                {
                    point.x = originX;
                    //printf("\n x less than 0\n");
                }

                if ((int)originY + ty < 0)
                {
                    point.y = originY;
                    //printf("\n y less than 0\n");
                }

                xng_image_buffer_rasterize_value_to_uint2_point(imageBuffer, width, height, numChannels, bytesPerChannel, &point, color_value);
                //vd_setRGBA8Pixel(originX+tx, originY+ty, width, height, pixelValue, imageBuffer);
            }
        }

        //the former loop math misses the last row of pixels
        //so catch it here
        for (i = -1 * radius; i <= radius; i++)
        {
            int tx = i;
            int ty = radius;

            //printf("\n#%d -- tx: %d, ty: %d", i, tx, ty);

            //if( ty > 0 )
            //    ty+=1;

            if (tx * tx + ty * ty <= r2)
            {
                point.x = originX + tx;
                point.y = originY + ty;

                if ((int)originX + tx < 0)
                {
                    point.x = originX;
                    //printf("\n x less than 0\n");
                }

                if ((int)originY + ty < 0)
                {
                    point.y = originY;
                    //printf("\n y less than 0\n");
                }

                xng_image_buffer_rasterize_value_to_uint2_point(imageBuffer, width, height, numChannels, bytesPerChannel, &point, color_value);
                //vd_setRGBA8Pixel(originX+tx, originY+ty, width, height, pixelValue, imageBuffer);
            }
        }


        /*
        //DOUBLE LOOP version of above
        for(int y=-radius; y<=radius; y++)
        {
            for(int x=-radius; x<=radius; x++)
            {
                if(x*x+y*y <= radius*radius)
                    vd_setRGBA8Pixel(originX+x, originY+y, width, height, pixelValue, imageBuffer);
            }
        }
        */

    }


    return XNG_IMAGE_API_SUCCESS;//(void*)imageBuffer;
}

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_buffer_rasterize_circles_from_uint2_points(unsigned char* imageBuffer, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bytesPerChannel, bool normalized, xnz_uint2* circle_centers, unsigned int numCircles, unsigned int radius, xnz_byte4_packed* color_value)
{
    //TO DO: implement normalized functionality

/*
    struct vd_rgba8 * pixelValue = (struct vd_rgba8*)malloc( sizeof(vd_rgba8) );
    pixelValue->r = (char)255;
    pixelValue->g = (char)0;
    pixelValue->b = (char)0;
    pixelValue->a = (char)255;
*/
//struct vd_rgba8 pixelValue;
//pixelValue.r = (char)255;
//pixelValue.g = (char)0;
//pixelValue.b = (char)0;
//pixelValue.a = (char)255;

//allocate an RGBA8 char * buffer
//unsigned int imageBufferSize = 4*sizeof(char)*width*height;

//unsigned char * imageBuffer = (unsigned char*)malloc( imageBufferSize );
//set the image buffer canvas color to white (and set alpha to 1.0f)
//memset(imageBuffer, 0, imageBufferSize);
    xnz_uint2 point;
    unsigned int r2, diameter, area, circleIndex;
    int i;
    r2 = radius * radius;       //r^2
    diameter = radius * 2;// << 1;     //2r = diameter
    area = 4 * r2 + diameter;// << 2;             //4r^2 + 2*radius

    for (circleIndex = 0; circleIndex < numCircles; circleIndex++)
    {
        unsigned int originX, originY;

        if (normalized)
        {
            originX = circle_centers[circleIndex].x * width;
            originY = circle_centers[circleIndex].y * height;
        }
        else
        {
            originX = circle_centers[circleIndex].x;
            originY = circle_centers[circleIndex].y;

        }

        //vd_setRGBA8Pixel(originX, originY, width, height, pixelValue, imageBuffer);

        //ignore drawing any circles that have centers within radius of the edge of the image
        //if( originX <= radius || originY <= radius )
        //    continue;
        //else if ( originX + radius >= width || originY+radius >= height )
        //    continue;

        //every unit of circle area can be mapped back to an x,y pixel coordinate
        for (i = 0; i < area; i++)
        {
            int tx = (i / diameter) - radius;
            int ty = (i % diameter) - radius;

            //printf("\n#%d -- tx: %d, ty: %d", i, tx, ty);

            //if( ty > 0 )
            //    ty+=1;

            if (tx * tx + ty * ty <= r2)
            {
                point.x = originX + tx;
                point.y = originY + ty;
                xng_image_buffer_rasterize_value_to_uint2_point(imageBuffer, width, height, numChannels, bytesPerChannel, &point, color_value);
                //vd_setRGBA8Pixel(originX+tx, originY+ty, width, height, pixelValue, imageBuffer);
            }
        }

        //the former loop math misses the last row of pixels
        //so catch it here
        for (i = -(int)radius; i <= (int)radius; i++)
        {
            int tx = i;
            int ty = radius;

            //printf("\n#%d -- tx: %d, ty: %d", i, tx, ty);

            //if( ty > 0 )
            //    ty+=1;

            if (tx * tx + ty * ty <= r2)
            {
                point.x = originX + tx;
                point.y = originY + ty;
                xng_image_buffer_rasterize_value_to_uint2_point(imageBuffer, width, height, numChannels, bytesPerChannel, &point, color_value);
                //vd_setRGBA8Pixel(originX+tx, originY+ty, width, height, pixelValue, imageBuffer);
            }
        }


        /*
        //DOUBLE LOOP version of above
        for(int y=-radius; y<=radius; y++)
        {
            for(int x=-radius; x<=radius; x++)
            {
                if(x*x+y*y <= radius*radius)
                    vd_setRGBA8Pixel(originX+x, originY+y, width, height, pixelValue, imageBuffer);
            }
        }
        */

    }


    return XNG_IMAGE_API_SUCCESS;//(void*)imageBuffer;
}

// *** Floating Point Buffer API Functions *** //


XNG_BUFFER_API XNG_BUFFER_INLINE void xng_fpf_buffer_draw_line_from_points(float * image_buffer, int16_t *width, int16_t *height, xnz_float2 *pointA, xnz_float2 *pointB)
{
    //println!("draw_line {} {}", p0, p1);
    
	//TO DO:  check if float points are equal in y direction
	//if p0.y == p1.y {
    //    return
    //}

	//xnz_float2 tmp;
	//xnz_float2 *points[2];
	//uint8_t smallerPointIndex;
	float a1, a2, s, sx0, halfs, x0float, a0, x1float, am, dam, ds, da0, x0term, dy, xnext, d, x0, x1, x0floor, x1ceil, dir, dxdy, x;
	uint32_t y0, yN, xIndex, yIndex, linestart;
	int32_t x0int, x1int, x0int1;
	xnz_float2  p0;
	xnz_float2  p1;
	int8_t sign, isign;
	uint8_t value;//, x0int1;
	//return;

	//TO DO: if we pass in the non transformed int16 em square values we can avoid the float comparison


	

	/*
    let (dir, p0, p1) = if p0.y < p1.y {
        (1.0, p0, p1)
    } else {
        (-1.0, p1, p0)
    };
	*/
		//points[0] = pointA;
		//points[1] = pointB;

		//p0 = pointA;
		//p1 = pointB;
	//return;
	//dir = pointA->y - pointB->y;
	//sign = (( (*(int32_t*)&dir) & 0x80000000) >> 31) ;//* -1;
	//isign = 1 - sign - sign;
	//if ( dir * (1 - sign - sign) < 0.001f)//(uint32_t)pointA->y == (uint32_t)pointB->y) 
	//	return;

	
	//dir = fabs( dir ) / dir;//
	
	//p0 = pointA;
	//p1 = pointB;
	//p0 = ( sign > 0 ? *pointA : *pointB);
	//p1 = ( sign > 0 ? *pointB : *pointA);
	//dir = ( pointA->y < pointB->y ? 1.f : -1.f );

	//dir = pointA->y - pointB->y;
	//sign = (( (*(int32_t*)&dir) & 0x80000000) >> 31) ;//* -1;
	//dir *= (1 - sign - sign);
	//p0 = *pointA;
	//p1 = *pointB;
		

	//isign = 1-sign;
	//p0.vector[0] = sign * pointA->vector[0] + isign * pointB->vector[0];
	//p0.vector[1] = sign * pointA->vector[1] + isign * pointB->vector[1];

	//p1.vector[0] = isign * pointA->vector[0] + sign * pointB->vector[0];
	//p1.vector[1] = isign * pointA->vector[1] + sign * pointB->vector[1];


	//sign *= -1;
	//dir = 1 - sign - sign;//sign + sign + 1;
	//printf("\n dir = %f\n", dir);
	/*
	if( sign < 0 )
	{
		p0 = pointA;
		p1 = pointB;
		//dir = 1.f;
	}
	else
	{
		p0 = pointB;
		p1 = pointA;
		//dir = -1.f;
	}
	*/
	/*
	dir = pointA->y - pointB->y;
	dir = fabs( dir ) / dir;//
	p0 = ( pointA->y < pointB->y ? pointA : pointB);
	p1 = ( pointA->y < pointB->y ? pointB : pointA);
	*/
	//dir = ( pointA->y < pointB->y ? 1.f : -1.f );

	//if( pointA->y < 0.f )
	//	pointA->y = 0.f;
	//if( pointB->y < 0.f )
	//	pointB->y = 0.f;

	//adjust negative x values to 0.f
	pointA->x -= pointA->x * (( (*(int32_t*)&(pointA->x)) & 0x80000000) >> 31);
	pointB->x -= pointB->x * (( (*(int32_t*)&(pointB->x)) & 0x80000000) >> 31);

	//adjust negative y values to 0.f prior to y value equality check
	pointA->y -= pointA->y * (( (*(int32_t*)&(pointA->y)) & 0x80000000) >> 31);
	pointB->y -= pointB->y * (( (*(int32_t*)&(pointB->y)) & 0x80000000) >> 31);

	//printf("\ndraw line from {%f, %f} to {%f, %f}\n", pointA->x, pointA->y, pointB->x, pointB->y);

	/*
	if( pointA->x > (float)*width ) //return;
		pointA->x = (float)*width;
	if( pointB->x > (float)*width ) //return;
		pointB->x = (float)*width ;
	*/	
	if( pointA->y > (float)*height  ) //return;
		pointA->y = (float)*height ;
	if( pointB->y > (float)*height  ) //return;
		pointB->y = (float)*height;
	

	if( pointA->y < pointB->y)// (( (*(int32_t*)&dir) & 0x80000000) >> 31) )
	{
		//if( dir > -0.001f )
			//return;
		if ( pointB->y - pointA->y < 0.015625f)//(uint32_t)pointA->y == (uint32_t)pointB->y) 
			return;

		p0 = *pointA;
		p1 = *pointB;
		dir = 1.f;

	
	}

	else
	{
		//if( dir < 0.001f )
		//	return;
		if ( pointA->y - pointB->y < 0.015625f)//(uint32_t)pointA->y == (uint32_t)pointB->y) 
			return;


		p0 = *pointB;
		p1 = *pointA;
		dir = -1.f;
	}
	/*
	tmp = *p0;
	p0->x = tmp.y; 
	p0->y = tmp.x;

	tmp = *p1;
	p1->x = tmp.y;
	p1->y = tmp.x;
	*/
	//return;
    dxdy =  (p1.x - p0.x) / (p1.y - p0.y);
    x = p0.x;
    y0 = (uint32_t)p0.y ;// note: implicit max of 0 because usize (TODO: really true?)
    if (p0.y < 0.f) {
        x -= p0.y * dxdy;
		p0.y = 0.f;
    }
	if( p1.y < 0.f )
		p1.y = 0.f;
	//yN = ((uint32_t)ceil(p1->y));
	//sign = (( (*(int32_t*)&(p1->y)) & 0x80000000) >> 31) ;//* -1;
	yN = ceil(p1.y);//(uint32_t)(p1->y);// lop off decimal to get a negative or positive int
	
	//if( p1->y - (float)yN)
	//	yN++;
	//yN = p1->y - (float)yN ;//ceil(p1->y);// MIN((*height), ((uint32_t)ceil(p1->y)));

	//linestart = (y0-1) * width;
	yIndex = y0;
	linestart = y0 * (*width);
	

	a0 = (float)(yIndex+1);
	dy = p1.y - a0;//(uint32_t)p1.y;
	sign = (( (*(int32_t*)&dy) & 0x80000000) >> 31) ;//* -1;
	dy = sign * p1.y + (1-sign) * a0 /*MIN( ((float)(yIndex+1)), p1.y )*/ - p0.y;//MAX( ((float)yIndex), p0.y );
		xnext = x + dxdy * dy;
		d = dy * dir;

		if(x < xnext)
		{
			x0 = x;
			x1 = xnext;
		}
		else
		{
			x0 = xnext;
			x1 = x;
		}

		x0floor = floor(x0);
		x0int = (int32_t)x0floor;
		x1ceil = ceil(x1);
		x1int = (int32_t)x1ceil;

		x0int1 = x0int+1;
		if( x1int <= x0int1 )
		{
			float xmf = (0.5f * (x + xnext) - x0floor) * d;
			image_buffer[linestart + (uint32_t)x0int] += d - xmf;
			image_buffer[linestart + (uint32_t)x0int1] += xmf;
		}
		else
		{
			//float s, x0float, a0, x1float, am;
			//float dam;
			//float da0, x0term;
			uint32_t x0int2 = x0int1+1;

			s = 1.f/(x1 - x0);//recip(x1 - x0);
			halfs = 0.5f * s;
			x0float = x0 - x0floor;
			sx0 = s * x0float;
			x0term = 1.f - x0float;
			a0 = halfs * x0term * x0term;
			//a0 = halfs - sx0 * (1.f - 0.5f*x0float);
			x1float = x1 - x1ceil + 1.f;
			am = halfs * x1float * x1float;
			da0 = d * a0;
			image_buffer[linestart + (uint32_t)x0int] += da0;//d * a0;
			
			dam = d * am;
			if( x1int == x0int2 )
				image_buffer[linestart + (uint32_t)x0int1] += d - da0 - dam;//d * (1.0f - a0 - am);
			else
			{

				//float a1, a2;
				//uint32_t xIndex;
				uint32_t x1int1 = x1int-1;
				
				ds = d * s;
				
				a1 = halfs + s - sx0;//s * (1.5f - x0float);
				image_buffer[linestart + (uint32_t)x0int1] += d*a1 - da0;
				for(xIndex = x0int2; xIndex< x1int1; xIndex ++)
				{
					image_buffer[linestart + (uint32_t)(xIndex)] += ds;//d * s;
				}
				a2 = a1 + ((float)(x1int1 - x0int2/* - 3*/))*s;
				image_buffer[linestart + (uint32_t)x1int1] += d - d*a2 - dam;//d * (1.0f - a2 - am);

			}
			image_buffer[linestart + (uint32_t)x1int] += dam;//d * am;
		}
		x = xnext;
	
	//dy = 1.f;//
	for( yIndex = y0+1; yIndex<yN-1; yIndex++)
	{
		
		linestart += (*width);//yIndex * width;
		//dy = 1.f;//(float)(yIndex+1)/*MIN( ((float)(yIndex+1)), p1.y )*/ - (float)yIndex;//MAX( ((float)yIndex), p0.y );
		xnext = x + dxdy;// * dy;
		//d = dy * dir;

		if(x < xnext)
		{
			x0 = x;
			x1 = xnext;
		}
		else
		{
			x0 = xnext;
			x1 = x;
		}

		x0floor = floor(x0);
		x0int = (int32_t)x0floor;
		x1ceil = ceil(x1);
		x1int = (int32_t)x1ceil;

		x0int1 = x0int+1;
		if( x1int <= x0int1 )
		{
			float xmf = (0.5f * (x + xnext) - x0floor) * dir;
			image_buffer[linestart + (uint32_t)x0int] += dir - xmf;
			image_buffer[linestart + x0int1] += xmf;
		}
		else
		{
			//float s, x0float, a0, x1float, am;
			//float da0, x0term;
			uint32_t x0int2 = x0int1+1;

			s = 1.f/(x1 - x0);//recip(x1 - x0);
			halfs = 0.5f * s;
			x0float = x0 - x0floor;
			sx0 = s * x0float;
			x0term = 1.f - x0float;
			a0 = halfs * x0term * x0term;
			//a0 = halfs - sx0 * (1.f - 0.5f*x0float);
			x1float = x1 - x1ceil + 1.f;
			am = halfs * x1float * x1float;
			da0 = dir * a0;
			image_buffer[linestart + (uint32_t)x0int] += da0;//d * a0;

			dam = dir * am;
			if( x1int == x0int2 )
				image_buffer[linestart + (uint32_t)x0int1] += dir - da0 - dam;//d * (1.0f - a0 - am);
			else
			{

				//float a1, a2;
				//uint32_t xIndex;
				
				/*
				uint32_t xStart, xEnd;

				
				if( x0int+2 < x1int -1 )
				{
					xStart = x0int+2;
					xEnd = x1int -1;
				}
				else{
					xEnd = x0int+2;
					xStart= x1int -1;
				}
				*/
				uint32_t x1int1 = x1int-1;
				ds = dir*s;//
				a1 = halfs + s - sx0;//s * (1.5f - x0float);
				image_buffer[linestart + (uint32_t)x0int1] += dir * a1 - da0;

				
				for(xIndex = x0int2; xIndex< x1int1; xIndex ++)
				{
					image_buffer[linestart + (uint32_t)(xIndex)] += ds;//d * s;
				}
				a2 = a1 + ((float)(x1int1 - x0int2/* - 3*/))*s;
				image_buffer[linestart + (uint32_t)x1int1] += dir - dir*a2 - dam;//d * (1.0f - a2 - am);

			}
			image_buffer[linestart + (uint32_t)x1int] += dam;//d * am;
		}
		x = xnext;
	}

	if( yIndex < yN )
	{
		linestart += (*width);//yIndex * width;
		dy = p1.y/*MIN( ((float)(yIndex+1)), p1->y ) */- (float)yIndex;//MAX( ((float)yIndex), p0->y );
		xnext = x + dxdy * dy;
		d = dy * dir;

		if(x < xnext)
		{
			x0 = x;
			x1 = xnext;
		}
		else
		{
			x0 = xnext;
			x1 = x;
		}

		x0floor = floor(x0);
		x0int = (int32_t)x0floor;
		x1ceil = ceil(x1);
		x1int = (int32_t)x1ceil;

		x0int1 = x0int+1;
		if( x1int <= x0int1 )
		{
			float xmf = (0.5f * (x + xnext) - x0floor) * d;
			image_buffer[linestart + (uint32_t)x0int] += d - xmf;
			image_buffer[linestart + (uint32_t)x0int1] += xmf;
		}
		else
		{
			//float s, x0float, a0, x1float, am;
			//float da0, x0term;
			uint32_t x0int2 = x0int1 + 1;

			s = 1.f/(x1 - x0);//recip(x1 - x0);
			halfs = 0.5f * s;

			x0float = x0 - x0floor;
			sx0 = s * x0float;
			x0term = 1.f - x0float;
			a0 = halfs * x0term * x0term;			//a0 = halfs - sx0 * (1.f - 0.5f*x0float);
			x1float = x1 - x1ceil + 1.f;
			am = halfs * x1float * x1float;
			da0 = d * a0;
			image_buffer[linestart + (uint32_t)x0int] += da0;//d * a0;
			
			dam = d * am;
			if( x1int == x0int2 )
				image_buffer[linestart + (uint32_t)x0int1] += d - da0 - dam;//d * (1.0f - a0 - am);
			else
			{

				//float a1, a2;
				//uint32_t xIndex;
				
				/*
				uint32_t xStart, xEnd;

				
				if( x0int+2 < x1int -1 )
				{
					xStart = x0int+2;
					xEnd = x1int -1;
				}
				else{
					xEnd = x0int+2;
					xStart= x1int -1;
				}
				*/
				uint32_t x1int1 = x1int -1;
				ds = d*s;//
				a1 = halfs + s - sx0;//s * (1.5f - x0float);
				image_buffer[linestart + (uint32_t)x0int1] += d*a1 - da0;
				for(xIndex = x0int2; xIndex< x1int1; xIndex ++)
				{
					image_buffer[linestart + (uint32_t)(xIndex)] += ds;//d * s;
				}
				a2 = a1 + ((float)(x1int1 - x0int2/* - 3*/))*s;
				image_buffer[linestart + (uint32_t)(x1int1)] += d - d*a2 - dam;//d * (1.0f - a2 - am);

			}
			image_buffer[linestart + (uint32_t)x1int] += dam;//d * am;
		}
		//x = xnext;
	}
	/*
    for y in y0..min(self.h, p1.y.ceil() as usize) {
        let linestart = y * self.w;
        let dy = ((y + 1) as f32).min(p1.y) - (y as f32).max(p0.y);
        let xnext = x + dxdy * dy;
        let d = dy * dir;
        let (x0, x1) = if x < xnext { (x, xnext) } else { (xnext, x) };
        let x0floor = x0.floor();
        let x0i = x0floor as i32;
        let x1ceil = x1.ceil();
        let x1i = x1ceil as i32;
        if x1i <= x0i + 1 {
            let xmf = 0.5 * (x + xnext) - x0floor;
            self.a[linestart + x0i as usize] += d - d * xmf;
            self.a[linestart + (x0i + 1) as usize] += d * xmf;
        } else {
            let s = recip(x1 - x0);
            let x0f = x0 - x0floor;
            let a0 = 0.5 * s * (1.0 - x0f) * (1.0 - x0f);
            let x1f = x1 - x1ceil + 1.0;
            let am = 0.5 * s * x1f * x1f;
            self.a[linestart + x0i as usize] += d * a0;
            if x1i == x0i + 2 {
                self.a[linestart + (x0i + 1) as usize] += d * (1.0 - a0 - am);
            } else {
                let a1 = s * (1.5 - x0f);
                self.a[linestart + (x0i + 1) as usize] += d * (a1 - a0);
                for xi in x0i + 2 .. x1i - 1 {
                    self.a[linestart + xi as usize] += d * s;
                }
                let a2 = a1 + (x1i - x0i - 3) as f32 * s;
                self.a[linestart + (x1i - 1) as usize] += d * (1.0 - a2 - am);
            }
            self.a[linestart + x1i as usize] += d * am;
        }
        x = xnext;
    }
	*/
    return;

}


// *** Public API Functions *** //

/*
XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_draw_uint_pixels(xng_image* image, xnz_uint2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed* color_value)
{

    // TO DO:  check input values
    if (!image->pixelData) return XNG_IMAGE_API_BUFFER_IS_NULL;

    printf("x:  %d, y:  %d, z:  %d, w:  %d\n", color_value->x, color_value->y, color_value->z, color_value->w);
    xng_image_buffer_rasterize_value_to_uint2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, point_coordinates, numCoordinates, color_value);

    return XNG_IMAGE_API_SUCCESS;

}


XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_draw_circles_uint16(xng_image* image, uint16_t* circle_center_x_values, uint16_t* circle_center_y_values, unsigned int numCircles, unsigned int radius, xnz_byte4_packed* color_value)
{

    // TO DO:  check input values
    if (!image->pixelData) return XNG_IMAGE_API_BUFFER_IS_NULL;

    //printf("x:  %d, y:  %d, z:  %d, w:  %d\n",  color_value->x, color_value->y, color_value->z, color_value->w);
    xng_image_buffer_rasterize_circles_from_uint16_x_y_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, false, circle_center_x_values, circle_center_y_values, numCircles, radius, color_value);
    //xng_image_buffer_rasterize_value_to_uint2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, point_coordinates, numCoordinates, color_value);

    return XNG_IMAGE_API_SUCCESS;

}

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_draw_circles(xng_image* image, xnz_uint2* circle_centers, unsigned int numCircles, unsigned int radius, xnz_byte4_packed* color_value)
{

    // TO DO:  check input values
    if (!image->pixelData) return XNG_IMAGE_API_BUFFER_IS_NULL;

    //printf("x:  %d, y:  %d, z:  %d, w:  %d\n",  color_value->x, color_value->y, color_value->z, color_value->w);
    xng_image_buffer_rasterize_circles_from_uint2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, false, circle_centers, numCircles, radius, color_value);
    //xng_image_buffer_rasterize_value_to_uint2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, point_coordinates, numCoordinates, color_value);

    return XNG_IMAGE_API_SUCCESS;

}

XNG_BUFFER_API XNG_BUFFER_INLINE XNG_IMAGE_API_STATUS xng_image_draw_float_pixels(xng_image* image, xnz_float2* point_coordinates, unsigned int numCoordinates, xnz_byte4_packed color_value)
{

    // TO DO:  check input values
    if (!image->pixelData) return XNG_IMAGE_API_BUFFER_IS_NULL;

    xng_image_buffer_rasterize_value_to_float2_points(image->pixelData, image->width, image->height, image->numChannels, image->depth, point_coordinates, numCoordinates, color_value);

    return XNG_IMAGE_API_SUCCESS;

}
*/

static XNG_BUFFER_INLINE void xng_floats_to_bytes_insitu(float* floats, unsigned int nFloats)
{
    //It is difficult to subjectively judge the best mapping from [0.0 - 1.0] -> [0, 255]

    //option #1 permutations that don't handle floating point multiplication rounding error
    //#define F2B(f) floor( f >= 1.f ? UCHAR_MAX : (unsigned char)(f*256.f) ) //float  multiply + cast to byte
    //#define F2B(f) (f >= 1.f ? 255 : (int)(f*256.f))                        //float  multiply + cast to int
    //#define F2B(f) (f >= 1.  ? 255 : (int)(f*256. ))                        //double multiply + cast to int

    //options that handle floating point multiplication rounding error without forcing 0<=f<=1
    #define F2B(f) (f >= 1.f ? UCHAR_MAX : (unsigned char)(f*255.f+0.5f)) //d3d style clamp + round away from 0 [-0.00196, 1.00196] -> [-0.5, 255.5] -> [0,255]
    unsigned char* bytes = (unsigned char*)floats;
    for (unsigned int fi = 0; fi < nFloats; fi++) bytes[fi] = F2B(*(floats + fi));
}


#endif //_XNG_BUFFER_H_
