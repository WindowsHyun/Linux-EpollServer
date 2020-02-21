<!DOCTYPE html>
<html lang="en">
<?php
include("head.php");
?>

<script type="text/javascript">
  $(document).ready(function() {
    // 이메일 정상 유무 처리
    var passEmail = false;
    $("#inputEmail").focusout(function(event) {
      var emailVal = $("#inputEmail").val();
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
    $("#inputEmail").keydown(function(event) {
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
      $("#resetPassword_frm").find("input[id='inputEmail']").each(function(index) {
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

<body class="bg-dark">

  <div class="container">
    <div class="card card-login mx-auto mt-5">
      <div class="card-header">Reset Password</div>
      <div class="card-body">
        <div class="text-center mb-4">
          <h4>Forgot your password?</h4>
          <p>Enter your email address and we will send you instructions on how to reset your password.</p>
        </div>
        <form id="resetPassword_frm">
          <div class="form-group">
            <div class="form-label-group">
              <input type="email" id="inputEmail" name="inputEmail" class="form-control" placeholder="Enter email address" required="required" autofocus="autofocus">
              <label for="inputEmail">Enter email address</label>
            </div>
          </div>
          <div class="text-center mb-4">
            <p id="console" style="color:#007bff; font-size:90%"></p>
          </div>
          <button id="resetPassword_btn" type="button" class="btn btn-primary btn-block">Reset Password</button>
        </form>
        <div class="text-center">
          <a class="d-block small mt-3" href="register">Register an Account</a>
          <a class="d-block small" href="login">Login Page</a>
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