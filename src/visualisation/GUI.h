#ifndef GUI_H_
#define GUI_H_

#include <IMGUI/imgui.h>
#include <IrrIMGUI/IrrIMGUI.h>


class GUI {
private:
    IrrIMGUI::IIMGUIHandle* mImGuiHandle;
    IrrIMGUI::CIMGUIEventReceiver mImGuiEventReceiver;

public:
    GUI(irr::IrrlichtDevice& device);
    ~GUI();

    void initWith(irr::IrrlichtDevice& device);
    void start();
    void render(irr::video::ITexture* renderTarget);
    IrrIMGUI::CIMGUIEventReceiver& getEventReceiver();

private:
    void initImGui(irr::IrrlichtDevice& device);
    void setImGuiStyle();
};

#endif // GUI_H_
