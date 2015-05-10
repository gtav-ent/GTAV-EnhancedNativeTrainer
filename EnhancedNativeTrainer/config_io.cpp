/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "config_io.h"
#include "keyboard.h"
#include "script.h"
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
	spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
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

	if (strcmp(function, "up") == 0)
	{
		key_up = vkID;
	}
	else if (strcmp(function, "down") == 0)
	{
		key_down = vkID;
	}
	else if (strcmp(function, "left") == 0)
	{
		key_left = vkID;
	}
	else if (strcmp(function, "right") == 0)
	{
		key_right = vkID;
	}
	else if (strcmp(function, "select") == 0)
	{
		key_confirm = vkID;
	}
	else if (strcmp(function,"back") == 0)
	{
		key_back = vkID;
	}
	else if (strcmp(function, "activate") == 0)
	{
		key_activate = vkID;
	}
	else if (strcmp(function, "boost") == 0)
	{
		key_boost = vkID;
	}
	else if (strcmp(function, "airbrake") == 0)
	{
		key_airbrake = vkID;
	}
	else if (strcmp(function, "rockets") == 0)
	{
		key_rockets = vkID;
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