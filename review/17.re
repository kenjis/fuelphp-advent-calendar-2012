= FuelPHPで既存DBをあつかうとか @<href>{https://twitter.com/ootatter,@ootatter}

この記事は @<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012}の17日目の記事です。
前日は@<href>{https://twitter.com/mayama4u,@mayama4u}さんの「FuelPHPで作るFacebook診断アプリ」でした。
作成事例として詳細に解説して頂いてるのでこれからFuelPHPでFacebookアプリを作るときはすごく参考になるかと思いました！
個人的にまだアプリを作ったことがないので参考にさせて頂きたいと思います!@<br>{}

さて、私はといえば、「既存DBをあつかう」と見出しにつけておりますがこれまでfuelphpを使うときは、

 * フロントエンド FuelPHP
 * バックエンド CakePHP

という構成で使ってました。
主に取得する目的で使ってきましたのでちょっと期待はずれ感があるかもしれませんがご了承頂ければと!
今回は、その中でも@<strong>{こうしてみました}っていうこところを部分的に紹介できればと思います。

== ページ送り時のデータ取得はどうしてますか？

これまで、ページ送りする場合、データ取得時にcount用のメソッド追加してましたが、count_last_queryを使うと便利でした。

#@# lang: .brush: .php;
//emlist[example)]{
$query = DB::select(*)
    ->from('items')
    ->order_by('items.id','DESC')
    ->limit(10)
    ->offset(0)
    ->execute();
$result = $query->execute();
$return['count'] = DB::count_last_query();
$return['result'] = $result->as_array();
//}

マニュアルにもバッチリ記載されていたでございます。

 * @<href>{http://fuelphp.com/docs/classes/database/db.html#/method_count_last_query,http://fuelphp.com/docs/classes/database/db.html#/method_count_last_query}

ドキュメントをきちんとチェックしないとですね…

@<href>{https://twitter.com/NEKOGET,@NEKOGET}さんの「@<href>{http://pneskin2.nekoget.com/press/?p=1044,FuelPHPドキュメント翻訳へのお誘い}」を見て翻訳にも貢献できればと思います。

== 例えばEC-CUBEのデータ(PostgreSQL)を取得したい時がありまして

シーケンスどうすれば!?と思ったんですが下記のようにしました。

model/app.phpを定義して

#@# lang: .brush: .php;
//emlist{
class Model_App extends \Orm\Model
{
    public static function sequenceId()
    {
        return static::$_sequence_id;
    }
    public static function nextID()
    {
        $query = \DB::query("SELECT NEXTVAL('".static::sequenceId()."')");
        $result = $query->execute(\Database_Connection::instance())->as_array();
        return $result[0]['nextval'];
    }
}
//}

model/order.phpでは

#@# lang: .brush: .php;
//emlist{
class Model_Order extends Model_App
{
    protected static $_table_name = 'dtb_products';
    protected static $_primary_key = array('product_id');
    protected static $_sequence_id = 'dtb_order_order_id_seq';

    public static function example_save(){
        $order_id = static::nextID();
        …省略…
    }
}
//}

== そしてWordPressのサイトをFuelPHPで置き換えたいな〜とふと思った時がありまして

モデルの部分だけでもパッケージ化に挑戦してみようかと思いました(ormを継承してます)。

 * @<href>{https://github.com/milds/fuelpress,https://github.com/milds/fuelpress}

ですが途中で、、中途半端に出してすみません。
せめてmodel定義だけでも何かしらの…@<br>{}

配慮したこと

 * WordPress用のdb定義を使うこと
 * テーブルのプレフィックスを付けること

ormを継承したときはdb.phpのprefixがつかずにinitメソッドで対応

#@# lang: .brush: .php;
//emlist{
public static function _init(){
    static::$_db = \Database_Connection::instance('fuelpress');
    static::$_table_name = \DB::table_prefix(static::$_table_name,'fuelpress');
}
//}

== 最後に

FuelPHPは既存のDBを扱いやすいんだな〜と思ったところでは、

 * 基本はormを継承して
 * $_propertiesでカラム定義して
 * アソシエーションはormを使うかQuery_Builderのjoinを使う

ormを継承していても複雑なアソシエーションの場合とかはDBクラスを使うとかいう選択肢もあるんじゃないかな〜と思います。

 * @<href>{https://github.com/milds/fuelpress/blob/master/classes/model/post.php,https://github.com/milds/fuelpress/blob/master/classes/model/post.php}

(定義だけはしつつも使ってないです…)@<br>{}

次は@<href>{https://twitter.com/tmd45,@tmd45}さんです。
執筆時点ではタイトル未定ですのでどんな内容か楽しみです！

//quote{
@<strong>{@ootatter}

Twitter: @<href>{https://twitter.com/ootatter,@ootatter}

Blog: @<href>{http://blog.milds.net/,http://blog.milds.net/}
//}

