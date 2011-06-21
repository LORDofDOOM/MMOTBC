SET NAMES 'utf8';

DROP TABLE IF EXISTS `version`;

CREATE TABLE IF NOT EXISTS `version` (
  `core_version` varchar(120) DEFAULT NULL COMMENT 'Core revision dumped at startup.',
  `core_revision` bigint(20) unsigned DEFAULT NULL,
  `db_version` varchar(120) DEFAULT NULL COMMENT 'Version of world DB.',
  `script_version` varchar(120) DEFAULT NULL COMMENT 'Version of scripts DB.'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Version Notes';


REPLACE INTO `version` (`core_version`, `core_revision`, `db_version`, `script_version`) VALUES
('MMOwningCore Rev: 0 Release Hash: (Unix,little-endian)', 0, 'MMODB base ODB 0.0.7 DEV  for OregonCore rev.1391', 'ACID 2.0.4 - Full Release for Oregon Core (2.4.3 Client)');
