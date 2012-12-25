
= FuelPHPでWEBAPIを作ってみる


@<href>{http://www.think-sv.net/blog/?p=957#respond,コメントをどうぞ}


この記事は@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012}の２５日目(最終日)です。@<br>{}
昨日は@<href>{https://mobile.twitter.com/kurotaky,@kurotaky}さんの@<href>{http://mo-fu.org/blog/2012/12/24/1/,「FuelPHPで簡単なWebアプリケーションを作ってみる」}でした。


本日の記事を担当する@<href>{https://twitter.com/Tukimikage,@tukimikage}です。@<br>{}
至らない点が多々あるかと思いますが、ご容赦ください。

== 1. はじめに

//hr


私がFuelPHPでWEBAPIを開発した時のネタをいくつか紹介します。@<br>{}
PHPでフレームワークを利用するのは、実はFuelPHPが初めてでして、私のように初めてフレームワークを利用する方の視点で書きたいと思います。@<br>{}
この記事のサンプルコードは@<href>{http://fuelphp.com/blogs/2012/09/fuelphp-releases-v1-3,FuelPHP Ver1.3} + MacOSX(Apache/2.2.21 + PHP 5.3.10)で動作確認を行っています。


@<strong>{尚、この記事中のサンプルでは、本来コントローラーとモデルで処理を分担させるべき所についても、簡略化のためにすべてコントローラー内に処理を記載しています。
ご了承ください。}

== 2. RESTコントローラーを使う

//hr


FuelPHPにはRESTfulAPIを簡単に実現するためのRESTコントローラーが用意されています。@<br>{}
使い方は以下のようになります。

//emlist{
<?php
class Controller_Fuelwebapi extends Controller_Rest //①
{
    public function get_name() //②
    {
        $_inputBuffer = Input::get('name') == null ? null:Security::htmlentities(Input::get('name'));
        $_outputBuffer = array('name' => $_inputBuffer == null ? '名無しさん':$_inputBuffer);
        return $this->response($_outputBuffer,200); //③
    }
}
//}


このサンプルは、getメソッドでクライアントからAPIにアップロードされたデータをエスケープ処理し、指定されたフォーマットでクライアントに返します。@<br>{}
IF文は省略形で書いていますので、必要あればググってください。


@<strong>{①の部分}@<br>{}
RESTコントローラーを継承してコントローラーを作成します。


@<strong>{②の部分}@<br>{}
通常のコントローラーはactionですが、RestコントローラーはHTTPメソッドを接頭語につけます。@<br>{}
この場合は、getリクエストを受け付けるアクションとなり、その他、POSTやPUTなどが使えます。@<br>{}
それぞれ指定したメソッド以外でリクエストすると、

//emlist{
HTTP/1.1 405 Method Not Allowed
//}


となり、勝手にエラーを返してくれます。


@<strong>{③の部分}@<br>{}
出力データを指定のフォーマットに変換し出力してくれます。@<br>{}
フォーマットを指定する一例としては、リクエストする際に、URIの末尾に@<b>{.json}などの出力指定をすると、自動的にFormatクラスのforgeメソッドを使って変換を行い出力してくれます。@<br>{}
また、それぞれのフォーマットにあわせた@<b>{Content-Type}もHTTPレスポンスヘッダーにつけてくれます。@<br>{}
注意点としては、どの形式で出力させたいかによって、入力するデータの形式が変わってきます。@<br>{}
例えば、もともとJSON形式のデータを何も考えずにJSONで出力すると、ダブルエンコードされてデータがおかしくなります。@<br>{}
その場合は、一度配列にデコードして出力させるか、Responseクラスのforgeメソッドを利用してノーエンコードで出力するようにしましょう。@<br>{}
どのようなデータを用意したらよいかは、Formatクラスの@<strong>{to_○○○メソッド}のマニュアルを参照してください。

== 3. HTTPレスポンスコードのカスタマイズ

//hr


WEBAPIのレスポンスを示すHTTPレスポンスコードですが、FuelPHPでは予め代表的なコードが規定されており、それ以外のオリジナルコードを使いたい場合等は、一工夫する必要があります。@<br>{}
先述したサンプル③の部分では、第二引数にHTTPレスポンスコードをしていしていますが、オリジナルのコードを使いたい場合、以下のように定義を追加する必要があります。@<br>{}
coreクラスのカスタマイズが可能であれば、方法1がスマートだと個人的には思います。


@<strong>{実現方法１}

//emlist{
ファイル：/fuel/core/classes/response.php
public static $statuses = array(
    :
    230 => 'OK request succeed', //追加
    :
    :
);
//}


@<strong>{実現方法２}

//emlist{
Response::$statuses += array(230 => 'OK request succeed'); //ソースの先頭や共通処理部に記載
//}


FuelPHPとは関係ありませんが、オリジナルコードを定義する際は、RFC2616の定義に従い既存コードと競合しないように定義しましょう。

== 4. マルチパートフォームデータを受信する

//hr


FuelPHPにはアップロードされたデータを取得する手段として、UploadクラスとInputクラスが用意されています。@<br>{}
これらのクラスを同時に利用することにより、マルチパートフォームデータを受信できます。@<br>{}
FileパートのデータをUploadクラスで、StringパートのデータをInputクラスでそれぞれ受信するサンプルを以下に示します。


@<strong>{プログラムの仕様}@<br>{}
* StringパートのAuthorフィールドの文字列を用いてtmpディレクトリ配下にファイル保存用ディレクトリを作成@<br>{}
* アップロードするファイルはxmlファイル(mimetype:application/xml)とし、拡張子とmimetypeの検査を行う@<br>{}
* アップロード結果をクライアントに返却する


@<strong>{入力ファイル}@<br>{}
* content1.xml — 中身はxmlファイル@<br>{}
* content2.json — 中身はxmlファイルだが拡張子はjson@<br>{}
* content3.xml — 中身はテキストファイル@<br>{}
* content4.xml — 中身はxmlファイル

//emlist{
public function post_upload()
{

    $_result = array();

    //マルチパートのStringパートを受信しディレクトリ名とする
    $_directoryname = Security::htmlentities(Input::post('Author'));

    //マルチパートのFileパートを受信するためにUploadクラスのコンフィグ設定
    $_config = array(
            'path' => APPPATH.'/tmp/', //ファイルの保存を許可するディレクトリを指定
            'create_path' => true, //ディレクトリが存在しなければ作成
            'auto_rename' => true, //ファイル名が重複した場合に自動でリネーム
            'ext_whitelist' => array('xml'), //拡張子xmlのみアップロードを許可
            'mime_whitelist' => array('application/xml') // mimetypeがapplication/xmlのみアップロードを許可
    );
    Upload::process($_config); //アップロード実行(コンフィグ指定が無ければ省略できる)
    $_files = Upload::get_files(); //アップロードしたファイルの情報を取り出す
    if(Upload::is_valid()){ //アップロードしたファイルの検査を行う
        //検査合格したファイルに対して実行
        foreach( $_files as $_key => $_value){
            Upload::save(APPPATH.'/tmp/'.$_directoryname.'/',$_key); //指定ディレクトリにファイルを保存
            $_result += array($_files[$_key]['name'] => 'Upload Successd');
        }
    }
    $_errorfiles = Upload::get_errors();  /検査不合格のファイル情報を取得
    foreach ($_errorfiles as $_key => $_value){ 
        //検査不合格だったファイルに対して実行
        foreach ($_value['errors'] as $_key => $_error){
                $_result += array($_value['name'] => $_error['error'].':'.$_error['message']);
        }
    }

    return $this->response($_result);
}
//}


実行結果(JSONで出力した場合)の例は以下の通り。

//emlist{
{
  "content1.xml" : "Upload Successd",
  "content4.xml" : "Upload Successd",
  "content2.json" : "103:Upload of files with this extension is not allowed",
  "content3.xml" : "107:Upload of files of this mime type is not allowed"
}
//}


補足ですが、FuelPHPのUploadクラスはmime-typeの判定について、HTTPヘッダーのContent-typeを利用するのではなく、PHPが一時領域に保存したファイルの実態をfinfo_file関数にかけてmime-typeを取得してくれているようです。@<br>{}
よって、HTTPヘッダーを偽装されてもきちんとファイルの実態にあったmime-typeを取得しファイルの検査を行ってくれます。

== 5. WEBAPIとtasksによる非同期バッチ処理

//hr


FuelPHPのtasks機能を用いてWEBAPIで処理を受付し非同期バッチ処理を行う事例を紹介します。


@<strong>{プログラムの仕様}@<br>{}
* WEBAPIのリクエスト時にgetメソッドでパラメーター(‘runmode’)を渡すことで同期/非同期処理を切り替える@<br>{}
* WEBAPIはリクエストを受けるとtaskを実行する@<br>{}
* taskは非同期っぷりが分かるように5秒間スリープした後画面出力を行う@<br>{}
* エラー処理などは省く


コントローラー部

//emlist{
public function get_runtask()
{
    //非同期処理は'> /dev/null &'で、処理をバックグラウンドに潜らせる
    $_run = Input::get('runmode') == 'asynchronous' ? '> /dev/null &':null;
    //oilコマンドを利用してタスクを実行
    system('php '.DOCROOT.'../oil refine originaltask ');
}
//}


タスク部(簡単な処理なのでコメントは省略)

//emlist{
<?php
namespace Fuel\Tasks;

class Originaltask
{
    public function run()
    {
        sleep(5);
        echo 'OK';
    }
}
//}


@<strong>{このサンプルは見ての通り、セキュリティ対策やtaskの準正常系処理、異常処理などは全く考えていないため、オンラインサーバー上では実行しないでください。
何が起きても筆者は責任を負いません。
また、クライアントからの入力をWEBAPI経由でtaskに直接渡す場合は、escapeshellarg()やescapeshellcmd()を利用して必ずエスケープ処理を行ってください。}

== 6. おまけ「RESTClient」について

//hr


WEBAPIの開発に限ったことではありませんが、テスト環境、デバッグ環境を整えることは重要ですね。


FuelPHPのテストについては、FuelPHP Advent Calendar 2012の6日目に@<href>{https://mobile.twitter.com/ounziw,@ounziw}さんが、@<href>{http://ounziw.com/2012/12/06/phpunit,「ユニットテストのすすめ」}という記事を書かれていますので、ぜひご一読ください！


今回紹介するのはデバッグを支援するツールです。@<br>{}
Viewを持たないWEBAPIはブラウザなどでデバッグするのは非常に骨がおれますと思います。(特にHTTPヘッダーを操作したりCookieの送受信をしたり…)@<br>{}
それらを簡単に実現するツールとして、私が今回選択したのは、@<href>{http://code.google.com/p/rest-client/,rest-client}というJAVAベースのツールです。@<br>{}
現在のバージョンではWindows、Macともに動作しますし、GUIも搭載しているので、関係者へのデモなどにも使えると思います。@<br>{}
有名なツールだと思いますが、まだ利用されたことがない方は是非！

== 7.終わりに

//hr


最後までお読みいただきありがとうございました。@<br>{}
@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012}はこれで終了となります。@<br>{}
駄文ではございますが、今年のAdvent Calendarに参加できたことを光栄に思います。


私を含め25名の有志による記事は如何でしたでしょうか。@<br>{}
2012年は日本においてFuelPHPの認知度が急上昇した年だったと思います。@<br>{}
 2013年はFuelPHPにとって、コミュニティ・ビジネス共に、今年以上に盛り上がる年になってくれる事を期待してやみません。


最後に、@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012}の発起人であり、様々なところでご尽力頂いている@<href>{https://twitter.com/kenji_s,@kenji_s}さんに感謝の意を述べさせていただきます。@<br>{}
 ありがとうございました。
