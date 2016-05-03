--TEST--
Check for yymodel where
--SKIPIF--
--FILE--
<?php 
$model = new YYmodel('test');
$sql = $model->where('uid > 5')->select();
var_dump($sql);
$model->select();
var_dump($sql);

?>
--EXPECT--
select * from test where uid > 5
select * from test;
