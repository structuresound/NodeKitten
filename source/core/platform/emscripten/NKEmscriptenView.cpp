#include "NKEmscriptenView.h"

#if TARGET_EMSCRIPTEN

int sdlWidth {0}, sdlHeight {0};
int runTimes = 0;
bool sdlLoaded {0};

NKView *sdl_based_view;
SDL_Surface* screen;

#warning HARD CODING
#include "ExampleScene.h"

void one_iter();
extern "C" void drawTriangle(float *translation);

extern "C" {
extern void notify_js_load();
}

extern "C" int main(void){

    cout << "*** INIT NODE KITTEN ***" << endl;
    
    if (!sdlLoaded){

        if (sdlWidth == 0) {
            cout << "error using static screen size" << endl;
            sdlWidth = 800;
            sdlHeight = 600;
        }
        
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0)
        {
            screen = SDL_SetVideoMode(sdlWidth, sdlHeight, 0, SDL_OPENGL | SDL_RESIZABLE);
            if (screen == NULL)
            {
                cerr << "Could not set video mode: " << SDL_GetError() << endl;
                return 0;
            }
        }
        else
        {
            cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
            return 0;
        }
        
        
        sdl_based_view = new NKView(V2Make(sdlWidth, sdlHeight));
        sdl_based_view->setScene(make_shared<ExampleScene>(S2Make(sdl_based_view->size().width, sdl_based_view->size().height)));
        
        emscripten_set_main_loop(one_iter, 0, 1);

    }
    sdlLoaded = true;

     return 0;
}



#ifdef HELLO_TRIANGLE
void helloTriangleSetup(int width, int height);
void helloTriangleDraw(float *translation);
#endif

extern "C" int initGL(int width, int height)
{
     cout << "*** INIT GL ***" << endl;
    //initialise SDL
    sdlWidth = width;
    sdlHeight = height;
    
#ifdef HELLO_TRIANGLE
    helloTriangleSetup(width, height);
#else
    main();
#endif
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    glViewport(0, 0, width, height);
    
    return 1;
}

extern "C" void drawTriangle(float *translation)
{
    //fill the screen with the clear color
    glClear(GL_COLOR_BUFFER_BIT);
    
    //sdl_based_view->draw();
#ifdef HELLO_TRIANGLE
    helloTriangleDraw(translation);
#else
    sdl_based_view->draw();
#endif
    
    SDL_GL_SwapBuffers();
}

#ifdef HELLO_TRIANGLE

GLuint loadShader(GLenum type, const char *source)
{
    //create a shader
    GLuint shader = glCreateShader(type);
    if (shader == 0)
    {
        cerr << "Error creating shader" << endl;
        return 0;
    }
    
    //load the shader source to the shader object and compile it
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    //check if the shader compiled successfully
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        cerr << "Shader compilation error" << endl;
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

GLuint buildProgram(GLuint vertexShader, GLuint fragmentShader, const char * vertexPositionName)
{
    //create a GL program and link it
    GLuint programObject = glCreateProgram();
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glBindAttribLocation(programObject, 0, vertexPositionName);
    glLinkProgram(programObject);
    
    //check if the program linked successfully
    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        cerr << "Program link error" << endl;
        glDeleteProgram(programObject);
        return 0;
    }
    return programObject;
}

GLuint programObject;

GLfloat vVertices[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
};

GLint uniformOriginX {0}, uniformOriginY {0}, uniformZoom {0};

void helloTriangleSetup(int width, int height){
    const char vertexShaderSource[] =
    "attribute vec4 vPosition;		                     \n"
    "uniform float originX, originY;                     \n"
    "uniform float zoom;                                 \n"
    "varying vec3 color;                                 \n"
    "void main()                                         \n"
    "{                                                   \n"
    "   gl_Position = vPosition;                         \n"
    "   gl_Position.x = (originX + gl_Position.x) * zoom;\n"
    "   gl_Position.y = (originY + gl_Position.y) * zoom;\n"
    "   color = gl_Position.xyz + vec3(0.5);             \n"
    "}                                                   \n";
    
    const char fragmentShaderSource[] =
    "precision mediump float;                     \n"
    "varying vec3 color;                          \n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_FragColor = vec4 ( color, 1.0 );        \n"
    "}                                            \n";
    
    //load vertex and fragment shaders
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    programObject = buildProgram(vertexShader, fragmentShader, "vPosition");
    
    //save location of uniform variables
    uniformOriginX = glGetUniformLocation(programObject, "originX");
    uniformOriginY = glGetUniformLocation(programObject, "originY");
    uniformZoom = glGetUniformLocation(programObject, "zoom");
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    glViewport(0, 0, width, height);
    
}

void helloTriangleDraw(float *translation) {
    
    //fill the screen with the clear color
    glClear(GL_COLOR_BUFFER_BIT);
    
    //enable our shader program
    glUseProgram(programObject);
    //set up the translation
    glUniform1f(uniformOriginX, translation[0]);
    glUniform1f(uniformOriginY, translation[1]);
    glUniform1f(uniformZoom, translation[2]);
    //set up the vertices array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    //draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
}

#endif // HELLO TRIANGLE

// The "main loop" function.
void one_iter() {
    drawTriangle(nullptr);
}

#endif // TARGET_EMSCRIPTEN