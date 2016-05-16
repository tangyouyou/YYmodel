<?php
$model = new YYmodel('user','yy_');
#$model->setPrefix('yy');
$str = $model->select();
var_dump($str);
