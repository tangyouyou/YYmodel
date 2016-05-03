--TEST--
Check for yymodel update
--SKIPIF--
--FILE--
<?php 
$model = new YYmodel('test');
$data = array('username'=>'tangyouyou','password'=>'123456','test'=>'78910');
$sql = $model->where('uid=5')->update($data);
var_dump($sql);
//$data = "`username`='zhongwei',`password`='654321'";
//$sql = $model->where('uid=5')->update($data);
//var_dump($sql);

?>
--EXPECT--
update `test` set `username`='tangyouyou',`password`='password' where uid=5;
update `test` set `username`='zhongwei',`password`='654321' where uid=5;
