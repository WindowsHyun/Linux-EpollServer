<!doctype html>
<html lang="kr">
<?php
// echo $SERVER['PHP_SELF'];
include("./head.php");
?>

<body class="sb-nav-fixed" id="page-top">
    <!-- topNavbar, modal Start -->
    <?php include("./topNavbar.php"); ?>
    <?php include("./modal.php"); ?>
    <!-- topNavbar, modal End -->

    <div id="wrapper">
        <!-- Menu Start -->
        <div id="layoutSidenav">
            <div id="layoutSidenav_nav">
                <?php include("./menu.php"); ?>
            </div>
            <!-- Menu End -->

            <!-- Content Start -->
            <div id="layoutSidenav_content">
                <!-- InContent Start -->
                <main>
                    <div id="container-fluid" class="container-fluid" style="padding-top:24px;"></div>
                </main>
                <!-- InContent End -->

                <!-- Tail Start -->
                <?php include("./tail.php"); ?>
                <!-- Tail End -->
            </div>
        </div>
        <!-- Content End -->

    </div>
    <!-- /#wrapper -->

    <!-- Scroll to Top Button Start-->
    <a class="scroll-to-top rounded" href="#page-top">
        <i class="fas fa-angle-up"></i>
    </a>
    <!-- Scroll to Top Button End-->

</body>

<script>
    $(document).ready(function() {
    	$("#container-fluid").load("./page/404.php");
    });

    function setVisible(selector, visible) {
    	document.querySelector(selector).style.display = visible ? 'block' : 'none';
    }

    document.addEventListener('DOMContentLoaded', function() {
    	setVisible('#loading', false);
    }, false);
</script>

</html>