<?php
$mysql_hostname = 'localhost';
$mysql_username = 'root';
$mysql_password = 'windowshyun';
$mysql_database = 'GameServer';
$mysql_admin_login_table = 'admin_table';
$mysql_member_login_table = 'user_table';
$mysql_regist_code = 'regist_code';
$mysql_menu = 'menu';
$mysqli = new  mysqli($mysql_hostname, $mysql_username, $mysql_password, $mysql_database);
// mysqli_connect_errno();
if (mysqli_connect_errno()) {
    printf("Connect failed: %s\n", mysqli_connect_error());
}
?>