
#ifndef OPENGL_STUDY_SHADER_H
#define OPENGL_STUDY_SHADER_H

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>


class Shader
{
    private:
        unsigned int ID;
    
    public:
        Shader(const char *vertexShaderString, const char *fragmentShaderString);

        void use() const { glUseProgram(ID); }

        unsigned int getID() const { return ID; }

        unsigned int setBool(const std::string & name, const bool value) const;
        unsigned int setInt(const std::string & name, const int value) const;
        unsigned int setFloat(const std::string & name, const float value) const;

        unsigned int set3f(const std::string & name, 
            const float x, const float y, const float z) const;

        unsigned int set4f(const std::string & name, 
            const float x, const float y, const float z, const float w) const;

        unsigned int setVec3(const std::string & name, const glm::vec3 & vec) const;

        unsigned int setMatrix4fv(const std::string & name, const glm::mat4 value) const;
};

#endif // SHADER_H
