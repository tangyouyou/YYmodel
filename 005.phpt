--TEST--
Check for yymodel join
--SKIPIF--
--FILE--
<?php 
$model = new YYmodel('test');
$sql = $model->select();
var_dump($sql);
$sql = $model->join('user','user.uid=test.uid')->select();
var_dump($sql);
$sql = $model->select();
var_dump($sql);
$sql = $model->join('abc','abc.uid=test.uid')->select();
var_dump($sql);

?>
--EXPECT--
select * from test;
select * from test inner join user on user.uid=test.uid;
select * from test;
string(53) "select * from test inner join abc on abc.uid=test.uid"
