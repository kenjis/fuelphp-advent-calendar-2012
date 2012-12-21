
= FuelPHPで１サイトを作ってみて気が付いた点など @<href>{https://twitter.com/konkon1234,@konkon1234}


@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012} (#FuelPHPAdvent2012) 21日目です。
前日の記事は@<href>{https://twitter.com/6q5,@6q5}さんの「FuelPHPのOrmを複数DBに対応させる方法」でした。@<br>{}


本日は私が初めてFuelPHPを使った時に、躓いたことの解決法を焦点を絞らずに書いていきたいと思います。

==   fieldsetでform出力したいけど見た目は場所によって変更したい


fieldsetでフィールドの定義をまとめて、表示はViewで自在に配置したい。
結構あるケースだと思います。


そんな時はViewで個別にfield単位で表示して、その際にtemplateを設定すれば入力欄のみだったりエラーメッセージのみを取得できます。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
<?php
// fieldset作成(Model側での処理
$form = \Fieldset::forge();
$form->add('user_id', 'ユーザー名')
    ->set_type('text')
    ->add_rule('required');
$form->add('gender', '性別')
    ->set_type('radio')
    ->set_options(array(1 => '男性', 2 => '女性'));

// 以下、Viewでの表示
?>

<dl>

<dt>
<?php echo $form->field('user_id')
    ->set_template('{label}{required}'); // ラベル単体表示 ?>
</dt>

<dd>
<?php echo $form->field('user_id')
    ->set_template('{field}')
    ->set_attribute(array('class' => 'txt_half')); // 入力欄表示 ?>
<?php echo $form->field('user_id')
    ->set_template('<span class="attention">{error_msg}</span>'); // エラーメッセージ ?>
</dd>

<dt>
<?php echo $form->field('gender')
    ->set_template('{label}{required}'); ?>
</dt>

<dd>
<?php echo $form->field('gender')
    ->set_template('{fields} {field} {label}<br /> {fields}'); // 入力欄表示 ?>
<?php echo $form->field('gender')
    ->set_template('<span class="attention">{error_msg}</span>'); // エラーメッセージ ?>
</dd>
</dl>
//}


こんな感じで出力されます。

//image[form_output][formの描画イメージ]{
//}


config/form.phpを参考に、使える置換文字列をset_template()で設定することでその項目の必要な部分のみ描画できます。
動的に出力されるタグに属性を追加したい場合はset_attribute()で追加します。@<br>{}


私は上の記述方法が結構好きなのですが、項目が多くなってきたりするとViewがとても残念なことになってくるのでもっと綺麗に書きたいと思うのがプログラマの性です。
 そんな時は@<href>{http://blog.omoon.org/20121207/523http://blog.omoon.org/20121207/523,Advent Calendar 7日目の@omoonさんの記事}等を参考にしてfieldset自体を拡張するのがお勧めです。

==   phpunitすると文字が化ける(Windows)


Windows環境下でoil testすると結果が下のように出てきて悩んでました。

#@# lang: .brush: .bash; .title: .; .notranslate title=""
//emlist{
[30;42m[2KOK (0 tests, 0 assertions)
[0m[2K
//}


これは文字化けというよりはカラー用のエスケープシーケンスがそのまま出ているようで、utf8がどうのとか文字コードを調べてしまうとなかなか解決に辿り着けません。以下を修正してください。




#@# lang: .brush: .xml; .title: .; .notranslate title=""
//emlist[COREPATH/phpunit.xml 3行目]{
- <phpunit colors="true" stopOnFailure="false" bootstrap="../core/bootstrapphpunit.php">
+ <phpunit colors="false" stopOnFailure="false" bootstrap="../core/bootstrapphpunit.php">
//}

==   クラスの名前空間はFuelPHPのスタイルに合わせて絶対でなく相対で


IDEの自動補完があるのでついついクラス名をルートから完全修飾したくなりますが、これが挙動にも影響を与えるケースがあります。


 * @<href>{https://groups.google.com/forum/#!topic/fuelphp_jp/JT4QgQqWNjE,fuelphp.jp | Google グループ　コントローラからのレスポンスのContent-Typeを変更できません}


郷に入っては郷に従えという諺に従いましょう。


クラス名だけで自動補完をするためのIDE用定義を生成するスクリプトを@<href>{https://twitter.com/kenji_s,@kenji_s}さんが公開してくださっているので、ここはありがたく使用させていただきます。


 * @<href>{http://d.hatena.ne.jp/Kenji_s/20120123/1327301678,A Day in Serenity @ kenjis | Eclipse で FuelPHP のコード補完を行わせる冴えたやり方}


Windowsで使用する場合は以下を修正してください。




#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist[関数の41行目]{
 if (is_array($file))
 {
- static::generate_class_definition($file, $str . ucfirst(rtrim($dir, '/') . '_'));
+ static::generate_class_definition($file, $str . ucfirst(rtrim($dir, DS) . '_'));
 }
//}

==   fieldsetの選択項目でauto_id_prefixが2重に付く


デフォルトの設定だと 'form_' になっているauto_id_prefixの値がradioなどの選択項目で@<code>{<label for='form_form_xxxxx'>}となるバグに遭遇しました(v1.4)。


調べてみるとCOREPATH/classes/fieldset/field.phpの以下が原因だったので修正。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
- $buildfield[$form->label($label, $attributes['id'])] = $this->type == 'radio'
+ $buildfield[$form->label($label, $attributes['id'], array('for' => $attributes['id']))@<raw>{|latex|\n}] = $this->type == 'radio'
//}


そしてpull requestを送っちゃうぞ〜と意気揚々とgithubを確認してみると、そこには既に修正済みのコードが掲載されていました。
 gitで最新取ってる人には無縁の悩みだったようです。

==   テキストフィールドに0を設定しても空になる


タイトルのまんまです(v1.4)。




#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist[Model側]{
$form->add('price', '金額')
 ->set_type('text')
 ->set_value('0');
//}




#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist[View側]{
<?php echo $form->field('price'); ?>
//}


とやっても空白なテキストフィールドが表示されてしまいました。(v1.4)


これはCORE/base.phpのarray_to_attrという関数内での条件が原因となっていました。
 案の定、GitHub上では修正されておりましたので修正内容をありがたくマージしました。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
// Ignore empty values (null/false/[empty string])
- if (empty($value))
+ if ($value === null or $value === false)
//}


prefixの件と合わせて本来はgit使って最新版使ってると悩まないことなのかもしれません。
 ですが私のように公式のzipアーカイブをダウンロードして、これを基に実装を進めるような人がフレームワーク側のバグっぽい現象に遭遇したら最初にGitHubで最新のソースを確認してみることをお勧めします。

==   別DBのテーブルをModel_Crudで参照する


APPPATH/config/db.phpに複数の接続情報を用意すると、Model別に使用する設定を変えることが出来ます。


まずは接続情報をconfigに追加します。




#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist[APPPATH/config/db.php]{
return array(
    'active' => 'default',

    'default' => array(
        'connection'  => array(
            'dsn'        => 'mysql:host=db_server;dbname=db',
            'username'   => 'xxxxxxxx',
            'password'   => 'xxxxxxxx',
        ),
    ),

+   'server2_db' => array(
+       'connection'  => array(
+           'dsn'        => 'mysql:host=db_server2;dbname=db',
+           'username'   => 'xxxxxxxx',
+           'password'   => 'xxxxxxxx',
+       )
+   ),

);
//}




#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist[APPPATH/config/各種ステージ/db.php]{
return array(
    'active' => 'default',

    /**
     * Base config, just need to set the DSN, username and password in env. config.
     */
    'default' => array(
        'type'        => 'pdo',
        'connection'  => array(
            'persistent' => false,
        ),
        'identifier'   => '`',
        'table_prefix' => '',
        'charset'      => 'utf8',
        'enable_cache' => true,
        'profiling'    => false,
    ),

+   'server2_db' => array(
+       'type'        => 'pdo',
+       'connection'  => array(
+           'persistent' => false,
+       ),
+       'identifier'   => '`',
+       'table_prefix' => '',
+       'charset'      => 'utf8',
+       'enable_cache' => true,
+       'profiling'    => false,
+   ),
//}


後はModel_Crud側で@<code>{$_connection}プロパティを設定します。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
class Model_Xxxxx extends Model_Crud
{
    // config/db.phpの連想配列のキーを設定する
+   protected static $_connection = 'server2_db';
//}


これで設定完了です。
@<code>{$_connection}を省略した際はactiveで示された設定が使用されます。



以上、他の日の記事と微妙に被ってる内容の多い21日目でした。@<br>{}


明日は@<href>{https://twitter.com/ts_asano,@ts_asano}さんの「(仮)QueryBuilderで小難しいSELECT文を書くためのノウハウ」です。
 表題からして素敵ですね。お楽しみに♪

//quote{
@<strong>{@konkon1234}

Twitter: @<href>{https://twitter.com/konkon1234,@konkon1234}

Blog: @<href>{http://www.crossl.net/blog/,http://www.crossl.net/blog/}
//}

