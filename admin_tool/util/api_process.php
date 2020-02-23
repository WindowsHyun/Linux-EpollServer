<?php
include("./db_config.php");
include("./define_text.php");
include("./EncryptUtil.php");
include("./session_data.php");

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

$API = $_POST['api'];
$sql = '';

switch ($API) {
	case ADMIN_ADD_MENU:
		$order = $_POST['me_order_add'];
		$subOrder = $_POST['me_suborder_add'];
		$level = $_POST['me_level_add'];
		$class = $_POST['me_class_add'];
		$menu = $_POST['me_name_add'];
		$icon = $_POST['me_icon_add'];
		$page = $_POST['me_href_add'];

		if (!isset($order) || !isset($subOrder) || !isset($level) || $order == "" || $subOrder == "" || $level == "") {
			print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
			exit();
		}

		// sql 실행
		$sql = sprintf(
			"INSERT INTO `" . $mysql_database . "`.`" . $mysql_menu . "` (`me_order`, `me_suborder`, `me_level`, `me_class`, `me_name`, `me_icon`, `me_href`) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
			$mysqli->real_escape_string($order),
			$mysqli->real_escape_string($subOrder),
			$mysqli->real_escape_string($level),
			$mysqli->real_escape_string($class),
			$mysqli->real_escape_string($menu),
			$mysqli->real_escape_string($icon),
			$mysqli->real_escape_string($page)
		);
		break;

	case ADMIN_DEL_MENU:
		$no = $_POST['me_no'];
		$order = $_POST['me_order'];
		$suborder = $_POST['me_suborder'];
		$level = $_POST['me_level'];
		$class = $_POST['me_class'];
		$name = $_POST['me_name'];
		$icon = $_POST['me_icon'];
		$href = $_POST['me_href'];

		if (!isset($no) || !isset($order) || !isset($suborder) || !isset($level) || !isset($name) || !isset($icon) || $no == "" || $order == "" || $suborder == "" || $level == "" || $name == "" || $icon == "") {
			print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
			exit();
		}

		// SQL 실행
		$sql = sprintf(
			"DELETE FROM `" . $mysql_database . "`.`" . $mysql_menu . "` WHERE  `me_no`=%s;",
			$mysqli->real_escape_string($no)
		);
		break;

	case ADMIN_ADD_MEMBER:
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
			"INSERT INTO `" . $mysql_database . "`.`" . $mysql_login_table . "` (`name`, `mail`, `pwd`) VALUES ('%s', '%s', '%s')",
			$mysqli->real_escape_string($name),
			$mysqli->real_escape_string($email),
			$mysqli->real_escape_string(Encrypt($pwd, $email))
		);
		break;

	case ADMIN_DEL_MEMBER:
		$no = $_POST['member_no'];
		$name = $_POST['member_name'];
		$mail = $_POST['member_mail'];

		if (!isset($no) || !isset($name) || !isset($mail) || $name == "" || $mail == "" || $no == "") {
			print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
			exit();
		}

		// 해당 DB 테이블에 해당 메일이 있는지 여부를 체크 한다.
		$sql = sprintf(
			"DELETE FROM `" . $mysql_database . "`.`" . $mysql_login_table . "` WHERE  `no`=%s;",
			$mysqli->real_escape_string($no)
		);
		break;
}

$result = $mysqli->query($sql);
if ($result == true) {
	echo "TRUE";
} else {
	echo "FALSE";
}
?>