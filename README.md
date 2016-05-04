# YYmodel--使用原形链生成SQL语句的扩展

### Requirement
php5.2+

### install

git clone https://github.com/tangyouyou/YYmodel.git

/path/to/phpize

./configure --with-php-config=/path/to/php-config

make && make install



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


