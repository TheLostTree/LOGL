//
// Created by admin on 11/28/2022.
//

#include "Shader.h"
#include "glad/glad.h"

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
    std::ifstream vFile;
    std::ifstream fFile;
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    // ensure ifstream objects can throw exceptions:
    // today i learned!
    vFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try{
        //open file
        vFile.open(vertexShaderPath);
        fFile.open(fragmentShaderPath);
        //create string streams
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vFile.rdbuf();
        fShaderStream << fFile.rdbuf();
        // close file handlers
        vFile.close();
        fFile.close();

        // get file contents
        vertexShaderSource = vShaderStream.str();
        fragmentShaderSource = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    // needs to be a c str :/
    const char* vShaderCode = vertexShaderSource.c_str();
    const char* fShaderCode = fragmentShaderSource.c_str();

    int success;
    char infoLog[512];
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // *const*

    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);

    //check err
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // compile fragment shader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);
    //check err
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };


    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);

    //check err
    glGetShaderiv(programId, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(programId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    };

    // NOTE: above we already called glLinkProgram, these shaders are "reference counted" by the shader program now, we can safely delete them
    // and at here it won't do anything
    // When we later delete the shader program, these objects will be automatically released (instead of having to manually call glDeleteShader
    // somewhere else in the code)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use(){
    glad_glUseProgram(programId);
}
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), value);

}
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}