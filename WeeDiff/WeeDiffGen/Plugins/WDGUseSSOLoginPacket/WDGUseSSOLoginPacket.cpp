// -----------------------------------------------------------------
// WDGUseSSOLoginPacket
// (c) 2012 Ai4rei/AN
//
// This work is licensed under a
// Creative Commons BY-NC-SA 3.0 Unported License
// http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// -----------------------------------------------------------------

#include "WDGUseSSOLoginPacket.h"

#ifndef _ARRAYSIZE
    #define _ARRAYSIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif  /* _ARRAYSIZE */

static /* const */ WDGPLUGININFO l_PluginInfo =
{
    _T("Use SSO Login Packet"),
    _T("Makes the client use login packet 0x825 on all langtypes."),
    _T("[Packet]"),
    _T(""),
    _T("Ai4rei/AN"),
    1,
    0,
    { 0x6161b1e8, 0xea19, 0x11e1, { 0xa7, 0x78, 0x0, 0x22, 0x15, 0x64, 0x8a, 0x98 } },  /* guid */
    _T(""),
};

static WDGPlugin* l_lpSelfReference = NULL;

void WDGPlugin::Release(void)
{
    this->m_DiffData.clear();
    l_lpSelfReference = NULL;
    delete this;
}

void WDGPlugin::Free(LPVOID lpBuf)
{
    delete lpBuf;
}

LPWDGPLUGININFO WDGPlugin::GetPluginInfo(void)
{
    return &l_PluginInfo;
}

INT32 WDGPlugin::Enabled(void)
{
    return 0;
}

INT32 WDGPlugin::Disabled(void)
{
    return 0;
}

LPCTSTR WDGPlugin::GetInputValue(void)
{
    return NULL;
}

DiffData* WDGPlugin::GeneratePatch(void)
{
    FINDDATA Fd;
    UINT32 uOffset, uPart;

    this->m_DiffData.clear();

    if(!this->IsSane())
    {
        return NULL;
    }

    try
    {
        uPart = 1;

        Fd.uMask = WFD_PATTERN|WFD_WILDCARD|WFD_SECTION;
        Fd.lpData =
            /*00*/"A1 '????'"    // MOV     EAX,DWORD PTR DS:[&g_serviceType]
            /*05*/"85C0"         // TEST    EAX,EAX
            /*07*/"0F84 '????'"  // JE      ADDR v
            /*0E*/"83F8 12"      // CMP     EAX,12h
            /*11*/"0F84 '????'"  // JE      ADDR v
            /*17*/"83F8 0C"      // CMP     EAX,0Ch
            /*1A*/"0F84 '????'"  // JE      ADDR v
            /*20*/;
        Fd.chWildCard = '?';
        Fd.lpszSection = ".text";

        uOffset = this->m_dgc->Match(&Fd)+0x07;

        this->SetByte(uOffset++, 0x90);  // JE -> NOP
        this->SetByte(uOffset++, 0xE9);  // JE -> JMP
    }
    catch(const char* lpszThrown)
    {
        char szErrMsg[1024];

        wsprintfA(szErrMsg, __FILE__" :: Part %u :: %s", uPart, lpszThrown);
        this->m_dgc->LogMsg(szErrMsg);
    }

    return this->m_DiffData.empty() ? NULL : &this->m_DiffData;
}

DiffData* WDGPlugin::GetDiffData(void)
{
    return this->m_DiffData.empty() ? NULL : &this->m_DiffData;
}

bool WDGPlugin::IsVC9Image(void)
{
    try
    {
        this->m_dgc->FindFunction("_encode_pointer");  // msvcr90.dll
    }
    catch(const char* lpszThrown)
    {
        return false;

        // unused
        (void)lpszThrown;
    }

    return true;
}

bool WDGPlugin::IsSane(void)
{// ensure that it actually is a valid RO client
#define ISSANEMAGIC "gravity"
    FINDDATA Fd;

    Fd.uMask       = WFD_SECTION;
    Fd.lpData      = ISSANEMAGIC;
    Fd.uDataSize   = sizeof(ISSANEMAGIC);
    Fd.lpszSection = this->IsVC9Image() ? ".rdata" : ".data";

    try
    {
        this->m_dgc->Match(&Fd);
    }
    catch(const char* lpszThrown)
    {
        this->m_dgc->LogMsg(__FILE__" :: Image is not sane.");
        return false;

        // unused
        (void)lpszThrown;
    }

    return true;
#undef ISSANEMAGIC
}

void WDGPlugin::SetByte(UINT32 uOffset, UCHAR uValue)
{
    DIFFDATA Diff = { uOffset, uValue };

    this->m_DiffData.push_back(Diff);
}

extern "C" __declspec(dllexport) WeeDiffGenPlugin::IWDGPlugin* InitPlugin(LPVOID lpData, USHORT huWeeDiffMajorVersion, USHORT huWeeDiffMinorVersion)
{
    if(l_lpSelfReference)
    {// double initialization
        DebugBreak();
    }
    else
    {
        l_lpSelfReference = new WDGPlugin(lpData);
    }

    return l_lpSelfReference;
}
