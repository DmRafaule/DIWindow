#include <GL/glew.h>
#include "DIDebuger/debuger.hpp"
#include "DIDebuger/log.hpp"
#include "win.hpp"
#include "viewport.hpp"

namespace DI{
  DI::WinData* eg_winData;

   void WinHandler::Init(){
      DI_LOG_TRACE("WinHandler say: Init Win.");
      if (!glfwInit())
       DI_LOG_ERROR("%s","WinHandler say: Cannot Init glfw.");
      
      // Init all data
      eg_winData          = new WinData();
      eg_winData->bg      = glm::vec3(41.f/255.f,46.f/255.f,54.f/255.f);
      eg_winData->isMinimized = false;
      eg_winData->isOpen  = true;
      eg_winData->pos     = glm::vec2(300,300);
      eg_winData->size    = glm::vec2(1260,786);
      eg_winData->pointSize = 5.f;
      eg_winData->lineWidth = 5.f;
      eg_winData->title   = std::string("DI Game Engine");
      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
      glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_TRUE);
      glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
      eg_winData->win = glfwCreateWindow(eg_winData->size.x, eg_winData->size.y,eg_winData->title.c_str(),NULL,NULL);
      if (eg_winData->win == NULL)
         DI_LOG_ERROR("%s","WinHandler say: Cannot create window.");
      glfwMakeContextCurrent(eg_winData->win);

      // Pull out OpenGL defined functions from gpu
      if (GLEW_OK != glewInit()){
         DI_LOG_ERROR("%s",glewGetErrorString(glewInit()));
      }
      // Init 
      glEnable(GL_SCISSOR_TEST);
      // Init debug stuff
      glEnable(GL_DEBUG_OUTPUT);
      // Init testing depth of pixels
      glEnable(GL_DEPTH_TEST);
      // For line width
      glEnable(GL_LINE_SMOOTH);
      // For dot width
      glEnable(GL_PROGRAM_POINT_SIZE);
      // For transparency
      glEnable(GL_BLEND); 
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
      glBlendEquation(GL_FUNC_ADD);
      // Enable changing point size
      glDebugMessageCallback( ErrorCallback, 0 );
      // Init drawing mode
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
   void WinHandler::Kill(){
      DI_LOG_TRACE("WinHandler say: Kill Win.");
      glfwDestroyWindow(eg_winData->win);
      glfwTerminate();
      delete eg_winData;
   }

   void WinHandler::Update(){
      if (!eg_winData->isMinimized)
         for (int i = 0; i < eg_winData->viewports.size(); i++)
            if (eg_winData->viewports[i]->isActive){
               // This is needed for optimizing events dispatcher
               // For now will be here
               glfwPostEmptyEvent();
               ViewportHandler::Update((ViewportsSupportedID)i);
            }
      glfwSwapBuffers(eg_winData->win);
   }
   
} // namespace DI
