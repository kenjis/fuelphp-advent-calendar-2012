
= FuelPHPで作るFacebook診断アプリ


こんにちは。@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012}に参加します山田と言います。


Advent Calendarの前日記事は@<href>{https://twitter.com/NEKOGET,@NEKOGET}さんでした。@<br>{}
 @<href>{http://pneskin2.nekoget.com/press/?p=1044,「FuelPHPドキュメント翻訳へのお誘い」}@<br>{}
 日本語化の方々いつもありがとうございます。毎度お世話になっております。


さて、私は就職活動に関するマッチングサイトOfferBoxを開発しており、そのシステムでFuelPHPを利用しています。これは、14日目に上村さんに紹介頂いています(@<href>{http://offerbox.jp/engineer/440/,就職活動サイトの構築にFuelPHPを使ったので事例紹介})。@<br>{}
 さらに、このOfferBoxのFacebookページで、適性診断アプリを作りまして、ここでもFuelPHPを使いました。


ここでは、この適性診断アプリをFacebookアプリの作成事例としてご紹介します。@<br>{}
 　

== 1. Facebookの適性診断アプリ概要


まずは、作成したアプリケーションの概要を説明します。@<br>{}
 　

=== (1)まず一度、実施してみてください


PCのみで実行可能ですが、一度動作をお試しください。@<br>{}
 →@<href>{http://www.facebook.com/offerbox/app_105514016282622,こちら}


//image[a4ad0c2be86b9d98bfe1e2580359c488][]{
//}


12月2日に公開いたしましたが、おかげさまで半月足らずで600回以上実施いただいております。現在スマホにも対応していますが、これは別途機会があればご説明します@<br>{}
 　

=== (2)適性診断アプリの特徴


適性診断の内容は、二択の質問に12問答えると、就職活動での適性を15パターン分けてどれに当てはまるか結果を表示します。診断の流れは、以下のとおりです。

 1. 　いいね！を押すことで診断開始。
 1. 　診断を開始すると、Facebookにログインとウォールへのアクセス許可を取る
 1. 　診断を実施後、診断結果をユーザのウォールに投稿する。


== 2. Facebook側での準備


Facebookアプリを利用するために、@<href>{https://developers.facebook.com/,Facebook開発者ページ}で、アプリケーションを登録します。登録手順は、広くWeb上で紹介されています。


@<href>{http://socialmediaexperience.jp/2638,Facebookアプリを開発してみよう}


大きな枠組みとしては、サーバにFuelPHPでアプリを準備し、Facebookアプリにそのパスを登録する、という流れになります。@<br>{}
 　

== 3. FuelPHPによるアプリの作成(サーバ)


ここからが本題になりますが、大きくFuelPHPの「ファイル構成」「コントローラの関数」「コード説明」の順で説明します。@<br>{}
 　

=== (1)ファイル構成


今回は、データベース(Model)は使わないシンプルなケースで、コントローラとビューのみで説明いたします。ディレクトリfuel以下のファイルは以下のとおりです。
//table[tbl1][]{
@<strong>{ファイル名}	@<strong>{ファイルの内容}
-----------------
fuel/app/classes/controller/diag.php	適性診断用コントローラ
fuel/app/views/answer.php	診断結果ビュー
fuel/app/views/diag.php	メイン画面ビュー
fuel/app/views/fbliked.php	適性診断開始ビュー(いいね！を押した後)
fuel/app/views/fbnolike.php	いいね！催促ビュー(いいね！を押す前)
fuel/app/views/fbnoaccess.php	Facebook経由でない場合のアクセス禁止用ビュー
fuel/app/views/oauth.php	Facebook認証用ビュー
fuel/app/views/question.php	適性診断質問ビュー
--------------------------------------	----------------------------------------------
//}

=== (2)コード説明(コントローラ)


ここでは、コントローラのコードについて説明していきます。@<br>{}
 　

==== Facebookライブラリの準備


まず、Facebookのライブラリを準備します。FacebookのPHP−SDKは、@<href>{https://github.com/facebook/facebook-php-sdk,ここから}ダウンロードできます。fuel/app/venderに展開ください。@<br>{}
 　@<br>{}
 fuel/app/classes/controller/diag.php

#@# lang: .brush:xml
//emlist{
require_once APPPATH.'vendor/facebook-php-sdk/src/facebook.php';

class Controller_Diag extends Controller
{

     public function before()
     {

          // Facebook SDK
          $config = array();
          $config['appId'] = 'アプリケーションID';
          $config['secret'] = 'シークレットキー';
          $config['cookie'] = true;
          $this->facebook = new Facebook($config);
     }
//}

==== トップページでの振り分け


トップページでは、「Facebookページ経由でのアクセス」「いいね！が押されている状態」「ユーザのログイン状態」により、処理を分岐させます。@<br>{}
 いいね！が押されているかの判定は、このページで紹介されています。


@<href>{http://phpjavascriptroom.com/?t=topic&p=facebookpage_liked#a_liked,facebookのiframeアプリでいいねの有無によって表示を分ける方法}


この中の、”parse_signed_request.php”をコントローラーに取り込み、呼び出します。


Facebook認証からのcallbackで返ってきた場合は、$user_idにユーザ情報が入ってトップページが呼び出されますので、適性診断の質問用ビュー(question)を表示します。。


 fuel/app/classes/controller/diag.php

#@# lang: .brush:xml
//emlist{
    public function action_index()
     {
          // view定義
          $view=View::forge('diag');

          if (isset($_POST['signed_request'])){

              $signed_request=$_POST['signed_request'];
              $data = $this->parse_signed_request($signed_request,"アプリケーションID");

              if ($data["page"]["liked"]){  // 分岐1："いいね済の人に見せるコンテンツ";

                    //ログイン状態を取得
                    $user_id = $this->facebook->getUser();

                    if($user_id) {     //ログインしている状態
                         $view->mainview     = View::forge('question'); // 質問を表示
                    } else {
                         $view->mainview     = View::forge('fbliked');  // 診断開始を表示
                    }

              } else {                     // 分岐2: "未いいねの人に見せるコンテンツ";

                    $view->mainview     = View::forge('fbnolike');

              }

              $view->signed_request = $data;

          } else {                        // 分岐3：アプリのURLを直接たたいた場合

               $view->mainview     = View::forge('fbnoaccess');
          }

          return $view;
     }
//}

==== 適性診断開始


適性診断の開始では、Facebookでのログインと認証を受ける処理をします。ここでは、ビューの中で、Facebook認証を受けるためのURLをコールしています。


 fuel/app/classes/controller/diag.php

#@# lang: .brush:xml
//emlist{
     public function action_start()
     {
          // view定義
          $view=View::forge('oauth');

          return $view;
     }
//}

==== 結果表示


ここでは、適性診断の質問に対する回答がフォームでPOSTされますので、診断結果を判定し、ユーザのウォールに書き込む処理をしています。@<br>{}
 　@<br>{}
 fuel/app/classes/controller/diag.php

#@# lang: .brush:xml
//emlist{
     public function action_result()
     {
          $view=View::forge('diag');

          if (Input::method() == 'POST')
          {
               // POST投稿時の処理
               $val = Validation::forge();

               // バリデーション

               if( $val->run() ){

                    $vars = $val->validated();

                    //                    
                    //     適性診断の計算実行               
                    //                    


                    //ログイン状態を取得
                    $user_id = $this->facebook->getUser();

                   try{

                         // リクエスト時のContentTypをmultipart/form-dataとする
                         $this->facebook->setFileUploadSupport( true );                       

                        //ウォールへ投稿
                        $result = $this->facebook->api("/me/photos", "post", array(
                                         'access_token' => $this->facebook->getAccessToken(),
                                        'source' => '@' . $patternpath,
                                        'message' => "ウォールに投稿するメッセージ"
                                        ));

                    } catch (FacebookApiException $e) {
                       error_log($e->getType());
                       error_log($e->getMessage());
                    }

                    $view->mainview     = View::forge('answer', $this->data);

               } else {

                    // バリデーションエラー
                    Session::set_flash('error', $val->show_errors());

                    // view定義
                    $view->mainview     = View::forge('question');
               }
          } else {
               $view->mainview     = View::forge('question');
          }

          return $view;
     }
//}

=== (3)コード説明(ビュー)


ここでは、メイン画面用のビューとFacebook認証用のビューを示します。その他のビューは、Facebookアプリの内容に合わせて作成ください。@<br>{}
 　

==== メイン画面ビュー


このビューは、全画面で共通的に使うビューで、Facebookページのiframeのサイズに合わせます。ソース真ん中あたりの、$mainviewに各ページのコンテンツを表示させます。@<br>{}
 　@<br>{}
 fuel/app/views/diag.php

#@# lang: .brush:xml
//emlist{
<!DOCTYPE html>
<html lang="ja" xmlns:fb="http://ogp.me/ns/fb#" xmlns:og="http://ogp.me/ns#">
  <head>
    <meta charset="utf-8">
    <meta name="robots" content="noindex" />

</head>

<body>
     <?php echo $mainview ?>

<!-- Facebook JavaScript SDK -->
<div id='fb-root'></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/ja_JP/all.js#xfbml=1&#038;appId=アプリケーションID";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));</script>
<script type='text/javascript'>
  window.fbAsyncInit = function() {
    FB.init({
      appId : 'アプリケーションID',
      status : true,
      cookie : true,
      xfbml : true,
      logging : true
    });
    /* キャンバスのサイズ(px) */
   FB.Canvas.setAutoGrow();
  }
</script>
<!-- Facebook JavaScript SDK // -->
</body>
</html>
//}

==== Facebook認証用ビュー


このビューは、Facebook認証用のURLを作成し、コールするためだけのビューです。ビューの中で、以下のスクリプトを書いてください。@<br>{}
 　@<br>{}
 fuel/app/views/oauth.php

#@# lang: .brush:xml
//emlist{
      var oauth_url = 'https://www.facebook.com/dialog/oauth/';
      oauth_url += '?client_id=アプリケーションID';
      oauth_url += '&redirect_uri=' + encodeURIComponent('https://www.facebook.com/offerbox/?sk=app_アプリケーションID');
      oauth_url += '&scope=publish_stream,photo_upload'

      window.top.location = oauth_url;
//}

== 4. Facebookページへの登録


Facebookページのページタブに、作成したアプリケーションを登録する方法は、プラウザで以下を撃込んでください。


http://www.facebook.com/dialog/pagetab?app_id=アプリケーションID&next=アプリケーションのURL


 @<href>{http://weboook.blog22.fc2.com/blog-entry-309.html,ページタブアプリ(iframe)の作成とFacebookページへの追加方法}@<br>{}
 　

== 5. さいごに


このように、Facebookアプリの枠組みとしてFuelPHPを利用することで、アプリのアイデアさえあれば、簡単にFacebookアプロを作成でき、Facebookページへの集客に役立てる事が出来ます。@<br>{}
 このノウハウが参考になれば幸いです。


次は@@<href>{https://twitter.com/ootatter,ootatter}で「fuelphpで既存DBをあつかうとか」です。DBまわりの内容でとても興味深いですね。
