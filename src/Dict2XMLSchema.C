//$$FILE$$
//$$VERSION$
//$$DATE$$
//$$LICENSE$$


#include <string.h>
#include <string>

#include "DictObjFile.h"
#include "DictObjCont.h"
#include "DictDataInfo.h"
#include "DictParentChild.h"
#include "XsdWriter.h"
#include "PdbMlSchema.h"


using std::exception;
using std::string;
using std::fstream;
using std::ios;
using std::cerr;
using std::endl;


class CmdLineOpts
{
  public:
    string dFile;
    string dictName;
    string prefix;
    string ns;

    string progName;

    CmdLineOpts(int argc, char* argv[]);

    void Usage();
};


static void MakeXmlSchemaFileName(string& fileName, const string& prefix,
  DictDataInfo& dictDataInfo);


int main(int argc, char* argv[])
{
    try
    {
        CmdLineOpts opts(argc, argv);

        //  Read dictionary object ...
        DictObjFile cb(opts.dFile);

        cb.Read();

        DictObjCont& dictObjCont = cb.GetDictObjCont(opts.dictName);

        DictDataInfo dictDataInfo(dictObjCont);

        DictParentChild dictParentChild(dictObjCont, dictDataInfo);
 
        string xmlSchemaFileName;
        MakeXmlSchemaFileName(xmlSchemaFileName, opts.prefix, dictDataInfo);

        fstream xmlSchema;
        xmlSchema.open(xmlSchemaFileName.c_str(), ios::out);

        XsdWriter xsdWriter(xmlSchema);

        PdbMlSchema dictToXmlSchema(xsdWriter, dictParentChild, dictDataInfo,
          opts.ns, opts.prefix);

        dictToXmlSchema.Convert();

        xmlSchema.close();
    }
    catch (const exception& exc)
    {
        cerr << exc.what();

        return (1);
    }
}


CmdLineOpts::CmdLineOpts(int argc, char* argv[])
{
    progName = argv[0];

    if (argc < 2)
    {
        Usage();
        throw InvalidOptionsException();
    }

    for (unsigned int i = 1; i < (unsigned int)argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-df") == 0)
            {
                i++;
                dFile = argv[i];          
            }
            else if (strcmp(argv[i], "-dictName") == 0)
            {
                i++;
                dictName = argv[i];          
            }
            else if (strcmp(argv[i], "-prefix") == 0)
            {
                i++;
                prefix = argv[i];          
            }
            else if (strcmp(argv[i], "-ns") == 0)
            {
                i++;
                ns = argv[i];
            }
            else
            {
                Usage();
                throw InvalidOptionsException();
            }
        }
        else
        {
            Usage();
            throw InvalidOptionsException();
        }
    }

    if (dFile.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }

    if (dictName.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }

    if (prefix.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }
}


void CmdLineOpts::Usage()
{
    cerr << progName << " usage =  " << endl;
    cerr << "             -df <dict_file>  -dictName <dict_name> " << endl;
    cerr << "             -ns <name_space>  -prefix <prefix_name> " << endl;
    cerr << endl;
    cerr << "  ALL PARAMETERS ARE REAQUIRED ... " << endl;
}


void MakeXmlSchemaFileName(string& fileName, const string& prefix,
  DictDataInfo& dictDataInfo)
{
    string dictVer;
    dictDataInfo.GetVersion(dictVer);

    if (dictVer.empty())
        dictVer = "1.00";

    PdbMlSchema::MakeSchemaFileName(fileName, prefix, dictVer);
}

