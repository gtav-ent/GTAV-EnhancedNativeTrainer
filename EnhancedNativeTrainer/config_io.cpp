/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "config_io.h"
#include "keyboard.h"
#include "script.h"
#include "debuglog.h"
#include <sstream>

// A global Windows "basic string". Actual memory is allocated by the
// COM methods used by MSXML which take &bstr. We must use SysFreeString() 
// to free this memory before subsequent uses, to prevent a leak.
BSTR bstr;

TrainerConfig *config = NULL;

/**Read the XML config file. Currently contains keyboard choices.*/
void read_config_file()
{
	TrainerConfig *result = new TrainerConfig();

	CoInitialize(NULL);

	//read XML
	MSXML2::IXMLDOMDocumentPtr spXMLDoc;
	spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (!spXMLDoc->load("ent-config.xml"))
	{
		write_text_to_log_file("No config found, using defaults");
		config = result; //the default config
	}

	IXMLDOMNodeListPtr nodes = spXMLDoc->selectNodes(L"//ent-config/keys/key");

	long length;
	nodes->get_length(&length);
	for (int i = 0; i < length; i++)
	{
		IXMLDOMNode *node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap *attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char *attrib_key_func = NULL;
		char *attrib_key_value = NULL;

		for (long j = 0; j < length_attribs; j++)
		{
			IXMLDOMNode *attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&bstr);
			if (wcscmp(bstr, L"function") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_key_func = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			else if (wcscmp(bstr, L"value") == 0)
			{
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_key_value = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			SysFreeString(bstr);
			attribNode->Release();
		}
		
		if (attrib_key_func != NULL && attrib_key_value != NULL)
		{
			result->get_key_config()->set_key(attrib_key_func, attrib_key_value);
		}
		
		delete attrib_key_func;
		delete attrib_key_value;

		attribs->Release();
		node->Release();
	}

	//nodes->Release(); //don't do this, it crashes on exit
	spXMLDoc.Release();
	CoUninitialize();
	
	config = result;
}

void KeyInputConfig::set_key(char* function, char* keyName)
{
	std::ostringstream ss;
	ss << "Key function " << function << " being given " << keyName;
	write_text_to_log_file(ss.str());

	int vkID = keyNameToVal(keyName);
	if (vkID == -1)
	{
		ss.str(""); ss.clear();
		ss << "Key function " << keyName << " didn't correspond to a value";
		write_text_to_log_file(ss.str());
		return;
	}

	if (strcmp(function, "menu_up") == 0)
	{
		key_menu_up = vkID;
	}
	else if (strcmp(function, "menu_down") == 0)
	{
		key_menu_down = vkID;
	}
	else if (strcmp(function, "menu_left") == 0)
	{
		key_menu_left = vkID;
	}
	else if (strcmp(function, "menu_right") == 0)
	{
		key_menu_right = vkID;
	}
	else if (strcmp(function, "menu_select") == 0)
	{
		key_menu_confirm = vkID;
	}
	else if (strcmp(function,"menu_back") == 0)
	{
		key_menu_back = vkID;
	}
	else if (strcmp(function, "toggle_main_menu") == 0)
	{
		key_toggle_main_menu = vkID;
	}
	else if (strcmp(function, "veh_boost") == 0)
	{
		key_veh_boost = vkID;
	}
	else if (strcmp(function, "veh_stop") == 0)
	{
		key_veh_stop = vkID;
	}
	else if (strcmp(function, "veh_rockets") == 0)
	{
		key_veh_rockets = vkID;
	}

	else if (strcmp(function, "toggle_airbrake") == 0)
	{
		key_toggle_airbrake = vkID;
	}
	else if (strcmp(function, "airbrake_up") == 0)
	{
		key_airbrake_up = vkID;
	}
	else if (strcmp(function, "airbrake_down") == 0)
	{
		key_airbrake_down = vkID;
	}
	else if (strcmp(function, "airbrake_forward") == 0)
	{
		key_airbrake_forward = vkID;
	}
	else if (strcmp(function, "airbrake_back") == 0)
	{
		key_airbrake_back = vkID;
	}
	else if (strcmp(function, "airbrake_rotate_left") == 0)
	{
		key_airbrake_rotate_left = vkID;
	}
	else if (strcmp(function, "airbrake_rotate_right") == 0)
	{
		key_airbrake_rotate_right = vkID;
	}
	else if (strcmp(function, "airbrake_speed") == 0)
	{
		key_airbrake_speed = vkID;
	}
	else if (strcmp(function, "hotkey_throughdoor") == 0)
	{
		key_hotkey_throughdoor = vkID;
	}

	else
	{
		ss.str(""); ss.clear();
		ss << "Key function " << function << " didn't correspond to a known function";
		write_text_to_log_file(ss.str());
	}
};

TrainerConfig::TrainerConfig()
{
	this->keyConfig = new KeyInputConfig();
}