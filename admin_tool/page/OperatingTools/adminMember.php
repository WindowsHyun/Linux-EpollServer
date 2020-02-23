<?php
include("../page/common.php");
include("../../util/db_config.php");
include("../../util/define_text.php");
include("../../util/define.php");
// 관리자 권한 체크 해야함.

// menu DB 불러오기
$sql = "SELECT * FROM `" . $mysql_database . "`.`" . $mysql_login_table . "`";
$result = $mysqli->query($sql);
$adminMemberArr = array();
while ($row = mysqli_fetch_assoc($result)) {
    $memberData = array();
    $memberData['no'] = $row['no'];
    $memberData['name'] = $row['name'];
    $memberData['mail'] = $row['mail'];
    $memberData['permission'] = $row['permission'];
    $memberData['lastLoginIP'] = $row['lastLoginIP'];
    $memberData['lastLogin'] = $row['lastLogin'];
    $adminMemberArr[] = $memberData;
}
?>
<!-- Modal 사용하려면 필수 -->
<script type="text/javascript" src="vendor/jquery/jquery.js"></script>
<script type="text/javascript" src="vendor/datatables/jquery.dataTables.js"></script>
<script type="text/javascript" src="vendor/bootstrap/js/bootstrap.bundle.min.js"></script>
<!-- Modal 사용하려면 필수 -->
<script type="text/javascript">

</script>

<!-- Breadcrumbs-->
<ol class="breadcrumb">
    <li class="breadcrumb-item"><?= $Site_Title ?></li>
    <li class="breadcrumb-item">운영도구</li>
    <li class="breadcrumb-item active">운영자 아이디 관리</li>
</ol>

<!-- DataTables Example -->
<div class="card mb-3">
    <div class="card-header">
        <i class="fas fa-table"></i>
        운영자 아이디 관리</div>
    <div class="card-body">
        <div class="table-responsive">
            <table class="table table-striped table-bordered display nowrap" id="adminDataTable" width="100%" cellspacing="0">
                <thead>
                    <tr style="text-align: center; word-break: keep-all;">
                        <th>No</th>
                        <th>이름</th>
                        <th>이메일</th>
                        <th>비밀번호</th>
                        <th>권한</th>
                        <th>IP</th>
                        <th>접속 시간</th>
                        <th>제어</th>
                    </tr>
                </thead>
                <tbody>
                    <?php foreach ($adminMemberArr as $data) { ?>
                        <tr style="text-align: center;" id="fixAdminMember_frm_<?= $data['no']; ?>">
                            <td>
                                <?= $data['no']; ?>
                                <input name="member_no" id="member_no" type="text" value="<?= $data['no']; ?>" hidden />
                            </td>
                            <td>
                                <?= $data['name']; ?>
                                <input name="member_name" id="member_name" type="text" value="<?= $data['name']; ?>" hidden />
                            </td>
                            <td>
                                <?= $data['mail']; ?>
                                <input name="member_mail" id="member_mail" type="text" value="<?= $data['mail']; ?>" hidden />
                            </td>
                            <td>
                                <input name="member_pwd" id="member_pwd" type="password" value="" style="width:100%;" />
                            </td>
                            <td>
                                <?= $data['permission']; ?>
                            </td>
                            <td>
                                <?= $data['lastLoginIP']; ?>
                            </td>
                            <td>
                                <?= $data['lastLogin']; ?>
                            </td>
                            <td>
                                <button type="button" onclick="editFixMenu('<?= $data['no']; ?>');" class="btn btn-primary btn-sm">수정</button>&nbsp;<button type="button" id="delMenu_btn" onclick="deleteAdminMember('<?= $data['no']; ?>', '<?= $data['mail']; ?>');" class="btn btn-danger btn-sm">삭제</button>
                            </td>
                        </tr>
                    <?php } ?>
                </tbody>
            </table>
        </div>
    </div>
    <div class="card-footer small text-muted">
        <form id="addMember_frm">
            <div class="table-responsive">
                <table class="table table-striped table-bordered table-sm" id="addDataTable" width="100%" cellspacing="0">
                    <thead>
                        <tr style="text-align: center; word-break: keep-all;">
                            <th>이름</th>
                            <th>이메일</th>
                            <th>비밀번호</th>
                            <th>권한</th>
                            <th>제어</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr style="text-align: center;">
                            <td><input name="member_add_name" id="member_add_name" type="text" value="" style="width:100%;" /></td>
                            <td><input name="member_add_email" id="member_add_email" type="text" value="" style="width:100%;" /></td>
                            <td><input name="member_add_pwd" id="member_add_pwd" type="password" value="" style="width:100%;" /></td>
                            <td><input name="member_add_permission" id="member_add_permission" type="text" value="" style="width:100%;" /></td>
                            <td><button id="addMenu_btn" type="button" class="btn btn-success btn-sm">추가</button></td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </form>
    </div>
</div>

<p class="small text-center text-muted my-5">
    <em>
        <p id="console" style="color:#007bff; font-size:90%"></p>
    </em>
    <em>More table examples coming soon...</em><br>
</p>

<script type="text/javascript">
    var doubleSubmitFlag = false;
    $(document).ready(function() {
        // Enter 입력시 로그인 처리
        $("#member_add_name").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#addMenu_btn").trigger("click");
            }
        });
        $("#member_add_email").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#addMenu_btn").trigger("click");
            }
        });
        $("#member_add_pwd").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#addMenu_btn").trigger("click");
            }
        });
        $("#member_add_permission").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#addMenu_btn").trigger("click");
            }
        });

        // 추가 버튼 클릭시 처리
        if (doubleSubmitFlag == false) {
            $("#addMenu_btn").click(function() {
                doubleSubmitFlag = true;
                setVisible('#loading', true);
                var btn = document.getElementById("addMenu_btn");
                btn.style.backgroundColor = "#6c757d";
                btn.style.borderColor = "#6c757d";
                // 로그인 Post 처리
                var dataFromForm = $('#addMember_frm').serialize();
                dataFromForm = 'api=<?= ADMIN_ADD_MEMBER ?>&' + dataFromForm;
                $.ajax({
                    type: "POST",
                    data: dataFromForm,
                    url: "./util/api_process.php",
                    success: function(data) {
                        $("#console").html(data);
                        checkTrueFalse(data);
                        btn.style.backgroundColor = "#007bff";
                        btn.style.borderColor = "#007bff";
                        setVisible('#loading', false);
                        doubleSubmitFlag = false;
                    },
                    error: function(data) {
                        alert("Error : " + data);
                        setVisible('#loading', false);
                        doubleSubmitFlag = false;
                    }
                });
            });
        }
    });

    function checkTrueFalse(data) {
        setVisible('#loading', true);
        $("#container-fluid").load("./page/OperatingTools/adminMember.php", function() {
            // 페이지 로딩이 완료시 표시 끄기 [jQuery .load()]
            setVisible('#loading', false);
        });
    }

    function deleteAdminMember(no, mail) {
        var subject = "멤버 삭제";
        var content = "<div class='alert alert-warning' role='alert'>";
        content += "'" + mail + "'" + " 아이디를 삭제하시겠습니까?";
        content += "</div>";
        $("#modal-title").text(subject);
        $("#modal-body").html(content);
        $("#modal-data-value").text(no);
        $("#modal-api-value").text('<?= ADMIN_DEL_MEMBER ?>');
        $('#confirmModal').modal({
            show: true
        });
    }

    $("#modal-confirm").click(function() {
        if (doubleSubmitFlag == flase) {
            doubleSubmitFlag = true;
            $('#confirmModal').modal("hide");
            setVisible('#loading', true);
            var btn = document.getElementById("delMenu_btn");
            btn.style.backgroundColor = "#6c757d";
            btn.style.borderColor = "#6c757d";
            // 로그인 Post 처리
            var memberNo = $("#modal-data-value").text();
            var api = $("#modal-api-value").text();
            var dataFromForm = $('#fixAdminMember_frm_' + memberNo + ' :input').serialize();
            dataFromForm = 'api=' + api + '&' + dataFromForm;
            $.ajax({
                type: "POST",
                data: dataFromForm,
                url: "./util/api_process.php",
                success: function(data) {
                    $("#console").html(data);
                    checkTrueFalse(data);
                    btn.style.backgroundColor = "#007bff";
                    btn.style.borderColor = "#007bff";
                    setVisible('#loading', false);
                    doubleSubmitFlag = false;
                },
                error: function(data) {
                    alert("Error : " + data);
                    setVisible('#loading', false);
                    doubleSubmitFlag = false;
                }
            });
        }
    });
</script>