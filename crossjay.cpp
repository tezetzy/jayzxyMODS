#include <mod/amlmod.h>

MYMOD(net.jayzxy.gtasa.mirafix, GTA:SA Crosshair Fix, 1.0, Jayzxy)
NEEDGAME(ro.alyn_sampmobile.game)

/* Saves */
static uintptr_t pGTASA = 0;
float *m_f3rdPersonCHairMultX, *m_f3rdPersonCHairMultY;

DECL_HOOK(void, DrawCrosshair)
{
    float save1 = *m_f3rdPersonCHairMultX; *m_f3rdPersonCHairMultX = 0.530f;
    float save2 = *m_f3rdPersonCHairMultY; *m_f3rdPersonCHairMultY = 0.530f;

    DrawCrosshair();

    *m_f3rdPersonCHairMultX = save1;
    *m_f3rdPersonCHairMultY = save2;
}

extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");

    aml->Unprot(pGTASA + 0x952CB0, 8);
    SET_TO(m_f3rdPersonCHairMultX, pGTASA + 0x952CB0);
    SET_TO(m_f3rdPersonCHairMultY, pGTASA + 0x952CB4);
    HOOKPLT(DrawCrosshair, pGTASA + 0x672880);
}
