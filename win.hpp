#pragma once 

#include <glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DI{

   // For now supporting only one window but with many viewports
  struct ViewportData;

  struct WinData{
    // background color
    glm::vec3      bg;
    // Current size of window
    glm::vec2      size;
    // Starting window size
    glm::vec2      original_size;
    // Position of window
    glm::vec2      pos; 
    // Size of drawing points
    float          pointSize;
    // Size of drawing lines
    float          lineWidth;
    // Window title
    std::string    title;
    // GLFW specific
    GLFWwindow*    win;
    // Viewports 
    std::vector<ViewportData*> viewports;
    // Logic for window
    bool           isOpen;
    bool           isMinimized;
    bool           isFocued;
  };
  namespace WinHandler{
    void Init();
    void Kill();
    void Update();
  }



}; // namespace DI
