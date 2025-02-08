#include "SodaGui.h"

#include "SodaInput.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace Soda
{
void SodaGui::DrawVec3Gui(const std::string &lable, glm::vec3 &vector,
                          float resetValue, float colWidth)
{
  ImGuiIO &io = ImGui::GetIO();
  auto boldFont = io.Fonts->Fonts[1];

  ImGui::PushID(lable.c_str());

  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, colWidth);

  ImGui::Text(lable.c_str());

  ImGui::NextColumn();
  ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

  float buttonHeight =
      GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
  ImVec2 buttonSize(buttonHeight + 3, buttonHeight);

  // X
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.95f, 0.1f, 0.1f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
  ImGui::PushFont(boldFont);
  if(ImGui::Button("X", buttonSize))
    vector.x = resetValue;
  ImGui::PopFont();
  ImGui::SameLine();
  ImGui::DragFloat("##X", &vector.x, 0.1f);
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::PopStyleColor(3);

  // Y
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.75f, 0.1f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.1f, 0.85f, 0.2f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.75f, 0.1f, 1.0f));
  ImGui::PushFont(boldFont);
  if(ImGui::Button("Y", buttonSize))
    vector.y = resetValue;
  ImGui::PopFont();
  ImGui::SameLine();
  ImGui::DragFloat("##Y", &vector.y, 0.1f);
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::PopStyleColor(3);

  // Z
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.8f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.1f, 0.1f, 0.95f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.8f, 1.0f));
  ImGui::PushFont(boldFont);
  if(ImGui::Button("Z", buttonSize))
    vector.z = resetValue;
  ImGui::PopFont();
  ImGui::SameLine();
  ImGui::DragFloat("##Z", &vector.z, 0.1f);
  ImGui::PopItemWidth();
  ImGui::PopStyleColor(3);

  ImGui::PopStyleVar();
  ImGui::Columns(1);

  ImGui::PopID();
}
} // namespace Soda