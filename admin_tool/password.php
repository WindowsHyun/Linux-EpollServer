<!DOCTYPE html>
<html lang="kr">
<?php
include("head.php");
?>

<script type="text/javascript">
    $(document).ready(function() {
        // 이메일 정상 유무 처리
        var passEmail = false;
        $("#inputEmailAddress").focusout(function(event) {
            var emailVal = $("#inputEmailAddress").val();
            if (emailVal != "") {
                var exptext = /^[A-Za-z0-9_\.\-]+@[A-Za-z0-9\-]+\.[A-Za-z0-9\-]+/;
                if (emailVal.match(exptext) == null) {
                    $("#console").html("잘못된 이메일 형식입니다.");
                    passEmail = false;
                } else {
                    $("#console").html("");
                    passEmail = true;
                }
            }
        });
        // Enter 입력시 로그인 처리
        $("#inputEmailAddress").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#resetPassword_btn").trigger("click");
            }
        });
        // 비밀번호 초기화 버튼 클릭시 처리
        $("#resetPassword_btn").click(function() {
            if (passEmail == false) {
                alert("잘못된 이메일 형식입니다.");
                exit();
            }
            $("#resetPassword_frm").find("input[id='inputEmailAddress']").each(function(index) {
                if ($(this).val() == '') {
                    $("#console").html("Please enter your email correctly.");
                    exit();
                }
            });
            var btn = document.getElementById("resetPassword_btn");
            btn.style.backgroundColor = "#6c757d";
            btn.style.borderColor = "#6c757d";
            // 로그인 Post 처리
            var dataFromForm = $('#resetPassword_frm').serialize();
            $.ajax({
                type: "POST",
                data: dataFromForm,
                url: "./util/forgot_password.php",
                success: function(data) {
                    $("#console").html(data);
                    btn.style.backgroundColor = "#007bff";
                    btn.style.borderColor = "#007bff";
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
                        <div class="col-lg-5">
                            <div class="card shadow-lg border-0 rounded-lg mt-5">
                                <div class="card-header">
                                    <h3 class="text-center font-weight-light my-4">Password Recovery</h3>
                                </div>
                                <div class="card-body">
                                    <div class="small mb-3 text-muted">Enter your email address and we will send you a link to reset your password.</div>
                                    <form id="resetPassword_frm">
                                        <div class="form-group">
                                            <label class="small mb-1" for="inputEmailAddress">Email</label>
                                            <input class="form-control py-4" id="inputEmailAddress" type="email" aria-describedby="emailHelp" placeholder="Enter email address" />
                                        </div>
                                        <div class="form-group d-flex align-items-center justify-content-between mt-4 mb-0">
                                            <a class="small" href="login">Return to login</a>
                                            <button id="resetPassword_btn" type="button" class="btn btn-primary">Reset Password</button>
                                        </div>
                                    </form>
                                </div>
                                <div class="card-footer text-center">
                                    <div class="small"><a href="register">Need an account? Sign up!</a></div>
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