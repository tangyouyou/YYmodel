--TEST--
Check for yymodel join
--SKIPIF--
--FILE--
<?php 
$model = new YYmodel('test');
$sql = $model->join('user','user.uid=test.uid')->select();
var_dump($sql);
$sql = $model->join('user','user.uid=test.uid')->join('test1','user.uid=test1.uid')->select();
var_dump($sql);
?>
--EXPECT--
string(55) "select * from test inner join user on user.uid=test.uid"
string(57) "select * from test inner join user on user.uid=test.uid inner join test1 on user.uid=test1.uid"

