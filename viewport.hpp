#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <functional>

namespace DI{

    class InterfaceUnit;

    enum ViewportStickSide{
        VIEWPORT_TOP,
        VIEWPORT_BOTTOM,
        VIEWPORT_RIGHT,
        VIEWPORT_LEFT,
        VIEWPORT_NONE,
    };

    enum ViewportsSupportedID{
        MAIN_VIEWPORT,
        VIEWPORT_ONE,
        VIEWPORT_TWO,
        VIEWPORT_THREE,
        VIEWPORT_FOUR,
    };

    struct ViewportData{
        bool isActive;
        float x, y;
        float w, h;
        glm::vec3 bg;
        ViewportsSupportedID id;
        ViewportsSupportedID stickedTo;
        ViewportStickSide    stickySide;
        float stickyRatio;
        std::function<void(ViewportData*)> callback; // What kind of function to call
        std::vector<InterfaceUnit*> units;
    };

    namespace ViewportHandler{
        void Init();
        void Kill();

        void Create(ViewportsSupportedID id);
        void Remove(ViewportsSupportedID id);

        void Update(ViewportsSupportedID id);
        // Refresh connections between viewports
        void Refresh();
        void UpdateOnWindowResize(float x, float y, float w, float h);
        /*
            @param target: to wich viewport to stick
            @param source: wich viewport gonna be sticked
            @param side: to wich side to stick
            @param size: how much to occupy
        */
        void SetStickTo(ViewportsSupportedID target, ViewportsSupportedID source, ViewportStickSide side, float size);
        void SetToFullWindowSize(ViewportsSupportedID id);
        void Set(ViewportsSupportedID id, std::function<void(ViewportData*)> func);
        void UnSet(ViewportsSupportedID id);
        ViewportData* GetViewportData(ViewportsSupportedID id);
        std::vector<ViewportData*> GetAllViewports();
    }
}
