/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a MIT-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#include "movedit_structure.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <cstdint>
#include <cstdlib>
#include "ZenLib/Dir.h"
#include "ZenLib/File.h"
#include "CLI/Help.h"
#include "Common/AdID.h"
#include "Common/mp4_Handler.h"
using namespace std;
using namespace ZenLib;

const size_t BUFFER_SIZE_MAX = 0x10000000;

int main(int argc, char* argv[])
{
    if (argc < 2)
        return Usage();

    bool     simulate=false;
    std::string clef_New=string();
    bool        clef_Delete=false;
    bool        clef_OK=true;
    std::string prof_New=string();
    bool        prof_Delete=false;
    bool        prof_OK=true;
    std::string enof_New=string();
    bool        enof_Delete=false;
    bool        enof_OK=true;
    std::string fiel_New=string();
    bool        fiel_Delete=false;
    bool        fiel_OK=true;
    std::string colr_New=string();
    bool        colr_Delete=false;
    bool        colr_OK=true;
    std::string gama_New=string();
    bool        gama_Delete=false;
    bool        gama_OK=true;
    std::string pasp_New=string();
    bool        pasp_Delete=false;
    bool        pasp_OK=true;
    std::string clap_New=string();
    bool        clap_Delete=false;
    bool        clap_OK=true;
    std::map<size_t, std::string> chan_New;
    std::map<size_t, bool>        chan_OK;
    bool        chan_Delete=false;
    std::string wscale_New=string();
    bool        wscale_OK=true;

    ZtringList FileNames;
    AdID AdID_Content;
    bool AdID_Requested=false;

    // Parse arguments
    for (int argp = 1; argp < argc; ++argp)
    {
        if (Ztring(argv[argp]) == __T("-h")
         || Ztring(argv[argp]) == __T("--help"))
            return Help();
        if (Ztring(argv[argp]) == __T("--help-adid"))
            return Help_AdID();
        if (Ztring(argv[argp]) == __T("--help-tech"))
            return Help_Tech();
        if (Ztring(argv[argp]) == __T("--version"))
            return Version();

        if (argp+1<argc && Ztring(argv[argp]) == __T("--adid"))
        {
            AdID_Requested=true;
            AdID_Content.Set(argv[argp+1]);
            argp++;
        }
        else if (argp+1<argc && Ztring(argv[argp]) == __T("--adid-registry"))
        {
            AdID_Requested=true;
            AdID_Content.SetRegistry(argv[argp+1]);
            argp++;
        }
        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("--clean")
          || Ztring(argv[argp]) == __T("-clean")))
        {

            double w=0, h=0;
            if (sscanf(argv[argp + 1], "%lfx%lf", &w, &h)!=2)
            {
                cout << "Can not understand clean aperture value " << argv[argp] << ", it must be in WxH format" << endl;
                return ReturnValue_ERROR;
            }
            clef_Delete=false;
            clef_New=argv[argp + 1];
            argp++;
        }
        else if ((Ztring(argv[argp]) == __T("--clean-delete")
               || Ztring(argv[argp]) == __T("-clean-delete")))
        {
            clef_New=string();
            clef_Delete=true;
        }

        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("--prod")
          || Ztring(argv[argp]) == __T("-prod")))
        {

            double w=0, h=0;
            if (sscanf(argv[argp + 1], "%lfx%lf", &w, &h)!=2)
            {
                cout << "Can not understand production aperture value " << argv[argp] << ", it must be in WxH format" << endl;
                return ReturnValue_ERROR;
            }
            prof_Delete=false;
            prof_New=argv[argp + 1];
            argp++;
        }
        else if ((Ztring(argv[argp]) == __T("--prod-delete")
               || Ztring(argv[argp]) == __T("-prod-delete")))
        {
            prof_New=string();
            prof_Delete=true;
        }
        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("--pixels")
          || Ztring(argv[argp]) == __T("-pixels")))
        {

            double w=0, h=0;
            if (sscanf(argv[argp + 1], "%lfx%lf", &w, &h)!=2)
            {
                cout << "Can not understand encoded pixels dimension value " << argv[argp] << ", it must be in WxH format" << endl;
                return ReturnValue_ERROR;
            }
            enof_Delete=false;
            enof_New=argv[argp + 1];
            argp++;
        }
        else if ((Ztring(argv[argp]) == __T("--pixels-delete")
               || Ztring(argv[argp]) == __T("-pixels-delete")))
        {
            enof_New=string();
            enof_Delete=true;
        }
        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("--field")
          || Ztring(argv[argp]) == __T("-field")))
        {

            uint16_t f=0, d=0;
            if (sscanf(argv[argp + 1], "%hu,%hu", &f, &d)!=2)
            {
                cout << "Can not understand field handling value " << argv[argp] << ", it must be in fields,detail format" << endl;
                return ReturnValue_ERROR;
            }
            fiel_Delete=false;
            fiel_New=argv[argp + 1];
            argp++;
        }
        else if ((Ztring(argv[argp]) == __T("--field-delete")
               || Ztring(argv[argp]) == __T("-field-delete")))
        {
            fiel_New=string();
            fiel_Delete=true;
        }
        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("--color")
          || Ztring(argv[argp]) == __T("-color")))
        {

            uint16_t p=0, t=0, m=0;
            if (sscanf(argv[argp + 1], "%hu,%hu,%hu", &p, &t, &m)!=3)
            {
                cout << "Can not understand color parameter value " << argv[argp] << ", it must be in primaries,transfer,matrix format" << endl;
                return ReturnValue_ERROR;
            }
            colr_Delete=false;
            colr_New=argv[argp + 1];
            argp++;
        }
        else if ((Ztring(argv[argp]) == __T("--color-delete")
               || Ztring(argv[argp]) == __T("-color-delete")))
        {
            colr_New=string();
            colr_Delete=true;
        }
        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("--gamma")
          || Ztring(argv[argp]) == __T("-gamma")))
        {
            double g=0;
            if (sscanf(argv[argp + 1], "%lf", &g)!=1)
            {
                cout << "Can not understand gamma value " << argv[argp] << ", it must be a real number" << endl;
                return ReturnValue_ERROR;
            }
            gama_Delete=false;
            gama_New=argv[argp + 1];
            argp++;
        }
        else if ((Ztring(argv[argp]) == __T("--gamma-delete")
               || Ztring(argv[argp]) == __T("-gamma-delete")))
        {
            gama_New=string();
            gama_Delete=true;
        }
        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("-p")
          || Ztring(argv[argp]) == __T("-par")
          || Ztring(argv[argp]) == __T("--par")))
        {
            uint32_t h=0, v=0;
            if (sscanf(argv[argp + 1], "%u:%u", &h, &v)!=2)
            {
                cout << "Can not understand PAR value " << argv[argp] << ", it must be a integer:integer value (e.g. '9:10')" << endl;
                return ReturnValue_ERROR;
            }
            pasp_Delete=false;
            pasp_New=argv[argp + 1];
            argp++;
        }
        else if ((Ztring(argv[argp]) == __T("-par-delete")
               || Ztring(argv[argp]) == __T("--par-delete")))
        {
            pasp_New=string();
            pasp_Delete=true;
        }
        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("-aperture")
          || Ztring(argv[argp]) == __T("--aperture")))
        {
            bool Ok=true;
            uint32_t  Val=0;
            istringstream iss(argv[argp + 1]);
            iss >> Val;
            if (!iss.good())
                Ok=false;
            if (iss.peek()==':')
            {
                iss.ignore();
                iss >> Val;
                if (!iss.good())
                    Ok=false;
            }
            if (iss.peek()!=',')
                Ok=false;
            iss.ignore();
            iss >> Val;
            if (!iss.good())
                Ok=false;
            if (iss.peek()==':')
            {
                iss.ignore();
                iss >> Val;
                if (!iss.good())
                    Ok=false;
            }
            if (iss.peek()!=',')
                Ok=false;
            iss.ignore();
            iss >> Val;
            if (!iss.good())
                Ok=false;
            if (iss.peek()==':')
            {
                iss.ignore();
                iss >> Val;
                if (!iss.good())
                    Ok=false;
            }
            if (iss.peek()!=',')
                Ok=false;
            iss.ignore();
            iss >> Val;
            if (iss.fail())
                Ok=false;
            if (iss.peek()==':')
            {
                iss.ignore();
                iss >> Val;
                if (iss.fail())
                    Ok=false;
            }

            if (!Ok)
            {
                cout << "Can not understand clean aperture value " << argv[argp] << ", it must be in with:d,height:d,hOffset:d,vOffset:d format (':d' (denominator) are optionnal')" << endl;
                return ReturnValue_ERROR;
            }

            clap_Delete=false;
            clap_New=argv[argp + 1];
            argp++;
        }
        else if ((Ztring(argv[argp]) == __T("-aperture-delete")
               || Ztring(argv[argp]) == __T("--aperture-delete")))
        {
            clap_New=string();
            clap_Delete=true;
        }

        else if ((Ztring(argv[argp]) == __T("-channels")
               || Ztring(argv[argp]) == __T("--channels")))
        {
            bool Ok=true;
            string chan(argv[argp + 1]);
            

            size_t start=0, end=0, index=0;
            uint32_t code;
            bool _delete;
            do
            {
                end = chan.find(',', start);
                string current=chan.substr(start, end!=string::npos?end-start:string::npos);

                if (size_t equal_pos=current.find('=')!=string::npos)
                {
                    if (!equal_pos || equal_pos+1==current.size())
                    {
                        Ok=false;
                        break;
                    }
                    istringstream iss(current.substr(0, equal_pos));
                    iss >> index;
                    if (iss.fail())
                    {
                        Ok=false;
                        break;
                    }

                    if (!mp4_chan_ChannelCode(current.substr(equal_pos+1, current.size()-equal_pos-1), code, _delete))
                    {
                        Ok=false;
                        break;
                    }

                    chan_New[index++]=current.substr(equal_pos+1, current.size()-equal_pos-1);
                }
                else
                {
                    if (!mp4_chan_ChannelCode(current, code, _delete))
                    {
                        Ok=false;
                        break;
                    }

                    chan_New[index++]=current;
                }
            }
            while((start=chan.find_first_not_of(',', end))!=string::npos);

            if (!Ok)
            {
                cout << "Can not understand channels label value " << argv[argp] << ", it must be in [track=]code[,[track=]code...] format" << endl;
                return ReturnValue_ERROR;
            }

            chan_Delete=false;
            argp++;
        }

        else if ((Ztring(argv[argp]) == __T("-channels-delete")
               || Ztring(argv[argp]) == __T("--channels-delete")))
        {
            chan_New.clear();
            chan_Delete=true;
        }
        else if (argp + 1 < argc
         && (Ztring(argv[argp]) == __T("--width-scale")
          || Ztring(argv[argp]) == __T("-width-scale")
          || Ztring(argv[argp]) == __T("-w")))
        {
            double w=0;
            if (sscanf(argv[argp + 1], "%lf", &w)!=1)
            {
                cout << "Can not understand width scale value " << argv[argp] << ", it must be a real number" << endl;
                return ReturnValue_ERROR;
            }
            wscale_New=argv[argp + 1];
            argp++;
        }
        else if (Ztring(argv[argp]) == __T("-s")
              || Ztring(argv[argp]) == __T("--simulate"))
        {
            simulate = true;
        }
        else
        {
            FileNames.push_back(argv[argp]);
        }
    }

    if ((!clef_New.empty() || clef_Delete ||
         !prof_New.empty() || prof_Delete ||
         !enof_New.empty() || enof_Delete ||
         !fiel_New.empty() || fiel_Delete ||
         !gama_New.empty() || gama_Delete ||
         !pasp_New.empty() || pasp_Delete ||
         !colr_New.empty() || colr_Delete ||
         !clap_New.empty() || clap_Delete ||
         !chan_New.empty() || chan_Delete ||
         !wscale_New.empty()) && AdID_Requested)
    {
        cout << "Core options and Ad-ID options can not currently be used together" << endl;
        return ReturnValue_ERROR;
    }

    if (FileNames.empty())
        return Usage();

    ReturnValue ToReturn=ReturnValue_OK;

    ZtringList List;
    for (size_t i=0; i<FileNames.size(); i++)
    {
        size_t List_PreviousSize=List.size();
        List+=Dir::GetAllFileNames(FileNames[i]);
        if (List.size()==List_PreviousSize)
            List.push_back(FileNames[i]);
    }
    std::vector<Structure*> Structures;
    for (size_t i = 0; i < List.size(); i++)
    {
        Ztring& Item=List[i];
        cout << "Parsing " << Item.To_Local() << std::endl;
        //if (AdID_Requested)
        {
            mp4_Handler* H=new mp4_Handler;
            if (!H->Open(Item.To_Local()))
            {
                cout << " Can not open file: " << H->Errors.str() << endl;
                ToReturn = ReturnValue_ERROR;
            }

            Structures.push_back((Structure*)H); //Hack for storing mp4_Handler

            continue;
        }

        File F;
        if (F.Open(Item, File::Access_Read))
        {

            Structure* S = new Structure(&F, Item);
            S->Parse();
            Structures.push_back(S);

            F.Close();
        }
        else
        {
            cout << " Can not open file " << endl;
            List.erase(List.begin()+i);
            i--;
            ToReturn = ReturnValue_ERROR;
        }
    }

    // Removing common data in file name
    if (!List.empty())
    {
        size_t Max = (size_t)-1;
        for (ZtringList::iterator Item = List.begin(); Item != List.end(); Item++)
        {
            if (Item->empty())
            {
                Max = (size_t)-1;
                break; // Problem
            }
            if (Max > Item->size())
                Max = Item->size() - 1;
            for (size_t Pos = 0; Pos < Max; ++Pos)
            {
                if ((*Item)[Pos] != List[0][Pos])
                {
                    Max = Pos;
                    break;
                }
            }
        }
        if (Max && Max != (size_t)-1)
        {
            if (List[0].find(PathSeparator, Max) == string::npos)
            {
                do
                {
                    --Max;
                }
                while (Max && List[0][Max] != PathSeparator);
                ++Max;
            }

            for (ZtringList::iterator Item = List.begin(); Item != List.end(); Item++)
                Item->erase(0, Max);
        }
    }
    size_t Max = 0;
    for (ZtringList::iterator Item = List.begin(); Item != List.end(); Item++)
        if (Max < Item->size())
            Max = Item->size();
    string FileNameFake;
    FileNameFake.resize(Max, __T(' '));

    if (!Structures.empty())
    {
    cout << endl;
    if (!AdID_Requested)
    {
    cout << "Summary:" << endl;
    cout << "OK = file is correctly detected and does not need to be modified ('Yes') or is" << endl;
    cout << "  correctly detected and need to be modified ('Mod') or is correctly detected" << endl;
    cout << "  and need to be modified but not supported ('No') or there was a problem with" << endl;
    cout << "  it (empty)" << endl;
    cout << "M = The field will be modified ('Y') or should be modified but it is not possible" << endl;
    cout << "  due to feature not implemented ('N')" << endl;
    cout << FileNameFake << "|OK?|Clean Ap.|M| Prod Ap.|M| Enc. Ap.|M|      PAR|M|w-scale|M|Field|M|   Color|M|Gamma|M|                 Aperture|M|                 Channels|M|" << endl;
    }
    else
        cout << FileNameFake << "|OK?| Registry|UniversalAdId value" << endl;
    ZtringList::iterator ItemName = List.begin();
    for (std::vector<Structure*>::iterator Item = Structures.begin(); Item != Structures.end(); Item++)
    {
        // Name
        string Name = ItemName->To_Local();
        Name.resize(Max, __T(' '));

        if (AdID_Requested)
        {
            cout << Name << '|';

            mp4_Handler* H = (mp4_Handler*)(*Item); //Hack for storing mp4_Handler

            AdID AdID_Content_Temp=AdID_Content;
            AdID_Content_Temp.SetName(ItemName->To_Local());
            bool OK;
            if (H->Errors.str().empty())
                OK = AdID_Content_Temp.Validate();
            else
                OK = false;
            if (OK)
            {
                if (!H->Errors.str().empty())
                    OK = false;
            }
            if (OK)
            {
                H->Set("com.universaladid.idregistry", AdID_Content_Temp.GetRegistry().c_str());
                H->Set("com.universaladid.idvalue", AdID_Content_Temp.Get().c_str());

                if (!simulate)
                    H->Save();

                if (!H->PerFile_Error.str().empty())
                    OK = false;
            }

            cout<< (OK?"Yes":" No") << '|';

            string Registry;
            if (OK)
                Registry = AdID_Content_Temp.GetRegistry();
            else if (!AdID_Content_Temp.ErrorMessage.empty() || !H->PerFile_Error.str().empty())
                Registry = H->Get("com.universaladid.idregistry"); // showing it if already present in the file
            if (Registry.size() < 9)
                Registry.insert(0, 9 - Registry.size(), ' ');

            cout << Registry << '|';

            if (!AdID_Content_Temp.ErrorMessage.empty())
            {
                cout << AdID_Content_Temp.ErrorMessage << endl;
                ToReturn = ReturnValue_ERROR;

                ItemName++;
                continue;
            }

            if (!H->PerFile_Error.str().empty())
            {
                string Value = H->Get("com.universaladid.idvalue"); // showing it if already present in the file
                if (!Value.empty())
                    cout << Value << ", ";

                string Error=H->PerFile_Error.str();
                size_t End = Error.find_last_not_of(" \r\n");
                if (End != string::npos)
                    Error.resize(End+1);
                
                cout << Error << endl;
                ToReturn = ReturnValue_ERROR;

                ItemName++;
                continue;
            }

            cout << AdID_Content_Temp.Get() << endl;

            ItemName++;
            continue;
        }
        else
        {
            cout << Name << '|';

            mp4_Handler* H = (mp4_Handler*)(*Item); //Hack for storing mp4_Handler

            bool OK;
            if (H->Errors.str().empty())
                OK = true; //TODO: validate input;
            else
                OK = false;

            if (OK)
            {
                if (!H->Errors.str().empty())
                    OK = false;
            }
            if (OK)
            {
                if (!clef_New.empty())
                {
                    if(!H->Set("clef", clef_New))
                    {
                        clef_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (clef_Delete)
                    H->Remove("clef");

                if (!prof_New.empty())
                {
                    if(!H->Set("prof", prof_New))
                    {
                        prof_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (prof_Delete)
                    H->Remove("prof");

                if (!enof_New.empty())
                {
                    if(!H->Set("enof", enof_New))
                    {
                        enof_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (enof_Delete)
                    H->Remove("enof");

                if (!pasp_New.empty())
                {
                    if(!H->Set("pasp", pasp_New))
                    {
                        pasp_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (pasp_Delete)
                    H->Remove("pasp");

                if (!wscale_New.empty())
                {
                    if(!H->Set("wscale", wscale_New))
                    {
                        wscale_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }

                if (!fiel_New.empty())
                {
                    if(!H->Set("fiel", fiel_New))
                    {
                        fiel_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (fiel_Delete)
                    H->Remove("fiel");

                if (!colr_New.empty())
                {
                    if(!H->Set("colr", colr_New))
                    {
                        colr_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (colr_Delete)
                    H->Remove("colr");

                if (!gama_New.empty())
                {
                    if(!H->Set("gama", gama_New))
                    {
                        gama_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (gama_Delete)
                    H->Remove("gama");
                if (!clap_New.empty())
                {
                    if(!H->Set("clap", clap_New))
                    {
                        clap_OK=false;
                        ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (clap_Delete)
                    H->Remove("clap");
                if (!chan_New.empty())
                {
                    for (map<size_t, string>::iterator It=chan_New.begin(); It!=chan_New.end(); It++)
                    {
                        stringstream ss; ss << It->first << "=" << It->second;
                        chan_OK[It->first]=H->Set("chan", ss.str());
                        if (!chan_OK[It->first])
                            ToReturn=ReturnValue_ERROR;
                    }
                }
                else if (chan_Delete)
                    H->Remove("chan");

                if (!simulate)
                    H->Save();

                if (!H->PerFile_Error.str().empty())
                    OK = false;
            }

            cout << (OK?"Yes":"No ") << '|';

            string clef = H->Get("clef");
            if (clef.size() < 9)
               clef.insert(0, 9 - clef.size(), ' ');
             cout << clef << "|" << ((!clef_New.empty() || clef_Delete) ? ((OK && clef_OK) ? "Y" : "N") : " ") << "|";

            string prof = H->Get("prof");
            if (prof.size() < 9)
               prof.insert(0, 9 - prof.size(), ' ');
             cout << prof << "|" << ((!prof_New.empty() || prof_Delete) ? ((OK && prof_OK) ? "Y" : "N") : " ") << "|";

            string enof = H->Get("enof");
            if (enof.size() < 9)
               enof.insert(0, 9 - enof.size(), ' ');
             cout << enof << "|" << ((!enof_New.empty() || enof_Delete) ? ((OK && enof_OK) ? "Y" : "N") : " ") << "|";

            string pasp = H->Get("pasp");
            if (pasp.size() < 9)
               pasp.insert(0, 9 - pasp.size(), ' ');
             cout << pasp << "|" << ((!pasp_New.empty() || pasp_Delete) ? ((OK && pasp_OK) ? "Y" : "N") : " ") << "|";

            string wscale = H->Get("wscale");
            if (wscale.size() < 7)
               wscale.insert(0, 7 - wscale.size(), ' ');
             cout << wscale << "|" << (!wscale_New.empty() ? ((OK && wscale_OK) ? "Y" : "N") : " ") << "|";

            string fiel = H->Get("fiel");
            if (fiel.size() < 5)
               fiel.insert(0, 5 - fiel.size(), ' ');
             cout << fiel << "|" << ((!fiel_New.empty() || fiel_Delete) ? ((OK && fiel_OK) ? "Y" : "N") : " ") << "|";

            string colr = H->Get("colr");
            if (colr.size() < 8)
               colr.insert(0, 8 - colr.size(), ' ');
             cout << colr << "|" << ((!colr_New.empty() || colr_Delete) ? ((OK && colr_OK) ? "Y" : "N") : " ") << "|";

            string gama = H->Get("gama");
            if (gama.size() < 5)
               gama.insert(0, 5 - gama.size(), ' ');
             cout << gama << "|" << ((!gama_New.empty() || gama_Delete) ? ((OK && gama_OK) ? "Y" : "N") : " ") << "|";

            string clap = H->Get("clap");
            if (clap.size() < 25)
               clap.insert(0, 25 - clap.size(), ' ');
             cout << clap << "|" << ((!clap_New.empty() || clap_Delete) ? ((OK && clap_OK) ? "Y" : "N") : " ") << "|";

            string chan = H->Get("chan");
            {
                size_t start=0, end=0;
                do
                {
                    end = chan.find(',', start);
                    string current=chan.substr(start, end - start);

                    size_t track=0;
                    istringstream(current) >> track;
                    if (current.size() < 25)
                        current.insert(0, 25 - current.size(), ' ');
                    if (start!=0)
                        cout << endl << string(FileNameFake.size() + 117, ' ') << '|';
                    cout << current << "|" << ((chan_New.find(track)!=chan_New.end() || chan_Delete) ? ((OK && chan_OK.find(track)!=chan_OK.end() && chan_OK[track]) ? "Y" : "N") : " ") << "|";
                }
                while((start=chan.find_first_not_of(',', end))!=std::string::npos);
            }

            cout << endl;
            ItemName++;
            continue;
        }

    }
    } //!Structures.empty()

    for (std::vector<Structure*>::iterator Item = Structures.begin(); Item != Structures.end(); Item++)
    {
            mp4_Handler* H = (mp4_Handler*)(*Item); //Hack for storing mp4_Handler
            if (!H->PerFile_Error.str().empty())
            {
                cout << H->PerFile_Error.str() << endl;
                ToReturn = ReturnValue_ERROR;
            }
    }

    //Cleanup
    for (std::vector<Structure*>::iterator Item = Structures.begin(); Item != Structures.end(); Item++)
    {
        //if (AdID_Requested)
        {
            mp4_Handler* H = (mp4_Handler*)(*Item); //Hack for storing mp4_Handler
            delete H;
        }
        //else
        //    delete *Item;
    }

    return ToReturn;
}

