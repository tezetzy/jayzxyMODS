#include <mod/amlmod.h>
//#include <mod/config.h>
#include "inc/private.h"

MYMOD(net.jayzxy.gtasa.abtfixes, GTA:SA ABT Fixes, 1.0, Jayzxy)
NEEDGAME(com.rockstargames.gtasa)

/* Saves */
static uintptr_t pGTASA = 0;
float *m_f3rdPersonCHairMultX, *m_f3rdPersonCHairMultY;
float* ms_fTimeStep;
static constexpr float fMagic = 50.0f / 30.0f;

DECL_HOOK(void, DrawCrosshair)
{
    float save1 = *m_f3rdPersonCHairMultX; *m_f3rdPersonCHairMultX = 0.52f;
    float save2 = *m_f3rdPersonCHairMultY; *m_f3rdPersonCHairMultY = 0.435f;

    DrawCrosshair();

    *m_f3rdPersonCHairMultX = save1;
    *m_f3rdPersonCHairMultY = save2;
}
DECL_HOOKv(ControlGunMove, void* self, CVector2D* vec2D)
{
    float save = *ms_fTimeStep;
    *ms_fTimeStep = fMagic;
    ControlGunMove(self, vec2D);
    *ms_fTimeStep = save;
}
extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");

    aml->Unprot(pGTASA + 0x952CB0, 8);
    SET_TO(m_f3rdPersonCHairMultX, pGTASA + 0x952CB0);
    SET_TO(m_f3rdPersonCHairMultY, pGTASA + 0x952CB4);
    HOOKPLT(DrawCrosshair, pGTASA + 0x672880);
    SET_TO(ms_fTimeStep, aml->GetSym(hGTASA, "_ZN6CTimer12ms_fTimeStepE"));
    HOOKPLT(ControlGunMove, pGTASA + 0x66F9D0);
}
