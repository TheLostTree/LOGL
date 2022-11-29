#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include "./Shader.h"

// Compile time config options
#define ENABLE_OPENGL_DEBUG 0


//todo: do a file thing and do file io to do it

// Raw string literal, syntax R"""()"""
//                             ^~~ some number of quotes

// https://www.jetbrains.com/help/clion/using-language-injections.html#use-language-injection-comments

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
};

void setupRenderState(RenderState &state) {
    //this is normalized between a range of -1 to 1
    //only points in the -1 to 1 range will be rendered/ shown

    float vertices[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top

    };

    glGenVertexArrays(1, &state.vao);
    glBindVertexArray(state.vao);

    // 1 refers to how many BufferObjectNames to be created
    // think of this as entityIds
    // you're retrieving arg[0] entity ids from a store of unused ones, and the method gives you that many
    // and they dont get released until you call glDeleteBuffers
    // they're not associated with any buffer until you call glBindBuffer
    glGenBuffers(1, &state.vbo);
    // you can only have one BufferObjectName bound to a certain type of enum
    // there's only one GL_ARRAY_BUFFER available etc...
    // check the documentation for the rest of the types
    glBindBuffer(GL_ARRAY_BUFFER, state.vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBufferData is just a blob of data -- analogy byte[]
    // glVertexAttribPointer tells opengl the data layout inside the blob -- analogy struct { float x, y, z, ... }

    //position data
    glVertexAttribPointer(
            // The first attribute '0' of the vertex
            0,
            // Is a vector of 3 floats
            3, GL_FLOAT,
            // Do not normalize (divide by their max value to get into [0,1] range) the components' value
            GL_FALSE,
            // Each vertex has this many bytes
            // (this parameter tells OpenGL to move forward by this many bytes when getting the next vertex)
            6 * sizeof(float),
            // This attribute starts at byte offset=0
            (void *) 0);
    glEnableVertexAttribArray(0); // This vertex has vertex '0'

    // color data
    glVertexAttribPointer(
            // second attribute 0
            1,
            // vector of 3 floats
            3, GL_FLOAT,
            // don't normalize
            GL_FALSE,
            // stride = 6 floats
            6 * sizeof(float),
            // starting position in the array (skip 3 floats)
            (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
//    glBindVertexArray(0);

    /*
    ANALOGY!
    int num = getUserInput(); // glGenBuffer
    gGlobalVariable = num; // glBindBuffer
    someFunction(gGlobalVariable) // glBufferData
    */

}

#if ENABLE_OPENGL_DEBUG
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                         const void *userParam) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}
#endif

int main() {
    std::filesystem::path cwd = std::filesystem::current_path();
    printf("currently running in the %s directory", cwd.string().c_str());

    // init glfw
    glfwInit();
    // tell glfw that we're using ogl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#if 1
    // Disables VSync
    // basically how many screen updates to wait per update
    glfwSwapInterval(0);
#else
    // Enables VSync
    glfwSwapInterval(1);
#endif


    // NOTE: writing {} "value-initializes" the struct, i.e. call constructor if they exist or set primitive types to 0
    // (0 in OpenGL GLuint object ids always mean null)
    RenderState state{};
    setupRenderState(state);

    Shader basic = Shader("../shaders/shader.vs.glsl", "../shaders/shader.fs.glsl");

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



//        glUseProgram(state.shaderProgram);
        basic.use();
//      basic.setFloat("someUniform", 1.0f); theoretically



        //now render.
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
