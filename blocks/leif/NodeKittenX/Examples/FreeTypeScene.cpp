//
// Created by Leif Shackelford on 4/25/15.
//

#include "FreeTypeScene.h"

#include "freetype-gl.h"
#include "edtaa3func.h"
#include "font-manager.h"
#include "vertex-buffer.h"
#include "text-buffer.h"
#include "markup.h"
#include "shader.h"
#include "mat4.h"

// ------------------------------------------------------- typedef & struct ---
typedef struct {
    float x, y, z;    // position
    float s, t;       // texture
    float r, g, b, a; // color
} vertex_t;


// ------------------------------------------------------- global variables ---
GLuint shader;
vertex_buffer_t *buffer;
texture_atlas_t * atlas = 0;
mat4  m_model, m_view, m_projection;

texture_font_t *font;

// --------------------------------------------------------------- add_text ---
vec4
add_text( vertex_buffer_t * buffer, texture_font_t * font,
          wchar_t * text, vec4 * color, vec2 * pen )
{
    vec4 bbox = {{0,0,0,0}};
    size_t i;
    float r = color->red, g = color->green, b = color->blue, a = color->alpha;
    for( i=0; i<wcslen(text); ++i )
    {
        texture_glyph_t *glyph = texture_font_get_glyph( font, text[i] );
        if( glyph != NULL )
        {
            float kerning = 0.0f;
            if( i > 0)
            {
                kerning = texture_glyph_get_kerning( glyph, text[i-1] );
            }
            pen->x += kerning;
            float x0  = (int)( pen->x + glyph->offset_x );
            float y0  = (int)( pen->y + glyph->offset_y );
            float x1  = (int)( x0 + glyph->width );
            float y1  = (int)( y0 - glyph->height );
            float s0 = glyph->s0;
            float t0 = glyph->t0;
            float s1 = glyph->s1;
            float t1 = glyph->t1;
            GLuint indices[6] = {0,1,2, 0,2,3};
            vertex_t vertices[4] = { { x0,y0,0,  s0,t0,  r,g,b,a },
                                     { x0,y1,0,  s0,t1,  r,g,b,a },
                                     { x1,y1,0,  s1,t1,  r,g,b,a },
                                     { x1,y0,0,  s1,t0,  r,g,b,a } };
            vertex_buffer_push_back( buffer, vertices, 4, indices, 6 );
            pen->x += glyph->advance_x;

            if  (x0 < bbox.x)                bbox.x = x0;
            if  (y1 < bbox.y)                bbox.y = y1;
            if ((x1 - bbox.x) > bbox.width)  bbox.width  = x1-bbox.x;
            if ((y0 - bbox.y) > bbox.height) bbox.height = y0-bbox.y;
        }
    }
    return bbox;
}


// ------------------------------------------------------ make_distance_map ---
unsigned char *
make_distance_map( unsigned char *img,
                   unsigned int width, unsigned int height )
{
    short * xdist = (short *)  malloc( width * height * sizeof(short) );
    short * ydist = (short *)  malloc( width * height * sizeof(short) );
    double * gx   = (double *) calloc( width * height, sizeof(double) );
    double * gy      = (double *) calloc( width * height, sizeof(double) );
    double * data    = (double *) calloc( width * height, sizeof(double) );
    double * outside = (double *) calloc( width * height, sizeof(double) );
    double * inside  = (double *) calloc( width * height, sizeof(double) );
    int i;

    // Convert img into double (data)
    double img_min = 255, img_max = -255;
    for( i=0; i<width*height; ++i)
    {
        double v = img[i];
        data[i] = v;
        if (v > img_max) img_max = v;
        if (v < img_min) img_min = v;
    }
    // Rescale image levels between 0 and 1
    for( i=0; i<width*height; ++i)
    {
        data[i] = (img[i]-img_min)/img_max;
    }

    // Compute outside = edtaa3(bitmap); % Transform background (0's)
    computegradient( data, width, height, gx, gy);
    edtaa3(data, gx, gy, width, height, xdist, ydist, outside);
    for( i=0; i<width*height; ++i)
        if( outside[i] < 0 )
            outside[i] = 0.0;

    // Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
    memset(gx, 0, sizeof(double)*width*height );
    memset(gy, 0, sizeof(double)*width*height );
    for( i=0; i<width*height; ++i)
        data[i] = 1 - data[i];
    computegradient( data, width, height, gx, gy);
    edtaa3(data, gx, gy, width, height, xdist, ydist, inside);
    for( i=0; i<width*height; ++i)
        if( inside[i] < 0 )
            inside[i] = 0.0;

    // distmap = outside - inside; % Bipolar distance field
    unsigned char *out = (unsigned char *) malloc( width * height * sizeof(unsigned char) );
    for( i=0; i<width*height; ++i)
    {
        outside[i] -= inside[i];
        outside[i] = 128+outside[i]*16;
        if( outside[i] < 0 ) outside[i] = 0;
        if( outside[i] > 255 ) outside[i] = 255;
        out[i] = 255 - (unsigned char) outside[i];
        //out[i] = (unsigned char) outside[i];
    }

    free( xdist );
    free( ydist );
    free( gx );
    free( gy );
    free( data );
    free( outside );
    free( inside );
    return out;
}

void FreeTypeScene::moveToView() {

    nkLog("freetype move to View");

    font = 0;
    atlas = texture_atlas_new( 512, 512, 1 );
    const char * filename = "fonts/Vera.ttf";
    wchar_t *text = L"A Quick Brown Fox Jumps Over The Lazy Dog 0123456789";
    buffer = vertex_buffer_new( "vertex:3f,tex_coord:2f,color:4f" );
    vec2 pen = {{0,0}};
    vec4 black = {{1,1,1,1}};
    font = texture_font_new_from_file( atlas, 48, filename );
    vec4 bbox = add_text( buffer, font, text, &black, &pen );
    size_t i;
    vector_t * vertices = buffer->vertices;
    for( i=0; i< vector_size(vertices); ++i )
    {
        vertex_t * vertex = (vertex_t *) vector_get(vertices,i);
        vertex->x -= (int)(bbox.x + bbox.width/2);
        vertex->y -= (int)(bbox.y + bbox.height/2);
    }


    glBindTexture( GL_TEXTURE_2D, atlas->id );
    nkLog("bind atlas texture : %d", atlas->id);

    fprintf( stderr, "Generating distance map...\n" );
    unsigned char *map = make_distance_map(atlas->data, atlas->width, atlas->height);
    fprintf( stderr, "done !\n");

    memcpy( atlas->data, map, atlas->width*atlas->height*sizeof(unsigned char) );
    free(map);
    texture_atlas_upload( atlas );

    shader = shader_load( "shaders/distance-field-2.vert",
                          "shaders/distance-field-2.frag" );
    nkLog("loaded shader: %d", shader);

    mat4_set_identity( &m_projection );
    mat4_set_identity( &m_model );
    mat4_set_identity( &m_view );

    mat4_set_orthographic( &m_projection, 0, _size.width, 0, _size.height, -1, 1);


    backgroundColor = NKColor(1);
}

static auto scales = 1.0;

void FreeTypeScene::customDraw() {

    this->setBlendMode(NKBlendModeAlpha);
    this->setUsesDepth(false);
    this->setCullFace(NKCullFaceBack);

    srand(4);
    vec4 color = {{0.067,0.333, 0.486, 1.0}};

    glUseProgram( shader );
    glBindTexture( GL_TEXTURE_2D, atlas->id );

    scales += .001;

    for(int i=0; i<40; ++i)
    {
        float scale = .25 + 4.75 * pow(rand()/(float)(RAND_MAX),2) * scales;
        float angle = 90*(rand()%2);
        float x = (.05 + .9*((rand() + NKTime::getCurrentTime())/(float)(RAND_MAX)))*_size.width;
        float y = (-.05 + .9*(rand()/(float)(RAND_MAX)))*_size.height + NKTime::getElapsedTime();
        float a =  0.1+.8*(pow((1.0-scale/5),2));

        mat4_set_identity( &m_model );
        mat4_rotate( &m_model, angle,0,0,1);
        mat4_scale( &m_model, scale, scale, 1);
        mat4_translate( &m_model, x, y, 0);

        glUniform1i( glGetUniformLocation( shader, "texture" ),
                     0 );
        glUniform4f( glGetUniformLocation( shader, "Color" ),
                     color.r, color.g, color.b, a);
        glUniformMatrix4fv( glGetUniformLocation( shader, "model" ),
                            1, 0, m_model.data);
        glUniformMatrix4fv( glGetUniformLocation( shader, "view" ),
                            1, 0, m_view.data);
        glUniformMatrix4fv( glGetUniformLocation( shader, "projection" ),
                            1, 0, m_projection.data);
        vertex_buffer_render( buffer, GL_TRIANGLES );
    }

}


void FreeTypeScene::handleEvent(NKEvent &event){

}