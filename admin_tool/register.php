<!DOCTYPE html>
<html lang="kr">
<?php include("./head.php"); ?>

<script type="text/javascript">
    $(document).ready(function() {
        // 이메일 주소 중복 체크
        var passEmail = false;
        $("#inputEmailAddress").focusout(function(event) {
            var emailVal = $("#inputEmailAddress").val();
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
        $("#inputConfirmPassword").keyup(function(event) {
            if ($("#inputPassword").val() == $("#inputConfirmPassword").val()) {
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
            $("#register_frm").find("input[id='inputEmailAddress']").each(function(index) {
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
            $("#register_frm").find("input[id='inputConfirmPassword']").each(function(index) {
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

<body class="bg-primary">
    <div id="layoutAuthentication">
        <div id="layoutAuthentication_content">
            <main>
                <div class="container">
                    <div class="row justify-content-center">
                        <div class="col-lg-7">
                            <div class="card shadow-lg border-0 rounded-lg mt-5">
                                <div class="card-header">
                                    <h3 class="text-center font-weight-light my-4">Create Account</h3>
                                </div>
                                <div class="card-body">
                                    <form>
                                        <div class="form-group">
                                            <label class="small mb-1" for="inputName">NickName</label>
                                            <input class="form-control py-4" id="inputName" type="text" aria-describedby="emailHelp" placeholder="Enter nickname" />
                                        </div>
                                        <div class="text-left mb-4">
                                            <p id="nameconsole" style="padding-left:5px;color:#dc3545; font-size:90%"></p>
                                        </div>
                                        <div class="form-group">
                                            <label class="small mb-1" for="inputEmailAddress">Email</label>
                                            <input class="form-control py-4" id="inputEmailAddress" type="email" aria-describedby="emailHelp" placeholder="Enter Email Address" />
                                        </div>
                                        <div class="text-left mb-4">
                                            <p id="emailconsole" style="padding-left:5px;color:#dc3545; font-size:90%"></p>
                                        </div>
                                        <div class="form-group">
                                            <label class="small mb-1" for="inputRegisterCode">Register Code</label>
                                            <input class="form-control py-4" id="inputRegisterCode" type="text" aria-describedby="emailHelp" placeholder="Enter Register Code" />
                                        </div>
                                        <div class="form-row">
                                            <div class="col-md-6">
                                                <div class="form-group">
                                                    <label class="small mb-1" for="inputPassword">Password</label>
                                                    <input class="form-control py-4" id="inputPassword" type="password" placeholder="Enter Password" />
                                                </div>
                                            </div>
                                            <div class="col-md-6">
                                                <div class="form-group">
                                                    <label class="small mb-1" for="inputConfirmPassword">Confirm Password</label>
                                                    <input class="form-control py-4" id="inputConfirmPassword" type="password" placeholder="Confirm Password" />
                                                </div>
                                            </div>
                                        </div>
                                        <div class="text-center mb-4">
                                            <p id="passwordconsole" style="padding-left:5px;color:#dc3545; font-size:90%"></p>
                                        </div>
                                        <div class="text-center mb-4">
                                            <p id="console" style="color:#007bff; font-size:90%"></p>
                                        </div>
                                        <div class="form-group mt-4 mb-0">
                                            <button id="register_btn" type="button" class="btn btn-primary btn-block">Create Account</button>
                                        </div>
                                    </form>
                                </div>
                                <div class="card-footer text-center">
                                    <div class="small"><a href="login">Have an account? Go to login</a></div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </main>
        </div>
        <div id="layoutAuthentication_footer">
            <?php include("./tail.php"); ?>
        </div>
    </div>
</body>

</html>