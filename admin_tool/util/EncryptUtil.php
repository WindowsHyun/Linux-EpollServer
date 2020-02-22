<?php
function Encrypt($str, $secret_iv)
{
    $secret_key = "WindowsHyun";
    $key = hash('sha256', $secret_key);
    $iv = substr(hash('sha256', $secret_iv, true), 0, 16);
    return str_replace("=", "", base64_encode(openssl_encrypt($str, "AES-256-CBC", $key, OPENSSL_RAW_DATA, $iv)));
}

function EncryptSession($str, $secret_iv)
{
    $secret_key = "secret.windowshyun.com";
    $key = hash('sha256', $secret_key);
    $iv = substr(hash('sha256', $secret_iv), 0, 16);

    return str_replace("=", "", base64_encode(openssl_encrypt($str, "AES-256-CBC", $key, 0, $iv)));
}

function DecryptSession($str, $secret_iv)
{
    $secret_key = "secret.windowshyun.com";
    $key = hash('sha256', $secret_key);
    $iv = substr(hash('sha256', $secret_iv), 0, 16);

    return openssl_decrypt(base64_decode($str), "AES-256-CBC", $key, 0, $iv);
}
?>