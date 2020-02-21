<?php
include($_SERVER['DOCUMENT_ROOT'] . "/util/db_config.php");
include($_SERVER['DOCUMENT_ROOT'] . "/util/EncryptUtil.php");
include($_SERVER['DOCUMENT_ROOT'] . "/util/session_data.php");

// 레벨 제한 체크
if ($user_level <= 9) {
	print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
	exit();
}

// Referer 체크
if (strpos($_SERVER['HTTP_REFERER'], "index") == false) {
	print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
	exit();
}

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

//INSERT INTO `mypc`.`menu` (`me_order`, `me_level`, `me_class`, `me_name`, `me_icon`, `me_href`) VALUES ('8', '2', '1', '2', '3', '4');

// 해당 DB 테이블에 해당 메일이 있는지 여부를 체크 한다.
$sql = sprintf(
	"INSERT INTO `mypc`.`menu` (`me_order`, `me_suborder`, `me_level`, `me_class`, `me_name`, `me_icon`, `me_href`) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
	$mysqli->real_escape_string($order),
	$mysqli->real_escape_string($subOrder),
	$mysqli->real_escape_string($level),
	$mysqli->real_escape_string($class),
	$mysqli->real_escape_string($menu),
	$mysqli->real_escape_string($icon),
	$mysqli->real_escape_string($page)
);
$result = $mysqli->query($sql);
if ($result == true) {
	echo "TRUE";
} else {
	echo "FALSE";
}
?>