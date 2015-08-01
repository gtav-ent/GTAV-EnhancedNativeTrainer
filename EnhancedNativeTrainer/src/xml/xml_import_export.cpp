/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "xml_import_export.h"
#include "..\version.h"

#include "..\debug\debuglog.h"

// A global Windows "basic string". Actual memory is allocated by the
// COM methods used by MSXML which take &keyconf_bstr. We must use SysFreeString() 
// to free this memory before subsequent uses, to prevent a leak.
BSTR xmlParser_bstr;

bool generate_xml_for_propset(SavedPropSet* props, std::string outputFile)
{
	//Create the XML
	IXMLDOMDocumentPtr pXMLDoc;
	HRESULT hr = pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (FAILED(hr))
	{
		write_text_to_log_file("Failed to create the XML class instance");
		return false;
	}

	VARIANT_BOOL bIsSuccessful;
	if (FAILED(pXMLDoc->loadXML(L"<object-set></object-set>", &bIsSuccessful)))
	{
		write_text_to_log_file("Root creation failed");
		handle_error(pXMLDoc);
		return false;
	}

	IXMLDOMProcessingInstructionPtr pXMLProcessingNode;
	pXMLDoc->createProcessingInstruction(L"xml", L" version=\"1.0\" encoding=\"UTF-8\"", &pXMLProcessingNode);

	//Get the root element just created    
	IXMLDOMElementPtr pXMLRootElem;
	pXMLDoc->get_documentElement(&pXMLRootElem);

	_variant_t vtObject;
	vtObject.vt = VT_DISPATCH;
	vtObject.pdispVal = pXMLRootElem;
	vtObject.pdispVal->AddRef();
	pXMLDoc->insertBefore(pXMLProcessingNode, vtObject, 0);

	//Add an attribute
	pXMLRootElem->setAttribute(L"set-name", _variant_t(props->saveName.c_str()));
	pXMLRootElem->setAttribute(L"ent-version", _variant_t(VERSION_STRING.c_str()));

	for each (SavedPropDBRow* row in props->items)
	{
		//Create child element
		IXMLDOMElementPtr objectNode;
		pXMLDoc->createElement(L"object", &objectNode);

		objectNode->setAttribute(L"title", _variant_t(row->title.c_str()));
		objectNode->setAttribute(L"model", _variant_t(row->model));

		objectNode->setAttribute(L"posX", _variant_t(row->posX));
		objectNode->setAttribute(L"posY", _variant_t(row->posY));
		objectNode->setAttribute(L"posZ", _variant_t(row->posZ));

		objectNode->setAttribute(L"roll", _variant_t(row->roll));
		objectNode->setAttribute(L"pitch", _variant_t(row->pitch));
		objectNode->setAttribute(L"yaw", _variant_t(row->yaw));

		objectNode->setAttribute(L"isImmovable", _variant_t((bool)(row->isImmovable == 1)));
		objectNode->setAttribute(L"isInvincible", _variant_t((bool)(row->isInvincible == 1)));
		objectNode->setAttribute(L"hasGravity", _variant_t((bool)(row->hasGravity == 1)));

		objectNode->setAttribute(L"alpha", _variant_t(row->alpha));
		objectNode->setAttribute(L"counter", _variant_t(row->counter));

		pXMLRootElem->appendChild(objectNode, 0);
	}

	char Path[MAX_PATH];

	IStreamPtr output;
	std::wstring ws;
	ws.assign(outputFile.begin(), outputFile.end());
	BSTR bs = SysAllocStringLen(ws.data(), ws.size());
	FileStream::OpenFile(bs, &output, true);

	if (!format_dom_document(pXMLDoc, output))
	{
		write_text_to_log_file("Save failed");
		write_text_to_log_file(outputFile);
		handle_error(pXMLDoc);
		return false;
	}
	else
	{
		write_text_to_log_file("Save complete");
		write_text_to_log_file(outputFile);
		return true;
	}
}

SavedPropSet* parse_xml_for_propset(HANDLE inputFile)
{
	CoInitialize(NULL);

	//read XML
	MSXML2::IXMLDOMDocumentPtr spXMLDoc;
	spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (!spXMLDoc->load(inputFile))
	{
		write_text_to_log_file("No config found, using defaults");
	}
	return false;
}

void handle_error(IXMLDOMDocumentPtr doc)
{
	std::ostringstream ss;
	IXMLDOMParseError* pError;
	ss << "XML error: ";
	doc->get_parseError(&pError);
	if (pError)
	{
		BSTR reasonStr;
		pError->get_reason(&reasonStr);
		ss << reasonStr;
	}
	else
	{
		ss << "Unknown";
	}
	write_text_to_log_file(ss.str());
}

bool format_dom_document(IXMLDOMDocument *pDoc, IStream *pStream)
{
	// Create the writer
	MSXML2::IMXWriterPtr pMXWriter;
	
	if (FAILED(pMXWriter.CreateInstance(__uuidof(MSXML2::MXXMLWriter60))))
	{
		return false;
	}
	MSXML2::ISAXContentHandler* pISAXContentHandler;
	if (FAILED(pMXWriter->QueryInterface(&pISAXContentHandler)))
	{
		return false;
	}
	MSXML2::ISAXErrorHandler* pISAXErrorHandler;
	if (FAILED(pMXWriter->QueryInterface(&pISAXErrorHandler)))
	{
		return false;
	}
	MSXML2::ISAXDTDHandler* pISAXDTDHandler;
	if (FAILED(pMXWriter->QueryInterface(&pISAXDTDHandler)))
	{
		return false;
	}

	if (FAILED(pMXWriter->put_omitXMLDeclaration(VARIANT_FALSE)) ||
		FAILED(pMXWriter->put_standalone(VARIANT_TRUE)) ||
		FAILED(pMXWriter->put_indent(VARIANT_TRUE)) ||
		FAILED(pMXWriter->put_encoding(L"UTF-8")))
	{
		return false;
	}

	// Create the SAX reader
	MSXML2::ISAXXMLReaderPtr pSAXReader;
	if (FAILED(pSAXReader.CreateInstance(__uuidof (MSXML2::SAXXMLReader60))))
	{
		return false;
	}

	wchar_t lexical_handler[] = L"http://xml.org/sax/properties/lexical-handler";
	wchar_t declaration_handler[] = L"http://xml.org/sax/properties/declaration-handler";

	if (FAILED(pSAXReader->putContentHandler(pISAXContentHandler)) ||
		FAILED(pSAXReader->putDTDHandler(pISAXDTDHandler)) ||
		FAILED(pSAXReader->putErrorHandler(pISAXErrorHandler)) ||
		FAILED(pSAXReader->putProperty(reinterpret_cast<unsigned short*>(lexical_handler), _variant_t(pMXWriter.GetInterfacePtr()))) ||
		FAILED(pSAXReader->putProperty(reinterpret_cast<unsigned short*>(declaration_handler), _variant_t(pMXWriter.GetInterfacePtr()))))
	{
		return false;
	}

	// Perform the write
	bool success1 = SUCCEEDED(pMXWriter->put_output(_variant_t(pStream)));
	bool success2 = SUCCEEDED(pSAXReader->parse(pDoc));

	return success1 && success2;
}