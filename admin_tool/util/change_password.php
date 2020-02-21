<?php
include("db_config.php");
include("EncryptUtil.php");

// 입력 값 확인
if (!isset($_POST['LoginID']) || !isset($_POST['Token']) ||  !isset($_POST['inputPassword']) ) {
    print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
    exit();
}

// 값이 비어 있을 경우 확인
if( $_POST['LoginID'] == "" || $_POST['Token'] == "" || $_POST['inputPassword'] = ""){
    print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
    exit();
}

// Referer 체크
if (strpos($_SERVER['HTTP_REFERER'], "changepassword") == false) {
    print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
    exit();
}

$user_mail = $_POST['LoginID'];
$token = $_POST['Token'];
$user_pw = $_POST['inputPassword'];

// token 으로 유저 아이디 검색
$sql = sprintf(
    "SELECT * FROM `".$mysql_login_table."` WHERE `token` = \"%s\"",
    $mysqli->real_escape_string($token)
);
$result = $mysqli->query($sql);
$row = mysqli_fetch_assoc($result);

// token 과 해당 유저의 이메일이 다를 경우
if ($row['mail'] != $user_mail) {
    print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
    exit();
}
// 유저 No 기록
$user_no = $row['no'];

// 비밀번호 변경 처리
$sql = sprintf(
    "UPDATE `".$mysql_database."`.`".$mysql_login_table."` SET `pwd`= \"%s\"  WHERE  `no`=%s",
    $mysqli->real_escape_string(Encrypt($user_pw, $row['mail'])),
    $mysqli->real_escape_string($user_no)
);
$result = $mysqli->query($sql);

// 토큰 삭제 처리
$sql = sprintf(
    "UPDATE `".$mysql_database."`.`".$mysql_login_table."` SET `token`= \"%s\"  WHERE  `no`=%s",
    $mysqli->real_escape_string(""),
    $mysqli->real_escape_string($user_no)
);
$result = $mysqli->query($sql);

// 로그인 세션을 모두 삭제 한다.
session_start();
session_destroy();

print "<script language=javascript> alert('비밀번호가 정상적으로 변경 되었습니다.'); location.replace('login'); </script>";
exit();
?>