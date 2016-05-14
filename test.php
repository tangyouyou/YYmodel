<?php
$model = new YYmodel('user');
#$model->setPrefix('yy');
$str = $model->select();
var_dump($str);
