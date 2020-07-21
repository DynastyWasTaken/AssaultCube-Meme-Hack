#include <windows.h>
#include <iostream>
#include <Tlhelp32.h>
#include "mem.h"
#include "proc.h"

struct Vector3 { float x, y, z; };

//RECLASS GENERATED CODE!

class ent
{
public:
    Vector3 head; //0x0004
    char pad_0010[36]; //0x0010
    Vector3 body; //0x0034
    Vector3 angle; //0x0040
    char pad_004C[172]; //0x004C
    int32_t health; //0x00F8
    char pad_00FC[297]; //0x00FC
    char name[16]; //0x0225
    char pad_0235[319]; //0x0235
    class weapon* currentWeapon; //0x0374

    virtual void Function0(); //fuck this
    virtual void Function1();
    virtual void Function2();
    virtual void Function3();
    virtual void Function4();
    virtual void Function5();
    virtual void Function6();
    virtual void Function7();
    virtual void Function8();
    virtual void Function9();

}; //Size: 0x0378
static_assert(sizeof(ent) == 0x378);

class weapon
{
public:
    char pad_0000[20]; //0x0000
    class ammoInClip* pAmmo; //0x0014
}; //Size: 0x0018
static_assert(sizeof(weapon) == 0x18);

class ammoInClip
{
public:
    int32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof(ammoInClip) == 0x4);

//im going insane

DWORD WINAPI hackThread(HMODULE hModule)
{
    /*Pseudocode
    -add console x
    -getmodulebase x
    -loop x
    -key input x
    -continuos write/freeze x
    -cleanup and inject x
    */

    //opening console
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);

    std::cout << "Iridium - internal meme trainer\n";
    
    std::cout << " F1: Health hack\n F2: Ammo Hack\n F3: NoRecoil\n F5: Team Switch\n F9: Nuker\n HOME: info\n INSERT: uninject" << std::endl;

    //defining a lot of shit
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

    uintptr_t pLocalPlayer = (uintptr_t)(moduleBase + 0x10f4f4);

    ent* localPlayer = *(ent**)(moduleBase + 0x10f4f4);

    uintptr_t pEntity = (uintptr_t)(moduleBase + 0x10f4f8);

    //not needed until i rework the entity structure
    //EnemyEnt* Enemy = *(EnemyEnt**)(moduleBase + 0x10f4f8);

    uintptr_t pMyTeam = mem::FindDMAAddy(moduleBase + 0x10f4f4, { 0x32C });

    uintptr_t entNum = (uintptr_t)(moduleBase + 0x110d98);

    bool    bHealth                 = false,
            bAmmo                   = false,
            bRecoil                 = false,
            bShowAmountOfPlayers    = false,
            bNuke                   = false,
            bTpTy                   = false;

    uintptr_t entity = mem::FindDMAAddy(pEntity, { 0x4 });

    uintptr_t pFrags;

    pFrags = mem::FindDMAAddy(pLocalPlayer, { 0x1FC });

    //main loop
    while (true)
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;
        }

        //switch teams
        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {

            //pls dont bully me for this shit code
            if (*(int*)pMyTeam == 1)
            {
                *(int*)pMyTeam = 0;
                //std::cout << "switched teams! \nCurrent team: " << *(int*)pMyTeam << "\n";
            }
            else
            {
                *(int*)pMyTeam = 1;
                //std::cout << "switched teams! \nCurrent team: " << *(int*)pMyTeam << "\n";
            }
        }

        //set frags to 1337
        if (GetAsyncKeyState(VK_NUMPAD6) & 1)
        {
            *(int*)pFrags = 1337;
        }

        //info. Usually only for debugging
        if (GetAsyncKeyState(VK_HOME) & 1)
        {
            int* entCount = (int*)entNum;

            if (entCount)
                std::cout << "players ingame - " << *entCount + 1 << "\n";

            std::cout << "current team - " << *(int*)pMyTeam << "\n";

            std::cout << "Frags - " << *(int*)pFrags << "\n";

            std::cout << "player X Y Z: " << localPlayer->body.x << " " << localPlayer->body.y << " " << localPlayer->body.z << std::endl;

            std::cout << "Health - " << *(int*)mem::FindDMAAddy(pLocalPlayer, { 0xF8 }) << "\n";
        }


        //bullshit code starts here (Nuker v2)!
        //crash fixed.
        if (GetAsyncKeyState(VK_NUMPAD9) & 1)
        {
            int* entCount = (int*)entNum;

            bNuke = !bNuke;

            int* myTeam = (int*)pMyTeam;

            int entity = mem::FindDMAAddy(pEntity, { 0x4 });

            int NukeCount = 0;

            float oldX, oldY, oldZ;

            oldX = localPlayer->body.x;
            oldY = localPlayer->body.y;
            oldZ = localPlayer->body.z;

            uintptr_t pPlayerX, pPlayerY, pPlayerZ;
            float* playerX, * playerY, * playerZ;

            pPlayerX     = mem::FindDMAAddy(pLocalPlayer, { 0x3C });
            playerX      = (float*)pPlayerX;
            pPlayerY     = mem::FindDMAAddy(pLocalPlayer, { 0x3C });
            playerY      = (float*)pPlayerY;
            pPlayerZ     = mem::FindDMAAddy(pLocalPlayer, { 0x3C });
            playerZ      = (float*)pPlayerZ;

            //i use all cout's for debug purposes btw :D

            for (int Count = 0; Count < *entCount; ++Count, entity += 0x4)
            {
                int* enemyHp = 0;
                uintptr_t enemyHpAddr = mem::FindDMAAddy(entity, { 0xF8 });
                enemyHp = (int*)enemyHpAddr;
                if (bNuke)
                {
                    NukeCount++;
                    if (*enemyHp != NULL)
                    {
                        if (*enemyHp <= 100 && *enemyHp > 0)
                        {
                            uintptr_t enemyX, enemyY, enemyZ;
                            float* EactualX, * EactualY, * EactualZ;

                            enemyX = mem::FindDMAAddy(entity, { 0x34 });
                            EactualX = (float*)enemyX;
                            enemyY = mem::FindDMAAddy(entity, { 0x38 });
                            EactualY = (float*)enemyY;
                            enemyZ = mem::FindDMAAddy(entity, { 0x3C });
                            EactualZ = (float*)enemyZ;

                            while (*enemyHp > 0)
                            {
                                if (NukeCount <= *entCount)
                                {
                                    *playerX = *EactualX;
                                    *playerY = *EactualY;
                                    *playerZ = *EactualZ;
                                    Sleep(5);
                                }
                                else
                                {
                                    bNuke = !bNuke;
                                    *playerX = oldX;
                                    *playerY = oldY;
                                    *playerZ = oldZ;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (*enemyHp)
                        {
                            entity += 0x4;
                        }
                        else
                        {
                            entity = mem::FindDMAAddy(pEntity, { 0x4 });
                        }
                    }
                }
            }
        }

        //NoRecoil
        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bRecoil = !bRecoil;
            if (bRecoil)
            {
                mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
            }
            else
            {
                //patch it back
                mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
            }
        }

        //leave the loop. Causes the software to uninject
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            break;
        }

        //health and ammo hack (ReClass.net code generator implementation)
        if (pLocalPlayer) //dereference to check whether it exists or not (engliski 100)
        {
            if (bHealth)
            {
                localPlayer->health = 1337;
            }
            if (bAmmo)
            {
                localPlayer->currentWeapon->pAmmo->ammo = 1337; //ignore N00000285. It should be LocalPlayer->currweapon->ammoptr->ammo.
            }
        }
        Sleep(5); //o   p   t   i   m   i   s   a   t   i   o   n
    }
    fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)hackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}