#include "viewport.hpp"
#include "win.hpp"
#include <DIDebuger/debuger.hpp>
#include <DIDebuger/log.hpp>
#include "DIUtils/utils.hpp"
#include <glfw3.h>

namespace DI{

    extern WinData* eg_winData;
    
    namespace ViewportHandler{

        void UpdateToGetBigPart(float& dataToChange, const float dataToUse, const float size);
        void UpdateToGetSmallPart(float& dataToChange, const float dataToUse, const float size);

        void Init(){
            DI_LOG_TRACE("ViewportHandler say: Init Viewports.");
            Create(ViewportsSupportedID::MAIN_VIEWPORT);
            SetToFullWindowSize(ViewportsSupportedID::MAIN_VIEWPORT);
            Create(ViewportsSupportedID::VIEWPORT_ONE);
            Create(ViewportsSupportedID::VIEWPORT_TWO);
            Create(ViewportsSupportedID::VIEWPORT_THREE);
            Create(ViewportsSupportedID::VIEWPORT_FOUR);
        }

        void Kill(){
            DI_LOG_TRACE("ViewportHandler say: Kill Viewports.");
            for (int i = 0; i < eg_winData->viewports.size(); i++)
                delete eg_winData->viewports[i];
            eg_winData->viewports.clear();
        }

        void Create(ViewportsSupportedID id){
            ViewportData* vd = new ViewportData();
            vd->isActive = true;
            vd->x = 0;
            vd->y = 0;
            vd->w = 0;
            vd->h = 0;
            //DEBUG_ONLY.
            vd->bg.r = Utils::GetRandFromZeroToOne();
            vd->bg.g = Utils::GetRandFromZeroToOne();
            vd->bg.b = Utils::GetRandFromZeroToOne();
            //DEBUG_ONLY.
            vd->stickedTo = ViewportsSupportedID::MAIN_VIEWPORT;
            vd->stickySide = ViewportStickSide::VIEWPORT_NONE;
            vd->stickyRatio = 1;
            vd->callback = [&](ViewportData*){};
            vd->id = id;
            eg_winData->viewports.push_back(vd);
        }
        void Update(ViewportsSupportedID id){
            ViewportData* viewport = eg_winData->viewports[id];
            glViewport(viewport->x,viewport->y,viewport->w,viewport->h);
            glScissor(viewport->x,viewport->y,viewport->w,viewport->h);
            glClearColor(viewport->bg.x,viewport->bg.g,viewport->bg.b, 1.0f );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
            viewport->callback(viewport);
        }
        void Refresh(){
            for (int i = 0; i < eg_winData->viewports.size(); i++){
                ViewportData* vd = eg_winData->viewports[i]; 
                SetStickTo(vd->stickedTo, vd->id, vd->stickySide, vd->stickyRatio);
            }
        }
        void UpdateOnWindowResize(float x, float y, float w, float h){
            // Update Main viewport(he is initial so and data will be initial)
            eg_winData->viewports[0]->x = x;
            eg_winData->viewports[0]->y = y;
            eg_winData->viewports[0]->w = w;
            eg_winData->viewports[0]->h = h;
            Refresh();
        }

        void Remove(ViewportsSupportedID id){
            delete eg_winData->viewports[id];
            eg_winData->viewports.erase(eg_winData->viewports.begin() + id);
        }

        void SetStickTo(ViewportsSupportedID targetID, ViewportsSupportedID sourceID, ViewportStickSide side, float size){
            if (targetID == sourceID)
                return;

            ViewportData* target = eg_winData->viewports[targetID];
            ViewportData* source = eg_winData->viewports[sourceID];
            source->stickedTo = targetID;
            source->stickySide = side;
            source->stickyRatio = size;
            // Inherit properties(some of them not gonna change)
            source->x = target->x;
            source->y = target->y;
            source->w = target->w;
            source->h = target->h;
            // For target
            switch(side){
                case VIEWPORT_TOP:{
                    UpdateToGetBigPart(target->h,target->h,size); 
                    break;
                }
                case VIEWPORT_BOTTOM:{
                    UpdateToGetSmallPart(target->y,target->h,size); 
                    UpdateToGetBigPart(target->h,target->h,size); 
                    break;
                }
                case VIEWPORT_RIGHT:{
                    UpdateToGetBigPart(target->w,target->w,size); 
                    break;
                }
                case VIEWPORT_LEFT:{
                    UpdateToGetSmallPart(target->x,target->w,size); 
                    UpdateToGetBigPart(target->w,target->w,size); 
                    break;
                }
            }
            // For source
            switch(side){
                case VIEWPORT_TOP:{
                    UpdateToGetBigPart(source->y,source->h,size); 
                    UpdateToGetSmallPart(source->h,source->h,size); 
                    break;
                }
                case VIEWPORT_BOTTOM:{
                    UpdateToGetSmallPart(source->h,source->h,size); 
                    break;
                }
                case VIEWPORT_RIGHT:{
                    UpdateToGetBigPart(source->x,source->w,size); 
                    UpdateToGetSmallPart(source->w,source->w,size); 
                    break;
                }
                case VIEWPORT_LEFT:{
                    UpdateToGetSmallPart(source->w,source->w,size); 
                    break;
                }
            }

        }

        void Set(ViewportsSupportedID id, std::function<void(ViewportData* vd)> func){
            eg_winData->viewports[id]->callback = func;
            eg_winData->viewports[id]->isActive = true;
        }
        void SetToFullWindowSize(ViewportsSupportedID id){
            ViewportData* vd = eg_winData->viewports[id];
            vd->x = 0;
            vd->y = 0;
            vd->w = eg_winData->size.x;
            vd->h = eg_winData->size.y;
        }
        ViewportData* GetViewportData(ViewportsSupportedID id){
            return eg_winData->viewports[id];
        }
        std::vector<ViewportData*> GetAllViewports(){
            return eg_winData->viewports;
        }
        void UnSet(ViewportsSupportedID id){
            eg_winData->viewports[id]->callback = [&](ViewportData*){};
            eg_winData->viewports[id]->isActive = false;
        }

        void UpdateToGetBigPart(float& dataToChange, const float dataToUse, const float size){
            dataToChange = (dataToUse - size);
        }
        void UpdateToGetSmallPart(float& dataToChange, const float dataToUse, const float size){
            dataToChange = size;
        }
    }
}
