#ifndef GUI_H_
#define GUI_H_

#include <IMGUI/imgui.h>
#include <ITexture.h>
#include <IVideoDriver.h>
#include <IrrIMGUI/IrrIMGUI.h>


class GUI {
private:
    struct SimulationView {
        IrrIMGUI::IGUITexture* texture;
        irr::video::ITexture* renderTarget;
        irr::core::dimension2du dimensions;
    };
    SimulationView mSimView;

    irr::video::IVideoDriver& mVidDriverHandle;
    IrrIMGUI::CIMGUIEventReceiver mImGuiEventReceiver;
    IrrIMGUI::IIMGUIHandle* mImGuiHandle;

public:
    GUI(irr::IrrlichtDevice& device, irr::core::dimension2du monitorResolution);
    ~GUI();

    void initWith(irr::IrrlichtDevice& device);
    void start();
    void drawAll();
    IrrIMGUI::CIMGUIEventReceiver& getEventReceiver();
    float getSimViewWindowAspectRatio() const;
    irr::video::ITexture& getSimViewRenderTarget() const;

private:
    void bindSimViewToRenderTarget();
    void initImGui(irr::IrrlichtDevice& device);
    void setImGuiStyle();
    void showMainDockspace();
    void showSimView();
    void showSimPlaybackControls();
};

#endif // GUI_H_

