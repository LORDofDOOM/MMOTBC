/* ScriptData
SDName: NPC_Heal
SD%Complete: 100
SDComment: Healer
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"

bool GossipHello_NPC_Heal(Player *player, Creature *_Creature)
{
    // Main Menu 
    player->ADD_GOSSIP_ITEM( 5, "Gib mir Blessing of Might"
    , GOSSIP_SENDER_MAIN, 1035);
    player->ADD_GOSSIP_ITEM( 5, "Gib mir Blessing of Wisdom"
    , GOSSIP_SENDER_MAIN, 1040);
    player->ADD_GOSSIP_ITEM( 5, "Gib mir Power Word: Fortitude"
    , GOSSIP_SENDER_MAIN, 1045);
    player->ADD_GOSSIP_ITEM( 5, "Bitte heil mich"
    , GOSSIP_SENDER_MAIN, 4010);
    player->ADD_GOSSIP_ITEM( 5, "Remove Resurrect Sickness"
    , GOSSIP_SENDER_MAIN, 5000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
    return true;
}

void SendDefaultMenu_NPC_Heal(Player *player, Creature *_Creature, uint32 action )
{

switch(action)
{

// Small Buff

case 1035: // Give me Blessing of Might
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,19838,false);
break;

case 1040: // Give me Blessing of Wisdom
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,25290,false);
break;

case 1045: // Give me Power Word: Fortitude
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,10938,false);
break;

// Player Tools

case 4010: // Heal me please
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,38588,false);
break;

case 5000: // Remove Res Sickness
    if(!player->HasAura(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS,0)) {
        _Creature->MonsterWhisper("You do not have resurrection sickness.", player->GetGUID());
        GossipHello_NPC_Heal(player, _Creature);
        return;
    }

    _Creature->CastSpell(player,38588,false); // Healing effect
    player->RemoveAurasDueToSpell(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS);
    player->CLOSE_GOSSIP_MENU();
    break;

    player->CLOSE_GOSSIP_MENU();

    } // end of switch
} //end of function

bool GossipSelect_NPC_Heal(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    // Main menu
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_NPC_Heal( player, _Creature, action );
    return true;
}

void AddSC_npc_heal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "npc_heal";
    newscript->pGossipHello = &GossipHello_NPC_Heal;
    newscript->pGossipSelect = &GossipSelect_NPC_Heal;
    newscript->RegisterSelf();
}

