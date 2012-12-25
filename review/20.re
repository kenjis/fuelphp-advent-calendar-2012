
= FuelPHPのOrmを複数DBに対応させる方法 @<href>{https://twitter.com/6q5,@6q5}


@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012} 20日目の担当の@<href>{https://twitter.com/6q5,@6q5}です。
昨日は、@<href>{https://twitter.com/ttikitt,@ttikitt}さんの「FuelPHPへのDoctrine2組み込み」でした。@<br>{}


今年は仕事で3つもFuelPHPを使った媒体に関わり、
 FuelPHPと共に過ごした1年でした。
 その経験で得た物の中から選りすぐりの1つ？を紹介します。

== OrmではDBクラスのexecuteのようにDB接続先を指定することができません


DBがMaster/Slave構成の時に、Ormを使って参照だけの時はSlaveに向けたいなどしたい時に不便です。@<br>{}

こんな感じでconfigに2つのDB設定がされてるとします。

//emlist[例 config/db.php]{
/**
 * The development database settings.
 */
return array(
    'default' => array(
        'connection' => array(
            'dsn' => 'mysql:host=xxx.xxx.xxx.xxx;dbname=BLOG_DEV',
            'username' => 'blog',
            'password' => 'xxxxxxxx',
        ),  
    ),
    'slave' => array(
        'connection' => array(
            'dsn' => 'mysql:host=yyy.yyy.yyy.yyy;dbname=BLOG_DEV',
            'username' => 'blog',
            'password' => 'xxxxxxxx',
        ),
    ),  
); 
//}

//emlist[例 DBクラス]{
$query = DB::select('user_id','exp')
    ->from('users')
    ->where('user_id', $user_id)
    ->execute('slave') // 引数で指定できる
    ->as_array();
//}

//emlist[例 ORM]{
$query = Model_User::find()
    ->where('user_id', $user_id)
    ->get(); // でっできないっっ
//}

== それを可能にするには

以下のようにします。

=== packages/orm/classes/query.phpを拡張するためapp以下にコピー

//cmd{
$ mkdir -p app/classes/core/orm/
$ cp ../packages/orm/classes/query.php app/classes/core/orm/
//}

=== app/classes/core/orm/query.phpに以下のメソッドを追加

//emlist{
+
+       /**
+        * Change Connection
+        *
+        * @param   String connection
+        * @return  Query
+        */
+       public function connection($connection='default')
+       {
+               $this->connection = $connection;
+               return $this;
+       }
//}

=== packages/orm/classes/bootstrap.phpを以下のように書き換え

//emlist{
-       'Orm\\Query'        => __DIR__.'/classes/query.php'
+       'Orm\\Query'        => APPPATH.'/classes/core/orm/query.php',
//}

=== 結果

//emlist{
$result = Model_User::find()
    ->where('user_id', $user_id)
    ->where('status', Model_User::STATUS_ACTIVE)
    ->connection('slave') // できたヽ(^o^)丿
    ->get_one();
//}


これでOrmを使って複数DBの切り替えが出来るようになりました。

== おまけ tasksをproductionモードでcronに設定する


fuel/app/tasks/batch.php をProductionモードでcronに設定する場合、以下のようにします。

//emlist{
5 * * * * env FUEL_ENV=production /usr/local/bin/php oil r batch
//}


開発環境と本番環境を区別させて実行するための必須設定ですね。
以上！@<br>{}


あすは、@<href>{https://twitter.com/konkon1234,@konkon1234}さんの「FuelPHPで1サイトを作ってみて気が付いた点など」です。
 お楽しみに！

//quote{
@<strong>{@6q5}

Twitter: @<href>{https://twitter.com/6q5,@6q5}

Blog: @<href>{http://php6.tumblr.com/,http://php6.tumblr.com/}
//}

