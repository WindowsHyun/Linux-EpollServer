<?php
session_start();
$user_mail = "";
$user_no = "";
$user_name = "";
$user_permission = "";

if (!isset($_SESSION['user_mail']) || $_SESSION['user_mail'] == "" || !isset($_SESSION['user_name']) || $_SESSION['user_name'] == "" || !isset($_SESSION['user_no']) || $_SESSION['user_no'] == "" || !isset($_SESSION['user_permission']) || $_SESSION['user_permission'] == "") {
    // 로그인이 안되어 있을 경우
    $user_mail = "";
    $user_no = "";
    $user_name = "";
    $user_permission = "";
} else {
    $user_mail = $_SESSION['user_mail'];
    $user_no = DecryptSession($_SESSION['user_no'], $_SESSION['user_mail']);
    $user_name = DecryptSession($_SESSION['user_name'], $_SESSION['user_mail']);
    $user_permission = DecryptSession($_SESSION['user_permission'], $_SESSION['user_mail']);
}
?>