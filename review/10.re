
= FuelPHP用ソーシャルログイン専用認証パッケージDsAuthでNinjAuthをもっと簡単に利用する @<href>{https://twitter.com/EGMC,@EGMC}


@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012}参加記事です。@<br>{}
 昨日は@<href>{https://twitter.com/fushiroyama,@fushiroyama}さんの「FuelPHPで保守性の高いマルチデバイス対応を考える」でした。@<br>{}
 Fuelはコアクラスを拡張してアレコレするのがやりやすいですね。


10日担当の私(@@<href>{https://twitter.com/EGMC,EGMC})は自作の認証パッケージの紹介です。@<br>{}
 「@<href>{http://dasalog.eg2mix.com/fuelphp-%e5%8b%89%e5%bc%b7%e4%bc%9a-%e6%9d%b1%e4%ba%ac-vol-2%e3%81%a7oauth%e3%83%ad%e3%82%b0%e3%82%a4%e3%83%b3%e3%81%ae%e7%99%ba%e8%a1%a8%e3%82%92%e3%81%97%e3%81%a6%e3%81%8d%e3%81%9f/,FuelPHP 勉強会 東京 vol.2でOAuthログインの発表をしてきた}」の続きでもあります。

== ソーシャル連携のログイン機能


最近のWebサービスにおいてよくみかける「facebookでログイン」、「Twitterでログイン」のような外部サービスを利用したログインの仕組みがあります。@<br>{}
 FuelPHPで利用可能なパッケージとしてNinjAuthがありますが、NinjAuthは標準でSimpleAuth、Sentry、Wardenの各ユーザー認証パッケージと連携可能となっています。


標準のSimpleAuthなどをそのまま使っても良いのですが、基本的にどの実装もemail、パスワード項目が必須となっています。


ログイン方法がわからなくなった場合の対応や、ユーザーへの連絡のためにemail、パスワードを設定必須にするというのはサービスによっては大事なことですが、自分が作りたいちょっとした小物webサービスにとってはそもそもアイパス認証なくてもいいかな、ということも多く、かつユーザーテーブルもカスタマイズしたかったので自前で専用の認証パッケージをこさえてみました。

== DsAuth


@<href>{https://github.com/egmc/fuel-dsauth,https://github.com/egmc/fuel-dsauth}

 * @<href>{https://github.com/happyninjas/fuel-ninjauth,NinjAuth}に完全に依存した、IDパスワードによるログイン機能を持たないユーザー認証パッケージ
 * ライセンスはMITで公開


== 動作サンプル


こちらで@<href>{http://dsauth-sample.eg2mix.com/,サンプルページ}を公開しています。


※実際にログイン可能ですがサンプルなのでアプリは特に何もしません。必要なユーザー情報とトークンは取得しています。

== インストール方法


GitHubで@<href>{https://github.com/egmc/fuel-dsauth#installation,簡単なドキュメント}を公開しています。@<br>{}
 基本的に必要なパッケージを配置してマイグレーションを実施すれば準備OKです。@<br>{}
 ポイントはNinjAuthのAdapterとしてDsAuthを指定することです。


コントローラーは例えば下記のように実装して、fuel/app/classes/controller/auth.phpとして配置します。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
<?php
class Controller_Auth extends \DsAuth\Controller {

    public function action_logout() {
        DsAuth::logout();
        Session::set_flash('flash_message', 'ログアウトしました');
        Response::redirect('/');
    }

    public function action_linked() {
        Session::set_flash('flash_message', 'アカウントの紐付けをしました');
        Response::redirect('/');
    }

}
//}

== 認証情報の取得


ログインに成功するとセッションに取得したユーザー情報が保持されます。@<br>{}
 ログイン情報へのアクセスはDsAuthクラスの各静的メソッドを経由して行います。


ログイン状態の確認

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
// ログインの有無を取得
DsAuth::is_logged_in()
//}


ログインユーザー情報の取得

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
// ログインユーザーレコードを配列で取得
DsAuth::get_user_info()
//}


なお、DsAuthクラスはFuelのコアクラス同様グローバルにエイリアスされているので、名前空間を都度指定する必要はありません。

== 指定可能なオプション

 * db_connection dbコネクション
 * login_hash_salt ログインチェック用ハッシュソルト
 * table_columns ログイン時に取得するカラムリスト
 * table_name ユーザーテーブル名
 * always_confirm_username 新規登録時に常にユーザー名を入力させる
 * allow_duplicated_username ユーザー名の重複を許可する
 * auto_modify_userinfo 外部サービスから取得した情報を自動で修正する


== 今後の予定

 * FuelPHP 1.4での確認
 * エラーチェックが甘いのでなんとかする
 * デフォルトのcookieセッションの場合、有効なcookieが送られるとDBの状態に関係なくログイン状態になってしまうのでそのあたりを考える
 * 小規模サービスで実際に使ってみる



とりあえず動くと思いますが現状はまだ甘い実装となっているので、是非フィードバックを頂ければと思います。

//quote{
@<strong>{@EGMC}


Twitter: @<href>{https://twitter.com/EGMC,@EGMC}


Blog: @<href>{http://dasalog.eg2mix.com/,http://dasalog.eg2mix.com/}
//}
