<?php
include("_common.php");
// include("../util/EncryptUtil.php");
?>

<script type="text/javascript">
    $(document).ready(function() {
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
        $("#changeMyAccount_btn").click(function() {
            // 비밀번호 체크
            if (passPassword == false) {
                alert("비밀번호가 서로 정확한지 확인하여 주세요.");
                return 0;
            }
            // 빈 텍스트 체크
            $("#changeMyAccount_frm").find("input[id='inputName']").each(function(index) {
                if ($(this).val() == '') {
                    alert("이름을 입력해 주세요.");
                    return 0;
                }
            });
            $("#changeMyAccount_frm").find("input[id='inputEmail']").each(function(index) {
                if ($(this).val() == '') {
                    alert("이메일를 입력해 주세요.");
                    return 0;
                }
            });
            $("#changeMyAccount_frm").find("input[id='inputPassword']").each(function(index) {
                if ($(this).val() == '') {
                    alert("비밀번호를 입력해 주세요.");
                    return 0;
                }
            });
            $("#changeMyAccount_frm").find("input[id='confirmPassword']").each(function(index) {
                if ($(this).val() == '') {
                    alert("비밀번호를 입력해 주세요.");
                    return 0;
                }
            });
            // 회원가입 처리
            var dataFromForm = $('#changeMyAccount_frm').serialize();
            $.ajax({
                type: "POST",
                data: dataFromForm,
                url: "./util/setting_password.php",
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

<div class="container" style="width:90%;">
    <div class="card card-login mx-auto mt-5" style="max-width:100%;">
        <div class="card-header">
            My Account Settings
        </div>
        <div class="card-body">
            <form id="changeMyAccount_frm">
                <div class="form-group">
                    <div class="form-label-group">
                        <label class="small mb-1" for="inputEmailAddress">Your email address</label>
                        <input type="email" id="inputEmail" name="inputEmail" class="form-control" placeholder="Your email address" required="required" value="<?php echo $_SESSION['user_mail']; ?>" readonly="readonly">
                    </div>
                </div>

                <div class="form-group">
                    <div class="form-label-group">
                        <label class="small mb-1" for="inputName">Your site nickname</label>
                        <input type="text" id="inputName" name="inputName" class="form-control" placeholder="Your site nickname" required="required" autofocus="autofocus" value="<?php echo DecryptSession($_SESSION['user_name'], $_SESSION['user_mail']); ?>" readonly="readonly">
                    </div>
                </div>

                <div class="form-group">
                    <div class="form-label-group">
                        <label class="small mb-1" for="inputOriginalPassword">Your original password</label>
                        <input type="password" id="inputOriginalPassword" name="inputOriginalPassword" class="form-control" placeholder="Your original password" required="required">
                    </div>
                </div>

                <div class="form-group">
                    <div class="form-row">
                        <div class="col-md-6">
                            <div class="form-label-group">
                                <label class="small mb-1" for="inputPassword">Password</label>
                                <input type="password" id="inputPassword" name="inputPassword" class="form-control" placeholder="Password" required="required">
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-label-group">
                                <label class="small mb-1" for="confirmPassword">Confirm Password</label>
                                <input type="password" id="confirmPassword" name="confirmPassword" class="form-control" placeholder="Confirm password" required="required">
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
                <button id="changeMyAccount_btn" type="button" class="btn btn-primary btn-block">Chnage MyAccount</button>
            </form>
        </div>
    </div>
</div>
<div style="padding-bottom:48px;"></div>