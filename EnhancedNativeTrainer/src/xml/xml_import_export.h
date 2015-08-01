/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <string>
#include <sstream>

#include "../storage/database.h"

#import <msxml6.dll> //read the GitHub project readme regarding what you need to make this work

bool generate_xml_for_propset(SavedPropSet* props, std::string outputFile);

SavedPropSet* parse_xml_for_propset(HANDLE inputFile);

void handle_error(IXMLDOMDocumentPtr doc);