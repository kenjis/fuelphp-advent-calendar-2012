
= ユニットテストのすすめ @<href>{https://twitter.com/ounziw,@ounziw}


@<href>{http://atnd.org/events/33753,FuelPHP Advent Calendar 2012} 6日目です。昨日は「FuelPHPで日本人にやさしいフォームを作る」(@<href>{https://twitter.com/redsnow_,@redsnow_}さん)でした。今日はユニットテストのすすめです。公式マニュアル@<href>{http://fuelphp.com/docs/general/unit_testing.html,http://fuelphp.com/docs/general/unit_testing.html}を参考にしています。

== PHPUnitのインストール


FuelPHPでは、PHPUnitというテスティングフレームワークを利用しています。まずPHPUnitをインストールしましょう。


PHPUnitは、PEARインストーラを使用してインストールします。まず、pear.phpunit.deからインストールできるようにします。

//cmd{
$ pear config-set auto_discover 1
//}

続いてインストールします。

//cmd{
$ pear install pear.phpunit.de/PHPUnit
//}

詳しくは@<href>{http://www.phpunit.de/manual/3.7/ja/installation.html,第3章 PHPUnit のインストール}をごらんください。なお、Ubuntuでは、@<tt>{sudo apt-get install phpunit}でインストールできます。

== 簡単なユニットテストの例


FuelPHPのコアコード@<href>{https://github.com/fuel/core/blob/1.4/master/tests/num.php,https://github.com/fuel/core/blob/1.4/master/tests/num.php}から引用です。

#@# lang: .brush: .php; .title: .; .notranslate title=""
//emlist{
namespace Fuel\Core;

/**
 * Numeric helper tests
 *
 * @package     Fuel
 * @category    Core
 * @author      Chase "Syntaqx" Hutchins
 *
 * @group Core
 * @group Num
 */
class Test_Num extends TestCase
{

    /**
     * @see     Num::bytes
     */
    public function test_bytes()
    {
        $output = Num::bytes('200K');
        $expected = '204800';

        $this->assertEquals($expected, $output);
    }
...
//}


ユニットテストするには、TestCaseクラスを継承したクラスを作ります(元々のPHPUnitではPHPUnit_Framework_TestCaseクラスを継承しますが、FuelPHPではTestCaseを継承します)。


テストのクラス名は、@<tt>{Test_テストするクラス名}とする規約になっています。上の例ではTest_Numです。テストするクラスがModel_Loginなら、Test_Model_Loginと命名します。


このクラスで、test_で始まるメソッドを定義してください。上の例ではtest_bytesメソッドが定義されており、このメソッドは@<tt>{Num::bytes('200K');}の実行結果が@<tt>{'204800'}に等しいことを確かめています。assertEqualsは、PHPUnitのメソッドで、1つ目の引数と2つ目の引数が等しいかどうかを確かめます。

== ユニットテストを書くメリット


ユニットテストを書くメリットは、テストを実行するコストが下がるので頻繁にテストを実行しやすくなること、クラス毎にテストを書くのでバグの箇所を見つけ易いこと、等があげられます。


プログラムを実行した結果をブラウザで目視して動作確認する、という方式だと、頻繁にテストするのは大変ですし、バグの箇所を見つけて修正するのも大変です。


ユニットテストを書いておけば、FuelPHPをインストールしたルートディレクトリで

//cmd{
$ oil test
//}

コマンドでテストを実行することができます。またテストに失敗した場合、どのテストが失敗したかが分かります。バグがあった場合に早期発見／修正しやすくなります。

== まずはテストしやすいところから


いきなり全部のコードのテストを完璧に書く、というのは難しいでしょう。まずはテストしやすいところから始めれば良いと思います。PHPUnitには様々なテストメソッドが用意されていますが、まずはassertEquals、assertTrue、assertFalseぐらいで良いと思います。FuelPHPのコアコードのテストコードも、大半はassertEquals、assertTrue、assertFalseです。

 * @<tt>{assertEquals($expected, $actual)} : @<tt>{$actual} が @<tt>{$expected} に等しい
 * @<tt>{assertTrue($actual)} : @<tt>{$actual} が TRUE
 * @<tt>{assertFalse($actual)} : @<tt>{$actual} が FALSE


== テストのグループ化


@<tt>{oil test}を実行すると、コアを含む全てのテストを実行します。自分のアプリケーションをテストしたい場合には、アプリケーション部分のみのテストができると嬉しいですね。

 * 自分で書いたテストのクラスに、@<tt>{@group App} アノテーションを書く
 * テストを書いたファイルをapp/testsフォルダに置く
 * @<tt>{oil test --group=App}を実行する



で、グループ化してテスト実行できます。


アノテーションは、上の例のTest_numでは、@<tt>{@group Core} となっています。自分のアプリケーションには、@<tt>{@group App} を付けると、自分のアプリケーションのテストだけを実行することができます。App の部分は好きな名前を付けることができます。


明日は「FuelPHP の Fieldset クラスをこんな風に拡張してまーす、という話」(@<href>{https://twitter.com/omoon,@omoon}さん)です。お楽しみに。

//quote{
@<strong>{@ounziw}


@<href>{http://www.novius-os.org/,Novius OS} のコード貢献者


Twitter: @<href>{https://twitter.com/ounziw,@ounziw}


Blog: @<href>{http://ounziw.com/,http://ounziw.com/}
//}
