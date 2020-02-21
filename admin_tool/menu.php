<?php
include("./util/db_config.php");
// menu DB 불러오기
$sql = "SELECT * FROM `".$mysql_database."`.`".$mysql_menu."` ORDER BY me_order ASC , me_suborder ASC";
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

$mb_level = DecryptSession($_SESSION['user_level'], $_SESSION['user_mail']);
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

<ul class="sidebar navbar-nav" id="sideMenu">
	<!-- 메뉴 foreach 시작 -->
	<?php foreach ($menuArr as $data) { ?>
		<!-- 메뉴 Level 권한 체크 시작 -->
		<?php if ($mb_level >= $data['me_level'] && $data['me_level'] !== 11) { ?>
			<!-- 메뉴 dropdown 체크 시작 -->
			<?php if ($data['me_href'] == "#") { ?>
				<li class="nav-item dropdown">
					<a class="nav-link dropdown-toggle" onclick="loadPage('<?php echo $data['me_href']; ?>');" style="cursor:pointer" id="pagesDropdown" role="button" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
						<i class="<?php echo $data['me_icon']; ?>"></i>
						<span><?php echo $data['me_name']; ?></span>
					</a>
					<div class="dropdown-menu" aria-labelledby="pagesDropdown">
						<!-- 서브메뉴 foreach 시작 -->
						<?php foreach ($submenuArr[$data['me_order']] as $subdata) { ?>
							<!-- 서브메뉴 Level 권한 체크 시작 -->
							<?php if ($mb_level >= $subdata['me_level']) { ?>
								<!-- 서브메뉴 header 체크 시작 -->
								<?php if ($subdata['me_class'] == "dropdown-header") { ?>
									<h6 class="dropdown-header"><?php echo $subdata['me_name']; ?></h6>
								<?php } else if ($subdata['me_class'] == "dropdown-divider") { ?>
									<div class="dropdown-divider"></div>
								<?php } else { ?>
									<a class="dropdown-item" onclick="loadPage('<?php echo $subdata['me_href']; ?>');" style="cursor:pointer"><?php echo $subdata['me_name']; ?></a>
								<?php } ?>
								<!-- 서브메뉴 header 체크 종료 -->
							<?php } ?>
							<!-- 서브메뉴 Level 권한 체크 종료 -->
						<?php } ?>
						<!-- 서브메뉴 foreach 종료 -->
					</div>
				</li>
			<?php } else { ?>
				<li class="nav-item active">
					<a class="nav-link" onclick="loadPage('<?php echo $data['me_href']; ?>');" style="cursor:pointer">
						<i class="<?php echo $data['me_icon']; ?>"></i>
						<span><?php echo $data['me_name']; ?></span>
					</a>
				</li>
			<?php } ?>
			<!-- 메뉴 dropdown 체크 종료 -->
		<?php } ?>
		<!-- 메뉴 Level 권한 체크 종료 -->
	<?php } ?>
	<!-- 메뉴 foreach 종료 -->
</ul>