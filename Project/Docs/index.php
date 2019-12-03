<?php
/*
 *	index.php
 *	Swift Docs
 *
 *	This document provides the frontend for interfacing with
 *	Swift Docs.
 */

// Includes
require_once('resources/Parsedown.php');

function is_sub_dir($path = null, $parent_folder = '.')
{
    //Get directory path minus last folder
    $dir = dirname($path);
    $folder = substr($path, strlen($dir));

    //Check the the base dir is valid
    $dir = realpath($dir);

    //Only allow valid filename characters
    $folder = preg_replace('/[^a-z0-9\.\-_]/i', '', $folder);

    //If this is a bad path or a bad end folder name
    if( !$dir || !$folder || $folder === '.')
    	return false;

    //Rebuild path
    $path = $dir . '/' . $folder;

    //If this path is higher than the parent folder
    return (strcasecmp($path, $parent_folder) > 0)
			? $path
			: false;
}

// Grab our arguments
$path = $_GET['path'];

$parsedown = new Parsedown();
$content = 'Something went wrong...';
$title = 'Index';

if (empty($path))
	$content = file_get_contents('./index.md');
else if (is_sub_dir($path) !== false && pathinfo($path, PATHINFO_EXTENSION) === 'md')
{
	if (!file_exists($path))
		$path = './index.md';
	
	$content = file_get_contents($path);
	$title = str_replace('.md', '', end(explode('/', $path)));
}
else if (path !== false)
	$content = file_get_contents('./index.md');

?>
<!DOCTYPE html>
<html>
	<head>
		<title><?php echo $title; ?> - Swift Docs</title>
		<link type="text/css" href="resources/styles/markdown.css" rel="stylesheet" media="screen">
		<link type="text/css" href="resources/styles/main.css" rel="stylesheet" media="screen">
		<script src="https://google-code-prettify.googlecode.com/svn/loader/run_prettify.js"></script>
	</head>
	<body>
		<div class="nav-container">
			<div style="text-align: center; padding: 5%">
				<a href="http://www.x2048.com/swift/docs/">
					<img src="resources/images/logo.png" alt="" style="width: 200px; 207px;">
				</a>
			</div>
			<form>
				<input type="search" placeholder="Search Swift Docs" name="searchValue">
			</form>

			<div class="copyright">
				<p>&copy; 2014 x2048 - Swift Docs</p>
			</div>
		</div>
		<div class="markdown">
			<?php echo $parsedown->parse($content); ?>
		</div>
	</body>
</html>