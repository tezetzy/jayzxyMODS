#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <dlfcn.h>

MYMODCFG(net.jayzxy.settings, Jtings, 1.0, Jayzxy)

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Saves     ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
uintptr_t pGTASA;
void* hGTASA;
static constexpr float fMagic = 50.0f / 30.0f;

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Vars      ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
float *ms_fTimeStep, *m_f3rdPersonCHairMultX, *m_f3rdPersonCHairMultY, *ms_fAspectRatio;
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Funcs     ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
struct CVector2D {
    float x, y;
};
////
void Redirect(uintptr_t addr, uintptr_t to)
{
    if(!addr) return;
    if(addr & 1)
    {
        addr &= ~1;
        if (addr & 2)
        {
            aml->PlaceNOP(addr, 1);
            addr += 2;
        }
        uint32_t hook[2];
        hook[0] = 0xF000F8DF;
        hook[1] = to;
        aml->Write(addr, (uintptr_t)hook, sizeof(hook));
    }
    else
    {
        uint32_t hook[2];
        hook[0] = 0xE51FF004;
        hook[1] = to;
        aml->Write(addr, (uintptr_t)hook, sizeof(hook));
    }
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Hooks     ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
extern "C" void adadad(void)
DECL_HOOKv(ControlGunMove, void* self, CVector2D* vec2D) // AimingRifleWalkFix
{
    float save = *ms_fTimeStep; *ms_fTimeStep = fMagic;
    ControlGunMove(self, vec2D);
    *ms_fTimeStep = save;
}
static constexpr float ar43 = 4.0f/3.0f;
DECL_HOOKv(DrawCrosshair)
{
    float save1 = *m_f3rdPersonCHairMultX; *m_f3rdPersonCHairMultX = 0.530f - (*ms_fAspectRatio - ar43) * 0.01125f;
    float save2 = *m_f3rdPersonCHairMultY; *m_f3rdPersonCHairMultY = 0.400f + (*ms_fAspectRatio - ar43) * 0.03600f;
    DrawCrosshair();
    *m_f3rdPersonCHairMultX = save1; *m_f3rdPersonCHairMultY = save2;
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Funcs     ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
extern "C" void OnModLoad()
{
    cfg->Bind("Author", "", "About")->SetString("JayzxyLGC");
    cfg->Bind("IdeasFrom", "", "About")->SetString("JuniorDjjr, XMDS");
    cfg->Bind("Discord", "", "About")->SetString("https://discord.gg/LOGIKA");
    cfg->Save();

    logger->SetTag("JayzSettings");
    pGTASA = aml->GetLib("libGTASA.so");
    hGTASA = dlopen("libGTASA.so", RTLD_LAZY);

    SET_TO(m_f3rdPersonCHairMultX,  aml->GetSym(hGTASA, "_ZN7CCamera22m_f3rdPersonCHairMultXE"));
    SET_TO(m_f3rdPersonCHairMultY,  aml->GetSym(hGTASA, "_ZN7CCamera22m_f3rdPersonCHairMultYE"));
    SET_TO(ms_fAspectRatio,  aml->GetSym(hGTASA, "_ZN5CDraw15ms_fAspectRatioE"));

    //Fix walking while rifle-aiming
    if(cfg->Bind("FixAimingWalkRifle", true, "Gameplay")->GetBool())
    {
        HOOKPLT(ControlGunMove, pGTASA + 0x66F9D0);
    }
    //fixing a crosshair position by very stupid math
    if(cfg->Bind("FixCrosshair", true, "Visual")->GetBool())
    {
        HOOKPLT(DrawCrosshair, pGTASA + 0x672880);
    }
}
