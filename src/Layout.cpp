#include "Layout.h"
// #include <implot.h>
#include <implot/implot.h>

//////////////////////////////////////////////////////////////////////////
//    Docking Layouts and Docking windows
//////////////////////////////////////////////////////////////////////////

//
// 1. Define the Docking splits (two versions are available)
//
std::vector<HelloImGui::DockingSplit> CreateDefaultDockingSplits()
{
    //    Define the default docking splits,
    //    i.e. the way the screen space is split in different target zones for the dockable windows
    //     We want to split "MainDockSpace" (which is provided automatically) into three zones, like this:
    //
    //    ___________________________________________
    //    |        |                                |
    //    | Command|                                |
    //    | Space  |    MainDockSpace               |
    //    |------- |                                |
    //    |        |--------------------------------|
    //    |        |       CommandSpace2            |
    //    -------------------------------------------
    //    |     MiscSpace                           |
    //    -------------------------------------------
    //

    // Then, add a space named "MiscSpace" whose height is 25% of the app height.
    // This will split the preexisting default dockspace "MainDockSpace" in two parts.
    HelloImGui::DockingSplit splitMainMisc;
    splitMainMisc.initialDock = "MainDockSpace";
    splitMainMisc.newDock = "MiscSpace";
    splitMainMisc.direction = ImGuiDir_Down;
    splitMainMisc.ratio = 0.25f;

    // Then, add a space to the left which occupies a column whose width is 25% of the app width
    HelloImGui::DockingSplit splitMainCommand;
    splitMainCommand.initialDock = "MainDockSpace";
    splitMainCommand.newDock = "CommandSpace";
    splitMainCommand.direction = ImGuiDir_Left;
    splitMainCommand.ratio = 0.25f;

    // Then, add CommandSpace2 below MainDockSpace
    HelloImGui::DockingSplit splitMainCommand2;
    splitMainCommand2.initialDock = "MainDockSpace";
    splitMainCommand2.newDock = "CommandSpace2";
    splitMainCommand2.direction = ImGuiDir_Down;
    splitMainCommand2.ratio = 0.5f;

    std::vector<HelloImGui::DockingSplit> splits {splitMainMisc, splitMainCommand, splitMainCommand2};
    return splits;
}

std::vector<HelloImGui::DockingSplit> CreateAlternativeDockingSplits()
{
    //    Define alternative docking splits for the "Alternative Layout"
    //    ___________________________________________
    //    |                |                        |
    //    | Misc           |                        |
    //    | Space          |    MainDockSpace       |
    //    |                |                        |
    //    -------------------------------------------
    //    |                       |                 |
    //    |                       | Command         |
    //    |     CommandSpace      | Space2          |
    //    |                       |                 |
    //    -------------------------------------------

    HelloImGui::DockingSplit splitMainCommand;
    splitMainCommand.initialDock = "MainDockSpace";
    splitMainCommand.newDock = "CommandSpace";
    splitMainCommand.direction = ImGuiDir_Down;
    splitMainCommand.ratio = 0.5f;

    HelloImGui::DockingSplit splitMainCommand2;
    splitMainCommand2.initialDock = "CommandSpace";
    splitMainCommand2.newDock = "CommandSpace2";
    splitMainCommand2.direction = ImGuiDir_Right;
    splitMainCommand2.ratio = 0.4f;

    HelloImGui::DockingSplit splitMainMisc;
    splitMainMisc.initialDock = "MainDockSpace";
    splitMainMisc.newDock = "MiscSpace";
    splitMainMisc.direction = ImGuiDir_Left;
    splitMainMisc.ratio = 0.5f;

    std::vector<HelloImGui::DockingSplit> splits {splitMainCommand, splitMainCommand2, splitMainMisc};
    return splits;
}

//
// 2. Define the Dockable windows
//
std::vector<HelloImGui::DockableWindow> CreateDockableWindows(AppState& appState)
{
    // A window named "FeaturesDemo" will be placed in "CommandSpace". Its Gui is provided by "GuiWindowDemoFeatures"
    HelloImGui::DockableWindow featuresDemoWindow;
    featuresDemoWindow.label = "Features Demo";
    featuresDemoWindow.dockSpaceName = "CommandSpace";
    // featuresDemoWindow.GuiFunction = [&] { GuiWindowDemoFeatures(appState); };
    featuresDemoWindow.GuiFunction = [] { ImGui::Text("featuresDemoWindow"); };

    // A layout customization window will be placed in "MainDockSpace". Its Gui is provided by "GuiWindowLayoutCustomization"
    HelloImGui::DockableWindow imPlotDemoWindow;
    imPlotDemoWindow.label = "ImPlot Demo";
    imPlotDemoWindow.dockSpaceName = "MainDockSpace";
    imPlotDemoWindow.imGuiWindowFlags = ImGuiWindowFlags_MenuBar;
    imPlotDemoWindow.GuiFunction = [] { ImPlot::ShowDemoWindow(); };

    // A Log window named "Logs" will be placed in "MiscSpace". It uses the HelloImGui logger gui
    HelloImGui::DockableWindow logsWindow;
    logsWindow.label = "Logs";
    logsWindow.dockSpaceName = "MiscSpace";
    logsWindow.GuiFunction = [] { HelloImGui::LogGui(); };

    // A Window named "Dear ImGui Demo" will be placed in "MainDockSpace"
    HelloImGui::DockableWindow dearImGuiDemoWindow;
    dearImGuiDemoWindow.label = "Dear ImGui Demo";
    dearImGuiDemoWindow.dockSpaceName = "MainDockSpace";
    dearImGuiDemoWindow.imGuiWindowFlags = ImGuiWindowFlags_MenuBar;
    dearImGuiDemoWindow.GuiFunction = [] { ImPlot::CreateContext(); ImGui::ShowDemoWindow(); };


    std::vector<HelloImGui::DockableWindow> dockableWindows {
        featuresDemoWindow,
        logsWindow,
        dearImGuiDemoWindow,
        imPlotDemoWindow
    };
    return dockableWindows;
}

//
// 3. Define the layouts:
//        A layout is stored inside DockingParams, and stores the splits + the dockable windows.
//        Here, we provide the default layout, and two alternative layouts.
//
HelloImGui::DockingParams CreateDefaultLayout(AppState& appState)
{
    HelloImGui::DockingParams dockingParams;
    // dockingParams.layoutName = "Default"; // By default, the layout name is already "Default"
    dockingParams.dockingSplits = CreateDefaultDockingSplits();
    dockingParams.dockableWindows = CreateDockableWindows(appState);
    return dockingParams;
}


std::vector<HelloImGui::DockingParams> CreateAlternativeLayouts(AppState& appState)
{
    HelloImGui::DockingParams alternativeLayout;
    {
        alternativeLayout.layoutName = "Alternative Layout";
        alternativeLayout.dockingSplits = CreateAlternativeDockingSplits();
        alternativeLayout.dockableWindows = CreateDockableWindows(appState);
    }
    HelloImGui::DockingParams tabsLayout;
    {
        tabsLayout.layoutName = "Tabs Layout";
        tabsLayout.dockableWindows = CreateDockableWindows(appState);
        // Force all windows to be presented in the MainDockSpace
        for (auto& window: tabsLayout.dockableWindows)
            window.dockSpaceName = "MainDockSpace";
        // In "Tabs Layout", no split is created
        tabsLayout.dockingSplits = {};
    }
    return {alternativeLayout, tabsLayout};
}
