<div class="container" style="width:90%;">
    <div class="card card-login mx-auto mt-5" style="max-width:100%;">
        <div class="card-header">
            Terms of use
        </div>
        <div class="card-body">
            <?php
            // 파일 열기
            $filePath = dirname(__DIR__ . '..');
            $fp = fopen($filePath."/txtBundle/terms-of-use.txt", "r");
            if ($fp == null) {
                echo "File Error<br>";
            } else {
                // 파일 내용 출력
                while (!feof($fp)) {
                    $txt = fgets($fp);
                    $txt = str_replace("{Site::URL}", "" . $Site_URL . "", $txt);
                    $txt = str_replace("{Site::Company}", "" . $Site_Title . "", $txt);
                    $txt = str_replace("{Site::Email}", "" . $Site_Email . "", $txt);
                    echo $txt;
                    echo "<br>";
                }
            }
            // 파일 닫기
            fclose($fp);
            ?>
        </div>
    </div>
</div>
<div style="padding-bottom:48px;"></div>