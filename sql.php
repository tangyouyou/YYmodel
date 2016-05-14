<?php
$conn = new mysqli("127.0.0.1","root","root","test");
if($conn->connect_error){
     die($conn->conncet_error);
}

$table_name = 't1';
$model = new yymodel($table_name);
//id = $_POST['id'];
$id = 100;
$sql = $model->where("id>?")->select();
$stmt = $conn->prepare($sql);
$stmt->bind_param("s",$id);

$stmt->execute();
//绑定结果集
$stmt->bind_result($id,$name);
while($stmt->fetch()){
   printf("the id of number is:%d, the name is %s\n",$id,$name);
}
$conn->close();

