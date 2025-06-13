#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <dlfcn.h>

#include <inc/CVector1.cpp>
#include <inc/CTimer1.h>

MYMODCFG(net.fixjiggle.jayz, JayMOD, 1.0, jayzxy)

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Saves     ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static uintptr_t pGTASA = 0;
float *m_f3rdPersonCHairMultX, *m_f3rdPersonCHairMultY;

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Vars      ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
float *ms_fTimeStep;
CPlayerInfo* WorldPlayers;

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Funcs     ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
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
DECL_HOOK(void, DrawCrosshair)
{
    float save1 = *m_f3rdPersonCHairMultX; *m_f3rdPersonCHairMultX = 0.52f;
    float save2 = *m_f3rdPersonCHairMultY; *m_f3rdPersonCHairMultY = 0.435f;

    DrawCrosshair();

    *m_f3rdPersonCHairMultX = save1;
    *m_f3rdPersonCHairMultY = save2;
}
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////     Funcs     ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");

    // Fix walking while rifle-aiming
    if(cfg->Bind("FixAimingWalkRifle", true, "Gameplay")->GetBool())
    {
        HOOKPLT(ControlGunMove, pGTASA + 0x66F9D0);
    }
    if(cfg->Bind("FixCrosshairbyJayz", true "Gameplay")->GetBool())
    {
        aml->Unprot(pGTASA + 0x952CB0, 8);
        SET_TO(m_f3rdPersonCHairMultX, pGTASA + 0x952CB0);
        SET_TO(m_f3rdPersonCHairMultY, pGTASA + 0x952CB4);
        HOOKPLT(DrawCrosshair, pGTASA + 0x672880);

    }
}
