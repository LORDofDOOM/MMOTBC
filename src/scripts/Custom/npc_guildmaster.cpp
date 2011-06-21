/* ScriptData
SDName: GuildMaster
SD%Complete: 95
SDComment: Guild House Master
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include "Config/Config.h"

extern DatabaseType CharacterDatabase;

#define MSG_GOSSIP_TELE          "Zum Gildenhaus teleportieren"
#define MSG_GOSSIP_NEXTPAGE      "Weiter -->"
#define MSG_INCOMBAT             "Bitte beende vorher deinen Kampf!"
#define MSG_NOGUILDHOUSE         "Deine Gilde besitzt kein Gildenhaus"
#define MSG_NOFREEGH             "Momentan sind leider keine Gildenhaeuser mehr frei"
#define MSG_ALREADYHAVEGH        "Deine Gilde hat bereits ein Gildenhaus (%s)."
#define MSG_NOTENOUGHMONEY       "Du hast die benoetigten %u gold um ein Gildenhaus zu kaufen."
#define MSG_GHOCCUPIED           "Leider ist das Gildenhaus bereits belegt"
#define MSG_CONGRATULATIONS      "Gratulation! Das Gildenhaus wurde erstellt"
#define MSG_SOLD                 "Dein Gildenhaus wurde verkauft, du kannst jetzt ein neues kaufen!"
#define MSG_NOTINGUILD           "Du bist in keiner Gilde"

#define CODE_SELL 				 "SELL"
#define MSG_CODEBOX_SELL 		 "Tippe \"" CODE_SELL "\" wenn du dein Gildenhaus WIRKLICH verkaufen moechtest.."

#define OFFSET_GH_ID_TO_ACTION   1500
#define OFFSET_SHOWBUY_FROM      10000

#define ACTION_TELE              1001
#define ACTION_SHOW_BUYLIST      1002  //deprecated. Use (OFFSET_SHOWBUY_FROM + 0) instead
#define ACTION_SELL_GUILDHOUSE   1003

#define ICON_GOSSIP_BALOON       0
#define ICON_GOSSIP_WING         2
#define ICON_GOSSIP_BOOK         3
#define ICON_GOSSIP_WHEEL1       4
#define ICON_GOSSIP_WHEEL2       5
#define ICON_GOSSIP_GOLD         6
#define ICON_GOSSIP_BALOONDOTS   7
#define ICON_GOSSIP_TABARD       8
#define ICON_GOSSIP_XSWORDS      9

#define GOSSIP_COUNT_MAX         10

bool isPlayerGuildLeader(Player *player)
{
    return (player->GetRank() == 0) && (player->GetGuildId() != 0);
}

bool getGuildHouseCoords(uint32 guildId, float &x, float &y, float &z, uint32 &map)
{
    if (guildId == 0)
    {
        //if player has no guild
        return false;
    }

    QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guild_houses` WHERE `guildId` = %u", guildId);
    if(result)
    {
        Field *fields = result->Fetch();
        x = fields[0].GetFloat();
        y = fields[1].GetFloat();
        z = fields[2].GetFloat();
        map = fields[3].GetUInt32();
        return true;
    }
    return false;
}

void teleportPlayerToGuildHouse(Player *player, Creature *_creature)
{
    if (player->GetGuildId() == 0)
    {
        //if player has no guild
        _creature->MonsterWhisper(MSG_NOTINGUILD, player->GetGUID());
        return;
    }

    if (!player->getAttackers().empty())
    {
        //if player in combat
        _creature->MonsterSay(MSG_INCOMBAT, LANG_UNIVERSAL, player->GetGUID());
        return;
    }

    float x, y, z;
    uint32 map;

    if (getGuildHouseCoords(player->GetGuildId(), x, y, z, map))
    {
        //teleport player to the specified location
        player->TeleportTo(map, x, y, z, 0.0f);
    }
    else
        _creature->MonsterWhisper(MSG_NOGUILDHOUSE, player->GetGUID());

}

bool showBuyList(Player *player, Creature *_creature, uint32 showFromId = 0)
{
    //show not occupied guildhouses

    QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT `id`, `comment` FROM `guild_houses` WHERE `guildId` = 0 AND `id` > %u ORDER BY `id` ASC LIMIT %u",
        showFromId, GOSSIP_COUNT_MAX);

    if (result)
    {
        uint32 guildhouseId = 0;
        std::string comment = "";
        do
        {

            Field *fields = result->Fetch();

            guildhouseId = fields[0].GetInt32();
            comment = fields[1].GetString();
            
            //send comment as a gossip item
            //transmit guildhouseId in Action variable
            player->ADD_GOSSIP_ITEM(ICON_GOSSIP_TABARD, comment, GOSSIP_SENDER_MAIN,
                guildhouseId + OFFSET_GH_ID_TO_ACTION);

        } while (result->NextRow());

        if (result->GetRowCount() == GOSSIP_COUNT_MAX)
        {
            //assume that we have additional page
            //add link to next GOSSIP_COUNT_MAX items
            player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOONDOTS, MSG_GOSSIP_NEXTPAGE, GOSSIP_SENDER_MAIN, 
                guildhouseId + OFFSET_SHOWBUY_FROM);
        }

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());

        return true;
    } else
    {
        if (showFromId == 0)
        {
            //all guildhouses are occupied
            _creature->MonsterWhisper(MSG_NOFREEGH, player->GetGUID());
            player->CLOSE_GOSSIP_MENU();
        } else
        {
            //this condition occurs when COUNT(guildhouses) % GOSSIP_COUNT_MAX == 0
            //just show GHs from beginning
            showBuyList(player, _creature, 0);
        }
    }

    return false;
}

bool isPlayerHasGuildhouse(Player *player, Creature *_creature, bool whisper = false)
{

    QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT `comment` FROM `guild_houses` WHERE `guildId` = %u",
        player->GetGuildId());

    if (result)
    {
        if (whisper)
        {
            //whisper to player "already have etc..."
            Field *fields = result->Fetch();
            char msg[100];
            sprintf(msg, MSG_ALREADYHAVEGH, fields[0].GetString());
            _creature->MonsterWhisper(msg, player->GetGUID());
        }

        return true;
    }
    return false;

}

void buyGuildhouse(Player *player, Creature *_creature, uint32 guildhouseId)
{
    uint32 buyprice = (sConfig.GetIntDefault("GuildMasterNPC.BuyPriceInGold",500)*10000);
    if (player->GetMoney() < buyprice)
    {
        //show how much money player need to buy GH (in gold)
        char msg[100];
        sprintf(msg, MSG_NOTENOUGHMONEY, buyprice / 10000);
        _creature->MonsterWhisper(msg, player->GetGUID());
        return;
    }

    if (isPlayerHasGuildhouse(player, _creature, true))
    {
        //player already have GH
        return;
    }

    //check if somebody already occupied this GH
    QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT `id` FROM `guild_houses` WHERE `id` = %u AND `guildId` <> 0",
        guildhouseId);

    if (result)
    {
        _creature->MonsterWhisper(MSG_GHOCCUPIED, player->GetGUID());
        return;
    }

    //update DB
    QueryResult_AutoPtr result_updt = CharacterDatabase.PQuery("UPDATE `guild_houses` SET `guildId` = %u WHERE `id` = %u",
        player->GetGuildId(), guildhouseId);
    
    player->ModifyMoney(-buyprice);
    _creature->MonsterSay(MSG_CONGRATULATIONS, LANG_UNIVERSAL, player->GetGUID());
    
}

void sellGuildhouse(Player *player, Creature *_creature)
{
    uint32 sellprice = (sConfig.GetIntDefault("GuildMasterNPC.SellPriceInGold",400)*10000);
    if (isPlayerHasGuildhouse(player, _creature))
    {
        QueryResult_AutoPtr result = CharacterDatabase.PQuery("UPDATE `guild_houses` SET `guildId` = 0 WHERE `guildId` = %u",
        player->GetGuildId());
        
        player->ModifyMoney(sellprice);

        //display message e.g. "here your money etc."
        char msg[100];
        sprintf(msg, MSG_SOLD, (sellprice/10000));
        _creature->MonsterWhisper(msg, player->GetGUID());
    }
}

bool GossipHello_npc_guildmaster(Player *player, Creature *_creature)
{
    player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOON, MSG_GOSSIP_TELE, 
        GOSSIP_SENDER_MAIN, ACTION_TELE);

    if (isPlayerGuildLeader(player))
    {
        uint8 buyEnabled = sConfig.GetIntDefault("GuildMasterNPC.BuyEnabled", 1);
        if ( buyEnabled == 1 )
        {
            //show additional menu for guild leader
            std::string buyGHGossip = sConfig.GetStringDefault("GuildMasterNPC.BuyGossip", "Buy a Guild House for 500 Gold");
            player->ADD_GOSSIP_ITEM(ICON_GOSSIP_GOLD, buyGHGossip,
                GOSSIP_SENDER_MAIN, ACTION_SHOW_BUYLIST);
        }
        if (isPlayerHasGuildhouse(player, _creature))
        {
            uint8 sellEnabled = sConfig.GetIntDefault("GuildMasterNPC.SellEnabled", 1);
            if ( sellEnabled == 1 )
            {
                //and additional for guildhouse owner
                std::string sellGHGossip = sConfig.GetStringDefault("GuildMasterNPC.SellGossip", "Sell your Guild House for 400 Gold");
                player->PlayerTalkClass->GetGossipMenu().AddMenuItem(ICON_GOSSIP_GOLD, sellGHGossip, GOSSIP_SENDER_MAIN, ACTION_SELL_GUILDHOUSE, MSG_CODEBOX_SELL, 0, true);
            }
        }
    }
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
    return true;
}


bool GossipSelect_npc_guildmaster(Player *player, Creature *_creature, uint32 sender, uint32 action )
{
    if (sender != GOSSIP_SENDER_MAIN)
        return false;

    switch (action)
    {
        case ACTION_TELE:
            //teleport player to GH
            player->CLOSE_GOSSIP_MENU();
            teleportPlayerToGuildHouse(player, _creature);
            break;
        case ACTION_SHOW_BUYLIST:
            //show list of GHs which currently not occupied
            showBuyList(player, _creature);
            break;
        default:
            if (action > OFFSET_SHOWBUY_FROM)
            {
                showBuyList(player, _creature, action - OFFSET_SHOWBUY_FROM);
            } else if (action > OFFSET_GH_ID_TO_ACTION)
            {
                //player clicked on buy list
                player->CLOSE_GOSSIP_MENU();
                //get guildhouseId from action
                //guildhouseId = action - OFFSET_GH_ID_TO_ACTION
                buyGuildhouse(player, _creature, action - OFFSET_GH_ID_TO_ACTION);
            }
            break;
    }
    
    return true;
}

bool GossipSelectWithCode_npc_guildmaster( Player *player, Creature *_creature,
                                      uint32 sender, uint32 action, const char* sCode )
{
    if(sender == GOSSIP_SENDER_MAIN)
    {
        if(action == ACTION_SELL_GUILDHOUSE)
        {
            int i = -1;
            try
            {
                //compare code
                if (strlen(sCode) + 1 == sizeof CODE_SELL)
                    i = strcmp(CODE_SELL, sCode);

            } catch(char *str) {error_db_log(str);}

            if (i == 0)
            {
                //right code
                sellGuildhouse(player, _creature);
            }
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
    }
    return false;
}


void AddSC_npc_guildmaster()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "npc_guildmaster";
    newscript->pGossipHello = &GossipHello_npc_guildmaster;
    newscript->pGossipSelect = &GossipSelect_npc_guildmaster;
    newscript->pGossipSelectWithCode =  &GossipSelectWithCode_npc_guildmaster;
    newscript->RegisterSelf();
}

