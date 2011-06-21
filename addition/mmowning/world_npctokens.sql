SET NAMES 'utf8';

-- Table structure for table `npc_tokens`

CREATE TABLE IF NOT EXISTS `npc_tokens` (
  `type` tinyint(3) unsigned NOT NULL default '0' COMMENT 'Valid: 0-Gold, 1-Level, 2-Honor, 3-Arena, 4-MaxSkill',
  `min_level` tinyint(3) unsigned NOT NULL default '1',
  `max_level` tinyint(3) unsigned NOT NULL default '70',
  `curr_item_id` mediumint(8) unsigned NOT NULL default '29434' COMMENT 'Currency - Dflt: Badge of Justice',
  `curr_cost` tinyint(3) unsigned NOT NULL default '1',
  `count_granted` mediumint(8) unsigned NOT NULL default '1',
  UNIQUE KEY `type` (`type`,`min_level`,`max_level`,`curr_item_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='NPC Tokens';

-- Data for table `npc_tokens`

INSERT INTO `npc_tokens` (`type`, `min_level`, `max_level`, `curr_item_id`, `curr_cost`, `count_granted`) VALUES
(0, 1, 70, 29434, 1, 500),
(1, 1, 70, 29434, 1, 1),
(2, 1, 70, 29434, 1, 500),
(3, 1, 70, 29434, 1, 100),
(4, 1, 70, 29434, 1, 1);

DELETE FROM creature_template WHERE `entry`=990001;

INSERT INTO `creature_template` (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(990001, 0, 21572, 0, 21572, 0, 'MMOwning Trader', 'Tokens eintauschen', NULL, 0, 70, 70, 5000, 5000, 0, 0, 2865, 35, 35, 1, 1, 1, 1, 60, 165, 0, 97, 1500, 1500, 0, 0, 0, 0, 0, 0, 0, 50, 100, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 1, 1, 1, 0, 0, 0, 'npc_tokens');
