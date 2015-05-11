/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "menu_functions.h"
#include "script.h"

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

bool menu_showing = false;

void(*periodic_feature_call)(void) = NULL;

void set_periodic_feature_call(void method(void))
{
	periodic_feature_call = method;
}

void make_periodic_feature_call()
{
	periodic_feature_call();
}

void set_menu_showing(bool showing)
{
	menu_showing = showing;
}

bool is_menu_showing()
{
	return menu_showing;
}

void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText)
{
	// default values
	int text_col[4] = { 255, 255, 255, 255.0f },
		rect_col[4] = { 255, 255, 255, 80.f };
	float text_scale = 0.35;
	int font = 0;
	bool outline = false;
	bool dropShadow = false;

	// correcting values for active line
	if (active)
	{
		text_col[0] = 0;
		text_col[1] = 0;
		text_col[2] = 0;

		rect_col[0] = 255;
		rect_col[1] = 180;
		rect_col[2] = 0;
		rect_col[3] = 200.0f;

		//outline = true;

		if (rescaleText) text_scale = 0.40;
	}
	else if (title)
	{
		text_col[0] = 255;
		text_col[1] = 255;
		text_col[2] = 255;

		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;
		rect_col[3] = 200.0f;

		if (rescaleText) text_scale = 0.60;
		font = 2;
	}
	else
	{
		outline = true;
	}

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height

	float lineLeftScaled = lineLeft / (float)screen_w;

	float textHeightScaled = (title ? TEXT_HEIGHT_TITLE : TEXT_HEIGHT_NORMAL) / (float)screen_h;

	// this is how it's done in original scripts

	// text upper part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);

	if (outline)
	{
		UI::SET_TEXT_OUTLINE();
	}

	if (dropShadow)
	{
		UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
	}

	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());

	UI::_DRAW_TEXT(textLeftScaled, lineTopScaled + (0.5f * (lineHeightScaled - textHeightScaled)));

	// rect
	draw_rect(lineLeftScaled, lineTopScaled,
		lineWidthScaled, lineHeightScaled,
		rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
}

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
{
	//this craziness is required - X and Y are strange
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
}

void set_status_text(std::string str, DWORD time, bool isGxtEntry)
{
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

void menu_beep()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		}
		else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5);
	}
}

void draw_menu_from_struct_def(StandardOrToggleMenuDef defs[], int lineCount, int* selectionRef, std::string caption, bool(*onConfirmation)(MenuItem<int> value))
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < lineCount; i++)
	{
		if (defs[i].pState != NULL)
		{
			ToggleMenuItem<int> *item = new ToggleMenuItem<int>();
			item->caption = defs[i].text;
			item->value = i;
			item->toggleValue = defs[i].pState;
			item->isLeaf = false;
			if (defs[i].pUpdated != NULL)
			{
				item->toggleValueUpdated = defs[i].pUpdated;
			}
			menuItems.push_back(item);
		}
		else if (defs[i].itemType != NULL && defs[i].itemType == WANTED)
		{
			WantedSymbolItem<int> *item = new WantedSymbolItem<int>();
			item->caption = defs[i].text;
			item->value = i;
			menuItems.push_back(item);
		}
		else
		{
			MenuItem<int> *item = new MenuItem<int>();
			item->caption = defs[i].text;
			item->value = i;
			item->isLeaf = defs[i].isLeaf;
			menuItems.push_back(item);
		}
	}

	draw_generic_menu<int>(menuItems, selectionRef, caption, onConfirmation, NULL, NULL);
}

void draw_menu_from_struct_def(StringStandardOrToggleMenuDef defs[], int lineCount, int* selectionRef, std::string caption, bool(*onConfirmation)(MenuItem<std::string> value))
{
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < lineCount; i++)
	{
		if (defs[i].pState != NULL)
		{
			ToggleMenuItem<std::string> *item = new ToggleMenuItem<std::string>();
			item->caption = defs[i].text;
			item->toggleValue = defs[i].pState;
			item->currentMenuIndex = i;
			item->value = defs[i].value;
			if (defs[i].pUpdated != NULL)
			{
				item->toggleValueUpdated = defs[i].pUpdated;
			}
			menuItems.push_back(item);
		}
		else
		{
			MenuItem<std::string> *item = new MenuItem<std::string>();
			item->caption = defs[i].text;
			item->value = defs[i].value;
			item->currentMenuIndex = i;
			menuItems.push_back(item);
		}
	}

	draw_generic_menu<std::string>(menuItems, selectionRef, caption, onConfirmation, NULL, NULL);
}


template<class T>
void MenuItem<T>::onConfirm()
{
	//set_status_text("Parent confirm");
	if (onConfirmFunction != NULL)
	{
		onConfirmFunction(*this);
	}
}


template<class T>
void ToggleMenuItem<T>::onConfirm()
{
	//set_status_text("Base confirm");

	//call super
	MenuItem::onConfirm();

	//toggle the value if there is none
	if (toggleValue != NULL)
	{
		*toggleValue = !*toggleValue;
		//set_status_text(*toggleValue ? "Now ON" : "Now OFF");
		if (toggleValueUpdated != NULL)
		{
			*toggleValueUpdated = true;
		}
	}
}

template<class T>
int WantedSymbolItem<T>::get_wanted_value()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	return PLAYER::GET_PLAYER_WANTED_LEVEL(player);
}

template<class T>
void WantedSymbolItem<T>::handleLeftPress()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0)
	{
		setFrozenWantedFeature(true);
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) - 1, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		setFrozenWantedLvl(PLAYER::GET_PLAYER_WANTED_LEVEL(player));
		std::stringstream st;
		if (getFrozenWantedLvl() > 0){ st << "Wanted Level: " << getFrozenWantedLvl() << " Star(s)"; }
		else{ st << "Wanted Level Cleared"; }
		set_status_text(st.str());
	}
}

template<class T>
void WantedSymbolItem<T>::handleRightPress()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) < 5)
	{
		setFrozenWantedFeature(true);
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) + 1, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		setFrozenWantedLvl(PLAYER::GET_PLAYER_WANTED_LEVEL(player));
		std::stringstream st;
		st << "Wanted Level: " << getFrozenWantedLvl() << " Star(s)";
		set_status_text(st.str());
	}
}