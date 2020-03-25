<?php
include("_common.php");
// 관리자 권한 체크 해야함.

// menu DB 불러오기
$sql = "SELECT * FROM `" . $mysql_database . "`.`" . $mysql_menu . "` ORDER BY me_order ASC , me_suborder ASC";
$result = $mysqli->query($sql);

$menuArr = array();
$submenuArr = array();
while ($row = mysqli_fetch_assoc($result)) {
    $menuData = array();
    $menuData['me_no'] = $row['me_no'];
    $menuData['me_order'] = $row['me_order'];
    $menuData['me_suborder'] = $row['me_suborder'];
    $menuData['me_class'] = $row['me_class'];
    $menuData['me_permission'] = $row['me_permission'];
    $menuData['me_name'] = $row['me_name'];
    $menuData['me_icon'] = $row['me_icon'];
    $menuData['me_href'] = $row['me_href'];

    if ($menuData['me_suborder'] == "0") {
        // 서브메뉴가 없을 경우 메인 메뉴에 추가
        $menuArr[] = $menuData;
    } else {
        // 서브메뉴가 있을 경우 서브 메뉴에 추가
        $submenuArr[$menuData['me_order']][] = $menuData;
    }
}
$num = 1;
?>
<p id="console" style="color:#007bff; font-size:90%" hidden></p>

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
        메인 페이지 메뉴 설정</div>
    <div class="card-body">
        <div class="table-responsive">
            <table class="table table-striped table-bordered display nowrap" id="adminDataTable" width="100%" cellspacing="0">
                <thead>
                    <tr style="text-align: center; word-break: keep-all;">
                        <th>No</th>
                        <th>Order</th>
                        <th>Sub Order</th>
                        <th>Level</th>
                        <th>Class</th>
                        <th>Menu</th>
                        <th>Icon</th>
                        <th>Page</th>
                        <th>Control</th>
                    </tr>
                </thead>
                <tbody>
                    <?php foreach ($menuArr as $data) { ?>
                        <tr style="text-align: center;" id="fixMenu_frm_<?= $data['me_no']; ?>">
                            <td>
                                <?= $num; ?><input name="me_no" id="me_no" type="text" value="<?= $data['me_no']; ?>" hidden />
                            </td>
                            <td>
                                <?= draw_SelectBox("me_order", $ORDER_LIST, $data['me_order'], 'true'); ?>
                            </td>
                            <td>
                                <?= draw_SelectBox("me_suborder", $ORDER_LIST, $data['me_suborder']); ?></td>
                            <td>
                                <?= draw_SelectBox("me_permission", $PERMISSION_LIST, $data['me_permission']); ?></td>
                            <td>
                                <?= draw_SelectBox("me_class", $MENU_CLASS_LIST, $data['me_class']); ?>
                            </td>
                            <td>
                                <input name="me_name" id="me_name" type="text" value="<?= $data['me_name']; ?>" style="width:100px;" />
                            </td>
                            <td>
                                <input name="me_icon" id="me_icon" type="text" value="<?= $data['me_icon']; ?>" style="width:100px;" />
                            </td>
                            <td>
                                <input name="me_href" id="me_href" type="text" value="<?= $data['me_href']; ?>" style="width:100px;" />
                            </td>
                            <td>
                                <button type="button" id="editMenu_btn" onclick="EditAdminMenu('<?= $data['me_no']; ?>', '<?= $data['me_name']; ?>');" class="btn btn-primary btn-sm">수정</button>&nbsp;<button type="button" id="delMenu_btn" onclick="deleteFixMenu('<?= $data['me_no']; ?>', '<?= $data['me_name']; ?>');" class="btn btn-danger btn-sm">삭제</button>
                            </td>
                            <?php $num++;  ?>
                        </tr>
                        <?php if ($data['me_href'] == "#") { ?>
                            <?php foreach ($submenuArr[$data['me_order']] as $subdata) { ?>
                                <tr style="text-align: center;" id="fixMenu_frm_<?= $subdata['me_no']; ?>">
                                    <td>
                                        <?= $num; ?>
                                        <input name="me_no" id="me_no" type="text" value="<?= $subdata['me_no']; ?>" hidden />
                                    </td>
                                    <td>
                                        <?= draw_SelectBox("me_order", $ORDER_LIST, $subdata['me_order'], 'true'); ?></td>
                                    <td>
                                        <?= draw_SelectBox("me_suborder", $ORDER_LIST, $subdata['me_suborder']); ?></td>
                                    <td>
                                        <?= draw_SelectBox("me_permission", $PERMISSION_LIST, $subdata['me_permission']); ?></td>
                                    <td>
                                        <?= draw_SelectBox("me_class", $MENU_CLASS_LIST, $subdata['me_class']); ?>
                                    </td>
                                    <td>
                                        <input name="me_name" id="me_name" type="text" value="<?= $subdata['me_name']; ?>" style="width:100px;" />
                                    </td>
                                    <td>
                                        <input name="me_icon" id="me_icon" type="text" value="<?= $subdata['me_icon']; ?>" style="width:100px;" />
                                    </td>
                                    <td>
                                        <input name="me_href" id="me_href" type="text" value="<?= $subdata['me_href']; ?>" style="width:100px;" />
                                    </td>
                                    <td>
                                        <button type="button"  id="editMenu_btn" onclick="EditAdminMenu('<?= $subdata['me_no']; ?>', '<?= $subdata['me_name']; ?>');" class="btn btn-primary btn-sm">수정</button>&nbsp;<button type="button" id="delMenu_btn" onclick="deleteFixMenu('<?= $subdata['me_no']; ?>', '<?= $subdata['me_name']; ?>');" class="btn btn-danger btn-sm">삭제</button>
                                    </td>
                                </tr>
                                <?php $num++;  ?>
                            <?php } ?>
                        <?php } ?>
                    <?php } ?>
                </tbody>
            </table>
        </div>
    </div>
    <div class="card-footer small text-muted">
        <form id="addMenu_frm">
            <div class="table-responsive">
                <table class="table table-striped table-bordered table-sm" id="addDataTable" width="100%" cellspacing="0">
                    <thead>
                        <tr style="text-align: center; word-break: keep-all;">
                            <th>Order</th>
                            <th>Sub Order</th>
                            <th>Level</th>
                            <th>Class</th>
                            <th>Menu</th>
                            <th>Icon</th>
                            <th>Page</th>
                            <th>Control</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr style="text-align: center;">
                            <td><?= draw_SelectBox("me_order_add", $ORDER_LIST, '1'); ?></td>
                            <td><?= draw_SelectBox("me_suborder_add", $ORDER_LIST, '0'); ?></td>
                            <td><?= draw_SelectBox("me_permission_add", $PERMISSION_LIST, '2'); ?></td>
                            <td><?= draw_SelectBox("me_class_add", $MENU_CLASS_LIST, '0'); ?></td>
                            <td><input name="me_name_add" id="me_name_add" type="text" value="" style="width:100px;" />
                            </td>
                            <td><input name="me_icon_add" id="me_icon_add" type="text" value="" style="width:100px;" />
                            </td>
                            <td><input name="me_href_add" id="me_href_add" type="text" value="" style="width:100px;" />
                            </td>
                            <td><button id="addMenu_btn" onclick="AddAdminMenu();" type="button" class="btn btn-success btn-sm">추가</button></td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </form>
    </div>
</div>

<!-- <p class="small text-center text-muted my-5">
    <em>
        <p id="console" style="color:#007bff; font-size:90%"></p>
    </em>
    <em>More table examples coming soon...</em><br>
</p> -->

<script type="text/javascript">
    var doubleSubmitFlag = false;

    function checkTrueFalse(data) {
        setVisible('#loading', true);
        if (data != "TRUE") {
            $("#console").html(data);
        } else {
            $("#container-fluid").load("./page/Setting/menu.php", function() {
                // 페이지 로딩이 완료시 표시 끄기 [jQuery .load()]
                setVisible('#loading', false);
            });
        }
    }

    function EditAdminMenu(no, name) {
        var subject = "메뉴 수정";
        var content = "<div class='alert alert-warning' role='alert'>";
        content += name;
        content += " 메뉴를 수정하시겠습니까?";
        content += "</div>";
        $("#modal-title").text(subject);
        $("#modal-body").html(content);
        $("#modal-btn-value").text('editMenu_btn');
        $("#modal-data-value").text('#fixMenu_frm_' + no + ' :input');
        $("#modal-api-value").text('<?= ADMIN_EDIT_MENU ?>');
        $('#confirmModal').modal({
            show: true
        });
    }

    function AddAdminMenu() {
        var subject = "메뉴 추가";
        var content = "<div class='alert alert-warning' role='alert'>";
        content += "메뉴를 추가하시겠습니까?";
        content += "</div>";
        $("#modal-title").text(subject);
        $("#modal-body").html(content);
        $("#modal-btn-value").text('addMenu_btn');
        $("#modal-data-value").text('#addMenu_frm');
        $("#modal-api-value").text('<?= ADMIN_ADD_MENU ?>');
        $('#confirmModal').modal({
            show: true
        });
    }

    function deleteFixMenu(no, name) {
        var subject = "메뉴 삭제";
        var content = "<div class='alert alert-warning' role='alert'>";
        content += "'" + name + "'" + " 메뉴를 삭제하시겠습니까?";
        content += "</div>";

        $("#modal-title").text(subject);
        $("#modal-body").html(content);
        $("#modal-btn-value").text('delMenu_btn');
        $("#modal-data-value").text('#fixMenu_frm_' + no);
        $("#modal-api-value").text('<?= ADMIN_DEL_MENU ?>');
        $('#confirmModal').modal({
            show: true
        });
    }

    $("#modal-confirm").click(function() {
        if (doubleSubmitFlag == false) {
            doubleSubmitFlag = true;
            $('#confirmModal').modal("hide");
            setVisible('#loading', true);
            var btnValue = $("#modal-btn-value").text();
            var btn = document.getElementById(btnValue);
            btn.style.backgroundColor = "#6c757d";
            btn.style.borderColor = "#6c757d";
            // 로그인 Post 처리
            var formValue = $("#modal-data-value").text();
            var api = $("#modal-api-value").text();
            var dataFromForm = $(formValue).serialize();
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

    $('#adminDataTable').DataTable({
        responsive: true,
        order: [
            [0, "asc"]
        ]
    });
</script>