/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <string>
#include <sstream>
#include <iostream>

#include "filestream.hpp"
#include "../storage/database.h"

#import <msxml6.dll> //read the GitHub project readme regarding what you need to make this work

bool generate_xml_for_propset(SavedPropSet* props, std::string outputFile);

bool parse_xml_for_propset(std::string inputFile, SavedPropSet* set);

void handle_error(IXMLDOMDocumentPtr doc);

bool format_dom_document(IXMLDOMDocument *pDoc, IStream *pStream);