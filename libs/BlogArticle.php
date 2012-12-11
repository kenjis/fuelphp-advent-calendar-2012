<?php

/**
 * BlogArticle Class
 *
 * @author     Kenji Suzuki https://github.com/kenjis
 * @copyright  2012 Kenji Suzuki
 * @license    MIT License http://www.opensource.org/licenses/mit-license.php
 * @link       https://github.com/kenjis/fuelphp-advent-calendar-2012
 */
 class BlogArticle
{
    const MARKDOWN_DIR = 'markdown';
    const HTML_DIR     = 'html';

    protected $url;
    protected $chap;
    protected $html;
    protected $markdown;
    protected $blogType;

    public function __construct($url, $chap)
    {
        $this->url  = $url;
        $this->chap = $chap;
        
        $this->html_file     = static::HTML_DIR .     '/' . $chap . '.html';
        $this->markdown_file = static::MARKDOWN_DIR . '/' . $chap . '.md';
    }
    
    public function checkUrl()
    {
        if ( ! filter_var($this->url, FILTER_VALIDATE_URL)) {
            throw new RuntimeException('Invalid URL!');
        }
    }

    public function getContents()
    {
        $html = file_get_contents($this->url);
        //var_dump($http_response_header);

        $encoding = mb_detect_encoding($html, 'SJIS,EUC-JP,JIS,UTF-8,ASCII');
        
        if ($encoding !== 'UTF-8') {
            $html = mb_convert_encoding($html, 'UTF-8', $encoding);
        }

        file_put_contents($this->html_file, $html);
    }
    
    public function detectBlogType()
    {
        if ($this->blogType !== null) {
            return $this->blogType;
        }
        
        $url = parse_url($this->url);
        $host = $url['host'];

        if (preg_match('/\Ad\.hatena\.ne\.jp\z/', $host)) {
            $this->blogType = 'hatena diary';
        } elseif (preg_match('/\.blogspot\.jp\z/', $host)) {
            $this->blogType = 'blogspot';
        }

        return $this->blogType;
    }

    public function scrapeArticle($html)
    {
        $dom = str_get_html($html, true, true, DEFAULT_TARGET_CHARSET, false);

        $this->detectBlogType();
        
        if ($this->blogType === 'blogspot') {
            if ( ! is_null($dom->find('.post-outer', 0))) {
                $html = $dom->find('.post-outer', 0)->outertext;
            } else {
                throw new RuntimeException('Error: can\'t scrape blogspot article');
            }
        } elseif ($this->blogType === 'hatena diary') {
            if ( ! is_null($dom->find('.section', 0))) {
                $html = $dom->find('.section', 0)->outertext;
            } else {
                throw new RuntimeException('Error: can\'t scrape hatena diary article');
            }
        }
        // WordPress
        elseif ( ! is_null($dom->find('article', 0))) {
            $html = $dom->find('article', 0)->outertext;
        } elseif ( ! is_null($dom->find('.post', 0))) {
            $html = $dom->find('.post', 0)->outertext;
        }

        //var_dump($html); exit;
        return $html;
    }

    public function convertToMarkdown()
    {
        $html = file_get_contents($this->html_file);
        $html = $this->scrapeArticle($html);
        file_put_contents($this->html_file, $html);

        system('pandoc --no-wrap -f html -t markdown ' . $this->html_file . ' > ' . $this->markdown_file);
    }
    
    public function getImageFiles()
    {
        // backup
        copy($this->markdown_file, $this->markdown_file . '.orig');
        
        $markdown = file_get_contents($this->markdown_file);
        $lines = explode("\n", $markdown);
        $contents = '';
        
        $img_dir = static::MARKDOWN_DIR . '/images/' . $this->chap;
        if ( ! file_exists($img_dir)) {
            mkdir($img_dir);
        }
        
        foreach ($lines as $line) {
            // image line
            if (preg_match('/(.*)!\[(.*?)\]\((.+?)\)(.*)/u', $line)) {
                $img = $this->processImageLine($line);

                // get image file
                $img_file = basename($img['path']);
                $ext = $this->getFileExtension($img_file);
                // LaTex can't handle filename with dot, so replace by "_"
                $img_file = basename($img_file, '.' . $ext);
                $img_file = str_replace('.', '_', $img_file) . '.' . $ext;

                if (file_exists($img_dir . '/' . $img_file)) {
                    echo 'Error: ', $img_file . ' already exists', PHP_EOL;
                } else {
                    echo 'Getting Image: ', $img['path'], PHP_EOL;
                    $img_data = file_get_contents($img['path']);
                    file_put_contents($img_dir . '/' . $img_file, $img_data);
                }

                // get linked image file
                $ext = $this->getFileExtension($img['link']);
                if ($ext === 'png' || $ext === 'jpg' || $ext === 'gif') {
                    $img_file = basename($img['link']);  // override
                    $img_file = basename($img_file, '.' . $ext);
                    $img_file = str_replace('.', '_', $img_file) . '.' . $ext;

                    if (file_exists($img_dir . '/' . $img_file)) {
                        echo 'Error: ', $img_file . ' already exists', PHP_EOL;
                    } else {
                        echo 'Getting Linked Image: ', $img['link'], PHP_EOL;
                        $img_data = file_get_contents($img['link']);
                        file_put_contents($img_dir . '/' . $img_file, $img_data);
                    }

                    $img['before'] = '';
                    $img['after']  = '';
                }

                // change image path
                $img['path'] = 'images/' . $this->chap . '/' . $img_file;
                if ($img['title'] !== '') {
                    $img['path'] .= ' "' . $img['title'] . '"';
                }
                $line = $img['before'] . '![' . $img['alt'] . '](' . $img['path'] . ')' . $img['after'];

                //var_dump($line);
            }
            
            $contents .= $line . "\n";
        }
        
        //var_dump($contents);
        file_put_contents($this->markdown_file, $contents);
    }
    
    public function getFileExtension($file)
    {
        $path_parts = pathinfo($file);
        if ( ! isset($path_parts['extension']))
        {
            $path_parts['extension'] = '';
        }
        return strtolower($path_parts['extension']);
    }

    public function processImageLine($line)
    {
        // ![Alt text](/path/to/img.jpg)
        // ![Alt text](/path/to/img.jpg "Optional title")

        $title = '';
        $link  = '';

        if (preg_match('/(.*)!\[(.*?)\]\((.+?)\)(.*)/u', $line, $matches)) {
            //var_dump($matches); exit;
            $before = $matches[1];
            $alt    = $matches[2];
            $path   = $matches[3];
            $after  = $matches[4];

            if (strpos($path, ' ') !== false) {
                $paths = explode(' ', $path, 2);
                
                $path  = $paths[0];
                $title = trim($paths[1], '"\'');
            }

            // has link to image file?
            if ($before === '[')
            {
                // ](/path/to/img.jpg "Optional title")

                if (preg_match('/\]\((.+?)\)(.*)/u', $after, $matches)) {
                    $link = $matches[1];

                    if (strpos($link, ' ') !== false) {
                        $paths = explode(' ', $link, 2);
                        
                        $link  = $paths[0];
                    }
                } else {
                    throw new RuntimeException('image link format error: ' . $line);
                }
            }
        } else {
            throw new RuntimeException('image format error: ' . $line);
        }
        
        // convert relative URL to absolute URL
        if ( ! filter_var($path, FILTER_VALIDATE_URL)) {
            $parts = parse_url($this->url);
            
            if (substr($path, 0, 1) === '/') {
                // absolute path
                $path = $parts['scheme'] . '://' . $parts['host'] . $path;
            } elseif (substr($path, 0, 1) === '.') {
                // relative path
                $path = $parts['scheme'] . '://' . $parts['host'] . '/' . $parts['path'] . '/' . $path;
            } else {
                // relative path
                $path = $parts['scheme'] . '://' . $parts['host'] . '/' . $parts['path'] . '/../' . $path;
            }
        }
        
        $img = array(
            'before' => $before,
            'alt'    => $alt,
            'path'   => $path,
            'link'   => $link,
            'title'  => $title,
            'after'  => $after,
        );
        
        return $img;
    }


    public function adjustMarkdown()
    {
        $markdown = file_get_contents($this->markdown_file);
        $lines = explode("\n", $markdown);
        $contents = '';

        $this->detectBlogType();
        
        foreach ($lines as $line) {
            $line = $this->replaceNoBreakSpace($line);
            $line = $this->convertBackslash($line);
            $line = $this->removeDoubleWidthSpace($line);
            $line = $this->convertHankakuKana($line);
            $line = $this->convertZenkakuParentheses($line);
            $line = $this->convertHankakuKanaDot($line);
            $line = $this->removeBackslash($line);

            if ($this->blogType === 'hatena diary') {
                $line = $this->removeHatenaKeywordLink($line);
            }
            
            $contents .= $line . "\n";
        }
        
        //var_dump($contents);
        file_put_contents($this->markdown_file, $contents);
    }

    public function replaceNoBreakSpace($line)
    {
        if (preg_match('/' . chr(0xC2) . chr(0xA0) . '/u', $line, $matches)) {
            echo 'Replace No Break Space: ', $line, PHP_EOL;
            $line = str_replace(chr(0xC2) . chr(0xA0), ' ', $line);
        }
        return $line;
    }
    
    public function convertBackslash($line)
    {
        if (mb_substr($line, -1) === '\\')
        {
            echo 'Convert Backslash to <br>: ', $line, PHP_EOL;
            $line = mb_substr($line, 0, -1) . '  ';
        }
        return $line;
    }
    
    public function removeDoubleWidthSpace($line)
    {
        if (mb_substr($line, 0, 1) === '　')
        {
            echo 'Remove Double Width Space: ', mb_substr($line, 0, 10), PHP_EOL;
            $line = mb_substr($line, 1);
        }
        return $line;
    }
    
    public function removeHatenaKeywordLink($line)
    {
        $newline = '';
        
        while (preg_match('/(.*?)\[(.+?)\]\((.+?)\)(.*)/u', $line, $matches))
        {
            //var_dump($matches);
            $before  = $matches[1];
            $keyword = $matches[2];
            $url     = $matches[3];
            $after   = $matches[4];
            
            if (substr($url, 0, 30) === 'http://d.hatena.ne.jp/keyword/') {
                echo 'Remove Hatena Keyword Link: ', $keyword, ' ', $url, PHP_EOL;
                $newline .= $before . $keyword;
                $line = $after;
            } else {
                $newline .= $before . '[' . $keyword . '](' . $url . ')';
                $line = $after;
            }
        }
        $newline .= $line;
        
        return $newline;
    }

    public function convertHankakuKana($line)
    {
        $newline = mb_convert_kana($line, 'KV');
        if ($line !== $newline) {
            echo 'Convert Hankaku Kana: ', $line, PHP_EOL;
        }
        return $newline;
    }
    
    public function convertZenkakuParentheses($line)
    {
        if (preg_match('/（|）/u', $line, $matches)) {
            echo 'Convert Zenkaku Parentheses: ', $line, PHP_EOL;
            $line = str_replace(array('（', '）'), array('(', ')'), $line);
        }
        return $line;
    }

    public function convertHankakuKanaDot($line)
    {
        if (preg_match('/･･･/u', $line, $matches)) {
            echo 'Convert Hankaku Kana Dot: ', $line, PHP_EOL;
            $line = $line = str_replace('･･･', '…', $line);
        }
        return $line;
    }

    public function removeBackslash($line)
    {
        // if there is '\$', remove '\'
        if (preg_match('/\\\\\$/u', $line, $matches)) {
            echo 'Remove Backslash: ', $line, PHP_EOL;
            $line = str_replace('\\$', '$', $line);
        }
        return $line;
    }
}

