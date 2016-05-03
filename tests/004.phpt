--TEST--
Check for yymodel insert
--SKIPIF--
--FILE--
<?php 
$model = new YYmodel('test');
$data = array('username'=>'tangyouyou','password'=>'123456');
//$data = 'username=tangyouyou,password=123456';
$sql = $model->insert($data);
var_dump($sql);

?>
--EXPECT--
string(69) "insert into `test` (username,password) values ('tangyouyou','123456')"

