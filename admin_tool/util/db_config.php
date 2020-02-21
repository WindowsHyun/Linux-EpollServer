<?php
$mysql_hostname = 'localhost';
$mysql_username = 'root';
$mysql_password = 'windowshyun';
$mysql_database = 'GameServer';
$mysql_login_table = 'admin_table';
$mysql_regist_code = 'regist_code';
$mysql_menu = 'menu';
$mysqli = new  mysqli($mysql_hostname, $mysql_username, $mysql_password, $mysql_database);
mysqli_connect_errno();
?>