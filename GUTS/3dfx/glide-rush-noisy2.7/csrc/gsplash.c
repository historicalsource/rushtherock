/*
** Copyright (c) 1996, 3Dfx Interactive, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of 3Dfx Interactive, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of 3Dfx Interactive, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.
**
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GSPLASH.C 9     6/20/96 6:35p Garymct $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GSPLASH.C $
 * 
 * 9     6/20/96 6:35p Garymct
 * Updated splash screen. . .lower poly count.
 * 
 * 21    6/20/96 6:27p Garymct
 * 
 * 20    6/20/96 4:37p Garymct
 * 
 * 19    6/20/96 1:02a Garymct
 * antialiases only the edges that need to be.
 * 
 * 18    6/19/96 10:19p Garymct
 * 
 * 16    6/19/96 5:22p Garymct
 * 
 * 15    6/19/96 12:36p Garymct
 * Got rid of some polys that were nearly zero area to make antialiasing
 * work.
 * 
 * 14    6/19/96 12:16a Garymct
 * 
 * 12    6/18/96 8:42p Garymct
 * 
 * 9     6/18/96 1:47p Garymct
 * Updated so that the matrix for a particular object is more accesible.
 * 
 * 8     6/18/96 11:58a Garymct
 * 
 * 4     6/18/96 10:49a Garymct
*/
#include <glide.h>
#include <math.h>

#define SPLASH_ANTIALIAS 1
#define DEBUG_SHADOWS 0
#define FADEIN_END_PERCENT ( 0.3f )
#define FADEOUT_BEGIN_PERCENT ( 0.8f )
#define MAX_NUM_VERTS 2556
#define SPLASH_PROFILE 0
#define MIN_GAMMA 0.6f

/*
 * Snap a vertex coordinate the nearest 1/16th the slow, portable way.
 * This'll need to be changed if performance becomes an issue.
 */
#define SNAP_COORD(v)   ( ( v ) = ( float )( ( long )( v * 16 ) ) / 16.0f )

/*
 * The vertex structure which is used to define the model
 * geometry.
 */
typedef struct
{
  float x, y, z;        /* object space coordinates */
  float nx, ny, nz;     /* object space vertex normal for lighting */
  float s, t;           /* pre-glide-ified texture coordinates */
} Vert;

typedef struct
{
  int v[3];             /* vertex indices into array of vertes for face */
  int mat_index;        /* material index */
  int aa_edge_flags;
} Face;

/* Object->View transform matrix */
typedef struct
{
  float data[4][4];
} Matrix;

typedef float Vector[3];

static int do_phong /*= 0*/;

static int pass;

/*
 * Define a light source direction for diffuse lighting.  Note
 * that this direction is not used for specular lighting since
 * specular lighting has been simplified such that the
 * light source is at the viewer's eye.
 */
/*Vector light = { -0.7071f, -0.7071f, 0.0f }; */
static const Vector light = { -0.57735f, -0.57735f, -0.57735f };

/*
 * A place to put verts that have been transformed from
 * object space to view space.
 */
static Vector transformed_verts[MAX_NUM_VERTS];

/*
 * A place to put vert normals that have been transformed
 * from object space to view space.
 */
static Vector transformed_norms[MAX_NUM_VERTS];

/*
 * The generated C-code data for the model/animation.
 */
#include "splshdat.c"

/*
 * Texture handle for the "3D" part of the model.
 */
static GrMipMapId_t text_mmid;

/*
 * Texture handle for the textured specular highlight.
 */
static GrMipMapId_t hilite_mmid;

/*
 * Texture handle for the shadow texture.
 */
static GrMipMapId_t shadow_mmid;

#if SPLASH_PROFILE
/*
 * Total number of antialiased edges.
 */
static int total_num_edges;
#endif

void DebugDrawTriangle( GrVertex *a, GrVertex *b, GrVertex *c,
                        FxBool aa_a, FxBool aa_b, FxBool aa_c )
{
  guColorCombineFunction( GR_COLORCOMBINE_CCRGB );
  grConstantColorValue( ~( FxU32 )0 );
  if( aa_a )
    grDrawLine( a, b );
  if( aa_b )
    grDrawLine( b, c );
  if( aa_c )
    grDrawLine( c, a );
#if SPLASH_PROFILE
  total_num_edges += aa_a + aa_b + aa_c;
#endif
}

/*#define grAADrawTriangle DebugDrawTriangle */


/*
 * Allocate and download a texture given a Gu3dfInfo.
 */
static GrMipMapId_t _DownloadTexture( Gu3dfInfo *info )
{
  GrMipMapId_t mmid;

  mmid = guTexAllocateMemory( 0, GR_MIPMAPLEVELMASK_BOTH,
                              info->header.width, info->header.height,
                              info->header.format,
                              GR_MIPMAP_NEAREST,
                              info->header.small_lod, info->header.large_lod,
                              info->header.aspect_ratio,
                              GR_TEXTURECLAMP_WRAP, GR_TEXTURECLAMP_WRAP,
                              GR_TEXTUREFILTER_BILINEAR, GR_TEXTUREFILTER_BILINEAR,
                              0.0F,
                              FXFALSE );

  if( mmid == GR_NULL_MIPMAP_HANDLE )
    {
      return mmid;
    }

  guTexDownloadMipMap( mmid, info->data, &info->table.nccTable );

  return mmid;
}

/*
 * Download all of the textures that are stored in the C-code
 * data.
 */
static void _DownloadTextures( void )
{
  Gu3dfInfo *info;

  /*
   * Download the texture for the "3D" part of the model.
   */
  info = ( Gu3dfInfo * )text_3dfinfo_raw;
  info->data = ( void * )text_3dfinfo_image;
  text_mmid = _DownloadTexture( info );

  /*
   * Download the texture for the specular highlight.
   */
  info = ( Gu3dfInfo * )hilite_3dfinfo_raw;
  info->data = ( void * )hilite_3dfinfo_image;
  hilite_mmid = _DownloadTexture( info );

  /*
   * Download the texture for the shadow.
   */
  info = ( Gu3dfInfo * )shadow_3dfinfo_raw;
  info->data = ( void * )shadow_3dfinfo_image;
  shadow_mmid = _DownloadTexture( info );
}

static void _TransformAndProject( int framenum, int objnum )
{
  Vector tmpvec;
  int vertnum;
  Matrix m;

  m = mat[framenum][objnum];

  /*
   * Transform and project verts.
   */
  for( vertnum = 0; vertnum < num_verts[objnum]; vertnum++ )
    {
      tmpvec[0] = vert[objnum][vertnum].x;
      tmpvec[1] = vert[objnum][vertnum].y;
      tmpvec[2] = vert[objnum][vertnum].z;

      /*
       * Transform the vert into view space.
       * transformed_vert = vert * mat
       */
      transformed_verts[vertnum][0] = 
        tmpvec[0] * m.data[0][0] +
        tmpvec[1] * m.data[1][0] +
        tmpvec[2] * m.data[2][0] + m.data[3][0];
      transformed_verts[vertnum][1] = 
        tmpvec[0] * m.data[0][1] +
        tmpvec[1] * m.data[1][1] +
        tmpvec[2] * m.data[2][1] + m.data[3][1];
      transformed_verts[vertnum][2] = 
        tmpvec[0] * m.data[0][2] +
        tmpvec[1] * m.data[1][2] +
        tmpvec[2] * m.data[2][2] + m.data[3][2];

      /*
       * Transform vertex normal for lighting.
       * Need to only transform the vert by the
       * rotational part of the matrix.
       */
      tmpvec[0] = vert[objnum][vertnum].nx;
      tmpvec[1] = vert[objnum][vertnum].ny;
      tmpvec[2] = vert[objnum][vertnum].nz;

      transformed_norms[vertnum][0] = 
        tmpvec[0] * m.data[0][0] +
        tmpvec[1] * m.data[1][0] +
        tmpvec[2] * m.data[2][0];

      transformed_norms[vertnum][1] = 
        tmpvec[0] * m.data[0][1] +
        tmpvec[1] * m.data[1][1] +
        tmpvec[2] * m.data[2][1];

      transformed_norms[vertnum][2] = 
        tmpvec[0] * m.data[0][2] +
        tmpvec[1] * m.data[1][2] +
        tmpvec[2] * m.data[2][2];

      /*
       * Project the vert.
       */
      transformed_verts[vertnum][0] = transformed_verts[vertnum][0] / transformed_verts[vertnum][2] * 480.0f + 320;
      transformed_verts[vertnum][1] = transformed_verts[vertnum][1] / transformed_verts[vertnum][2] * 480.0f + 240;

      SNAP_COORD( transformed_verts[vertnum][0] );
      SNAP_COORD( transformed_verts[vertnum][1] );
    }
}

static void _SetupMaterial( int material_index )
{
  switch( material_index )
    {
    case 0:
      /* 3d */
      if( pass == 1 )
        {
          guTexSource( text_mmid );
          guColorCombineFunction( GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB );
          do_phong = 1;
        }
      else
        {
          guTexSource( hilite_mmid );
          guColorCombineFunction( GR_COLORCOMBINE_DECAL_TEXTURE );
        }
      break;
    case 1:
      /* fx */
      guTexSource( hilite_mmid );
      guColorCombineFunction( GR_COLORCOMBINE_DECAL_TEXTURE );
      do_phong = 1;
      break;
    case 2:
    case 3:
    case 4:
      guColorCombineFunction( GR_COLORCOMBINE_ITRGB );
      do_phong = 0;
      break;
    }
}

void _CalculateIntensity( int material_index, Vector intensity_factor, int frame )
{
  switch( material_index )
    {
    case 0:
      /* 3d */
      intensity_factor[0] = 1.0f;
      intensity_factor[1] = 1.0f;
      intensity_factor[2] = 1.0f;
      break;
    case 1:
      /* fx */
      intensity_factor[0] = 0.125f;
      intensity_factor[1] = 0.125f;
      intensity_factor[2] = 0.125f;
      break;
    case 2:
      /* cyan */
      intensity_factor[0] = ( 10.0f / 255.0f );
      intensity_factor[1] = ( 75.0f / 255.0f );
      intensity_factor[2] = ( 120.0f / 255.0f );
      break;
    case 3:
      /* white */
      intensity_factor[0] = 1.0f;
      intensity_factor[1] = 1.0f;
      intensity_factor[2] = 1.0f;
      break;
    case 4:
      /* yellow */
      intensity_factor[0] = ( 248.0f / 255.0f );
      intensity_factor[1] = ( 204.0f / 255.0f );
      intensity_factor[2] = 0.0f;
      break;
    }
}

static int prev_mat_index;

static void _DrawFaces( int frame, int objnum )
{
  int facenum;
  int material_index;
  float intensity_factor[3];
  GrVertex gvert[3];
/*static int prev_mat_index = 0xffff;*/
  int i;
  FxBool aa_a, aa_b, aa_c;

  for( facenum = 0; facenum < num_faces[objnum]; facenum++ )
    {
      material_index = face[objnum][facenum].mat_index;
      if( material_index != prev_mat_index )
        {
          _SetupMaterial( material_index );
          _CalculateIntensity( material_index,
                               intensity_factor, frame );
          prev_mat_index = material_index;
        }

      if( ( material_index != 0 ) && ( pass == 2 ) )
        continue;
          
#if SPLASH_ANTIALIAS
      aa_a = aa_b = aa_c = FXFALSE;
      if( face[objnum][facenum].aa_edge_flags & 4 )
        aa_a = FXTRUE;
      if( face[objnum][facenum].aa_edge_flags & 2 )
        aa_b = FXTRUE;
      if( face[objnum][facenum].aa_edge_flags & 1 )
        aa_c = FXTRUE;
#endif

      for( i = 0; i < 3; i++ )
        {
          float *transformed_vert, *transformed_norm;
          const Vert *v;
          int vertnum;

          vertnum = face[objnum][facenum].v[i];
          transformed_vert = transformed_verts[vertnum];
          transformed_norm = transformed_norms[vertnum];
          v = &vert[objnum][vertnum];

          gvert[i].x = transformed_vert[0];
          gvert[i].y = transformed_vert[1];
          gvert[i].oow = 1.0f / transformed_vert[2];
          gvert[i].tmuvtx[0].oow = gvert[i].oow;
          gvert[i].tmuvtx[0].sow = v->s * gvert[i].oow;
          gvert[i].tmuvtx[0].tow = v->t * gvert[i].oow;

          gvert[i].r = 
            ( ( light[0] * transformed_norm[0] + 
                light[1] * transformed_norm[1] + 
                light[2] * transformed_norm[2] ) + 1.0f ) * 127.5f * 
            intensity_factor[0];
          gvert[i].g = 
            ( ( light[0] * transformed_norm[0] + 
                light[1] * transformed_norm[1] + 
                light[2] * transformed_norm[2] ) + 1.0f ) * 127.5f * 
            intensity_factor[1];
          gvert[i].b = 
            ( ( light[0] * transformed_norm[0] + 
                light[1] * transformed_norm[1] + 
                light[2] * transformed_norm[2] ) + 1.0f ) * 127.5f * 
            intensity_factor[2];

          gvert[i].a = 255.0f;
        }

      if( pass == 2 )
        {
          for( i = 0; i < 3; i++ )
            {
              float *transformed_norm;

              transformed_norm = transformed_norms[face[objnum][facenum].v[i]];

              gvert[i].tmuvtx[0].sow = gvert[i].oow * 
                ( 128.0f + transformed_norm[0] * 128.0f );
              gvert[i].tmuvtx[0].tow = gvert[i].oow * 
                ( 128.0f + transformed_norm[1] * 128.0f );

              gvert[i].r = intensity_factor[0] * 255.0f;
              gvert[i].g = intensity_factor[1] * 255.0f;
              gvert[i].b = intensity_factor[2] * 255.0f;
            }                 

          grDrawTriangle( &gvert[0], &gvert[1], &gvert[2] );

          continue;
        }

      /* Do this if 3D. */
      if( material_index == 0 )
        {
          /*
           * Draw the textured 3D without specular. 
           */
          guTexSource( text_mmid );
          guColorCombineFunction( GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB );
#if SPLASH_ANTIALIAS
          grAADrawTriangle( &gvert[0], &gvert[1], &gvert[2], aa_a, aa_b, aa_c );
#else
          grDrawTriangle( &gvert[0], &gvert[1], &gvert[2] );
#endif
        }
      else if( material_index != 1 )
        {
          /*
           * Just go ahead and draw for things that don't
           * have specular highlights.
           */
#if SPLASH_ANTIALIAS
          grAADrawTriangle( &gvert[0], &gvert[1], &gvert[2], aa_a, aa_b, aa_c );
#else
          grDrawTriangle( &gvert[0], &gvert[1], &gvert[2] );
#endif
        }

      if( do_phong && ( material_index != 0 ) )
        {
          /* 
           * figure out texture coords in [0..255.0f] based on the normal
           * the specular highlight.
           */
          for( i = 0; i < 3; i++ )
            {
              float *transformed_norm;

              transformed_norm = transformed_norms[face[objnum][facenum].v[i]];

              gvert[i].tmuvtx[0].sow = gvert[i].oow * 
                ( 128.0f + transformed_norm[0] * 128.0f );
              gvert[i].tmuvtx[0].tow = gvert[i].oow * 
                ( 128.0f + transformed_norm[1] * 128.0f );

              gvert[i].r = intensity_factor[0] * 255.0f;
              gvert[i].g = intensity_factor[1] * 255.0f;
              gvert[i].b = intensity_factor[2] * 255.0f;
            }                 

#if SPLASH_ANTIALIAS
          grAADrawTriangle( &gvert[0], &gvert[1], &gvert[2], aa_a, aa_b, aa_c );
#else
          grDrawTriangle( &gvert[0], &gvert[1], &gvert[2] );
#endif
        }
    }
}

static void _IntersectLineWithZPlane( Vector result, Vector p1, 
                                      Vector p2, float z )
{
  float t;

  t = ( z - p1[2] ) / ( p2[2] - p1[2] );
  result[0] = p1[0] + ( p2[0] - p1[0] ) * t;
  result[1] = p1[1] + ( p2[1] - p1[1] ) * t;
  result[2] = z;
}

static float _VectorMag( Vector v )
{
  return ( float )sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] );
}

static void _DrawShadow( int frame, int shadow_object_index, 
                         int receiver_object_index, Vector light_position )
{
  const Matrix *shadow_object_matrix;
  const Matrix *receiver_object_matrix;
  Vector view_verts[4];
  Vector projected_view_verts[4];
  int i;
#if DEBUG_SHADOWS
  int level;
#endif
  Vector local_verts[4] = {
    { -280.0f, 0.0f, -160.0f },
    { -280.0f, 0.0f,  150.0f },
    {  280.0f, 0.0f,  150.0f },
    {  280.0f, 0.0f, -160.0f } 
  };
  float texcoords[4][2] = {
    {  10.5f, 127.5f },
    {  10.5f,   0.5f },
    { 255.0f,   0.5f },
    { 255.0f, 127.5f }
  };
  GrVertex gvert[4];
  GrVertex projected_gvert[4];
  GrVertex light_gvert;
  /*
   * The point relative to the back part of the shield that 
   * the shadow is going to be projected onto.
   */
  /*  Vector shadow_light = { 200.0f, 200.0f, -200.0f }; */
  /*  Vector shadow_light = { 5.0f, 150.0f, -1000.0f }; */
  Vector shadow_light;
  
  shadow_light[0] = light_position[0];
  shadow_light[1] = light_position[1];
  shadow_light[2] = light_position[2];

  shadow_object_matrix   = &mat[frame][shadow_object_index];
  receiver_object_matrix = &mat[frame][receiver_object_index];

  /*
   * Offset the light by the Z position of the backplane that we are projected
   * onto.
   */
  shadow_light[2] += receiver_object_matrix->data[3][2];

  for( i = 0; i < 4; i++ )
    {
      view_verts[i][0] = 
        local_verts[i][0] * shadow_object_matrix->data[0][0] +
        local_verts[i][1] * shadow_object_matrix->data[1][0] +
        local_verts[i][2] * shadow_object_matrix->data[2][0] + 
        shadow_object_matrix->data[3][0];
      view_verts[i][1] = 
        local_verts[i][0] * shadow_object_matrix->data[0][1] +
        local_verts[i][1] * shadow_object_matrix->data[1][1] +
        local_verts[i][2] * shadow_object_matrix->data[2][1] + 
        shadow_object_matrix->data[3][1];
      view_verts[i][2] = 
        local_verts[i][0] * shadow_object_matrix->data[0][2] +
        local_verts[i][1] * shadow_object_matrix->data[1][2] +
        local_verts[i][2] * shadow_object_matrix->data[2][2] + 
        shadow_object_matrix->data[3][2];
          
      /*
       * project . . . 
       */
      gvert[i].x = view_verts[i][0] / view_verts[i][2] * 480.0f + 320.0f;
      gvert[i].y = view_verts[i][1] / view_verts[i][2] * 480.0f + 240.0f;
      SNAP_COORD( gvert[i].x );
      SNAP_COORD( gvert[i].y );
      gvert[i].oow = 1.0f / view_verts[i][2];

      /*
       * Set up texture coordinates.
       */
      gvert[i].tmuvtx[0].sow = texcoords[i][0] * gvert[i].oow;
      gvert[i].tmuvtx[0].tow = texcoords[i][1] * gvert[i].oow;
      gvert[i].tmuvtx[0].oow = gvert[i].oow;
    }
  
#if DEBUG_SHADOWS
  /*
   * Draw the shadow object.
   */
  guColorCombineFunction( GR_COLORCOMBINE_DECAL_TEXTURE );
  guTexSource( shadow_mmid );
  grConstantColorValue( level * 40 );
  guDrawTriangleWithClip( &gvert[0], &gvert[1], &gvert[2] );
  guDrawTriangleWithClip( &gvert[0], &gvert[2], &gvert[3] );
  guDrawTriangleWithClip( &gvert[0], &gvert[2], &gvert[1] );
  guDrawTriangleWithClip( &gvert[0], &gvert[3], &gvert[2] );
#endif

  /*
       * Intersect each line formed by the light source and a
       * particular corner of the shadow object with the 
       * plane which the texture is to be projected onto.
       */
  for( i = 0; i < 4; i++ )
    {
      Vector tmpvect;
      float q;

      _IntersectLineWithZPlane( projected_view_verts[i], shadow_light, view_verts[i], 
                                receiver_object_matrix->data[3][2] - 26.0f );
      projected_gvert[i].x = projected_view_verts[i][0] / projected_view_verts[i][2] *
        480.0f + 320.0f;
      projected_gvert[i].y = projected_view_verts[i][1] / projected_view_verts[i][2] *
        480.0f + 240.0f;
      SNAP_COORD( projected_gvert[i].x );
      SNAP_COORD( projected_gvert[i].y );
      projected_gvert[i].oow = 1.0f / projected_view_verts[i][2];

      tmpvect[0] = projected_view_verts[i][0] - shadow_light[0];
      tmpvect[1] = projected_view_verts[i][1] - shadow_light[1];
      tmpvect[2] = projected_view_verts[i][2] - shadow_light[2];
      q = _VectorMag( tmpvect );

      /*
       * Set up texture coordinates.
       */
      projected_gvert[i].tmuvtx[0].oow = projected_gvert[i].oow * q;
      projected_gvert[i].tmuvtx[0].sow = texcoords[i][0] * projected_gvert[i].oow;
      projected_gvert[i].tmuvtx[0].tow = texcoords[i][1] * projected_gvert[i].oow;
    }

  light_gvert.x = shadow_light[0] / shadow_light[2] * 480.0f + 320.0f;
  light_gvert.y = shadow_light[1] / shadow_light[2] * 480.0f + 240.0f;
  SNAP_COORD( light_gvert.x );
  SNAP_COORD( light_gvert.y );
  light_gvert.oow = 1.0f / shadow_light[2];

      /*
       * Draw a segment between the light and the point which hits
       * the surface that the light is being projected onto.
       */
  guColorCombineFunction( GR_COLORCOMBINE_DECAL_TEXTURE );
  guTexSource( shadow_mmid );

  grAlphaBlendFunction( GR_BLEND_DST_COLOR, GR_BLEND_ZERO,
                        GR_BLEND_ONE, GR_BLEND_ZERO );
      

  grDrawTriangle( &projected_gvert[0], 
                  &projected_gvert[1], 
                  &projected_gvert[2] );
  grDrawTriangle( &projected_gvert[0], 
                  &projected_gvert[2], 
                  &projected_gvert[3] );
  grDrawTriangle( &projected_gvert[0], 
                  &projected_gvert[2], 
                  &projected_gvert[1] );
  grDrawTriangle( &projected_gvert[0], 
                  &projected_gvert[3], 
                  &projected_gvert[2] );
      
  grAlphaBlendFunction( GR_BLEND_ONE, GR_BLEND_ZERO,
                        GR_BLEND_ONE, GR_BLEND_ZERO );

#if 0
  grConstantColorValue( ~( FxU32 )0 );
  for( i = 0; i < 4; i++ )
    {
      if( ( projected_gvert[i].x >= 0.0f ) &&
          ( projected_gvert[i].x <= 640.0f ) &&
          ( projected_gvert[i].y >= 0.0f ) &&
          ( projected_gvert[i].y <= 480.0f ) )
        {
          grDrawLine( &projected_gvert[i], &light_gvert );
        }
    }
#endif
}

void grSplash( void )
{
  int frame;
  Vector lights[] = {
    { 5.0f, 300.0f, -1500.0f },
    { 5.0f, 150.0f, -1000.0f },
    { -30.0f, 150.0f, -1000.0f },
    { -30.0f, 100.0f, -1000.0f },
    { 30.0f, 70.0f, -1000.0f },
  };
  int num_lights = 1;
  int lightnum;
  float gamma;

  prev_mat_index = 0xffff;
  _DownloadTextures();

  guAlphaSource( GR_ALPHASOURCE_ITERATED_ALPHA );
  grAlphaBlendFunction( GR_BLEND_ONE, GR_BLEND_ZERO,
                        GR_BLEND_ONE, GR_BLEND_ZERO );

  grDepthBufferFunction( GR_CMP_LEQUAL );

  /*  grRenderBuffer( GR_BUFFER_FRONTBUFFER ); */

  for( frame = 1; frame < total_num_frames; frame++ )
    {
      /*
       * Set the gamma value for this frame.
       */
      if( frame < total_num_frames * FADEIN_END_PERCENT )
        {
          gamma = 1.70f * ( frame + 1 ) / ( total_num_frames * FADEIN_END_PERCENT );
        }
      else if( frame > ( total_num_frames * FADEOUT_BEGIN_PERCENT ) )
        {
          gamma = 1.70f * ( total_num_frames - frame ) / 
            ( total_num_frames - 
              ( FADEOUT_BEGIN_PERCENT ) * total_num_frames );
        }
      else
        {
          gamma = 1.70f;
        }

      grGammaCorrectionValue( gamma );

#if SPLASH_PROFILE
      /*
       * Reset the total number of antialiased edges.
       */
      total_num_edges = 0;
#endif

      grBufferClear( 0x00000000, 0, GR_WDEPTHVALUE_FARTHEST );

      pass = 1;

      /*
       * Avoid Z-aliasing between the shield and the 3dfx by
       * not writing Z for the shield.
       */
      grDepthMask( FXFALSE );

      /* cyan part of shield */
      _TransformAndProject( frame, 2 );
      _DrawFaces( frame, 2 );

      /* yellow and white part of shield. */
      _TransformAndProject( frame, 0 );
      _DrawFaces( frame, 0 );

      /*
       * Reanable writes to the depth-buffer.
       */
      grDepthMask( FXTRUE );

      /*
       * Draw the shadow projected from the 3Dfx logo onto
       * the rest of the powershield.
       */
      grDepthBufferFunction( GR_CMP_ALWAYS );
      for( lightnum = 0; lightnum < num_lights; lightnum++ )
        {
          _DrawShadow( frame, 1, 0, lights[lightnum] );
        }
      grDepthBufferFunction( GR_CMP_LEQUAL );

      /* 3Dfx logo */
      _TransformAndProject( frame, 1 );
      _DrawFaces( frame, 1 );
      pass = 2;
      grAlphaBlendFunction( GR_BLEND_ONE, GR_BLEND_ONE, 
                            GR_BLEND_ONE, GR_BLEND_ZERO );
      _DrawFaces( frame, 1 );
      pass = 1;
      grAlphaBlendFunction( GR_BLEND_ONE, GR_BLEND_ZERO, 
                            GR_BLEND_ONE, GR_BLEND_ZERO );

      grBufferSwap( 1 );

      /*
       * Wait for the hardware to finish.
       */
      grSstIdle();
    }

  /*
   * Clean up after yourself!
   */
  grBufferClear( 0x00000000, 0, GR_WDEPTHVALUE_FARTHEST );
  grBufferSwap( 1 );
  grBufferClear( 0x00000000, 0, GR_WDEPTHVALUE_FARTHEST );
  grBufferSwap( 1 );
  grSstIdle();
  grGammaCorrectionValue( 1.70f );
}
