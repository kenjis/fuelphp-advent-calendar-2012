
= FuelPHP + eXcale @<href>{https://twitter.com/yamamoto_manabu,@yamamoto_manabu}


@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012}の13日目担当の@<href>{https://twitter.com/yamamoto_manabu,@yamamoto_manabu}です。
 昨日は@<href>{https://twitter.com/seltzer,@seltzer}さんの「Coda + FuelPHPでお手軽開発環境を作る」でした。


私は、Eclipse→NetBeans→PhpStormをお試し中。と定番IDEを渡り歩いてきたのですが、記事を読ませて頂くとCodaもあるある機能は網羅してそうですし、iPad版もあるようなので、ちょっと浮気してみようと思います。@<br>{}


さて、今日で13日目といことで折り返し地点を迎えた訳ですが、本日のお題は今年10月にTISさんが公開したPaaSサービス@<strong>{「eXcale(えくすけーる)」}です。
 気にはなっていたものの触れず仕舞いだったのですが、今回「FuelPHP Advent Calendar 2012」に参加させて頂くあたって、題材にさせて頂こうと思います。@<br>{}


まずは簡単に「eXcale」のご紹介。オフィシャルサイトは@<href>{https://www.excale.net/,こちら}。
ほぼオフィシャルサイトからの抜粋ですが、以下のような感じ。

 1. 「eXcale」は、オートスケール、負荷分散の機能を有したPaaSサービスです
 1. インスタンスに割り当てられるメモリは最大268MB
 1. 対応言語はRuby、Java、PHP、Node.js。PHPのバージョンは5.3。フレームワークの制限はありません
 1. データベースはMySQL 5.5。容量は500MB
 1. アプリケーションデプロイはGit、ブラウザからのアップロードに対応しています
 1. β版ということで@<strong>{無料！}料金周りは2013年春頃にアナウンスがあるようです
 1. 国産ということで@<strong>{ドキュメントが日本語}。英語アレルギーの方には朗報です



という訳でPHPも5.3なので、FuelPHPが動くはず。やってみましょう。  

== アプリケーション作成


@<href>{https://www.excale.net/,eXcaleオフィシャルサイト}でサインアップを済ませるとアプリケーション一覧画面が表示されます。


//image[1-1024x597][eXcale アプリケーション一覧]{
//}


右上の青い「作成」ボタンをクリックして、アプリケーション作成画面に遷移したら、アプリケーション名を入力、言語はPHPを選択して「作成」ボタンをクリックします。
 ここで指定したアプリケーション名は、@<strong>{http://アプリケーション名.excale.net}のようにURLに利用されるので、慎重かつ大胆に決めましょう。

== アプリケーションパッケージの作成


今回はGitではなく、ブラウザからのアップロードでアプリケーションをデプロイしてみようと思います。
 ブラウザからアップロードする際は、tar.gz形式にする必要があるので、そのアプリケーションパッケージを作成します。
@<href>{http://fuelphp.com,FuelPHPオフィシャルサイト}からFuelPHPをダウンロードします。@<br>{}
# v1.1、1.2の頃から遠ざかっていたのですが、ずいぶんと印象変わりましたね…@<br>{}


ダウンロード時点ではzip形式になっているので、これをtar.gz形式に固め直します。


と、その前に、default_timezoneの設定が必須になったようなので、設定ファイルを修正します。
 「fuel/app/config/config.php」を以下のように修正します。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
return array(
    'default_timezone' => 'Asia/Tokyo'
);
//}


それでは気を取り直してtar.gz形式に。参考までに私の環境でのコマンドを。
 FuelPHPのバージョンは1.4を前提にしています。
 またtar.gz形式にする際、docsなど不要なファイル群も含めてしまっていますが、デモということで大目に見てくださいませ。

#@# lang: .brush: .bash; .title: .; .notranslate title=""
//emlist{
cd /fuelphp-1.4/
tar -czf ./app.tar.gz .
//}


アプリケーションパッケージができたら、早速アップロードしてみます。
 eXcaleのアプリケーション一覧で、作成したアプリケーションの行にあるアップロードボタンをクリック。
 作成したアプリケーションパッケージを選択し、アップロード。@<br>{}


エラーメッセージが表示されなければ、アップロード＆デプロイは成功と思われますが、結果はログからも確認することができます。
 アプリケーション一覧の詳細ボタンをクリック、ログ行の表示ボタンをクリック、ログ種類にシステムログを選択し、表示ボタンをクリック。で、ログを確認することができます。@<br>{}


こんな感じのログが出力されていればOKです。

#@# lang: .brush: .plain; .title: .; .notranslate title=""
//emlist{
Tue Dec 11 21:15:23 UTC 2012 [phptest] 2012-12-11 21:15:23 Stop existing instance..
Tue Dec 11 21:15:23 UTC 2012 [phptest] 2012-12-11 21:15:23 Create and start new instance..
Tue Dec 11 21:15:25 UTC 2012 [phptest] 2012-12-11 21:15:25 Instance stopped.
Tue Dec 11 21:15:29 UTC 2012 [phptest] 2012-12-11 21:15:29 Deploying..
Tue Dec 11 21:15:30 UTC 2012 [phptest] 2012-12-11 21:15:30 Deployed.
Tue Dec 11 21:15:34 UTC 2012 [phptest] 2012-12-11 21:15:34 Instance started.
//}


それでは、@<strong>{http://アプリケーション名.excale.net/public/index.php/hello}にアクセスしてみましょう。
 見慣れた画面が表示されたら無事成功です。

//image[2][FuelPHPのHello World]{
//}


ここまでの手順は@<href>{http://doc.excale.net/getting_started/getting_started_with_php.html,コチラ}の内容を簡略化したものなので、もっと詳しくという方はリンク先をご参照あれ。

== DB接続


もっと苦戦するかと思いきや、あっさりとトラブル無くデプロイできてしまったので、DB接続にもチャレンジしてみたいと思います。
DBの接続情報はアプリケーション詳細画面で確認できます。画像は諸々マスクしてます。

//image[3][eXcale アプリケーション詳細]{
//}


この内容を元に設定ファイルを修正します。
 「fuel/app/config/development/db.php」を以下のように修正します。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
<?php
/**
 * The development database settings. These get merged with the global settings.
 */
return array(
    'default' => array(
        'connection' => array(
            'dsn' => 'mysql:host=<DB接続先ホスト>;port=<DB接続ポート>;dbname=<アプリケーション名>',
            'username' => '<DB接続ユーザID>',
            'password' => '<DB接続パスワード>',
        ),
    ),
);
//}


次にMySQLの準備です。
 eXcaleのMySQLにデータをインポートする場合には、SQLファイルかmysqldumpで出力したファイルが利用可能とのことです。
 正確な情報は@<href>{http://doc.excale.net/getting_started/getting_started_data_import_export.html,コチラ}。@<br>{}


私はXAMPPのphpMyAdminを使って、これまたXAMPPのデモアプリが使っているcdcol.cdsのデータをSQL形式でエクスポートしてテストデータを作成しました。
 できあがったのがコチラ。

#@# lang: .brush: .sql; .title: .; .notranslate title=""
//emlist{
-- phpMyAdmin SQL Dump
-- version 3.2.4
-- http://www.phpmyadmin.net
--
-- ホスト: localhost
-- 生成時間: 2012 年 12 月 11 日 19:05
-- サーバのバージョン: 5.1.44
-- PHP のバージョン: 5.3.1

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- データベース: `cdcol`
--

-- --------------------------------------------------------

--
-- テーブルの構造 `cds`
--

DROP TABLE IF EXISTS `cds`;
CREATE TABLE IF NOT EXISTS `cds` (
  `titel` varchar(200) DEFAULT NULL,
  `interpret` varchar(200) DEFAULT NULL,
  `jahr` int(11) DEFAULT NULL,
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=10 ;

--
-- テーブルのデータをダンプしています `cds`
--

INSERT INTO `cds` (`titel`, `interpret`, `jahr`, `id`) VALUES
('Beauty', 'Ryuichi Sakamoto', 1990, 1),
('Goodbye Country (Hello Nightclub)', 'Groove Armada', 2001, 4),
('Glee', 'Bran Van 3000', 1997, 5);
//}


アプリケーション詳細画面で「データベース詳細」ボタンをクリックすると遷移するデータインポート/エクスポート画面で、作成したSQLファイルをインポートします。
これでDB接続する為の準備と、データの作成は終わりましたので、検証用のPHPコードを書いてみます。@<br>{}


ルーティングを追加。
 「fuel/app/config/routes.php」を以下のように修正します。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
<?php
return array(
    '_root_'  => 'welcome/index',  // The default route
    '_404_'   => 'welcome/404',    // The main 404 route

    'hello(/:name)?' => array('welcome/hello', 'name' => 'hello'),
    'db' => 'db/index',
);
//}


コントローラを作成。
 「fuel/app/classes/controller/db.php」を新規作成します。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
<?php

class Controller_Db extends Controller
{
    public function action_index()
    {
        $tests = DB::query('SELECT * FROM cds')->execute();
        Debug::dump($tests);
    }
}
//}


それでは、@<strong>{http://アプリケーション名.excale.net/public/index.php/db}にアクセスしてみましょう。
 以下のように表示されればDB接続成功です。

//image[4][FuelPHP Dbコントローラ]{
//}


如何だったでしょう？初見でも数時間でアプリケーションのデプロイ、DB接続に辿り着けたのではないでしょうか。
 HerokuやPagoda Boxもありますが、なんだかんだで日本語ドキュメントがあると、やっぱり手を出すに当たってのハードルは下がる印象です。
 商用利用に足るかどうかは不安ですが、Controller_Restを使って、個人作成スマホアプリの対向WebAPIを作る、といったシチュエーションにはバッチリなのではないでしょうか。


まだβ版ということで料金体系が不明ですが、そこはTISさんが頑張ってくれることを信じつつ、無料の間に遊び倒したいと思います！@<br>{}


さて、明日は@<href>{https://twitter.com/uemera,@uemera}さんの「就職活動サイトの構築にFuelPHPを使ったので事例紹介」です。
 事例紹介ということで、リアルな声が楽しみですね！

//quote{
@<strong>{@yamamoto_manabu}


Twitter: @<href>{https://twitter.com/yamamoto_manabu,@yamamoto_manabu}


Blog: @<href>{http://yamamoto.phpapps.jp/,http://yamamoto.phpapps.jp/}
//}
