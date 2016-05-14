<?php
$conn = mysql_connect('127.0.0.1','root','wozhiainiwlf');
if(!$conn) {
   die('Could not connect:'.mysql_error());
}
mysql_select_db('blog',$conn) or die ( 'Can\'t use test : '  .  mysql_error ());

$model = new yymodel('yy_user');
// 存在SQL注入问题
$username = "1' or '1'='1";
$sql = $model->where("username='{$username}'")->select();
// string(49) "select * from test where username='1' or '1' = 1 "

// 解决办法
$username = trim(addslashes(htmlspecialchars($username)));
$sql = $model->where("username='{$username}'")->select();
// string(53) "select * from test where username=\'1\' or \'1\' = 1 "

// 解决思路: 重点是addslashes使用反斜杠引用字符串，htmlspecialchars
// 可以用于防止xss攻击

$result = mysql_query($sql) or die( "Invalid query: "  .  mysql_error ());

$row =  mysql_fetch_row ( $result );

var_dump($row);



