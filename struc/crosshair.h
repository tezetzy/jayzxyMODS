enum eWeaponType
{
    WEAPON_UNARMED = 0,
    WEAPON_COUNTRYRIFLE = 33,
    // Tambahkan jenis senjata lain jika perlu
};

struct CWeapon
{
    int m_eWeaponType;
    // tambahkan jika perlu
};

struct CPlayerPed
{
    char pad[0x5A0];                         // offset ke slot senjata (perkirakan offset sesuai versi)
    CWeapon m_WeaponSlots[13];               // 13 slot senjata
    int m_nCurrentWeapon;                    // indeks slot aktif
};
