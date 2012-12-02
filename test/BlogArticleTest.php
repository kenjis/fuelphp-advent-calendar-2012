<?php

class BlogArticleTest extends PHPUnit_Framework_TestCase
{
    public function setUp()
    {
        $this->blog = new BlogArticle('dummy', '01');
    }
    
    public function test_processImageLine()
    {
        $line = '[![f:id:Kenji\_s:20121109102611p:image](http://cdn-ak.f.st-hatena.com/images/fotolife/K/Kenji_s/20121109/20121109102611.png "f:id:Kenji_s:20121109102611p:image")](http://f.hatena.ne.jp/Kenji_s/20121109102611)';
        $test = $this->blog->processImageLine($line);
        $expected = array(
            'before' => '[',
            'alt'   => 'f:id:Kenji\_s:20121109102611p:image',
            'path'  => 'http://cdn-ak.f.st-hatena.com/images/fotolife/K/Kenji_s/20121109/20121109102611.png',
            'title' => 'f:id:Kenji_s:20121109102611p:image',
            'after' => '](http://f.hatena.ne.jp/Kenji_s/20121109102611)',
        );
        $this->assertEquals($expected, $test);
    }
    
    public function test_processImageLine_without_alt()
    {
        $line = '[![](http://ounziw.com/wp-content/uploads/2011/12/fork-300x235.png "fork")](http://ounziw.com/wp-content/uploads/2011/12/fork.png)';
        $test = $this->blog->processImageLine($line);
        $expected = array(
            'before' => '[',
            'alt'   => '',
            'path'  => 'http://ounziw.com/wp-content/uploads/2011/12/fork-300x235.png',
            'title' => 'fork',
            'after' => '](http://ounziw.com/wp-content/uploads/2011/12/fork.png)',
        );
        $this->assertEquals($expected, $test);
    }
    
    public function test_processImageLine_without_alt_and_title()
    {
        $line = '[![](http://1.bp.blogspot.com/-WFb7gLeUcmg/TtwVe_TvNlI/AAAAAAAAAvM/WvZsJuhQPYA/s320/1-1.jpg)](http://1.bp.blogspot.com/-WFb7gLeUcmg/TtwVe_TvNlI/AAAAAAAAAvM/WvZsJuhQPYA/s1600/1-1.jpg)
';
        $test = $this->blog->processImageLine($line);
        $expected = array(
            'before' => '[',
            'alt'   => '',
            'path'  => 'http://1.bp.blogspot.com/-WFb7gLeUcmg/TtwVe_TvNlI/AAAAAAAAAvM/WvZsJuhQPYA/s320/1-1.jpg',
            'title' => '',
            'after' => '](http://1.bp.blogspot.com/-WFb7gLeUcmg/TtwVe_TvNlI/AAAAAAAAAvM/WvZsJuhQPYA/s1600/1-1.jpg)',
        );
        $this->assertEquals($expected, $test);
    }

    public function test_replaceNoBreakSpace()
    {
        $line = '    private static $exts = array(); // nbsp';  // nbsp (C2 A0)
        $test = $this->blog->replaceNoBreakSpace($line);
        $expected = '    private static $exts = array(); // nbsp';
        $this->assertEquals($expected, $test);
    }

    public function test_replaceNoBreakSpace_with_no_nbsp()
    {
        $line = '    private static $exts = array();';  // space
        $test = $this->blog->replaceNoBreakSpace($line);
        $expected = '    private static $exts = array();';
        $this->assertEquals($expected, $test);
    }
    
    public function test_scrapeArticle()
    {
        $html = file_get_contents(TESTPATH . 'fixture/01.html');
        $test = $this->blog->scrapeArticle($html);
        $expected = file_get_contents(TESTPATH . 'fixture/01.html.scraped');
        $this->assertEquals($expected, $test);
    }
    
    public function test_removeBackSlash()
    {
        $line = '\\';
        $test = $this->blog->removeBackSlash($line);
        $expected = '';
        $this->assertEquals($expected, $test);
    }
    
        
    public function test_removeDoubleWidthSpace()
    {
        $line = '　とはいえ';
        $test = $this->blog->removeDoubleWidthSpace($line);
        $expected = 'とはいえ';
        $this->assertEquals($expected, $test);
    }
}

