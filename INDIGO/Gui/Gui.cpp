#include "Gui.h"

using namespace Client;

//[enc_string_enable /]
//[junk_enable /]

bool bIsGuiInitalize = false;
bool bIsGuiVisible = false;
WNDPROC WndProc_o = nullptr;

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

LRESULT WINAPI GUI_WndProc( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam );

CGui::CGui() {}

CGui::~CGui()
{
	ImGui_ImplDX9_Shutdown();
}

void CGui::GUI_Init( IDirect3DDevice9 * pDevice )
{
	HWND hWindow = FindWindowA( "Valve001" , 0 );

	ImGui_ImplDX9_Init( hWindow , pDevice );

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	io.IniFilename = GuiFile.c_str();
	
	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0,
	};
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 16.0f, &font_config, ranges);

	ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF( 
		Avalon_compressed_data , Avalon_compressed_size , 12.f );

	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.5f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.FramePadding = ImVec2(4, 2);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 0.2f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 4.f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;

	RedSheme();

	ImGui_ImplDX9_CreateDeviceObjects();

	WndProc_o = (WNDPROC)SetWindowLongA( hWindow , GWL_WNDPROC , (LONG)(LONG_PTR)GUI_WndProc );

	bIsGuiInitalize = true;
}

void CGui::GUI_Begin_Render()
{
	ImGui_ImplDX9_NewFrame();
}

void CGui::GUI_End_Render()
{
	ImGui::Render();
}

LRESULT WINAPI GUI_WndProc( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
{
	static bool is_down = false;
	static bool is_clicked = false;
	static bool check_closed = false;

	if ( GUI_KEY_DOWN( VK_INSERT ) )
	{
		is_clicked = false;
		is_down = true;
	}
	else if ( !GUI_KEY_DOWN( VK_INSERT ) && is_down )
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if ( !bIsGuiVisible && !is_clicked && check_closed )
	{
		string msg = "cl_mouseenable " + to_string( !bIsGuiVisible );
		Interfaces::Engine()->ClientCmd_Unrestricted2( msg.c_str() );
		check_closed = false;
	}

	if ( is_clicked )
	{
		bIsGuiVisible = !bIsGuiVisible;

		string msg = "cl_mouseenable " + to_string( !bIsGuiVisible );
		Interfaces::Engine()->ClientCmd_Unrestricted2( msg.c_str() );

		if ( !check_closed )
			check_closed = true;
	}

	if ( bIsGuiVisible && ImGui_ImplDX9_WndProcHandler( hwnd , uMsg , wParam , lParam ) )
		return true;

	return CallWindowProcA( WndProc_o , hwnd , uMsg , wParam , lParam );
}

void CGui::GUI_Draw_Elements()
{
	if ( !bIsGuiInitalize || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsActiveApp() )
		return;

	g_pGui->GUI_Begin_Render();

	ImGui::GetIO().MouseDrawCursor = bIsGuiVisible;

	if ( bIsGuiVisible )
	{
		int pX , pY;
		Interfaces::InputSystem()->GetCursorPosition( &pX , &pY );

		ImGui::GetIO().MousePos.x = (float)pX;
		ImGui::GetIO().MousePos.y = (float)pY;

		Client::OnRenderGUI();
	}
	
	if ( g_pRadar )
		g_pRadar->OnRender();

	if (Settings::Misc::Other_Spectators) g_pMisc->OnRenderSpectatorList();

	g_pGui->GUI_End_Render();
}

void CGui::RedSheme()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.36f, 0.98f, 0.26f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.36f, 0.98f, 0.26f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
	colors[ImGuiCol_Button] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_Header] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.19f, 0.85f, 0.08f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.19f, 0.85f, 0.08f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_CloseButton] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_CloseButtonActive] = ImVec4(0.36f, 0.98f, 0.26f, 0.73f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
}

void CGui::Red2Sheme()
{
	ImGuiStyle& style = ImGui::GetStyle();

	float tR = Settings::Menu::Menu_MainColor[0];
	float tG = Settings::Menu::Menu_MainColor[1];
	float tB = Settings::Menu::Menu_MainColor[2];
	float tA = Settings::Menu::Menu_MainColor[3];

	float bR = Settings::Menu::Menu_BackGround[0];
	float bG = Settings::Menu::Menu_BackGround[1];
	float bB = Settings::Menu::Menu_BackGround[2];
	float bA = Settings::Menu::Menu_BackGround[3];

	float b1R = Settings::Menu::Menu_BackGround1[0];
	float b1G = Settings::Menu::Menu_BackGround1[1];
	float b1B = Settings::Menu::Menu_BackGround1[2];
	float b1A = Settings::Menu::Menu_BackGround1[3];

	float t1R = Settings::Menu::Menu_Text[0];
	float t1G = Settings::Menu::Menu_Text[1];
	float t1B = Settings::Menu::Menu_Text[2];
	float t1A = Settings::Menu::Menu_Text[3];

	style.Colors[ImGuiCol_Text] = ImVec4(t1R, t1G, t1B, 0.78f); // - 
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(tR, tG, tB, 0.78f); // - 
	style.Colors[ImGuiCol_WindowBg] = ImVec4(bR, bG, bB, 1.00f); // - 
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(b1R, b1G, b1B, 0.58f); // - 
	style.Colors[ImGuiCol_PopupBg] = ImVec4(b1R, b1G, b1B, 1.00f); // - 
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f); // - 
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // - 
	style.Colors[ImGuiCol_FrameBg] = ImVec4(b1R, b1G, b1B, 1.00f); // - 
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(tR, tG, tB, 0.78f); // + 
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_TitleBg] = ImVec4(b1R, b1G, b1B, 1.f); // - 
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(b1R, b1G, b1B, 0.75f); // - 
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(b1R, b1G, b1B, 0.47f); // - 
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(b1R, b1G, b1B, 1.f); // - 
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f); // - 
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(tR, tG, tB, 0.78f); // + 
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_CheckMark] = ImVec4(tR, tG, tB, 1.f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(tR, tG, tB, 0.37f); // + 
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_Button] = ImVec4(tR, tG, tB, 0.75f); // + 
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(tR, tG, tB, 0.86f); // + 
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_Header] = ImVec4(tR, tG, tB, 0.76f); // + 
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(tR, tG, tB, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_Column] = ImVec4(0.15f, 0.00f, 0.00f, 0.35f); // - 
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(tR, tG, tB, 0.69f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(tR, tG, tB, 0.63f); // + 
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(tR, tG, tB, 0.78f); // + 
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f); // - 
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f); // - 
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f); // - 
	style.Colors[ImGuiCol_PlotLines] = ImVec4(t1R, t1G, t1B, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(t1R, t1G, t1B, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(tR, tG, tB, 1.f); // + 
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(tR, tG, tB, 0.44f); // + 
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(b1R, b1G, b1B, 0.73f); // - 
}

namespace ImGui
{
	static auto vector_getter = []( void* vec , int idx , const char** out_text )
	{
		auto& vector = *static_cast<std::vector<std::string>*>( vec );
		if ( idx < 0 || idx >= static_cast<int>( vector.size() ) ) { return false; }
		*out_text = vector.at( idx ).c_str();
		return true;
	};

	IMGUI_API bool ComboBoxArray( const char* label , int* currIndex , std::vector<std::string>& values )
	{
		if ( values.empty() ) { return false; }
		return Combo( label , currIndex , vector_getter ,
					  static_cast<void*>( &values ) , values.size() );
	}

	IMGUI_API bool TabLabels(const char ** tabLabels, int tabSize, int & tabIndex, int * tabOrder)
	{
		ImGuiStyle & style = ImGui::GetStyle();
		const ImVec2 itemSpacing = style.ItemSpacing;
		const ImVec4 color = style.Colors[ImGuiCol_Button];
		const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
		const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
		const ImVec4 colorText = style.Colors[ImGuiCol_Text];
		style.ItemSpacing.x = 1;
		style.ItemSpacing.y = 1;
		const ImVec4 colorSelectedTab = ImVec4(color.x, color.y, color.z, color.w * 0.5f);
		const ImVec4 colorSelectedTabHovered = ImVec4(colorHover.x, colorHover.y, colorHover.z, colorHover.w * 0.5f);
		const ImVec4 colorSelectedTabText = ImVec4(colorText.x * 0.8f, colorText.y * 0.8f, colorText.z * 0.8f, colorText.w * 0.8f);
		if (tabSize> 0 && (tabIndex <0 || tabIndex >= tabSize))
		{
			if (!tabOrder)
			{
				tabIndex = 0;
			}
			else
			{
				tabIndex = -1;
			}
		}
		float windowWidth = 0.f, sumX = 0.f;
		windowWidth = ImGui::GetWindowWidth() - style.WindowPadding.x - (ImGui::GetScrollMaxY()> 0 ? style.ScrollbarSize : 0.f);
		static int draggingTabIndex = -1; int draggingTabTargetIndex = -1;
		static ImVec2 draggingtabSize(0, 0);
		static ImVec2 draggingTabOffset(0, 0);
		const bool isMMBreleased = ImGui::IsMouseReleased(2);
		const bool isMouseDragging = ImGui::IsMouseDragging(0, 2.f);
		int justClosedTabIndex = -1, newtabIndex = tabIndex;
		bool selection_changed = false; bool noButtonDrawn = true;
		for (int j = 0, i; j <tabSize; j++)
		{
			i = tabOrder ? tabOrder[j] : j;
			if (i == -1) continue;
			if (sumX> 0.f)
			{
				sumX += style.ItemSpacing.x;
				sumX += ImGui::CalcTextSize(tabLabels[i]).x + 2.f * style.FramePadding.x;
				if (sumX> windowWidth)
				{
					sumX = 0.f;
				}
				else
				{
					ImGui::SameLine();
				}
			}
			if (i!= tabIndex)
			{
				// Push the style
				style.Colors[ImGuiCol_Button] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonActive] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonHovered] = colorSelectedTabHovered;
				style.Colors[ImGuiCol_Text] = colorSelectedTabText;
			}
			// Draw the button
			ImGui::PushID(i); // otherwise two tabs with the same name would clash.
			if (ImGui::Button(tabLabels[i], ImVec2(131.91f, 26.f))) { selection_changed = (tabIndex!= i); newtabIndex = i; } // if you want to change the button width and hegiht x = width, y = height .;
			ImGui::PopID();
			if (i!= tabIndex)
			{
				// Reset the style
				style.Colors[ImGuiCol_Button] = color;
				style.Colors[ImGuiCol_ButtonActive] = colorActive;
				style.Colors[ImGuiCol_ButtonHovered] = colorHover;
				style.Colors[ImGuiCol_Text] = colorText;
			}
			noButtonDrawn = false;
			if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line
			if (ImGui::IsItemHoveredRect())
			{
				if (tabOrder)
				{
					// tab reordering
					if (isMouseDragging)
					{
						if (draggingTabIndex == -1)
						{
							draggingTabIndex = j;
							draggingtabSize = ImGui::GetItemRectSize();
							const ImVec2 & mp = ImGui::GetIO().MousePos;
							const ImVec2 draggingTabCursorPos = ImGui::GetCursorPos();
							draggingTabOffset = ImVec2(
								mp.x + draggingtabSize.x * 0.5f - sumX + ImGui::GetScrollX(),
								mp.y + draggingtabSize.y * 0.5f - draggingTabCursorPos.y + ImGui::GetScrollY()
							);
						}
					}
					else if (draggingTabIndex >= 0 && draggingTabIndex <tabSize && draggingTabIndex!= j)
					{
						draggingTabTargetIndex = j; // For some odd reasons this seems to get called only when draggingTabIndex < i ! (Probably during mouse dragging ImGui owns the mouse someway and sometimes ImGui::IsItemHovered() is not getting called)
					}
				}
			}
		}
		tabIndex = newtabIndex;
		// Restore the style
		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		style.Colors[ImGuiCol_Text] = colorText;
		style.ItemSpacing = itemSpacing;
		return selection_changed;
	}
}