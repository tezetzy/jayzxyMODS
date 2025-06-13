//main
#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <dlfcn.h>

MYMODCFG(net.jayzxy.settings, Jtings, 1.0, Jayzxy)

uintptr_t pGTASA;
void* hGTASA;
static constexpr float fMagic = 50.0f / 30.0f;

float *ms_fTimeStep;

struct CVector2D {
    float x, y;
};

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
    hGTASA = dlopen("libGTASA.so", RTLD_LAZY);

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
