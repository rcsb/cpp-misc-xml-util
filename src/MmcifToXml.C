//$$FILE$$
//$$VERSION$$
//$$DATE$$
//$$LICENSE$$


#include <string>
#include <iostream>
#include <algorithm>

#include "GenCont.h"
#include "RcsbFile.h"
#include "CifFile.h"
#include "PdbMlSchema.h"
#include "DataInfo.h"
#include "MmcifToXml.h"


using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::ios;
using std::sort;


const string MmcifToXml::All("all");
const string MmcifToXml::NoAtom("noatom");
const string MmcifToXml::ExtAtom("extatom");


MmcifToXml::MmcifToXml(CifFile& cifFile, DataInfo& dataInfo,
  const string& schemaPrefix) : _cifFile(cifFile), _dataInfo(dataInfo)
{
    NoAtomCategories.push_back("atom_site");
    NoAtomCategories.push_back("atom_site_anisotrop");

    ExtAtomCategories.push_back("atom_site");

    _schemaPrefix = "pdbx";

    if (!schemaPrefix.empty())
        _schemaPrefix = schemaPrefix;
}


MmcifToXml::~MmcifToXml()
{

}


void MmcifToXml::Convert(const string& xmlFileName, const string& ns,
  const vector<string>& opts)
{
    for (unsigned int optI = 0; optI < opts.size(); ++optI)
    {
        string outFile = xmlFileName;
        AppendToOutputFileName(outFile, opts[optI]);

        Convert(outFile, ns, opts[optI]);
    }
}


void MmcifToXml::Convert(const string& xmlFileName, const string& ns,
  const string& opt)
{
    SetSchemaFileNamePrefix(opt);

    ofstream outXml;
    outXml.open(xmlFileName.c_str(), ios::out | ios::trunc);

    PdbMlWriter pdbMlWriter(outXml, ns, _dataInfo);

    if (opt.empty())
    {
        WriteXmlFile(pdbMlWriter);
    }
    else if (opt == NoAtom)
    {
        // No atoms nor anisotrop temperature factors ...
        WriteXmlFile(pdbMlWriter, opt, NoAtomCategories);
    }
    else if (opt == ExtAtom)
    {
        // Only alternate atom records ...
        WriteXmlFile(pdbMlWriter, opt, ExtAtomCategories);
    }

    outXml.close();
}


void MmcifToXml::WriteXmlFile(PdbMlWriter& pdbMlWriter,
  const string& option, const vector<string>& specialCategories)
{
    pdbMlWriter.WriteDeclaration();
    pdbMlWriter.WriteNewLine();

    vector<string> blockNames;
    _cifFile.GetBlockNames(blockNames);
    for (unsigned int ib = 0; ib < blockNames.size(); ++ib)
    {
        const string& blockName = blockNames[ib];
        Block& block = _cifFile.GetBlock(blockName);

        pdbMlWriter.WriteDatablockOpeningTag();

        if (!blockName.empty())
        {
            string dataBlockName;
            MakeDataBlockNameAttributeValue(dataBlockName, blockName, option);

            pdbMlWriter.WriteDatablockAttribute(dataBlockName);
        }

        pdbMlWriter.WriteNewLine();

        pdbMlWriter.IncrementIndent();

        pdbMlWriter.Indent();
        WriteNamespaceDeclaration(pdbMlWriter);
        pdbMlWriter.WriteNewLine();
 
        pdbMlWriter.Indent();
        pdbMlWriter.WriteXsiNamespace();
        pdbMlWriter.WriteNewLine();

        pdbMlWriter.Indent();
        WriteSchemaLocation(pdbMlWriter);

        pdbMlWriter.WriteClosingBracket();
        pdbMlWriter.WriteNewLine();

        pdbMlWriter.DecrementIndent();

        pdbMlWriter.IncrementIndent();

        vector<string> tableNames;
        block.GetTableNames(tableNames);

        sort(tableNames.begin(), tableNames.end());

        for (unsigned int it = 0; it < tableNames.size(); ++it)
        {
            if (!option.empty())
            {
                if (option == ExtAtom)
                {
                    if (!GenCont::IsInVectorCi(tableNames[it],
                      specialCategories))
                    {
                        tableNames[it].clear();
                        continue;
                    }
                }
                else
                {
                    if (GenCont::IsInVectorCi(tableNames[it],
                      specialCategories))
                    {
                        tableNames[it].clear();
                        continue;
                    }
                }
            }

            if (!_dataInfo.IsCatDefined(tableNames[it]))
            {
                cerr << " Skipping conversion to XML of the unknown "\
                  "table \"" << tableNames[it] << "\"" << endl;
                tableNames[it].clear();
                continue;
            }

            ISTable* t = block.GetTablePtr(tableNames[it]);
            if ((t != NULL) && (t->GetNumRows() > 0))
            {
                if (option == ExtAtom)
	            pdbMlWriter._writeAlternateAtomSiteTable(t);
                else
                {
                    vector<unsigned int> emptyWidths;
	            pdbMlWriter.WriteTable(t, emptyWidths);
                }
            }
            tableNames[it].clear();
        }

        pdbMlWriter.DecrementIndent();

        pdbMlWriter.WriteDatablockClosingTag();
    }
}


void MmcifToXml::SetSchemaFileNamePrefix(const string& opt)
{
    _schemaFileNamePrefix = _schemaPrefix;

    if (opt == ExtAtom)
    {
        _schemaFileNamePrefix += "-ext";
    }
}


void MmcifToXml::WriteNamespaceDeclaration(PdbMlWriter& pdbMlWriter)
{
    string fullSchemaFileName;
    PdbMlSchema::MakeFullSchemaFileName(fullSchemaFileName,
      _schemaFileNamePrefix);

    pdbMlWriter.WriteNamespaceAttribute(pdbMlWriter.GetNamespace(),
      fullSchemaFileName, true);
}


void MmcifToXml::WriteSchemaLocation(PdbMlWriter& pdbMlWriter)
{
    string fullSchemaFileName;
    PdbMlSchema::MakeFullSchemaFileName(fullSchemaFileName,
      _schemaFileNamePrefix);

    string schemaFileName;
    PdbMlSchema::MakeSchemaFileName(schemaFileName, _schemaFileNamePrefix);

    pdbMlWriter.WriteSchemaLocationAttribute(fullSchemaFileName + " " +
     schemaFileName, true);
}


void MmcifToXml::MakeOutputFileName(string& oFile, const string& inFile)
{
    string relInFileName;
    RcsbFile::RelativeFileName(relInFileName, inFile);

    oFile = relInFileName + ".xml";
}


void MmcifToXml::AppendToOutputFileName(string& oFile, const string& funct)
{
    if (!funct.empty())
    {
        oFile += "-";
        oFile += funct;
    }
}


void MmcifToXml::MakeDataBlockNameAttributeValue(string& dataBlockName,
      const string& blockName, const string& option)
{
    dataBlockName = blockName;

    if (!option.empty())
    {
        dataBlockName += "-";
        dataBlockName += option;
    }
}

