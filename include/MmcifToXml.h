//$$FILE$$
//$$VERSION$$
//$$DATE$$
//$$LICENSE$$


/**
** \file MmcifToXml.h
**
** CIF to XML converter class.
*/


#ifndef MMCIFTOXML_H
#define MMCIFTOXML_H


#include <string>
#include <vector>

#include "DataInfo.h"
#include "CifFile.h"
#include "PdbMlWriter.h"


class MmcifToXml
{
  public:
    static const std::string All;
    static const std::string NoAtom;
    static const std::string ExtAtom;

    static void MakeOutputFileName(std::string& oFile,
      const std::string& inFile);
    static void AppendToOutputFileName(std::string& oFile,
      const std::string& funct);

    MmcifToXml(CifFile& cifFile, DataInfo& dataInfo,
      const std::string& schemaPrefix);
    ~MmcifToXml();

    void Convert(const std::string& xmlFileName, const std::string& ns,
      const std::vector<std::string>& opts);
    void Convert(const std::string& xmlFileName, const std::string& ns,
      const std::string& opt = std::string());

  private:
    std::vector<std::string> NoAtomCategories;
    std::vector<std::string> ExtAtomCategories;

    CifFile& _cifFile;

    DataInfo& _dataInfo;

    std::string _schemaPrefix;

    std::string _schemaFileNamePrefix;

    void SetSchemaFileNamePrefix(const std::string& opt);

    void WriteNamespaceDeclaration(PdbMlWriter& pdbMlWriter);
    void WriteSchemaLocation(PdbMlWriter& pdbMlWriter);

    void WriteXmlFile(PdbMlWriter& pdbMlWriter,
      const std::string& option = std::string(),
      const std::vector<std::string>& specialCategories =
      std::vector<std::string>());

    void MakeDataBlockNameAttributeValue(std::string& dataBlockName,
      const std::string& blockName, const std::string& option);
};


#endif
