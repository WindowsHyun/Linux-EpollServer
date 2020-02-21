<!doctype html>
<html lang="kr">
<?php include("./head.php"); ?>

<script type="text/javascript">
    $(document).ready(function() {
        // Enter 입력시 로그인 처리
        $("#inputEmail").keydown(function(event) {
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
            $("#login_frm").find("input[id='inputEmail']").each(function(index) {
                if ($(this).val() == '') {
                    $("#console").html("아이디를 입력해 주세요.");
                }
            });
            $("#login_frm").find("input[id='inputPassword']").each(function(index) {
                if ($(this).val() == '') {
                    $("#console").html("패스워드를 입력해 주세요.");
                }
            });
            setVisible('#loading', true);
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
                    alert("Error : " + data);
                    setVisible('#loading', false);
                }
            });
        });
    });
</script>

<body class="bg-dark">
    <div class="container">
        <div class="card card-login mx-auto mt-5">
            <div class="card-header">
                Login
            </div>
            <div class="card-body">
                <form id="login_frm">
                    <div class="form-group">
                        <div class="form-label-group">
                            <input type="email" id="inputEmail" name="inputEmail" class="form-control" placeholder="Email address" required="required" autofocus="autofocus" value="">
                            <label for="inputEmail">Email address</label>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="form-label-group">
                            <input type="password" id="inputPassword" name="inputPassword" class="form-control" placeholder="Password" required="required" onkeydown=''>
                            <label for="inputPassword">Password</label>
                        </div>
                    </div>
                </form>
                <div class="text-center mb-4">
                    <p id="console" style="color:#007bff; font-size:90%"></p>
                </div>
                <button id="login_btn" type="button" class="btn btn-primary btn-block">Login</button>
                <div class="text-center">
                    <!-- <a class="d-block small mt-3" href="register">Register an Account</a>
                    <a class="d-block small" href="password">Forgot Password?</a> -->
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