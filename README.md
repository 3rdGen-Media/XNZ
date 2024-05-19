<div>
<h2>XNZ</h1>
</div>

<p>
XNZ is a collection of ANSI/C89 compatible header-only APIs designed to support decoding block compressed 
media container formats while streaming from source (eg over a network connection or from file on disk) with 
the intent of queueing resources to an accelerated hardware pipeline.

Motivation to author a custom streaming block decompression library began with a desire for cross-platform 
parity with Apple's [compression_stream](https://developer.apple.com/documentation/compression/compression_stream?language=objc) API on Microsoft platforms [without the need to explicitly bootstrap zlib](https://www.reddit.com/r/C_Programming/comments/1bp2w11/how_to_deflate_zlib_compressed_buffer_using_win32/).  ```xnz_png_inflate_blobs``` provides 
an analogous implementation using ```compression_stream_process``` with the ```COMPRESSION_ZLIB``` algorithm for comparison.
</p>

<p>BC[#] and GDeflate support to come.  No roadmap for LZ4 as Apple's compression_stream and Pixar's USD implementations are satisfactory.</p>

<br/>
<div><a name="xnz_apis"></a></div>

| Library | Category | Description | Specification
--------------------- | -------- | -------------------------------- | --- 
**[xnz.h](xnz/xnz.h)** | decoder | bitstream loading/decoding from memory | RFC 1951
**[xnz_icc_profile.h](xnz/xnz_icc_profile.h)** | colorspace | color profile loading/decoding from file/memory | ICC
**[xnz_bmp_image.h](xnz/xnz_bmp_image.h)** | image | image encoding/export  to file/memory | BMP
**[xnz_dds_image.h](xnz/xnz_dds_image.h)** | image | image loading/decoding from file/memory | DDS
**[xnz_png_image.h](xnz/xnz_png_image.h)** | image | image loading/decoding from file/memory | PNG
**[xnz_sgi_image.h](xnz/xnz_sgi_image.h)** | image | image loading/decoding from file/memory | SGI

<p></p>

<div>
<h2>XNG</h1>
</div>

<p>
XNG is a sister layer that extends XNZ with other image formats that aren't based on block compression and additionally wraps all XNZ header-only library implementations for consumption via a single shared library/API.  
</p>
<br/>
<div><a name="xnb_classes"></a></div>

| File | Category | Description | Type
--------------------- | -------- | -------------------------------- | --- 
**[xng_image_formats.h](xnz/xng_image_formats.h)** | enum | xng image api enumerations + typedefs | ENUM
**[xng_image_buffer.h](xnz/xng_image_buffer.h)** | buffer | raw pixel buffer raster routines | PIXELS
**[xng_raw_image.h](xnz/xng_raw_image.h)** | image | image loading/decoding from file/memory | UINT8
**[xng_fpf_image.h](xnz/xng_fpf_image.h)** | image | image loading/decoding from file/memory | FLOAT32
**[xng_ttf_font.h](xnz/xnz_ttf_font.h)** | font | parse, decode, and rasterize characters from truetype fonts | TRUETYPE
**[xng_font.c](xnz/xng_font.c)** | virtual | base "class" to expose all component font apis | STRUCT
**[xng_image.c](xnz/xng_image.c)** | virtual | base "class" to expose all component image apis | STRUCT
**[xng.h](xng.h)** | header | global library header to expose all xnz + xng apis | HEADER

*XNG is the modular Image Resource Layer that operates in parallel with 3rdGen's Accelerated Graphics Layer, Core Render.*
*CoreRender's C libraries embody the foundational layer of 3rdGen's proprietary simulation engine and cross-platform application framework.*

<br/>
