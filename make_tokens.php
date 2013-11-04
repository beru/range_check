<?php

// Tokenのリストを生成するプログラム

if ($argc != 3) {
	printf("usage : %s filename prefix\n", $argv[0]);
	exit;
}

$prefix = $argv[2];	// enum class name
$lines = file($argv[1], FILE_SKIP_EMPTY_LINES);

foreach ($lines as $line) {
	$cols = str_getcsv($line);
	$name = $cols[0];
	$str = strlen($cols[1]) ? $cols[1] : $cols[0];
//	printf("\tD(%s) \\\r\n", $name);	// enum entry
	printf("\"%s\"\t{ return %s::%s; }\r\n", $str, $prefix, $name);	// re2c
}


?>
