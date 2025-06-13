#pragma once

struct CVector { float x, y, z; };
struct CVector2D { float x, y; };

enum RwRenderState
{
    rwRENDERSTATENARENDERSTATE = 0,
    rwRENDERSTATETEXTUREADDRESS = 1,
    // dst...
};

enum eEntityType
{
    ENTITY_TYPE_PED = 1,
    // dst...
};

enum ePedType
{
    PED_TYPE_PLAYER1 = 0,
    // dst...
};

struct CPhysical
{
    void* vtbl;
    uint8_t _pad[20];
    uint8_t m_nType;
};

struct CPed : public CPhysical
{
    uint8_t _padPed[60];
    uint8_t m_nPedType;
};

typedef int ScriptHandle;

enum ePickupType
{
    PICKUP_ONCE = 1,
    // dst...
};

struct CPlayerInfo { /* isikan jika dibutuhkan */ };
