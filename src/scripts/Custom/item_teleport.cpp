/* ScriptData
SDName: Item_Teleport
SD%Complete: 100
SDComment: Used for Teleport Item Scripts
SDCategory: Items
EndScriptData */

#include "ScriptPCH.h"
#include <cstring>

bool ItemUse_item_teleport(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    // Declare variables
    float x_pos, y_pos, z_pos, o_pos;
    uint32 mapid;
    uint32 itemId = _Item->GetEntry();
    extern DatabaseType WorldDatabase;

    // Fetch the coordinates
    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `mapid`, `X_pos`, `Y_pos`, `Z_pos`, `orientation`  FROM `item_teleports` WHERE `entry` = '%i' LIMIT 1",itemId);
    if(result)
    {
        Field *fields = result->Fetch();
        // Read coords from the query result
        mapid = fields[0].GetInt32();
        x_pos = fields[1].GetFloat();
        y_pos = fields[2].GetFloat();
        z_pos = fields[3].GetFloat();
        o_pos = fields[4].GetFloat();
        // Teleport the player and show info in log
        player->TeleportTo(mapid, x_pos, y_pos, z_pos, o_pos);
        outstring_log ("Teleport aktiviert: %i (m:%i x:%f y:%f z:%f o:%f)",itemId,mapid,x_pos,y_pos,z_pos,o_pos);
        return true;
    } else {
        // Teleport failed - show info in log
        outstring_log ("Teleport abgebrochen: %i (m:%i x:%f y:%f z:%f o:%f)",itemId,mapid,x_pos,y_pos,z_pos,o_pos);
    }
}

void AddSC_item_teleport()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_teleport";
    newscript->pItemUse = &ItemUse_item_teleport;
    newscript->RegisterSelf();
}
