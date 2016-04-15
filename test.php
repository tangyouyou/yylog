<?php
//echo yylog_version();
//echo yylog_author();
$yylog = new YYlog();
$yylog->setBasePath('/log/test');
$yylog->setLogger('youyou');
//echo $yylog->getBasePath().PHP_EOL;
//echo $yylog->getLogger().PHP_EOL;
$yylog->error('this is a test');
//var_dump($yylog);
