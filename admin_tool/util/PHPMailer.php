<?php
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;
class mailer {
    public function sendMail($email, $subject, $message)
    {
        // 값이 준비 안되어 있을 경우
        if ( !isset($email) || !isset($subject) || !isset($message) ){
            exit();
        }else if ( $email == "" || $subject == "" || $message == "" ){
            exit();
        }
        require('./PHPMailer/src/PHPMailer.php');
        require('./PHPMailer/src/SMTP.php');
        require('./PHPMailer/src/Exception.php');
        $mail = new PHPMailer(true);
        $mail->isSMTP();
        $mail->SMTPDebug = 0;
        $mail->SMTPAuth = true;
        $mail->Host = "smtp.office365.com";             // Office365 smtp 서버
	    $mail->SMTPAuth = true;                         // SMTP 인증을 사용함
	    $mail->Username = "이메일주소";                 // 메일 계정 (지메일일경우 지메일 계정)
	    $mail->Password = "비밀번호";                   // 메일 비밀번호
	    $mail->SMTPSecure = "STARTTLS";                 // STARTTLS 사용함
	    $mail->Port = 587;                              // email 보낼때 사용할 포트를 지정
	    $mail->CharSet = "utf-8";                       // 문자셋 인코딩
        $mail->addAddress($email);
        $mail->setFrom('발송자 이메일', '발송자 닉네임');
        //$mail->addReplyTo("email_to@gmail.com", "Alias");
        $mail->Subject = $subject;
        $mail->msgHTML($message);
        $mail->send();
    }
}
?>