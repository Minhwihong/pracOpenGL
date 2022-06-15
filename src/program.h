#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

CLASS_PTR(Program)
class Program {
public:
    static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders);

    static std::unique_ptr<Program> Create(const std::string& vertShaderFilename,
        const std::string& fragShaderFilename);

    ~Program();
    uint32_t Get() const { return m_program; }
    void Use() const;

    // transfter integer
    void SetUniform(const std::string& name, int value) const;

    // transfer 4x4 matrix
    void SetUniform(const std::string& name, const glm::mat4& value) const;

    // transfer float value
    void SetUniform(const std::string& name, float value) const;

    void SetUniform(const std::string& name, const glm::vec2& value) const;

    // transfer 3d vector
    void SetUniform(const std::string& name, const glm::vec3& value) const;

    void SetUniform(const std::string& name, const glm::vec4& value) const;
    
private:
    Program() {}
    bool Link(
        const std::vector<ShaderPtr>& shaders);
    uint32_t m_program { 0 };
};

#endif // __PROGRAM_H__