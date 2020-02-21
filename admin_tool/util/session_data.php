<?php
session_start();
$user_mail = "";
$user_no = "";
$user_name = "";
if (!isset($_SESSION['user_mail']) || $_SESSION['user_mail'] == "" || !isset($_SESSION['user_name']) || $_SESSION['user_name'] == "" || !isset($_SESSION['user_no']) || $_SESSION['user_no'] == "") {
    // 로그인이 안되어 있을 경우
    $user_mail = "";
    $user_no = "";
    $user_name = "";
} else {
    $user_mail = $_SESSION['user_mail'];
    $user_no = DecryptSession($_SESSION['user_no'], $_SESSION['user_mail']);
    $user_name = DecryptSession($_SESSION['user_name'], $_SESSION['user_mail']);
}
?>