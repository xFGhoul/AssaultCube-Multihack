#include <pch.h>
#include <base.h>

static clock_t WatermarkClock = 0.0f;
static enum ConfigState_t { CFG_NONE, CFG_GOOD_LOAD, CFG_BAD_LOAD, CFG_GOOD_SAVE, CFG_BAD_SAVE };
static ConfigState_t ConfigState;

BOOL __stdcall Base::Hooks::SwapBuffers(_In_ HDC hdc)
{
	Data::oContext = wglGetCurrentContext();

	if (!Data::InitSwapBuffers)
	{
		Data::hWindow = WindowFromDC(hdc);
		Data::oWndProc = (WndProc_t)SetWindowLongPtr(Data::hWindow, GWL_WNDPROC, (LONG_PTR)Hooks::WndProc);

		Data::glContext = wglCreateContext(hdc);
		wglMakeCurrent(hdc, Data::glContext);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		glOrtho(0, m_viewport[2], m_viewport[3], 0, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 1);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = NULL;
		io.LogFilename = NULL;

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.ColumnsMinSpacing = 0.0f;
		style.WindowTitleAlign = ImVec2(0.5, 0.5);
		style.Colors[ImGuiCol_Text] = ImColor(1.f, 1.f, 1.f, 1.f);
		style.Colors[ImGuiCol_TextDisabled] = ImColor(0.8f, 0.8f, 0.8f, 1.f);
		style.Colors[ImGuiCol_WindowBg] = ImColor(0.025f, 0.025f, 0.025f, 1.f);
		style.Colors[ImGuiCol_ChildBg] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_PopupBg] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_Border] = ImColor(0.8f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_BorderShadow] = ImColor(0.f, 0.f, 0.f, 0.f);
		style.Colors[ImGuiCol_FrameBg] = ImColor(0.1f, 0.1f, 0.1f, 1.f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImColor(0.175f, 0.175f, 0.175f, 1.f);
		style.Colors[ImGuiCol_FrameBgActive] = ImColor(0.25f, 0.25f, 0.25f, 1.f);
		style.Colors[ImGuiCol_TitleBg] = ImColor(0.7f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_TitleBgActive] = ImColor(0.9f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0.9f, 0.f, 0.f, 0.5f);
		style.Colors[ImGuiCol_MenuBarBg] = ImColor(0.f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImColor(0.1f, 0.1f, 0.1f, 1.f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(0.5f, 0.5f, 0.5f, 1.f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(0.7f, 0.7f, 0.7f, 1.f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(0.9f, 0.9f, 0.9f, 1.f);
		style.Colors[ImGuiCol_CheckMark] = ImColor(1.f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_SliderGrab] = style.Colors[ImGuiCol_ScrollbarGrab];
		style.Colors[ImGuiCol_SliderGrabActive] = style.Colors[ImGuiCol_ScrollbarGrabActive];
		style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_FrameBg];
		style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_FrameBgHovered];
		style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_FrameBgActive];
		style.Colors[ImGuiCol_Header] = style.Colors[ImGuiCol_FrameBg];
		style.Colors[ImGuiCol_HeaderHovered] = style.Colors[ImGuiCol_FrameBgHovered];
		style.Colors[ImGuiCol_HeaderActive] = style.Colors[ImGuiCol_FrameBgActive];
		style.Colors[ImGuiCol_Separator] = ImColor();
		style.Colors[ImGuiCol_SeparatorHovered] = ImColor();
		style.Colors[ImGuiCol_SeparatorActive] = ImColor();
		style.Colors[ImGuiCol_ResizeGrip] = ImColor();
		style.Colors[ImGuiCol_ResizeGripHovered] = ImColor();
		style.Colors[ImGuiCol_ResizeGripActive] = ImColor();
		style.Colors[ImGuiCol_Tab] = ImColor(0.2f, 0.2f, 0.2f, 0.2f);
		style.Colors[ImGuiCol_TabHovered] = ImColor(0.7f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_TabActive] = ImColor(1.f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_TabUnfocused] = style.Colors[ImGuiCol_Tab];
		style.Colors[ImGuiCol_TabUnfocusedActive] = style.Colors[ImGuiCol_TabActive];
		style.Colors[ImGuiCol_PlotLines] = ImColor();
		style.Colors[ImGuiCol_PlotLinesHovered] = ImColor();
		style.Colors[ImGuiCol_PlotHistogram] = ImColor();
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImColor();
		style.Colors[ImGuiCol_TableHeaderBg] = ImColor();
		style.Colors[ImGuiCol_TableBorderStrong] = ImColor();
		style.Colors[ImGuiCol_TableBorderLight] = ImColor();
		style.Colors[ImGuiCol_TableRowBg] = ImColor();
		style.Colors[ImGuiCol_TableRowBgAlt] = ImColor();
		style.Colors[ImGuiCol_TextSelectedBg] = ImColor();
		style.Colors[ImGuiCol_DragDropTarget] = ImColor();
		style.Colors[ImGuiCol_NavHighlight] = ImColor();
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImColor();
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImColor();
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImColor();

		ImFontConfig cfg = {};
		cfg.FontDataOwnedByAtlas = false;
		Data::FontTitle = io.Fonts->AddFontFromMemoryTTF((void*)Ubuntu_Regular, sizeof(Ubuntu_Regular), 18.0f, &cfg);
		Data::FontMenu = io.Fonts->AddFontFromMemoryTTF((void*)Ubuntu_Regular, sizeof(Ubuntu_Regular), 14.0f, &cfg);
		Data::FontHack = io.Fonts->AddFontFromMemoryTTF((void*)UbuntuMono_Regular, sizeof(UbuntuMono_Regular), 12.0f, &cfg);
		ImGui_ImplWin32_Init(Data::hWindow);
		ImGui_ImplOpenGL2_Init();

		Data::InitSwapBuffers = true;
	}

	GetClientRect(Data::hWindow, &Data::WindowRect);
	Data::WindowWidth = Data::WindowRect.right - Data::WindowRect.left;
	Data::WindowHeight = Data::WindowRect.bottom - Data::WindowRect.top;

	wglMakeCurrent(hdc, Data::glContext);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImDrawList* Draw = ImGui::GetBackgroundDrawList();

	ImGui::PushFont(Data::FontHack);
	for (int i = 0; Data::game.players && Data::game.players->inrange(i); i++)
	{
		playerent* ent = Data::game.players->operator[](i);
		if (!ent || ent == Data::game.player1) continue;

		playerinfo_t info = playerinfo_t(ent, (float)Data::WindowWidth, (float)Data::WindowHeight);
		if (info.is_valid)
		{
			Hacks::ESP_Snaplines(&info);
			Hacks::ESP_Box(&info);
			Hacks::ESP_Info(&info);
		}
	}

	Hacks::Crosshair();

	ImGui::PopFont();

	if (Data::ShowWatermark)
	{
		if (!WatermarkClock) WatermarkClock = clock();

		clock_t now = clock();

		ImGui::PushFont(Data::FontHack);
		Draw->AddText(ImVec2(10.0f, 10.0f), Data::WatermarkColor, "AssaultCube Multihack by Ghoul");

		if ((now - WatermarkClock) >= 500)
		{
			WatermarkClock = 0.0f;
			if (Data::WatermarkColor.Value.x >= 1.0f)
			{
				Data::WatermarkColor.Value.x = 0.2f;
				Data::WatermarkColor.Value.y = 1.0f;
				Data::WatermarkColor.Value.z = 0.2f;
			}

			else if (Data::WatermarkColor.Value.y >= 1.0f)
			{
				Data::WatermarkColor.Value.x = 0.4f;
				Data::WatermarkColor.Value.y = 0.4f;
				Data::WatermarkColor.Value.z = 1.0f;
			}

			else if (Data::WatermarkColor.Value.z >= 1.0f)
			{
				Data::WatermarkColor.Value.x = 1.0f;
				Data::WatermarkColor.Value.y = 0.2f;
				Data::WatermarkColor.Value.z = 0.2f;
			}

			else
			{
				Data::WatermarkColor.Value.x = 1.0f;
				Data::WatermarkColor.Value.y = 0.2f;
				Data::WatermarkColor.Value.z = 0.2f;
			}
		}

		ImGui::PopFont();
	}

	if (Data::ShowMenu)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		static const char* g_MenuEntries[] = { "Aim", "Visuals", "ESP", "Misc" };
		static const int    g_MenuCount = sizeof(g_MenuEntries) / sizeof(g_MenuEntries[0]);
		static const ImVec2 g_MenuTabSize = ImVec2(120.0f, 32.0f);
		static int          g_MenuActiveTab = 0;
		static const float  g_MenuContentSize = 300.0f;
		static const float  g_MenuSafeSize = 25.0f;
		static const ImVec2 g_MenuWindowSize = ImVec2(g_MenuTabSize.x * g_MenuCount, g_MenuTabSize.y + g_MenuContentSize);
		static const ImVec2 g_MenuSettingsMinSize = ImVec2(0.0f, 50.0f);
		static const ImVec2 g_MenuSettingsSize = ImVec2(0.0f, 150.0f);
		static const ImVec2 g_MenuKeySettingsSize = ImVec2(0.0f, 100.0f);

		ImGui::SetNextWindowSize(g_MenuWindowSize);

		ImGui::PushFont(Data::FontTitle);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		ImGui::Begin("AssaultCube Multihack by Ghoul", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysUseWindowPadding);

		ImGui::Columns(g_MenuCount, 0, false);

		for (int i = 0; i < g_MenuCount; i++)
		{
			if (g_MenuActiveTab == i)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_TabActive]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_TabActive]);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_TabActive]);
			}

			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_Tab]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_TabHovered]);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_TabActive]);
			}

			if (ImGui::Button(g_MenuEntries[i], g_MenuTabSize))
				g_MenuActiveTab = i;

			ImGui::PopStyleColor(3);
			ImGui::NextColumn();
		}
		ImGui::PopFont();
		ImGui::PopStyleVar(2);

		ImGui::PushFont(Data::FontMenu);
		ImGui::Columns(1, 0, false);

		ImGui::BeginChild("menu-content", ImVec2(g_MenuWindowSize.x, g_MenuContentSize - g_MenuSafeSize), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

		switch (g_MenuActiveTab)
		{
		case 0: //Aim
			ImGui::Checkbox("Aimbot", &Data::Settings::EnableAimbot);
			if (Data::Settings::EnableAimbot)
			{
				if (ImGui::TreeNode("Aimbot Settings..."))
				{
					ImGui::BeginChild("aimbot-settings", g_MenuSettingsSize, true);
					const char* AimbotTargetPreferences[] = { "Closest to Player", "Closest to Aim" };
					ImGui::Combo("Aimbot Target Preference", &Data::Settings::AimbotTargetPreference, AimbotTargetPreferences, sizeof(AimbotTargetPreferences) / sizeof(AimbotTargetPreferences[0]));
					ImGui::Checkbox("Smooth", &Data::Settings::AimbotSmooth);
					if (Data::Settings::AimbotSmooth)
						ImGui::SliderFloat("Smooth Value", &Data::Settings::AimbotSmoothValue, 1.0f, 10.0f, "%.0f");
					ImGui::Checkbox("Toggle Key", &Data::Settings::AimbotToggle);
					if (Data::Settings::AimbotToggle)
					{
						ImGui::Checkbox("Toggle State", &Data::Settings::AimbotToggleState);

						if (Data::Keys::ToChange == &Data::Keys::Aimbot)
							ImGui::Text("Key: [...]");
						else
							ImGui::Text("Key: %i", Data::Keys::Aimbot);

						if (ImGui::Button("Change Key..."))
							Data::Keys::ToChange = &Data::Keys::Aimbot;
					}
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("No Recoil", &Data::Settings::EnableNoRecoil);
			ImGui::Checkbox("Always Headshot", &Data::Settings::EnableAlwaysHeadshot);
			ImGui::Checkbox("Auto Guns", &Data::Settings::EnableAutoGuns);

			ImGui::Checkbox("Triggerbot", &Data::Settings::EnableTriggerbot);
			if (Data::Settings::EnableTriggerbot)
			{
				if (ImGui::TreeNode("Triggerbot Settings..."))
				{
					ImGui::BeginChild("triggerbot-settings", g_MenuKeySettingsSize, true);
					ImGui::Checkbox("Delay", &Data::Settings::TriggerbotDelay);
					if (Data::Settings::TriggerbotDelay)
						ImGui::SliderFloat("Delay Value", &Data::Settings::TriggerbotDelayValue, 0.0f, 1.0f, "%.3f");
					ImGui::Checkbox("Toggle Key", &Data::Settings::TriggerbotToggle);
					ImGui::Checkbox("Toggle State", &Data::Settings::TriggerbotToggleState);

					if (Data::Keys::ToChange == &Data::Keys::Triggerbot)
						ImGui::Text("Key: [...]");
					else
						ImGui::Text("Key: %i", Data::Keys::Triggerbot);

					if (ImGui::Button("Change Key..."))
						Data::Keys::ToChange = &Data::Keys::Triggerbot;
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}
			break;
		case 1: //Visuals
			ImGui::Checkbox("No Scope", &Data::Settings::EnableNoScope);
			ImGui::Checkbox("FullBright", &Data::Settings::EnableFullBright);
			ImGui::Checkbox("Radar Hack", &Data::Settings::EnableRadarHack);
			ImGui::Checkbox("Custom Crosshair", &Data::Settings::EnableCrosshair);
			if (Data::Settings::EnableCrosshair)
			{
				if (ImGui::TreeNode("Custom Crosshair Settings..."))
				{
					ImGui::BeginChild("crosshair-settings", g_MenuSettingsSize, true);
					const char* CrosshairTypes[] = { "Default", "Triangle", "Square", "Circle" };
					ImGui::Combo("Crosshair Type", &Data::Settings::CrosshairType, CrosshairTypes, sizeof(CrosshairTypes) / sizeof(CrosshairTypes[0]));
					ImGui::SliderFloat("Crosshair Length", &Data::Settings::CrosshairLength, 0, 100, "%.0f");
					ImGui::SliderFloat("Crosshair Thickness", &Data::Settings::CrosshairThickness, 0, 100, "%.0f");
					ImGui::SliderFloat("Crosshair Gap", &Data::Settings::CrosshairGap, 0, 100, "%.0f");
					ImGui::Checkbox("Crosshair Top", &Data::Settings::CrosshairTop);
					ImGui::Checkbox("Crosshair Left", &Data::Settings::CrosshairLeft);
					ImGui::Checkbox("Crosshair Bottom", &Data::Settings::CrosshairBottom);
					ImGui::Checkbox("Crosshair Right", &Data::Settings::CrosshairRight);
					ImGui::Checkbox("Crosshair Dot", &Data::Settings::CrosshairDot);
					if (Data::Settings::CrosshairDot)
						ImGui::Checkbox("Crosshair Dot Filled", &Data::Settings::CrosshairDotFilled);
					ImGui::ColorEdit4("Crosshair Color", Data::Settings::CrosshairColor);
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}
			break;
		case 2: //ESP
			ImGui::Checkbox("Wallhack", &Data::Settings::EnableWallhack);
			ImGui::Checkbox("Enable ESP Team", &Data::Settings::EnableEspTeam);
			ImGui::Checkbox("Enable ESP Enemy", &Data::Settings::EnableEspEnemy);
			ImGui::Checkbox("ESP Name", &Data::Settings::EnableEspName);
			if (Data::Settings::EnableEspName)
			{
				if (ImGui::TreeNode("ESP Name Settings"))
				{
					if (Data::Settings::EnableEspTeam)
						ImGui::ColorEdit4("ESP Name Color Team", Data::Settings::EspNameColorTeam);
					if (Data::Settings::EnableEspEnemy)
						ImGui::ColorEdit4("ESP Name Color Enemy", Data::Settings::EspNameColorEnemy);
					ImGui::TreePop();
				}
			}
			ImGui::Checkbox("ESP Health", &Data::Settings::EnableEspHealth);
			ImGui::Checkbox("ESP Armor", &Data::Settings::EnableEspArmor);
			ImGui::Checkbox("ESP Box", &Data::Settings::EnableEspBox);
			if (Data::Settings::EnableEspBox)
			{
				if (Data::Settings::EnableEspTeam || Data::Settings::EnableEspEnemy)
				{
					if (ImGui::TreeNode("ESP Box Settings..."))
					{
						ImGui::BeginChild("esp-box-settings", g_MenuSettingsSize, true);
						ImGui::Checkbox("Visibility Check", &Data::Settings::EspBoxVisibilyCheck);
						ImGui::SliderFloat("ESP Box Thickness", &Data::Settings::EspBoxThickness, 0, 100, "%.0f");

						if (Data::Settings::EnableEspTeam)
						{
							ImGui::ColorEdit4("ESP Box Color Team", Data::Settings::EspBoxColorTeam);
							ImGui::ColorEdit4("ESP Box Color Fill Team", Data::Settings::EspBoxColorFillTeam);

							if (Data::Settings::EspBoxVisibilyCheck)
							{
								ImGui::ColorEdit4("ESP Box Color Team Visible", Data::Settings::EspBoxColorTeamVisible);
								ImGui::ColorEdit4("ESP Box Color Fill Team Visible", Data::Settings::EspBoxColorFillTeamVisible);
							}
						}

						if (Data::Settings::EnableEspEnemy)
						{
							ImGui::ColorEdit4("ESP Box Color Enemy", Data::Settings::EspBoxColorEnemy);
							ImGui::ColorEdit4("ESP Box Color Fill Enemy", Data::Settings::EspBoxColorFillEnemy);

							if (Data::Settings::EspBoxVisibilyCheck)
							{
								ImGui::ColorEdit4("ESP Box Color Enemy Visible", Data::Settings::EspBoxColorEnemyVisible);
								ImGui::ColorEdit4("ESP Box Color Fill Enemy Visible", Data::Settings::EspBoxColorFillEnemyVisible);
							}
						}

						ImGui::EndChild();
						ImGui::TreePop();
					}
				}
			}

			ImGui::Checkbox("ESP Snaplines", &Data::Settings::EnableEspSnaplines);
			if (Data::Settings::EnableEspSnaplines)
			{
				if (Data::Settings::EnableEspTeam || Data::Settings::EnableEspEnemy)
				{
					if (ImGui::TreeNode("ESP Snaplines Settings"))
					{
						ImGui::BeginChild("esp-snaplines-settings", g_MenuSettingsSize, true);
						const char* SnaplinesPos[] = { "Bottom", "Top" };
						ImGui::Checkbox("Visibility Check", &Data::Settings::EspSnaplinesVisibilityCheck);
						ImGui::SliderFloat("ESP Snaplines Thickness", &Data::Settings::EspSnaplinesThickness, 0, 100, "%.0f");
						if (Data::Settings::EnableEspTeam)
						{
							ImGui::ColorEdit4("ESP Snaplines Color Team", Data::Settings::EspSnaplinesColorTeam);
							if (Data::Settings::EspSnaplinesVisibilityCheck)
								ImGui::ColorEdit4("ESP Snaplines Color Team Visible", Data::Settings::EspSnaplinesColorTeamVisible);
						}
						if (Data::Settings::EnableEspEnemy)
						{
							ImGui::ColorEdit4("ESP Snaplines Color Enemy", Data::Settings::EspSnaplinesColorEnemy);
							if (Data::Settings::EspSnaplinesVisibilityCheck)
								ImGui::ColorEdit4("ESP Snaplines Color Enemy Visible", Data::Settings::EspSnaplinesColorEnemyVisible);
						}
						ImGui::Combo("ESP Snaplines Position", &Data::Settings::EspSnaplinesPos, SnaplinesPos, sizeof(SnaplinesPos) / sizeof(SnaplinesPos[0]));
						ImGui::EndChild();
						ImGui::TreePop();
					}
				}
			}

			break;
		case 3: //Misc
			if (ImGui::TreeNode("Config Settings..."))
			{
				ImGui::BeginChild("config-settings", g_MenuSettingsSize, true);
				ImGui::InputText("Config Name", Data::ConfigName, sizeof(Data::ConfigName));
				if (ImGui::Button("Save Config"))
					ConfigState = Base::SaveConfig(Data::ConfigName) ? CFG_GOOD_SAVE : CFG_BAD_SAVE;
				if (ImGui::Button("Load Config"))
					ConfigState = Base::LoadConfig(Data::ConfigName) ? CFG_GOOD_LOAD : CFG_BAD_LOAD;
				switch (ConfigState)
				{
				case CFG_BAD_SAVE:
					ImGui::Text("Unable to Save Config!");
					break;
				case CFG_GOOD_SAVE:
					ImGui::Text("Config Successfully Saved!");
					break;
				case CFG_BAD_LOAD:
					ImGui::Text("Unable to Load Config!");
					break;
				case CFG_GOOD_LOAD:
					ImGui::Text("Config Successfully Loaded!");
					break;
				case CFG_NONE:
				default:
					break;
				}
				ImGui::EndChild();
				ImGui::TreePop();
			}
			ImGui::Checkbox("Show Watermark", &Data::ShowWatermark);
			ImGui::Checkbox("Fly Hack", &Data::Settings::EnableFlyHack);
			if (Data::Settings::EnableFlyHack)
			{
				if (ImGui::TreeNode("FlyHack Settings..."))
				{
					ImGui::BeginChild("flyhack-settings", g_MenuKeySettingsSize, true);
					ImGui::Checkbox("Toggle Key", &Data::Settings::FlyHackToggle);
					ImGui::Checkbox("Toggle State", &Data::Settings::FlyHackToggleState);

					if (Data::Keys::ToChange == &Data::Keys::FlyHack)
						ImGui::Text("Key: [...]");
					else
						ImGui::Text("Key: %i", Data::Keys::FlyHack);

					if (ImGui::Button("Change Key..."))
						Data::Keys::ToChange = &Data::Keys::FlyHack;
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Speedhack", &Data::Settings::EnableSpeedhack);
			if (Data::Settings::EnableSpeedhack)
			{
				if (ImGui::TreeNode("Speedhack Settings..."))
				{
					ImGui::BeginChild("speedhack-settings", g_MenuSettingsSize, true);
					ImGui::SliderFloat("Speedhack Value", &Data::Settings::SpeedhackValue, 0.1f, 1.0f, "%.1f");
					ImGui::Checkbox("Toggle Key", &Data::Settings::SpeedhackToggle);
					ImGui::Checkbox("Toggle State", &Data::Settings::SpeedhackToggleState);

					if (Data::Keys::ToChange == &Data::Keys::Speedhack)
						ImGui::Text("Key: [...]");
					else
						ImGui::Text("Key: %i", Data::Keys::Speedhack);

					if (ImGui::Button("Change Key..."))
						Data::Keys::ToChange = &Data::Keys::Speedhack;
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}
			ImGui::Checkbox("Bunnyhop", &Data::Settings::EnableBunnyhop);
			if (Data::Settings::EnableBunnyhop)
			{
				if (ImGui::TreeNode("Bunnyhop Settings..."))
				{
					ImGui::BeginChild("bunnyhop-settings", g_MenuKeySettingsSize, true);
					ImGui::Checkbox("Toggle Key", &Data::Settings::BunnyhopToggle);
					ImGui::Checkbox("Toggle State", &Data::Settings::BunnyhopToggleState);

					if (Data::Keys::ToChange == &Data::Keys::Bhop)
						ImGui::Text("Key: [...]");
					else
						ImGui::Text("Key: %i", Data::Keys::Bhop);

					if (ImGui::Button("Change Key..."))
						Data::Keys::ToChange = &Data::Keys::Bhop;
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Name Changer", &Data::Settings::EnableNameChanger);
			if (Data::Settings::EnableNameChanger)
			{
				if (ImGui::TreeNode("Name Changer Settings..."))
				{
					const char* NameChangerTypes[] = { "None", "Bot Names", "Connected Player Names" };
					ImGui::BeginChild("name-changer-settings", g_MenuSettingsSize, true);
					ImGui::Combo("Name Changer Type", &Data::Settings::NameChangerType, NameChangerTypes, sizeof(NameChangerTypes) / sizeof(NameChangerTypes[0]));
					ImGui::Checkbox("AntiSpam", &Data::Settings::NameChangerAntiSpam);
					if (Data::Settings::NameChangerAntiSpam)
						ImGui::SliderFloat("Delay", &Data::Settings::NameChangerAntiSpamValue, 0.0f, 10.0f, "%.1f");
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Ping Changer", &Data::Settings::EnablePingChanger);
			if (Data::Settings::EnablePingChanger)
			{
				if (ImGui::TreeNode("Ping Changer Settings..."))
				{
					ImGui::BeginChild("ping-changer-settings", g_MenuSettingsMinSize, true);
					ImGui::SliderInt("Ping", &Data::Settings::PingChangerValue, 0, 1000);
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Chat Spam", &Data::Settings::EnableChatSpam);
			if (Data::Settings::EnableChatSpam)
			{
				if (ImGui::TreeNode("Chat Spam Settings..."))
				{
					const char* ChatSpamTypes[] = { "None", "Team", "All" };
					ImGui::BeginChild("chat-spam-settings", g_MenuSettingsSize, true);
					ImGui::Combo("Chat Spam Type", &Data::Settings::ChatSpamType, ChatSpamTypes, sizeof(ChatSpamTypes) / sizeof(ChatSpamTypes[0]));
					ImGui::InputText("Message", Data::Settings::ChatSpamText, sizeof(Data::Settings::ChatSpamText));
					ImGui::Checkbox("Delay", &Data::Settings::ChatSpamDelay);
					if (Data::Settings::ChatSpamDelay)
						ImGui::SliderFloat("Delay Value", &Data::Settings::ChatSpamDelayValue, 0.0f, 10.0f, "%.1f");
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Items Magnet", &Data::Settings::EnableTeleportItems);
			if (Data::Settings::EnableTeleportItems)
			{
				if (ImGui::TreeNode("Items Magnet Settings..."))
				{
					ImGui::BeginChild("items-magnet-settings", g_MenuSettingsSize, true);
					ImGui::Checkbox("Health", &Data::Settings::TeleportItemsHealth);
					ImGui::Checkbox("Armor", &Data::Settings::TeleportItemsArmor);
					ImGui::Checkbox("Grenade", &Data::Settings::TeleportItemsGrenade);
					ImGui::Checkbox("Ammo", &Data::Settings::TeleportItemsAmmo);
					ImGui::Checkbox("Clips", &Data::Settings::TeleportItemsClips);
					ImGui::Checkbox("Akimbo", &Data::Settings::TeleportItemsAkimbo);
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Teleport Players", &Data::Settings::EnableTeleportPlayers);
			if (Data::Settings::EnableTeleportPlayers)
			{
				if (ImGui::TreeNode("Teleport Players Settings..."))
				{
					ImGui::BeginChild("teleport-players-settings", g_MenuSettingsSize, true);
					ImGui::SliderFloat("Teleport Distance", &Data::Settings::TeleportPlayersDistance, 0.0f, 50.0f, "%.1f");
					ImGui::Checkbox("Teleport Team", &Data::Settings::TeleportPlayersTeam);
					ImGui::Checkbox("Teleport Enemy", &Data::Settings::TeleportPlayersEnemy);
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Teleport", &Data::Settings::EnableTeleport);
			if (Data::Settings::EnableTeleport)
			{
				if (ImGui::TreeNode("Teleport Settings..."))
				{
					ImGui::BeginChild("teleport-settings", g_MenuSettingsSize, true);

					ImGui::InputFloat3("Teleport Position", Data::Settings::TeleportPosition, "%.1f");
					ImGui::Checkbox("Force X", &Data::Settings::TeleportForce[0]);
					ImGui::Checkbox("Force Y", &Data::Settings::TeleportForce[1]);
					ImGui::Checkbox("Force Z", &Data::Settings::TeleportForce[2]);

					if (Data::Keys::ToChange == &Data::Keys::TeleportSavePos)
						ImGui::Text("Save Position Key: [...]");
					else
						ImGui::Text("Save Position Key: %i", Data::Keys::TeleportSavePos);

					if (Data::Keys::ToChange == &Data::Keys::Teleport)
						ImGui::Text("Teleport Key: [...]");
					else
						ImGui::Text("Teleport Key: %i", Data::Keys::Teleport);

					if (ImGui::Button("Change Save Position Key..."))
						Data::Keys::ToChange = &Data::Keys::TeleportSavePos;

					if (ImGui::Button("Change Teleport Key..."))
						Data::Keys::ToChange = &Data::Keys::Teleport;
					if (ImGui::Button("Save Position")) Data::Settings::TeleportSaveQueued = true;
					if (ImGui::Button("Teleport")) Data::Settings::TeleportQueued = true;
					ImGui::EndChild();
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Server Sided Hacks", &Data::Settings::EnableServerSide);
			if (Data::Settings::EnableServerSide)
			{
				if (ImGui::TreeNode("Server Sided Hacks..."))
				{
					ImGui::Text("The hacks on this section do not work online.");
					ImGui::Checkbox("Godmode", &Data::Settings::EnableGodmode);
					ImGui::Checkbox("Instant Kill", &Data::Settings::EnableInstantKill);
					ImGui::Checkbox("Infinite Health", &Data::Settings::EnableInfiniteHealth);
					ImGui::Checkbox("Infinite Armor", &Data::Settings::EnableInfiniteArmor);
					ImGui::Checkbox("Infinite Ammo", &Data::Settings::EnableInfiniteAmmo);
					ImGui::Checkbox("No Gun Wait", &Data::Settings::EnableNoGunWait);
					ImGui::TreePop();
				}
			}

			/*
			if (ImGui::Button("Detach"))
			{
				ImGui::PopStyleColor(1);
				ImGui::EndChild();
				ImGui::PopFont();
				ImGui::End();
				ImGui::EndFrame();
				ImGui::Render();
				ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
				wglMakeCurrent(hdc, Data::oContext);
				wglDeleteContext(Data::glContext);
				Base::Unload();
				return Data::oSwapBuffers(hdc);
			}
			*/

			break;
		default:
			g_MenuActiveTab = 0;
			break;
		}
		ImGui::PopStyleColor(1);
		ImGui::EndChild();
		ImGui::PopFont();
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	wglMakeCurrent(hdc, Data::oContext);

	if (!Data::UnloadReady)
		Data::UnloadReady = true;

	return Data::oSwapBuffers(hdc);
}