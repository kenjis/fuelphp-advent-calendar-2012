# 使い方

## 必要なソフトウェア

- PHP 5.3
- Pandoc https://github.com/jgm/pandoc
- md2review https://github.com/takahashim/md2review
- ReVIEW https://github.com/kmuto/review

## ブログ記事のMarkdown化

### ブログページの取得

	$ php get-blog.php <URL> html <章>

	例:
	$ php get-blog.php http://d.hatena.ne.jp/Kenji_s/20121201/fuelphp_this_year html 01

### Markdown化

	$ php get-blog.php <URL> md <章>

	例:
	$ php get-blog.php http://d.hatena.ne.jp/Kenji_s/20121201/fuelphp_this_year md 01

不要な部分は削除します。

### Markdownファイルの調整

	$ php get-blog.php <URL> adjust <章>

	例:
	$ php get-blog.php http://d.hatena.ne.jp/Kenji_s/20121201/fuelphp_this_year adjust 01

### 画像ファイルの取得

	$ php get-blog.php <URL> image <章>

	例:
	$ php get-blog.php http://d.hatena.ne.jp/Kenji_s/20121201/fuelphp_this_year image 01

## MarkdownファイルのReVIEW化

Markdownファイルの見出しレベルが正しくない場合は修正します。

ReVIEWの設定ファイルはあらかじめ用意してください。

	$ cd review
	$ ./make-pdf.sh  ... mdをreに変換しPDFを作成します
	$ ./make-epub.sh ... 既存reファイルからEPUBを作成します

横幅が紙面より小さい画像がPDFで拡大されてしまう現象へ対処するには以下のコマンドを実行します。

	$ ./convert-image.sh <dir>

	例:
	$ ./convert-image.sh images/01

## Markdownについて

### tableについて

PHP Markdown Extraの形式が使えますが、Pandocが出力するものは少々形式が違うため修正が必要になります。

- http://michelf.ca/projects/php-markdown/extra/#table
