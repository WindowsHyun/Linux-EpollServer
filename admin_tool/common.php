<?php
session_start();
include("./util/EncryptUtil.php");		// 로그인 시 비밀번호 암호화 유틸리티
include("./util/session_data.php");
include("./util/define_text.php");
$Site_Title = "AdminTool";
$Site_URL = "http://localhost/admin_tool/";
$Site_Email = "admintool@localhost";
// if (!isset($_SERVER["HTTPS"])) {
// 	// https가 아닌경우 https로 이동 시켜 준다.
// 	if ($_SERVER['SERVER_NAME'] == "admintool.com") {
// 		$http_host = $_SERVER['HTTP_HOST'];
// 		$request_uri = $_SERVER['REQUEST_URI'];
// 		$url = 'https://' . $http_host . $request_uri;
// 		header("Location: " . $url . "");
// 	}
// }
?>