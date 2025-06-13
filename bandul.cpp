//main
#include <mod/amlmod.h>
#include <mod/logger.h>
//#include <mod/config.h>

MYMODCFG(net.jayzxy.settings, Jtings, 1.0, Jayzxy)

uintptr_t pGTASA;
static constexpr float fMagic = 50.0f / 30.0f;

float *ms_fTimeStep;
struct CVector2D
{
    float x;
    float y;
};

//declarasi
DECL_HOOKv(ControlGunMove, void* self, CVector2D* vec2D);
// Hook untuk ControlGunMove (rifle aiming walk fix)
void HookOf_ControlGunMove(void* self, CVector2D* vec2D)
{
    float save = *ms_fTimeStep;
    *ms_fTimeStep = fMagic;
    ControlGunMove(self, vec2D);
    *ms_fTimeStep = save;
}
/////////////////////////////
extern "C" void OnModLoad()
{
    cfg->Bind("Author", "", "About")->SetString("JayzxyLGC");
    cfg->Bind("IdeasFrom", "", "About")->SetString("JuniorDjjr, XMDS, RusJJ");
    cfg->Bind("Discord", "", "About")->SetString("https://discord.gg/LOGIKA");
    cfg->Save();

    logger->SetTag("JayzSettings");

    pGTASA = aml->GetLib("libGTASA.so");

    //Fix walking while rifle-aiming
    if(cfg->Bind("FixAimingWalkRifle", true, "Gameplay")->GetBool())
    {
        HOOKPLT(ControlGunMove, pGTASA + 0x66F9D0);
    }

    //Fix crosshair position
    if(cfg->Bind("FixCrosshair", true, "Visual")->GetBool())
    {
        //null
    }
}
