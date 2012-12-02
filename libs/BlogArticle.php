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
    
    public function __construct($url, $chap)
    {
        $this->url  = $url;
        $this->chap = $chap;
        
        $this->html_file     = static::HTML_DIR .     '/' . $chap . '.html';
        $this->markdown_file = static::MARKDOWN_DIR . '/' . $chap . '.md';
    }
    
    public function getMarkdown()
    {
        return $this->markdown_file;
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
    
    public function convertToMarkdown()
    {
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
                echo 'Getting: ', $img['path'], PHP_EOL;
                $img_data = file_get_contents($img['path']);
                
                // save image file
                $img_file = basename($img['path']);
                if (file_exists($img_dir . '/' . $img_file)) {
                    echo 'Error: ', $img_file . ' already exists', PHP_EOL;
                } else {
                    file_put_contents($img_dir . '/' . $img_file, $img_data);
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
    
    public function processImageLine($line)
    {
        // ![Alt text](/path/to/img.jpg)
        // ![Alt text](/path/to/img.jpg "Optional title")

        $title = '';
        
        if (preg_match('/(.*)!\[(.*?)\]\((.+?)\)(.*)/u', $line, $matches)) {
            //var_dump($matches); exit;            
            $alt  = $matches[2];
            $path = $matches[3];
            
            if (strpos($path, ' ') !== false) {
                $paths = explode(' ', $path, 2);
                
                $path  = $paths[0];
                $title = trim($paths[1], '"\'');
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
            'before' => $matches[1],
            'alt'    => $alt,
            'path'   => $path,
            'title'  => $title,
            'after'  => $matches[4],
        );
        
        return $img;
    }
}

