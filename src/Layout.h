#include "hello_imgui/hello_imgui.h"
#include "AppState.h"

#ifndef LAYOUT_H
#define LAYOUT_H

std::vector<HelloImGui::DockingSplit> CreateDefaultDockingSplits();

std::vector<HelloImGui::DockableWindow> CreateDockableWindows(AppState& appState);

HelloImGui::DockingParams CreateDefaultLayout(AppState& appState);

std::vector<HelloImGui::DockingSplit> CreateAlternativeDockingSplits();

std::vector<HelloImGui::DockingParams> CreateAlternativeLayouts(AppState& appState);

#endif // LAYOUT_H