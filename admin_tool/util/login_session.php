<?php
session_start();
function get_client_ip()
{
	$ipaddress = '';
	if (getenv('HTTP_CLIENT_IP'))
		$ipaddress = getenv('HTTP_CLIENT_IP');
	else if (getenv('HTTP_X_FORWARDED_FOR'))
		$ipaddress = getenv('HTTP_X_FORWARDED_FOR');
	else if (getenv('HTTP_X_FORWARDED'))
		$ipaddress = getenv('HTTP_X_FORWARDED');
	else if (getenv('HTTP_FORWARDED_FOR'))
		$ipaddress = getenv('HTTP_FORWARDED_FOR');
	else if (getenv('HTTP_FORWARDED'))
		$ipaddress = getenv('HTTP_FORWARDED');
	else if (getenv('REMOTE_ADDR'))
		$ipaddress = getenv('REMOTE_ADDR');
	else
		$ipaddress = 'UNKNOWN';
	return $ipaddress;
}
if (strpos($_SERVER['REQUEST_URI'], "register")) {
	// register 주소에 들어온 상태에서, 회원으로 로그인 한 상태일 경우
	if (isset($_SESSION['user_mail']) == true || isset($_SESSION['user_name']) == true) {
		echo "<meta http-equiv='refresh' content='0;url=index'>";
		exit;
	}
} else if ( strpos($_SERVER['REQUEST_URI'], "login") || strpos($_SERVER['REQUEST_URI'], "privacy") || strpos($_SERVER['REQUEST_URI'], "terms-of-use") || strpos($_SERVER['REQUEST_URI'], "password")) {
	// login, privacy, termofuse, password 주소에 들어온 상태일 경우 넘긴다.

} else {
	if (!isset($_SESSION['user_mail']) || $_SESSION['user_mail'] == "" || !isset($_SESSION['user_name']) || $_SESSION['user_name'] == "" || !isset($_SESSION['user_no']) || $_SESSION['user_no'] == "" || !isset($_SESSION['user_permission']) || $_SESSION['user_permission'] == "") {
		echo "<meta http-equiv='refresh' content='0;url=login'>";
		exit;
	} else {
		if ($_SESSION['user_ip'] != get_client_ip()) {
			echo "<script>alert('IP주소가 변경 되어, 다시 로그인 해주시길 바랍니다.');</script>";
			session_destroy();
			echo "<meta http-equiv='refresh' content='0;url=login'>";
			exit;
		}
		// 로그인시 표시
		//echo "".$_SESSION['user_mail']." | ".DecryptSession($_SESSION['user_name'], $_SESSION['user_mail'])." | ".DecryptSession($_SESSION['user_level'], $_SESSION['user_mail'])."";
	}
}
?>