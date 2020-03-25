<?php
include("./db_config.php");
include("./EncryptUtil.php");

$register_code = $_POST['inputRegisterCode'];
$user_pw = $_POST['inputPassword'];
$user_email = $_POST['inputEmail'];
$user_name = $_POST['inputName'];
$regtime = "" . date("Y-m-d H:i:s") . "";

// 이메일 중복 체크를 한번 더 한다.
$sql = sprintf(
	"SELECT mail FROM ".$mysql_member_login_table." WHERE mail = \"%s\"",
	$mysqli->real_escape_string($user_email)
);
$result = $mysqli->query($sql);
$row = mysqli_fetch_assoc($result);

if ($row['mail'] != '') {
	echo "중복 된 이메일 주소 입니다.";
	exit();
}

// 가입 코드를 확인 한다.
$sql = sprintf(
	"SELECT * FROM ".$mysql_regist_code." WHERE `regist_code` = \"%s\"",
	$mysqli->real_escape_string($register_code)
);
$result = $mysqli->query($sql);
$row = mysqli_fetch_assoc($result);

if ($row['count'] > 0) {
	// 가입할 수 있는 코드가 있을 경우
	$count = --$row['count'];
} else {
	echo "가입 할 수 없는 코드 입니다.";
	exit();
}

// 실제 가입 처리를 한다.
$sql = sprintf(
	" INSERT INTO `".$mysql_database."`.`".$mysql_member_login_table."` (`name`, `mail`, `pwd`, `level`, `code`, `token`, `regtime` , `lastLogin`) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
	$mysqli->real_escape_string($user_name),
	$mysqli->real_escape_string($user_email),
	$mysqli->real_escape_string(Encrypt($user_pw, $user_email)),
	$mysqli->real_escape_string("2"),
	$mysqli->real_escape_string($register_code),
	$mysqli->real_escape_string(""),
	$mysqli->real_escape_string($regtime),
	$mysqli->real_escape_string($regtime)
);
$result = $mysqli->query($sql);

if ($result == true) {
	// 가입 카운트는 줄어 든다.
	$sql = sprintf(
		"UPDATE `".$mysql_database."`.`".$mysql_regist_code."` SET `count`='%s' WHERE  `no`=%s",
		$mysqli->real_escape_string($count),
		$mysqli->real_escape_string($row['no'])
	);
	$result = $mysqli->query($sql);
	echo "가입 완료!";
	echo "<meta http-equiv='refresh' content='0;url=index'>";
} else {
	echo "가입 실패!";
	exit();
}
?>