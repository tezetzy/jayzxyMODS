#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <dlfcn.h>

// Ambil definisi dari header buatan/mod (gunakan struktur yang kamu miliki atau saya bisa bantu buatkan)
#include "inc/private.h" // Kamu perlu buat header ini nanti

MYMODCFG(net.jayzxy.jaypatch, JPatch, 1.0, Jayzxy)

uintptr_t pGTASA;
void* hGTASA;

float* ms_fTimeStep;
CPlayerInfo* WorldPlayers;
static constexpr float fMagic = 50.0f / 30.0f;

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

void (*_rwOpenGLSetRenderState)(RwRenderState, int);
void (*_rwOpenGLGetRenderState)(RwRenderState, void*);
void (*ClearPedWeapons)(CPed*);

float f = 0.5f;
asm volatile("VMOV.F32 S0, %0" :: "r"(f));

DECL_HOOKv(ControlGunMove, void* self, CVector2D* vec2D)
{
    float save = *ms_fTimeStep;
    *ms_fTimeStep = fMagic;
    ControlGunMove(self, vec2D);
    *ms_fTimeStep = save;
}

DECL_HOOKv(ProcessBuoyancy, void* self, CPhysical* phy, float unk, CVector* vec1, CVector* vec2)
{
    if (phy->m_nType == ENTITY_TYPE_PED && ((CPed*)phy)->m_nPedType == PED_TYPE_PLAYER1)
    {
        float save = *ms_fTimeStep;
        *ms_fTimeStep = (1.5f + ((save / fMagic) / 1.9f)) * (save / fMagic);
        ProcessBuoyancy(self, phy, unk, vec1, vec2);
        *ms_fTimeStep = save;
        return;
    }
    ProcessBuoyancy(self, phy, unk, vec1, vec2);
}

DECL_HOOK(ScriptHandle, GenerateNewPickup, float x, float y, float z, int16_t modelId, ePickupType pickupType, int ammo, int16_t moneyPerDay, bool isEmpty, const char* msg)
{
    if (modelId == 1277 && x == 1263.05f && y == -773.67f && z == 1091.39f)
    {
        return GenerateNewPickup(1291.2f, -798.0f, 1089.39f, modelId, pickupType, ammo, moneyPerDay, isEmpty, msg);
    }
    return GenerateNewPickup(x, y, z, modelId, pickupType, ammo, moneyPerDay, isEmpty, msg);
}

extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");
    hGTASA = dlopen("libGTASA.so", RTLD_LAZY);

    // Fungsi
    SET_TO(_rwOpenGLSetRenderState, aml->GetSym(hGTASA, "_Z23_rwOpenGLSetRenderState13RwRenderStatePv"));
    SET_TO(_rwOpenGLGetRenderState, aml->GetSym(hGTASA, "_Z23_rwOpenGLGetRenderState13RwRenderStatePv"));
    SET_TO(ClearPedWeapons, aml->GetSym(hGTASA, "_ZN4CPed12ClearWeaponsEv"));

    // Variabel
    ms_fTimeStep = (float*)(pGTASA + 0x96B4F8);
    SET_TO(WorldPlayers, aml->GetSym(hGTASA, "_ZN6CWorld7PlayersE"));

    // Perbaikan konfigurasi
    if(cfg->Bind("FixVertexWeight", true, "Visual")->GetBool())
    {
        aml->Write(pGTASA + 0x1C8064, (uintptr_t)"\x01", 1);
        aml->Write(pGTASA + 0x1C8082, (uintptr_t)"\x01", 1);
    }

    if(cfg->Bind("FixAimingWalkRifle", true, "Gameplay")->GetBool())
    {
        HOOKPLT(ControlGunMove, pGTASA + 0x67F5AC);
    }

    if(cfg->Bind("BuoyancySpeedFix", true, "Gameplay")->GetBool())
    {
        HOOKPLT(ProcessBuoyancy, pGTASA + 0x67130C);
    }
}
