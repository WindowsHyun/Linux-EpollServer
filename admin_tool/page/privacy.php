<div class="container" style="width:90%;">
    <div class="card card-login mx-auto mt-5" style="max-width:100%;">
        <div class="card-header">
            Privacy
        </div>
        <div class="card-body">
            <?php
            // 파일 열기
            $fp = fopen($_SERVER['DOCUMENT_ROOT'] . "/txtBundle/privacy.txt", "r");
            // 파일 내용 출력
            while (!feof($fp)) {
                echo fgets($fp);
                echo "<br>";
            }
            // 파일 닫기
            fclose($fp);
            ?>
        </div>
    </div>
</div>
<div style="padding-bottom:48px;"></div>