<!DOCTYPE html>
<html lang="en">
<?php
include("head.php");
// 입력 값 확인
if (!isset($_GET['token']) || !isset($_GET['whdb'])) {
  print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
  exit();
}

if($_GET['token'] == "" || $_GET['whdb'] == ""){
  print "<script language=javascript> alert('비정상 적인 접근 입니다.'); location.replace('login'); </script>";
  exit();
}

$token = $_GET['token'];
$SessionTime = "" . date("Y-m-d") . "";
$user_mail = DecryptSession($_GET['whdb'], $SessionTime);

$check_email = preg_match("/^[_\.0-9a-zA-Z-]+@([0-9a-zA-Z][0-9a-zA-Z-]+\.)+[a-zA-Z]{2,6}$/i", $user_mail);
if ($check_email == false) {
  print "<script language=javascript> alert('비밀번호 변경 페이지 세션이 만료되어, 다시 한번 비밀번호 찾기 과정을 진행 부탁드립니다.'); location.replace('login'); </script>";
  exit();
}
?>

<script type="text/javascript">
  $(document).ready(function() {
    // 비밀번호 서로 동일한지 확인 처리
    var passPassword = false;
    $("#confirmPassword").keyup(function(event) {
      if ($("#inputPassword").val() == $("#confirmPassword").val()) {
        $("#console").html("비밀번호를 정확하게 입력 하였습니다.");
        var x = document.getElementById("console");
        x.style.color = "#007bff";
        passPassword = true;
      } else {
        $("#console").html("비밀번호가 서로 다릅니다.");
        var x = document.getElementById("console");
        x.style.color = "#dc3545";
        passPassword = false;
      }
    });
    // Enter 입력시 로그인 처리
    $("#inputPassword").keydown(function(event) {
      if (event.keyCode == 13) {
        $("#resetPassword_btn").trigger("click");
      }
    });
    $("#confirmPassword").keydown(function(event) {
      if (event.keyCode == 13) {
        $("#resetPassword_btn").trigger("click");
      }
    });
    // 비밀번호 초기화 버튼 클릭시 처리
    $("#resetPassword_btn").click(function() {
      if (passPassword == false) {
        alert("비밀번호가 서로 다릅니다.");
        exit();
      }
      $("#resetPassword_frm").find("input[id='confirmPassword']").each(function(index) {
        if ($(this).val() == '') {
          $("#console").html("Please enter password correctly.");
          exit();
        }
      });
      $("#resetPassword_frm").find("input[id='inputPassword']").each(function(index) {
        if ($(this).val() == '') {
          $("#console").html("Please enter password correctly.");
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
        url: "./util/change_password.php",
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
          <h4>Reset your password</h4>
        </div>
        <form id="resetPassword_frm">
          <div class="form-group">
          <input type="id" id="LoginID" name="LoginID" class="form-control" required="required" value =<?php echo $user_mail; ?> hidden>
          <input type="id" id="Token" name="Token" class="form-control" required="required" value =<?php echo $token; ?> hidden>
            <div class="form-label-group">
              <input type="password" id="inputPassword" name="inputPassword" class="form-control" placeholder="Enter password" required="required" autofocus="autofocus">
              <label for="inputPassword">Enter password</label>
            </div>
          </div>
          <div class="form-group">
            <div class="form-label-group">
              <input type="password" id="confirmPassword" name="confirmPassword" class="form-control" placeholder="Confirm password" required="required" autofocus="autofocus">
              <label for="confirmPassword">Confirm password</label>
            </div>
          </div>
          <div class="text-center mb-4">
            <p id="console" style="color:#007bff; font-size:90%"></p>
          </div>
          <button id="resetPassword_btn" type="button" class="btn btn-primary btn-block">Reset Password</button>
        </form>
        <!-- <div class="text-center">
          <a class="d-block small mt-3" href="register">Register an Account</a>
          <a class="d-block small" href="login">Login Page</a>
        </div> -->
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