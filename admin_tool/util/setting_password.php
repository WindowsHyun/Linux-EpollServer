<?php
include("./db_config.php");
include("./EncryptUtil.php");

session_start();

$user_opw = $_POST['inputOriginalPassword'];
$user_pw = $_POST['inputPassword'];
$user_cpw = $_POST['confirmPassword'];
$user_email = $_POST['inputEmail'];
$user_name = $_POST['inputName'];
$regtime = "" . date("Y-m-d H:i:s") . "";

if ( ($user_pw == $user_cpw) && $user_pw == $user_opw ){
	echo "이전 비밀번호를 사용 할 수 없습니다.";
	exit();
}else if ($user_pw != $user_cpw){
	echo "비밀번호가 서로 정확한지 확인하여 주세요.";
	exit();
}else if ( $_SESSION['user_mail'] != $user_email ){
	echo "이메일이 서로 정확한지 확인하여 주세요.";
	exit();
}


// 비밀번호 변경 처리를 한다.
$sql = sprintf(
	"UPDATE `".$mysql_database."`.`".$mysql_login_table."` SET `pwd`='%s' WHERE  `mail`='%s';",
	$mysqli->real_escape_string(Encrypt($user_pw, $user_email)),
	$mysqli->real_escape_string($user_email)
);
$result = $mysqli->query($sql);

if ($result == true) {
	echo "비밀번호 변경 완료!";
	echo "<meta http-equiv='refresh' content='0;url=logout'>";
} else {
	echo "비밀번호 변경 실패!";
	exit();
}
?>