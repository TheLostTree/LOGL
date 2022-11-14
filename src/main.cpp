#include <iostream>
#include <fmt/format.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
//todo: do a file thing and do file io to do it 
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // resolution, title, null null
    // those are for fullscreen stuff, so its better to ignore it for now
    GLFWwindow* window = glfwCreateWindow(800, 600, "LOGLAttempt", NULL, NULL);


    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);



    // glfwGetProcAddress is the method that you use to get gl functions by name
    // gladLoadGLLoader should use that method to load everything

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    } 

    // tells ogl what size your window is(or viewpoint, doesnt actually need to be the same as the window size);
    // ogl basically maps -1 to 1 to 0 to 800 and 0 to 600 for x and y respectively
    glViewport(0, 0, 800, 600);

    
    // set resize callback basically
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 


    // frame loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        //swap front and back pixel buffers?
        glfwSwapBuffers(window);
        glfwPollEvents();    
    } 

    // cleans up glfw stuff that you did
    glfwTerminate();

    
    
    

    return 0;
}

void renderThing(){
    //this is normalized between a range of -1 to 1
    //only points in the -1 to 1 range will be rendered/ shown
    float vertices[] = {
        //x     y     z
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };  


    // VBO is for vertex buffer objects 
    unsigned int VBO;

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
    glGenBuffers(1, &VBO);
    // this should just be a number
    std::cout << VBO << std::endl;
    // you can only have one BufferObjectName bound to a certain type of enum 
    // theres only one GL_ARRAY_BUFFER availible etc...
    // check the documentation for the rest of the types
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    /*
        4th argument
        GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        GL_STATIC_DRAW: the data is set only once and used many times.
        GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //another id thingie
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);


    // *const*
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
}