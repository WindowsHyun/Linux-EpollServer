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
$no = $_POST['member_no'];
$name = $_POST['member_name'];
$mail = $_POST['member_mail'];

if (!isset($no) || !isset($name) || !isset($mail) || $name == "" || $mail == "" || $no == "") {
	// print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
	exit();
}

// 해당 DB 테이블에 해당 메일이 있는지 여부를 체크 한다.
$sql = sprintf(
	"DELETE FROM `".$mysql_database."`.`".$mysql_login_table."` WHERE  `no`=%s;",
	$mysqli->real_escape_string($no)
);
$result = $mysqli->query($sql);
if ($result == true) {
	echo "TRUE";
} else {
	echo "FALSE";
}
?>