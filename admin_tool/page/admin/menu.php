<?php
include($_SERVER['DOCUMENT_ROOT'] . "/page/common.php");
include($_SERVER['DOCUMENT_ROOT'] . "/util/db_config.php");
include($_SERVER['DOCUMENT_ROOT'] . "/util/define.php");
// 관리자 권한 체크 해야함.

// menu DB 불러오기
$sql = "SELECT * FROM `mypc`.`menu` ORDER BY me_order ASC , me_suborder ASC";
$result = $mysqli->query($sql);

$menuArr = array();
$submenuArr = array();
while ($row = mysqli_fetch_assoc($result)) {
    $menuData = array();
    $menuData['me_no'] = $row['me_no'];
    $menuData['me_order'] = $row['me_order'];
    $menuData['me_suborder'] = $row['me_suborder'];
    $menuData['me_class'] = $row['me_class'];
    $menuData['me_level'] = $row['me_level'];
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
<!-- Modal 사용하려면 필수 -->
<script type="text/javascript" src="vendor/jquery/jquery.js"></script>
<script type="text/javascript" src="vendor/datatables/jquery.dataTables.js"></script>
<script type="text/javascript" src="vendor/bootstrap/js/bootstrap.bundle.min.js"></script>
<!-- Modal 사용하려면 필수 -->
<script type="text/javascript">
    $(document).ready(function() {
        // Enter 입력시 로그인 처리
        $("#me_class_add").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#addMenu_btn").trigger("click");
            }
        });
        $("#me_name_add").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#addMenu_btn").trigger("click");
            }
        });
        $("#me_icon_add").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#addMenu_btn").trigger("click");
            }
        });
        $("#me_href_add").keydown(function(event) {
            if (event.keyCode == 13) {
                $("#addMenu_btn").trigger("click");
            }
        });

        // Login 버튼 클릭시 처리
        $("#addMenu_btn").click(function() {
            setVisible('#loading', true);
            var btn = document.getElementById("addMenu_btn");
            btn.style.backgroundColor = "#6c757d";
            btn.style.borderColor = "#6c757d";
            // 로그인 Post 처리
            var dataFromForm = $('#addMenu_frm').serialize();
            $.ajax({
                type: "POST",
                data: dataFromForm,
                url: "./util/admin/addMenu_process.php",
                success: function(data) {
                    $("#console").html(data);
                    checkTrueFalse(data);
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

    function checkTrueFalse(data) {
        setVisible('#loading', true);
        $("#container-fluid").load("./page/admin/menu.php", function() {
            // 페이지 로딩이 완료시 표시 끄기 [jQuery .load()]
            setVisible('#loading', false);
        });
    }
</script>

<!-- Breadcrumbs-->
<ol class="breadcrumb">
    <li class="breadcrumb-item"><?=$Site_Title?></li>
    <li class="breadcrumb-item">Admin</li>
    <li class="breadcrumb-item active">Menu</li>
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
                        <form id="fixMenu_frm_<?php echo $data['me_no']; ?>">
                            <tr style="text-align: center;">
                                <?php $me_no = $data['me_no']; ?>
                                <td><?php echo $num; ?><input name="me_no_<?php echo $me_no; ?>" id="me_no_<?php echo $me_no; ?>" type="text" value="<?php echo $data['me_no']; ?>" hidden /></td>
                                <td>
                                    <?php echo draw_SelectBox("me_order_$me_no", $ORDER_LIST, $data['me_order'], 'true'); ?>
                                </td>
                                <td>
                                    <?php echo draw_SelectBox("me_suborder_$me_no", $ORDER_LIST, $data['me_suborder']); ?></td>
                                <td>
                                    <?php echo draw_SelectBox("me_level_$me_no", $LEVEL_LIST, $data['me_level']); ?></td>
                                <td><input name="me_class_<?php echo $me_no; ?>" id="me_class_<?php echo $me_no; ?>" type="text" value="<?php echo $data['me_class']; ?>" style="width:100px;" /></td>
                                <td><input name="me_name_<?php echo $me_no; ?>" id="me_name_<?php echo $me_no; ?>" type="text" value="<?php echo $data['me_name']; ?>" style="width:100px;" /></td>
                                <td><input name="me_icon_<?php echo $me_no; ?>" id="me_icon_<?php echo $me_no; ?>" type="text" value="<?php echo $data['me_icon']; ?>" style="width:100px;" /></td>
                                <td><input name="me_href_<?php echo $me_no; ?>" id="me_href_<?php echo $me_no; ?>" type="text" value="<?php echo $data['me_href']; ?>" style="width:100px;" /></td>
                                <td><button type="button" onclick="editFixMenu('<?php print $me_no ?>');" class="btn btn-primary btn-sm">수정</button>&nbsp;<button type="button" onclick="deleteFixMenu('<?php print $me_no ?>');" class="btn btn-danger btn-sm">삭제</button></td>
                                <?php $num++;  ?>
                            </tr>
                        </form>
                        <?php if ($data['me_href'] == "#") { ?>
                            <?php foreach ($submenuArr[$data['me_order']] as $subdata) { ?>
                                <form id="fixMenu_frm_<?php echo $data['me_no']; ?>">
                                    <tr style="text-align: center;">
                                        <?php $me_no = $data['me_no']; ?>
                                        <td><?php echo $num; ?><input name="me_no_<?php echo $me_no; ?>" id="me_no_<?php echo $me_no; ?>" type="text" value="<?php echo $data['me_no']; ?>" hidden /></td>
                                        <td>
                                            <?php echo draw_SelectBox("me_order", $ORDER_LIST, $subdata['me_order'], 'true'); ?></td>
                                        <td>
                                            <?php echo draw_SelectBox("me_suborder", $ORDER_LIST, $subdata['me_suborder']); ?></td>
                                        <td>
                                            <?php echo draw_SelectBox("me_level", $LEVEL_LIST, $subdata['me_level']); ?></td>
                                        <td><input name="me_class_<?php echo $me_no; ?>" id="me_class_<?php echo $me_no; ?>" type="text" value="<?php echo $subdata['me_class']; ?>" style="width:100px;" /></td>
                                        <td><input name="me_name_<?php echo $me_no; ?>" id="me_name_<?php echo $me_no; ?>" type="text" value="<?php echo $subdata['me_name']; ?>" style="width:100px;" /></td>
                                        <td><input name="me_icon_<?php echo $me_no; ?>" id="me_icon_<?php echo $me_no; ?>" type="text" value="<?php echo $subdata['me_icon']; ?>" style="width:100px;" /></td>
                                        <td><input name="me_href_<?php echo $me_no; ?>" id="me_href_<?php echo $me_no; ?>" type="text" value="<?php echo $subdata['me_href']; ?>" style="width:100px;" /></td>
                                        <td><button type="button" class="btn btn-primary btn-sm">수정</button>&nbsp;<button type="button" class="btn btn-danger btn-sm">삭제</button></td>
                                    </tr>
                                </form>
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
                        <tr style="text-align: center;">
                            <td>n</td>
                            <td><?php echo draw_SelectBox("me_order_add", $ORDER_LIST, '1'); ?></td>
                            <td><?php echo draw_SelectBox("me_suborder_add", $ORDER_LIST, '0'); ?></td>
                            <td><?php echo draw_SelectBox("me_level_add", $LEVEL_LIST, '2'); ?></td>
                            <td><input name="me_class_add" id="me_class_add" type="text" value="" style="width:100px;" /></td>
                            <td><input name="me_name_add" id="me_name_add" type="text" value="" style="width:100px;" />
                            </td>
                            <td><input name="me_icon_add" id="me_icon_add" type="text" value="" style="width:100px;" />
                            </td>
                            <td><input name="me_href_add" id="me_href_add" type="text" value="" style="width:100px;" />
                            </td>
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
    function deleteFixMenu(no) {
        var subject = "타이틀 테스트";
        var content = "<div class='alert alert-warning' role='alert'>";
        content += ("입력하시겠습니까?");
        content += "</div>";

        $('#confirmModal').modal({show : true});
    }
</script>