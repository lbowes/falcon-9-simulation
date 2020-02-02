#ifndef GRAPHICS_GUI_H_
#define GRAPHICS_GUI_H_

#include <IMGUI/imgui.h>
#include <IrrIMGUI/IrrIMGUI.h>


namespace Graphics {

    class GUI {
    private:
        IrrIMGUI::IIMGUIHandle* mImGuiHandle;
        IrrIMGUI::CIMGUIEventReceiver mImGuiEventReceiver;

    public:
        GUI() = default;
        ~GUI() = default;

        void init(irr::IrrlichtDevice& device);
        void start();
        void render();
        inline IrrIMGUI::CIMGUIEventReceiver& getEventReceiver() const;

    private:
        void initImGui(irr::IrrlichtDevice& device);
        void setImGuiStyle();
    };

} // namespace Graphics

#endif // GRAPHICS_GUI_H_
