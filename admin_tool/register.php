<!DOCTYPE html>
<html lang="en">

<?php
include("head.php");
include("./util/login_session.php");
?>
<script type="text/javascript">
    $(document).ready(function() {
        // 이메일 주소 중복 체크
        var passEmail = false;
        $("#inputEmail").focusout(function(event) {
            var emailVal = $("#inputEmail").val();
            var exptext = /^[A-Za-z0-9_\.\-]+@[A-Za-z0-9\-]+\.[A-Za-z0-9\-]+/;
            if (emailVal.match(exptext) != null) {
                emailVal = "chkMail=" + emailVal;
                $.ajax({
                    type: "POST",
                    data: emailVal,
                    url: "./util/email_process.php",
                    success: function(data) {
                        if (data.includes("true") == true) {
                            $("#emailconsole").html(data.replace(' true', ''));
                            var x = document.getElementById("emailconsole");
                            x.style.color = "#007bff";
                            passEmail = true;
                        } else {
                            $("#emailconsole").html(data.replace(' false', ''));
                            var x = document.getElementById("emailconsole");
                            x.style.color = "#dc3545";
                            passEmail = false;
                        }
                    },
                    error: function(data) {
                        alert("Error : " + data);
                    }
                });
            } else {
                $("#emailconsole").html("");
            }
        });

        // 닉네임 주소 중복 체크
        var passName = false;
        $("#inputName").focusout(function(event) {
            var nickNamelVal = $("#inputName").val();
            nickNamelVal = "chkName=" + nickNamelVal;
            $.ajax({
                type: "POST",
                data: nickNamelVal,
                url: "./util/name_process.php",
                success: function(data) {
                    if (data.includes("true") == true) {
                        $("#nameconsole").html(data.replace(' true', ''));
                        var x = document.getElementById("nameconsole");
                        x.style.color = "#007bff";
                        passName = true;
                    } else {
                        $("#nameconsole").html(data.replace(' false', ''));
                        var x = document.getElementById("nameconsole");
                        x.style.color = "#dc3545";
                        passName = false;
                    }
                },
                error: function(data) {
                    alert("Error : " + data);
                }
            });
        });

        // 비밀번호 동일한지 체크
        var passPassword = false;
        $("#confirmPassword").keyup(function(event) {
            if ($("#inputPassword").val() == $("#confirmPassword").val()) {
                $("#passwordconsole").html("비밀번호를 정확하게 입력 하였습니다.");
                var x = document.getElementById("passwordconsole");
                x.style.color = "#007bff";
                passPassword = true;
            } else {
                $("#passwordconsole").html("비밀번호가 서로 다릅니다.");
                var x = document.getElementById("passwordconsole");
                x.style.color = "#dc3545";
                passPassword = false;
            }
        });
        // 회원가입 버튼 클릭 시
        $("#register_btn").click(function() {
            // 이메일, 비밀번호 체크
            if (passEmail == false) {
                alert("이메일 중복여부를 확인하여 주세요.");
                exit();
            }
            if (passPassword == false) {
                alert("비밀번호가 서로 정확한지 확인하여 주세요.");
                exit();
            }
            if (passName == false) {
                alert("닉네임 중복여부를 확인하여 주세요.");
                exit();
            }
            // 빈 텍스트 체크
            $("#register_frm").find("input[id='inputName']").each(function(index) {
                if ($(this).val() == '') {
                    alert("이름을 입력해 주세요.");
                    exit();
                }
            });
            $("#register_frm").find("input[id='inputEmail']").each(function(index) {
                if ($(this).val() == '') {
                    alert("이메일를 입력해 주세요.");
                    exit();
                }
            });
            $("#register_frm").find("input[id='inputRegisterCode']").each(function(index) {
                if ($(this).val() == '') {
                    alert("가입 코드를 입력해 주세요.");
                    exit();
                }
            });
            $("#register_frm").find("input[id='inputPassword']").each(function(index) {
                if ($(this).val() == '') {
                    alert("비밀번호를 입력해 주세요.");
                    exit();
                }
            });
            $("#register_frm").find("input[id='confirmPassword']").each(function(index) {
                if ($(this).val() == '') {
                    alert("비밀번호를 입력해 주세요.");
                    exit();
                }
            });
            // 회원가입 처리
            var dataFromForm = $('#register_frm').serialize();
            $.ajax({
                type: "POST",
                data: dataFromForm,
                url: "./util/register_process.php",
                success: function(data) {
                    $("#console").html(data);
                },
                error: function(data) {
                    alert("Error : " + data);
                }
            });

        });
    });
</script>

<body class="bg-dark">
    <div class="container">
        <div class="card card-register mx-auto mt-5">
            <div class="card-header">Register an Account</div>
            <div class="card-body">
                <form id="register_frm">
                    <div class="form-group">
                        <div class="form-label-group">
                            <input type="name" id="inputName" name="inputName" class="form-control" placeholder="NickName" required="required">
                            <label for="inputName">NickName</label>
                        </div>
                        <div class="text-left mb-4">
                            <p id="nameconsole" style="padding-left:5px;color:#dc3545; font-size:90%"></p>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="form-label-group">
                            <input type="email" id="inputEmail" name="inputEmail" class="form-control" placeholder="Email address" required="required">
                            <label for="inputEmail">Email address</label>
                        </div>
                        <div class="text-left mb-4">
                            <p id="emailconsole" style="padding-left:5px;color:#dc3545; font-size:90%"></p>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="form-label-group">
                            <input type="text" id="inputRegisterCode" name="inputRegisterCode" class="form-control" placeholder="Register Code" required="required">
                            <label for="inputRegisterCode">Register Code</label>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="form-row">
                            <div class="col-md-6">
                                <div class="form-label-group">
                                    <input type="password" id="inputPassword" name="inputPassword" class="form-control" placeholder="Password" required="required">
                                    <label for="inputPassword">Password</label>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-label-group">
                                    <input type="password" id="confirmPassword" name="confirmPassword" class="form-control" placeholder="Confirm password" required="required">
                                    <label for="confirmPassword">Confirm password</label>
                                </div>
                            </div>
                        </div>
                        <div class="text-center mb-4">
                            <p id="passwordconsole" style="padding-left:5px;color:#dc3545; font-size:90%"></p>
                        </div>
                    </div>
                    <div class="text-center mb-4">
                        <p id="console" style="color:#007bff; font-size:90%"></p>
                    </div>
                    <button id="register_btn" type="button" class="btn btn-primary btn-block">Register</button>
                </form>
                <div class="text-center">
                    <a class="d-block small mt-3" href="login">Login Page</a>
                    <a class="d-block small" href="password">Forgot Password?</a>
                </div>
            </div>
        </div>
    </div>

    <!-- Bootstrap core JavaScript-->
    <script src="vendor/jquery/jquery.min.js"></script>
    <script src="vendor/bootstrap/js/bootstrap.bundle.min.js"></script>

    <!-- Core plugin JavaScript-->
    <script src="vendor/jquery-easing/jquery.easing.min.js"></script>

</body>

</html>