/* ScriptData
SDName: Token Exchange NPC
SD%Complete: 100
SDComment: By Celtus based on concepts from the TC2 Forums
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include <cstring>

extern DatabaseType WorldDatabase;

uint32 goldItemId;
uint32 tokensForGold;
uint32 goldGranted;
uint32 levelItemId;
uint32 tokensForLevel;
uint32 levelsGranted;
uint32 honorItemId;
uint32 tokensForHonor;
uint32 honorGranted;
uint32 arenaItemId;
uint32 tokensForArena;
uint32 arenaGranted;
uint32 skillItemId;
uint32 tokensForMaxSkill;

bool GossipHello_npc_tokens(Player* pPlayer, Creature* pCreature)
{
    uint32 playerLevel = pPlayer->getLevel();

    QueryResult_AutoPtr result0 = WorldDatabase.PQuery("SELECT `curr_item_id`, `curr_cost`, `count_granted` FROM `npc_tokens` WHERE `type` = 0 AND `min_level` <= %i AND `max_level` >= %i LIMIT 1", playerLevel, playerLevel);
    if (result0) 
    {
      Field *fields0 = result0->Fetch();
      goldItemId = fields0[0].GetUInt32(); 
      tokensForGold = fields0[1].GetUInt32();
      goldGranted = fields0[2].GetUInt32();
      pPlayer->ADD_GOSSIP_ITEM( 9, "Tokens für Gold eintauschen", GOSSIP_SENDER_MAIN, 1000);
    }
    QueryResult_AutoPtr result1 = WorldDatabase.PQuery("SELECT `curr_item_id`, `curr_cost`, `count_granted` FROM `npc_tokens` WHERE `type` = 1 AND `min_level` <= %i AND `max_level` >= %i LIMIT 1", playerLevel, playerLevel);
    if (result1)
    {
      Field *fields1 = result1->Fetch();
      levelItemId = fields1[0].GetUInt32();
      tokensForLevel = fields1[1].GetUInt32();
      levelsGranted = fields1[2].GetUInt32();
      pPlayer->ADD_GOSSIP_ITEM( 9, "Tokens für Level eintauschen", GOSSIP_SENDER_MAIN, 2000);
    }
    QueryResult_AutoPtr result2 = WorldDatabase.PQuery("SELECT `curr_item_id`, `curr_cost`, `count_granted` FROM `npc_tokens` WHERE `type` = 2 AND `min_level` <= %i AND `max_level` >= %i LIMIT 1", playerLevel, playerLevel);
    if (result2)
    {
      Field *fields2 = result2->Fetch();
      honorItemId = fields2[0].GetUInt32();
      tokensForHonor = fields2[1].GetUInt32();
      honorGranted = fields2[2].GetUInt32();
      pPlayer->ADD_GOSSIP_ITEM( 9, "Tokens für Ehrenpunkte eintauschen", GOSSIP_SENDER_MAIN, 3000);
    }
    QueryResult_AutoPtr result3 = WorldDatabase.PQuery("SELECT `curr_item_id`, `curr_cost`, `count_granted` FROM `npc_tokens` WHERE `type` = 3 AND `min_level` <= %i AND `max_level` >= %i LIMIT 1", playerLevel, playerLevel);
    if (result3)
    {
      Field *fields3 = result3->Fetch();
      arenaItemId = fields3[0].GetUInt32();
      tokensForArena = fields3[1].GetUInt32();
      arenaGranted = fields3[2].GetUInt32();      
      pPlayer->ADD_GOSSIP_ITEM( 9, "Tokens für Arenapunkte eintauschen", GOSSIP_SENDER_MAIN, 4000);
    }
    QueryResult_AutoPtr result4 = WorldDatabase.PQuery("SELECT `curr_item_id`, `curr_cost` FROM `npc_tokens` WHERE `type` = 4 AND `min_level` <= %i AND `max_level` >= %i LIMIT 1", playerLevel, playerLevel);
    if (result4)    {
      Field *fields4 = result4->Fetch();
      skillItemId = fields4[0].GetUInt32();
      tokensForMaxSkill = fields4[1].GetUInt32();
      pPlayer->ADD_GOSSIP_ITEM( 9, "Tokens für max. Skills eintauschen", GOSSIP_SENDER_MAIN, 5000);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;    
}

void SendDefaultMenu_npc_tokens(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{
    // Not allowed if in combat.
    if (pPlayer->isInCombat())
    {
      pPlayer->CLOSE_GOSSIP_MENU();
      pCreature->MonsterSay("Du bist im Kampf!", LANG_UNIVERSAL, NULL);
      return;
    }
    
    // Process menu selection
    switch(uiAction)
    {
        case 1000:
            // Trade Tokens for Gold
            if(pPlayer->HasItemCount(goldItemId, tokensForGold , true))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->DestroyItemCount(goldItemId, tokensForGold, true);
                pPlayer->ModifyMoney(goldGranted * 10000);
                pCreature->MonsterWhisper("Dein Gold wurde hinzugefügt.", pPlayer->GetGUID());
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("Du hast nicht genügend Tokens!", LANG_UNIVERSAL, NULL);
            }
            break;

        case 2000:
            // Trade Tokens for Level
            if(pPlayer->HasItemCount(levelItemId, tokensForLevel, true))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->DestroyItemCount(levelItemId, tokensForLevel, true);
                pPlayer->GiveLevel(pPlayer->getLevel() + levelsGranted);
                pCreature->MonsterWhisper("Dein Level wurde geändert.", pPlayer->GetGUID());
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("Du hast nicht genügend Tokens!", LANG_UNIVERSAL, NULL);
            }
            break;

        case 3000:
            // Trade Tokens for Honor Points
            if(pPlayer->HasItemCount(honorItemId, tokensForHonor, true))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->DestroyItemCount(honorItemId, tokensForHonor, true);
                pPlayer->ModifyHonorPoints(+ honorGranted);
                pCreature->MonsterWhisper("Deine Ehrenpunkte wurden hinzugefügt.", pPlayer->GetGUID());
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("Du hast nicht genügend Tokens!", LANG_UNIVERSAL, NULL);
            }
            break;
 
        case 4000:
            // Trade Tokens for Arena Points
            if(pPlayer->HasItemCount(arenaItemId, tokensForArena, true))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->DestroyItemCount(arenaItemId, tokensForArena, true);
                pPlayer->ModifyArenaPoints(+ arenaGranted);
                pCreature->MonsterWhisper("Deine Arenapunkte wurden hinzugefügt.", pPlayer->GetGUID());
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("Du hast nicht genügend Tokens!", LANG_UNIVERSAL, NULL);
            }
            break;

        case 5000:
            // Trade Tokens for MaxSkill
            if(pPlayer->HasItemCount(skillItemId, tokensForMaxSkill, true))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->DestroyItemCount(skillItemId, tokensForMaxSkill, true);
                pPlayer->UpdateSkillsToMaxSkillsForLevel();
                pCreature->MonsterWhisper("Deine Skills sind jetzt auf maximum", pPlayer->GetGUID());
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("Du hast nicht genügend Tokens!", LANG_UNIVERSAL, NULL);
            }
            break;
    }
}

bool GossipSelect_npc_tokens(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    // Show menu
    if (uiSender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_npc_tokens(pPlayer, pCreature, uiAction);
    return true;
}

void AddSC_npc_tokens()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "npc_tokens";
    newscript->pGossipHello = &GossipHello_npc_tokens;
    newscript->pGossipSelect = &GossipSelect_npc_tokens;
    newscript->RegisterSelf();
}

