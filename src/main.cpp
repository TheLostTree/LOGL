#include <iostream>
//#include <fmt/format.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>


// Compile time config options
#define ENABLE_OPENGL_DEBUG 0


//todo: do a file thing and do file io to do it

// Raw string literal, syntax R"""()"""
//                             ^~~ some number of quotes

// https://www.jetbrains.com/help/clion/using-language-injections.html#use-language-injection-comments
// language=GLSL
const char *vertexShaderSource = R"""(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)""";

// language=GLSL
const char *fragmentShaderSource = R"""(
#version 330 core
out vec4 out_color;
void main()
{
    out_color = vec4(1.0,1.0,0.0,1.0)
}
)""";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

struct RenderState {
    /*
     */
    GLuint vao;

    // VBO is for vertex buffer objects
    GLuint vbo;
    GLuint shaderProgram;
};

void setupRenderState(RenderState &state) {
    //this is normalized between a range of -1 to 1
    //only points in the -1 to 1 range will be rendered/ shown
    float vertices[] = {
            //x     y     z
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    glGenVertexArrays(1, &state.vao);
    glBindVertexArray(state.vao);

    // glGenBuffers seems to take (GLsizei n, GLuint *buffers) as an argument
    // 1 refers to how many BufferObjectNames to be created
    // &VBO is an array in which the buffer names are stored
    // if you were to do glGenBuffers(2, ... ) you'd have to pass in an actual array
    // so theoretically you'd actually have to make an array with size of 2
    // but with this theres no point bc its just one
    // think of this as entityIds
    // you're retrieving arg[0] entity ids from a store of unused ones, and the method gives you that many
    // and they dont get released until you call glDeleteBuffers
    // they're not associated with any buffer until you call glBindBuffer
    glGenBuffers(1, &state.vbo);
    // this should just be a number
    // std::cout << state.vbo << std::endl;
    // you can only have one BufferObjectName bound to a certain type of enum
    // there's only one GL_ARRAY_BUFFER availible etc...
    // check the documentation for the rest of the types
    glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
    /*
        4th argument
        GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        GL_STATIC_DRAW: the data is set only once and used many times.
        GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBufferData is just a blob of data -- analogy byte[]
    // glVertexAttribPointer tells opengl the data layout inside the blob -- analogy struct { float x, y, z, ... }
    glEnableVertexAttribArray(0); // This vertex has vertex '0'
    glVertexAttribPointer(
            // The first attribute '0' of the vertex
            0,
            // Is a vector of 3 floats
            3, GL_FLOAT,
            // Do not normalize (divide by their max value to get into [0,1] range) the components' value
            GL_FALSE,
            // Each vertex has this many bytes
            // (this parameter tells OpenGL to move forward by this many bytes when getting the next vertex)
            sizeof(float) * 3,
            // This attribute starts at byte offset=0
            (void *) (uintptr_t) 0);

    glBindVertexArray(0);

    /*
    int num = getUserInput(); // glGenBuffer
    gGlobalVariable = num; // glBindBuffer
    someFunction(gGlobalVariable) // glBufferData
    */

    //another id thingie
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // *const*
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    state.shaderProgram = glCreateProgram();
    glAttachShader(state.shaderProgram, vertexShader);
    glAttachShader(state.shaderProgram, fragmentShader);
    glLinkProgram(state.shaderProgram);

    // NOTE: above we already called glLinkProgram, these shaders are "reference counted" by the shader program now, we can safely delete them
    // and at here it won't do anything
    // When we later delete the shader program, these objects will be automatically released (instead of having to manually call glDeleteShader
    // somewhere else in the code)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

#if ENABLE_OPENGL_DEBUG
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                         const void *userParam) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}
#endif

int main() {
    auto time = std::chrono::system_clock::now().time_since_epoch().count();

    std::cout << "time rn... " << time << std::endl;

    // init glfw
    glfwInit();

    // tell glfw that we're using ogl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // blame apple.
    // I think only doing this on apple system is recommended? I've never tried
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // resolution, title, null, null
    // those are for fullscreen stuff, so it's better to ignore it for now
    GLFWwindow *window = glfwCreateWindow(800, 600, "LOGLAttempt", nullptr, nullptr);


    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

#if ENABLE_OPENGL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&openGLDebugCallback, 0);
#endif

    // glfwGetProcAddress is the method that you use to get gl functions by name
    // gladLoadGLLoader should use that method to load everything

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // tells ogl what size your window is(or viewpoint, doesn't actually need to be the same as the window size);
    // ogl basically maps -1 to 1 to 0 to 800 and 0 to 600 for x and y respectively
    glViewport(0, 0, 800, 600);


    // set resize callback basically
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // NOTE this is a preprocessor trick to quickly switch between code for debugging purposes
    // 1 => true takes the first branch, 0 => false takes the second branch
#if 1
    // Disables VSync
    glfwSwapInterval(0);
#else
    // Enables VSync
    glfwSwapInterval(1);
#endif


    // NOTE: writing {} "value-initializes" the struct, i.e. call constructor if they exist or set primitive types to 0
    // (0 in OpenGL GLuint object ids always mean null)
    RenderState state{};
    setupRenderState(state);

    // frame loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // NOTE(tianyi): A blue-ish background color, it's just one that I always use, no particular significance
        // this shouldn't affect whether a buffer is successfully bound or not, but it's just good practice to clear the color and depth buffer before rendering
        // this also isn't needed if your scene fills every pixel on the screen
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // This is always good practice, to keep the Z early cull working
        glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(state.shaderProgram);
        glBindVertexArray(state.vao);
        glDrawArrays(GL_TRIANGLES, /*first vertex's offset, in array index*/0, /*primitive count*/3);

        //swap front and back pixel buffers?
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleans up glfw stuff that you did
    glfwTerminate();


    return 0;
}
