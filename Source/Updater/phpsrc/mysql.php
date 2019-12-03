<?php
/*
 *	mysql.php
 *	Swift Auto-Updater
 *		MySQL Driver
 */

require_once("config.php");

$database = new PDO("mysql:host={$mysql_config['host']};dbname={$mysql_config['database']}", $mysql_config['username'], $mysql_config['password']);
$database->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
