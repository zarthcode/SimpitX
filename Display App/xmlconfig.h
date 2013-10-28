// Xerces-C based XML configuration class
// Allows the storage/retrieval of XML name-value pairs
#include <string>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>

class XMLConfig
{
public:
	XMLConfig();
	~XMLConfig();

	void Load(std::string FilePath);
	void Save();
	void SaveAs(std::string FilePath);

	bool GetSetting(std::string Name);

//	template <type T>
//	T& GetSetting(std::string SettingName);

protected:


private:

	Poco::AutoPtr<Poco::XML::Document> m_pDocument;
	std::string m_CurrentFilePath;

};