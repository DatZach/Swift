<?php
/*
 *	push.php
 *	Swift Auto-Updater
 *		Push new revision to server.
 */

require_once('mysql.php');

$ALLOWED_EXTS = array('zip');
$PASSWORD_HASH = '55d70c649e2f1c10bfaac8928e473ed0b0f2b7f6';

// Get password
if (empty($_GET['pwd']))
	die('Bad password');

$password = $_GET['pwd'];
if (sha1($password) !== $PASSWORD_HASH)
	die('Bad password');

// Get revtag
if (empty($_GET['revtag']))
	die('Empty revtag');

$revTag = $_GET['revtag'];

// Upload file
$ext = end(explode('.', $_FILES['file']['name']));
if (!($_FILES['file']['type'] === 'application/zip' ||
	  $_FILES['file']['type'] === 'application/octet-stream') ||
	!in_array($ext, $ALLOWED_EXTS))
		die('Illegal upload.');

if ($_FILES['file']['error'] > 0)
	die('File error code: ' . $_FILES['file']['error']);

if (file_exists('packages/' . $_FILES['file']['name']))
	die('Remote package already exists: ' . $_FILES['file']['name']);

if (!move_uploaded_file($_FILES['file']['tmp_name'],
						'packages/' . $_FILES['file']['name']))
	die('Failed to upload package.');

// INSERT INTO REVISIONS (RevTag, Package) VALUES ('a1a1e77', 'a1a1e77.zip');
$statement = $database->prepare('INSERT INTO REVISIONS (RevTag, Package) VALUES(:RevTag, :Package);');
$statement->execute(array(
	':RevTag'	=> $revTag,
	':Package'	=> $_FILES['file']['name']
));

echo 'Successfully pushed revision ' . $revTag;
