<?php
	include("./db_config.php");
	// 입력 값 확인
	if( !isset($_POST['chkMail']) ){
		echo "비정상 적인 접근 입니다.";
		exit;
	}

	// 공백 입력 확인
	if ( $_POST['chkMail'] == '' ){
		echo "비정상 적인 접근 입니다.";
		exit;
	}

	// Referer 체크
	if ( strpos($_SERVER['HTTP_REFERER'] , "register") == false){
		echo "비정상 적인 접근 입니다.";
		exit;
	}

	$chk_mail = $_POST['chkMail'];

	$sql = sprintf("SELECT mail FROM ".$mysql_member_login_table ." WHERE mail = \"%s\"",
                    $mysqli->real_escape_string($chk_mail) );
	$result = $mysqli->query($sql);
	$row = mysqli_fetch_assoc($result);

	if($row['mail'] != '') {
		echo "중복 된 이메일 주소 입니다. false";
	}else{
		echo "사용하셔도 좋은 이메일 주소 입니다. true";
	}
?>