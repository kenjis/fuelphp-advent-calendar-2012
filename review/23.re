
= FuelPHP vs CodeIgniter @<href>{https://twitter.com/mukaken,@mukaken}


この記事は @<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012 : ATND} の 23日目の記事です。
昨日は@<href>{https://twitter.com/ts_asano,@ts_asano}さんの「Query Builderで小難しいSELECT文を書くためのノウハウ」でした。@<br>{}


本日の「FuelPHP Advent Calendar 2012」は @mukaken こと向井が担当させて頂きます。よろしくお願いします。
本日は「FuelPHP vs CodeIgniter」について書きます。
FuelPHPはCodeIgniterに関わった事のある人達が作ったFWですので、両FWは非常に似ています。
また、私自身、FuelPHP と CodeIgniter の両方のユーザなので、両方の比較記事を書きます。

== ライセンス、対応する PHP のバージョン、開発元


ライセンスや対応するPHPのバージョン、開発元などについての比較です。

//table[tbl1][]{
名称	本家ホームページ	ライセンス	対応するPHP	最新の安定バージョン	開発元
--------------------------------------------------------------------------------------
FuelPHP	@<href>{http://fuelphp.com,http://fuelphp.com}	MIT	5.3 以上	1.4	コミュニティ
CodeIgniter	@<href>{http://ellislab.com/codeigniter,http://ellislab.com/codeigniter}	CodeIgniter License	5.1.6 以上	2.1.3	アメリカのEllisLab
//}

=== 本家ホームページ

今年、両方ともリニューアルされました。特に大きな変化があったのはCodeIgniterで、URL が今年の12月になり変わり、ellislab.comの直下になりました。これは、何かの意図があるのかは不明ですが、より企業ベースで開発されている感が感じられるようになりました。

=== ライセンス

FuelPHP は MIT ですので何も問題ないでしょう。
CodeIgniter のライセンスは、現時点では CodeIgniter License ですが、今後は OSL 3.0 になる予定です。この OSL 3.0 といわれるライセンスが非常に難しく、変更になる件については一時期、本家のフォーラム(@<href>{http://ellislab.com/forums/viewthread/202562/,CodeIgniter changes license to OSL 3.0? / Forums / Community / EllisLab})や、いろいろなサイトで活発に議論されていましたが、ここ最近半年は議論している人を見かけなくなりました。

=== 対応するPHPのバージョン

FuelPHP は PHP 5.3 以上なので新しい機能を積極的に取り入れています。また、下位互換がないために簡潔にコードを記述する事ができます。少しの事なのですが、開発者にとっては気分が良いです。
CodeIgniter は 過去のしがらみがあるために、メンテナンスをしていると PHP 4 で記述されたコードをチラホラと見かけます。メンテナンス時などは気分が沈みます。

=== 運用時の問題

現在の PHP の Old stable は PHP 5.3.20 なので、メンテナンスされているサーバであれば両方とも問題なく動くと思います。
ですが、ごくたまに、 FuelPHP で開発していて開発環境は PHP 5.3 だったのに本番環境は PHP 5.2 だったので作り直した、という話を見かけますので注意です。
CodeIgniter は PHP 5.1.6 以上なので、大抵の環境では動くと思います。

== コミュニティ

=== FuelPHP

日本のコミュニティの活動は凄く活発です。今年、短期間で爆発的に増大しました。
私自身、「@<href>{https://www.facebook.com/jp.fuel,FuelPHP 日本語ニュース | Facebook}」でFuelPHPの記事についてウォッチしているのですが、ほぼ毎日、日本語で何かしらのブログが書かれています。


日本以外ではブラジル、アジア諸国で人気が高いと感じます。
また、本家フォーラムの方もより一層活発になってきています。

=== CodeIgniter

日本では、それほど活発ではありませんが、世界的にはかなり巨大なコミュニティになっています。
日本以外だと、FuelPHPと同じくブラジル、アジア諸国がとくに活発だと感じます。
本家フォーラムは活発過ぎてカオス状態になっています。何が何だか分からない状態です。

== 注目度


注目度の比較です。
Web での注目度の指標として「Google トレンド」が使われるのを、よく見かけますが、今回は開発者に注目されている指標として近いであろう「はてなブックマーク」と「Stack Overflow」で比較してみました。
やはり、実際の運用や業務で使用していて、困った事がでてきたときに開発事例が豊富にあり探せるというのは心強いです。

=== はてなブックマーク

日本国内で使われているであろうサービスで、Web系サービスの人気度が分かるといえば、はてなブックマークです。
ブックマーク数が1user以上で、それぞれのタグ名別で比較しました。

 * @<href>{http://b.hatena.ne.jp/search/tag?q=fuelphp&users=1&sort=recent,タグ「fuelphp」を検索 - はてなブックマーク}
 ** FuelPHP : 1,195

 * @<href>{http://b.hatena.ne.jp/search/tag?sort=recent&q=codeigniter&users=1,タグ「codeigniter」を検索 - はてなブックマーク}
 ** CodeIgniter : 1,342



ブックマーク数の結果は大差ありませんでした。
CodeIgniter より FuelPHP の方が新しいフレームワークという点を考慮すると、短期間で数が増えた FuelPHP の方が勝っています。


=== Stack Overflow


@<href>{http://stackoverflow.com/,Stack Overflow} は世界中のプログラマーが連日連夜、技術的な疑問を投稿しているサイトです。
Stack Overflow でのタグの数を比較してみました。

 * FuelPHP : 198
 * CodeIgniter : 15,047



やはり、世界的にみると CodeIgniter が強いみたいです。 FuelPHP はこれからといった感じでしょうか。

== ドキュメントの読みやすさ

 * @<href>{http://fuelphp.com/docs/,FuelPHP Documentation}
 * @<href>{http://ellislab.com/codeigniter/user-guide/,Welcome to CodeIgniter : CodeIgniter User Guide}



両方とも、とても使いやすいドキュメントページだと思います。FuelPHP や CodeIgniter が、他のフレームワークと比べて優れている点は、やはりドキュメントの読みやすさだと思います。


両方ともすばらしいのですが、FuelPHP の方が目次をマウスオーバすれば色分けされて表示されるなど、より見やすいように工夫されています。

== パフォーマンスの比較


パフォーマンスの比較をしてみました。

=== 前提

 * FuelPHP 1.4
 * CodeIgniter 2.1.3


//quote{
Model Name: MacBook Air


Model Identifier: MacBookAir4,1


Processor Name: Intel Core i5


Processor Speed: 1.6 GHz


Memory: 4 GB


PHP Version 5.3.15


Apache/2.2.22


APC なし
//}


localhost にて、それぞれのFWはデフォルト設定でコントローラーに "Hello World!" と表示する単純な例で比較しました。


比較ツールは siege を使い、下記の設定で実行しました。

//cmd{
$ siege -b -c 10 -t 3S http://localhost
//}

=== 結果

 * FuelPHP
 ** Transaction rate : 93.06 trans/sec
 * CodeIgniter
 ** Transaction rate : 155.67 trans/sec



(Transaction rate : 数値が高い方が、よりアクセスがさばけている)


パフォーマンスの比較では FuelPHP より CodeIgniter の方が性能が良いという結果になりました。FuelPHP の方が何かと便利ですが、速いとなると CodeIgniter の方です。


ただ、ある程度のアクセスが見込めるようなパフォーマンスが気になるサイトとなると、リバースプロキシを使ったりとWebのシステム全体で最適化するので、今回比べたような Web FW 自体の純粋な性能は問題にならないかもしれないです。

== 案件での採用のしやすさ

=== 実案件での国内での代表的な採用実績

 * FuelPHP
 ** @<href>{http://animita.tv/top,あにみた！ あなたが見たアニメを楽しく記録するサービス}
 * CodeIgniter
 ** @<href>{http://www.mtvjapan.com/home,音楽のことなら MTV JAPAN! | 邦楽・洋楽・イベントなどの音楽エンターテイメント}



上記以外の、日本でのビジネス的な実案件での採用実績を調べようとしたのですが公開されている企業が少なく分かりませんでした。
ですので、PHP のイベントや IT 勉強会で聞いた、私の知る限りで書きます。


現在、実稼働しているサービスで多く使われている方は CodeIgniter でした。


ただし、小規模な受託案件(開発メンバは1から3人)や自社サービスなどの開発環境を開発者が自ら自由に選べる新規案件では FuelPHP を採用したという話をよく聞きました。
現時点で実稼働しているサービスは CodeIgniter で、新規案件では FuelPHP を採用するという流れみたいです。


開発メンバーの集めやすさでは FuelPHP にしろ CodeIgniter にしろ学習コストが低いので、PHPプログラマーならすぐに開発メンバーに加われるので問題ないと思います。

== イースター・エッグ


イースター・エッグとはプログラムの裏技の事です。


FuelPHP には Mac 限定で喋る機能や、 oil から表示されるロボットなど、いろいろな楽しい仕組みが提供されています。
CodeIgniter には裏技的なものがありません。
イースター・エッグ については FuelPHP の圧勝です！
少しの事ですが、楽しく開発できる事は大切ですね。

== 機能面＆まとめ


機能面の比較に関しての詳細な比較は、Google で検索すると、すばらしい記事を書かれたブログが多数ヒットします(例えば @<href>{http://ilikekillnerds.com/2012/10/codeigniter-vs-fuelphp-revisited/,Codeigniter vs FuelPHP (Revisited) | I Like Kill Nerds} など)。
ですので、詳しくはそちらを参考にしていただくとして、今回は私が感じた事を書きます。@<br>{}


機能の多さは FuelPHP > CodeIgniter です。FuelPHP にあって CodeIgniter にない機能は、大抵は CodeIgniter の本家フォーラムを探せば同じような機能の追加方法(ハック)が見つかります。しかし、CodeIgniter のコアを改造しなければいけないものもあり、そうするくらいなら最初から、ある程度まとまっている FuelPHP を採用した方がいいように思います。@<br>{}


新規案件で FuelPHP か CodeIgniter を採用するのを迷っているなら、 思いきって FuelPHP を採用するのもいいかもしれません。FuelPHP の方が凄く開発が活発で新機能が積極的に取り入れられていますし、日本のコミュニティが活発だからです。
そして、何より開発時のコードがシンプルに保たれていて開発者の気分が良いからです。

== 来年の抱負


今年は「FuelPHP 勉強会 東京」を2回開催できたのはよかったのですが、継続開催ができませんでした。来年こそは継続して開催したいと思います。やはり、OSSにおいてコミュニティの継続活動は大事だと思います。


また、来年は本家の FuelPHP Github へのPRを積極的にしたいと思います。@<br>{}


明日は@<href>{https://twitter.com/kurotaky,@kurotaky}さんです。お楽しみに！

//quote{
@<strong>{@mukaken}

Twitter: @<href>{https://twitter.com/mukaken,@mukaken}

Blog: @<href>{http://d.hatena.ne.jp/mukaken/,http://d.hatena.ne.jp/mukaken/}
//}
