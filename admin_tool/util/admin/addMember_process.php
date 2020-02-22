<?php
include("../db_config.php");
include("../EncryptUtil.php");
include("../session_data.php");

// 레벨 제한 체크
// if ($user_level <= 9) {
// 	print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
// 	exit();
// }

// Referer 체크
if (strpos($_SERVER['HTTP_REFERER'], "index") == false) {
	print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
	exit();
}

$name = $_POST['member_add_name'];
$email = $_POST['member_add_email'];
$pwd = $_POST['member_add_pwd'];
$permission = $_POST['member_add_permission'];

if (!isset($name) || !isset($email) || !isset($pwd) || $name == "" || $email == "" || $pwd == "") {
	print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
	exit();
}

// 해당 DB 테이블에 해당 메일이 있는지 여부를 체크 한다.
$sql = sprintf(
	"INSERT INTO `".$mysql_database."`.`".$mysql_login_table."` (`name`, `mail`, `pwd`) VALUES ('%s', '%s', '%s')",
	$mysqli->real_escape_string($name),
	$mysqli->real_escape_string($email),
	$mysqli->real_escape_string(Encrypt($pwd, $email))
);
$result = $mysqli->query($sql);
if ($result == true) {
	echo "TRUE";
} else {
	echo "FALSE";
}
?>