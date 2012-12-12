
= モジュール分割の利点とスケルトンのリファクタリング @<href>{https://twitter.com/sa2yasu,@sa2yasu}


@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012} ４日目担当の@<href>{https://twitter.com/sa2yasu,@sa2yasu}です。


昨日は@<href>{https://twitter.com/mkkn_info,@mkkn_info}さんの「FuelPHP を使ったモジュール開発入門」でした。


今日はモジュール分割の話と
oilコマンドで作られるスケルトンコードに不満があったので、それを解消するFuelTaskを作った話になります。

== モジュール化のメリット


@<href>{http://press.nekoget.com/fuelphp_doc/general/modules.html,ここ}に書いてある通りなのですが、

//quote{
モジュールとは、独立した MVC 要素のグループのことです。モジュールを使うと、コードを再利用しカプセル化することが可能になります。 
モジュールは、通常、アプリケーションディレクトリの下の modules ディレクトリに置かれます。
結構な量のコードをもつ大きなプロジェクトを動かしている場合、 モジュールを使ってみてはいかがでしょうか。 
モジュールは、物事をきちんと秩序立てて整理する助けになるでしょう。


例えばブログモジュールやアルバムモジュールのように、モジュールは独立して動作させることが可能です。
グローバルなコードに対するいかなるアクセスも行わず、モジュールのコントローラに対して直接ルーティングすることが出来ます。 
モジュールは、純粋な HMVC コンテクストの中で使うことも出来ます。その場合、
ページ生成に必要な諸結果 (の一部) を得るために、コントローラは別のコントローラを呼び出すことが出来ます。
//}


上記から考察するモジュール化のメリットは

 1. 独立したモジュールは再利用が可能
 1. 多人数での平行開発が容易(モジュール毎のチーム編成が可能)
 1. モジュール毎にディレクトリが分かれるので開発、保守性向上



と考えています。


実際のところ、つい先日までFuelPHPを用いて実案件でCMS構築をしていたのですが、コンテンツ部分はfuel/app/classesに、管理部分をモジュール化することでコードの見通しがよくなりました。
最初からfuel/app下に何もかも実装してしまうと後からモジュール分割する際にリファクタリングが大変だと思うので、開発初期の段階でサブシステムのイメージが決まっているなら、最初からモジュール化を検討する方がよいのかもしれません。

== oilコマンドで生成されるスケルトンコードへの不満


これは僕のやり方なのかもしれませんが、FuelPHPで開発する時は、初めにモデル設計やURI設計をざっくりメモ書きして、
oilコマンドでひたすらコードスケルトンを生成、設計通りにルーティング、リレーションされるかどうかを確認してから細部の実装に取り掛かります。
ですが、現在のoilコマンドはmodulesディレクトリ以下にスケルトンコードを生成できるように出来ていないんですね(泣


ちなみにバージョン2.0ではmodule指定に対応するようです。


@<href>{https://github.com/fuel/oil/issues/86,Make Scaffold support Modules ［Feature］}

== スケルトンコードをmodules下にリファクタリングするTask


ということでoilコマンドで生成されるスケルトンコードへの不満を解消するFuelTaskを作ってみました。
やっていることは単純で、対象のパスのソースをmodules下にコピーしてnamespaceをつけたり、置換しているだけです。

=== インストール


@<href>{https://github.com/ya-sasaki/fuel-module-refactor,https://github.com/ya-sasaki/fuel-module-refactor}


git cloneでもzip展開でも良いのですが、使うのはmodule_refactor.phpのみですので、これをfuel/app/tasksに置いてください。

=== Scaffold生成


手っ取り早くScaffoldで例示しますね。

#@# lang: .syntax-highlight
//emlist{
$ oil g scaffold sample_user user_id:varchar[10]
$ oil r migrate
//}


@<href>{http://localhost/sample/user,http://localhost/sample/user}にアクセスしてScaffoldが成功していることを確認

=== リファクタリングタスクの実行


以下のように実行できます。

#@# lang: .syntax-highlight
//emlist{
$ oil r module_refactor <module_name> <type> <name> <delete>
//}


第1引数はfuel/app/modules/<module_name>です。
第2引数は"controller" or "model"です。指定した方をリファクタリングします。controllerを指定した場合は、あればですが、対象のapp/views, app/classes/viewもリファクタリングします。
第3引数はoil g したときのクラス名です。上記Scaffoldの例だと"sample_user"ですね。
第4引数はリファクタリング後にスケルトンコードを削除するかどうかを指定します。デフォルトはfalseです。


ScaffoldはMVC全部あるので以下のようにタスクを実行します。

#@# lang: .syntax-highlight
//emlist{
$ oil r module_refactor test controller sample_user true
$ oil r module_refactor test model sample_user true
//}


タスク実行後は、以前のURLはPage Not Found。@<href>{http://localhost/test/sample/user,http://localhost/test/sample/user}でリファクタリングしたScaffoldを確認できます。


がっつりコード書いた後のTask実行は試してないですけど多分失敗します(笑)
一応READMEでは「oil g・・・」直後のTask実行を推奨していますのであしからず。
(module_refactor.phpの$replacesで置換文字列を定義しているので、ココを上手く設定してあげれば綺麗にリファクタリングできるかも)

#@# lang: .syntax-highlight
//emlist{
<?php
・・・・省略・・・
    private static $replaces = array(
        'controller' => array(
            'Controller_Template' => '\Controller_Template',
            'View::' => '\View::',
            'ViewModel' => '\ViewModel',
            'Viewmodel' => '\Viewmodel',
            'Response::' => '\Response::',
            'Input::' => '\Input::',
            'Session::' => '\Session::',
        ),
        'model' => array(
            'use Orm\Model;' => 'use \Orm\Model;',
            'Validation' => '\Validation',
        ),
        'view_model' => array(
            'Viewmodel' => '\Viewmodel',
            'ViewModel' => '\ViewModel',
        ),
        'views' => array(

        ),
    ); 
//}

== 参考URL

 * @<href>{http://d.hatena.ne.jp/dix3/20111212/1323660316,http://d.hatena.ne.jp/dix3/20111212/1323660316}
 * @<href>{http://press.nekoget.com/fuelphp_doc/general/modules.html,http://press.nekoget.com/fuelphp_doc/general/modules.html}



明日は@<href>{https://twitter.com/redsnow_,@redsnow_}さんの「FuelPHPで日本人にやさしいフォームを作る」です。
乞うご期待！

//quote{
@<strong>{@sa2yasu}


Twitter: @<href>{https://twitter.com/sa2yasu,@sa2yasu}


Blog: @<href>{http://d.hatena.ne.jp/ya_sasak__ja/,http://d.hatena.ne.jp/ya_sasak__ja/}
//}
