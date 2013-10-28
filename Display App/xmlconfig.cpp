#include "xmlconfig.h"

#include <exception>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Text.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/SAX/InputSource.h>
#include <fstream>

using namespace Poco;
using namespace Poco::XML;

XMLConfig::XMLConfig()
{

}

XMLConfig::~XMLConfig()
{

}

void XMLConfig::Load( std::string FilePath )
{
	// Attempt to open the file
	std::ifstream infile(FilePath);

	if (!infile.is_open())
	{
		throw std::exception(FilePath.insert(0,"XMLConfig::Load() Could not locate file: ").c_str());
	}

	m_CurrentFilePath = FilePath;

	// Parse the XML document
	InputSource src(infile);
	DOMParser parser;
	m_pDocument = parser.parse(&src);


	// Create config objects
	NodeIterator itNode(m_pDocument, NodeFilter::SHOW_ELEMENT|NodeFilter::SHOW_ATTRIBUTE);
	Node* pNode = itNode.nextNode();

	while (pNode)
	{
		switch(pNode->nodeType())
		{
		case Node::ELEMENT_NODE:
			if (pNode->nodeName() == "MFCDX")
			{
				// root element
			}
			else if (pNode->nodeName() == "Server")
			{
				// The server/source (there can only be one, for now.)

			}
			else if (pNode->nodeName() == "Window")
			{
				// A local directx window
			}
			else if (pNode->nodeName() == "Source")
			{
				// Add a source texture to the current window.
			}
			break;
		case Node::ATTRIBUTE_NODE:

			break;
		default:

			// Don't care
			break;
		}
		

		pNode = itNode.nextNode();
	}

}

void XMLConfig::Save()
{

}

void XMLConfig::SaveAs( std::string FilePath )
{

}
