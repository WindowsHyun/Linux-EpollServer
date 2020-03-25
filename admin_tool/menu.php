<?php
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

$mb_level = DecryptSession($_SESSION['user_permission'], $_SESSION['user_mail']);
$collapse_num = 0;
?>

<script>
    function loadPage(url, data = '') {
        if (url != "#") {
            // 페이지 로딩중 표시
            setVisible('#loading', true);
            if (data == '') {
                $("#container-fluid").load("./page/" + url + ".php", function() {
                    // 페이지 로딩이 완료시 표시 끄기 [jQuery .load()]
                    setVisible('#loading', false);
                });
            } else {
                $("#container-fluid").load("./page/" + url + ".php", {
                    data
                }, function() {
                    // 페이지 로딩이 완료시 표시 끄기 [jQuery .load()]
                    setVisible('#loading', false);
                });
            }
            // 뒤로가기를 위한 pushState
            window.history.pushState({
                "url": url,
                "data": data
            }, null, "index");
        }
    }
    window.onpopstate = function(event) {
        // pushState에서 url, data를 받아서 loadPage에 넣어준다.
        loadPage(history.state["url"], history.state["data"]);
    };
</script>

<nav class="sb-sidenav accordion sb-sidenav-dark" id="sidenavAccordion">
    <div class="sb-sidenav-menu">
        <div class="nav">
            <!-- 메뉴 foreach 시작 -->
            <?php foreach ($menuArr as $data) { ?>
                <!-- 메뉴 Level 권한 체크 시작 -->
                <?php if ($mb_level >= $data['me_level'] && $data['me_level'] !== PERMISSION_HIDE) { ?>
                    <!-- 메뉴 dropdown 체크 시작 -->
                    <?php if ($data['me_class'] == HEADING_MENU) { ?>
                        <div class="sb-sidenav-menu-heading"><?= $data['me_name']; ?></div>
                    <?php } else if ($data['me_class'] == DROPDOWN_MENU) { ?>
                        <a class="nav-link collapsed" href="#" data-toggle="collapse" data-target="#collapse_<?= $collapse_num ?>" aria-expanded="false" aria-controls="collapse_<?= $collapse_num ?>">
                            <div class="sb-nav-link-icon"><i class="<?= $data['me_icon']; ?>"></i></div>
                            <?= $data['me_name']; ?>
                            <div class="sb-sidenav-collapse-arrow"><i class="fas fa-angle-down"></i></div>
                        </a>
                        <div class="collapse" id="collapse_<?= $collapse_num ?>" aria-labelledby="headingOne" data-parent="#sidenavAccordion">
                            <nav class="sb-sidenav-menu-nested nav">
                                <!-- 서브메뉴 foreach 시작 -->
                                <?php foreach ($submenuArr[$data['me_order']] as $subdata) { ?>
                                    <a class="nav-link" onclick="loadPage('<?= $subdata['me_href']; ?>');" style="cursor:pointer">
                                        <?= $subdata['me_name']; ?>
                                    </a>
                                <?php } ?>
                                <!-- 서브메뉴 foreach 종료 -->
                            </nav>
                        </div>
                    <?php } ?>
                <?php } else if ($data['me_class'] == LINK_PAGE) { ?>
                    <a class="nav-link" onclick="loadPage('<?= $data['me_href']; ?>');" style="cursor:pointer">
                        <?= $data['me_name']; ?>
                    </a>
                <?php } ?>
                <?php $collapse_num++; ?>
                <!-- 메뉴 Level 권한 체크 종료 -->
            <?php } ?>
            <!-- 메뉴 foreach 종료 -->
        </div>
    </div>
    <div class="sb-sidenav-footer">
        <div class="small">Logged in as:</div>
        <?= DecryptSession($_SESSION['user_name'], $_SESSION['user_mail']); ?>
    </div>
</nav>