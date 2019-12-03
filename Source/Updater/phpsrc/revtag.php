<?php
/*
 *	revtag.php
 *	Swift Auto-Updater
 *		Return current version's revision tag.
 */

require_once('mysql.php');

/*
	SELECT RevTag FROM REVISIONS
		ORDER BY Timestamp DESC
		LIMIT 1;
 */

$statement = $database->prepare('SELECT RevTag FROM REVISIONS ORDER BY Timestamp DESC LIMIT 1;');
$statement->execute();

echo $statement->fetch()['RevTag'];
