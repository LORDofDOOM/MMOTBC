--
-- Table structure for table `guild_houses`
--


/**
* Records Guildhouse Keeper
*/
DELETE FROM creature_template WHERE `entry`=13;
INSERT INTO `creature_template` VALUES ('13', '0', '0', '0', '0', '0', '26789', '0', '0', '0', 'MMOwning Gildenhäuser', 'Guildhouse Keeper', '', '0', '80', '80', '0', '35', '35', '1', '1.48', '1.14286', '0.75', '0', '181', '189', '0', '158', '1', '1400', '1900', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '1', '3', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', 'guildmaster', '0');

DELETE FROM trinity_string WHERE `entry`=11500;
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES (11500, 'Deine Gilde besitzt noch kein Gildenhaus.');
DELETE FROM `command` WHERE `name`='gh';
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('gh', 0, 'Syntax: .gh Teleportiert dich zu eurem Gildenhaus (Kann nicht im Kampf, Flug oder auf einem Mount verwendet werden)');


CREATE TABLE IF NOT EXISTS `guild_houses` (
  `id` int(8) unsigned NOT NULL auto_increment,
  `guildId` bigint(20) NOT NULL default '0',
  `x` double NOT NULL,
  `y` double NOT NULL,
  `z` double NOT NULL,
  `map` int(11) NOT NULL,
  `comment` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;

--
-- Dumping data for table `guild_houses`
--

INSERT INTO `guild_houses` (`id`, `guildId`, `x`, `y`, `z`, `map`, `comment`) VALUES
(1, 1, 16222, 16266, 14.2, 1, 'GM Island'),
(2, 0, -10711, 2483, 8, 1, 'Village on the Veiled Sea (Silithus)'),
(3, 0, -8323, -343, 146, 0, 'Elwynn Falls Camp (Elwynn Forest)'),
(4, 0, -1840, -4233, 2.14, 0, 'Arathi Village (Arathi Highlands, Forbidding Sea)'),
(5, 0, -6374, 1262, 7, 0, 'Harbor House (Elwynn Forest)'),
(6, 0, 4303, -2760, 16.8, 0, 'Quel''Thalas Tower');


