#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef WIN32
#include <windows.h>
#endif
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define u32 uint32_t
#define s32 int32_t
#define u8 uint8_t
#define s8 int8_t
#define u16 uint16_t
#define s16 int16_t


#define CLASS_PTR(klassName) \
class klassName; \
using klassName ## UPtr = std::unique_ptr<klassName>; \
using klassName ## Ptr = std::shared_ptr<klassName>; \
using klassName ## WPtr = std::weak_ptr<klassName>;

std::optional<std::string> LoadTextFile(const std::string& filename);
glm::vec3 GetAttenuationCoeff(float distance);
#endif // __COMMON_H__