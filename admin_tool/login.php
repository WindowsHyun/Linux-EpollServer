<!doctype html>
<html lang="kr">
<?php include("head.php"); ?>

<script type="text/javascript">
    $(document).ready(function() {
        // Enter 입력시 로그인 처리
        $("#inputEmailAddress").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#login_btn").trigger("click");
            }
        });
        $("#inputPassword").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#login_btn").trigger("click");
            }
        });
        // Login 버튼 클릭시 처리
        $("#login_btn").click(function() {
            $("#login_frm").find("input[id='inputEmailAddress']").each(function(index) {
                if ($(this).val() == '') {
                    $("#console").html("아이디를 입력해 주세요.");
                }
            });
            $("#login_frm").find("input[id='inputPassword']").each(function(index) {
                if ($(this).val() == '') {
                    $("#console").html("패스워드를 입력해 주세요.");
                }
            });
            // setVisible('#loading', true);
            var btn = document.getElementById("login_btn");
            btn.style.backgroundColor = "#6c757d";
            btn.style.borderColor = "#6c757d";
            // 로그인 Post 처리
            var dataFromForm = $('#login_frm').serialize();
            $.ajax({
                type: "POST",
                data: dataFromForm,
                url: "./util/login_process.php",
                success: function(data) {
                    $("#console").html(data);
                    btn.style.backgroundColor = "#007bff";
                    btn.style.borderColor = "#007bff";
                    setVisible('#loading', false);
                },
                error: function(data) {
                    setVisible('#loading', false);
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
                                    <h3 class="text-center font-weight-light my-4">Login</h3>
                                </div>
                                <div class="card-body">
                                    <form id="login_frm">
                                        <div class="form-group"><label class="small mb-1" for="inputEmailAddress">Email</label><input class="form-control py-4" id="inputEmailAddress" name="inputEmailAddress" type="email" placeholder="Enter email address" /></div>
                                        <div class="form-group"><label class="small mb-1" for="inputPassword">Password</label><input class="form-control py-4" id="inputPassword" name="inputPassword" type="password" placeholder="Enter password" /></div>
                                        <div class="form-group">
                                            <div class="custom-control custom-checkbox"><input class="custom-control-input" id="rememberPasswordCheck" type="checkbox" /><label class="custom-control-label" for="rememberPasswordCheck">Remember password</label></div>
                                        </div>
                                        <div class="text-center mb-4">
                                            <p id="console" style="color:#007bff; font-size:90%"></p>
                                        </div>
                                        <div class="form-group d-flex align-items-center justify-content-between mt-4 mb-0"><a class="small" href="password">Forgot Password?</a><button id="login_btn" type="button" class="btn btn-primary">Login</button></div>
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