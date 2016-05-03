--TEST--
Check for yymodel field
--SKIPIF--
--FILE--
<?php 
$model = new YYmodel('test');
$field = array('username','password','uid');
$sql = $model->field($field)->select();
var_dump($sql);
?>
--EXPECT--
select username,password,uid from test;
