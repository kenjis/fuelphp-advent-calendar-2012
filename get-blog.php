<?php

/**
 * get-blog command
 *
 * @author     Kenji Suzuki https://github.com/kenjis
 * @copyright  2012 Kenji Suzuki
 * @license    MIT License http://www.opensource.org/licenses/mit-license.php
 * @link       https://github.com/kenjis/fuelphp-advent-calendar-2012
 */

require './libs/BlogArticle.php';

if ( ! isset($argv[1]) || ! isset($argv[2]) || ! isset($argv[3])) {
    echo $argv[0] . ' (URL) page (chapter)' . PHP_EOL;
    echo $argv[0] . ' (URL) image (chapter)' . PHP_EOL;
    exit(1);
}

$url  = $argv[1];
$cmd  = $argv[2];
$chap = $argv[3];

if ( ! filter_var($url, FILTER_VALIDATE_URL)) {
    echo 'Invalid URL!' . PHP_EOL;
    exit(1);
}

$article = new BlogArticle($url, $chap);

if ($cmd === 'page') {
    $article->getContents();
    $article->convertToMarkdown();
} elseif ($cmd === 'image') {
    $article->getImageFiles();
}

