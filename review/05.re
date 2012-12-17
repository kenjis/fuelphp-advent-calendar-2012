
= FuelPHPで日本人にやさしいフォームを作る @<href>{https://twitter.com/redsnow_,@redsnow_}


@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012} 5日目の@redsnow_です。
昨日は@sa2yasuさんの「モジュール分割の利点とスケルトンのリファクタリング」でした。@<br>{}


自分はValidationの小ネタについて書こうと思います。

== 結論から

#@# lang: .php style="font-family:monospace;"
//emlist{
$form = Fieldset::forge();

$form->add('hoge', 'フィールド名')
    ->add_rule(function($form) { return mb_convert_kana($form, 'n');})
    ->add_rule('valid_string', 'numeric')
    ->add_rule('max_length', 8);
//}


FuelPHPのValidationに使えるvalid_string numericは@<strong>{半角数字のみ}許可するバリデーションです。@<br>{}
 でも全角数字を入力された人に、「有効な文字列ではありません。半角数字 で入力して下さい。」というエラーメッセージを出したくない…


そんな時にadd_ruleにPHPのネイティブの関数か無名関数を突っ込むと解決できます。


@<href>{http://fuelphp.com/docs/classes/validation/validation.html,マニュアル}でもさらっと触れられています。

//quote{
The first parameter of the add_rule() method can contain PHP native function names, any valid PHP callback and Closures in addition to the provided validation methods. The method will get the value to be validated as its first argument and any further arguments can be given to the add_rule() method.@<br>{}
 We also provide a shorter syntax which is very limited in comparison. It will not accept array-callbacks, closures or parameters other than strings.
//}

=== 注意点 1


add_ruleは上から順番に処理されるので書く順番に気をつけないといけない。

#@# lang: .php style="font-family:monospace;"
//emlist{
$form->add('hoge', 'フィールド名')
    ->add_rule('valid_string', 'numeric')
    ->add_rule(function($form) { return mb_convert_kana($form, 'n');})
    ->add_rule('max_length', 8);
//}


これだと数字で入力して下さいエラーが出ちゃいます。

=== 注意点 2


処理結果を取得するにはvalidated()を使う。

#@# lang: .php style="font-family:monospace;"
//emlist{
$hoge = $form->validated('hoge');
//}


こちらだと全角の数字が取れちゃうのでご注意を。

#@# lang: .php style="font-family:monospace;"
//emlist{
$hoge = Input::post('hoge');
//}

=== 明日は


@<href>{https://twitter.com/ounziw,@ounziw}さんの「ユニットテストのすすめ」です。お楽しみに〜

//quote{
@<strong>{@redsnow_}


Twitter: @<href>{https://twitter.com/redsnow_,@redsnow_}


Blog: @<href>{http://nob-log.info/,http://nob-log.info/}
//}
