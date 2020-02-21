<?php
$LEVEL_LIST = array(
    11        => "[0] 숨기기",
    1         => "[1] 비회원",
    2         => "[2] 회원",
    3         => "[3] 회원",
    4         => "[4] 회원",
    5         => "[5] 회원",
    6         => "[6] 회원",
    7         => "[7] 회원",
    8         => "[8] 회원",
    9         => "[9] 회원",
    10        => "[10] 관리자",
);

$ORDER_LIST = array(
    0         => "0",
    1         => "1",
    2         => "2",
    3         => "3",
    4         => "4",
    5         => "5",
    6         => "6",
    7         => "7",
    8         => "8",
    9         => "9",
    10        => "10",
    11        => "11",
    12        => "12",
    13        => "13",
    14        => "14",
    15        => "15",
);

function draw_SelectBox($className, $dataArray, $selectValue, $readonly='false')
{
    $tag = null;
    if( $readonly == "true"){
        $readonly = "onFocus='this.initialSelect = this.selectedIndex;' onChange='this.selectedIndex = this.initialSelect;'";
        $readonlydata = "style='background-color:#CCCCCC'";
    }else{
        $readonly = "";
        $readonlydata = "";
    }

    $tag .= "<select name='$className' id='$className' $readonly>";
    foreach ($dataArray as $key => $value) {
        if ($selectValue == $key) { 
            $tag .= "<option value='$key' selected='selected'>$value</option>";
        } else {
            $tag .= "<option value='$key' $readonlydata>$value</option>";
        }
    }
    $tag .= "</select>";

    return $tag;
}
?>