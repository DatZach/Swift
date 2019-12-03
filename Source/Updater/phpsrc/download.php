<?php
/*
 *	download.php
 *	Swift Auto-Updater
 *		Return a revtag's package.
 */

ob_start();

require_once('mysql.php');

if (empty($_GET['revtag']))
{
	ob_end_flush();
	die();
}

$revTag = $_GET['revtag'];

/*
	SELECT Package FROM REVISIONS WHERE RevTag = '@tag';
 */

$statement = $database->prepare('SELECT Package FROM REVISIONS WHERE RevTag = :RevTag;');
$statement->execute(array(':RevTag' => $revTag));
if ($statement->rowCount() === 0)
{
	ob_end_flush();
	die();
}

$package = $statement->fetch()['Package'];
header('Location: packages/' . $package);
ob_end_flush();
