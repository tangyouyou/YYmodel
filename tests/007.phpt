--TEST--
Check for yymodel select,count,find
--SKIPIF--
--FILE--
<?php 
$model = new YYmodel('test');
$sql = $model->where('uid > 5')->select();
var_dump($sql);
$sql = $model->select();
var_dump($sql);
$sql = $model->count();
var_dump($sql);
$sql = $model->where('uid = 5')->count();
var_dump($sql);
$sql = $model->find();
var_dump($sql);

?>
--EXPECT--
string(33) "select * from test where uid > 5 "
string(18) "select * from test"
string(18) "select count(*) from test"
string(40) "select count(*) from test where uid = 5 "
string(26) "select * from test limit 1"


