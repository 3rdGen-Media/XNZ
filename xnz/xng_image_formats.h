#ifndef _XNG_IMAGE_FORMATS_H_
#define _XNG_IMAGE_FORMATS_H_

#ifdef __cplusplus
extern "C" {
#endif

//custom define boolean type for c lib if c++ is not defined
#ifndef __cplusplus
#define bool  int
#define true  1
#define false 0
#endif

//define custom types in place of XNG_TRUE/XNG_FALSE
#define false 0
#define true  1

//inline doesn't exist in C89, __inline is MSVC specific
#ifdef _WIN32
#define CR_INLINE __inline  //Windows
#else
#define CR_INLINE           //iOS
#endif

#pragma mark -- XNZ Types

//OpenGL memory locations, basically select defines from GLEW

/* MatrixMode */
//#define XNG_MODELVIEW                      0x1700
//#define XNG_PROJECTION                     0x1701
//#define XNG_TEXTURE                        0x1702

/* MeshMode1 */
/*      XNG_POINT */
/*      XNG_LINE */

/* MeshMode2 */
/*      XNG_POINT */
/*      XNG_LINE */
/*      XNG_FILL */

/* NormalPointerType */
/*      XNG_BYTE */
/*      XNG_SHORT */
/*      XNG_INT */
/*      XNG_FLOAT */
/*      XNG_DOUBLE */
#define XNG_BITMAP						   0x1A00
/* PixelCopyType */
#define XNG_COLOR                          0x1800
#define XNG_DEPTH                          0x1801
#define XNG_STENCIL                        0x1802

/* PixelFormat */
#define XNG_COLOR_INDEX                    0x1900
#define XNG_STENCIL_INDEX                  0x1901
#define XNG_DEPTH_COMPONENT                0x1902
#define XNG_RED                            0x1903
#define XNG_GREEN                          0x1904
#define XNG_BLUE                           0x1905
#define XNG_ALPHA                          0x1906
#define XNG_RGB                            0x1907
#define XNG_RGBA                           0x1908
#define XNG_LUMINANCE                      0x1909
#define XNG_LUMINANCE_ALPHA                0x190A

//define data types for the opengl graphics contexts as specified by the opengl specification
//needed and used by crImage
#define XNG_BYTE						   0x1400
#define XNG_UNSIGNED_BYTE				   0x1401
#define XNG_SHORT						   0x1402
#define XNG_UNSIGNED_SHORT				   0x1403
#define XNG_INT							   0x1404
#define XNG_UNSIGNED_INT				   0x1405
#define XNG_FLOAT						   0x1406
#define XNG_2_BYTES						   0x1407
#define XNG_3_BYTES						   0x1408
#define XNG_4_BYTES						   0x1409
#define XNG_DOUBLE						   0x140A
#define XNG_FIXED						   0x140C

/* PixelInternalFormat */
/* PixelInternalFormat */
#define XNG_R3_G3_B2                       0x2A10
#define XNG_ALPHA4                         0x803B
#define XNG_ALPHA8                         0x803C
#define XNG_ALPHA12                        0x803D
#define XNG_ALPHA16                        0x803E
#define XNG_LUMINANCE4                     0x803F
#define XNG_LUMINANCE8                     0x8040
#define XNG_LUMINANCE12                    0x8041
#define XNG_LUMINANCE16                    0x8042
#define XNG_LUMINANCE4_ALPHA4              0x8043
#define XNG_LUMINANCE6_ALPHA2              0x8044
#define XNG_LUMINANCE8_ALPHA8              0x8045
#define XNG_LUMINANCE12_ALPHA4             0x8046
#define XNG_LUMINANCE12_ALPHA12            0x8047
#define XNG_LUMINANCE16_ALPHA16            0x8048
#define XNG_INTENSITY                      0x8049
#define XNG_INTENSITY4                     0x804A
#define XNG_INTENSITY8                     0x804B
#define XNG_INTENSITY12                    0x804C
#define XNG_INTENSITY16                    0x804D
#define XNG_RGB4                           0x804F
#define XNG_RGB5                           0x8050
#define XNG_RGB8                           0x8051
#define XNG_RGB10                          0x8052
#define XNG_RGB12                          0x8053
#define XNG_RGB16                          0x8054
#define XNG_RGBA2                          0x8055
#define XNG_RGBA4                          0x8056
#define XNG_RGB5_A1                        0x8057
#define XNG_RGBA8                          0x8058
#define XNG_RGB10_A2                       0x8059
#define XNG_RGBA12                         0x805A
#define XNG_RGBA16                         0x805B

/* OpenGL12 */
//#define XNG_PACK_SKIP_IMAGES               0x806B
//#define XNG_PACK_IMAGE_HEIGHT              0x806C
//#define XNG_UNPACK_SKIP_IMAGES             0x806D
//#define XNG_UNPACK_IMAGE_HEIGHT            0x806E
//#define XNG_TEXTURE_3D                     0x806F
//#define XNG_PROXY_TEXTURE_3D               0x8070
//#define XNG_TEXTURE_DEPTH                  0x8071
//#define XNG_TEXTURE_WRAP_R                 0x8072
//#define XNG_MAX_3D_TEXTURE_SIZE            0x8073
#define XNG_BGR                            0x80E0
#define XNG_BGRA                           0x80E1
#define XNG_UNSIGNED_BYTE_3_3_2            0x8032	//needed by crImage dds loader
#define XNG_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define XNG_UNSIGNED_SHORT_5_6_5           0x8363
#define XNG_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define XNG_UNSIGNED_SHORT_4_4_4_4         0x8033
#define XNG_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define XNG_UNSIGNED_SHORT_5_5_5_1         0x8034
#define XNG_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define XNG_UNSIGNED_INT_8_8_8_8           0x8035
#define XNG_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define XNG_UNSIGNED_INT_10_10_10_2        0x8036
#define XNG_UNSIGNED_INT_2_10_10_10_REV    0x8368
//#define XNG_RESCALE_NORMAL                 0x803A
//#define XNG_LIGHT_MODEL_COLOR_CONTROL      0x81F8
//#define XNG_SINGLE_COLOR                   0x81F9
//#define XNG_SEPARATE_SPECULAR_COLOR        0x81FA
//#define XNG_CLAMP_TO_EDGE                  0x812F
//#define XNG_TEXTURE_MIN_LOD                0x813A
//#define XNG_TEXTURE_MAX_LOD                0x813B
//#define XNG_TEXTURE_BASE_LEVEL             0x813C
//#define XNG_TEXTURE_MAX_LEVEL              0x813D
//#define XNG_MAX_ELEMENTS_VERTICES          0x80E8
//#define XNG_MAX_ELEMENTS_INDICES           0x80E9
//#define XNG_ALIASED_POINT_SIZE_RANGE       0x846D
//#define XNG_ALIASED_LINE_WIDTH_RANGE       0x846E

/* OpenGL13 */
/*
#define XNG_ACTIVE_TEXTURE                 0x84E0
#define XNG_CLIENT_ACTIVE_TEXTURE          0x84E1
#define XNG_MAX_TEXTURE_UNITS              0x84E2
#define XNG_TEXTURE0                       0x84C0
#define XNG_TEXTURE1                       0x84C1
#define XNG_TEXTURE2                       0x84C2
#define XNG_TEXTURE3                       0x84C3
#define XNG_TEXTURE4                       0x84C4
#define XNG_TEXTURE5                       0x84C5
#define XNG_TEXTURE6                       0x84C6
#define XNG_TEXTURE7                       0x84C7
#define XNG_TEXTURE8                       0x84C8
#define XNG_TEXTURE9                       0x84C9
#define XNG_TEXTURE10                      0x84CA
#define XNG_TEXTURE11                      0x84CB
#define XNG_TEXTURE12                      0x84CC
#define XNG_TEXTURE13                      0x84CD
#define XNG_TEXTURE14                      0x84CE
#define XNG_TEXTURE15                      0x84CF
#define XNG_TEXTURE16                      0x84D0
#define XNG_TEXTURE17                      0x84D1
#define XNG_TEXTURE18                      0x84D2
#define XNG_TEXTURE19                      0x84D3
#define XNG_TEXTURE20                      0x84D4
#define XNG_TEXTURE21                      0x84D5
#define XNG_TEXTURE22                      0x84D6
#define XNG_TEXTURE23                      0x84D7
#define XNG_TEXTURE24                      0x84D8
#define XNG_TEXTURE25                      0x84D9
#define XNG_TEXTURE26                      0x84DA
#define XNG_TEXTURE27                      0x84DB
#define XNG_TEXTURE28                      0x84DC
#define XNG_TEXTURE29                      0x84DD
#define XNG_TEXTURE30                      0x84DE
#define XNG_TEXTURE31                      0x84DF
#define XNG_NORMAL_MAP                     0x8511
#define XNG_REFLECTION_MAP                 0x8512
#define XNG_TEXTURE_CUBE_MAP               0x8513
#define XNG_TEXTURE_BINDING_CUBE_MAP       0x8514
#define XNG_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define XNG_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define XNG_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define XNG_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define XNG_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define XNG_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define XNG_PROXY_TEXTURE_CUBE_MAP         0x851B
#define XNG_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define XNG_COMBINE                        0x8570
#define XNG_COMBINE_RGB                    0x8571
#define XNG_COMBINE_ALPHA                  0x8572
#define XNG_RGB_SCALE                      0x8573
#define XNG_ADD_SIGNED                     0x8574
#define XNG_INTERPOLATE                    0x8575
#define XNG_CONSTANT                       0x8576
#define XNG_PRIMARY_COLOR                  0x8577
#define XNG_PREVIOUS                       0x8578
#define XNG_SOURCE0_RGB                    0x8580
#define XNG_SOURCE1_RGB                    0x8581
#define XNG_SOURCE2_RGB                    0x8582
#define XNG_SOURCE0_ALPHA                  0x8588
#define XNG_SOURCE1_ALPHA                  0x8589
#define XNG_SOURCE2_ALPHA                  0x858A
#define XNG_OPERAND0_RGB                   0x8590
#define XNG_OPERAND1_RGB                   0x8591
#define XNG_OPERAND2_RGB                   0x8592
#define XNG_OPERAND0_ALPHA                 0x8598
#define XNG_OPERAND1_ALPHA                 0x8599
#define XNG_OPERAND2_ALPHA                 0x859A
#define XNG_SUBTRACT                       0x84E7
#define XNG_TRANSPOSE_MODELVIEW_MATRIX     0x84E3
#define XNG_TRANSPOSE_PROJECTION_MATRIX    0x84E4
#define XNG_TRANSPOSE_TEXTURE_MATRIX       0x84E5
#define XNG_TRANSPOSE_COLOR_MATRIX         0x84E6
*/
#define XNG_COMPRESSED_ALPHA               0x84E9
#define XNG_COMPRESSED_LUMINANCE           0x84EA
#define XNG_COMPRESSED_LUMINANCE_ALPHA     0x84EB
#define XNG_COMPRESSED_INTENSITY           0x84EC
#define XNG_COMPRESSED_RGB                 0x84ED
#define XNG_COMPRESSED_RGBA                0x84EE
/*
#define XNG_TEXTURE_COMPRESSION_HINT       0x84EF
#define XNG_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define XNG_TEXTURE_COMPRESSED             0x86A1
#define XNG_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define XNG_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define XNG_DOT3_RGB                       0x86AE
#define XNG_DOT3_RGBA                      0x86AF
#define XNG_CLAMP_TO_BORDER                0x812D
#define XNG_MULTISAMPLE                    0x809D
#define XNG_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define XNG_SAMPLE_ALPHA_TO_ONE            0x809F
#define XNG_SAMPLE_COVERAGE                0x80A0
#define XNG_SAMPLE_BUFFERS                 0x80A8
#define XNG_SAMPLES                        0x80A9
#define XNG_SAMPLE_COVERAGE_VALUE          0x80AA
#define XNG_SAMPLE_COVERAGE_INVERT         0x80AB
#define XNG_MULTISAMPLE_BIT                0x20000000
*/


/* ------------------------- XNG_EXT_texture_integer ------------------------ */

#ifndef XNG_EXT_texture_integer
#define XNG_EXT_texture_integer 1

#define XNG_RGBA32UI_EXT 0x8D70
#define XNG_RGB32UI_EXT 0x8D71
#define XNG_ALPHA32UI_EXT 0x8D72
#define XNG_INTENSITY32UI_EXT 0x8D73
#define XNG_LUMINANCE32UI_EXT 0x8D74
#define XNG_LUMINANCE_ALPHA32UI_EXT 0x8D75
#define XNG_RGBA16UI_EXT 0x8D76
#define XNG_RGB16UI_EXT 0x8D77
#define XNG_ALPHA16UI_EXT 0x8D78
#define XNG_INTENSITY16UI_EXT 0x8D79
#define XNG_LUMINANCE16UI_EXT 0x8D7A
#define XNG_LUMINANCE_ALPHA16UI_EXT 0x8D7B
#define XNG_RGBA8UI_EXT 0x8D7C
#define XNG_RGB8UI_EXT 0x8D7D
#define XNG_ALPHA8UI_EXT 0x8D7E
#define XNG_INTENSITY8UI_EXT 0x8D7F
#define XNG_LUMINANCE8UI_EXT 0x8D80
#define XNG_LUMINANCE_ALPHA8UI_EXT 0x8D81
#define XNG_RGBA32I_EXT 0x8D82
#define XNG_RGB32I_EXT 0x8D83
#define XNG_ALPHA32I_EXT 0x8D84
#define XNG_INTENSITY32I_EXT 0x8D85
#define XNG_LUMINANCE32I_EXT 0x8D86
#define XNG_LUMINANCE_ALPHA32I_EXT 0x8D87
#define XNG_RGBA16I_EXT 0x8D88
#define XNG_RGB16I_EXT 0x8D89
#define XNG_ALPHA16I_EXT 0x8D8A
#define XNG_INTENSITY16I_EXT 0x8D8B
#define XNG_LUMINANCE16I_EXT 0x8D8C
#define XNG_LUMINANCE_ALPHA16I_EXT 0x8D8D
#define XNG_RGBA8I_EXT 0x8D8E
#define XNG_RGB8I_EXT 0x8D8F
#define XNG_ALPHA8I_EXT 0x8D90
#define XNG_INTENSITY8I_EXT 0x8D91
#define XNG_LUMINANCE8I_EXT 0x8D92
#define XNG_LUMINANCE_ALPHA8I_EXT 0x8D93
#define XNG_RED_INTEGER_EXT 0x8D94
#define XNG_GREEN_INTEGER_EXT 0x8D95
#define XNG_BLUE_INTEGER_EXT 0x8D96
#define XNG_ALPHA_INTEGER_EXT 0x8D97
#define XNG_RGB_INTEGER_EXT 0x8D98
#define XNG_RGBA_INTEGER_EXT 0x8D99
#define XNG_BGR_INTEGER_EXT 0x8D9A
#define XNG_BGRA_INTEGER_EXT 0x8D9B
#define XNG_LUMINANCE_INTEGER_EXT 0x8D9C
#define XNG_LUMINANCE_ALPHA_INTEGER_EXT 0x8D9D
#define XNG_RGBA_INTEGER_MODE_EXT 0x8D9E

#endif

//texture compression types

/* -------------------- XNG_EXT_texture_compression_rgtc -------------------- */

#ifndef XNG_EXT_texture_compression_rgtc
#define XNG_EXT_texture_compression_rgtc 1

#define XNG_COMPRESSED_RED_RGTC1_EXT 0x8DBB
#define XNG_COMPRESSED_SIGNED_RED_RGTC1_EXT 0x8DBC
#define XNG_COMPRESSED_RED_GREEN_RGTC2_EXT 0x8DBD
#define XNG_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT 0x8DBE

#endif /* XNG_EXT_texture_compression_rgtc */

/* -------------------- XNG_EXT_texture_compression_s3tc -------------------- */

#ifndef XNG_EXT_texture_compression_s3tc
#define XNG_EXT_texture_compression_s3tc 1

#define XNG_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define XNG_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define XNG_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define XNG_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

/* -------------------------- XNG_ARB_texture_float ------------------------- */

#ifndef XNG_ARB_texture_float
#define XNG_ARB_texture_float 1

#define XNG_RGBA32F_ARB 0x8814
#define XNG_RGB32F_ARB 0x8815
#define XNG_ALPHA32F_ARB 0x8816
#define XNG_INTENSITY32F_ARB 0x8817
#define XNG_LUMINANCE32F_ARB 0x8818
#define XNG_LUMINANCE_ALPHA32F_ARB 0x8819
#define XNG_RGBA16F_ARB 0x881A
#define XNG_RGB16F_ARB 0x881B
#define XNG_ALPHA16F_ARB 0x881C
#define XNG_INTENSITY16F_ARB 0x881D
#define XNG_LUMINANCE16F_ARB 0x881E
#define XNG_LUMINANCE_ALPHA16F_ARB 0x881F
#define XNG_TEXTURE_RED_TYPE_ARB 0x8C10
#define XNG_TEXTURE_GREEN_TYPE_ARB 0x8C11
#define XNG_TEXTURE_BLUE_TYPE_ARB 0x8C12
#define XNG_TEXTURE_ALPHA_TYPE_ARB 0x8C13
#define XNG_TEXTURE_LUMINANCE_TYPE_ARB 0x8C14
#define XNG_TEXTURE_INTENSITY_TYPE_ARB 0x8C15
#define XNG_TEXTURE_DEPTH_TYPE_ARB 0x8C16
#define XNG_UNSIGNED_NORMALIZED_ARB 0x8C17

//#define GLEW_ARB_texture_float GLEW_GET_VAR(__GLEW_ARB_texture_float)

#endif /* XNG_ARB_texture_float */

//#define GLEW_EXT_texture_compression_s3tc GLEW_GET_VAR(__GLEW_EXT_texture_compression_s3tc)

#endif /* XNG_EXT_texture_compression_s3tc */

/* ---------------------------- XNG_NV_half_float --------------------------- */

#ifndef XNG_NV_half_float
#define XNG_NV_half_float 1

#define XNG_HALF_FLOAT_NV 0x140B

#endif

/* -------------------------- XNG_NV_texture_shader ------------------------- */

#ifndef XNG_NV_texture_shader
#define XNG_NV_texture_shader 1

#define XNG_OFFSET_TEXTURE_RECTANGLE_NV 0x864C
#define XNG_OFFSET_TEXTURE_RECTANGLE_SCALE_NV 0x864D
#define XNG_DOT_PRODUCT_TEXTURE_RECTANGLE_NV 0x864E
#define XNG_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV 0x86D9
#define XNG_UNSIGNED_INT_S8_S8_8_8_NV 0x86DA
#define XNG_UNSIGNED_INT_8_8_S8_S8_REV_NV 0x86DB
#define XNG_DSDT_MAG_INTENSITY_NV 0x86DC
#define XNG_SHADER_CONSISTENT_NV 0x86DD
#define XNG_TEXTURE_SHADER_NV 0x86DE
#define XNG_SHADER_OPERATION_NV 0x86DF
#define XNG_CULL_MODES_NV 0x86E0
#define XNG_OFFSET_TEXTURE_2D_MATRIX_NV 0x86E1
#define XNG_OFFSET_TEXTURE_MATRIX_NV 0x86E1
#define XNG_OFFSET_TEXTURE_2D_SCALE_NV 0x86E2
#define XNG_OFFSET_TEXTURE_SCALE_NV 0x86E2
#define XNG_OFFSET_TEXTURE_2D_BIAS_NV 0x86E3
#define XNG_OFFSET_TEXTURE_BIAS_NV 0x86E3
#define XNG_PREVIOUS_TEXTURE_INPUT_NV 0x86E4
#define XNG_CONST_EYE_NV 0x86E5
#define XNG_PASS_THROUGH_NV 0x86E6
#define XNG_CULL_FRAGMENT_NV 0x86E7
#define XNG_OFFSET_TEXTURE_2D_NV 0x86E8
#define XNG_DEPENDENT_AR_TEXTURE_2D_NV 0x86E9
#define XNG_DEPENDENT_GB_TEXTURE_2D_NV 0x86EA
#define XNG_DOT_PRODUCT_NV 0x86EC
#define XNG_DOT_PRODUCT_DEPTH_REPLACE_NV 0x86ED
#define XNG_DOT_PRODUCT_TEXTURE_2D_NV 0x86EE
#define XNG_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV 0x86F0
#define XNG_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV 0x86F1
#define XNG_DOT_PRODUCT_REFLECT_CUBE_MAP_NV 0x86F2
#define XNG_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV 0x86F3
#define XNG_HILO_NV 0x86F4
#define XNG_DSDT_NV 0x86F5
#define XNG_DSDT_MAG_NV 0x86F6
#define XNG_DSDT_MAG_VIB_NV 0x86F7
#define XNG_HILO16_NV 0x86F8
#define XNG_SIGNED_HILO_NV 0x86F9
#define XNG_SIGNED_HILO16_NV 0x86FA
#define XNG_SIGNED_RGBA_NV 0x86FB
#define XNG_SIGNED_RGBA8_NV 0x86FC
#define XNG_SIGNED_RGB_NV 0x86FE
#define XNG_SIGNED_RGB8_NV 0x86FF
#define XNG_SIGNED_LUMINANCE_NV 0x8701
#define XNG_SIGNED_LUMINANCE8_NV 0x8702
#define XNG_SIGNED_LUMINANCE_ALPHA_NV 0x8703
#define XNG_SIGNED_LUMINANCE8_ALPHA8_NV 0x8704
#define XNG_SIGNED_ALPHA_NV 0x8705
#define XNG_SIGNED_ALPHA8_NV 0x8706
#define XNG_SIGNED_INTENSITY_NV 0x8707
#define XNG_SIGNED_INTENSITY8_NV 0x8708
#define XNG_DSDT8_NV 0x8709
#define XNG_DSDT8_MAG8_NV 0x870A
#define XNG_DSDT8_MAG8_INTENSITY8_NV 0x870B
#define XNG_SIGNED_RGB_UNSIGNED_ALPHA_NV 0x870C
#define XNG_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D
#define XNG_HI_SCALE_NV 0x870E
#define XNG_LO_SCALE_NV 0x870F
#define XNG_DS_SCALE_NV 0x8710
#define XNG_DT_SCALE_NV 0x8711
#define XNG_MAGNITUDE_SCALE_NV 0x8712
#define XNG_VIBRANCE_SCALE_NV 0x8713
#define XNG_HI_BIAS_NV 0x8714
#define XNG_LO_BIAS_NV 0x8715
#define XNG_DS_BIAS_NV 0x8716
#define XNG_DT_BIAS_NV 0x8717
#define XNG_MAGNITUDE_BIAS_NV 0x8718
#define XNG_VIBRANCE_BIAS_NV 0x8719
#define XNG_TEXTURE_BORDER_VALUES_NV 0x871A
#define XNG_TEXTURE_HI_SIZE_NV 0x871B
#define XNG_TEXTURE_LO_SIZE_NV 0x871C
#define XNG_TEXTURE_DS_SIZE_NV 0x871D
#define XNG_TEXTURE_DT_SIZE_NV 0x871E
#define XNG_TEXTURE_MAG_SIZE_NV 0x871F

#define GLEW_NV_texture_shader GLEW_GET_VAR(__GLEW_NV_texture_shader)

#endif /* XNG_NV_texture_shader */

/* ------------------------- XNG_NV_texture_shader2 ------------------------- */

#ifndef XNG_NV_texture_shader2
#define XNG_NV_texture_shader2 1

#define XNG_UNSIGNED_INT_S8_S8_8_8_NV 0x86DA
#define XNG_UNSIGNED_INT_8_8_S8_S8_REV_NV 0x86DB
#define XNG_DSDT_MAG_INTENSITY_NV 0x86DC
#define XNG_DOT_PRODUCT_TEXTURE_3D_NV 0x86EF
#define XNG_HILO_NV 0x86F4
#define XNG_DSDT_NV 0x86F5
#define XNG_DSDT_MAG_NV 0x86F6
#define XNG_DSDT_MAG_VIB_NV 0x86F7
#define XNG_HILO16_NV 0x86F8
#define XNG_SIGNED_HILO_NV 0x86F9
#define XNG_SIGNED_HILO16_NV 0x86FA
#define XNG_SIGNED_RGBA_NV 0x86FB
#define XNG_SIGNED_RGBA8_NV 0x86FC
#define XNG_SIGNED_RGB_NV 0x86FE
#define XNG_SIGNED_RGB8_NV 0x86FF
#define XNG_SIGNED_LUMINANCE_NV 0x8701
#define XNG_SIGNED_LUMINANCE8_NV 0x8702
#define XNG_SIGNED_LUMINANCE_ALPHA_NV 0x8703
#define XNG_SIGNED_LUMINANCE8_ALPHA8_NV 0x8704
#define XNG_SIGNED_ALPHA_NV 0x8705
#define XNG_SIGNED_ALPHA8_NV 0x8706
#define XNG_SIGNED_INTENSITY_NV 0x8707
#define XNG_SIGNED_INTENSITY8_NV 0x8708
#define XNG_DSDT8_NV 0x8709
#define XNG_DSDT8_MAG8_NV 0x870A
#define XNG_DSDT8_MAG8_INTENSITY8_NV 0x870B
#define XNG_SIGNED_RGB_UNSIGNED_ALPHA_NV 0x870C
#define XNG_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D

#define GLEW_NV_texture_shader2 GLEW_GET_VAR(__GLEW_NV_texture_shader2)

#endif /* XNG_NV_texture_shader2 */

/* ------------------------- XNG_NV_texture_shader3 ------------------------- */

#ifndef XNG_NV_texture_shader3
#define XNG_NV_texture_shader3 1

#define XNG_OFFSET_PROJECTIVE_TEXTURE_2D_NV 0x8850
#define XNG_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV 0x8851
#define XNG_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8852
#define XNG_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV 0x8853
#define XNG_OFFSET_HILO_TEXTURE_2D_NV 0x8854
#define XNG_OFFSET_HILO_TEXTURE_RECTANGLE_NV 0x8855
#define XNG_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV 0x8856
#define XNG_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8857
#define XNG_DEPENDENT_HILO_TEXTURE_2D_NV 0x8858
#define XNG_DEPENDENT_RGB_TEXTURE_3D_NV 0x8859
#define XNG_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV 0x885A
#define XNG_DOT_PRODUCT_PASS_THROUGH_NV 0x885B
#define XNG_DOT_PRODUCT_TEXTURE_1D_NV 0x885C
#define XNG_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV 0x885D
#define XNG_HILO8_NV 0x885E
#define XNG_SIGNED_HILO8_NV 0x885F
#define XNG_FORCE_BLUE_TO_ONE_NV 0x8860

#define GLEW_NV_texture_shader3 GLEW_GET_VAR(__GLEW_NV_texture_shader3)

#endif /* XNG_NV_texture_shader3 */


#pragma mark --- XNZ Image Formats

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
    ((unsigned int)(char)(ch0) | ((unsigned int)(char)(ch1) << 8) |   \
    ((unsigned int)(char)(ch2) << 16) | ((unsigned int)(char)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)


typedef struct xng_rgb_format
{
    const char* name;
    unsigned int         bitCount;
    unsigned int         rBitMask;
    unsigned int         gBitMask;
    unsigned int         bBitMask;
    unsigned int         aBitMask;
    unsigned int		 internalFormat;
    unsigned int		 pixelFormat;
    unsigned int		 dataType;
}xng_rgb_format;

static const unsigned int XNG_IMAGE_UNSUPPORTED_PIXEL_FORMAT = 0;
static const unsigned int XNG_IMAGE_UNSUPPORTED_PIXEL_TYPE = 0;

//all the formats we want to support for our image loaders (starting with .dds)
static const xng_rgb_format XNG_IMAGE_RGB_FORMATS[] =
{
    { "R3G3B2"     ,  8,       0xe0,       0x1c,       0x03,       0x00,
        XNG_RGB , XNG_RGB , XNG_UNSIGNED_BYTE_3_3_2 },

    { "R5G6B5"     , 16,     0xf800,     0x07e0,     0x001f,     0x0000,
        XNG_RGB , XNG_RGB , XNG_UNSIGNED_SHORT_5_6_5 },
    { "A1R5G5B5"   , 16,     0x7c00,     0x03e0,     0x001f,     0x8000,
        XNG_RGBA, XNG_BGRA, XNG_UNSIGNED_SHORT_1_5_5_5_REV },
    { "X1R5G5B5"   , 16,     0x7c00,     0x03e0,     0x001f,     0x0000,
        XNG_RGB , XNG_BGRA, XNG_UNSIGNED_SHORT_1_5_5_5_REV },
    { "A4R4G4B4"   , 16,     0x0f00,     0x00f0,     0x000f,     0xf000,
        XNG_RGBA, XNG_BGRA, XNG_UNSIGNED_SHORT_4_4_4_4_REV },
    { "X4R4G4B4"   , 16,     0x0f00,     0x00f0,     0x000f,     0x0000,
        XNG_RGB , XNG_BGRA, XNG_UNSIGNED_SHORT_4_4_4_4_REV },
    { "A8R3G3B2"   , 16,     0x00e0,     0x001c,     0x0003,     0xff00,
        XNG_RGBA, XNG_BGRA, 0 },

    { "R8G8B8",      24,   0xff0000,   0x00ff00,   0x0000ff,   0x000000,
        XNG_RGB , XNG_BGR , XNG_UNSIGNED_BYTE },

    { "B8G8R8",      24,   0x0000ff,   0x00ff00,   0xff0000,   0x000000,
        XNG_RGB , XNG_RGB , XNG_UNSIGNED_BYTE },

    { "A8R8G8B8",    32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000,
        XNG_RGBA, XNG_BGRA, XNG_UNSIGNED_BYTE },
    { "X8R8G8B8",    32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000,
        XNG_RGB , XNG_BGRA, XNG_UNSIGNED_BYTE },
    { "A8B8G8R8",    32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000,
        XNG_RGBA, XNG_RGBA, XNG_UNSIGNED_BYTE },
    { "X8B8G8R8",    32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000,
        XNG_RGB , XNG_RGBA, XNG_UNSIGNED_BYTE },
    { "A2R10G10B10", 32, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000,
        XNG_RGBA, XNG_BGRA, XNG_UNSIGNED_INT_2_10_10_10_REV },
    { "A2B10G10R10", 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000,
        XNG_RGBA, XNG_RGBA, XNG_UNSIGNED_INT_2_10_10_10_REV },
    { "G16R16",      32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000,
        XNG_RGB, 0, XNG_UNSIGNED_SHORT },
};


#ifdef __cplusplus
}
#endif


#endif //_XNG_IMAGE_FORMATS_H_
