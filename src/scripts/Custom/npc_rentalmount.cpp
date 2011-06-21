#include "ScriptPCH.h" 
#include "Config/Config.h"

#define MSG_GOSSIP_TEXT_1  "I would like to rent a mount." 
#define MSG_GOSSIP_TEXT_2  "I would like to rent a very fast mount." 
#define MSG_NOT_MONEY      "You do not have enough money." 
#define MSG_MOUTED         "You already have a mount." 
#define MOUNT_SPELL_ID_1   43899 
#define MOUNT_SPELL_ID_2   43900 

bool GossipHello_npc_rentalmount(Player *player, Creature *_creature) 
{ 
    player->ADD_GOSSIP_ITEM(4, MSG_GOSSIP_TEXT_1, GOSSIP_SENDER_MAIN, 1001); 
    player->ADD_GOSSIP_ITEM(4, MSG_GOSSIP_TEXT_2, GOSSIP_SENDER_MAIN, 1002); 
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID()); 
    return true; 
} 

bool GossipSelect_npc_rentalmount(Player *player, Creature *_creature, uint32 sender, uint32 action ) 
{ 
    if (sender != GOSSIP_SENDER_MAIN) 
        return false; 

    if (player->IsMounted()){ 
        _creature->MonsterWhisper(MSG_MOUTED, player->GetGUID()); 
         return false; 
    } 

    uint32 slowprice = sConfig.GetIntDefault("RentalMountNPC.SlowPrice",100);
    uint32 fastprice = sConfig.GetIntDefault("RentalMountNPC.FastPrice",500);

    switch (action) 
    { 
    case 1001: 
        if (player->GetMoney() < slowprice) 
        { 
            _creature->MonsterWhisper(MSG_NOT_MONEY, player->GetGUID()); 
        } else { 
            player->AddAura(MOUNT_SPELL_ID_1,    player); 
            player->ModifyMoney(-slowprice); 
        } 
        break; 
    case 1002: 
        if (player->GetMoney() < fastprice) 
        { 
            _creature->MonsterWhisper(MSG_NOT_MONEY, player->GetGUID()); 
        } else { 
            player->AddAura(MOUNT_SPELL_ID_2,    player); 
            player->ModifyMoney(-fastprice); 
        } 
        break; 
    } 
    player->CLOSE_GOSSIP_MENU();
    return true; 
} 

void AddSC_npc_rentalmount() 
{ 
     Script *newscript; 
     newscript = new Script; 
     newscript->Name = "npc_rentalmount"; 
     newscript->pGossipHello = &GossipHello_npc_rentalmount; 
     newscript->pGossipSelect = &GossipSelect_npc_rentalmount; 
     newscript->RegisterSelf(); 
}
