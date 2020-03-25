<?php
include("db_config.php");
include("EncryptUtil.php");
include("PHPMailer.php");

// 입력 값 확인
if (!isset($_POST['inputEmail'])) {
    print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
    exit();
}

// 값 비어있는지 확인
if ($_POST['inputEmail'] == ""){
    print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
    exit();
}

// Referer 체크
if (strpos($_SERVER['HTTP_REFERER'], "password") == false) {
    print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
    exit();
}

$user_mail = $_POST['inputEmailAddress'];

// 이메일 형식 체크
$check_email = preg_match("/^[_\.0-9a-zA-Z-]+@([0-9a-zA-Z][0-9a-zA-Z-]+\.)+[a-zA-Z]{2,6}$/i", $user_mail);
if ($check_email == false) {
    echo "잘못된 이메일 형식입니다.";
    exit();
}

// 해당 DB 테이블에 해당 메일이 있는지 여부를 체크 한다.
$sql = sprintf(
    "SELECT * FROM `".$mysql_member_login_table."` WHERE `mail` = \"%s\"",
    $mysqli->real_escape_string($user_mail)
);
$result = $mysqli->query($sql);
$row = mysqli_fetch_assoc($result);
$checkTime = "" . date("Y-m-d H:i:s") . "";

if ($row == null) {
    // DB에 해당 아이디가 없을 경우
    print "<script language=javascript> alert('비밀번호 재설정을 위한 Email이 사용자의 계정으로 전송되었습니다.\\n" . $user_mail . " 메일을 확인해 주세요.'); location.replace('login'); </script>";
    exit();
} else {
    $token = EncryptSession("" . $checkTime . "|" . $user_mail . "|" . $row['name'] . "", $user_mail);
    // DB에 해당 아이디가 있을 경우
    $sql = sprintf(
        "UPDATE `".$mysql_database."`.`".$mysql_member_login_table."` SET `token`=\"%s\" WHERE  `no`=%s",
        $mysqli->real_escape_string($token),
        $mysqli->real_escape_string($row['no'])
    );
    $result = $mysqli->query($sql);

    $SessionTime = "" . date("Y-m-d") . "";

    $content = file_get_contents("../txtBundle/forgotpassword.txt");
    $content = str_replace("[%nickName]", $row['name'], $content);
    $content = str_replace("[%URL]", "https://admintool.com/changepassword?token=".$token."&whdb=".EncryptSession($user_mail, $SessionTime)."", $content);

    $email_send = new mailer();
    $email_send->sendMail($user_mail,"암호 재설정 안내 메일 입니다.", $content);
    print "<script language=javascript> alert('비밀번호 재설정을 위한 Email이 사용자의 계정으로 전송되었습니다.\\n" . $user_mail . " 메일을 확인해 주세요.'); location.replace('login'); </script>";
    exit();
}
exit();
?>