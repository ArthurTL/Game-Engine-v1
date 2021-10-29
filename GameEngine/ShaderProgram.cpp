#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const string& vertexFile, const string &fragmentFile) {
    vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
    fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    bindAttributes();
    glLinkProgram(programID);
    glValidateProgram(programID);
  
}


GLuint ShaderProgram::getUniformLocation(const string& uniformName) {
    const GLchar* uniform = (const GLchar*)uniformName.c_str();
    return glGetUniformLocation(programID, uniform);
}

void ShaderProgram::start() {
    glUseProgram(programID);
}

void ShaderProgram::stop() {
    glUseProgram(0);
}

void ShaderProgram::cleanUp() {
    stop();
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteProgram(programID);

}

void ShaderProgram::bindAttribute(GLuint attribute, const string& variableName) {
    const GLchar* variable = (const GLchar*)variableName.c_str();
    glBindAttribLocation(programID, attribute,variable);
}

void ShaderProgram::loadInt(GLuint location, int value) {
    glUniform1i(location, value);
}
void ShaderProgram::loadFloat(GLuint location, float value) {
    glUniform1f(location, value);
}

void ShaderProgram::loadVector(GLuint location, glm::vec3 vector) {
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::load2DVector(GLuint location, glm::vec2 vector) {
    glUniform2f(location, vector.x, vector.y);
}

void ShaderProgram::load4DVector(GLuint location, glm::vec4 vector) {
    glUniform4f(location, vector.x, vector.y, vector.z,vector.w);
}

void ShaderProgram::loadBoolean(GLuint location, bool value) {
    float toLoad = 0;
    if (value) toLoad = 1;
    glUniform1f(location, toLoad);
}



void ShaderProgram::loadMatrix(GLuint location, glm::mat4 matrix) {
    glUniformMatrix4fv(location,1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint ShaderProgram::loadShader(const string &file, GLuint type) {
    
    GLuint shader = glCreateShader(type);
    std::string shaderSourceString = file2String(file); 
    if (shaderSourceString.empty()) {
        std::cerr << "No content in shader " << file << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    const GLchar* shaderSource = (const GLchar*)shaderSourceString.c_str(); 
    glShaderSource(shader, 1, &shaderSource, NULL); 
    glCompileShader(shader);   
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLsizei len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        GLchar* log = new GLchar[len + 1];
        glGetShaderInfoLog(shader, len, &len, log);
        std::cerr << "Compilation error in shader " << file << " : " << std::endl << log << std::endl;
        delete[] log;
        glDeleteShader(shader);
        return 0;
    }
    return shader;

}

string ShaderProgram::file2String(const string &filename) {
    ifstream input(filename.c_str());
    if (!input)
        throw std::ios_base::failure("[Shader Program][file2String] Error: cannot open " + filename);
    stringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

void ShaderProgram::bindAttributes() {};

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(programID);
}
