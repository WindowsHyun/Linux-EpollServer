<?php

$cookies = "";

function checkSite($url, $post, $referer, $port){
	//특정 주소 파싱하기.
	$agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko';
	global $cookies;
	$curlsession = curl_init();
	curl_setopt($curlsession, CURLOPT_URL, $url);
	curl_setopt($curlsession, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($curlsession, CURLOPT_POST, 1);
	curl_setopt($curlsession, CURLOPT_POSTFIELDS, $post);
	curl_setopt($curlsession, CURLOPT_USERAGENT, $agent);
	curl_setopt($curlsession, CURLOPT_REFERER, "" . $referer . ":" . $port . "/sess-bin/login_session.cgi");
	curl_setopt($curlsession, CURLOPT_TIMEOUT, 60); // 해당 웹사이트가 오래걸릴수 있으므로 2분동안 타임아웃 대기
	if ($cookies && $cookies !== "") {
		//echo "쿠키값 제공 : ".$cookies."\n";
		curl_setopt($curlsession, CURLOPT_COOKIE, $cookies);
	}
	curl_setopt($curlsession, CURLOPT_HEADER, 1);		// 쿠키값 가져오기
	$buffer = curl_exec($curlsession);
	$http_code = curl_getinfo($curlsession, CURLINFO_HTTP_CODE);
	curl_close($curlsession);
	if ($http_code != "200") {
		return false;
	}else{
		return true;
	}
}

function post_data($url, $post, $referer, $port) {
	//특정 주소 파싱하기.
	$agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko';
	global $cookies;
	$curlsession = curl_init ();
	curl_setopt ($curlsession, CURLOPT_URL, $url);
	curl_setopt ($curlsession, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt ($curlsession, CURLOPT_POST, 1);
	curl_setopt ($curlsession, CURLOPT_POSTFIELDS, $post);
	curl_setopt ($curlsession, CURLOPT_USERAGENT, $agent);
	curl_setopt ($curlsession, CURLOPT_REFERER, "".$referer.":".$port."/sess-bin/login_session.cgi");
	curl_setopt ($curlsession, CURLOPT_TIMEOUT, 30); // 해당 웹사이트가 오래걸릴수 있으므로 2분동안 타임아웃 대기
	if ( $cookies && $cookies !== ""){
		//echo "쿠키값 제공 : ".$cookies."\n";
		curl_setopt ($curlsession, CURLOPT_COOKIE, $cookies);
	}
	curl_setopt ($curlsession, CURLOPT_HEADER, 1);		// 쿠키값 가져오기
	$buffer = curl_exec ($curlsession);
	$http_code = curl_getinfo( $curlsession, CURLINFO_HTTP_CODE );
	curl_close($curlsession);
	if(  $http_code != "200" ){
		return false;
	}
	// if ($cinfo['http_code'] != 200){
	// 	return $cinfo['http_code'];
	// }

	if ( stripos($buffer, "setCookie") !== false){
		// 쿠키 값이 없으면 쿠키값을 생성 한다.
		$tmp = explode("setCookie('", $buffer );
		$tmp = explode("');",$tmp[1]);
		$cookies =  "efm_session_id=".$tmp[0]."" ; 
	}
	return $buffer;
}

function get_data($url, $referer, $port) {
	//특정 주소 파싱하기.
	$agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko';
	global $cookies;
	$curlsession = curl_init ();
	curl_setopt ($curlsession, CURLOPT_URL, $url);
	curl_setopt ($curlsession, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt ($curlsession, CURLOPT_POST, 0);
	curl_setopt ($curlsession, CURLOPT_USERAGENT, $agent);
	curl_setopt ($curlsession, CURLOPT_REFERER, "".$referer.":".$port."");
	curl_setopt ($curlsession, CURLOPT_TIMEOUT, 120); // 해당 웹사이트가 오래걸릴수 있으므로 2분동안 타임아웃 대기
	if ( $cookies && $cookies !== ""){
		//echo "쿠키값 제공 : ".$cookies."\n";
		curl_setopt ($curlsession, CURLOPT_COOKIE, $cookies);
	}
	curl_setopt ($curlsession, CURLOPT_HEADER, 1);		// 쿠키값 가져오기
	$buffer = curl_exec ($curlsession);
	$cinfo = curl_getinfo($curlsession);
	curl_close($curlsession);

	if ($cinfo['http_code'] != 200){
		return $cinfo['http_code'];
	}

	if ( stripos($buffer, "setCookie") !== false){
		// 쿠키 값이 없으면 쿠키값을 생성 한다.
		$tmp = explode("setCookie('", $buffer );
		$tmp = explode("');",$tmp[1]);
		$cookies =  "efm_session_id=".$tmp[0]."" ; 
	}
	return $buffer;
}

function get_content($userdata,$onedata, $twodata) {
	//이미 구해진 부분에서 특정 부분만 가져오기
	$tmp = explode($onedata, $userdata );
	$tmp = explode($twodata,$tmp[1]);
	$buffer = $tmp[0]; // $data 부분에는 원하는 부분만 들어가겠죠
	return $buffer;
}

?>