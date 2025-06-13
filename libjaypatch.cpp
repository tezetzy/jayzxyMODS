#include <mod/amlmod.h>

// Ambil definisi dari header buatan/mod (gunakan struktur yang kamu miliki atau saya bisa bantu buatkan)
#include "inc/private.h" // Kamu perlu buat header ini nanti

MYMODCFG(net.jayzxy.jiggle, jiggle, 1.0, Jayzxy)

uintptr_t pGTASA;
void* hGTASA;

float* ms_fTimeStep;
static constexpr float fMagic = 50.0f / 30.0f;
CPlayerInfo* WorldPlayers;


DECL_HOOKv(ControlGunMove, void* self, CVector2D* vec2D)
{
    save = *ms_fTimeStep; *ms_fTimeStep = fMagic;
    ControlGunMove(self, vec2D);
    *ms_fTimeStep = save;
}

extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");
    hGTASA = dlopen("libGTASA.so", RTLD_LAZY);

    SET_TO(ms_fTimeStep, aml->GetSym(hGTASA, "_ZN6CTimer12ms_fTimeStepE"));
    HOOKPLT(ControlGunMove, pGTASA + 0x66F9D0);
}
