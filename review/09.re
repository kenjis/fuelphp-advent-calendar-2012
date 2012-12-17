
= FuelPHPで保守性の高いマルチデバイス対応を考える @<href>{https://twitter.com/fushiroyama,@fushiroyama}

== FuelPHP Advent Calendar 2012


@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012} に参加してみました。こんばんは、 @<href>{https://twitter.com/fushiroyama,@fushiroyama} です。僕の担当は9日目です。昨日は @<href>{https://twitter.com/web2citizen,@web2citizen} さんの「FuelPHPのスパムフィルタパッケージ」でした。@<br>{}


9日目も張り切っていきましょう！

== マルチデバイス対応


ウェブエンジニアにとって「マルチデバイス対応」は避けて通れない課題だと思います。急速にスマートフォンの波が押し寄せている昨今ですが、まだまだ案件によってはフィーチャーフォン(いわゆるガラケー)は無視できないシェアを持っているからです。@<br>{}


私事ですが、先日入籍いたしました。挙式するにあたって妻に、

//quote{
「招待状の送り先住所や宿泊の有無・交通手段などをメモするのが大変だから、メールに貼り付けて送れるウェブシステム作ってよ。出来るでしょ、エンジニア！」
//}

と言われました。

これは勉強し始めたばかりのFuelPHPを使って何か作るには格好のチャンスです！

最初は「2時間で出来る！(キリッ」と勢い良く返答したのですが、よく考えると私の友人も妻の友人も少なからぬ割合のガラケーユーザーがいるのです。

//quote{
「弱ったな…仕事ですら色々苦労が絶えないマルチデバイス対応しないといけないじゃないか…」
//}

これが最初の正直な感想です。@<br>{}


しかしご安心ください。FuelPHPはさすがでした！

FuelPHPを使うといとも簡単にマルチデバイス対応なウェブサービスを構築することができたのです。本稿ではそのときのノウハウなどをご紹介できればと思います。@<br>{}


なお、本稿ではFuelPHPのHello World的なことは解説しておりません。FuelPHPの基本的な使い方やMVCの考え方などは、他の素晴らしい記事や @<href>{https://twitter.com/kenji_s,@kenji_s} さんの著書『@<href>{http://tatsu-zine.com/books/fuelphp1st,はじめてのフレームワークとしてのFuelPHP}』等をご参照ください。

== 保守性の高いウェブサービスとはなんだろうか


FuelPHPでマルチデバイス対応を考えた時、まず真っ先に思いつくのが「デバイスごとにコントローラを分ける」という方法です。

 * http://shiroyama.us/service [PC]
 * http://shiroyama.us/service_sp [SmartPhone]
 * http://shiroyama.us/service_mb [FeaturePhone]



といった具合です。


FuelPHPではControllerクラス群を継承し、対応するパスのPHPクラスを用意するだけでいとも簡単に以上のような対応が可能です。ところがこの方法は個人的にはオススメできません。

これは普段ウェブサービスに関わる身として実感を持って断言できるのですが、同じ役目をするコントローラがデバイスが違うという理由だけで複数存在するとロクなことになりません。開発・保守・改良すべてにおいて膨大な負債として後々重くのしかかってきます。


しかも、今回の私の案件では「ひとつのURIをメールにコピペして送れること」という要求がクライアント(妻)から突き付けられています。尚更この方法は採れません。@<br>{}


では「コントローラの中でデバイスごとに場合分けする」方法にしましょうか。
以下のコードをご覧ください(条件式だけ擬似コードです)。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
    public function action_index() {
        $view;
        if ($is_feature_phone) {
            $view = View::forge('mobile/marriage/index');
        } elseif ($is_smart_phone) {
            $view = View::forge('smartphone/marriage/index');
        } else {
            $view = View::forge('pc/marriage/index');
        }
        $view->set('hoge', $hoge);
        $view->set('fuga', $fuga);
        $this->template->content = $view;
//}


そんなに悪くはなさそうです。
ただ、処理内容が明らかに異なるときに場合分けするのは自然なのですが、単にViewを出し分けるためだけにif elseをダラダラと書いてしまっているのがいかにも面倒です。
これをすべてのコントローラ内で書いていくと思うと目眩がしてきますね。@<br>{}


やはり、

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
    public function action_index() {
        $view = View::forge('marriage/index');
        $view->set('hoge', $hoge);
        $view->set('fuga', $fuga);
        $this->template->content = $view;
//}


この方が処理内容に集中できそうです。@<br>{}


本稿のテーマは「保守性の高いマルチデバイス対応」です。
したがって本エントリのゴールのひとつは、このようなシンプルなコードで "Viewだけデバイスによって自動的に選び分けられている" ところを目指すことです。
実は簡単にできるんですよ。そう、FuelPHPならね。

== FuelPHPのコアクラスの拡張性


個人的に感じているFuelPHPの最大の魅力は、コアクラス群の読みやすさ、拡張のしやすさです。


実は私自身、FuelPHPを本格的に使ったのは今回が初めてに等しく、何を隠そうPHPすら数年前に数行書いたことがある程度です。そんな私でもちょっと中身をみて簡単に拡張することができました。


これから順を追って、

 1. Agentコアクラスを拡張し、ガラケーやスマホ判定を追加
 1. Inputコアクラスを拡張し、ガラケーからの入力を適切に変換
 1. Responseコアクラスを拡張し、ガラケーへの出力を適切に変換
 1. Viewコアクラスを拡張し、自動的にデバイスごとにViewを選び分ける



までを解説していきたいと思います。

== Agentコアクラスを拡張し、ガラケーやスマホ判定を追加


マルチデバイス対応にあたり、まずはやはりデバイスの判定を適切にできなくてはなりません。
FuelPHPにはAgentクラスという便利なクラスがあり、アクセスしているユーザのブラウザや言語情報などを知ることができます。
しかしながら残念ことに、この島国のフィーチャーフォンを判定できるような機能までは備わっていません。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
Agent::is_mobiledevice()
//}


というメソッドはあるのですが、これは「PC以外」というぐらいのザックリした判定で、iPhoneもiPadもAndroidもガラケーも一緒くたに判定されてしまうようです。

ということで、日本でウェブサービスを展開する上で使えそうな「ガラケー判定」や「スマホ判定」を追加しましょう。@<br>{}


尚、この先のステップでコアクラスの拡張作業がいくつか発生するので、念のためコアクラスの拡張方法を以下にに箇条書きします。

 1. fuel/app/classes/ 以下に、コアクラスと同じ名前でphpファイルを作成
 1. class Agent extends \Fuel\Core\Agent {} のように、同名で継承
 1. 好きなメソッドを追加したりオーバーライドする
 1. bootstrap.php にAutoloader設定を追加(後述)



まずは以下のような agent.php を用意します。


※スマホ判定の部分は、時間の都合で @<href>{http://btt.hatenablog.com/,BTT’s blog} さんの「@<href>{http://btt.hatenablog.com/entry/2012/07/04/001254,FuelPHPのAgentクラスと拡張}」をそのまま流用させていただいております。とても参考になるエントリですので、是非合わせてお読みください。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
/* fuel/app/classes/agent.php */

<?php
class Agent extends Fuel\Core\Agent
{
    public static function _init()
    {
        parent::_init();

        // smartphone
        $sp_list = array(
                       'iPhone',
                       'iPod',
                       'Android',
                       'IEMobile',
                       'dream',
                       'CUPCAKE',
                       'blackberry9500',
                       'blackberry9530',
                       'blackberry9520',
                       'blackberry9550',
                       'blackberry9800',
                       'webOS',
                       'incognito',
                       'webmate'
                   );

        $pattern = '/'.implode('|', $sp_list).'/i';
        static::$properties['x_issmartphone'] = 
            preg_match($pattern, static::$user_agent) ? true : false;

        // docomo
        static::$properties['x_isdocomo'] = 
            preg_match("/DoCoMo/", static::$user_agent) ? true : false;

        // softbank
        static::$properties['x_issoftbank'] = 
            preg_match("/Vodafone|J\-PHONE|MOT|SoftBank/", static::$user_agent) 
            ? true : false;

        // au
        static::$properties['x_isau'] = 
            preg_match("/UP\.Browser/", static::$user_agent) ? true : false;

        // feature phone
        static::$properties['x_isfeaturephone'] = 
            (static::is_docomo() || static::is_softbank() || static::is_au());
    }

    public static function is_smartphone()
    {
        return static::$properties['x_issmartphone'];
    }

    public static function is_docomo()
    {
        return static::$properties['x_isdocomo'];
    }

    public static function is_softbank()
    {
        return static::$properties['x_issoftbank'];
    }

    public static function is_au()
    {
        return static::$properties['x_isau'];
    }

    public static function is_feature_phone()
    {
        return static::$properties['x_isfeaturephone'];
    }

}
//}


やっていることはシンプルで、UserAgent情報を正規表現でマッチさせてフラグを立ててstaticメソッドで参照しているだけです。ガラケーに関しては各キャリアがIPアドレス帯も公開しているので、IPアドレス判定も追加するとUA偽装にも対処できますね。@<br>{}


次に bootstrap.php を編集します。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
Autoloader::add_classes(array(
    'Agent'    => APPPATH . 'classes/agent.php',
    'Input'    => APPPATH . 'classes/input.php',
    'Response' => APPPATH . 'classes/response.php',
    'View'     => APPPATH . 'classes/view.php',
));
//}


こうすることでコアクラスの代わりに自作のクラスをロードすることができます。
Input、Response、View も、このあと追加していくので先走って書いておきました。

== ガラケーの文字コード問題


Input、Response の拡張の前にこの話はしておかないといけないなと思ったので書いておきます。ガラケー対応の際に忘れてはならないのが、文字コードです。


PCの近代的なブラウザやスマホのほとんどのブラウザは複数の文字コードに対応しています。これはヘッダで文字コードが正しく宣言されており、コンテンツが対応する文字コードで正しくエンコードされている限りにおいて、正しくマルチバイト文字を表示できることを意味します。


一方、ガラケーは特定の文字コードしか扱えないケースがまだまだ多いのが現状です。一応ウェブサービスを作る上での定石では「ガラケーはShift_JIS」というのがあります。ただしソフトバンクの一部機種などにUTF-8を扱えるものがあることから、今回は「ソフトバンクでUTF-8を扱えるものはUTF-8、それ以外のガラケーはShift_JIS」という方針で作っていこうと思います。


なお、FuelPHPのガラケー対応については、去年のAdvent Calendarで @<href>{http://d.hatena.ne.jp/codeigniter/,bossatama} さんが「@<href>{http://d.hatena.ne.jp/codeigniter/20111225/1324801262,FuelPHPでフィーチャフォン(ガラケー)対応について考えてみた}」というエントリで書いておられます。文字コードについても分かりやすく的確に言及されている記事ですので、合わせて読んでいただければと思います。@<br>{}


本エントリの例でも、モデル・ビュー・コントローラ・その他拡張したコアクラスなど、すべてUTF-8で記述するように統一してあります。他の文字コードが必要な境界面でのみ適宜変換を行うことで余計なトラブルに巻き込まれることが減ると思います。

== Inputコアクラスを拡張し、ガラケーからの入力を適切に変換


ガラケーをShift_JISにするという方針なので、ガラケーからの入力は当然Shift_JISで来ます。ところがPCその他のデバイスからの入力はUTF-8で来ます。


前述の保守性の高いウェブサービスに対する考察の通り、コントローラ内でいちいちガラケーかどうか判定して文字列を変換する処理を入れるのは避けたいです。
コントローラでは何も考えずに処理に集中するために、その前段階のInputクラスを拡張して文字コード変換を隠蔽します。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
/* fuel/app/classes/input.php */

<?php                  

class Input extends \Fuel\Core\Input
{                  

    protected static function hydrate()
    {   
        parent::hydrate();

        if (Agent::is_feature_phone() && !Agent::is_softbank()) {
            $encoded = array();
            foreach (static::$input as $key => $val) {
                $encoded[$key] = mb_convert_encoding($val, "UTF-8", "SJIS-win");
            }
            static::$input = $encoded;
        }
    }
}
//}


先ほど作成したAgentクラスを早速活用し、ガラケーかつソフトバンク以外の場合には入力をUTF-8に変換しています。
こうすることでPCその他のデバイスも含め、コントローラ内ではすべてUTF-8として扱えるので保守性が向上します。

== Responseコアクラスを拡張し、ガラケーへの出力を適切に変換


こちらは先程のInputの逆で、実際にユーザに表示する際に適宜文字コード変換して返す処理を隠蔽します。
こうすることでコントローラ内では何も考えずにUTF-8のまま文字列を扱い、Viewに渡った時には自動的に適切な文字コードに変換されます。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
/* fuel/app/classes/response.php */

class Response extends Fuel\Core\Response {

    public function body($value = false) {
        if ($value === false) {
            return $this->body;
        }

        // PC, smartphone
        if (!Agent::is_feature_phone()) {
            $this->set_header("Content-type", "text/html; charset=UTF-8");
            return parent::body($value);
        }

        // auのキャッシュ対策
        if (Agent::is_au()) {
            $this->set_header('Cache-Control', 'no-cache');
        }

        // softbankでUTF-8を表示可能なもの
        if (Agent::is_softbank() && Agent::accepts_charset('UTF-8')) {
            $this->set_header("Content-type", "application/xhtml+xml; charset=UTF-8");
            return parent::body($value);
        }

        // それ以外はShift_JISに変換して返す
        $this->body = mb_convert_encoding($value, "SJIS-win", "UTF-8");
        $this->set_header("Content-type", "application/xhtml+xml; charset=Shift_JIS");
        return $this;
    }
}
//}


書いてある通りなので特に解説することもないかと思います。

== Viewコアクラスを拡張し、自動的にデバイスごとにViewを選び分ける


いよいよ大詰めです！


本稿の最初の方で書いた、Viewの選び分けの隠蔽処理です。
まずはコードを見ていただき、その後で解説しようと思います。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{

class View extends \Fuel\Core\View
{
    public function set_filename($file) {
        if ( Agent::is_smartphone() ) {
            $file = 'smartphone/' . $file;
        } else if ( Agent::is_feature_phone() ) {
            $file = 'featurephone/'. $file;
        }

        return parent::set_filename($file); 
    }           
}
//}


…といってもやっていることはたったこれだけです。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
fuel/app/views
//}


の下にviewファイルを設置するかと思いますが、その時に smartphone、featurephone など任意の名前のディレクトリを作り、PCのビューと全く同じファイル構成にします。smartphone、featurephone という一階層深くなる以外は全く同じようにファイルを配置しないと正しく動かないことに注意してください。


たとえば http://shiroyama.us/marriage/index にアクセスしてもらってマルチデバイス対応するためには以下のようにファイルを配置します。

 * fuel/app/views/marriage/index.php
 * fuel/app/views/marriage/smartphone/index.php
 * fuel/app/views/marriage/featurephone/index.php



これで http://shiroyama.us/marriage にアクセスすると、まったく同じURIでUserAgent次第で違うViewファイルが適切な文字コードで出力されます！ワーパチパチパチ


コード上も

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
$view = View::forge('marriage/index');
//}


するだけで、スマホなら勝手に /smartphone/marriage/index が、ガラケーなら /featurephone/marriage/index が選択され、コントローラ内では余計なことをまったく考えずに処理に集中できる訳です。

== まとめ


いかがだったでしょうか。@<br>{}


趣味でちょっとしたものを作られている方の中には「マルチデバイス対応だけで何てしちめんどうくさいことをしているんだ…」と思われた方も少なくないかも知れません。

ただ、職業的ウェブエンジニアとして大規模かつ長期的メンテナンスが必要なものに関わった場合、こういった一手間が将来の何100時間を節約してくれるのも事実です。

こういったことが、むしろこの程度のコード片でデキてしまうFuelPHPの使いやすさ、改めて好きになりました！みなさんもFuelPHPで楽しいウェブ開発ライフを！

//quote{
@<strong>{@fushiroyama}


Twitter: @<href>{https://twitter.com/fushiroyama,@fushiroyama}


Blog: @<href>{http://shiroyama.us/blog/,http://shiroyama.us/blog/}
//}
