#include <pch.h>
#include <base.h>

void Base::Hacks::ESP_Info(playerinfo_t* p_info)
{
	if (!p_info || !p_info->is_valid || !((Data::Settings::EnableEspTeam && p_info->ent->team == Data::game.player1->team) || (Data::Settings::EnableEspEnemy && p_info->ent->team != Data::game.player1->team))) return;

	ImDrawList* Draw = ImGui::GetBackgroundDrawList();
	float Height = p_info->pos2D.y - p_info->headpos2D.y;
	float BarWidth = Height / 2;
	float BarHeight = Height / 20;
	float Padding = Height / 14;
	float SpaceDown = 0.0f;
	float SpaceUp = 0.0f;
	float FontHeight = 18.0f;
	float FontWidth = FontHeight * 0.325f;

	if (Data::Settings::EnableEspHealth && !(p_info->pos2D_out & (OUT_TOP | OUT_BOTTOM | OUT_LEFT | OUT_RIGHT)))
	{
		SpaceDown += Padding;
		ImVec2 HealthBarPos = ImVec2(p_info->pos2D.x - BarWidth / 2, p_info->pos2D.y + SpaceDown);
		ImVec2 HealthBarEnd = ImVec2(p_info->pos2D.x + BarWidth / 2, HealthBarPos.y + BarHeight);
		ImColor DamageColor = ImColor(Data::Settings::EspHealthDmgColor[0], Data::Settings::EspHealthDmgColor[1], Data::Settings::EspHealthDmgColor[2], Data::Settings::EspHealthDmgColor[3]);
		ImColor HealthColor = ImColor(Data::Settings::EspHealthColor[0], Data::Settings::EspHealthColor[1], Data::Settings::EspHealthColor[2], Data::Settings::EspHealthColor[3]);

		ImVec2 p1 = HealthBarPos;
		ImVec2 p2 = ImVec2(p1.x, HealthBarEnd.y);
		ImVec2 p3 = HealthBarEnd;
		ImVec2 p4 = ImVec2(p3.x, p1.y);

		float OutlineThickness = (p2.y - p1.y) / 4.0f;
		ImVec2 op1 = { p1.x - OutlineThickness / 2.0f, p1.y - OutlineThickness / 2.0f };
		ImVec2 op2 = { p2.x - OutlineThickness / 2.0f, p2.y + OutlineThickness / 2.0f };
		ImVec2 op3 = { p3.x + OutlineThickness / 2.0f, p3.y + OutlineThickness / 2.0f };
		ImVec2 op4 = { p4.x + OutlineThickness / 2.0f, p4.y - OutlineThickness / 2.0f };

		Draw->AddQuadFilled(p1, p2, p3, p4, DamageColor);

		p1 = p4;
		p1.x -= ((float)p_info->ent->health / 100.0f) * BarWidth;
		p2.x = p1.x;

		Draw->AddQuadFilled(p1, p2, p3, p4, HealthColor);
		Draw->AddQuad(op1, op2, op3, op4, ImColor(0.0f, 0.0f, 0.0f, 1.0f), OutlineThickness);

		SpaceDown += BarHeight;
		SpaceDown += OutlineThickness;
	}

	if (Data::Settings::EnableEspArmor && !(p_info->pos2D_out & (OUT_TOP | OUT_BOTTOM | OUT_LEFT | OUT_RIGHT)))
	{
		SpaceDown += Padding;
		ImVec2 ArmorBarPos = ImVec2(p_info->pos2D.x - BarWidth / 2, p_info->pos2D.y + SpaceDown);
		ImVec2 ArmorBarEnd = ImVec2(p_info->pos2D.x + BarWidth / 2, ArmorBarPos.y + BarHeight);
		ImColor DamageColor = ImColor(Data::Settings::EspArmorDmgColor[0], Data::Settings::EspArmorDmgColor[1], Data::Settings::EspArmorDmgColor[2], Data::Settings::EspArmorDmgColor[3]);
		ImColor ArmorColor = ImColor(Data::Settings::EspArmorColor[0], Data::Settings::EspArmorColor[1], Data::Settings::EspArmorColor[2], Data::Settings::EspArmorColor[3]);

		ImVec2 p1 = ArmorBarPos;
		ImVec2 p2 = ImVec2(p1.x, ArmorBarEnd.y);
		ImVec2 p3 = ArmorBarEnd;
		ImVec2 p4 = ImVec2(p3.x, p1.y);

		float OutlineThickness = (p2.y - p1.y) / 4.0f;
		ImVec2 op1 = { p1.x - OutlineThickness / 2.0f, p1.y - OutlineThickness / 2.0f };
		ImVec2 op2 = { p2.x - OutlineThickness / 2.0f, p2.y + OutlineThickness / 2.0f };
		ImVec2 op3 = { p3.x + OutlineThickness / 2.0f, p3.y + OutlineThickness / 2.0f };
		ImVec2 op4 = { p4.x + OutlineThickness / 2.0f, p4.y - OutlineThickness / 2.0f };

		Draw->AddQuadFilled(p1, p2, p3, p4, DamageColor);

		p1 = p4;
		p1.x -= ((float)p_info->ent->armour / 100.0f) * BarWidth;
		p2.x = p1.x;

		Draw->AddQuadFilled(p1, p2, p3, p4, ArmorColor);
		Draw->AddQuad(op1, op2, op3, op4, ImColor(0.0f, 0.0f, 0.0f, 1.0f), OutlineThickness);

		SpaceDown += BarHeight;
		SpaceDown += OutlineThickness;
	}

	if (Data::Settings::EnableEspName && !(p_info->headpos2D_out & (OUT_TOP | OUT_BOTTOM | OUT_LEFT | OUT_RIGHT)))
	{
		SpaceUp += Padding;
		size_t  TextLength = strlen(p_info->ent->name);
		float   TextWidth = (float)TextLength * FontWidth;
		float   TextHeight = FontHeight;
		ImColor TextColor = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
		if (p_info->ent->team == Data::game.player1->team && (m_teammode || m_coop))
			TextColor = ImColor(Data::Settings::EspNameColorTeam[0], Data::Settings::EspNameColorTeam[1], Data::Settings::EspNameColorTeam[2], Data::Settings::EspNameColorTeam[3]);
		else
			TextColor = ImColor(Data::Settings::EspNameColorEnemy[0], Data::Settings::EspNameColorEnemy[1], Data::Settings::EspNameColorEnemy[2], Data::Settings::EspNameColorEnemy[3]);
		ImVec2  TextPos = ImVec2(p_info->pos2D.x - (TextWidth / 2), p_info->pos2D.y - Height - SpaceUp - TextHeight);

		Draw->AddText(TextPos, TextColor, (const char*)p_info->ent->name);

		SpaceUp += TextHeight;
	}
}