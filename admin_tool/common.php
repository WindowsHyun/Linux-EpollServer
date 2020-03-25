<?php
session_start();
include(__DIR__ . "/util/db_config.php");
include(__DIR__ . "/util/EncryptUtil.php");        // 로그인 시 비밀번호 암호화 유틸리티
include(__DIR__ . "/util/session_data.php");
include(__DIR__ . "/util/login_session.php");
include(__DIR__ . "/util/define.php");
include(__DIR__ . "/util/define_text.php");
$Site_Title = "AdminTool";
$Site_URL = "http://localhost";
$Site_Email = "admintool@localhost";
?>