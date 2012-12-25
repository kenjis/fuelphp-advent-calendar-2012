= QueryBuilderで小難しいSELECT文を書くためのノウハウ @<href>{https://twitter.com/ts_asano,@ts_asano}


この記事は @<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012} の 22日目の記事です。
昨日は@<href>{https://twitter.com/konkon1234,@konkon1234}さんの「FuelPHPで1サイトを作ってみて気が付いた点など」でした。@<br>{}


あさのひとしと申します。
今回はタイトルの通り、FuelPHPのQueryBuilderを使いこなすために悪戦苦闘した結果得られたエッセンスをノウハウとしてまとめてみました。
少し長いですが、どうぞよろしくお願い致します。

== はじめに


さて、みなさんは普段どれくらいの「SQL文」を書かれるでしょうか？
ORMが登場して、ほとんどSQL文自体を書くことは少なくなったのかなと思います。
ただ、時には効率重視にする必要がある場合もあります。
その場合はDBからデータを取得して処理するのではなく、できる限りの処理をデータベース側に行わせたいものです。@<br>{}


そういったときにサブクエリが効いてくるわけですが、これをFuelPHPのQueryBuilderで表現するには？というのがこの記事の目的です。
ORMの有用性はご存知のとおりと思いますので、ここではORMには触れずに参ります。@<br>{}


以下すべてのソースコードは、PHP5.4のビルトインサーバ(PHP5.4.9)＋FuelPHP 1.4、MySQLバージョン5.5.28にて動作確認済みです。
以下すべてのソースコードはそれ単体で動くものではなく、また著作権表示・許諾表示を怠っておりますが、MITライセンスを適用させて頂きます。

== 0. 環境準備


まずは、まっさらのFuelPHP 1.4環境を用意します。@<fn>{fuel-dev}

//footnote[fuel-dev][FuelPHPの環境準備手順は省略します。]

どのようなSQL文が生成されているか分かりやすくなるよう、profilingをtrueにしておきましょう。
データベースのprofilingもtrueにしておく必要があります。



#@# lang: .syntax-highlight
//emlist[fuel/app/config/development/config.php]{
return array(
    'profiling' => true,
);
//}



#@# lang: .syntax-highlight
//emlist[fuel/app/config/development/db.php]{
return array(
    'default' => array(
        'connection' => ({省略}),
        'profiling' => true,
    ),
);
//}


今回は説明用に、oilを使用してテーブルを4つ作成します。
簡単な図書館の蔵書・貸出管理システムをイメージして頂ければと思います。

 * 出版社テーブル 出版社名
 * 書籍テーブル 書籍名と出版社ID
 * 貸出テーブル 貸出日時、書籍IDと会員ID
 * 会員テーブル 名前と誕生日



ということで、コマンドラインで以下を実行します。

#@# lang: .syntax-highlight
//cmd{
$ oil g model publisher  name:string[10]
$ oil g model book  name:string[20] publisher_id:int[11]
$ oil g model member  name:string[10] birth_date:date
$ oil g model rental  rental_dttm:datetime book_id:int[11] member_id:int[11]
$ oil r migrate
//}


お手数ですが、これらのテーブルに適当にデータを投入して下さい。
ここまでで、環境準備は終了です。

== 1. QueryBuilderのおさらい


まずは、QueryBuilderについてのおさらいです。

#@# lang: .syntax-highlight
//emlist{
$search_book_id = \Input::post('book_id');
$search_member_id = \Input::post('member_id');

$query = \DB::select('*')->from('rentals');

if ($search_book_id)
{
    // 検索条件に本のIDがあれば、where条件に追加
    $query = $query->where('book_id', '=', $search_book_id);
}
if ($search_member_id)
{
    // 検索条件に会員IDがあれば、where条件に追加
    $query = $query->where('member_id', '=', $search_member_id);
}

$query = $query->order_by('rental_dttm', 'desc');
$result  = $query->execute();
//}


今更書くほどのことでもないと思いますが、条件に応じて動的にwhere条件を追加できるのが便利ですよね。
ただ、少し難しいSQL文、例えばMySQL関数を使おうとする場合には少しコツが必要になります。  

== 2. MySQL関数を使うためのノウハウ


クエリビルダはエスケープ処理を自動でかけてくれるため、場合によっては困る場合があります。
MySQL関数を使う場合には、エスケープ処理が行われないようにする必要があります。
そういう場合に使用できるのが、DB::expr() メソッドです。


たとえばMAX関数を使う場合は、こんな感じになります。

#@# lang: .syntax-highlight
//emlist{
$result = \DB::select(\DB::expr('MAX(`birth_date`)'))
    ->from('members')
    ->execute();
//}


しかし実は、以下の書き方でもOKです。
バッククォートをダブルクォートにするのがポイントです。

#@# lang: .syntax-highlight
//emlist{
$result = \DB::select('MAX("birth_date")')
    ->from('members')
    ->execute();
//}


これはselect句に限られませんので、where句で書いてもOKです。
たとえば「40歳以上の会員を取得」するには、こんな感じで書けばOKです。

#@# lang: .syntax-highlight
//emlist{
$result = \DB::select('*')
    ->from('members')
    ->where('FLOOR(CURDATE()+0 - DATE_FORMAT("birth_date", \'%Y%m%d\')+0 ) / 10000', '>=', '40')
    ->execute();
//}


カラム名をダブルクォートで囲う。コレがポイントです。
適切にエスケープすることで、シングルクォートも使えます。


ここでひとつ注意点ですが、これらの方法を使う場合、エスケープ処理が行われません。
それはつまり、ユーザーが入力した内容をこの内部に反映させる場合、SQLインジェクション攻撃を受ける可能性があるという事になります。@<fn>{sql-injection}

//footnote[sql-injection][ただし、私が試した限りはSQLインジェクション攻撃を引き起こすことはできませんでした。]

ユーザー入力をDB::expr() メソッド内またはそれに準ずる方法で処理する場合は必ずDB::escape() メソッドを使用し、以下のように対策を行うようにして下さい。

#@# lang: .syntax-highlight
//emlist{
// $date_strには、'20121222'のような文字列を期待する
$date_str = \Input::post('base_date_str');
$result = \DB::select('*')
    ->from('members')
    ->where(
        'FLOOR('. \DB::escape($date_str). '+0 - DATE_FORMAT("birth_date", \'%Y%m%d\')+0 ) / 10000',
        '>=',
        '40'
    )
    ->execute();
//}

== 3. サブクエリを使うためのノウハウ


さて、それでは「A社とC社が発行している本についての蔵書、各々最後に貸し出した日付、借りた人物を取得」してみたいと思います。

普通はこんなにややこしい処理を一文で書くことは少ないかもしれませんが、
「データベースでできることはデータベースで」というポリシーで処理してみるとこんな感じになります。

#@# lang: .syntax-highlight
//emlist{
$query = \DB::query(
    'select p.name as publisher_name, b.name as book_name, r.rental_dttm, m.name as member_name'.
    ' from books as b'.
    ' inner join publishers as p on b.publisher_id = p.id'.
    ' left outer join ('.
    '  select out_r.rental_dttm, out_r.book_id, out_r.member_id'.
    '  from rentals as out_r'.
    '  where (out_r.rental_dttm, out_r.book_id) in'.
    '   (select MAX(in_r.rental_dttm) as rental_dttm, in_r.book_id'.
    '    from rentals as in_r'.
    '    group by in_r.book_id)'.
    ' ) as r on b.id = r.book_id'.
    ' left outer join members as m on r.member_id = m.id'.
    ' where p.name in ("A社", "C社")'.
    ' order by p.id, b.id');
$result = $query->execute();
//}


100行200行のSQL文を見慣れている方であれば大したことはないですが、SQL文に慣れていないかたはぎょっとするかも知れません。  


それではこれをQBで書き換えてみましょう。

#@# lang: .syntax-highlight
//emlist{
$query = \DB::select( array('p.name', 'publisher_name'),
        array('b.name', 'book_name'), 'r.rental_dttm', array('m.name', 'member_name'))
    ->from(array('books', 'b'))
    ->join(array('publishers', 'p'), 'inner')->on('b.publisher_id', '=', 'p.id')
    ->join(array(DB::expr('(select out_r.rental_dttm, out_r.book_id, out_r.member_id'.
        ' from rentals as out_r'.
        ' where (out_r.rental_dttm, out_r.book_id) in'.
        '  (select MAX(in_r.rental_dttm) as rental_dttm, in_r.book_id'.
        '   from rentals as in_r'.
        '   group by in_r.book_id) )')
            , 'r'), 'left outer')->on('b.id', '=', 'r.book_id')
    ->join(array('members', 'm'), 'left outer')->on('r.member_id', '=', 'm.id')
    ->where('p.name', 'in', array('A社', 'C社'))
    ->order_by('p.id')
    ->order_by('b.id');
$result = $query->execute();
//}


これぐらい長い箇所がある場合は、無理にダブルクォートを使うより、素直にDB::expr() メソッドを使うことをおすすめします。
どうしてもDB::expr() メソッドを使いたくないという場合は、各カラム名をダブルクォートで囲むようにして下さい。

== 4. where句の右辺にMySQL関数を使いたい時のノウハウ


最後に、一番お伝えしたかったのがこのノウハウです。
where句の右辺にMySQL関数を使うと値扱いでクォートされてしまい、データを取得できません。
「直近30日の貸し出し履歴を書籍名、借りた人物込みで取得」する事を想定してみます。

#@# lang: .syntax-highlight
//emlist{
$query = \DB::select('r.id', 'r.rental_dttm',
        array('b.name', 'book_name'), array('m.name', 'member_name'))
    ->from(array('rentals', 'r'))
    ->join(array('books', 'b'), 'inner')->on('r.book_id', '=', 'b.id')
    ->join(array('members', 'm'), 'inner')->on('r.member_id', '=', 'm.id')
    ->where('DATE(r."rental_dttm")', '>=', 'ADDDATE( CURDATE(), INTERVAL -30 DAY )')
    ->order_by('r.rental_dttm', 'DESC');
$result = $query->execute()->as_array();
//}


この場合、where句は以下のように解釈されてしまいます。

#@# lang: .syntax-highlight
//emlist{
  WHERE DATE(r.`rental_dttm`) >= 'ADDDATE( CURDATE(), INTERVAL -30 DAY )'
//}


しかしこれも小技を効かせれば、解決可能です。

#@# lang: .syntax-highlight
//emlist{
$query = \DB::select('r.id', 'r.rental_dttm', array('b.name', 'book_name'), array('m.name', 'member_name'))
    ->from(array('rentals', 'r'))
    ->join(array('books', 'b'), 'inner')->on('r.book_id', '=', 'b.id')
    ->join(array('members', 'm'), 'inner')->on('r.member_id', '=', 'm.id')
    ->where('DATE(r."rental_dttm") >= ADDDATE( CURDATE(), INTERVAL -30 DAY )', '=', true)
    ->order_by('r.rental_dttm', 'DESC');
$result = $query->execute()->as_array();
//}


こうすると、where句が以下のように解釈されます。

#@# lang: .syntax-highlight
//emlist{
  WHERE DATE(r.`rental_dttm`) >= ADDDATE( CURDATE(), INTERVAL -30 DAY ) = '1'
//}


ぱっと見でわかりにくいのですが、「@<code>{DATE(r.`rental_dttm`) >= ADDDATE( CURDATE(), INTERVAL -30 DAY )}」がtrueのデータ、つまり「1」と評価できるデータが取得対象ということになります。

これは「@<code>{DATE(r.`rental_dttm`) >= ADDDATE( CURDATE(), INTERVAL -30 DAY )}」そのものとして処理される、という事になります。


この方法は、where in でサブクエリを使いたい場合にも応用が効くノウハウです。  

== おわりに


いかがでしたでしょうか？
FuelPHPにはマニュアルでは触れられていない、思わぬ機能がまだまだ隠されています。


当然ながらcoreのソースもPHPで書かれていますので、処理の中身を読むことができます。
気になる箇所があれば、時間を掛けて読み込んでみたいものです。
思わぬ発見があるかも知れませんよ？


この記事がFuelPHPでDBを扱う方にとって、お役に立てますと幸いです！@<br>{}


明日は@<href>{https://twitter.com/mukaken,@mukaken}さんの「FuelPHP vs CodeIgniter」です。お楽しみに！


# mukakenさん、いつもはてなスターありがとうございます！

 この場をお借りしてお礼申し上げます。

//quote{
@<strong>{@ts_asano}

(株)トゥ・ステップ勤務。趣味は電子工作。FuelPHPと自作ガジェットを繋ぐのが今の目標。

Twitter: @<href>{https://twitter.com/ts_asano,@ts_asano}

Blog: @<href>{http://d.hatena.ne.jp/ts_asano/,http://d.hatena.ne.jp/ts_asano/}
//}

