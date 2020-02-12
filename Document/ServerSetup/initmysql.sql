-- --------------------------------------------------------
-- 호스트:                          192.168.56.43
-- 서버 버전:                        5.7.28-0ubuntu0.16.04.2 - (Ubuntu)
-- 서버 OS:                        Linux
-- HeidiSQL 버전:                  10.2.0.5599
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- GameServer 데이터베이스 구조 내보내기
CREATE DATABASE IF NOT EXISTS `GameServer` /*!40100 DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci */;
USE `GameServer`;

-- 테이블 GameServer.menu 구조 내보내기
CREATE TABLE IF NOT EXISTS `menu` (
  `me_no` int(11) NOT NULL AUTO_INCREMENT,
  `me_order` int(11) DEFAULT '0',
  `me_suborder` int(11) DEFAULT '0',
  `me_level` int(11) DEFAULT '0',
  `me_class` text NOT NULL,
  `me_name` text NOT NULL,
  `me_icon` text NOT NULL,
  `me_href` text NOT NULL,
  PRIMARY KEY (`me_no`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8mb4;

-- 테이블 데이터 GameServer.menu:~0 rows (대략적) 내보내기
DELETE FROM `menu`;
/*!40000 ALTER TABLE `menu` DISABLE KEYS */;
/*!40000 ALTER TABLE `menu` ENABLE KEYS */;

-- 테이블 GameServer.regist_code 구조 내보내기
CREATE TABLE IF NOT EXISTS `regist_code` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `regist_code` text NOT NULL,
  `count` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`no`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 테이블 데이터 GameServer.regist_code:~0 rows (대략적) 내보내기
DELETE FROM `regist_code`;
/*!40000 ALTER TABLE `regist_code` DISABLE KEYS */;
/*!40000 ALTER TABLE `regist_code` ENABLE KEYS */;

-- 테이블 GameServer.user_table 구조 내보내기
CREATE TABLE IF NOT EXISTS `user_table` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(11) NOT NULL,
  `mail` text NOT NULL,
  `pwd` text NOT NULL,
  `level` int(11) NOT NULL,
  `code` text NOT NULL,
  `token` text,
  `regtime` datetime NOT NULL,
  `lastLogin` datetime DEFAULT NULL,
  PRIMARY KEY (`no`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4;

-- 테이블 데이터 GameServer.user_table:~0 rows (대략적) 내보내기
DELETE FROM `user_table`;
/*!40000 ALTER TABLE `user_table` DISABLE KEYS */;
/*!40000 ALTER TABLE `user_table` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
