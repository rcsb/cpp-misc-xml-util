//$$FILE$$
//$$VERSION$$
//$$DATE$$
//$$LICENSE$$


#include <string.h>
#include <exception>
#include <string>
#include <iostream>

#include "Exceptions.h"
#include "DictObjCont.h"
#include "DictObjFile.h"
#include "DictDataInfo.h"
#include "CifDataInfo.h"
#include "CifFileUtil.h"
#include "MmcifToXml.h"


using std::exception;
using std::string;
using std::cout;
using std::cerr;
using std::endl;


class CmdLineOpts
{
  public:
    string dFile;
    string dictSdbFileName;
    string dictName;
    string inpFile;
    string ns;
    string schemaPrefix;
    bool verbose;
    vector<string> convOpt;
    string progName;

    CmdLineOpts(int argc, char* argv[]);

    void Usage();
};


int main(int argc, char* argv[])
{
    try
    {
        CmdLineOpts opts(argc, argv);

        if (!opts.dFile.empty())
        {
            if (opts.verbose)
            {
                cout << "Searching " << opts.dFile << " for dictionary \"" <<
                  opts.dictName << "\"" << endl;
            }

            DictObjFile cb(opts.dFile);

            cb.Read();

            DictObjCont& dictObjCont = cb.GetDictObjCont(opts.dictName);

            DictDataInfo dictDataInfo(dictObjCont);

            CifFile* cifFileP = ParseCif(opts.inpFile);

            const string& parsingDiags = cifFileP->GetParsingDiags();

            if (!parsingDiags.empty())
            {
                string::size_type errLoc = parsingDiags.find("ERROR");
                if (errLoc != string::npos)
                {
                    cerr << "Fatal parsing errors in \"" <<
                      cifFileP->GetSrcFileName() << "\": " <<
                      parsingDiags << endl;
                    return (1);
                }

                cout << "Diags for file " << cifFileP->GetSrcFileName() <<
                  "  = " << parsingDiags << endl;
            }

            MmcifToXml mmcifToXml(*cifFileP, dictDataInfo, opts.schemaPrefix);

            string xmlFile;
            MmcifToXml::MakeOutputFileName(xmlFile, opts.inpFile);

            mmcifToXml.Convert(xmlFile, opts.ns, opts.convOpt);
        }
        else
        {
            DicFile* dictFileP = GetDictFile(NULL, string(),
              opts.dictSdbFileName);

            CifDataInfo cifDataInfo(*dictFileP);

            CifFile* cifFileP = ParseCif(opts.inpFile);

            const string& parsingDiags = cifFileP->GetParsingDiags();

            if (!parsingDiags.empty())
            {
                string::size_type errLoc = parsingDiags.find("ERROR");
                if (errLoc != string::npos)
                {
                    cerr << "Fatal parsing errors in \"" <<
                      cifFileP->GetSrcFileName() << "\": " <<
                      parsingDiags << endl;
                    return (1);
                }

                cout << "Diags for file " << cifFileP->GetSrcFileName() <<
                  "  = " << parsingDiags << endl;
            }

            MmcifToXml mmcifToXml(*cifFileP, cifDataInfo, opts.schemaPrefix);

            string xmlFile;
            MmcifToXml::MakeOutputFileName(xmlFile, opts.inpFile);

            mmcifToXml.Convert(xmlFile, opts.ns, opts.convOpt);

            delete (dictFileP);
        }
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
    if (argc < 4)
    {
        Usage();
        throw InvalidOptionsException();
    }

    string funct;
    verbose = false;

    for (unsigned int i = 1; i < (unsigned int)argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-df") == 0)
            {
                i++;
                dFile = argv[i];          
            }
            else if (strcmp(argv[i], "-dictSdbFile") == 0)
            {
                i++;
                dictSdbFileName = argv[i];
            }
            else if (strcmp(argv[i], "-dictName") == 0)
            {
                i++;
                dictName = argv[i];
            }
            else if (strcmp(argv[i], "-funct") == 0)
            {
                i++;
                funct = argv[i];
            }
            else if (strcmp(argv[i], "-f") == 0)
            {
                i++;
                inpFile = argv[i];          
            }
            else if (strcmp(argv[i], "-ns") == 0)
            {
                i++;
	        ns = argv[i];
            }
            else if (strcmp(argv[i], "-prefix") == 0)
            {
                i++;
        	schemaPrefix = argv[i];
            }
            else if (strcmp(argv[i], "-v") == 0)
            {
	        verbose = true;
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

    if (dictName.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }

    if (dFile.empty() && dictSdbFileName.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }

    if (!dFile.empty() && !dictSdbFileName.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }

    if (inpFile.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }

    if (ns.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }

    if (schemaPrefix.empty())
    {
        Usage();
        throw InvalidOptionsException();
    }

    if (funct == "mmcif2xmlaltatom")
        funct = "mmcif2xmlextatom";

    if (!funct.empty())
    {
        funct = funct.substr((string("mmcif2xml")).size());

        if ((!funct.empty()) && (funct != MmcifToXml::NoAtom) &&
         (funct != MmcifToXml::ExtAtom) && (funct != MmcifToXml::All))
        {
            Usage();
            throw InvalidOptionsException();
        }

        if (funct == MmcifToXml::All)
        {
            convOpt.push_back(string());
            convOpt.push_back(MmcifToXml::NoAtom);
            convOpt.push_back(MmcifToXml::ExtAtom);
        }
        else
        {
            convOpt.push_back(funct);
        }
    }
    else
    {
            convOpt.push_back(string());
    }
}


void CmdLineOpts::Usage()
{
    cerr << progName << " usage =  " << endl;
    cerr << "  -dictName <dict_name> " << endl;
    cerr << "  -df <dictionary object file> | -dictSdbFile "\
      "<dict_SDB_file_name>" << endl;
    cerr << "  -df <dictionary object file>  -dictName <dict_name> " << endl;
    cerr << "  -funct <option>" << endl;
    cerr << "    mmcif2xml = translate PDB exchange mmCIF to XML" << endl;
    cerr << "    mmcif2xmlnoatom  = translate PDB exchange mmCIF to XML (excluding atom_site records)" << endl;
    cerr << "    mmcif2xmlaltatom = translate PDB exchange mmCIF to XML (alt atom records only)" << endl;
    cerr << "    mmcif2xmlall     = translate PDB exchange mmCIF to XML (all forms)" << endl;
    cerr << "  -f <input data file>" << endl;
    cerr << "  -ns <name space>" << endl;
    cerr << "  -prefix <schema_prefix_name>" << endl;
    cerr << "  -v (verbose)"  << endl;
}


#ifdef VLAD_DELETED_TIME_MAY_BE_USED
    time_t curr_time;
    time(&curr_time);

    struct tm* tmptr = localtime(&curr_time);

    char timeString[31];
    strftime(timeString, 30, "%Y-%m-%d", tmptr);
#endif

