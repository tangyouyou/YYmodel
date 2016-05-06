# YYmodel--使用原形链生成SQL语句的扩展

### Requirement
php5.2+

### install
```
git clone https://github.com/tangyouyou/YYmodel.git

/path/to/phpize

./configure --with-php-config=/path/to/php-config

make && make install
```


### 例子如下：

$table_name = 'user';
$model = new yymodel($table_name);
$model->field('uid,username')->select();

或者采用数组的形式
$field = array('uid','username');
$model->field($field)->select();

# 支持where、limit、group、order、join等语法

$model->join('test','test.uid=user.uid')->where('uid>55')->order('uid desc')->limit("55")->select();

# 支持DML语句

###insert例子

$data = array('username'=>'tangyouyou','password'=>'123456');

$model->insert($data);

或者采用字符串的形式

$data = "username=tangyouyou,password=123456";

$model->insert($data);

###update例子

$data = array('username'=>'tangyouyou','password'=>'123456');

$model->where('uid=55')->update($data);

或者采用字符串的形式

$data = "username=tangyouyou,password=123456";

$model->where('uid=55')->update($data);

# 可以动态获取生成的SQL
$sql = $model->getLastSql();

var_dump($sql);

###搭配MYSQL预处理防止SQL注入例子
```
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
```



