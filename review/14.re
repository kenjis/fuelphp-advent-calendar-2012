
= 就職活動サイトの構築にFuelPHPを使ったので事例紹介 @<href>{https://twitter.com/uemera,@uemera}


こんにちは。@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012}に参加します上村と言います。
Advent Calendarの前日記事は@<href>{https://twitter.com/yamamoto_manabu,@yamamoto_manabu}さんの「FuelPHP + eXcale」でした。


eXcaleという国産PaaSサービス上でFuelPHPを動かす方法を紹介しています。eXcaleは今なら無料だそうですよ！これはいいですね。@<br>{}


 さて、私は就職活動に関するマッチングサイトOfferBoxを開発しており、そのシステムでFuelPHPを利用しています。
ここではその事例紹介と、ログイン周辺の実装についてご紹介します。

== OfferBoxの紹介


@<href>{http://offerbox.jp/,OfferBox}という、就職活動マッチングサイトをFuelPHPで構築し、2012年9月から運用しています。
非会員用の紹介サイトはWordPressで作り、会員専用画面をFuelPHPで作っています。@<br>{}


ログイン画面はこちらです。

 * @<href>{https://offerbox.jp/offerbox/slogin,https://offerbox.jp/offerbox/slogin}


=== @<strong>{開発の流れ}

 * 2012/7 開発着手
 * 2012/9 α版リリース
 * 2012/12 β版リリース


=== @<strong>{開発規模}

 * さくらVPS 4G
 * Apache + MySQL + PHP
 * FuelPHP v1.2.1
 * MySQL Table数: 13
 * Controller数: 26
 * View数: 87



このOfferBoxというサイトでは、企業が自分のニーズに合った新卒の採用人材を検索し、適合する人材にコンタクトをとることができます。
学生とのコミュニケーションを図れるSNSのような機能を持っており、企業が学生に対してお気に入りをつけたりメッセージを送受信することができます。
気に入った学生が見つかれば、オファーをして面接の段取りを進めることができます。


//image[Untitled][]{
//}

== 2種類のログイン画面。その実装方法


このシステムは学生と企業の双方が使うシステムであり、それぞれログインが必要ですので、学生用のログインと企業用のログインを別々に用意しています。@<br>{}


FuelPHPではログイン処理を作る際のひな形として@<href>{http://fuelphp.com/docs/packages/auth/simpleauth/intro.html,SimpleAuth}があり、その枠組みを利用するとログイン機能を1から作らなくても済みます。
SimpleAuthを使ったログイン機能の実装の事例は、いくつか紹介されています。

 * @<href>{http://w.builwing.info/2012/02/28/fuelphp%E3%81%A7%E7%B0%A1%E5%8D%98%E8%AA%8D%E8%A8%BC%E3%82%B7%E3%82%B9%E3%83%86%E3%83%A0/,FuelPHPで簡単認証システム | WinRoad徒然草}
 * @<href>{http://btt.hatenablog.com/entry/2012/06/16/015506,FuelPHPでログイン機能をサクっと実装 – BTT's blog}
 * @<href>{http://9ensan.com/blog/programming/php/fuelphp/fuelphp-login-auth/,[FuelPHP] FuelPHPで作るログイン管理 | 9ensanのLifeHack}



しかし、複数のログイン機能を持ったサイト事例は無いみたいで、いろいろ探したのですが見つかりませんでした。
そのような状況で手探りで実装し、分からないことがいっぱいでしたが、なんとか作れましたのでご紹介します。

=== auth使用の宣言


fuel/app/config/config.php に定義します。

#@# lang: .brush:php
//emlist{
'packages'  => array(
    'auth'
),
//}


fuel/packages/auth/config/auth.php を fuel/app/config/ の下にコピーし、以下のように書きます。@<strong>{StudentAuth}(学生ログイン用)と@<strong>{CompanyAuth}(企業ログイン用)の2つのドライバを登録します。

#@# lang: .brush:php
//emlist{
return array(
    'driver' => array('StudentAuth', 'CompanyAuth'),
    'verify_multiple_logins' => false,
    'salt' => '(適当な文字列)'
);
//}

=== ログイン用テーブルの作成


fuel/packages/auth/config/simpleauth.php をコピーして、fuel/packages/config/studentauth.php にrenameし、以下のように書きます。

#@# lang: .brush:php
//emlist{
    'table_name' => 'student',
//}


これはテーブル名の定義ですが、usersとなっていたテーブル名をstudentという名前に変えてます。
同様に、fuel/packages/config/companyauth.php を作成し、以下のように書きます。

#@# lang: .brush:php
//emlist{
    'table_name' => 'company',
//}


こちらは企業ログイン用のテーブルです。
MySQLテーブルは、本家@<href>{http://fuelphp.com/docs/packages/auth/simpleauth/intro.html,SimpleAuthのマニュアル}に載っているSQL文を使います。テーブル名はstudentとcompanyで作ります。

#@# lang: .brush:sql
//emlist{
CREATE TABLE IF NOT EXISTS `student` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  `group` int(11) NOT NULL DEFAULT '1',
  `email` varchar(255) NOT NULL,
  `last_login` varchar(25) NOT NULL,
  `login_hash` varchar(255) NOT NULL,
  `profile_fields` text NOT NULL,
  `created` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`,`email`)
);
CREATE TABLE IF NOT EXISTS `company` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  `group` int(11) NOT NULL DEFAULT '1',
  `email` varchar(255) NOT NULL,
  `last_login` varchar(25) NOT NULL,
  `login_hash` varchar(255) NOT NULL,
  `profile_fields` text NOT NULL,
  `created` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`,`email`)
)
//}

=== 認証ロジックの作成


認証のためのロジックはfuel/packeges/の下に置くことになります。
このディレクトリはFuelPHPパッケージコアに含まれる部分なので、ここに手を入れるとFuelPHPコアとユーザ開発ロジックが分離できなくなるし、バージョンアップ時にも気を遣うので、僕はこのディレクトリはあまり触りたくないのですが、まぁでもここを触るしか方法が無いのでしょうがないですね。@<br>{}


fuel/packages/auth/bootstrap.phpは以下のようにします。StudentAuthとCompanyAuth関連の定義を追加しています。

#@# lang: .brush:php; .highlight:[9,10,14,15,22,23,24,25,26,27]
//emlist{
Autoloader::add_classes(array(
    'Auth\\Auth'           => __DIR__.'/classes/auth.php',
    'Auth\\AuthException'  => __DIR__.'/classes/auth.php',

    'Auth\\Auth_Driver'  => __DIR__.'/classes/auth/driver.php',

    'Auth\\Auth_Acl_Driver'     => __DIR__.'/classes/auth/acl/driver.php',
    'Auth\\Auth_Acl_SimpleAcl'  => __DIR__.'/classes/auth/acl/simpleacl.php',
    'Auth\\Auth_Acl_StudentAcl'  => __DIR__.'/classes/auth/acl/studentacl.php',
    'Auth\\Auth_Acl_CompanyAcl'  => __DIR__.'/classes/auth/acl/companyacl.php',

    'Auth\\Auth_Group_Driver'       => __DIR__.'/classes/auth/group/driver.php',
    'Auth\\Auth_Group_SimpleGroup'  => __DIR__.'/classes/auth/group/simplegroup.php',
    'Auth\\Auth_Group_StudentGroup'  => __DIR__.'/classes/auth/group/studentgroup.php',
    'Auth\\Auth_Group_CompanyGroup'  => __DIR__.'/classes/auth/group/companygroup.php',

    'Auth\\Auth_Login_Driver'          => __DIR__.'/classes/auth/login/driver.php',
    'Auth\\Auth_Login_SimpleAuth'      => __DIR__.'/classes/auth/login/simpleauth.php',
    'Auth\\SimpleUserUpdateException'  => __DIR__.'/classes/auth/login/simpleauth.php',
    'Auth\\SimpleUserWrongPassword'    => __DIR__.'/classes/auth/login/simpleauth.php',

    'Auth\\Auth_Login_StudentAuth'      => __DIR__.'/classes/auth/login/studentauth.php',
    'Auth\\StudentUserUpdateException'  => __DIR__.'/classes/auth/login/studentauth.php',
    'Auth\\StudentUserWrongPassword'    => __DIR__.'/classes/auth/login/studentauth.php',
    'Auth\\Auth_Login_CompanyAuth'      => __DIR__.'/classes/auth/login/companyauth.php',
    'Auth\\CompanyUserUpdateException'  => __DIR__.'/classes/auth/login/companyauth.php',
    'Auth\\CompanyUserWrongPassword'    => __DIR__.'/classes/auth/login/companyauth.php',
));
//}


Authパッケージには、ディレクトリはacl、group、loginの3種類あります。
aclとgroupのディレクトリは今回の要件には使わないので、必要なloginディレクトリの部分だけ拡張すれば良いと思っていましたが、acl、groupも同じように作らないとうまく機能しませんでした。
ということで以下のファイルを新規作成することになります。

//image[121211-0002][]{
//}

==== 学生ログインロジックの作成


@<strong>{studentauth.php}を作ります。simpleauth.phpをコピーしたものをベースに、そこから修正していきます。

 * simpleauth.phpの中で、"Simple"や"simple"となっているところをかたっぱしから"Student"、"student"に置換します。
 ** クラス名、設定名などあらゆるもの。大文字、小文字にも気をつけて変換するようにします。
 * Session変数は学生用と企業用で名前が重複しないように考慮する必要があります。

例:@<br>{}
\Session::set('username', $this->user['username']);@<br>{}
の場合は@<br>{}
\Session::set('studentusername', $this->user['username']);@<br>{}
のようにします。

 * @<strong>{studentgroup.php}を作ります。simplegroup.phpをコピーしたものをベースに、そこから同じように文字列置換します。
 * @<strong>{studentacl.php}を作ります。simpleacl.phpをコピーしたものをベースに、同じように文字列置換します。


==== 企業ログインロジックの作成


@<strong>{companyauth.php}を作ります。simpleauth.phpをコピーしたものをベースに、そこから修正していきます。


 * simpleauth.phpの中で、SimpleやsimpleとなっているところをかたっぱしからCompany、companyに置換します。
 * @<strong>{companygroup.php}を作ります。simplegroup.phpをコピーしたものをベースにします。
 * @<strong>{companyacl.php}を作ります。simpleacl.phpをコピーしたものをベースにします。

=== ログイン制御(Contoller)と画面(View)を作る


学生ログイン画面を作っていきます。

==== Controllerの作成

#@# lang: .brush:php
//emlist[fuel/app/class/controller/studentlogin.php]{
public function action_login()
{
    $data = array();

    // submitボタンによるPOST時
    if (Input::post())
    {
        // Authオブジェクトを取得します。引数でStudentAuthを指定します。
        $auth = Auth::instance('StudentAuth');

        if ($auth->login())
        {
            // 認証OKならログイン後のページへリダイレクトします。
            Response::redirect('studentindex');
        }
        else
        {
            // ログイン失敗時の処理
            $data['username']    = Input::post('username');
            $data['login_error'] = 'Wrong username/password combo. Try again';
        }
    }

    // 学生ログインページの表示
    echo View::forge('studentlogin',$data);
}
//}

==== Viewの作成

#@# lang: .brush:xml
//emlist[fuel/app/views/studentlogin.php]{
<?php echo Form::open( '/studentlogin/login'); ?>
ユーザ名<?php echo Form::input('username',  Input::post('username') ) ?>
パスワード<?php echo Form::password('password',  Input::post('password') ) ?>
<?php echo Form::submit('submit', 'ログイン',); ?>
<?php echo Form::close(); ?>
//}


//image[121212-0004][]{
//}


企業ログインは、@<strong>{companylogin.php}とします。上記ソースコードの student の部分がcompnayになるだけです。

=== ユーザ登録


ユーザ登録のメソッドをstudentlogin.phpに書きます。

==== Controller

#@# lang: .brush:php
//emlist[fuel/app/class/controller/studentlogin.php]{
public function action_add_user()
{
    if (Input::post())
    {
        $username=Input::post('username');
              $password=Input::post('password');
              $email=Input::post('email');

              // Authオブジェクトを取得します。引数でStudentAuthを指定します。
        $auth = Auth::instance('StudentAuth');
        // ユーザ作成
        $auth->create_user($username,$password,$email);
    }
    // 登録フォームの表示
    echo View::forge('studentadduser_user');
}
//}


 企業ユーザ登録も同様に、companylogin.phpに同じメソッドを作ります。上記ソースコードの student の部分がcompnayになるだけです。

==== View

#@# lang: .brush:xml
//emlist[fuel/app/views/studentadduser.php]{
<?php echo Form::open( '/studentlogin/add_user'); ?>
ユーザ名<?php echo Form::input('username',  Input::post('username') ) ?>
パスワード<?php echo Form::password('password',  Input::post('password') ) ?>
Email<?php echo Form::input('email',  Input::post('email') ) ?>
<?php echo Form::submit('submit', '登録') ?>
<?php echo Form::close(); ?>
//}


企業ユーザ用View(companyadduser.php)も同様です。studentの部分をcompanyに置換します。


//image[121212-0003][]{
//}

=== ログアウト


学生ログアウトのメソッドをstudentlogin.phpに書きます。

==== Controller

#@# lang: .brush:php
//emlist[fuel/app/class/controller/studentlogin.php]{
public function action_logout()
{
    //ログアウト
    Auth::logout();
    //ログアウト画面の表示
    echo View::forge('studentlogout');
}   
//}


企業ログアウトはcompnaylogin.phpに書きます。@<br>{}


Viewはこちら。

#@# lang: .brush:xml
//emlist[fuel/app/views/studentlogout.php]{
ログアウトしました
ログイン
//}


企業ユーザも同様です。studentの部分をcompanyにします。


//image[121212-0005][]{
//}

=== ログイン後の画面


ログイン後の画面とは、ログインに成功したら表示する最初の画面のことです。
ログイン中の状態の場合のみ表示が許される画面ですので、認証が成功しているかどうかをControllerでチェックする必要があります。@<br>{}


企業ユーザでログイン中にもかかわらず、学生用の画面を開いた場合は、誤動作を防ぐために企業側のログアウト処理をするようにしています。
実際のユースケースでは企業と学生の両方のアカウントを持つ人はいないのですが、開発の現場ではテスト時にそういう使い方もするので、一応その考慮を入れています。

#@# lang: .brush:php
//emlist[fuel/app/class/controller/studentlogin.php]{
// 事前処理
public function before()
{
    $this->auth = Auth::instance('StudentAuth');
    // ここで、企業ログイン中であればログアウトする。
    if( Session::get('companyusername') ){
        Auth::logout();
    }

    // ログインチェック
    if( !$this->auth->check('StudentAuth') ){
        // 認証に失敗したらログイン画面にジャンプ
        Response::redirect('/studentlogin');
    }
}

// ログイン後トップ画面
public function action_index()
{
    // ログイン後トップ画面の表示
    echo View::forge('studentindex');
}
//}


企業用も同様です。studentの部分をcompanyにします。companyの部分はstudentにします。

=== さいごに


このように、1つのサイトで複数のログイン機能を実装する場合は、SimpleAuthを単にコピーしてSimpleの部分の文字列を置換するだけではダメです。


追加作業として

 * セッションの名前が重複しないようにする。
 * 学生と企業の同時多重ログインをしないようにする。

などの考慮が必要になります。@<br>{}


 実際の製作段階では、単純に2つのAuthを作っただけではうまくいかず、うまくログアウトされなかったり、無限ループに陥ったりしていろいろ苦労しました。
 結局、SimpleAuth認証のロジック内を細かく精査して、不具合の原因となる部分を潰していったので、SimpleAuthを参考にしたと言っても全然Simpleにはいかなったです。
このノウハウが参考になれば幸いです。@<br>{}


次は@<href>{https://twitter.com/NEKOGET,@NEKOGET}さんの「FuelPHPドキュメント翻訳へのお誘い」です。日本語化の方々お疲れ様です。いつもありがたく使わせていただいております!

//quote{
@<strong>{@uemera}


Twitter: @<href>{https://twitter.com/uemera,@uemera}


Blog: @<href>{http://offerbox.jp/engineer/,http://offerbox.jp/engineer/}
//}
