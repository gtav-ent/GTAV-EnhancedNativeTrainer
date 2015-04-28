#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

#include "io.h"

#include <string>
#include <sstream> 

#include <ctime>
#include <vector>

#pragma warning(disable : 4244 4305) // double <-> float conversions

extern void(*periodic_feature_call)(void);

void make_periodic_feature_call();

void set_periodic_feature_call(void method(void));

void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true);

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7);

std::string line_as_str(std::string text, bool *pState);

/**This is in the header because of the use of templates. There's probably a better way.*/
template<typename T>
bool drawGenericMenu(std::vector<std::string> captions, std::vector<T> values, int currentSelectionIndex, std::string headerText,
	bool(*onConfirmation)(int selectedIndex, std::string caption, T value),
	void(*onHighlight)(int selectedIndex, std::string caption, T value),
	void(*onExit)(bool returnValue))
{
	bool result = false;
	DWORD waitTime = 150;
	const int totalItems = (int)captions.size();
	const int itemsPerLine = 10;
	const int lineCount = (int)(ceil((double)totalItems / (double)itemsPerLine));

	if (onHighlight != NULL)
	{
		onHighlight(currentSelectionIndex, captions[currentSelectionIndex], values[currentSelectionIndex]);
	}

	while (true)
	{
		const int currentLine = floor((double)currentSelectionIndex / (double)itemsPerLine);

		const int originalIndex = currentSelectionIndex;

		int positionOnThisLine = currentSelectionIndex % itemsPerLine;
		int lineStartPosition = currentSelectionIndex - positionOnThisLine;
		int itemsOnThisLine = (lineStartPosition + itemsPerLine > totalItems) ? (totalItems - lineStartPosition) : itemsPerLine;

		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			std::ostringstream ss;
			ss << headerText;
			if (lineCount > 1)
			{
				ss << " " << (currentLine + 1) << " / " << lineCount;
			}

			draw_menu_line(ss.str(), 350.0, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < itemsOnThisLine; i++)
			{
				std::string thisItem = captions[lineStartPosition + i];

				/*
				//old horizontals
				float lineWidth = 100.0f;
				float lineHeight = 5.0f;
				float lineTop = 200.0;
				float lineLeft = 100.0f + i * 110.0f;
				*/

				float lineWidth = 350.0f;
				float lineHeight = 10.0f;
				float lineTop = 100.0 + (i * 50.0f);
				float lineLeft = 50.0f;

				draw_menu_line(thisItem, lineWidth, lineHeight, lineTop, lineLeft, 5.0f, i == positionOnThisLine, false, false);
			}

			if ( periodic_feature_call != NULL )
			{
				periodic_feature_call();
			}

			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);

		if (bSelect)
		{
			menu_beep();
			result = onConfirmation(currentSelectionIndex, captions[currentSelectionIndex], values[currentSelectionIndex]);
			if (result)
			{
				waitTime = 200;
				result = false; //to avoid cascading upwards
				break;
			}
		}
		else
		{
			if (bBack)
			{
				menu_beep();
				waitTime = 200;
				result = false;
				break;
			}
			else
			{
				if (bDown)
				{
					menu_beep();
					currentSelectionIndex++;
					if (currentSelectionIndex >= totalItems)
						currentSelectionIndex = lineStartPosition;
					waitTime = 100;
				}
				else
					if (bUp)
					{
						menu_beep();
						currentSelectionIndex--;
						if (currentSelectionIndex < 0)
							currentSelectionIndex = lineStartPosition + itemsOnThisLine - 1;
						waitTime = 100;
					}
					else
						if (bLeft)
						{
							menu_beep();
							currentSelectionIndex -= itemsPerLine;
							if (currentSelectionIndex < 0)
								currentSelectionIndex = totalItems - 1;
							waitTime = 200;
						}
						else
							if (bRight)
							{
								menu_beep();
								currentSelectionIndex += itemsPerLine;
								if (currentSelectionIndex >= totalItems)
									currentSelectionIndex = 0;
								waitTime = 200;
							}

				if (onHighlight != NULL && originalIndex != currentSelectionIndex)
				{
					onHighlight(currentSelectionIndex, captions[currentSelectionIndex], values[currentSelectionIndex]);
				}
			}
		}
	}

	if (onExit != NULL)
	{
		onExit(result);
	}

	// wait before exit
	if (waitTime > 0)
	{
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;
	}

	return result;
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false);

void menu_beep();

void update_status_text();