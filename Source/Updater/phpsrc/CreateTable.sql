/*
 *	CreateTable.sql
 *	Swift Auto-Updater
 *		Create Auto-Updater Table
 */

CREATE TABLE REVISIONS (
	RevTag		CHAR(7)			NOT NULL,
	Package		VARCHAR(255)	NOT NULL,
	Timestamp	TIMESTAMP		NOT NULL,
	
	CONSTRAINT PRIMARY KEY (RevTag)
);
