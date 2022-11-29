//
// Created by admin on 11/28/2022.
//

#ifndef LOGLATTEMPT90_SHADER_H
#define LOGLATTEMPT90_SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int programId;

    Shader(const char* vertexShader, const char* fragmentShader);
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};


#endif //LOGLATTEMPT90_SHADER_H
