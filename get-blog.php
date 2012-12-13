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
require './vendor/autoload.php';

mb_internal_encoding('UTF-8');

if ( ! isset($argv[1]) || ! isset($argv[2]) || ! isset($argv[3])) {
    echo $argv[0] . ' <URL> html <chapter>' . PHP_EOL;
    echo $argv[0] . ' <URL> md <chapter>' . PHP_EOL;
    echo $argv[0] . ' <URL> adjust <chapter>' . PHP_EOL;
    echo $argv[0] . ' <URL> image <chapter>' . PHP_EOL;
    exit(1);
}

$url  = $argv[1];
$cmd  = $argv[2];
$chap = $argv[3];

$article = new BlogArticle($url, $chap);

if ($cmd === 'html') {
    $article->checkUrl();
    $article->getContents();
} elseif ($cmd === 'md') {
    $article->convertToMarkdown();
} elseif ($cmd === 'image') {
    $article->checkUrl();
    $article->getImageFiles();
} elseif ($cmd === 'adjust') {
    $article->adjustMarkdown();
}

