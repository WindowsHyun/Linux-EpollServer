<?php
include("./common.php");
?>

<head>
    <meta charset="UTF-8">
    <title><?php echo $Site_Title; ?></title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">
    <!-- <meta property="og:image" content="img/favicon.png" /> -->
    <meta property="og:description" content="<?php echo $Site_Title; ?> - WindowsHyun" />
    <meta property="og:url" content="localhost" />
    <meta property="og:title" content="<?php echo $Site_Title; ?>" />
    <meta property="og:image:width" content="100%" />
    <meta property="og:image:height" content="100%" />

    <!-- Bootstrap core CSS-->
    <link href="vendor/bootstrap/css/bootstrap.min.css" rel="stylesheet">
    <link href="vendor/bootstrap/css/bootstrap4-toggle.min.css" rel="stylesheet">

    <!-- Custom fonts for this template-->
    <link href="vendor/fontawesome-free/css/all.min.css" rel="stylesheet" type="text/css">

    <!-- Page level plugin CSS-->
    <!-- <link href="vendor/datatables/dataTables.bootstrap4.css" rel="stylesheet"> -->

    <!-- Custom styles for this template-->
    <link href="css/sb-admin.css" rel="stylesheet">

    <!-- Bootstrap core JavaScript-->
    <script src="vendor/jquery/jquery.js"></script>
    <script src="vendor/datatables/jquery.dataTables.js"></script>
    <script src="vendor/bootstrap/js/bootstrap.bundle.min.js"></script>
    <script src="vendor/bootstrap/js/bootstrap4-toggle.min.js"></script>

    <!-- Long Click Script -->
    <script src="vendor/jquery/jquery.mobile.js?ver=20191031"></script>

    <!-- Bootstrap DataTable JavaScript-->
    <script src="vendor/datatables/dataTables.responsive.min.js"></script>
    <script src="vendor/datatables/dataTables.rowReorder.min.js"></script>

    <!-- Core plugin JavaScript-->
    <script src="vendor/jquery-easing/jquery.easing.min.js"></script>

    <!-- Custom scripts for all pages-->
    <script src="js/sb-admin.js"></script>

    <!-- <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.0.0/jquery.min.js"></script> -->


    <link rel="shortcut icon" type="image/x-icon" href="./img/favicon.ico" />

    <!-- Mobile top URL Delete [AOS, IOS] -->
    <meta name="theme-color" content="#343a40">
    <meta name="mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <link rel="manifest" href="manifest.json">

    <link href="css/loading.css?ver=1" rel="stylesheet">
    <div id="loading"><img id="loading-image" src="img/LoadingText.gif" alt="Loading..." /></div>

</head>


<script type="text/javascript">
    function setVisible(selector, visible) {
        document.querySelector(selector).style.display = visible ? 'block' : 'none';
    }

    document.addEventListener('DOMContentLoaded', function() {
        setVisible('#loading', false);
    }, false);
</script>