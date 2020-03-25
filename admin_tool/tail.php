<footer class="py-4 bg-light mt-auto">
    <div class="container-fluid">
        <div class="d-flex align-items-center justify-content-between small">
            <div class="text-muted">Copyright &copy; <?=$Site_Title?> 2020</div>
            <div>
                <?php if( $_SESSION['user_mail'] != ''){ ?>

                <?php }else{?>
                <a href="privacy">Privacy Policy</a>
                &middot;
                <a href="terms-of-use">Terms &amp; Conditions</a>
                <?php }?>
            </div>
        </div>
    </div>
</footer>
<script src="./js/scripts.js"></script>